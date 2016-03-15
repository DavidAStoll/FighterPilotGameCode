/*
 ============================================================================
 Name		: RestartLastTutorialScreenIfDiedEvent.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CRestartLastTutorialScreenIfDiedEvent declaration
 ============================================================================
 */

#ifndef RESTARTLASTTUTORIALSCREENIFDIEDEVENT_H
#define RESTARTLASTTUTORIALSCREENIFDIEDEVENT_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

class CAircraft;

// CLASS DECLARATION

/**
 *  CRestartLastTutorialScreenIfDiedEvent
 * 
 */
class CRestartLastTutorialScreenIfDiedEvent : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CRestartLastTutorialScreenIfDiedEvent();
	CRestartLastTutorialScreenIfDiedEvent(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfDies);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here

	};

#endif // RESTARTLASTTUTORIALSCREENIFDIEDEVENT_H
