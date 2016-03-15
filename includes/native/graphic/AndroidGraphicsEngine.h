/*
 * AndroidGraphicsEngine.h
 *
 *  Created on: Jun 5, 2012
 *      Author: dstoll
 */

#ifndef ANDROIDGRAPHICSENGINE_H_
#define ANDROIDGRAPHICSENGINE_H_

#include <jni.h>

class CAndroidGraphicsEngine
{
public:
	static void InitJavaAndroidGraphicsEngine(JNIEnv* aJniEnv);

	//need to call JNI methods
	static jobject JavaAndroidGraphicsEngineObject;
};


#endif /* ANDROIDGRAPHICSENGINE_H_ */
