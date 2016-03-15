/*
 ============================================================================
 Name		: CreateMitchellToAttackTargetClockTimer.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCreateMitchellToAttackTargetClockTimer declaration
 ============================================================================
 */

#ifndef CREATEMITCHELLTOATTACKTARGETCLOCKTIMER_H
#define CREATEMITCHELLTOATTACKTARGETCLOCKTIMER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

class CClockTimer;
class CGameObject;

#define CREATE_MITCHELL_TO_ATTACK_TARGET_EVENT_TIMER_X_LOCATION 90
#define CREATE_MITCHELL_TO_ATTACK_TARGET_EVENT_TIMER_Y_LOCATION 570

// CLASS DECLARATION

/**
 *  CCreateMitchellToAttackTargetClockTimer
 * 
 */
class CCreateMitchellToAttackTargetClockTimer : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CCreateMitchellToAttackTargetClockTimer();

	static CCreateMitchellToAttackTargetClockTimer* New(CGameObject* aTarget,TInt aSecondsUntilSpawnEvent);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CCreateMitchellToAttackTargetClockTimer(CGameObject* aTarget,TInt aSecondsUntilSpawnEvent);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	TInt iSecondsUntilSpawnEvent;
	TInt iFramesLeftUntilNextSecond;
	CGameObject* iTargetObject;
	CClockTimer* iClockTimer;

	};

#endif // CREATEMITCHELLTOATTACKTARGETCLOCKTIMER_H
