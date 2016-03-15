/*
 * CUserStatsTrackerDaemon.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: dstoll
 */

/*
 ============================================================================
 Name		: UserStatsTrackerDaemon.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CUserStatsTrackerDaemon implementation
 ============================================================================
 */
#include "includes/core/utility/CUserStatsTrackerDaemon.h"
#include "includes/core/utility/IPC.h"
#include "includes/core/utility/CSystem.h"
#include "includes/GlobalDefines.h"

#define KUserStatsFile "UserStatsFile.ini"
#define MAXIMUM_STATS_ENTRIES_TO_BE_READ_FROM_DISK 800
#define KWhiteSpaceCharacter " "  //needed when we construct the message
#define KDateSeperatorCharacter "-" //needed when we construct the message
#define KTimeSeperatorCharacter ":" //needed when we construct the message
#define KGameFullVersion "Full"
#define KGameTrialVersion "Trial2"
#define KMessageEnd "SOS"

void* DaemonThreadMethod(void* aDaemonObject)
{
	CUserStatsTrackerDaemon* lUserStatsTrackerDeamon = (CUserStatsTrackerDaemon*) aDaemonObject;

	//need to get PhoneId And OS might block for some time depending on OS
	lUserStatsTrackerDeamon->iPhoneSerialNumber = CSystem::GetPhoneSerialNumberNative();
	lUserStatsTrackerDeamon->iPhoneOSVersion = CSystem::GetPhoneOSVersionNative();
	//all done, ready to upload stats
	lUserStatsTrackerDeamon->iUserStatsTrackerState = EUserStatsTrackerStateRunning;

	while (true)
	{
		if (lUserStatsTrackerDeamon->iUserStatsTrackerState == EUserStatsTrackerStateRunning)
		{
			//send data to server
			lUserStatsTrackerDeamon->SendUserStatsIfPossible();

			//go back to sleep
			CSystem::SleepForMicroSecondsNative((TInt64) lUserStatsTrackerDeamon->iSecsIntervalForSending * 1000000);
		}
		else
		{
			return NULL;
		}
	}

	return NULL;
}

CUserStatsTrackerDaemon::CUserStatsTrackerDaemon(TUint aSecsUploadStatsInterval)
{
	iSecsIntervalForSending = aSecsUploadStatsInterval;
	iUserStatsTrackerState = EUserStatsTrackerStateInitializing;
	iUploadWithoutWLan = false;
	iAgreedToUploadStats = false;
}

CUserStatsTrackerDaemon::~CUserStatsTrackerDaemon()
{
	//write data that we were not able to send to disk
	SaveUserStatsToDisk();
	RemoveUserStatsEntries();

	if(iTCPConnection != NULL)
	{
		delete iTCPConnection;
		iTCPConnection = NULL;
	}
	if(iUserStatsEntries != NULL)
	{
		delete iUserStatsEntries;
		iUserStatsEntries = NULL;
	}
	if(iPhoneSerialNumber != NULL)
	{
		delete iPhoneSerialNumber;
		iPhoneSerialNumber = NULL;
	}
}

CUserStatsTrackerDaemon* CUserStatsTrackerDaemon::New(TUint aSecsUploadStatsInterval)
{
	CUserStatsTrackerDaemon* self = new CUserStatsTrackerDaemon(aSecsUploadStatsInterval);
	self->Construct();
	return self;
}

void CUserStatsTrackerDaemon::Construct()
{
	iUserStatsEntries = CPointerArray<SUserStatsEntry>::New();
	iTCPConnection = CTCPConnectionEngine::New(GSTATS_TRACKER_SERVER_IP, GSTATS_TRACKER_SERVER_PORT, GSTATS_TRACKER_SERVER_CONNECTION_TIMEOUT);

	//create Deamon Thread
	CNativeThread::CreateNativeThread(DaemonThreadMethod, this);
}

// ---------------------- functions --------------------------- //

void CUserStatsTrackerDaemon::AddValueToUserStatsEntry(SUserStatsEntry* aEntry, const char* aValueName, const char* aValueAsString)
{
	SUserStatsValue lNewValue;
	lNewValue.ValueName = new CString(aValueName);
	lNewValue.Value = new CString(aValueAsString);

	aEntry->ValueEntries->Append(new SUserStatsValue(lNewValue));
}

