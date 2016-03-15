/*
 ============================================================================
 Name		: WeaponBomb500Kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBomb500Kg implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Projectiles/CBomb500kg.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb500Kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponBomb500Kg::CWeaponBomb500Kg(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed) :
	CWeaponBomb(aAmmunition, EWeaponIdentifier500kgBomb, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_BOMB_500KG_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	// No implementation required
}

CWeaponBomb500Kg::~CWeaponBomb500Kg()
{
}

CWeaponBomb500Kg* CWeaponBomb500Kg::New(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed)
{
	CWeaponBomb500Kg* self = new CWeaponBomb500Kg(aAmmunition, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	self->ConstructL();
	return self;
}

void CWeaponBomb500Kg::ConstructL()
{
	TPointIntFloat lFloatPoint = GetRelativeWeaponPosition();
	TPoint lLocation;
	lLocation.iX = lFloatPoint.iX.GetIntInBaseInt();
	lLocation.iY = lFloatPoint.iY.GetIntInBaseInt();

	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdBomb500KgBomb), BOMB_500KG_WIDTH, BOMB_500KG_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 0.9375, 0, 1);
}

//----------------- functions ---------------


void CWeaponBomb500Kg::Shoot()
{
	if(iIsDisabled || *iIsUpSideDown)
	{
		//maybe play a sound
		CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Lock, GetRelativeWeaponPosition());
	}
	else
	{
		if(iAmmunitionLeft == 0)
		{
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Empty, GetRelativeWeaponPosition());
		}
		else if(iReadyToShoot && iAmmunitionLeft > 0)
		{
			//play sound
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_release, GetRelativeWeaponPosition());
			iReadyToShoot = false;
			iCurrentFrame = 0;
			iAmmunitionLeft--;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb500kg::New(GetRelativeWeaponPosition(), (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
		}
	}
}

void CWeaponBomb500Kg::Explode()
{
	if(iAmmunitionLeft > 0)
	{
		iAmmunitionLeft = 0;
		TPointIntFloat lBombPosition = GetRelativeWeaponPosition();
		lBombPosition.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb500kg::New(lBombPosition, (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
	}
}
