/*
 * CAndroidAssetDirectory.cpp
 *
 *  Created on: Jul 2, 2012
 *      Author: dstoll
 */

#include "includes/native/utility/CAndroidAssetDirectory.h"
#include "includes/native/CAndroidFighterPilotThePacificWar.h"

jobject CAndroidAssetDirectory::JavaAssetDirectoryObject;

void CAndroidAssetDirectory::InitJavaAssetDirectoryObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidAssetDirectory", &CAndroidAssetDirectory::JavaAssetDirectoryObject);
}

TInt CAssetDirectory::GetAssetFileSizeInBytes(const char* aFileName)
{
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI
	TInt lSizeInBytes = 0;
	//get IDs
	jclass lWavLoaderClass = lJniEnv->GetObjectClass(CAndroidAssetDirectory::JavaAssetDirectoryObject);
	jmethodID lGetWavFileSizeMethod = lJniEnv->GetStaticMethodID(lWavLoaderClass, "GetAssetFileSizeInBytes", "(Ljava/lang/String;)I");

	//call method
	jstring lJavaString = lJniEnv->NewStringUTF(aFileName);
	lSizeInBytes = (TInt) lJniEnv->CallStaticIntMethod(lWavLoaderClass, lGetWavFileSizeMethod, lJavaString);

	//release references
	lJniEnv->DeleteLocalRef(lWavLoaderClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lSizeInBytes;
}

void CAssetDirectory::PlayVideoInAssetDirectoryNative(const char* aVideoName, TInt aVolume)
{
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI
	//get IDs
	jclass lWavLoaderClass = lJniEnv->GetObjectClass(CAndroidAssetDirectory::JavaAssetDirectoryObject);
	jmethodID lPlayVideoFromAssetDirectory = lJniEnv->GetStaticMethodID(lWavLoaderClass, "PlayVideoFromAssetDirectory", "(Ljava/lang/String;I)V");

	//call method
	jstring lJavaString = lJniEnv->NewStringUTF(aVideoName);
	lJniEnv->CallStaticVoidMethod(lWavLoaderClass, lPlayVideoFromAssetDirectory, lJavaString, aVolume);

	//release references
	lJniEnv->DeleteLocalRef(lWavLoaderClass);
	lJniEnv->DeleteLocalRef(lJavaString);
}

TUint8* CAssetDirectory::ReadAssetFile(const char* aFileName)
{
	TUint8* lFileAsBytes = NULL;
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI
	//get IDs
	jclass lWavLoaderClass = lJniEnv->GetObjectClass(CAndroidAssetDirectory::JavaAssetDirectoryObject);
	jmethodID lGetWavFileInBytesMethod = lJniEnv->GetStaticMethodID(lWavLoaderClass, "ReadAssetFile", "(Ljava/lang/String;)[B");

	//call method
	jstring lJavaString = lJniEnv->NewStringUTF(aFileName);
	jbyteArray lJavaByteArray = (jbyteArray) lJniEnv->CallStaticObjectMethod(lWavLoaderClass, lGetWavFileInBytesMethod, lJavaString);
	TUint8* lCharacterArray = (TUint8*) lJniEnv->GetByteArrayElements(lJavaByteArray, NULL);
	int lByteArraySize = lJniEnv->GetArrayLength(lJavaByteArray);
	lFileAsBytes = new TUint8[lByteArraySize];

	//make a copy of the content
	memcpy(lFileAsBytes, lCharacterArray, lByteArraySize);

	//release references
	lJniEnv->ReleaseByteArrayElements(lJavaByteArray, (jbyte*) lCharacterArray, 0);
	lJniEnv->DeleteLocalRef(lJavaByteArray);
	lJniEnv->DeleteLocalRef(lWavLoaderClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lFileAsBytes;
}


TUint8* CAssetDirectory::ReadBytesFromAssetFile(const char* aFileName, TUint aNumberOfBytes, TUint aOffsetInBytes)
{
	TUint8* lFileAsBytes = NULL;
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI
	//get IDs
	jclass lWavLoaderClass = lJniEnv->GetObjectClass(CAndroidAssetDirectory::JavaAssetDirectoryObject);
	jmethodID lGetWavFileInBytesMethod = lJniEnv->GetStaticMethodID(lWavLoaderClass, "ReadBytesFromAssetFile", "(Ljava/lang/String;II)[B");

	//call method
	jstring lJavaString = lJniEnv->NewStringUTF(aFileName);
	jbyteArray lJavaByteArray = (jbyteArray) lJniEnv->CallStaticObjectMethod(lWavLoaderClass, lGetWavFileInBytesMethod, lJavaString, aNumberOfBytes, aOffsetInBytes);
	TUint8* lCharacterArray = (TUint8*) lJniEnv->GetByteArrayElements(lJavaByteArray, NULL);
	int lByteArraySize = lJniEnv->GetArrayLength(lJavaByteArray);
	lFileAsBytes = new TUint8[lByteArraySize];

	//make a copy of the content
	memcpy(lFileAsBytes, lCharacterArray, lByteArraySize);

	//release references
	lJniEnv->ReleaseByteArrayElements(lJavaByteArray, (jbyte*) lCharacterArray, 0);
	lJniEnv->DeleteLocalRef(lJavaByteArray);
	lJniEnv->DeleteLocalRef(lWavLoaderClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lFileAsBytes;
}
