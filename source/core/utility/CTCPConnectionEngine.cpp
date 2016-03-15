/*
 * CTCPConnectionEngine.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: dstoll
 */

#include "includes/core/utility/CTCPConnectionEngine.h"
#include "includes/core/utility/CSystem.h"

#define KMessageDelimiter "|"
#define KMessageCharacterError "?"

//capital letters
#define KCapitalA "A"
#define KCapitalB "B"
#define KCapitalC "C"
#define KCapitalD "D"
#define KCapitalE "E"
#define KCapitalF "F"
#define KCapitalG "G"
#define KCapitalH "H"
#define KCapitalI "I"
#define KCapitalJ "J"
#define KCapitalK "K"
#define KCapitalL "L"
#define KCapitalM "M"
#define KCapitalN "N"
#define KCapitalO "O"
#define KCapitalP "P"
#define KCapitalQ "Q"
#define KCapitalR "R"
#define KCapitalS "S"
#define KCapitalT "T"
#define KCapitalU "U"
#define KCapitalV "V"
#define KCapitalW "W"
#define KCapitalX "X"
#define KCapitalY "Y"
#define KCapitalZ "Z"
//lower Letters
#define KLowerA "a"
#define KLowerB "b"
#define KLowerC "c"
#define KLowerD "d"
#define KLowerE "e"
#define KLowerF "f"
#define KLowerG "g"
#define KLowerH "h"
#define KLowerI "i"
#define KLowerJ "j"
#define KLowerK "k"
#define KLowerL "l"
#define KLowerM "m"
#define KLowerN "n"
#define KLowerO "o"
#define KLowerP "p"
#define KLowerQ "q"
#define KLowerR "r"
#define KLowerS "s"
#define KLowerT "t"
#define KLowerU "u"
#define KLowerV "v"
#define KLowerW "w"
#define KLowerX "x"
#define KLowerY "y"
#define KLowerZ "z"
//Numbers
#define KNumber0 "0"
#define KNumber1 "1"
#define KNumber2 "2"
#define KNumber3 "3"
#define KNumber4 "4"
#define KNumber5 "5"
#define KNumber6 "6"
#define KNumber7 "7"
#define KNumber8 "8"
#define KNumber9 "9"
//special letters
#define KEndAndStartOfMessage "|"
#define KDot "."
#define KSemicolon ":"
#define KDash "-"
#define KWhitespace " "
#define KUnderscoreCharacter "_"

CTCPConnectionEngine::CTCPConnectionEngine(const char* aServerIpAddress, TInt aServerPort, TInt aConnectionTimeoutValueInSecs)
{
	iState = ETCPConnectionEngineStateIdle;
	iServerIpAddress = new CString(aServerIpAddress);
	iServerPort = aServerPort;
	iConnectionTimeoutInSecs = aConnectionTimeoutValueInSecs;
	iSendMessagesWithoutWLan = false;
}

CTCPConnectionEngine::~CTCPConnectionEngine()
{
	for(TInt lIndex = 0; lIndex < iCodePoints->GetCount(); lIndex++)
	{
		delete iCodePoints->Get(lIndex)->Character;
	}
	iCodePoints->ClearAndDestroy();

	for(TInt lIndex = 0; lIndex < iMessageQueue->GetCount(); lIndex++)
	{
		delete iMessageQueue->Get(lIndex)->MessageContent;
	}
	iMessageQueue->ClearAndDestroy();

	if(iServerIpAddress != NULL)
	{
		delete iServerIpAddress;
		iServerIpAddress = NULL;
	}
	if(iSocket != NULL)
	{
		iSocket->Close();
		delete iSocket;
	}
}

CTCPConnectionEngine* CTCPConnectionEngine::New(const char* aServerIpAddress, TInt aServerPort, TInt aConnectionTimeoutValueInSecs)
{
	CTCPConnectionEngine* self = new CTCPConnectionEngine(aServerIpAddress, aServerPort, aConnectionTimeoutValueInSecs);
	self->Construct();
	return self;
}

void CTCPConnectionEngine::Construct()
{
	//create socket for communication
	iSocket = CNativeSocket::New(iServerIpAddress->GetData(), iServerPort);

	//for the Encoder
	iCodePoints = CPointerArray<SCodePoint>::New();
	iMessageQueue = CPointerArray<SRemoteMessageInternalFormat>::New();
	CreateCodePoints();
}

