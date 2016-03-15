/*
 ============================================================================
 Name		: WeaponTorpedo.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponTorpedo implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponTorpedo.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb500kg.h"
#include "includes/game/backend/gameObjects/Projectiles/CTorpedo.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponTorpedo::CWeaponTorpedo(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed) :
	CWeaponBomb(aAmmunition, EWeaponIdentifierTorpedo, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_TORPEDO_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	// No implementation required
}

CWeaponTorpedo::~CWeaponTorpedo()
{
}

CWeaponTorpedo* CWeaponTorpedo::New(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed)
{
	CWeaponTorpedo* self = new CWeaponTorpedo(aAmmunition, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	self->ConstructL();
	return self;
}

void CWeaponTorpedo::ConstructL()
{
	TPointIntFloat lFloatPoint = GetRelativeWeaponPosition();
	TPoint lLocation;
	lLocation.iX = lFloatPoint.iX.GetIntInBaseInt();
	lLocation.iY = lFloatPoint.iY.GetIntInBaseInt();

	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdBombTorpedo), TORPEDO_WIDTH, TORPEDO_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 0.90625, 0, 1);
}

void CWeaponTorpedo::Shoot()
{
	if(iIsDisabled || *iIsUpSideDown)
	{
		//maybe play a sound
		CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Lock, GetRelativeWeaponPosition());
	}else
	{
		if(iAmmunitionLeft == 0)
		{
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Empty, GetRelativeWeaponPosition());
		}else if(iReadyToShoot && iAmmunitionLeft > 0)
		{
			//play sound
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_release, GetRelativeWeaponPosition());
			iReadyToShoot = false;
			iCurrentFrame = 0;
			iAmmunitionLeft--;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CTorpedo::New(GetRelativeWeaponPosition(), (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
		}
	}
}

void CWeaponTorpedo::Explode()
{
	if(iAmmunitionLeft > 0)
	{
		iAmmunitionLeft = 0;
		TPointIntFloat lBombPosition = GetRelativeWeaponPosition();
		lBombPosition.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CTorpedo::New(lBombPosition, TIntFloat::Convert(UNDER_WATER_TORPED_MAX_SPEED_OTHERWISE_EXPLODE), *iAngel, *iWeaponReflectedOverYAxis));
	}
}
