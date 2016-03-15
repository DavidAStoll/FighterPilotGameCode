/*
 ============================================================================
 Name		: ClockTimerEvent.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CClockTimerEvent declaration
 ============================================================================
 */

#ifndef CLOCKTIMEREVENT_H
#define CLOCKTIMEREVENT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

class CClockTimer;

// CLASS DECLARATION

/**
 *  CClockTimerEvent
 * 
 */
class CClockTimerEvent : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CClockTimerEvent();
	CClockTimerEvent(TInt aSecondsUntilTimerExpires);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	void ConstructL();
	TInt iSecondsUntilChecking;
	TInt iFramesLeftUntilNextSecond;
	CClockTimer* iClockTimer;

	};

#endif // CLOCKTIMEREVENT_H
