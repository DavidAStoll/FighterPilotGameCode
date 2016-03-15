/*
 * CWeaponCannon20mm.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponCannon20mm.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponCannon20mm::CWeaponCannon20mm(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngle,  const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId) :
	CWeapon(CANNON_20MM_DEFAULT_AMMUNITION, EWeaponIdentifier20mmCannon, CANNON_20MM_DEFAULT_AMMUNITION, false, aLocation, &aAngle, aXOffset, aYOffset, aReflected, aConflictSide, CANNON_20MM_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(aMachineGunSoundFileId, GetRelativeWeaponPosition());
	iOwnerSpeed = aOwnerSpeed;
	iHarmGroundUnits = aHarmGroundUnits;
}

CWeaponCannon20mm::~CWeaponCannon20mm()
{
}

CWeaponCannon20mm* CWeaponCannon20mm::New(CGameObject* aOwner, TInt aXOffSet,TInt aYOffSet, TBool aHarmGroundUnits, const TInt& aAngle, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId)
{
	CWeaponCannon20mm* self = new CWeaponCannon20mm(&aOwner->GetCurrentPosition(), &aOwner->IsReflected(), &aOwner->GetConflictSide(), aXOffSet, aYOffSet, aHarmGroundUnits, aAngle, aOwnerSpeed, aMachineGunSoundFileId);
	return self;
}

CWeaponCannon20mm* CWeaponCannon20mm::New(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngel,  const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId)
{
	CWeaponCannon20mm* self = new CWeaponCannon20mm(aLocation, aReflected, aConflictSide, aXOffset, aYOffset, aHarmGroundUnits, aAngel, aOwnerSpeed, aMachineGunSoundFileId);
	return self;
}

//--------------------------functions-------------------------

//stub function
void CWeaponCannon20mm::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		if(iOwnerSpeed)
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBulletCannon20mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, *iOwnerSpeed, iHarmGroundUnits));
		else
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBulletCannon20mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, iHarmGroundUnits));
	}
}

CBulletCannon20mm* CWeaponCannon20mm::ShootAndReturnBullet()
{
	CBulletCannon20mm* lReturnBullet = NULL;

	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		if(iOwnerSpeed)
			lReturnBullet = CBulletCannon20mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, *iOwnerSpeed, iHarmGroundUnits);
		else
			lReturnBullet = CBulletCannon20mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, iHarmGroundUnits);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lReturnBullet);
	}
	return lReturnBullet;
}

