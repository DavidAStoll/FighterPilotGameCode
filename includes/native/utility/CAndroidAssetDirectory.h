/*
 * CAndroidAssetDirectory.h
 *
 *  Created on: Jul 2, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDASSETDIRECTORY_H_
#define CANDROIDASSETDIRECTORY_H_

#include <jni.h>
#include "includes/core/utility/CAssetDirectory.h"

class CAndroidAssetDirectory
{
public:
	//needed for JNI
	static void InitJavaAssetDirectoryObject(JNIEnv* aJniEnv);
	static jobject JavaAssetDirectoryObject;
};

#endif /* CANDROIDASSETDIRECTORY_H_ */
