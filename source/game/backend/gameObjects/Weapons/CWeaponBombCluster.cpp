/*
 ============================================================================
 Name		: WeaponBombCluster.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBombCluster implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponBombCluster.h"
#include "includes/game/backend/gameObjects/Projectiles/CClusterBomb.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb500kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponBombCluster::CWeaponBombCluster(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed, TInt aTimerFrames) :
	CWeaponBomb(aAmmunition, EWeaponIdentifierClusterBomb, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_CLUSTER_BOMB_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	iTimerFrames = aTimerFrames;
}

CWeaponBombCluster::~CWeaponBombCluster()
{
}

CWeaponBombCluster* CWeaponBombCluster::New(TInt aAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TBool aDoNotDraw, TIntFloat* aObjectSpeed, TInt aTimerFrames)
{
	CWeaponBombCluster* self = new CWeaponBombCluster(aAmmunition, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed, aTimerFrames);
	self->ConstructL();
	return self;
}

void CWeaponBombCluster::ConstructL()
{
	TPointIntFloat lFloatPoint = GetRelativeWeaponPosition();
	TPoint lLocation;
	lLocation.iX = lFloatPoint.iX.GetIntInBaseInt();
	lLocation.iY = lFloatPoint.iY.GetIntInBaseInt();

	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdBombClusterBomb), CLUSTER_BOMB_WIDTH, CLUSTER_BOMB_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 0.625, 0, 0.625);
}

//----------------- functions ---------------


void CWeaponBombCluster::Shoot()
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
		}
		else if(iReadyToShoot && iAmmunitionLeft > 0)
		{
			//play sound
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_release, GetRelativeWeaponPosition());
			iReadyToShoot = false;
			iCurrentFrame = 0;
			iAmmunitionLeft--;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CClusterBomb::New(GetRelativeWeaponPosition(), (*iSpeed), *iAngel, iTimerFrames, *iWeaponReflectedOverYAxis));
		}
	}
}

void CWeaponBombCluster::Explode()
{
	if(iAmmunitionLeft > 0)
	{
		iAmmunitionLeft = 0;
		TPointIntFloat lBombPosition = GetRelativeWeaponPosition();
		lBombPosition.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb500kg::New(lBombPosition, (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));
	}
}

void CWeaponBombCluster::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponBombObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iTimerFrames);
}

void CWeaponBombCluster::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponBombObjectLoadContentFromDisk(aReadStream);
	iTimerFrames = aReadStream.ReadInt32();
}
