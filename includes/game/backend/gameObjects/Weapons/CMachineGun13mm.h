/*
 ============================================================================
 Name		: Gun13mm.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMachineGun13mm declaration
 ============================================================================
 */

#ifndef MACHINEGUN13mm_H
#define MACHINEGUN13mm_H

#include "includes/game/backend/basicClasses/CWeapon.h"
#include "includes/game/backend/gameObjects/Projectiles/CBullet13mm.h"

// CLASS DECLARATION

class CMachineGun13mm : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CMachineGun13mm();

	//speed may be NULL
	static CMachineGun13mm* New(CGameObject* aOwner, TInt aXOffSet,TInt aYOffSet, TBool aHarmGroundUnits, const TInt& aAngle, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId);
	//for objects that are not gameObjects
	static CMachineGun13mm* New(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngel,  const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId);


	virtual void Shoot();
	//some owners of the gun might want to modify the object a bit after it has been added to the game
	CBullet13mm* ShootAndReturnBullet();
	CBullet13mm* ShootAndReturnBulletWithoutReflection();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMachineGun13mm(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngle,  const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	const TIntFloat* iOwnerSpeed;
	TBool iHarmGroundUnits;//sometimes we want to create bullets that don't harm groundUnits

	};

#endif // MACHINEGUN13mm_H
