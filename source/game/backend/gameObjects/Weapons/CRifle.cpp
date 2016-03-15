/*
 ============================================================================
 Name		: Rifle.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright HQ Interactive
 Description : CRifle implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CRifle::CRifle(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide) :
	CWeapon(RIFLE_DEFAULT_AMMUNITION, EWeaponIdentifierRifle, RIFLE_DEFAULT_LIMITED_AMMUNITION, aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide, RIFLE_FRAMES_BETWEEN_FIRE)
{
	iSoundObject = CSoundObject::New(ESoundFileId_Rifle_Gun, GetRelativeWeaponPosition());
}

CRifle::~CRifle()
{
}

CRifle* CRifle::New(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide)
{
	CRifle* self = new CRifle(aDisabled, aLocation, aAngel, aXOffSet, aYOffSet, aReflected, aConflictSide);
	return self;
}

//--------------------------functions-------------------------

void CRifle::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot)
	{
		iReadyToShoot = false;
		TInt lAngleOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(RIFLE_LOWER_ANGLE_RANDOM_VALUE, RIFLE_UPPER_ANGLE_RANDOM_VALUE);//give certain randomness to shot
		//need to make adjustments of the angle depending if we are reflected or not
		if(!(*iWeaponReflectedOverYAxis))//means it is looking left
			lAngleOffset = lAngleOffset * (-1); //actually lets  the projectile fire at a higher arc, since we start with 180 degrees

		TIntFloat lExtraBulletSpeed = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(RIFLE_EXTRA_BULLET_SPEED_LOWER_VALUE, RIFLE_EXTRA_BULLET_SPEED_UPPER_VALUE);//give certain randomness to shot

		//play sound
		iSoundObject->PlayCheckPoint(GetRelativeWeaponPosition());
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBulletRifle::New(GetRelativeWeaponPosition(), *iConflictSide, *iAngel + lAngleOffset, *iWeaponReflectedOverYAxis, lExtraBulletSpeed));
	}
}