//------------------------- functions ----------------------------------//

void CTCPConnectionEngine::SetConnectionTimeout(TInt aConnectionTimeoutValueInSecs)
{
	iConnectionTimeoutInSecs = aConnectionTimeoutValueInSecs;
}

void CTCPConnectionEngine::SetIfWLanNotNeededToConnect(TBool aValue)
{
	iSendMessagesWithoutWLan = aValue;
}

void CTCPConnectionEngine::AddMessage(const SRemoteMessageInternalFormat& aMessage)
{
	iMessageQueue->Append(new SRemoteMessageInternalFormat(aMessage));
}

TTCPConnectionEngineSendStatus CTCPConnectionEngine::HandshakeThenSendMessagesAndReceive(const char* aHandshakeMessage, const char* aHandshakePositiveReply, CPointerArray<SRemoteMessageInternalFormat>* aReceivedMessageBuffer, const char* aSendingDoneSignal, const char* aReceivingDoneSignal)
{
	if((iSendMessagesWithoutWLan || CSystem::GetIsConnectedToWLanNative()) && iMessageQueue->GetCount() > 0)
	{
		if(iState == ETCPConnectionEngineStateIdle)
		{
			//not busy with sending data already
			//run code to initiate sending loop of data, establish a connection to the server,
			iState = ETCPConnectionEngineStateConnecting;
			TBool lOpenSuccess = iSocket->Open(iConnectionTimeoutInSecs);//creates a blank socket for use for a specific protocol

			if(!lOpenSuccess)
			{
				//server must be down
				iState = ETCPConnectionEngineStateIdle;
				return ETCPConnectionEngineSendStatusFailedNoServer;
			}
		}

		while(true) //keep spinning until all messages have been send
		{
			switch(iState)
			{
				case ETCPConnectionEngineStateConnecting:
				{
					//connection has been established
					//send Handshake
					SendMessage(aHandshakeMessage);
					iState = ETCPConnectionEngineStateHandshakeWaitingForReply;
					break;
				}
				case ETCPConnectionEngineStateHandshakeWaitingForReply:
				{
					//get message from socket and convert
					SRemoteMessageNetworkFormat lMessageNetworkFormat;
					iSocket->Read(lMessageNetworkFormat.Buffer, TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE);
					SRemoteMessageInternalFormat lMessageInternalFormat = ConvertToInternalFormat(lMessageNetworkFormat);

					//check if we made a valid handshake
					if(lMessageInternalFormat.MessageContent->EqualTo(aHandshakePositiveReply))
					{
						//made a valid handshake, can proceed with sending data
						iState = ETCPConnectionEngineStateSending;
						delete lMessageInternalFormat.MessageContent; //not needed
						SendNextMessageInQueue(); //send the first message
					}
					else //didn't get the reply we wanted
					{
						//exit with failed handshake error
						iSocket->Close();
						iState = ETCPConnectionEngineStateIdle;
						return ETCPConnectionEngineSendStatusFailedHandshake;
					}
					break;
				}
				case ETCPConnectionEngineStateSending:
				{
					if(iMessageQueue->GetCount() > 0)
					{
						//last message has been send
						delete iMessageQueue->Get(0)->MessageContent;
						iMessageQueue->RemoveAndDestroy(0);
						//send the next message if one is available
						SendNextMessageInQueue();
					}
					else
					{
						//no more message to send, send signal that we are done sending and go into receiving mode
						iState = ETCPConnectionEngineStateReceiving;
						SendMessage(aSendingDoneSignal);
					}
					break;
				}
				case ETCPConnectionEngineStateReceiving:
				{
					//get message from socket and convert
					SRemoteMessageNetworkFormat lMessageNetworkFormat;
					iSocket->Read(lMessageNetworkFormat.Buffer, TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE);
					SRemoteMessageInternalFormat lMessageInternalFormat = ConvertToInternalFormat(lMessageNetworkFormat);

					//check if last message
					if(lMessageInternalFormat.MessageContent->EqualTo(aReceivingDoneSignal))
					{
						//recveived all data
						iState = ETCPConnectionEngineStateDisconnecting;
						delete lMessageInternalFormat.MessageContent; //not needed
					}
					//check if something went wrong during converting
					else if(lMessageInternalFormat.MessageContent->EqualTo(TCP_CONNECTION_ENGINE_ERROR_CONVERT_TO_INTERNAL_FORMAT))
					{
						//drop this message
						delete lMessageInternalFormat.MessageContent;
					}
					else
					{
						aReceivedMessageBuffer->Append(new SRemoteMessageInternalFormat(lMessageInternalFormat));
					}
					break;
				}
				case ETCPConnectionEngineStateDisconnecting:
				{
					//disconnect message has been send, close down
					iSocket->Close();
					iState = ETCPConnectionEngineStateIdle;
					return ETCPConnectionEngineSendStatusSuccess;
				}
				default:
				{
					//undefined state
					return ETCPConnectionEngineSendStatusFailedUnknown;
				}
			}
		}
	}

	return ETCPConnectionEngineSendStatusFailedNoWLan; //can only fail because can't use phone network or no wlan
}

