/*
 ============================================================================
 Name		: Mitchell.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMitchell declaration
 ============================================================================
 */

#ifndef MITCHELL_H
#define MITCHELL_H

// INCLUDES
// INCLUDES
#include "includes/game/backend/basicClasses/CAircraft.h"

// CLASS DECLARATION
class CBackGunMachineGun8mm;
class CAircraftSmokeCreator;
class CWeaponBomb;

/**
 *  CMitchell
 * 
 */
class CMitchell : public CAircraft
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CMitchell();

	/**
	 * Two-phased constructor.
	 */
	static CMitchell* New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation);

	static void InitilizeMitchell();
	static void RemoveMitchell();
	virtual void UpdateAircraft();
	virtual void AfterTurnSettings();
	virtual void BeforeTurnSettings();
	virtual void Draw();
	virtual void Fire1();
	virtual void Fire2();
	//used to check if the Fire1(Primary) and Fire2(Secondary) weapon has still ammo
	virtual TBool Fire1HasAmmunitionLeft();
	virtual TBool Fire2HasAmmunitionLeft();
	virtual CWeapon* GetPrimaryWeapon();
	virtual CWeapon* GetSecondaryWeapon();
	void AssignBombingMission(SHeavyBombingMission aMission);
	CHeavyBomberAI* GetHeavyBomberAI();
	virtual void PlayTurnAnimation();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	CMitchell(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void PlaneCrash(); //causes secondary weapon to explode
	//functions intilize global data shared by all dauntless objects, such Bounding Collision points and textures
	static void LoadMitchellTexturesL();
	static void UnloadMitchellTextures();
	static void CreateMitchellCollisionPointsL();
	static void RemoveMitchellCollisionPoints();

	static SBoundingCollisionPoint* CollisionPoints; //holds the bounding points used for collision
	CBackGunMachineGun8mm* iFrontCannon;
	CBackGunMachineGun8mm* iTopCannon;
	CBackGunMachineGun8mm* iBackCannon;
	CBackGunMachineGun8mm* iBottomCannon;
	CAircraftSmokeCreator* iSmokeCreatorEngine1;
	CAircraftSmokeCreator* iSmokeCreatorEngine2;
	CWeaponBomb* iFirstHeavyBombWeapon;
	CWeaponBomb* iFirstMediumBombWeapon;
	CWeaponBomb* iSecondHeavyBombWeapon;
	CWeaponBomb* iSecondMediumBombWeapon;
	CHeavyBomberAI* iBombingAI;

	};

#endif // MITCHELL_H
