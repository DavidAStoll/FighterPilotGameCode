/*
 * CAndroidImageStore.h
 *
 *  Created on: Jun 12, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDIMAGESTORE_H_
#define CANDROIDIMAGESTORE_H_

#include <jni.h>
#include "includes/core/utility/CImageStore.h"

class CAndroidImageStore
{
public:
	static void InitJavaImageStoreObject(JNIEnv* aJniEnv);

	//need to call JNI methods
	static jobject JavaImageStoreObject;
};


#endif /* CANDROIDIMAGESTORE_H_ */
