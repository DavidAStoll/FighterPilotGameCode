/*
 ============================================================================
 Name		: WeaponsTutorialBomb1000kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialBomb1000kg declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALBOMB1000KG_H
#define WEAPONSTUTORIALBOMB1000KG_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
// CLASS DECLARATION

/**
 *  CWeaponsTutorialBomb1000kg
 * 
 */
class CWeaponsTutorialBomb1000kg : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialBomb1000kg();
	CWeaponsTutorialBomb1000kg(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he
	};

#endif // WEAPONSTUTORIALBOMB1000KG_H
