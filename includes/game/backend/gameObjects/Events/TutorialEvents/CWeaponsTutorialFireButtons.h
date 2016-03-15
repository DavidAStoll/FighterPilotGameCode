/*
 ============================================================================
 Name		: WeaponsTutorialFireButtons.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialFireButtons declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALFIREBUTTONS_H
#define WEAPONSTUTORIALFIREBUTTONS_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialFireButtons
 * 
 */
class CWeaponsTutorialFireButtons : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialFireButtons();
	CWeaponsTutorialFireButtons(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in he
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
protected:
	TBool iFire1ButtonPressed;
	TBool iFire2ButtonPressed;
	};

#endif // WEAPONSTUTORIALFIREBUTTONS_H
