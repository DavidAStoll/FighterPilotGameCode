/*
 * CMachineGun8mm.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CMachineGun8mm::CMachineGun8mm(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, const TInt& aAngel, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId) :
	CWeapon(MACHINEGUN_8MM_DEFAULT_AMMUNITION, EWeaponIdentifier8mmMachineGun, MACHINEGUN_8MM_DEFAULT_LIMITED_AMMUNITION, false, aLocation, &aAngel, aXOffset, aYOffset, aReflected, aConflictSide, MACHINEGUN_8MM_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(ESoundFileId_Rifle_Gun, GetRelativeWeaponPosition());
	iOwnerSpeed = aOwnerSpeed;
}

CMachineGun8mm::~CMachineGun8mm()
{
}

CMachineGun8mm* CMachineGun8mm::New(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, const TInt& aAngel, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId)
{
	CMachineGun8mm* self = new CMachineGun8mm(aLocation, aReflected, aConflictSide, aXOffset, aYOffset, aAngel, aOwnerSpeed, aMachineGunSoundFileId);
	return self;
}

//--------------------------functions-------------------------

void CMachineGun8mm::Shoot()
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
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBullet8mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, *iOwnerSpeed));
		else
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBullet8mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis));
	}
}

CBullet8mm* CMachineGun8mm::ShootAndReturnBullet()
{
	CBullet8mm* lBullet = NULL;

	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		if(iOwnerSpeed)
			lBullet = CBullet8mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, *iOwnerSpeed);
		else
			lBullet = CBullet8mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lBullet);
	}

	return lBullet;
}
