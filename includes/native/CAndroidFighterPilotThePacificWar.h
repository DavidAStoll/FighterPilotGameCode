/*
 * CAndroidFighterPilotThePacificWar.h
 *
 *  Created on: May 8, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDFIGHTERPILOTTHEPACIFICWAR_H_
#define CANDROIDFIGHTERPILOTTHEPACIFICWAR_H_

#include "includes/core/sound/CMusicPlayer.h"
#include "includes/core/sound/CSoundEngine.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include <jni.h>

class CAndroidFighterPilotThePacificWar : public CFighterPilotThePacificWar
{
public:

	CAndroidFighterPilotThePacificWar(JNIEnv* aJniEnv);
	virtual ~CAndroidFighterPilotThePacificWar();
	//this method is used to get an instance to JNI, it more for convience to avoid rewriting the same code over and over again
	static JNIEnv* GetJNI();
	//this method is needed to creates global Java objects, these can be used to JNI calls from different threads
	static void CreateJavaObjectForVM(JNIEnv* aEnv, const char* aJavaClassPath, jobject* aObjectPointer);

	static JavaVM* VM;

};

#endif /* CANDROIDFIGHTERPILOTTHEPACIFICWAR_H_ */
