/*
 * CAndroidAmazonGameCircleFunctions.h
 *
 *  Created on: Dec 28, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDAMAZONGAMECIRCLEFUNCTIONS_H_
#define CANDROIDAMAZONGAMECIRCLEFUNCTIONS_H_

#include <jni.h>
#include "includes/core/utility/CAssetDirectory.h"

class CAndroidAmazonGameCircleFunctions
{
public:
	//needed for JNI
	static void InitJavaAndroidAmazonGameCircleFunctionsObject(JNIEnv* aJniEnv);
	static jobject JavaAndroidAmazonGameCircleFunctionsObject;
	static void UploadScore(const char* aLeaderboardId, int aScore);
};

#endif /* CANDROIDAMAZONGAMECIRCLEFUNCTIONS_H_ */
