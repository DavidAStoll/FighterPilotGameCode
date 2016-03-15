/*
 ============================================================================
 Name		: ControlsTutorialSpeedUp.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialSpeedUp declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALSPEEDUP_H
#define CONTROLSTUTORIALSPEEDUP_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CControlsTutorialSpeedUp
 * 
 */
class CControlsTutorialSpeedUp : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialSpeedUp();

	CControlsTutorialSpeedUp(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here
	
	};

#endif // CONTROLSTUTORIALSPEEDUP_H
