/*
 * CBackGunDoubleDoubleMachineGun8mm.cpp
 *
 *  Created on: Jun 20, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CBackGunDoubleMachineGun8mm::CBackGunDoubleMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft) :
	CMobileAAGun(aLocation, WeaponIdentifierBackGunDoubleMachineGun8mm, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_WAIT_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aAircraft, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_ROTATION_SPEED, BULLET8MM_SPEED, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MIN_RANGE,
			WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MAX_RANGE)
{
}

CBackGunDoubleMachineGun8mm::CBackGunDoubleMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle) :
	CMobileAAGun(aLocation, WeaponIdentifierBackGunDoubleMachineGun8mm, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_WAIT_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aAircraft, JAPANESE_AA_20MM_MACHINE_CANNON_ROTATION_SPEED, BULLET8MM_SPEED, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MIN_RANGE,
			WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MAX_RANGE)
{
	iOffSetAngleFromNormalPosition = aDefaultOffsetAngle;
}

CBackGunDoubleMachineGun8mm::~CBackGunDoubleMachineGun8mm()
{
	if(iMachineGun1)
	{
		delete iMachineGun1;
		iMachineGun1 = NULL;
	}
	if(iMachineGun2)
	{
		delete iMachineGun2;
		iMachineGun2 = NULL;
	}
}

CBackGunDoubleMachineGun8mm* CBackGunDoubleMachineGun8mm::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft)
{
	CBackGunDoubleMachineGun8mm* self = new CBackGunDoubleMachineGun8mm(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aAircraft);
	self->ConstructL();
	return self;
}

CBackGunDoubleMachineGun8mm* CBackGunDoubleMachineGun8mm::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle)
{
	CBackGunDoubleMachineGun8mm* self = new CBackGunDoubleMachineGun8mm(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aAircraft,aDefaultOffsetAngle);
	self->ConstructL();
	return self;
}

void CBackGunDoubleMachineGun8mm::ConstructL()
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdAircraftDoubleBackCannon), TIntFloat::Convert(WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_WIDTH), TIntFloat::Convert(WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_HEIGHT), NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftDoubleBackCannon);
	lDefaultRequest.TotalImages = 1; //show default image only
	lDefaultRequest.TotalLength = 96;
	lDefaultRequest.Width = WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new  SAnimationRequest(lDefaultRequest));

	SBackCannonAIWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_AI_WAIT_FRAMES_AFTER_SEARCH;
	lWaitingCase.FrameDelayAfterShot = WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_AI_WAIT_FRAMES_AFTER_SHOOT;
	iBackCannonAI = new CBackCannonAI(this, lWaitingCase, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_AI_FIRES_PER_BURST, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_WAIT_FRAMES_BETWEEN_FIRE);

	iMachineGun1 = CMachineGun8mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MACHINE_GUN_1_X_POSITION , WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MACHINE_GUN_1_Y_POSITION, iRealAngle, iSpeed, ESoundFileId_Plane_Back_MachineGun13mm);
	iMachineGun2 = CMachineGun8mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide, WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MACHINE_GUN_2_X_POSITION , WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_MACHINE_GUN_2_Y_POSITION, iRealAngle, iSpeed, ESoundFileId_Plane_Back_MachineGun13mm);
}

void CBackGunDoubleMachineGun8mm::Shoot(TInt aRange)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else
	{
		Shoot();
	}
}

void CBackGunDoubleMachineGun8mm::UpdateAdditionalWeapons()
{
	iMachineGun1->Update();
	iMachineGun2->Update();
}

void CBackGunDoubleMachineGun8mm::InitilizeBackGunDoubleMachineGun8mm()
{
	CBackGunDoubleMachineGun8mm::LoadBackGunDoubleMachineGun8mmTextures();
}

void CBackGunDoubleMachineGun8mm::RemoveBackGunDoubleMachineGun8mm()
{
	CBackGunDoubleMachineGun8mm::UnloadBackGunDoubleMachineGun8mmTextures();
}

void CBackGunDoubleMachineGun8mm::LoadBackGunDoubleMachineGun8mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftDoubleBackCannon);
}

void CBackGunDoubleMachineGun8mm::UnloadBackGunDoubleMachineGun8mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftDoubleBackCannon);
}

void CBackGunDoubleMachineGun8mm::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iMachineGun1->GetIsReadyToShoot() && iMachineGun2->GetIsReadyToShoot())
	{
		iReadyToShoot = false;

		//Animation Player
		SAnimationRequest lFireRequest;
		lFireRequest.FramesUntilSwitch = WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFireRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftDoubleBackCannon);
		lFireRequest.TotalImages = 4;
		lFireRequest.TotalLength = 96;
		lFireRequest.Width = WEAPON_BACK_GUN_DOUBLE_MACHINE_GUN_8MM_WIDTH;
		lFireRequest.LowerT = 0;
		lFireRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFireRequest));

		//play animation
		//Gun 1
		CBullet8mm* lBullet = iMachineGun1->ShootAndReturnBullet();
		if(lBullet)
		{
			lBullet->SetPixelsPerMoveX(*iMoveableObject->GetPixelsPerMoveX() + *lBullet->GetPixelsPerMoveX());
			lBullet->SetPixelsPerMoveY(*iMoveableObject->GetPixelsPerMoveY() + *lBullet->GetPixelsPerMoveY());
		}
		//Gun 2
		lBullet = iMachineGun2->ShootAndReturnBullet();
		if(lBullet)
		{
			lBullet->SetPixelsPerMoveX(*iMoveableObject->GetPixelsPerMoveX() + *lBullet->GetPixelsPerMoveX());
			lBullet->SetPixelsPerMoveY(*iMoveableObject->GetPixelsPerMoveY() + *lBullet->GetPixelsPerMoveY());
		}
	}
}

void CBackGunDoubleMachineGun8mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileAAGunOnDisk(aOutputStream);
	iMachineGun1->SaveOnDisk(aOutputStream);
	iMachineGun2->SaveOnDisk(aOutputStream);
}

void CBackGunDoubleMachineGun8mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileAAGunFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iMachineGun1->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iMachineGun2->LoadFromDisk(aReadStream);
}

