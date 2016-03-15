/*
 * CWeapon.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CWEAPON_H_
#define CWEAPON_H_

// CLASS DECLARATION
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/core/sound/CSoundObject.h"

/**
 *  CWeapon
 *
 */

enum TWeaponIdentifier
{
	EWeaponIdentifierRifle,
	EWeaponIdentifier20mmCannon,
	EWeaponIdentifier13mmMachineGun,
	EWeaponIdentifier8mmMachineGun,
	WeaponIdentifierAA88mmGun,
	EWeaponIdentifier1000kgBomb,
	EWeaponIdentifier500kgBomb,
	EWeaponIdentifier100kgBomb,
	EWeaponIdentifierClusterBomb,
	WeaponIdentifierAA75mmGun,
	EWeaponStandardRocket,
	EWeaponIdentifierTorpedo,
	EWeaponIdentifierGrenadeLauncher,
	EWeaponIdentifierTankGun88mm,
	WeaponIdentifierAirplaneSmokeCreator,
	WeaponIdentifierGeneralSmokeCreator,
	WeaponIdentifierBackGunMachineGun8mm,
	WeaponIdentifierBackGunDoubleMachineGun8mm,
	WeaponIdentifierBackGunDoubleMachineGun13mm,
	WeaponIdentifierTankAAGun,
	WeaponIdentifierShipDualMachineGun,
	WeaponIdentifierShipDual75mmGun,
	WeaponIdentifierShip7InchGun,
	WeaponIdentifierShip14InchGun,
	WeaponIdentifierExplosive7InchGun,
	WeaponIdentifierExplosive14InchGun,
	EWeaponIdentifierAircraftCannon20mm,
	EWeaponIdentifierAircraftMachineGun13mm,
	EWeaponIdentifierAircraftMachineGun8mm
};

class CWeapon
{
public:
	// Constructors and destructor


	/**
	 * Destructor.
	 */
	virtual ~CWeapon();

	virtual void Shoot(); //shoots at the specific locations, needs to be implemented by child class
	virtual void Shoot(TInt aRange); //shoot method for Projectiles that explode at a certain range, for explosive ammunition
	virtual void Shoot(TInt aRange, TInt aAngleOffset);//for weapons that want to shoot with a certain random angel offset
	virtual void ShootWithSpeed(TIntFloat aSpeed); //some weapons might need to dictate at what speed the projectile has to fly
	virtual void Draw();// some weapons might want to draw them self
	virtual void Update();//some weapons might need an update function

	//implemented by this class
	TInt GetAmmunitionLeft();//returns how much ammunition is left
	TBool GetIsReadyToShoot(); //return is weapon ready to shoot or not
	TInt GetMaxAmmunitionLeft();//returns how much ammunition is left
	TBool HasLimitedAmmunition(); // returns if the gun has limited ammunition
	TBool IsDisabled();
	virtual void SetDisabled(TBool aValue);//disables or enables the gun
	virtual void SetAmmunitionToValue(TInt aAmount);//resets the ammunition to its default
	virtual void ResetAmmunitionToMaxValue();
	TPointIntFloat GetRelativeWeaponPosition();
	TPointIntFloat GetRelativeWeaponPositionWithoutReflection();
	TPointIntFloat GetRelativeWeaponPosition(TInt aExtraXOffset, TInt aExtraYOffset);
	TPointIntFloat GetRelativeWeaponPositionWithAngel(TInt& aAngle);
	TWeaponIdentifier GetWeaponIdentifier();
	void WeaponObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void WeaponObjectLoadContentFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	virtual void UpdateAdditionalWeapons(); //might include additionals or compound weapons

	TInt iAmmunitionLeft;
	TInt iMaxAmmunitionLeft;
	TInt iCurrentFrame;
	TInt iFramesBetweenFire;
	TBool iReadyToShoot;
	TBool ihasLimitedAmmunition;
	TBool iIsDisabled;
	CSoundObject* iSoundObject;
	const TPointIntFloat* iCoordinate;//pointer to the object's location owned
	const TInt* iAngel;
	TInt iXOffset;//relative position to iCoordinate, dictates where the projectiles will be created
	TInt iYOffset;//relative position to iCoordinate
	const TBool* iWeaponReflectedOverYAxis;//used if an object reflects over the Y axis, so that the weapon is pointing correctly as well
	const TConflictSide* iConflictSide;
	TWeaponIdentifier iWeaponIdentifier;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWeapon(TInt aAmmunition, TWeaponIdentifier aWeaponIdentifier, TBool aHasLimitedAmmunition, TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aFramesBetweenFire);
};


#endif /* CWEAPON_H_ */
