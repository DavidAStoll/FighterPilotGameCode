/*
 ============================================================================
 Name		: ControlsTutorialLandOnCarrier.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialLandOnCarrier declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALLANDONCARRIER_H
#define CONTROLSTUTORIALLANDONCARRIER_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
// CLASS DECLARATION

/**
 *  CControlsTutorialLandOnCarrier
 * 
 */
class CControlsTutorialLandOnCarrier : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialLandOnCarrier();
	CControlsTutorialLandOnCarrier(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here

	};

#endif // CONTROLSTUTORIALLANDONCARRIER_H
