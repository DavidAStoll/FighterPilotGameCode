/*
 * CUserStatsTrackerDaemon.h
 *
 *  Created on: Oct 20, 2012
 *      Author: dstoll
 */

#ifndef CUSERSTATSTRACKERDAEMON_H_
#define CUSERSTATSTRACKERDAEMON_H_

//INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"
#include "includes/core/utility/CTCPConnectionEngine.h"

struct SUserStatsValue
{
	CString* ValueName;
	CString* Value;
};

struct SUserStatsEntry
{
	CString* UserStatsName;
	CString* Time;
	CString* Date;
	CPointerArray<SUserStatsValue>* ValueEntries;
};

enum TUserStatsTrackerState
{
	EUserStatsTrackerStateInitializing,
	EUserStatsTrackerStateRunning,
	EUserStatsTrackerStateShuttingDown
};

class CUserStatsTrackerDaemon
	{

public:

	virtual ~CUserStatsTrackerDaemon();
	static CUserStatsTrackerDaemon* New(TUint aSecsUploadStatsInterval);

	//adds a value to a User Stats Entry, where the value is given has a String
	void AddValueToUserStatsEntry(SUserStatsEntry* aEntry, const char* aValueName, const char* aValueAsString);
	//adds a value to a User Stats Entry, where the value is given has an TInt
	void AddValueToUserStatsEntry(SUserStatsEntry* aEntry, const char* aValueName, TInt aValueAsInt);
	//adds the previously created entry to queue for sending
	void AddUserStatsEntry(SUserStatsEntry* aEntry);
	//creates a User Stats Entry and return a reference to it, this entry can then be used to add more data to it
	//once it it is ready it needs to be added to the stats Tracker
	SUserStatsEntry* CreateUserStatsEntry(const char* aNameOfUserStat);
	//return PhoneId String
	const CString* GetPhoneSerialNumber();
	//changes if deamon is allowed to upload stats, stats will still be recorded and saved to disk
	void SetAgreedToUploadStats(TBool aValue);
	//changes permission if it can always upload the data through the network
	void SetUploadUserStatsWithoutWLan(TBool aValue);
	//checks if it can make internet connection with the server and sends the data
	void SendUserStatsIfPossible();
	//set connection timeout for TCP connection
	void SetConnectionTimeout(TInt aConnectionTimeoutInSecs);
	//deletes all the User Stats entries cached in memory
	void RemoveUserStatsEntries();

private:

	CUserStatsTrackerDaemon(TUint aSecsUploadStatsInterval);
	void Construct();

	//adds default data to the message that any Stats entry needs
	SRemoteMessageInternalFormat ConvertUserStatsEntryToInternalMessageFormat(SUserStatsEntry& aEntry);
	//reads the previous stores content of previous store message on disk
	SRemoteMessageInternalFormat ReadUserStatsFromDisk(CFileReadStream* aReadStream);
	//stores the content of the message on disk
	void SaveUserStatToDisk(SRemoteMessageInternalFormat& aEntry, CFileWriteStream* aWriteStream);
	//writes all user stats entries that have not been send to the server to disk
	void SaveUserStatsToDisk();
	//reads all previous entries saved to disk and sends it through the network to the server
	void SendUserStatsFromDiskToServer();
	//contains the logic that will run in the background in its own Threads. Will prediocially try to upload stats to the server
	friend void* DaemonThreadMethod(void* aDaemonObject);

	CTCPConnectionEngine* iTCPConnection;
	TInt iSecsIntervalForSending;
	CPointerArray<SUserStatsEntry>* iUserStatsEntries;
	TUserStatsTrackerState iUserStatsTrackerState;
	TBool iAgreedToUploadStats;
	TBool iUploadWithoutWLan;
	CString* iPhoneSerialNumber;	//needed to get Phone IMEI
	CString* iPhoneOSVersion;

	};

#endif /* CUSERSTATSTRACKERDAEMON_H_ */
