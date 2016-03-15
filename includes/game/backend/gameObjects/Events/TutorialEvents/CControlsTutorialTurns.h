/*
 ============================================================================
 Name		: ControlsTutorialTurns.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialTurns declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALTURNS_H
#define CONTROLSTUTORIALTURNS_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"


/**
 *  CControlsTutorialTurns
 * 
 */
class CControlsTutorialTurns : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialTurns();
	CControlsTutorialTurns(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	TBool iHasDoneRightTurn;

	/**
	 * Constructor for performing 1st stage construction
	 */



	};

#endif // CONTROLSTUTORIALTURNS_H
