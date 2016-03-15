/*
 * CAndroidImageStore.cpp
 *
 *  Created on: Jun 12, 2012
 *      Author: dstoll
 */

#include <stdlib.h>
#include "includes/native/utility/CAndroidImageStore.h"
#include "includes/native/CAndroidFighterPilotThePacificWar.h"

jobject CAndroidImageStore::JavaImageStoreObject;

void CAndroidImageStore::InitJavaImageStoreObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidImage", &CAndroidImageStore::JavaImageStoreObject);
}




