/*
 * CAndroidTCPConnectionEngine.h
 *
 *  Created on: Oct 21, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDTCPCONNECTIONENGINE_H_
#define CANDROIDTCPCONNECTIONENGINE_H_

#include "includes/core/utility/CTCPConnectionEngine.h"
#include <jni.h>

class CAndroidTCPConnectionEngine
{
public:

	//need to call JNI methods
	static jobject JavaTCPConnectionEngineObject;
	static void InitJavaAndroidTCPConnectionEngineObject(JNIEnv* aJniEnv);
};

class CAndroidSocket : public CNativeSocket
{
public:

	virtual ~CAndroidSocket();

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

	CAndroidSocket(const char* aIpOfServer, TInt aPort);

protected:

	void Construct();

	jobject iJavaSocketObject;
};

#endif /* CANDROIDTCPCONNECTIONENGINE_H_ */
