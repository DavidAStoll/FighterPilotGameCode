/*
 ============================================================================
 Name		: ControlsTutorialLooping.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialLooping declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALLOOPING_H
#define CONTROLSTUTORIALLOOPING_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

class CAircraft;

enum ControlsTutorialLoopPosition
{
	EControlsTutorialLoopPositionUp90, EControlsTutorialLoopPositionUp0,EControlsTutorialLoopPositionUp270, EControlsTutorialLoopPositionUp180,
	EControlsTutorialLoopPositionDown90, EControlsTutorialLoopPositionDown0,EControlsTutorialLoopPositionDown270, EControlsTutorialLoopPositionDown180
};

// CLASS DECLARATION

/**
 *  CControlsTutorialLooping
 * 
 */
class CControlsTutorialLooping : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialLooping();
	CControlsTutorialLooping(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here

private:
	
	ControlsTutorialLoopPosition iCurrentLoopPosition;
	};

#endif // CONTROLSTUTORIALLOOPING_H
