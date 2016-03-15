/*
 ============================================================================
 Name		: ControlsTutorialMoveToEndOfMap.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialMoveToEndOfMap declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALMOVETOENDOFMAP_H
#define CONTROLSTUTORIALMOVETOENDOFMAP_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CControlsTutorialMoveToEndOfMap
 * 
 */
class CControlsTutorialMoveToEndOfMap : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialMoveToEndOfMap();
	CControlsTutorialMoveToEndOfMap(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here

	};

#endif // CONTROLSTUTORIALMOVETOENDOFMAP_H
