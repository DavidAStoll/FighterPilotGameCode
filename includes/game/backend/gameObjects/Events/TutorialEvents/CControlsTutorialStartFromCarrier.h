/*
 ============================================================================
 Name		: ControlsTutorialStartFromCarrier.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialStartFromCarrier declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALSTARTFROMCARRIER_H
#define CONTROLSTUTORIALSTARTFROMCARRIER_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
// CLASS DECLARATION

/**
 *  CControlsTutorialStartFromCarrier
 * 
 */
class CControlsTutorialStartFromCarrier : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialStartFromCarrier();
	CControlsTutorialStartFromCarrier(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here

	};

#endif // CONTROLSTUTORIALSTARTFROMCARRIER_H
