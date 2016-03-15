/*
 ============================================================================
 Name		: WeaponsTutorialLoseIfAllAmericanSoldiersDied.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialLoseIfAllAmericanSoldiersDied declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALLOSEIFALLAMERICANSOLDIERSDIED_H
#define WEAPONSTUTORIALLOSEIFALLAMERICANSOLDIERSDIED_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialLoseIfAllAmericanSoldiersDied
 * 
 */
class CWeaponsTutorialLoseIfAllAmericanSoldiersDied : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialLoseIfAllAmericanSoldiersDied();
	CWeaponsTutorialLoseIfAllAmericanSoldiersDied(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he
	};

#endif // WEAPONSTUTORIALLOSEIFALLAMERICANSOLDIERSDIED_H
