/*
 ============================================================================
 Name		: GrenadeLauncher.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGrenadeLauncher implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CGrenadeLauncher::CGrenadeLauncher(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide) :
	CWeapon(GRENADELAUNCHER_DEFAULT_AMMUNITION, EWeaponIdentifierGrenadeLauncher, GRENADELAUNCHER_DEFAULT_LIMITED_AMMUNITION, aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide, GRENADELAUNCHER_FRAMES_BETWEEN_FIRE)
{
	// No implementation required
	iSoundObject = CSoundObject::New(ESoundFileId_Rocket_Launcher, GetRelativeWeaponPosition());
}

CGrenadeLauncher::~CGrenadeLauncher()
{
}

CGrenadeLauncher* CGrenadeLauncher::New(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide)
{
	CGrenadeLauncher* self = new CGrenadeLauncher(aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide);
	return self;
}


//--------------------------functions-------------------------

void CGrenadeLauncher::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;

		TInt lAngleOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(GRENADELAUNCHER_LOWER_ANGLE_RANDOM_VALUE, GRENADELAUNCHER_UPPER_ANGLE_RANDOM_VALUE);//give certain randomness to shot
		//need to make adjustments of the angle depending if we are reflected or not
		if(!(*iWeaponReflectedOverYAxis))//means it is looking left
			lAngleOffset = lAngleOffset * (-1); //actually lets  the projectile fire at a higher arc, since we start with 180 degrees

		TIntFloat lExtraBulletSpeed = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(GRENADELAUNCHER_EXTRA_BULLET_SPEED_LOWER_VALUE, GRENADELAUNCHER_EXTRA_BULLET_SPEED_UPPER_VALUE);//give certain randomness to shot
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CPropelledGrenade::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel + lAngleOffset, *iWeaponReflectedOverYAxis, lExtraBulletSpeed));
		
		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
	}
}
