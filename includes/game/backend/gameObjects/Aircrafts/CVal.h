/*
 ============================================================================
 Name		: Val.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CVal declaration
 ============================================================================
 */

#ifndef VAL_H
#define VAL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CWeaponBomb.h"

class CBackGunMachineGun8mm;
class CWeaponAircraftMachineGun8mm;
class CFighterAI;


class CVal : public CAircraft
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CVal();

	/**
	 * Two-phased constructor.
	 */
	static CVal* New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation);
	
	static void InitilizeVal();
	static void RemoveVal();
	virtual void UpdateAircraft();
	virtual void AfterTurnSettings();
	virtual void BeforeTurnSettings();
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

private:

	virtual void PlaneCrash();//causes secondary weapon to explode
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
	static SBoundingCollisionPoint* CollisionPointForFrontWheel;
	static SBoundingCollisionPoint* CollisionPointForBackWheel;
	CWeaponAircraftMachineGun8mm* iFrontMachineGun1;
	CWeaponAircraftMachineGun8mm* iFrontMachineGun2;
	CBackGunMachineGun8mm* iBackCannon;
	CWeaponBomb* iSecondaryWeapon;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CVal(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes);
	static void LoadValTexturesL();
	static void UnloadValTextures();
	static void CreateValCollisionPointsL();
	static void RemoveValCollisionPoints();
	void ConstructL();

	};

#endif // VAL_H
