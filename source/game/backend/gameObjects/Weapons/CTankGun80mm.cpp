/*
 ============================================================================
 Name		: TankGun80mm.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTankGun80mm implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CTankGun80mm::CTankGun80mm(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide) :
	CWeapon(TANKGUN88MM_DEFAULT_AMMUNITION, EWeaponIdentifierTankGun88mm, TANKGUN88MM_DEFAULT_LIMITED_AMMUNITION, aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide, TANKGUN88MM_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(ESoundFileId_Howitzer_In_Tank, GetRelativeWeaponPosition());
}

CTankGun80mm::~CTankGun80mm()
{
}

CTankGun80mm* CTankGun80mm::New(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide)
{
	CTankGun80mm* self = new CTankGun80mm(aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide);
	return self;
}

//--------------------------functions-------------------------

void CTankGun80mm::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		TInt lAngleOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(TANKGUN88MM_LOWER_ANGLE_RANDOM_VALUE, TANKGUN88MM_UPPER_ANGLE_RANDOM_VALUE);//give certain randomness to shot
		TIntFloat lExtraBulletSpeed = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(TANKGUN88MM_EXTRA_BULLET_SPEED_LOWER_VALUE, TANKGUN88MM_EXTRA_BULLET_SPEED_UPPER_VALUE);//give certain randomness to shot
		//need to make adjustments of the angle depending if we are reflected or not
		if(!(*iWeaponReflectedOverYAxis))//means it is looking left
			lAngleOffset = lAngleOffset * (-1); //actually lets  the projectile fire at a higher arc, since we start with 180 degrees

		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CTankShell80mm::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel + lAngleOffset, *iWeaponReflectedOverYAxis, lExtraBulletSpeed));
		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
	}
}
