/*
 ============================================================================
 Name		: WeaponsTutorialAttackBomber.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialAttackBomber declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALATTACKBOMBER_H
#define WEAPONSTUTORIALATTACKBOMBER_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialAttackBomber
 * 
 */
class CWeaponsTutorialAttackBomber : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialAttackBomber();
	CWeaponsTutorialAttackBomber(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he
	};

#endif // WEAPONSTUTORIALATTACKBOMBER_H
