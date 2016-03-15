/*
 ============================================================================
 Name		: WeaponsTutorialDiveBombingKill.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialDiveBombingKill declaration
 ============================================================================
 */

#ifndef WEAPONSTUTORIALDIVEBOMBINGKILL_H
#define WEAPONSTUTORIALDIVEBOMBINGKILL_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"

// CLASS DECLARATION

/**
 *  CWeaponsTutorialDiveBombingKill
 * 
 */
class CWeaponsTutorialDiveBombingKill : public CBasicAircraftNeededTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponsTutorialDiveBombingKill();
	CWeaponsTutorialDiveBombingKill(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess, TTutorialMethodCallIds aMethodCallIfFailed);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:
	TBool iSuccess;
	TBool iBombReleasedDuringDive;
	TTutorialMethodCallIds iIfFailed;
	};

#endif // WEAPONSTUTORIALDIVEBOMBINGKILL_H