void CUserStatsTrackerDaemon::AddValueToUserStatsEntry(SUserStatsEntry* aEntry, const char* aValueName, TInt aValueAsInt)
{
	SUserStatsValue lNewValue;
	lNewValue.ValueName = new CString(aValueName);
	lNewValue.Value = (new CString(""))->AppendNum(aValueAsInt);

	aEntry->ValueEntries->Append(new SUserStatsValue(lNewValue));
}

//adds the previously created entry to queue for sending
void CUserStatsTrackerDaemon::AddUserStatsEntry(SUserStatsEntry* aEntry)
{
	//add new entry to deamon
	iUserStatsEntries->Append(aEntry);
}

SUserStatsEntry* CUserStatsTrackerDaemon::CreateUserStatsEntry(const char* aNameOfUserStat)
{
	SUserStatsEntry* lNewUserStatsEntry = new SUserStatsEntry;
	lNewUserStatsEntry->ValueEntries = CPointerArray<SUserStatsValue>::New(); //need allocate a dynamic array
	lNewUserStatsEntry->UserStatsName = new CString(aNameOfUserStat);
	lNewUserStatsEntry->Date = CSystem::GetDateNative();
	lNewUserStatsEntry->Time = CSystem::GetTimeNative();

	//return to caller
	return lNewUserStatsEntry;
}

const CString* CUserStatsTrackerDaemon::GetPhoneSerialNumber()
{
	return iPhoneSerialNumber;
}

void CUserStatsTrackerDaemon::SetAgreedToUploadStats(TBool aValue)
{
	iAgreedToUploadStats = aValue;
}

void CUserStatsTrackerDaemon::SetUploadUserStatsWithoutWLan(TBool aValue)
{
	iUploadWithoutWLan = aValue;
	iTCPConnection->SetIfWLanNotNeededToConnect(iUploadWithoutWLan);
}

void CUserStatsTrackerDaemon::SendUserStatsIfPossible()
{
	if(iAgreedToUploadStats && (iUploadWithoutWLan || CSystem::GetIsConnectedToWLanNative()) && iUserStatsEntries->GetCount() > 0)
	{
		//send old data first if any
		SendUserStatsFromDiskToServer();

		//add messages to the RemoteConnectionBuffer
		for(TInt lIndex = 0; lIndex < iUserStatsEntries->GetCount(); lIndex++)
		{
			SRemoteMessageInternalFormat lInternalMessage = ConvertUserStatsEntryToInternalMessageFormat(*iUserStatsEntries->Get(lIndex));
			iTCPConnection->AddMessage(lInternalMessage);
		}

		//all messages have been added, start sending
		if(iTCPConnection->SendMessages(KMessageEnd) != ETCPConnectionEngineSendStatusSuccess)
		{
			//something went wrong, probably server, down
			SaveUserStatsToDisk();
		}
		//we assume this data reached the server, thus we can delete the messages in our buffer or it is saved to disk
		RemoveUserStatsEntries();
	}
	else
	{
		SaveUserStatsToDisk(); //user didn't agree, save to disk for later use then
		RemoveUserStatsEntries(); //don't want to send it twice
	}
}

void CUserStatsTrackerDaemon::SetConnectionTimeout(TInt aConnectionTimeoutInSecs)
{
	iTCPConnection->SetConnectionTimeout(aConnectionTimeoutInSecs);
}

void CUserStatsTrackerDaemon::RemoveUserStatsEntries()
{
	//reset and destroy dynamic array in each entry
	for(TInt lIndex = 0; lIndex < iUserStatsEntries->GetCount(); lIndex++)
	{
		delete iUserStatsEntries->Get(lIndex)->Date;
		delete iUserStatsEntries->Get(lIndex)->Time;
		delete iUserStatsEntries->Get(lIndex)->UserStatsName;
		CPointerArray<SUserStatsValue>* lUserStatsValues = iUserStatsEntries->Get(lIndex)->ValueEntries;

		for(TInt lInnerIndex = 0; lInnerIndex < lUserStatsValues->GetCount(); lInnerIndex++)
		{
			delete lUserStatsValues->Get(lInnerIndex)->Value;
			delete lUserStatsValues->Get(lInnerIndex)->ValueName;
		}
		iUserStatsEntries->Get(lIndex)->ValueEntries->ClearAndDestroy();
		delete lUserStatsValues; //need to remove pointer array as well
	}

	//remove and delete all entries
	iUserStatsEntries->ClearAndDestroy();
}

