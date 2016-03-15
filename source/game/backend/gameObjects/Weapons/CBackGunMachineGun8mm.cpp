/*
 ============================================================================
 Name		: DauntlessBackCannon.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBackGunMachineGun8mm implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CBackGunMachineGun8mm::CBackGunMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft) :
	CMobileAAGun(aLocation, WeaponIdentifierBackGunMachineGun8mm, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, WEAPON_BACK_GUN_MACHINE_GUN_8MM_WAIT_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aAircraft, WEAPON_BACK_GUN_MACHINE_GUN_8MM_ROTATION_SPEED, BULLET8MM_SPEED, WEAPON_BACK_GUN_MACHINE_GUN_8MM_MIN_RANGE,
			WEAPON_BACK_GUN_MACHINE_GUN_8MM_MAX_RANGE)
{
}

CBackGunMachineGun8mm::CBackGunMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle) :
	CMobileAAGun(aLocation, WeaponIdentifierBackGunMachineGun8mm, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, WEAPON_BACK_GUN_MACHINE_GUN_8MM_WAIT_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aAircraft, WEAPON_BACK_GUN_MACHINE_GUN_8MM_ROTATION_SPEED, BULLET8MM_SPEED, WEAPON_BACK_GUN_MACHINE_GUN_8MM_MIN_RANGE,
			WEAPON_BACK_GUN_MACHINE_GUN_8MM_MAX_RANGE)
{
	iOffSetAngleFromNormalPosition = aDefaultOffsetAngle;
}

CBackGunMachineGun8mm::~CBackGunMachineGun8mm()
{
	if(iMachineGun)
	{
		delete iMachineGun;
		iMachineGun = NULL;
	}
}

CBackGunMachineGun8mm* CBackGunMachineGun8mm::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft)
{
	CBackGunMachineGun8mm* self = new CBackGunMachineGun8mm(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aAircraft);
	self->ConstructL();
	return self;
}

CBackGunMachineGun8mm* CBackGunMachineGun8mm::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle)
{
	CBackGunMachineGun8mm* self = new CBackGunMachineGun8mm(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aAircraft,aDefaultOffsetAngle);
	self->ConstructL();
	return self;
}

void CBackGunMachineGun8mm::ConstructL()
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdAircraftBackCannon), TIntFloat::Convert(WEAPON_BACK_GUN_MACHINE_GUN_8MM_WIDTH), TIntFloat::Convert(WEAPON_BACK_GUN_MACHINE_GUN_8MM_HEIGHT), NOT_IMPORTANT_Z_VALUE, 0, 1, 0, 1);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = WEAPON_BACK_GUN_MACHINE_GUN_8MM_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftBackCannon);
	lDefaultRequest.TotalImages = 1; //show default image only
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = WEAPON_BACK_GUN_MACHINE_GUN_8MM_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new  SAnimationRequest(lDefaultRequest));

	SBackCannonAIWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = WEAPON_BACK_GUN_MACHINE_GUN_8MM_AI_WAIT_FRAMES_AFTER_SEARCH;
	lWaitingCase.FrameDelayAfterShot = WEAPON_BACK_GUN_MACHINE_GUN_8MM_AI_WAIT_FRAMES_AFTER_SHOOT;
	iBackCannonAI = new CBackCannonAI(this, lWaitingCase, WEAPON_BACK_GUN_MACHINE_GUN_8MM_AI_FIRES_PER_BURST, WEAPON_BACK_GUN_MACHINE_GUN_8MM_WAIT_FRAMES_BETWEEN_FIRE);

	iMachineGun = CMachineGun8mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide, WEAPON_BACK_GUN_MACHINE_GUN_8MM_MACHINE_GUN_X_POSITION , WEAPON_BACK_GUN_MACHINE_GUN_8MM_MACHINE_GUN_Y_POSITION, iRealAngle, iSpeed, ESoundFileId_Plane_Back_MachineGun13mm);
}

void CBackGunMachineGun8mm::Shoot(TInt aRange)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else
	{
		Shoot();
	}
}

void CBackGunMachineGun8mm::UpdateAdditionalWeapons()
{
	iMachineGun->Update();
}

void CBackGunMachineGun8mm::InitilizeBackGunMachineGun8mm()
{
	CBackGunMachineGun8mm::LoadBackGunMachineGun8mmTextures();
}

void CBackGunMachineGun8mm::RemoveBackGunMachineGun8mm()
{
	CBackGunMachineGun8mm::UnloadBackGunMachineGun8mmTextures();
}

void CBackGunMachineGun8mm::LoadBackGunMachineGun8mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftBackCannon);
}

void CBackGunMachineGun8mm::UnloadBackGunMachineGun8mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftBackCannon);
}

void CBackGunMachineGun8mm::Shoot()
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iMachineGun->GetIsReadyToShoot())
	{
		iReadyToShoot = false;

		//Animation Player
		SAnimationRequest lFireRequest;
		lFireRequest.FramesUntilSwitch = WEAPON_BACK_GUN_MACHINE_GUN_8MM_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFireRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftBackCannon);
		lFireRequest.TotalImages = 4;
		lFireRequest.TotalLength = 128;
		lFireRequest.Width = WEAPON_BACK_GUN_MACHINE_GUN_8MM_WIDTH;
		lFireRequest.LowerT = 0;
		lFireRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFireRequest));

		//play animation
		CBullet8mm* lBullet = iMachineGun->ShootAndReturnBullet();
		if(lBullet)
		{
			lBullet->SetPixelsPerMoveX(*iMoveableObject->GetPixelsPerMoveX() + *lBullet->GetPixelsPerMoveX());
			lBullet->SetPixelsPerMoveY(*iMoveableObject->GetPixelsPerMoveY() + *lBullet->GetPixelsPerMoveY());
		}
	}
}

void CBackGunMachineGun8mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileAAGunOnDisk(aOutputStream);
	iMachineGun->SaveOnDisk(aOutputStream);
}

void CBackGunMachineGun8mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileAAGunFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iMachineGun->LoadFromDisk(aReadStream);
}
