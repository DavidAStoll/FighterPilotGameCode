/*
 * CAndroidAmazonGameCircleFunctions.cpp
 *
 *  Created on: Dec 28, 2012
 *      Author: dstoll
 */

#include "includes/native/utility/CAndroidAmazonGameCircleFunctions.h"
#include "includes/native/CAndroidFighterPilotThePacificWar.h"

jobject CAndroidAmazonGameCircleFunctions::JavaAndroidAmazonGameCircleFunctionsObject;

void CAndroidAmazonGameCircleFunctions::InitJavaAndroidAmazonGameCircleFunctionsObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AmazonGameCircleFunctions", &CAndroidAmazonGameCircleFunctions::JavaAndroidAmazonGameCircleFunctionsObject);
}

void CAndroidAmazonGameCircleFunctions::UploadScore(const char* aLeaderboardId, int aScore)
{
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI

	//get IDs
	jclass lAmazonGameCircleClass = lJniEnv->GetObjectClass(CAndroidAmazonGameCircleFunctions::JavaAndroidAmazonGameCircleFunctionsObject);
	jmethodID lUploadScore = lJniEnv->GetStaticMethodID(lAmazonGameCircleClass, "UploadScoreToLeaderboard", "(Ljava/lang/String;I)V");

	//call method
	jstring lJavaString = lJniEnv->NewStringUTF(aLeaderboardId);
	lJniEnv->CallStaticVoidMethod(lAmazonGameCircleClass, lUploadScore, lJavaString, aScore);

	//release references
	lJniEnv->DeleteLocalRef(lAmazonGameCircleClass);
	lJniEnv->DeleteLocalRef(lJavaString);
}

