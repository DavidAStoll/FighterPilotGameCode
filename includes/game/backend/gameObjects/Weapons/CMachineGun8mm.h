/*
 * CMachineGun8mm.h
 *
 *  Created on: Jun 17, 2013
 *      Author: dstoll
 */

#ifndef CMACHINEGUN8MM_H_
#define CMACHINEGUN8MM_H_

#include "includes/game/backend/basicClasses/CWeapon.h"
#include "includes/game/backend/gameObjects/Projectiles/CBullet8mm.h"

class CMachineGun8mm : public CWeapon
	{

public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CMachineGun8mm();

	static CMachineGun8mm* New(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, const TInt& aAngel, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId);
	virtual void Shoot();
	CBullet8mm* ShootAndReturnBullet();

private:

	const TIntFloat* iOwnerSpeed;
  CMachineGun8mm(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, const TInt& aAngel, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId);
	};

#endif /* CMACHINEGUN8MM_H_ */
