/*
 ============================================================================
 Name		: WeaponsTutorialAttackShip.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialAttackShip declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALATTACKSHIP_H
#define WEAPONSTUTORIALATTACKSHIP_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialAttackShip
 * 
 */
class CWeaponsTutorialAttackShip : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialAttackShip();
	CWeaponsTutorialAttackShip(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he

	};

#endif // WEAPONSTUTORIALATTACKSHIP_H
