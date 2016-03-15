/*
 * CAndroidSystem.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: dstoll
 */
#include "includes/native/CAndroidFighterPilotThePacificWar.h"
#include "includes/native/utility/CAndroidSystem.h"
#include <time.h>

//need to call JNI methods
jobject CAndroidSystem::JavaSystemObject;

void CAndroidSystem::InitJavaAndroidSystemObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidSystem", &CAndroidSystem::JavaSystemObject);
}

void CSystem::ExitNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lExitMethod = lJniEnv->GetStaticMethodID(lSystemClass, "Exit", "()V");

	//make exit call
	lJniEnv->CallStaticVoidMethod(lSystemClass, lExitMethod);

	//garbage collection
	lJniEnv->DeleteLocalRef(lSystemClass);
}

TInt64 CSystem::GetCurrentTimeInMicroSecondsNative()
{
	timespec lTime;
	clock_gettime(CLOCK_MONOTONIC, &lTime);

	//seems like structer keeps secs stored in tv_sec and the remainder in tv_nsec thus we need to add bother together
	return (TInt64) (lTime.tv_sec * 1000000000LL + lTime.tv_nsec) / 1000; //given time is in nano secs ^-9, but we just want micro ^-6
}

CString* CSystem::GetDateNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lGetDateMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetDate", "()Ljava/lang/String;");

	//Write String to file
	jstring lJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lSystemClass, lGetDateMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lJavaString, NULL);
	CString* lDateString = new CString(lNativeString);

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lSystemClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lDateString;
}

CString* CSystem::GetPhoneSerialNumberNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lSerialNumberMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetPhoneSerialNumber", "()Ljava/lang/String;");

	//Write String to file
	jstring lJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lSystemClass, lSerialNumberMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lJavaString, NULL);
	CString* lSerialNumberString = new CString(lNativeString);

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lSystemClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lSerialNumberString;
}

CString* CSystem::GetPhoneModelNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lGetPhoneModelMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetPhoneModel", "()Ljava/lang/String;");

	//Write String to file
	jstring lJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lSystemClass, lGetPhoneModelMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lJavaString, NULL);
	CString* lPhoneModelString = new CString(lNativeString);

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lSystemClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lPhoneModelString;
}

CString* CSystem::GetPhoneOSVersionNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lPhoneOsVersionMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetPhoneOSVersion", "()Ljava/lang/String;");

	//Write String to file
	jstring lJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lSystemClass, lPhoneOsVersionMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lJavaString, NULL);
	CString* lOsVersionString = new CString(lNativeString);

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lSystemClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lOsVersionString;
}

CString* CSystem::GetTimeNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lTimeMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetTime", "()Ljava/lang/String;");

	//Write String to file
	jstring lJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lSystemClass, lTimeMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lJavaString, NULL);
	CString* lTimeString = new CString(lNativeString);

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lSystemClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lTimeString;
}

TBool CSystem::GetIsConnectedToWLanNative()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lConnectedToWLanMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetIsConnectedToWLan", "()Z");

	//Write String to file
	TBool lIsConnected = (jboolean) lJniEnv->CallStaticBooleanMethod(lSystemClass, lConnectedToWLanMethod);

	//garbage collection
	lJniEnv->DeleteLocalRef(lSystemClass);

	return lIsConnected;
}

TBool CSystem::GetIsSilentModeActivated()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lSilentModeMethod = lJniEnv->GetStaticMethodID(lSystemClass, "GetIsInSilentMode", "()Z");

	//Write String to file
	TBool lSilentModeActivated = (jboolean) lJniEnv->CallStaticBooleanMethod(lSystemClass, lSilentModeMethod);

	//garbage collection
	lJniEnv->DeleteLocalRef(lSystemClass);

	return lSilentModeActivated;
}

void CSystem::SleepForMicroSecondsNative(TUint64 aSleepTimeInMicroSeconds)
{
	if(aSleepTimeInMicroSeconds > 0)
	{
		timespec lTime;
		lTime.tv_sec = aSleepTimeInMicroSeconds / 1000000; //go from Micro to Secs
		lTime.tv_nsec = (aSleepTimeInMicroSeconds - lTime.tv_sec * 1000000) * 1000; //need to convert from Micro to Nano

		nanosleep(&lTime, NULL); //don't care if thread wakes up earlier than requested
	}
}

void CSystem::OpenWebsite(const char* aURL)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lSystemClass = lJniEnv->GetObjectClass(CAndroidSystem::JavaSystemObject);
	//method ID to Write String
	jmethodID lOpenWebsiteMethod = lJniEnv->GetStaticMethodID(lSystemClass, "OpenWebsite", "(Ljava/lang/String;)V");

	//OpenURL
	jstring lJavaString = lJniEnv->NewStringUTF(aURL);
	lJniEnv->CallStaticVoidMethod(lSystemClass, lOpenWebsiteMethod, lJavaString);

	//garbage collection
	lJniEnv->DeleteLocalRef(lSystemClass);
	lJniEnv->DeleteLocalRef(lJavaString);
}


