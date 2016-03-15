/*
 ============================================================================
 Name		: TimeEvent.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTimeEvent declaration
 ============================================================================
 */

#ifndef TIMEEVENT_H
#define TIMEEVENT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

#define TIME_EVENT_INFINITE_TIME_EVENTS 999999

// CLASS DECLARATION

/**
 *  CTimeEvent
 * 
 */
class CTimeEvent : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTimeEvent();
	CTimeEvent(TEventIdentifier aEventId, TInt aPeriodInSecs,TInt aInitialExtraDelayInSec, TInt aNumberOfRepeatEvents);
	//this method is called every frame and calls Run() method every time we waited one period. In that case 
	//it returns the boolean value returned by that method depending if that event should only happen once or several times during
	//its life time
	virtual TBool CheckIfEventConditionIsMet();
	virtual void Run()=0;
	void SetNumberRepeatEventsLeft(TInt aValue);
	void TimeEvenSaveOnDisk(CFileWriteStream &aOutputStream);
	void TimeEventLoadFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	
	TInt iPeriodInFrames;
	TInt iFramesLeft;
	TInt iNumberOfRepeatEventsLeft;

	};

#endif // TIMEEVENT_H
