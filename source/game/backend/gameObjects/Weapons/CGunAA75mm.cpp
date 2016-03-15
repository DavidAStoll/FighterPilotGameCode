/*
 ============================================================================
 Name		: GunAA75mm.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGunAA75mm implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CGunAA75mm.h"
#include "includes/game/backend/gameObjects/Projectiles/CExplosive75mmShell.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CGunAA75mm::CGunAA75mm(const TPointIntFloat* aLocation, const TInt* aAngel, TInt aXOffSet, TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide) :
	CWeapon(GUNAA75MM_DEFAULT_AMMUNITION, WeaponIdentifierAA75mmGun, GUNAA75MM_DEFAULT_LIMITED_AMMUNITION, false, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide, GUNAA75MM_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(ESoundFileId_Anti_Air_Craft_Medium, GetRelativeWeaponPosition());
}

CGunAA75mm::~CGunAA75mm()
{
}

CGunAA75mm* CGunAA75mm::New(const TPointIntFloat* aLocation, const TInt* aAngel, TInt aXOffSet, TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide)
{
	CGunAA75mm* self = new CGunAA75mm(aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide);
	return self;
}

//-------------- functions ----------------------//

void CGunAA75mm::Shoot(TInt aRange)
{
	if(iReadyToShoot)
	{
		iReadyToShoot = false;
		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CExplosive75mmShell::New(GetRelativeWeaponPosition(), *iAngel, *iWeaponReflectedOverYAxis, aRange));
	}
}

void CGunAA75mm::Shoot(TInt aRange, TInt aAngleOffset)
{
	if(iReadyToShoot)
	{
		iReadyToShoot = false;
		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CExplosive75mmShell::New(GetRelativeWeaponPosition(), *iAngel + aAngleOffset, *iWeaponReflectedOverYAxis, aRange));
	}
}
