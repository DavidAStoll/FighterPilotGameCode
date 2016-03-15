/*
 ============================================================================
 Name		: Explosive14InchGun.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive14InchGun implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CExplosive14InchGun::CExplosive14InchGun(TBool aDisabled, const TPointIntFloat* aLocation,  const TInt* aAngel, TInt aXOffSet, TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide) :
	CWeapon(INCH_14_GUN_DEFAULT_AMMUNITION, WeaponIdentifierExplosive14InchGun, INCH_14_GUN_DEFAULT_LIMITED_AMMUNITION, aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide, GFRAMES_PER_SECOND_BACKEND)
{
	iSoundObject = CSoundObject::New(ESoundFileId_Battle_Ship_Cannon, GetRelativeWeaponPosition());
}

CExplosive14InchGun::~CExplosive14InchGun()
{
}

CExplosive14InchGun* CExplosive14InchGun::New(TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel, TInt aXOffSet, TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide)
{
	CExplosive14InchGun* self = new CExplosive14InchGun(aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide);
	return self;
}

//--------------------------functions-------------------------

void CExplosive14InchGun::ShootWithSpeed(TIntFloat aSpeed)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if (iReadyToShoot)
	{
		iReadyToShoot = false;

		TInt lAngleOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(INCH_14_GUN_LOWER_ANGLE_RANDOM_VALUE, INCH_14_GUN_UPPER_ANGLE_RANDOM_VALUE);//give certain randomness to shot
		//need to make adjustments of the angle depending if we are reflected or not
		if(!(*iWeaponReflectedOverYAxis))//means it is looking left
			lAngleOffset = lAngleOffset * (-1); //actually lets  the projectile fire at a higher arc, since we start with 180 degrees
		TIntFloat lExtraBulletSpeed = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(INCH_14_GUN_EXTRA_BULLET_SPEED_LOWER_VALUE, INCH_14_GUN_EXTRA_BULLET_SPEED_UPPER_VALUE);//give certain randomness to shot
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CExplosive14InchShell::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel + lAngleOffset, *iWeaponReflectedOverYAxis, aSpeed + lExtraBulletSpeed));
		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
	}
}
