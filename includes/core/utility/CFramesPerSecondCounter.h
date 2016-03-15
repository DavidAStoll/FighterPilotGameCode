/*
 * CFramesPerSecondCounter.h
 *
 *  Created on: Jul 17, 2012
 *      Author: dstoll
 */

#ifndef CFRAMESPERSECONDCOUNTER_H_
#define CFRAMESPERSECONDCOUNTER_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

class CGraphicsFont;

class CFramesPerSecondCounter
{
public:
	static CFramesPerSecondCounter* New(TUint aFramesPerSecond);
	virtual ~CFramesPerSecondCounter();

	//This method should be called after any Drawing is done
	void CounterFunctionAfterDrawingCode();
	//This method should be called before any drawing is done
	void CounterFunctionBeforeDrawingCode();
	//Should be called after all logic code has been run
	void CounterFunctionAfterLogicCode();
	//Should be called before anything else in a game loop, it will mark the new frame that is runnig
	void CounterFunctionBeforeLogicCode();
	//Draws the current stats to the screen
	void Draw(CGraphicsFont* aGraphicsFont);
	//return what currently the target value is in terms of Frames per Second
	TUint GetFramesPerSecond();
	//return total Average percentage used of Frame
	TInt GetPercentageOfFrameUsed();
	//returns how much percent has been used in the frame, relative to the given time
	TInt GetPercentageOfFrameTimeUsed(TUint64 aRelativeTime);
	//changes the Frames Per Second and thus the timing interval per frame
	void SetFramesPerSecond(TUint aFramesPerSecond);
	//Sleep for the remainging time in the frame
	void SleepRemainingTimeInFrame();

protected:
	CFramesPerSecondCounter(TUint aFramesPerSecond);

	//keep track of how many Frames we currently run for the previous and current one
	TUint iRequiredFramesPerSecond; //this how many frames we want to have per Sec
	TInt iFramesPerSec; //this tells us how many we actually had
	TInt iTempFramesPerSec; //copies its value to FramesPerSec, once the current second is up
	TInt iUpdatePercentageUsedCounter; //keeps track when we update the Percentage Used numbers

	//is a constant value that tells us how much MicroSecs we can use for a frame, if some are unused in a particular frame they will be used for the sleep
	TInt64 iTotalMircoSecsPerFrame;

	//different time values we keep track of
	TInt64 iMicroTimeBeforeLogicCode;
	TInt64 iMicroTimeAfterLogicCode;
	TInt64 iMicroTimeBeforeDrawingCode;
	TInt64 iMicroTimeAfterDrawingCode;
	TInt64 iMicroTimeBeforeSleep;
	TInt64 iMicroRequestedSleepTime;
	TInt64 iMicroSecsValueFromLastSecond;

	//values to keep track of percentage usage
	TInt iTotalPercentageUsedByLogicCode;
	TInt iTotalPercentageUsedByDrawingCode;
	TInt iAveragePercentageUsedByLogicCode;
	TInt iAveragePercentageUsedByDrawingCode;

	//keeps track of values gather over 1 sec
	TInt64 iTotalMicroSecsBetweenFramesPerSec;
	TInt64 iTotalMicroSecsLostDueToSleepPerSec; //keeps track of how many microsecs we lost due to sleeping and waking up
	TInt64 iAverageMicroSecsBetweenFrames;
	TInt64 iAverageMicroSecsLossDueToSleep;

};

#endif /* CFRAMESPERSECONDCOUNTER_H_ */
