/*
 ============================================================================
 Name		: TankAAGun.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTankAAGun implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CTankAAGun::CTankAAGun(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank) :
	CMobileAAGun(aLocation, WeaponIdentifierTankAAGun, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, MACHINEGUN_13MM_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aTank, JAPANESE_AA_20MM_MACHINE_CANNON_ROTATION_SPEED, BULLET13MM_SPEED, JAPANESE_AA_20MM_MACHINE_CANNON_MIN_RANGE,
			JAPANESE_AA_20MM_MACHINE_CANNON_MAX_RANGE)
{
}

CTankAAGun::~CTankAAGun()
{
	if(iLeftMachineGun)
	{
		delete iLeftMachineGun;
		iLeftMachineGun = NULL;
	}
	if(iRightMachineGun)
	{
		delete iRightMachineGun;
		iRightMachineGun = NULL;
	}
}

CTankAAGun* CTankAAGun::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank)
{
	CTankAAGun* self = new CTankAAGun(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank);
	self->ConstructL();
	return self;
}

void CTankAAGun::ConstructL()
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankAAGun), TIntFloat::Convert(AMERICAN_AA_SHERMAN_TANK_20MM_MACHINE_CANNON_GUN_WIDTH), TIntFloat::Convert(AMERICAN_AA_SHERMAN_TANK_20MM_MACHINE_CANNON_GUN_HEIGHT), AMERICAN_SHERMAN_TANK_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);

	iLeftMachineGun = CMachineGun13mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide, AMERICAN_AA_SHERMAN_TANK_LEFT_MACHINE_GUN_X_POSITION,AMERICAN_AA_SHERMAN_TANK_LEFT_MACHINE_GUN_Y_POSITION , false, iRealAngle, iSpeed, ESoundFileId_Plane_Back_MachineGun13mm);
	iRightMachineGun = CMachineGun13mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide, AMERICAN_AA_SHERMAN_TANK_RIGHT_MACHINE_GUN_X_POSITION,  AMERICAN_AA_SHERMAN_TANK_RIGHT_MACHINE_GUN_Y_POSITION,false, iRealAngle, iSpeed, ESoundFileId_Plane_Back_MachineGun13mm);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100020;//since we use only one image, only call method once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankAAGun);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = AMERICAN_AA_SHERMAN_TANK_20MM_MACHINE_CANNON_GUN_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	SBackCannonAIWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = JAPANESE_AA_20MM_MACHINE_CANNON_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE;
	lWaitingCase.FrameDelayAfterShot = JAPANESE_AA_20MM_MACHINE_CANNON_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE_AFTER_SHOT;
	iBackCannonAI = new CBackCannonAI(this, lWaitingCase, 4, MACHINEGUN_13MM_FRAMES_BETWEEN_FIRE);
}

//---------------------------- functions -----------------------------------//

void CTankAAGun::Shoot(TInt aRange)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iLeftMachineGun->GetIsReadyToShoot() && iRightMachineGun->GetIsReadyToShoot())
	{
		iReadyToShoot = false;

		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_20MM_MACHINE_CANNON_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankAAGun);
		lFiringRequest.TotalImages = 4;
		lFiringRequest.TotalLength = 256;
		lFiringRequest.Width = AMERICAN_AA_SHERMAN_TANK_20MM_MACHINE_CANNON_GUN_WIDTH;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));

		CBullet13mm* lBullet = iLeftMachineGun->ShootAndReturnBulletWithoutReflection();
		if(lBullet)
		{
			lBullet->SetPixelsPerMoveX(*iMoveableObject->GetPixelsPerMoveX() + *lBullet->GetPixelsPerMoveX());
			lBullet->SetPixelsPerMoveY(*iMoveableObject->GetPixelsPerMoveY() + *lBullet->GetPixelsPerMoveY());
		}
		lBullet = iRightMachineGun->ShootAndReturnBulletWithoutReflection();
		if(lBullet)
		{
			lBullet->SetPixelsPerMoveX(*iMoveableObject->GetPixelsPerMoveX() + *lBullet->GetPixelsPerMoveX());
			lBullet->SetPixelsPerMoveY(*iMoveableObject->GetPixelsPerMoveY() + *lBullet->GetPixelsPerMoveY());
		}
	}
}

void CTankAAGun::UpdateAdditionalWeapons()
{
	iLeftMachineGun->Update();
	iRightMachineGun->Update();
}

void CTankAAGun::InitilizeTankAAGun()
{
	CTankAAGun::LoadTankAAGunTextures();
}

void CTankAAGun::RemoveTankAAGun()
{
	CTankAAGun::UnloadTankAAGunTextures();
}

void CTankAAGun::LoadTankAAGunTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAAShermanTankAAGun);
}

void CTankAAGun::UnloadTankAAGunTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAAShermanTankAAGun);
}

void CTankAAGun::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileAAGunOnDisk(aOutputStream);
	iLeftMachineGun->SaveOnDisk(aOutputStream);
	iRightMachineGun->SaveOnDisk(aOutputStream);
}

void CTankAAGun::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileAAGunFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iLeftMachineGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iRightMachineGun->LoadFromDisk(aReadStream);
}
