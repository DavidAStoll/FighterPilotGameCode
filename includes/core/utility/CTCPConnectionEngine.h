/*
 * CTCPConnectionEngine.h
 *
 *  Created on: Oct 21, 2012
 *      Author: dstoll
 */

#ifndef CTCPCONNECTIONENGINE_H_
#define CTCPCONNECTIONENGINE_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

#define TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE 300
#define TCP_CONNECTION_ENGINE_ERROR_CONVERT_TO_INTERNAL_FORMAT "ERROR DURING CONVERT TO INTERNAL!!!"

class CNativeSocket;

struct SRemoteMessageInternalFormat
{
	CString* MessageContent;
};

struct SRemoteMessageNetworkFormat
{
	TInt32 Buffer[TCP_CONNECTION_ENGINE_MAX_CHARACTERS_PER_MESSAGE];
};

struct SCodePoint
{
	CString* Character;
	TInt32 CodeValue;
};

enum TTCPConnectionEngineSendStatus
{
	ETCPConnectionEngineSendStatusSuccess = 1,
	ETCPConnectionEngineSendStatusFailedNoWLan = -1,
	ETCPConnectionEngineSendStatusFailedNoServer = -2,
	ETCPConnectionEngineSendStatusFailedHandshake = -3,
	ETCPConnectionEngineSendStatusFailedUnknown = -4
};

enum TTCPConnectionEngineState
{
	ETCPConnectionEngineStateIdle, //default state, if everything has been closed
	ETCPConnectionEngineStateDisconnecting, //state when the client is going to disconnect from the server

	ETCPConnectionEngineStateConnecting, //state when it want it tries to connect to a remote host to send data - client call
	ETCPConnectionEngineStateAccepting, //state when it tries to accept in coming client calls - server call

	ETCPConnectionEngineStateHandshakeWaitingForReply, //state when after sending the Handshake message it waits to get a reply

	ETCPConnectionEngineStateSending, //state when it initated sending a packet of data - client call
	ETCPConnectionEngineStateReceiving //state when it initated to receive a packet of data - server call
};

enum TPSWlanIndicator
{
	EPSWlanIndicatorNone = 0, EPSWlanIndicatorAvailable, EPSWlanIndicatorActive, EPSWlanIndicatorActiveSecure
};

class CTCPConnectionEngine
{
public:

	virtual ~CTCPConnectionEngine();
	static CTCPConnectionEngine* New(const char* aServerIpAddress, TInt aServerPort, TInt aConnectionTimeoutValueInSecs);

	//adds a message to the queue
	void AddMessage(const SRemoteMessageInternalFormat& aMessage);
	//returns if the Connection is in Use
	TBool IsInUse();
	//first sends a handshake message to the server, and then waits to receive a message, if the message received matches the required reply it will send its messages that have been stored
	//after sending its messages, it goes into receiving mode until it gets a message that receiving is done
	TTCPConnectionEngineSendStatus HandshakeThenSendMessagesAndReceive(const char* aHandshakeMessage, const char* aHandshakePositiveReply, CPointerArray<SRemoteMessageInternalFormat>* aReceivedMessageBuffer, const char* aSendingDoneSignal, const char* aReceivingDoneSignal);
	//sends all Messages to the predefined server from its queue
	//returns true if it succeeded
	TTCPConnectionEngineSendStatus SendMessages(const char* aSendingDoneSignal);
	//sends all stored messages to server, then goes into receiving mode until signal message has been received that it is done
	//appends all received messages into the ReceivedMessageBuffer CPointerArray
	TTCPConnectionEngineSendStatus SendMessagesAndReceive(CPointerArray<SRemoteMessageInternalFormat>* aReceivedMessageBuffer, const char* aSendingDoneSignal, const char* aReceivingDoneSignal);
	//Set for how many Secs the socket tries to connect to the server before giving up
	void SetConnectionTimeout(TInt aConnectionTimeoutValueInSecs);
	//Set if a WLan is NOT needed to send the data
	void SetIfWLanNotNeededToConnect(TBool aValue);
	//sets the Server address to send the data to
	void SetServerAddress(const char* aServerIpAddress, TInt aServerPort);

protected:

	CTCPConnectionEngine(const char* aServerIpAddress, TInt aPort, TInt aConnectionTimeoutValueInSecs);
	void Construct();

	TInt32 EncodeCharacter(CString* aCharacter);
	CString* DecodeCharacter(TInt32 aEncodedCharacter);

	//populates the CodePoint array
	void CreateCodePoints();
	//converts the message from internal to network format
	SRemoteMessageNetworkFormat ConvertToNetworkFormat(SRemoteMessageInternalFormat& lRemoteMessage);
	//converts message from network to internal format
	SRemoteMessageInternalFormat ConvertToInternalFormat(SRemoteMessageNetworkFormat& lNetworkMessage);
	//inspects the queue and tries to send the next message, if no messages are left in the queue. It will close the connection with the server
	virtual void SendNextMessageInQueue();
	//this message is send if the client wants to disconnect from the server, usually after sending all remaining messages
	void SendMessage(const char* aMessage);

	TTCPConnectionEngineState iState;
	TBool iSendMessagesWithoutWLan;
	//needed to connect to server
	CNativeSocket* iSocket; //a socket for a particular connection
	CString* iServerIpAddress;
	TInt iServerPort;
	TInt iConnectionTimeoutInSecs;
	//for sending messages
	CPointerArray<SRemoteMessageInternalFormat>* iMessageQueue;
	CPointerArray<SCodePoint>* iCodePoints; //for encoding messages
};

//obvisouly a native Socket that is an abstraction from a OS Socket, should have all the basic functions to send TCP packets to a server
class CNativeSocket
{
public:

	//going to use static function here to construct object, gives us the flexibility that we can return
	//a child that has more extra functions or data
	static CNativeSocket* New(const char* aIpOfServer, TInt aPortOfServer);
	virtual ~CNativeSocket();

	//Closes the sockets, does everything nessary to gracefully close the sockets, should be blocking
	virtual void Close();
	//waits on the Socket to receive the given amount of data, method should block until given amount of Ints are received or connection closes
	//should convert data from BigEndian to LittleEndian before returning
	virtual void Read(TInt32* aMessageBuffer, TInt aSizeOfMessageInInts);
	//sends data to previously connected server, should be blocking. After method call we can assume that the message has been delivered
	//should convert data from LittleEndian To BigEndian before sending it
	virtual void Send(TInt32* aMessageData, TInt aSizeOfMessageInInts);
	//Opens a Connection the server, function should be blocking, after this call we should be ready to send data through this socket
	//returns a true if Connection was made successfully
	//For how many Secs it keeps trying to connect with the server until it fails
	virtual TBool Open(TInt aConnectionTimeoutInSecs);

protected:
	CNativeSocket(const char* aIpOfServer, TInt aPortOfServer);

	CString* iServerIp;
	TInt iPort;
};

#endif /* CTCPCONNECTIONENGINE_H_ */
