/*
 * CAndroidTouchEventHandler.cpp
 *
 *  Created on: Jul 8, 2012
 *      Author: dstoll
 */

#include <jni.h>
#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/CGame.h"

void CTouchEventHandler::AddRawEvent(const STouchEventLocationRaw& aRawEvent)
{
	//don't really need to do anything here, just gonna pass it on
	AddAndConvertRawEvent(aRawEvent);
}

extern "C"
{
	JNIEXPORT void JNICALL Java_fighterPilot_RisingSunAlpha_FighterPilotActivity_AddRawTouchEvent(JNIEnv* lEnv, jobject obj, jint aXLocation, jint aYLocation, jint aTypeEvent, jint aFingerId)
	{
		STouchEventLocationRaw lRawEvent;
		lRawEvent.XPixelLocation = aXLocation;
		lRawEvent.YPixelLocation = aYLocation;

		//find out what type of event it is. For Android 0 is Down, 1 is Draw and 2 is Up
		if(aTypeEvent == 0)
		{
			lRawEvent.Type = ETouchEventHandlerTouchTypeDown;
		}
		else if(aTypeEvent == 1)
		{
			lRawEvent.Type = ETouchEventHandlerTouchTypeDrag;
		}
		else
		{
			lRawEvent.Type = ETouchEventHandlerTouchTypeUp;
		}

		lRawEvent.FingerId = aFingerId;

		CGame::Game->iTouchEventHandler->AddRawEvent(lRawEvent);
	}
}

