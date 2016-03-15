/*
 ============================================================================
 Name		: WeaponsTutorialLoseIfFailedToKillAnything.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialLoseIfFailedToKillAnything declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALLOSEIFFAILEDTOKILLANYTHING_H
#define WEAPONSTUTORIALLOSEIFFAILEDTOKILLANYTHING_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialLoseIfFailedToKillAnything
 * 
 */
class CWeaponsTutorialLoseIfFailedToKillAnything : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialLoseIfFailedToKillAnything();
	CWeaponsTutorialLoseIfFailedToKillAnything(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he
	};

#endif // WEAPONSTUTORIALLOSEIFFAILEDTOKILLANYTHING_H
