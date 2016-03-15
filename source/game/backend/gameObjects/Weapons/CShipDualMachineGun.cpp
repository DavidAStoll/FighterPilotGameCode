/*
 ============================================================================
 Name		: ShipDualMachineGun.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipDualMachineGun implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CShipDualMachineGun::CShipDualMachineGun(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank) :
	CMobileAAGun(aLocation, WeaponIdentifierShipDualMachineGun, aAngel, aNormalPositionAngel, DUAL_MACHINE_GUN_CENTER_X_POSITION - aXOffset, aYOffset + DUAL_MACHINE_GUN_CENTER_Y_POSITION, aReflected, aConflictSide, aSpeed, DUAL_MACHINE_GUN_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aTank, JAPANESE_AA_20MM_MACHINE_CANNON_ROTATION_SPEED, BULLET13MM_SPEED, JAPANESE_AA_20MM_MACHINE_CANNON_MIN_RANGE,
			JAPANESE_AA_20MM_MACHINE_CANNON_MAX_RANGE)
{
	iXOffsetForTurret = aXOffset;
	iYOffsetForTurret = aYOffset;
	iTurrentBaseOffsetAngle = 0;
}

CShipDualMachineGun::~CShipDualMachineGun()
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
	if(iTurretTextureObject)
	{
		delete iTurretTextureObject;
		iTurretTextureObject = NULL;
	}
}

CShipDualMachineGun* CShipDualMachineGun::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank)
{
	CShipDualMachineGun* self = new CShipDualMachineGun(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank);
	self->ConstructL(false);
	return self;
}

CShipDualMachineGun* CShipDualMachineGun::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank, TBool aUseCamoTexture)
{
	CShipDualMachineGun* self = new CShipDualMachineGun(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank);
	self->ConstructL(aUseCamoTexture);
	return self;
}

void CShipDualMachineGun::ConstructL(TBool aUseCamoTexture)
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunDualMachineGunFire), TIntFloat::Convert(DUAL_MACHINE_GUN_20MM_MACHINE_CANNON_GUN_WIDTH), TIntFloat::Convert(DUAL_MACHINE_GUN_20MM_MACHINE_CANNON_GUN_HEIGHT), DUAL_MACHINE_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	
	lLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
	if(aUseCamoTexture)
		iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunCamoflageDualMachineGunTurret), TIntFloat::Convert(DUAL_MACHINE_GUN_WIDTH), TIntFloat::Convert(DUAL_MACHINE_GUN_HEIGHT), DUAL_MACHINE_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	else
		iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunDualMachineGunTurret), TIntFloat::Convert(DUAL_MACHINE_GUN_WIDTH), TIntFloat::Convert(DUAL_MACHINE_GUN_HEIGHT), DUAL_MACHINE_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
		
	iLeftMachineGun = CMachineGun13mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide,DUAL_MACHINE_GUN_LEFT_MACHINE_GUN_X_POSITION,DUAL_MACHINE_GUN_LEFT_MACHINE_GUN_Y_POSITION, false, iRealAngle, iSpeed, ESoundFileId_Ship_MachineGun13mm);
	iRightMachineGun = CMachineGun13mm::New(&iWeaponCoordinate, iWeaponReflectedOverYAxis, iConflictSide, DUAL_MACHINE_GUN_RIGHT_MACHINE_GUN_X_POSITION, DUAL_MACHINE_GUN_RIGHT_MACHINE_GUN_Y_POSITION, false, iRealAngle, iSpeed, ESoundFileId_Ship_MachineGun13mm);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100020;//since we use only one image, only call method once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGunDualMachineGunFire);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 136;
	lDefaultRequest.Width = DUAL_MACHINE_GUN_20MM_MACHINE_CANNON_GUN_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	SBackCannonAIWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = JAPANESE_AA_20MM_MACHINE_CANNON_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE;
	lWaitingCase.FrameDelayAfterShot = JAPANESE_AA_20MM_MACHINE_CANNON_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE_AFTER_SHOT;
	iBackCannonAI = new CBackCannonAI(this, lWaitingCase, 4, MACHINEGUN_13MM_FRAMES_BETWEEN_FIRE);
}

//---------------------------- functions -----------------------------------//

void CShipDualMachineGun::Draw()
{
	if(!iDoNotDraw && !iIsDisabled && iTextureObject)
	{
		//UpdateMobileRotatingWeapon();
		iTextureObject->DrawArrays();
		//draw turret as well
		TPointIntFloat lLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
		Rotate(0,lLocation);
		iTurretTextureObject->DrawArrays();
	}
}

void CShipDualMachineGun::Rotate(TInt aDegree, TPointIntFloat aLocation)
{
	iTurrentBaseOffsetAngle += aDegree;
	TInt lAngleWithReflection = iTurrentBaseOffsetAngle;
	if(!(*iWeaponReflectedOverYAxis)) //ship faces to the left
		lAngleWithReflection += 180; //need to add since object might be reflected or not
	iTurretTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(lAngleWithReflection, *iWeaponReflectedOverYAxis, aLocation);
}

void CShipDualMachineGun::Shoot(TInt aRange)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iLeftMachineGun->GetIsReadyToShoot() && iRightMachineGun->GetIsReadyToShoot())
	{
		//reset
		iReadyToShoot = false;

		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_20MM_MACHINE_CANNON_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGunDualMachineGunFire);
		lFiringRequest.TotalImages = 4;
		lFiringRequest.TotalLength = 136;
		lFiringRequest.Width = DUAL_MACHINE_GUN_20MM_MACHINE_CANNON_GUN_WIDTH;
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

void CShipDualMachineGun::UpdateAdditionalWeapons()
{
	iLeftMachineGun->Update();
	iRightMachineGun->Update();
}

void CShipDualMachineGun::InitilizeShipDualMachineGun()
{
	CShipDualMachineGun::LoadShipDualMachineGunTextures();
}

void CShipDualMachineGun::RemoveShipDualMachineGun()
{
	CShipDualMachineGun::UnloadShipDualMachineGunTextures();
}

void CShipDualMachineGun::InitilizeCamoShipDualMachineGun()
{
	CShipDualMachineGun::LoadCamoShipDualMachineGunTextures();
}

void CShipDualMachineGun::RemoveCamoShipDualMachineGun()
{
	CShipDualMachineGun::UnloadCamoShipDualMachineGunTextures();
}

void CShipDualMachineGun::LoadShipDualMachineGunTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunDualMachineGunFire);
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunDualMachineGunTurret);
}

void CShipDualMachineGun::UnloadShipDualMachineGunTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunDualMachineGunFire);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunDualMachineGunTurret);
}

void CShipDualMachineGun::LoadCamoShipDualMachineGunTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunCamoflageDualMachineGunTurret);
}

void CShipDualMachineGun::UnloadCamoShipDualMachineGunTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunCamoflageDualMachineGunTurret);
}

void CShipDualMachineGun::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileAAGunOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iXOffsetForTurret);
	aOutputStream.WriteInt32(iYOffsetForTurret);
	aOutputStream.WriteInt32(iTurrentBaseOffsetAngle);
	iLeftMachineGun->SaveOnDisk(aOutputStream);
	iRightMachineGun->SaveOnDisk(aOutputStream);
	iTurretTextureObject->SaveOnDisk(aOutputStream);
}

void CShipDualMachineGun::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileAAGunFromDisk(aReadStream);
	iXOffsetForTurret = aReadStream.ReadInt32();
	iYOffsetForTurret = aReadStream.ReadInt32();
	iTurrentBaseOffsetAngle = aReadStream.ReadInt32();
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iLeftMachineGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iRightMachineGun->LoadFromDisk(aReadStream);
	iTurretTextureObject->LoadFromDisk(aReadStream);
}
