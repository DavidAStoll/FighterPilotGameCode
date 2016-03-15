/*
 ============================================================================
 Name		: WeaponsTutorialAttackFighter.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialAttackFighter declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALATTACKFIGHTER_H
#define WEAPONSTUTORIALATTACKFIGHTER_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialAttackFighter
 * 
 */
class CWeaponsTutorialAttackFighter : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialAttackFighter();
	CWeaponsTutorialAttackFighter(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he

	};

#endif // WEAPONSTUTORIALATTACKFIGHTER_H
