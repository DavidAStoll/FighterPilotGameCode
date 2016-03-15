/*
 * CAndroidTCPConnectionEngine.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: dstoll
 */

#include "includes/native/utility/CAndroidTCPConnectionEngine.h"
#include "includes/native/CAndroidFighterPilotThePacificWar.h"

//need to call JNI methods
jobject CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject;

void CAndroidTCPConnectionEngine::InitJavaAndroidTCPConnectionEngineObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidTCPConnectionEngine", &CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject);
}

CNativeSocket* CNativeSocket::New(const char* aIpOfServer, TInt aPortOfServer)
{
	CAndroidSocket* lSelf = new CAndroidSocket(aIpOfServer, aPortOfServer);
	return lSelf;
}

void CNativeSocket::Close()
{
	//will be overridden by child
}

void CNativeSocket::Read(TInt32* aMessageBuffer, TInt aSizeOfMessageInInts)
{
	//will be overridden by child
}

void CNativeSocket::Send(TInt32* aMessageData, TInt aSizeOfMessageInInts)
{
	//will be overridden by child
}

TBool CNativeSocket::Open(TInt aConnectionTimeoutInSecs)
{
	//will be overridden by child
	return true;
}

//------------------ Android Socket Implementation ------------------------//

CAndroidSocket::CAndroidSocket(const char* aIpOfServer, TInt aPort)
:CNativeSocket(aIpOfServer, aPort)
{
	Construct(); //make a call to java to create SocketChannel object
}

CAndroidSocket::~CAndroidSocket()
{
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	lJniEnv->DeleteGlobalRef(iJavaSocketObject);
}

void CAndroidSocket::Construct()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//get IDs
	jclass lTCPSocketClass = lJniEnv->GetObjectClass(CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject);
	jmethodID lCreateObjectMethod = lJniEnv->GetMethodID(lTCPSocketClass, "<init>", "()V");

	//create read object
	jobject lTCPSocketObject = (jobject) lJniEnv->NewObject(lTCPSocketClass, lCreateObjectMethod);
	lJniEnv->NewGlobalRef(lTCPSocketObject);
	lJniEnv->DeleteLocalRef(lTCPSocketObject);

	//garbage collection
	lJniEnv->DeleteLocalRef(lTCPSocketClass);

	iJavaSocketObject = lTCPSocketObject;
}

void CAndroidSocket::Close()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lTCPSocketClass = lJniEnv->GetObjectClass(CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject);
	//method ID
	jmethodID lCloseSocketMethod = lJniEnv->GetStaticMethodID(lTCPSocketClass, "Close", "(LfighterPilot/RisingSunAlpha/AndroidTCPConnectionEngine;)V");

	//Close Socket
	lJniEnv->CallStaticVoidMethod(lTCPSocketClass, lCloseSocketMethod, (jobject) iJavaSocketObject);

	//garbage collection
	lJniEnv->DeleteLocalRef(lTCPSocketClass);
}

void CAndroidSocket::Read(TInt32* aMessageBuffer, TInt aSizeOfMessageInInts)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lTCPSocketClass = lJniEnv->GetObjectClass(CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject);
	//method ID
	jmethodID lReadMethod = lJniEnv->GetStaticMethodID(lTCPSocketClass, "Read", "(LfighterPilot/RisingSunAlpha/AndroidTCPConnectionEngine;I)[I");

	//send message into java socket
	jintArray lJavaIntArray = (jintArray) lJniEnv->CallStaticObjectMethod(lTCPSocketClass, lReadMethod, (jobject) iJavaSocketObject, (jint) aSizeOfMessageInInts);

	//copy array back into C space
	TInt32* lJavaCopy = (TInt32*) lJniEnv->GetIntArrayElements(lJavaIntArray, 0);
	memcpy(aMessageBuffer, lJavaCopy, aSizeOfMessageInInts * sizeof(TInt32));

	//garbage collection
	lJniEnv->ReleaseIntArrayElements(lJavaIntArray, (jint*) lJavaCopy, 0); //release the array
	lJniEnv->DeleteLocalRef(lTCPSocketClass);
	lJniEnv->DeleteLocalRef(lJavaIntArray);
}

void CAndroidSocket::Send(TInt32* aMessageData, TInt aSizeOfMessageInInts)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lTCPSocketClass = lJniEnv->GetObjectClass(CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject);
	//method ID
	jmethodID lSendMethod = lJniEnv->GetStaticMethodID(lTCPSocketClass, "Send", "(LfighterPilot/RisingSunAlpha/AndroidTCPConnectionEngine;[I)V");

	//coppy message into java array
	jintArray lJavaIntArray = lJniEnv->NewIntArray(aSizeOfMessageInInts);
	TInt32* lJavaCopy = (TInt32*) lJniEnv->GetIntArrayElements(lJavaIntArray, 0);
	memcpy(lJavaCopy, aMessageData, aSizeOfMessageInInts * sizeof(TInt32));

	//send message into java socket
	lJniEnv->CallStaticVoidMethod(lTCPSocketClass, lSendMethod, (jobject) iJavaSocketObject, lJavaIntArray);

	//garbage collection
	lJniEnv->ReleaseIntArrayElements(lJavaIntArray, (jint*) lJavaCopy, 0); //release the array
	lJniEnv->DeleteLocalRef(lTCPSocketClass);
	lJniEnv->DeleteLocalRef(lJavaIntArray);
}

TBool CAndroidSocket::Open(TInt aConnectionTimeoutInSecs)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lTCPSocketClass = lJniEnv->GetObjectClass(CAndroidTCPConnectionEngine::JavaTCPConnectionEngineObject);
	//method ID
	jmethodID lOpenSocketMethod = lJniEnv->GetStaticMethodID(lTCPSocketClass, "Open", "(LfighterPilot/RisingSunAlpha/AndroidTCPConnectionEngine;Ljava/lang/String;II)Z");

	//Open Socket
	jstring lJavaStringIpOfServer = lJniEnv->NewStringUTF(iServerIp->GetData());
	TBool lSocketOpenSuccess = (jboolean) lJniEnv->CallStaticBooleanMethod(lTCPSocketClass, lOpenSocketMethod, (jobject) iJavaSocketObject,lJavaStringIpOfServer, iPort, aConnectionTimeoutInSecs);

	//garbage collection
	lJniEnv->DeleteLocalRef(lTCPSocketClass);
	lJniEnv->DeleteLocalRef(lJavaStringIpOfServer);

	return lSocketOpenSuccess;
}


