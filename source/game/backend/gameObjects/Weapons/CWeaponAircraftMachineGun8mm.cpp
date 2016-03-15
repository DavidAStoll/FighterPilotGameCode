/*
 * CWeaponAircraftMachineGun8mm.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftMachineGun8mm.h"
#include "includes/game/backend/gameObjects/Weapons/CMachineGun8mm.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponAircraftMachineGun8mm::CWeaponAircraftMachineGun8mm(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed)
:CWeaponAttachedInvisible(1, EWeaponIdentifierAircraftMachineGun8mm, false, aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_AIRCRAFT_MACHINE_GUN_8MM_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed, WEAPON_AIRCRAFT_MACHINE_GUN_8MM_ANIMATION_LENGTH_IN_FRAMES)
{
	// TODO Auto-generated constructor stub

}

CWeaponAircraftMachineGun8mm::~CWeaponAircraftMachineGun8mm()
{

}

CWeaponAircraftMachineGun8mm* CWeaponAircraftMachineGun8mm::New(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed)
{
	CWeaponAircraftMachineGun8mm* lSelf = new CWeaponAircraftMachineGun8mm(aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	lSelf->Construct();
	return lSelf;
}


void CWeaponAircraftMachineGun8mm::Construct()
{
	//Texture Object
	iTextureObject = CTextureObject::New(TPoint(0,0), &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponMachineGun8mm), WEAPON_AIRCRAFT_MACHINE_GUN_8MM_WIDTH, WEAPON_AIRCRAFT_MACHINE_GUN_8MM_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponMachineGun8mm);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 21;
	lDefaultRequest.Width = 7;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//Gun
	iGun = CMachineGun8mm::New(iCoordinate, iWeaponReflectedOverYAxis, iConflictSide, iXOffset, iYOffset, *iAngel, iSpeed,ESoundFileId_Dauntless_Machine_Gun);
}

void CWeaponAircraftMachineGun8mm::InitilizeWeaponAircraftMachineGun8mm()
{
	LoadWeaponAircraftMachineGun8mmTextures();
}

void CWeaponAircraftMachineGun8mm::RemoveWeaponAircraftMachineGun8mm()
{
	UnloadWeaponAircraftMachineGun8mmTextures();
}

void CWeaponAircraftMachineGun8mm::Shoot()
{
		if(!iIsDisabled && iReadyToShoot && iGun->GetIsReadyToShoot())
		{
			//set Animation
			SAnimationRequest lFireRequest;
			lFireRequest.FramesUntilSwitch = 1;
			lFireRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftWeaponMachineGun8mm);
			lFireRequest.TotalImages = 3;
			lFireRequest.TotalLength = 21;
			lFireRequest.Width = 7;
			lFireRequest.LowerT = 0;
			lFireRequest.UpperT = 1;
			iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFireRequest));

			//create bullet
			CBullet8mm* lBullet = iGun->ShootAndReturnBullet();
			lBullet->AssignDamageType(EDamageTypePlaneBullet); //needed for Aircraft AI

			//do miscelleanous stuff
			iFrameInFireAnimation = 1;//to start animation loop
			iReadyToShoot = false;
		}
}

void CWeaponAircraftMachineGun8mm::UpdateAdditionalWeapons()
{
	iGun->Update();
}

void CWeaponAircraftMachineGun8mm::LoadWeaponAircraftMachineGun8mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftWeaponMachineGun8mm);
}

void CWeaponAircraftMachineGun8mm::UnloadWeaponAircraftMachineGun8mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftWeaponMachineGun8mm);
}

void CWeaponAircraftMachineGun8mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponAttachedInvisibleObjectSaveContentToDisk(aOutputStream);
	iGun->SaveOnDisk(aOutputStream);
}

void CWeaponAircraftMachineGun8mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponAttachedInvisibleObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iGun->LoadFromDisk(aReadStream);
}