SRemoteMessageInternalFormat CUserStatsTrackerDaemon::ConvertUserStatsEntryToInternalMessageFormat(SUserStatsEntry& aEntry)
{
	SRemoteMessageInternalFormat lInternalMessage;
	lInternalMessage.MessageContent = new CString("");

	//first thing should be the name of the User Stat
	lInternalMessage.MessageContent->Append(aEntry.UserStatsName);
	//add UID
	lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
	lInternalMessage.MessageContent->Append(iPhoneSerialNumber);
	//OS Version
	lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
	lInternalMessage.MessageContent->Append(iPhoneOSVersion);
	//Game Version
#if GGAME_FULL_VERSION
	lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
	lInternalMessage.MessageContent->Append(KGameFullVersion);
#else
	lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
	lInternalMessage.MessageContent->Append(KGameTrialVersion);
#endif
	//add date
	lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
	lInternalMessage.MessageContent->Append(aEntry.Date);
	//add time
	lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
	lInternalMessage.MessageContent->Append(aEntry.Time);

	//now add all the values needed for this user stat
	for(TInt lIndex = 0; lIndex < aEntry.ValueEntries->GetCount(); lIndex++)
	{
		//seperate from previous value
		lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
		//add value name
		lInternalMessage.MessageContent->Append((*aEntry.ValueEntries).Get(lIndex)->ValueName);
		//seperate
		lInternalMessage.MessageContent->Append(KWhiteSpaceCharacter);
		//add value
		lInternalMessage.MessageContent->Append((*aEntry.ValueEntries).Get(lIndex)->Value);
	}

	return lInternalMessage;
}

SRemoteMessageInternalFormat CUserStatsTrackerDaemon::ReadUserStatsFromDisk(CFileReadStream* aReadStream)
{
	SRemoteMessageInternalFormat lSavedMessage;
	lSavedMessage.MessageContent = NULL;

	aReadStream->ReadStringEncrypted(lSavedMessage.MessageContent, "UserStatReadingFromDiskFailed");
	return lSavedMessage;
}

void CUserStatsTrackerDaemon::SaveUserStatToDisk(SRemoteMessageInternalFormat& aEntry, CFileWriteStream* aWriteStream)
{
	aWriteStream->WriteStringEncrypted(aEntry.MessageContent);
}

void CUserStatsTrackerDaemon::SaveUserStatsToDisk()
{
	CNativeFile* lSaveFile = new CNativeFile(KUserStatsFile);
	CFileWriteStream* lWriteStream = lSaveFile->OpenForWriting(EFileWriteStreamModeAppend);

	if(lWriteStream != NULL) //no problems
	{
		//write all user stats entries to disk
		for(TInt lIndex = 0; lIndex < iUserStatsEntries->GetCount(); lIndex++)
		{
			SRemoteMessageInternalFormat lInternalMessage = ConvertUserStatsEntryToInternalMessageFormat(*iUserStatsEntries->Get(lIndex));
			SaveUserStatToDisk(lInternalMessage, lWriteStream);
			//clean up
			delete lInternalMessage.MessageContent;
		}

		lWriteStream->CloseWriteStream();
		delete lWriteStream;
	}
}

void CUserStatsTrackerDaemon::SendUserStatsFromDiskToServer()
{
	CNativeFile* lSaveFile = new CNativeFile(KUserStatsFile);
	CFileReadStream* lReadStream = lSaveFile->OpenForReading();

	if(lReadStream != NULL)
	{
		//read values from file into temp values and if no errors occurs continue otherwise delete file and use default Values
		TInt lCounter = 0;
		while(true)
		{
			lCounter++;
			SRemoteMessageInternalFormat lMessage = ReadUserStatsFromDisk(lReadStream);

			if(lMessage.MessageContent->EqualTo("") || lCounter > MAXIMUM_STATS_ENTRIES_TO_BE_READ_FROM_DISK)
			{
				break;//all done
			}
			iTCPConnection->AddMessage(lMessage);
		}

		//all done, send messages to server
		TTCPConnectionEngineSendStatus lSendStatus = iTCPConnection->SendMessages(KMessageEnd);

		lReadStream->CloseReadStream();
		delete lReadStream;
		if(lSendStatus == ETCPConnectionEngineSendStatusSuccess) //data has been send, need to delete it otherwise we send duplicate data
			lSaveFile->DeleteFile();
	}

	delete lSaveFile;
}