TTCPConnectionEngineSendStatus CTCPConnectionEngine::SendMessages(const char* aSendingDoneSignal)
{
	if((iSendMessagesWithoutWLan || CSystem::GetIsConnectedToWLanNative()) && iMessageQueue->GetCount() > 0)
	{
		if(iState == ETCPConnectionEngineStateIdle)
		{
			//not busy with sending data already
			//run code to initiate sending loop of data, establish a connection to the server,
			iState = ETCPConnectionEngineStateConnecting;
			TBool lOpenSuccess = iSocket->Open(iConnectionTimeoutInSecs); //creates a blank socket for use for a specific protocol

			if(!lOpenSuccess)
			{
				//server must be down
				iState = ETCPConnectionEngineStateIdle;
				return ETCPConnectionEngineSendStatusFailedNoServer;
			}
		}

		while(true) //keep spinning until all messages have been send
		{
			switch(iState)
			{
				case ETCPConnectionEngineStateConnecting:
				{
					//connection has been established
					//now can Send data
					SendNextMessageInQueue();
					break;
				}
				case ETCPConnectionEngineStateSending:
				{
					if(iMessageQueue->GetCount() > 0)
					{
						//last message has been send
						delete iMessageQueue->Get(0)->MessageContent;
						iMessageQueue->RemoveAndDestroy(0);
						//send the next message if one is available
						SendNextMessageInQueue();
					}
					else
					{
						//no more messages to send, dis connect
						iState = ETCPConnectionEngineStateDisconnecting;
						SendMessage(aSendingDoneSignal);
					}
					break;
				}
				case ETCPConnectionEngineStateDisconnecting:
				{
					//disconnect message has been send, close down
					iSocket->Close();
					iState = ETCPConnectionEngineStateIdle;
					return ETCPConnectionEngineSendStatusSuccess;
				}
				default:
				{
					//undefined state
					return ETCPConnectionEngineSendStatusFailedUnknown;
				}
			}
		}
	}

	return ETCPConnectionEngineSendStatusFailedNoWLan; //can only fail because can't use phone network or no wlan
}

