/*
 ============================================================================
 Name		: GunAA88mm.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGunAA88mm implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CGunAA88mm.h"
#include "includes/game/backend/gameObjects/Projectiles/CExplosive88mmShell.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CGunAA88mm::CGunAA88mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide) :
	CWeapon(GUNAA88MM_DEFAULT_AMMUNITION, WeaponIdentifierAA88mmGun, GUNAA88MM_DEFAULT_LIMITED_AMMUNITION, false, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide, GUNAA88MM_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(ESoundFileId_Black_Powder_Cannon, GetRelativeWeaponPosition());
}

CGunAA88mm::~CGunAA88mm()
{
}

CGunAA88mm* CGunAA88mm::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide)
{
	CGunAA88mm* self = new CGunAA88mm(aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide);
	return self;
}

//------------------ functions ---------------------//

void CGunAA88mm::Shoot(TInt aRange)
{
	if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CExplosive88mmShell::New(GetRelativeWeaponPosition(), *iAngel, *iWeaponReflectedOverYAxis, aRange));
	}
}

void CGunAA88mm::Shoot(TInt aRange, TInt aAngleOffset)
{
	if(iReadyToShoot)
	{
		iReadyToShoot = false;

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CExplosive88mmShell::New(GetRelativeWeaponPosition(), *iAngel + aAngleOffset, *iWeaponReflectedOverYAxis, aRange));
	}
}
