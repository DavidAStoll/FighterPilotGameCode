/*
 * CAndroidSystem.h
 *
 *  Created on: Oct 21, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDSYSTEM_H_
#define CANDROIDSYSTEM_H_

#include <jni.h>
#include "includes/core/utility/CSystem.h"

class CAndroidSystem
{
public:
	static void InitJavaAndroidSystemObject(JNIEnv* aJniEnv);

	//need to call JNI methods
	static jobject JavaSystemObject;
};


#endif /* CANDROIDSYSTEM_H_ */