TTCPConnectionEngineSendStatus CTCPConnectionEngine::SendMessagesAndReceive(CPointerArray<SRemoteMessageInternalFormat>* aReceivedMessageBuffer, const char* aSignalThatSendingIsDone, const char* aSignalThatReceivingIsDone)
{
	if((iSendMessagesWithoutWLan || CSystem::GetIsConnectedToWLanNative()) && iMessageQueue->GetCount() > 0)
	{
		if(iState == ETCPConnectionEngineStateIdle)
		{
			//not busy with sending data already
			//run code to initiate sending loop of data, establish a connection to the server,
			iState = ETCPConnectionEngineStateConnecting;
			TBool lOpenSuccess = iSocket->Open(iConnectionTimeoutInSecs);//creates a blank socket for use for a specific protocol

			if(!lOpenSuccess)
			{
				//server must be down
				iState = ETCPConnectionEngineStateIdle;
				return ETCPConnectionEngineSendStatusFailedNoServer;
			}
		}

		while(true) //keep spinning until all messages have been send
		{
			switch(iState)
			{
				case ETCPConnectionEngineStateConnecting:
				{
					//connection has been established
					//now can Send data
					SendNextMessageInQueue();
					break;
				}
				case ETCPConnectionEngineStateSending:
				{
					if(iMessageQueue->GetCount() > 0)
					{
						//last message has been send
						delete iMessageQueue->Get(0)->MessageContent;
						iMessageQueue->RemoveAndDestroy(0);
						//send the next message if one is available
						SendNextMessageInQueue();
					}
					else
					{
						//no more message to send, send signal that we are done sending and go into receiving mode
						iState = ETCPConnectionEngineStateReceiving;
						SendMessage(aSignalThatSendingIsDone);
					}
					break;
				}
				case ETCPConnectionEngineStateReceiving:
				{
					//get message from socket and convert
					SRemoteMessageNetworkFormat lMessageNetworkFormat;
					iSocket->Read(lMessageNetworkFormat.Buffer, TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE);
					SRemoteMessageInternalFormat lMessageInternalFormat = ConvertToInternalFormat(lMessageNetworkFormat);

					//check if last message
					if(lMessageInternalFormat.MessageContent->EqualTo(aSignalThatReceivingIsDone))
					{
						//recveived all data
						iState = ETCPConnectionEngineStateDisconnecting;
						delete lMessageInternalFormat.MessageContent; //not needed
					}
					//check if something went wrong during converting
					else if(lMessageInternalFormat.MessageContent->EqualTo(TCP_CONNECTION_ENGINE_ERROR_CONVERT_TO_INTERNAL_FORMAT))
					{
						//drop this message
						delete lMessageInternalFormat.MessageContent;
					}
					else
					{
						aReceivedMessageBuffer->Append(new SRemoteMessageInternalFormat(lMessageInternalFormat));
					}
					break;
				}
				case ETCPConnectionEngineStateDisconnecting:
				{
					//disconnect message has been send, close down
					iSocket->Close();
					iState = ETCPConnectionEngineStateIdle;
					return ETCPConnectionEngineSendStatusSuccess;
				}
				default:
				{
					//undefined state
					return ETCPConnectionEngineSendStatusFailedUnknown;
				}
			}
		}
	}

	return ETCPConnectionEngineSendStatusFailedNoWLan; //can only fail because can't use phone network or no wlan
}

void CTCPConnectionEngine::SendNextMessageInQueue()
{
	//make sure we are in the right state
	iState = ETCPConnectionEngineStateSending;

	if(iMessageQueue->GetCount() > 0 && (iSendMessagesWithoutWLan || CSystem::GetIsConnectedToWLanNative())) //there is still something left to send,
	{
		SRemoteMessageInternalFormat& lMessageToSend = *iMessageQueue->Get(0);
		SRemoteMessageNetworkFormat lNetworkMessage = ConvertToNetworkFormat(lMessageToSend);
		TInt32* lTempBuffer = lNetworkMessage.Buffer;

		//ready to send
		iSocket->Send(lTempBuffer, TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE);
	}
}

void CTCPConnectionEngine::SendMessage(const char* aMessage)
{
	//construct message
	SRemoteMessageInternalFormat lMessage;
	lMessage.MessageContent = new CString(aMessage);

	//ready to send
	SRemoteMessageNetworkFormat lNetworkMessage = ConvertToNetworkFormat(lMessage);
	iSocket->Send(lNetworkMessage.Buffer, TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE);

	//meesage has been send, need to clean up temp message
	delete lMessage.MessageContent;
}

SRemoteMessageNetworkFormat CTCPConnectionEngine::ConvertToNetworkFormat(SRemoteMessageInternalFormat& lRemoteMessage)
{
	SRemoteMessageInternalFormat lInteralFormatMessage;
	SRemoteMessageNetworkFormat lNetworkFormatMessage;//don't want screw up original message

	//add delimiter so server knows where message starts and ends
	lInteralFormatMessage.MessageContent = new CString(KMessageDelimiter);
	lInteralFormatMessage.MessageContent->Append(lRemoteMessage.MessageContent);
	lInteralFormatMessage.MessageContent->Append(KMessageDelimiter);

	//zero out network message
	memset(lNetworkFormatMessage.Buffer, 0, sizeof(TInt32) * TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE);

	//transform into network format
	for(TInt lIndex = 0; lIndex < lInteralFormatMessage.MessageContent->GetLength(); lIndex++)
	{
		//need to convert to BigEndian to make it compatiable with Java
		CString* lCharacterToEncode = lInteralFormatMessage.MessageContent->GetSubString(lIndex, 1);
		lNetworkFormatMessage.Buffer[lIndex] = EncodeCharacter(lCharacterToEncode); //store in message buffer
		delete lCharacterToEncode;
	}

	//clean up temp message
	delete lInteralFormatMessage.MessageContent;

	return lNetworkFormatMessage;
}

