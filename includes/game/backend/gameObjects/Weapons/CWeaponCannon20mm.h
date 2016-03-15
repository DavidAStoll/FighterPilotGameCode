/*
 * CWeaponCannon20mm.h
 *
 *  Created on: Jun 21, 2013
 *      Author: dstoll
 */

#ifndef CWEAPONCANNON20MM_H_
#define CWEAPONCANNON20MM_H_

#include "includes/game/backend/basicClasses/CWeapon.h"
#include "includes/game/backend/gameObjects/Projectiles/CBulletCannon20mm.h"

class CWeaponCannon20mm : public CWeapon
{

public:
	/**
	 * Destructor.
	 */
	~CWeaponCannon20mm();

	//speed may be NULL
	static CWeaponCannon20mm* New(CGameObject* aOwner, TInt aXOffSet,TInt aYOffSet, TBool aHarmGroundUnits, const TInt& aAngle, const TIntFloat* aOwnerSpeed, TSoundFileId aGunSoundFileId);
	//for objects that are not gameObjects
	static CWeaponCannon20mm* New(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngel,  const TIntFloat* aOwnerSpeed, TSoundFileId aGunSoundFileId);


	virtual void Shoot();
	//some owners of the gun might want to modify the object a bit after it has been added to the game
	CBulletCannon20mm* ShootAndReturnBullet();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWeaponCannon20mm(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngle,  const TIntFloat* aOwnerSpeed, TSoundFileId aGunSoundFileId);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	const TIntFloat* iOwnerSpeed;
	TBool iHarmGroundUnits;//sometimes we want to create bullets that don't harm groundUnits
};

#endif /* CWEAPONCANNON20MM_H_ */
