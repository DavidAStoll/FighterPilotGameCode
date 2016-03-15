/*
 * CWeaponAircraftCannon20mm.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftCannon20mm.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponCannon20mm.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponAircraftCannon20mm::CWeaponAircraftCannon20mm(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed)
:CWeaponAttachedInvisible(1, EWeaponIdentifierAircraftCannon20mm, false, aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_AIRCRAFT_CANNON_20MM_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed, WEAPON_AIRCRAFT_CANNON_20MM_ANIMATION_LENGTH_IN_FRAMES)
{
	// TODO Auto-generated constructor stub

}

CWeaponAircraftCannon20mm::~CWeaponAircraftCannon20mm()
{

}

CWeaponAircraftCannon20mm* CWeaponAircraftCannon20mm::New(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed)
{
	CWeaponAircraftCannon20mm* lSelf = new CWeaponAircraftCannon20mm(aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	lSelf->Construct();
	return lSelf;
}


void CWeaponAircraftCannon20mm::Construct()
{
	//Texture Object
	iTextureObject = CTextureObject::New(TPoint(0,0), &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponCannon20mm), WEAPON_AIRCRAFT_CANNON_20MM_WIDTH, WEAPON_AIRCRAFT_CANNON_20MM_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponCannon20mm);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 42;
	lDefaultRequest.Width = 14;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//Gun
	iGun = CWeaponCannon20mm::New(iCoordinate, iWeaponReflectedOverYAxis, iConflictSide, iXOffset, iYOffset, true, *iAngel, iSpeed,ESoundFileId_Dauntless_Machine_Gun);
}

void CWeaponAircraftCannon20mm::InitilizeWeaponAircraftCannon20mm()
{
	LoadWeaponAircraftCannon20mmTextures();
}

void CWeaponAircraftCannon20mm::RemoveWeaponAircraftCannon20mm()
{
	UnloadWeaponAircraftCannon20mmTextures();
}

void CWeaponAircraftCannon20mm::Shoot()
{
		if(!iIsDisabled && iReadyToShoot && iGun->GetIsReadyToShoot())
		{
			//set Animation
			SAnimationRequest lFireRequest;
			lFireRequest.FramesUntilSwitch = 1;
			lFireRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponCannon20mm);
			lFireRequest.TotalImages = 3;
			lFireRequest.TotalLength = 42;
			lFireRequest.Width = 14;
			lFireRequest.LowerT = 0;
			lFireRequest.UpperT = 1;
			iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFireRequest));

			//create bullet
			CBulletCannon20mm* lBullet = iGun->ShootAndReturnBullet();
			lBullet->AssignDamageType(EDamageTypePlaneBullet); //needed for Aircraft AI

			//do miscelleanous stuff
			iFrameInFireAnimation = 1;//to start animation loop
			iReadyToShoot = false;
		}
}

void CWeaponAircraftCannon20mm::UpdateAdditionalWeapons()
{
	iGun->Update();
}


void CWeaponAircraftCannon20mm::LoadWeaponAircraftCannon20mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftWeaponCannon20mm);
}

void CWeaponAircraftCannon20mm::UnloadWeaponAircraftCannon20mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftWeaponCannon20mm);
}

void CWeaponAircraftCannon20mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponAttachedInvisibleObjectSaveContentToDisk(aOutputStream);
	iGun->SaveOnDisk(aOutputStream);
}

void CWeaponAircraftCannon20mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponAttachedInvisibleObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iGun->LoadFromDisk(aReadStream);
}

