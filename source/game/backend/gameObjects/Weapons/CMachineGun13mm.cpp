/*
 ============================================================================
 Name		: Gun13mm.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMachineGun13mm implementation
 ============================================================================

 */

#include "includes/game/backend/gameObjects/Weapons/CMachineGun13mm.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CMachineGun13mm::CMachineGun13mm(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngle,  const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId) :
	CWeapon(MACHINEGUN_13MM_DEFAULT_AMMUNITION, EWeaponIdentifier13mmMachineGun, MACHINEGUN_13MM_DEFAULT_LIMITED_AMMUNITION, false, aLocation, &aAngle, aXOffset, aYOffset, aReflected, aConflictSide, MACHINEGUN_13MM_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(aMachineGunSoundFileId, GetRelativeWeaponPosition());
	iOwnerSpeed = aOwnerSpeed;
	iHarmGroundUnits = aHarmGroundUnits;
}

CMachineGun13mm::~CMachineGun13mm()
{
}

CMachineGun13mm* CMachineGun13mm::New(CGameObject* aOwner, TInt aXOffSet,TInt aYOffSet, TBool aHarmGroundUnits, const TInt& aAngle, const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId)
{
	CMachineGun13mm* self = new CMachineGun13mm(&aOwner->GetCurrentPosition(), &aOwner->IsReflected(), &aOwner->GetConflictSide(), aXOffSet, aYOffSet, aHarmGroundUnits, aAngle, aOwnerSpeed, aMachineGunSoundFileId);
	return self;
}

CMachineGun13mm* CMachineGun13mm::New(const TPointIntFloat* aLocation, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aXOffset, TInt aYOffset, TBool aHarmGroundUnits, const TInt& aAngel,  const TIntFloat* aOwnerSpeed, TSoundFileId aMachineGunSoundFileId)
{
	CMachineGun13mm* self = new CMachineGun13mm(aLocation, aReflected, aConflictSide, aXOffset, aYOffset, aHarmGroundUnits, aAngel, aOwnerSpeed, aMachineGunSoundFileId);
	return self;
}

//--------------------------functions-------------------------

//stub function
void CMachineGun13mm::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		if(iOwnerSpeed)
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBullet13mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, *iOwnerSpeed, iHarmGroundUnits));
		else
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBullet13mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, iHarmGroundUnits));
	}
}

CBullet13mm* CMachineGun13mm::ShootAndReturnBulletWithoutReflection()
{
	CBullet13mm* lReturnBullet = NULL;

	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPositionWithoutReflection());
		if(iOwnerSpeed)
			lReturnBullet = CBullet13mm::New(GetRelativeWeaponPositionWithoutReflection(), *iConflictSide, *iAngel, false, *iOwnerSpeed, iHarmGroundUnits);
		else
			lReturnBullet = CBullet13mm::New(GetRelativeWeaponPositionWithoutReflection(), *iConflictSide, *iAngel, false, iHarmGroundUnits);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lReturnBullet);
	}
	return lReturnBullet;
}

CBullet13mm* CMachineGun13mm::ShootAndReturnBullet()
{
	CBullet13mm* lReturnBullet = NULL;

	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		if(iOwnerSpeed)
			lReturnBullet = CBullet13mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, *iOwnerSpeed, iHarmGroundUnits);
		else
			lReturnBullet = CBullet13mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel, *iWeaponReflectedOverYAxis, iHarmGroundUnits);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lReturnBullet);
	}
	return lReturnBullet;
}