SRemoteMessageInternalFormat CTCPConnectionEngine::ConvertToInternalFormat(SRemoteMessageNetworkFormat& lNetworkMessage)
{
	SRemoteMessageNetworkFormat lNetworkMessageTemp = lNetworkMessage;
	SRemoteMessageInternalFormat lMessageInternalFormat;
	lMessageInternalFormat.MessageContent = new CString("");
	TBool lStartReading = false;

	for(TInt lIndex = 0; lIndex < TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE; lIndex++)
	{
		//get character for this code value
		CString* lDecodedCharacter = DecodeCharacter(lNetworkMessage.Buffer[lIndex]);
		if(lDecodedCharacter->EqualTo(KMessageDelimiter))
		{
			delete lDecodedCharacter;
			lStartReading = !lStartReading; //is false first

			if(!lStartReading)
			{
				//means that was the end character. We are done with the message
				return lMessageInternalFormat;
			}
		}
		else if(lStartReading)
		{
			//copy over character into the message
			lMessageInternalFormat.MessageContent->Append(lDecodedCharacter);
			delete lDecodedCharacter;
		}
	}

	//something went wrong, maybe return a debug message
	lMessageInternalFormat.MessageContent->Replace(TCP_CONNECTION_ENGINE_ERROR_CONVERT_TO_INTERNAL_FORMAT);
	return lMessageInternalFormat;
}

void CTCPConnectionEngine::SetServerAddress(const char* aServerIpAddress, TInt aServerPort)
{
	iServerIpAddress->Replace(aServerIpAddress);
	iServerPort = aServerPort;
}

TBool CTCPConnectionEngine::IsInUse()
{
	if(iState == ETCPConnectionEngineStateIdle)
	{
		return false;
	}else
	{
		return true;
	}
}

TInt32 CTCPConnectionEngine::EncodeCharacter(CString* aCharacter)
{
	TInt lNumberOfCodePoints = iCodePoints->GetCount();
	for(TInt lIndex = 0; lIndex < lNumberOfCodePoints; lIndex++)
	{
		SCodePoint* lCodePoint = iCodePoints->Get(lIndex);
		if(aCharacter->EqualTo(lCodePoint->Character))
		{
			return lCodePoint->CodeValue;
		}
	}

	//wasn't found, just return its value, don't encode it
	return -aCharacter->GetCharacterCodeAsUtf8(0);
}

CString* CTCPConnectionEngine::DecodeCharacter(TInt32 aEncodedCharacter)
{
	TInt lNumberOfCodePoints = iCodePoints->GetCount();
	CString* lNotEncodedCharacter = NULL;

	for(TInt lIndex = 0; lIndex < lNumberOfCodePoints; lIndex++)
	{
		SCodePoint* lCodePoint = iCodePoints->Get(lIndex);
		if(aEncodedCharacter == lCodePoint->CodeValue)
		{
			lNotEncodedCharacter = new CString(lCodePoint->Character->GetData());
			return lNotEncodedCharacter;
		}
	}

	//wasn't found, is not encoded
	if(aEncodedCharacter < 0)
	{
		aEncodedCharacter *= -1;
		lNotEncodedCharacter = new CString("");
		lNotEncodedCharacter->AppendUtf8Value(aEncodedCharacter);
		return lNotEncodedCharacter;
	}
	else
	{
		//something went wrong
		lNotEncodedCharacter = new CString(KMessageCharacterError);
		return lNotEncodedCharacter;
	}
}

