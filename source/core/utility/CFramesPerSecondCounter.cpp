/*
 * CFramesPerSecondCounter.cpp
 *
 *  Created on: Jul 17, 2012
 *      Author: dstoll
 */

#include "includes/core/utility/CFramesPerSecondCounter.h"
#include "includes/core/utility/CSystem.h"
#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/core/CGame.h"

#define MICRO_SECONDS_PER_SECOND 1000000

CFramesPerSecondCounter::CFramesPerSecondCounter(TUint aFramesPerSecond)
{
	iMicroRequestedSleepTime = 0;
	iMicroSecsValueFromLastSecond = 0;
	iTotalMicroSecsBetweenFramesPerSec = 0;
	iTotalMicroSecsLostDueToSleepPerSec = 0;
	iTempFramesPerSec = 1; //otherwise might have division by 0 for Average Stats
	iFramesPerSec = 0;
	iTotalPercentageUsedByLogicCode = 0;
	iTotalPercentageUsedByDrawingCode = 0;
	iAveragePercentageUsedByLogicCode = 0;
	iAveragePercentageUsedByDrawingCode = 0;
	iUpdatePercentageUsedCounter = 0;

	iRequiredFramesPerSecond = aFramesPerSecond;
	iTotalMircoSecsPerFrame = MICRO_SECONDS_PER_SECOND / aFramesPerSecond;
}

CFramesPerSecondCounter::~CFramesPerSecondCounter()
{
	// TODO Auto-generated destructor stub
}

CFramesPerSecondCounter* CFramesPerSecondCounter::New(TUint aFramesPerSecond)
{
	CFramesPerSecondCounter* lSelf = new CFramesPerSecondCounter(aFramesPerSecond);
	return lSelf;
}


//---------------------- functions ------------------------//

void CFramesPerSecondCounter::CounterFunctionAfterDrawingCode()
{
	iMicroTimeAfterDrawingCode = CSystem::GetCurrentTimeInMicroSecondsNative();
	TInt64 lTimeUsedByDrawingCode = iMicroTimeAfterDrawingCode - iMicroTimeBeforeDrawingCode;
	iTotalPercentageUsedByDrawingCode += (lTimeUsedByDrawingCode * 100) / iTotalMircoSecsPerFrame; //multiply be 100 to get percentage and to avoid underflow
}

void CFramesPerSecondCounter::CounterFunctionBeforeDrawingCode()
{
	iMicroTimeBeforeDrawingCode = CSystem::GetCurrentTimeInMicroSecondsNative();
	TInt64 lMicroSecsUsedSinceLastSec = iMicroTimeBeforeDrawingCode - iMicroSecsValueFromLastSecond;

	iUpdatePercentageUsedCounter++;
	if(iUpdatePercentageUsedCounter >= 5)
	{
		iAveragePercentageUsedByLogicCode = iTotalPercentageUsedByLogicCode / 5;
		iAveragePercentageUsedByDrawingCode = iTotalPercentageUsedByDrawingCode / 5;
		iTotalPercentageUsedByLogicCode = 0;
		iTotalPercentageUsedByDrawingCode = 0;
		iUpdatePercentageUsedCounter = 0;
	}

	//check if we are already over 1 sec
	if(lMicroSecsUsedSinceLastSec >= MICRO_SECONDS_PER_SECOND)
	{
		//update average stats
		iAverageMicroSecsBetweenFrames = iTotalMicroSecsBetweenFramesPerSec / iTempFramesPerSec;
		iAverageMicroSecsLossDueToSleep = iTotalMicroSecsLostDueToSleepPerSec / iTempFramesPerSec;
		iTotalMicroSecsBetweenFramesPerSec = 0;
		iTotalMicroSecsLostDueToSleepPerSec = 0;

		//update frames per second
		iFramesPerSec = iTempFramesPerSec;
		iTempFramesPerSec = 0;
		iMicroSecsValueFromLastSecond = iMicroTimeBeforeDrawingCode;
	}

	iTempFramesPerSec++;
}

void CFramesPerSecondCounter::CounterFunctionAfterLogicCode()
{
	iMicroTimeAfterLogicCode = CSystem::GetCurrentTimeInMicroSecondsNative();

	TInt64 lTimeUsedByLogicCode = iMicroTimeAfterLogicCode - iMicroTimeBeforeLogicCode;
	iTotalPercentageUsedByLogicCode += (lTimeUsedByLogicCode * 100) / iTotalMircoSecsPerFrame; //multiply be 100 to get percentage and to avoid underflow
}

void CFramesPerSecondCounter::CounterFunctionBeforeLogicCode()
{
	TInt64 lMicroTimeBeforeLogicCodeLastFrame = iMicroTimeBeforeLogicCode;
	iMicroTimeBeforeLogicCode = CSystem::GetCurrentTimeInMicroSecondsNative();

	TInt64 lMircoSecsDuringSleep = iMicroTimeBeforeLogicCode - iMicroTimeBeforeSleep;
	iTotalMicroSecsLostDueToSleepPerSec += lMircoSecsDuringSleep - iMicroRequestedSleepTime;
	iTotalMicroSecsBetweenFramesPerSec += iMicroTimeBeforeLogicCode - lMicroTimeBeforeLogicCodeLastFrame;
}

