/*
 * CDauntless.h
 *
 *  Created on: Aug 28, 2012
 *      Author: dstoll
 */

#ifndef CDAUNTLESS_H_
#define CDAUNTLESS_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftCannon20mm.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftMachineGun13mm.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftMachineGun8mm.h"
#include "includes/game/backend/basicClasses/CWeaponBomb.h"

class CBackGunMachineGun8mm;
class CBackGunDoubleMachineGun8mm;
class CFighterAI;

enum TDauntlessTextureNames
{
	EDauntlessNormalView = 0, EDauntlessFireSequence, EDauntlessGear, EDauntlessTurn , EDauntlessCrash
};

// CLASS DECLARATION

/**
 *  CDauntless
 *
 */
class CDauntless: public CAircraft
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CDauntless();

	/**
	 * Two-phased constructor.
	 */
	static CDauntless* New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation);

	static void InitilizeDauntless();
	static void RemoveDauntless();
	virtual void UpdateAircraft();
	virtual void BeforeTurnSettings();
	virtual void AfterTurnSettings();
	virtual void Draw();
	virtual void Fire1();
	virtual void Fire2();
	virtual TBool Fire1HasAmmunitionLeft();
	virtual TBool Fire2HasAmmunitionLeft();
	virtual CWeapon* GetPrimaryWeapon();
	virtual CWeapon* GetSecondaryWeapon();
	virtual TInt GetAmmunitionLeftOfSecondaryWeapon();
	virtual void RearmAircraft(); //call to this function will rearm all the airplanes weapons
	virtual void EquipWithNewSecondaryWeapon(TWeaponIdentifier aWeaponIdendifier);
	//repositions the object to the new location
	virtual void SetPosition(TPoint& aPoint);
	virtual void SetPosition(TPointIntFloat& aPoint);
	virtual void PlayTurnAnimation();
	virtual void DrawAdvancedHitBoxL();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	virtual void PlaneCrash();//causes secondary weapon to explode
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
	static SBoundingCollisionPoint* CollisionPointForFrontWheel;
	static SBoundingCollisionPoint* CollisionPointForBackWheel;
	CWeaponAircraftMachineGun13mm* iPrimaryWeaponMachineGun1;
	CWeaponAircraftMachineGun13mm* iPrimaryWeaponMachineGun2;
	CBackGunDoubleMachineGun8mm* iBackCannon;
	CWeaponBomb* iSecondaryWeapon;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CDauntless(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes);
	static void LoadDauntlessTexturesL();
	static void UnloadDauntlessTextures();
	static void CreateDauntlessCollisionPointsL();
	static void RemoveDauntlessCollisionPoints();
	void ConstructL();

};

#endif /* CDAUNTLESS_H_ */