void CTCPConnectionEngine::CreateCodePoints()
{
	SCodePoint lCodePoint;
	//A
	lCodePoint.Character = new CString(KCapitalA);
	lCodePoint.CodeValue = 100;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//B
	lCodePoint.Character = new CString(KCapitalB);
	lCodePoint.CodeValue = 53532;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//C
	lCodePoint.Character = new CString(KCapitalC);
	lCodePoint.CodeValue = 32942;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//D
	lCodePoint.Character = new CString(KCapitalD);
	lCodePoint.CodeValue = 3594289;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//E
	lCodePoint.Character = new CString(KCapitalE);
	lCodePoint.CodeValue = 94289;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//F
	lCodePoint.Character = new CString(KCapitalF);
	lCodePoint.CodeValue = 90424;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//G
	lCodePoint.Character = new CString(KCapitalG);
	lCodePoint.CodeValue = 5;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//H
	lCodePoint.Character = new CString(KCapitalH);
	lCodePoint.CodeValue = 2315;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//I
	lCodePoint.Character = new CString(KCapitalI);
	lCodePoint.CodeValue = 7654;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//J
	lCodePoint.Character = new CString(KCapitalJ);
	lCodePoint.CodeValue = 3248;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//K
	lCodePoint.Character = new CString(KCapitalK);
	lCodePoint.CodeValue = 8679;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//L
	lCodePoint.Character = new CString(KCapitalL);
	lCodePoint.CodeValue = 868;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//M
	lCodePoint.Character = new CString(KCapitalM);
	lCodePoint.CodeValue = 29;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//N
	lCodePoint.Character = new CString(KCapitalN);
	lCodePoint.CodeValue = 2277;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//O
	lCodePoint.Character = new CString(KCapitalO);
	lCodePoint.CodeValue = 7646;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//P
	lCodePoint.Character = new CString(KCapitalP);
	lCodePoint.CodeValue = 77;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Q
	lCodePoint.Character = new CString(KCapitalQ);
	lCodePoint.CodeValue = 9999;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//R
	lCodePoint.Character = new CString(KCapitalR);
	lCodePoint.CodeValue = 325;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//S
	lCodePoint.Character = new CString(KCapitalS);
	lCodePoint.CodeValue = 886637;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//T
	lCodePoint.Character = new CString(KCapitalT);
	lCodePoint.CodeValue = 5902;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//U
	lCodePoint.Character = new CString(KCapitalU);
	lCodePoint.CodeValue = 6603;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//V
	lCodePoint.Character = new CString(KCapitalV);
	lCodePoint.CodeValue = 232;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//W
	lCodePoint.Character = new CString(KCapitalW);
	lCodePoint.CodeValue = 4;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//X
	lCodePoint.Character = new CString(KCapitalX);
	lCodePoint.CodeValue = 909;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Y
	lCodePoint.Character = new CString(KCapitalY);
	lCodePoint.CodeValue = 55558;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Z
	lCodePoint.Character = new CString(KCapitalZ);
	lCodePoint.CodeValue = 221789;
	iCodePoints->Append(new SCodePoint(lCodePoint));

	//a
	lCodePoint.Character = new CString(KLowerA);
	lCodePoint.CodeValue = 10090;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//b
	lCodePoint.Character = new CString(KLowerB);
	lCodePoint.CodeValue = 8;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//c
	lCodePoint.Character = new CString(KLowerC);
	lCodePoint.CodeValue = 2390;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//d
	lCodePoint.Character = new CString(KLowerD);
	lCodePoint.CodeValue = 321;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//e
	lCodePoint.Character = new CString(KLowerE);
	lCodePoint.CodeValue = 942829;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//f
	lCodePoint.Character = new CString(KLowerF);
	lCodePoint.CodeValue = 54525;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//g
	lCodePoint.Character = new CString(KLowerG);
	lCodePoint.CodeValue = 3;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//h
	lCodePoint.Character = new CString(KLowerH);
	lCodePoint.CodeValue = 324922;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//i
	lCodePoint.Character = new CString(KLowerI);
	lCodePoint.CodeValue = 22147;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//j
	lCodePoint.Character = new CString(KLowerJ);
	lCodePoint.CodeValue = 57432;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//k
	lCodePoint.Character = new CString(KLowerK);
	lCodePoint.CodeValue = 247;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//l
	lCodePoint.Character = new CString(KLowerL);
	lCodePoint.CodeValue = 112;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//m
	lCodePoint.Character = new CString(KLowerM);
	lCodePoint.CodeValue = 9324;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//n
	lCodePoint.Character = new CString(KLowerN);
	lCodePoint.CodeValue = 28967;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//o
	lCodePoint.Character = new CString(KLowerO);
	lCodePoint.CodeValue = 353566;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//p
	lCodePoint.Character = new CString(KLowerP);
	lCodePoint.CodeValue = 2021;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//q
	lCodePoint.Character = new CString(KLowerQ);
	lCodePoint.CodeValue = 2389;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//r
	lCodePoint.Character = new CString(KLowerR);
	lCodePoint.CodeValue = 12345678;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//s
	lCodePoint.Character = new CString(KLowerS);
	lCodePoint.CodeValue = 905634;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//t
	lCodePoint.Character = new CString(KLowerT);
	lCodePoint.CodeValue = 2221;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//u
	lCodePoint.Character = new CString(KLowerU);
	lCodePoint.CodeValue = 23134;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//v
	lCodePoint.Character = new CString(KLowerV);
	lCodePoint.CodeValue = 8987;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//w
	lCodePoint.Character = new CString(KLowerW);
	lCodePoint.CodeValue = 33226;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//x
	lCodePoint.Character = new CString(KLowerX);
	lCodePoint.CodeValue = 218;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//y
	lCodePoint.Character = new CString(KLowerY);
	lCodePoint.CodeValue = 919;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//z
	lCodePoint.Character = new CString(KLowerZ);
	lCodePoint.CodeValue = 3774872;
	iCodePoints->Append(new SCodePoint(lCodePoint));

	//0
	lCodePoint.Character = new CString(KNumber0);
	lCodePoint.CodeValue = 4812;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//1
	lCodePoint.Character = new CString(KNumber1);
	lCodePoint.CodeValue = 652;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//2
	lCodePoint.Character = new CString(KNumber2);
	lCodePoint.CodeValue = 21209;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//3
	lCodePoint.Character = new CString(KNumber3);
	lCodePoint.CodeValue = 2784;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//4
	lCodePoint.Character = new CString(KNumber4);
	lCodePoint.CodeValue = 3856;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//5
	lCodePoint.Character = new CString(KNumber5);
	lCodePoint.CodeValue = 221133;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//6
	lCodePoint.Character = new CString(KNumber6);
	lCodePoint.CodeValue = 784321;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//7
	lCodePoint.Character = new CString(KNumber7);
	lCodePoint.CodeValue = 3214;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//8
	lCodePoint.Character = new CString(KNumber8);
	lCodePoint.CodeValue = 83;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//9
	lCodePoint.Character = new CString(KNumber9);
	lCodePoint.CodeValue = 4312;
	iCodePoints->Append(new SCodePoint(lCodePoint));

	//Dot
	lCodePoint.Character = new CString(KDot);
	lCodePoint.CodeValue = 6784;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Semicolon
	lCodePoint.Character = new CString(KSemicolon);
	lCodePoint.CodeValue = 614324;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Dash
	lCodePoint.Character = new CString(KDash);
	lCodePoint.CodeValue = 8873;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Start and End of Message
	lCodePoint.Character = new CString(KEndAndStartOfMessage);
	lCodePoint.CodeValue = 48907;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Whitespace
	lCodePoint.Character = new CString(KWhitespace);
	lCodePoint.CodeValue = 24571;
	iCodePoints->Append(new SCodePoint(lCodePoint));
	//Underscore
	lCodePoint.Character = new CString(KUnderscoreCharacter);
	lCodePoint.CodeValue = 28771874;
	iCodePoints->Append(new SCodePoint(lCodePoint));
}

//-------------------------- CNative Sockets ---------------------------//

CNativeSocket::CNativeSocket(const char* aIpOfServer, TInt aPortOfServer)
{
	if(aIpOfServer != NULL)
		iServerIp = new CString(aIpOfServer);
	else
		iServerIp = NULL;

	iPort = aPortOfServer;
}

CNativeSocket::~CNativeSocket()
{
	if(iServerIp != NULL)
	{
		delete iServerIp;
		iServerIp = NULL;
	}
}