void CFramesPerSecondCounter::Draw(CGraphicsFont* aGraphicsFont)
{
	//going to use this Font
	SFont lDrawFont = aGraphicsFont->CreateFont(30);
	aGraphicsFont->SetFont(lDrawFont);
	TPoint lDrawLocation (1024,600);

#if FRAMES_PER_SECOND_SHOW_FPS
	//draw Fps
	aGraphicsFont->SetColour(KColourRed);
	CString* lFpsString = new CString("Fps : ");
	lFpsString->AppendNum(iFramesPerSec);
	aGraphicsFont->DrawStringToScreenHorizontally(TPoint(lDrawLocation.iX - aGraphicsFont->GetWidthOfString(lDrawFont, lFpsString),lDrawLocation.iY), lFpsString);
	delete lFpsString;
	lDrawLocation.iY -= 40;
#endif

#if FRAMES_PER_SECOND_SHOW_AVG_TIME_BETWEEN_FRAMES
	//draw Average MicroSeconds Between Frames
	aGraphicsFont->SetColour(KColourRed);
	CString* lTimeBetweenFramesString = new CString("");
	lTimeBetweenFramesString->AppendNum(iAverageMicroSecsBetweenFrames);
	aGraphicsFont->DrawStringToScreenHorizontally(TPoint(lDrawLocation.iX - aGraphicsFont->GetWidthOfString(lDrawFont, lTimeBetweenFramesString),lDrawLocation.iY), lTimeBetweenFramesString);
	delete lTimeBetweenFramesString;
	lDrawLocation.iY -= 40;
#endif

#if FRAMES_PER_SECOND_SHOW_AVG_LOSS_DUE_TO_SLEEP
	//draw Average Time Loss due to Sleep
	aGraphicsFont->SetColour(KColourRed);
	CString* lTimeLossDueToSleepString = new CString("");
	lTimeLossDueToSleepString->AppendNum(iAverageMicroSecsLossDueToSleep);
	aGraphicsFont->DrawStringToScreenHorizontally(TPoint(lDrawLocation.iX - aGraphicsFont->GetWidthOfString(lDrawFont, lTimeLossDueToSleepString),lDrawLocation.iY), lTimeLossDueToSleepString);
	delete lTimeLossDueToSleepString;
	lDrawLocation.iY -= 40;
#endif

#if FRAMES_PER_SECOND_SHOW_PERCENTAGE_USED_BY_LOGIC
	//draw Percentage used by Logic Code
	aGraphicsFont->SetColour(KColourDarkBlue);
	CString* lPercentageForLogicString = new CString("");
	lPercentageForLogicString->AppendNum(iAveragePercentageUsedByLogicCode);
	aGraphicsFont->DrawStringToScreenHorizontally(TPoint(lDrawLocation.iX - aGraphicsFont->GetWidthOfString(lDrawFont, lPercentageForLogicString),lDrawLocation.iY), lPercentageForLogicString);
	delete lPercentageForLogicString;
	lDrawLocation.iY -= 40;
#endif

#if FRAMES_PER_SECOND_SHOW_PERCENTAGE_USED_BY_DRAW
	//draw Percentage used by Draw Code
	aGraphicsFont->SetColour(KColourRed);
	CString* lPercentageForDrawString = new CString("");
	lPercentageForDrawString->AppendNum(iAveragePercentageUsedByDrawingCode);
	aGraphicsFont->DrawStringToScreenHorizontally(TPoint(lDrawLocation.iX - aGraphicsFont->GetWidthOfString(lDrawFont, lPercentageForDrawString),lDrawLocation.iY), lPercentageForDrawString);
	delete lPercentageForDrawString;
	lDrawLocation.iY -= 40;
#endif

#if FRAMES_PER_SECOND_SHOW_PERCENTAGE_USED_BY_LOGIC_AND_DRAW
	//draw Percentage used by Logic And Draw Code
	aGraphicsFont->SetColour(KColourGreen);
	CString* lTotalPercentageUsedString = new CString("");
	lTotalPercentageUsedString->AppendNum(iAveragePercentageUsedByLogicCode + iAveragePercentageUsedByDrawingCode);
	aGraphicsFont->DrawStringToScreenHorizontally(TPoint(lDrawLocation.iX - aGraphicsFont->GetWidthOfString(lDrawFont, lTotalPercentageUsedString),lDrawLocation.iY), lTotalPercentageUsedString);
	delete lTotalPercentageUsedString;
	lDrawLocation.iY -= 40;
#endif
}

void CFramesPerSecondCounter::SleepRemainingTimeInFrame()
{
	iMicroTimeBeforeSleep = CSystem::GetCurrentTimeInMicroSecondsNative();
	TInt64 lMicroTimeUsedInFrame = iMicroTimeBeforeSleep - iMicroTimeBeforeLogicCode;
	iMicroRequestedSleepTime = iTotalMircoSecsPerFrame - lMicroTimeUsedInFrame;

	if(iMicroRequestedSleepTime > 0)
	{
		//have some time left in the frame, due some sleeping
		CSystem::SleepForMicroSecondsNative(iMicroRequestedSleepTime);
	}
	else
	{
		iMicroRequestedSleepTime = 0; //no time to sleep
	}
}

TUint CFramesPerSecondCounter::GetFramesPerSecond()
{
	return iRequiredFramesPerSecond;
}

TInt CFramesPerSecondCounter::GetPercentageOfFrameUsed()
{
	return iAveragePercentageUsedByDrawingCode + iAveragePercentageUsedByLogicCode;
}

TInt CFramesPerSecondCounter::GetPercentageOfFrameTimeUsed(TUint64 aRelativeTime)
{
	TUint64 lCurrentTime = CSystem::GetCurrentTimeInMicroSecondsNative();

	TUint64 lTimeUsed = lCurrentTime - aRelativeTime;
	TInt lPercentageUsedOfFrame = (lTimeUsed * 100) / iTotalMircoSecsPerFrame;
	return lPercentageUsedOfFrame;
}

void CFramesPerSecondCounter::SetFramesPerSecond(TUint aFramesPerSecond)
{
	iRequiredFramesPerSecond = aFramesPerSecond;
	iTotalMircoSecsPerFrame = MICRO_SECONDS_PER_SECOND / aFramesPerSecond;
}


