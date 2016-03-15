/*
 * CWeaponAircraftMachineGun13mm.cpp
 *
 *  Created on: May 3, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftMachineGun13mm.h"
#include "includes/game/backend/gameObjects/Weapons/CMachineGun13mm.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponAircraftMachineGun13mm::CWeaponAircraftMachineGun13mm(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed)
:CWeaponAttachedInvisible(1, EWeaponIdentifierAircraftMachineGun13mm, false, aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_AIRCRAFT_MACHINE_GUN_13MM_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed, WEAPON_AIRCRAFT_MACHINE_GUN_13MM_ANIMATION_LENGTH_IN_FRAMES)
{
	// TODO Auto-generated constructor stub

}

CWeaponAircraftMachineGun13mm::~CWeaponAircraftMachineGun13mm()
{

}

CWeaponAircraftMachineGun13mm* CWeaponAircraftMachineGun13mm::New(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed)
{
	CWeaponAircraftMachineGun13mm* lSelf = new CWeaponAircraftMachineGun13mm(aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	lSelf->Construct();
	return lSelf;
}


void CWeaponAircraftMachineGun13mm::Construct()
{
	//Texture Object
	iTextureObject = CTextureObject::New(TPoint(0,0), &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponMachineGun13mm), WEAPON_AIRCRAFT_MACHINE_GUN_13MM_WIDTH, WEAPON_AIRCRAFT_MACHINE_GUN_13MM_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponMachineGun13mm);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 30;
	lDefaultRequest.Width = 10;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//Gun
	iGun = CMachineGun13mm::New(iCoordinate, iWeaponReflectedOverYAxis, iConflictSide, iXOffset, iYOffset, true, *iAngel, iSpeed,ESoundFileId_Dauntless_Machine_Gun);
}

void CWeaponAircraftMachineGun13mm::InitilizeWeaponAircraftMachineGun13mm()
{
	LoadWeaponAircraftMachineGun13mmTextures();
}

void CWeaponAircraftMachineGun13mm::RemoveWeaponAircraftMachineGun13mm()
{
	UnloadWeaponAircraftMachineGun13mmTextures();
}

void CWeaponAircraftMachineGun13mm::Shoot()
{
		if(!iIsDisabled && iReadyToShoot && iGun->GetIsReadyToShoot())
		{
			//set Animation
			SAnimationRequest lFireRequest;
			lFireRequest.FramesUntilSwitch = 1;
			lFireRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponMachineGun13mm);
			lFireRequest.TotalImages = 3;
			lFireRequest.TotalLength = 30;
			lFireRequest.Width = 10;
			lFireRequest.LowerT = 0;
			lFireRequest.UpperT = 1;
			iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFireRequest));

			//create bullet
			CBullet13mm* lBullet = iGun->ShootAndReturnBullet();
			lBullet->AssignDamageType(EDamageTypePlaneBullet); //needed for Aircraft AI

			//do miscelleanous stuff
			iFrameInFireAnimation = 1;//to start animation loop
			iReadyToShoot = false;
		}
}

void CWeaponAircraftMachineGun13mm::UpdateAdditionalWeapons()
{
	iGun->Update();
}

void CWeaponAircraftMachineGun13mm::LoadWeaponAircraftMachineGun13mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftWeaponMachineGun13mm);
}

void CWeaponAircraftMachineGun13mm::UnloadWeaponAircraftMachineGun13mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftWeaponMachineGun13mm);
}

void CWeaponAircraftMachineGun13mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponAttachedInvisibleObjectSaveContentToDisk(aOutputStream);
	iGun->SaveOnDisk(aOutputStream);
}

void CWeaponAircraftMachineGun13mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponAttachedInvisibleObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iGun->LoadFromDisk(aReadStream);
}

