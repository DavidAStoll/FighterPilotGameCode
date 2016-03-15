/*
 ============================================================================
 Name		: ControlsTutorialSlowDown.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialSlowDown declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALSLOWDOWN_H
#define CONTROLSTUTORIALSLOWDOWN_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CControlsTutorialSlowDown
 * 
 */
class CControlsTutorialSlowDown : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialSlowDown();
	CControlsTutorialSlowDown(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfPlaneCrashed, TTutorialMethodCallIds TTutorialMethodCallIfPlaneDidNotCrash);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:
	
	TBool iSpeedUpAgain;
	TBool iSlowedDown;
	TBool iPlayerDied;
	TTutorialMethodCallIds iMethodCallIfNotCrashed;



	};

#endif // CONTROLSTUTORIALSLOWDOWN_H
