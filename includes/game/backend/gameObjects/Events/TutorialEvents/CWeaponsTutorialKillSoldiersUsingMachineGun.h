/*
 ============================================================================
 Name		: WeaponsTutorialKillSoldiersUsingMachineGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialKillSoldiersUsingMachineGun declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALKILLSOLDIERSUSINGMACHINEGUN_H
#define WEAPONSTUTORIALKILLSOLDIERSUSINGMACHINEGUN_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
// CLASS DECLARATION

/**
 *  CWeaponsTutorialKillSoldiersUsingMachineGun
 * 
 */
class CWeaponsTutorialKillSoldiersUsingMachineGun : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialKillSoldiersUsingMachineGun();
	CWeaponsTutorialKillSoldiersUsingMachineGun(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he
	};

#endif // WEAPONSTUTORIALKILLSOLDIERSUSINGMACHINEGUN_H
