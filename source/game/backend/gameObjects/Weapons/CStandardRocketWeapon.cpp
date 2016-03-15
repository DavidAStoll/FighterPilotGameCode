/*
 ============================================================================
 Name		: StandardRocketWeapon.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStandardRocketWeapon implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CStandardRocketWeapon.h"
#include "includes/game/backend/gameObjects/Projectiles/CStandardRocket.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb500kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CStandardRocketWeapon::CStandardRocketWeapon(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed, CGameObject* aCreator) :
	CWeaponBomb(aAmmunition, EWeaponStandardRocket, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_STANDARD_ROCKET_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	iOwnerObject = aCreator;
}

CStandardRocketWeapon::~CStandardRocketWeapon()
{
}

CStandardRocketWeapon* CStandardRocketWeapon::New(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed, CGameObject* aCreator)
{
	CStandardRocketWeapon* self = new CStandardRocketWeapon(aAmmunition, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed, aCreator);
	self->ConstructL();
	return self;
}

void CStandardRocketWeapon::ConstructL()
{
	TPointIntFloat lFloatPoint = GetRelativeWeaponPosition();
	TPoint lLocation;
	lLocation.iX = lFloatPoint.iX.GetIntInBaseInt();
	lLocation.iY = lFloatPoint.iY.GetIntInBaseInt();

	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdRocketStandardRocketNormalView), WEAPON_STANDARD_ROCKET_WIDTH, WEAPON_STANDARD_ROCKET_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);
}

//----------------- functions ---------------


void CStandardRocketWeapon::InitilizeStandardRocketWeapon()
{
	CStandardRocketWeapon::LoadStandardRocketWeaponTextures();
}

void CStandardRocketWeapon::RemoveStandardRocketWeapon()
{
	CStandardRocketWeapon::UnloadStandardRocketWeaponTextures();
}

void CStandardRocketWeapon::LoadStandardRocketWeaponTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdRocketStandardRocketNormalView);
}

void CStandardRocketWeapon::UnloadStandardRocketWeaponTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdRocketStandardRocketNormalView);
}

void CStandardRocketWeapon::Shoot()
{
	if(iIsDisabled)
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
			iReadyToShoot = false;
			iCurrentFrame = 0;
			iAmmunitionLeft--;
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_release, GetRelativeWeaponPosition());
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CStandardRocket::New(GetRelativeWeaponPosition(), (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis, iOwnerObject));
		}
	}
}

void CStandardRocketWeapon::Explode()
{
	if(iAmmunitionLeft > 0)
	{
		iAmmunitionLeft = 0;
		TPointIntFloat lBombPosition = GetRelativeWeaponPosition();
		lBombPosition.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb500kg::New(lBombPosition, (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
	}
}
