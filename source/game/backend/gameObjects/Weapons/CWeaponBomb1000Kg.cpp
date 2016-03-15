/*
 ============================================================================
 Name		: WeaponBomb1000Kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBomb1000Kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb1000Kg.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb1000kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponBomb1000Kg::CWeaponBomb1000Kg(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed) :
	CWeaponBomb(aAmmunition, EWeaponIdentifier1000kgBomb, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_BOMB_1000KG_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	// No implementation required
}

CWeaponBomb1000Kg::~CWeaponBomb1000Kg()
{
}

CWeaponBomb1000Kg* CWeaponBomb1000Kg::New(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed)
{
	CWeaponBomb1000Kg* self = new CWeaponBomb1000Kg(aAmmunition, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	self->ConstructL();
	return self;
}

void CWeaponBomb1000Kg::ConstructL()
{
	TPointIntFloat lFloatPoint = GetRelativeWeaponPosition();
	TPoint lLocation;
	lLocation.iX = lFloatPoint.iX.GetIntInBaseInt();
	lLocation.iY = lFloatPoint.iY.GetIntInBaseInt();

	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdBomb1000KgBomb), BOMB_1000KG_WIDTH, BOMB_1000KG_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);
}

//----------------- functions ---------------


void CWeaponBomb1000Kg::Shoot()
{
	if(iIsDisabled || *iIsUpSideDown)
	{
		//maybe play a sound
		CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Lock, GetRelativeWeaponPosition());
	}
	else //should be able to shoot if we have enough ammo
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
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb1000kg::New(GetRelativeWeaponPosition(), (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
		}
	}
}

void CWeaponBomb1000Kg::Explode()
{
	if(iAmmunitionLeft > 0)
	{
		iAmmunitionLeft = 0;
		TPointIntFloat lBombPosition = GetRelativeWeaponPosition();
		lBombPosition.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb1000kg::New(lBombPosition, (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
	}
}
