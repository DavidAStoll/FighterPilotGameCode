/*
 ============================================================================
 Name		: ShipDual75mmGun.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipDual75mmGun implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CShipDual75mmGun::CShipDual75mmGun(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip) :
	CMobileAAGun(aLocation, WeaponIdentifierShipDual75mmGun, aAngel, aNormalPositionAngel, DUAL_75MM_GUN_CENTER_X_POSITION - aXOffset, aYOffset + DUAL_75MM_GUN_CENTER_Y_POSITION, aReflected, aConflictSide, aSpeed, DUAL_75MM_GUN_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aShip, JAPANESE_AA_75MM_TYPE88_GUN_ROTATION_SPEED, EXPLOSIVE75MMSHELL_SPEED, JAPANESE_AA_75MM_TYPE88_MIN_RANGE,
			JAPANESE_AA_75MM_TYPE88_MAX_RANGE)
{
	iXOffsetForTurret = aXOffset;
	iYOffsetForTurret = aYOffset;
	iTurrentBaseOffsetAngle = 0;
}

CShipDual75mmGun::~CShipDual75mmGun()
{
	if(iLeftGun)
	{
		delete iLeftGun;
		iLeftGun = NULL;
	}
	if(iMiddleGun)
	{
		delete iMiddleGun;
		iMiddleGun = NULL;
	}
	if(iRightGun)
	{
		delete iRightGun;
		iRightGun = NULL;
	}
	if(iTurretTextureObject)
	{
		delete iTurretTextureObject;
		iTurretTextureObject = NULL;
	}
}

CShipDual75mmGun* CShipDual75mmGun::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank)
{
	CShipDual75mmGun* self = new CShipDual75mmGun(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank);
	self->ConstructL(false);
	return self;
}

CShipDual75mmGun* CShipDual75mmGun::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank, TBool aUseCamoTexture)
{
	CShipDual75mmGun* self = new CShipDual75mmGun(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank);
	self->ConstructL(aUseCamoTexture);
	return self;
}

void CShipDual75mmGun::ConstructL(TBool aUseCamoTexture)
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunDual75mmGunFire), TIntFloat::Convert(DUAL_75MM_GUN_CANNON_GUN_WIDTH), TIntFloat::Convert(DUAL_75MM_GUN_CANNON_GUN_HEIGHT), DUAL_75MM_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	
	lLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
	if(aUseCamoTexture)
		iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunCamoflageDual75mmGunTurret), TIntFloat::Convert(DUAL_75MM_GUN_WIDTH), TIntFloat::Convert(DUAL_75MM_GUN_HEIGHT), DUAL_75MM_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	else
		iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunDual75mmGunTurret), TIntFloat::Convert(DUAL_75MM_GUN_WIDTH), TIntFloat::Convert(DUAL_75MM_GUN_HEIGHT), DUAL_75MM_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);

	iLeftGun = CGunAA75mm::New(&iWeaponCoordinate, &iRealAngle,DUAL_75MM_GUN_LEFT_GUN_X_POSITION, DUAL_75MM_GUN_LEFT_GUN_Y_POSITION,iWeaponReflectedOverYAxis,iConflictSide);
	iMiddleGun = CGunAA75mm::New(&iWeaponCoordinate, &iRealAngle,DUAL_75MM_GUN_MIDDLE_GUN_X_POSITION, DUAL_75MM_GUN_MIDDLE_GUN_Y_POSITION,iWeaponReflectedOverYAxis,iConflictSide);
	iRightGun = CGunAA75mm::New(&iWeaponCoordinate, &iRealAngle,DUAL_75MM_GUN_RIGHT_GUN_X_POSITION, DUAL_75MM_GUN_RIGHT_GUN_Y_POSITION,iWeaponReflectedOverYAxis,iConflictSide);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100020;//since we use only one image, only call method once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGunDual75mmGunFire);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 516;
	lDefaultRequest.Width = DUAL_75MM_GUN_CANNON_GUN_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	SBackCannonAIWaitingCase lWaitingCase;
  lWaitingCase.FrameDelay = JAPANESE_AA_75MM_TYPE88_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE;
  lWaitingCase.FrameDelayAfterShot = JAPANESE_AA_75MM_TYPE88_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE_AFTER_SHOT;
	iBackCannonAI = new CBackCannonAI(this, lWaitingCase, 4, GUNAA75MM_FRAMES_BETWEEN_FIRE);
}

//---------------------------- functions -----------------------------------//

void CShipDual75mmGun::Draw()
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

void CShipDual75mmGun::Rotate(TInt aDegree, TPointIntFloat aLocation)
{
	iTurrentBaseOffsetAngle += aDegree;
	TInt lAngleWithReflection = iTurrentBaseOffsetAngle;
	if(!(*iWeaponReflectedOverYAxis)) //ship faces to the left
		lAngleWithReflection += 180; //need to add since object might be reflected or not
	iTurretTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(lAngleWithReflection, *iWeaponReflectedOverYAxis, aLocation);
}

void CShipDual75mmGun::Shoot(TInt aRange)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iLeftGun->GetIsReadyToShoot() && iMiddleGun->GetIsReadyToShoot() && iRightGun->GetIsReadyToShoot())
	{
		iReadyToShoot = false;

		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_75MM_TYPE88_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGunDual75mmGunFire);
		lFiringRequest.TotalImages = 4;
		lFiringRequest.TotalLength = 516;
		lFiringRequest.Width = DUAL_75MM_GUN_CANNON_GUN_WIDTH;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));
		
	  SRandomFiringOffset lOffset;
	  lOffset.AngleOffset= CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_LOWER_BOUND,JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_UPPER_BOUND);
	  lOffset.RangeOffset= CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_LOWER_BOUND,JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_UPPER_BOUND);
	  iLeftGun->Shoot(aRange+lOffset.RangeOffset,lOffset.AngleOffset);
	  lOffset.AngleOffset= CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_LOWER_BOUND,JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_UPPER_BOUND);
	  lOffset.RangeOffset= CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_LOWER_BOUND,JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_UPPER_BOUND);
	  iMiddleGun->Shoot(aRange+lOffset.RangeOffset,lOffset.AngleOffset);
	  lOffset.AngleOffset= CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_LOWER_BOUND,JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_UPPER_BOUND);
	  lOffset.RangeOffset= CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_LOWER_BOUND,JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_UPPER_BOUND);
	  iRightGun->Shoot(aRange+lOffset.RangeOffset,lOffset.AngleOffset);
	}
}

void CShipDual75mmGun::UpdateAdditionalWeapons()
{
	iLeftGun->Update();
	iMiddleGun->Update();
	iRightGun->Update();
}

void CShipDual75mmGun::InitilizeShipDual75mmGun()
{
	CShipDual75mmGun::LoadShipDual75mmGunTextures();
}

void CShipDual75mmGun::RemoveShipDual75mmGun()
{
	CShipDual75mmGun::UnloadShipDual75mmGunTextures();
}

void CShipDual75mmGun::InitilizeCamoShipDual75mmGun()
{
	CShipDual75mmGun::LoadCamoShipDual75mmGunTextures();
}

void CShipDual75mmGun::RemoveCamoShipDual75mmGun()
{
	CShipDual75mmGun::UnloadCamoShipDual75mmGunTextures();
}

void CShipDual75mmGun::LoadShipDual75mmGunTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunDual75mmGunFire);
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunDual75mmGunTurret);
}

void CShipDual75mmGun::UnloadShipDual75mmGunTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunDual75mmGunFire);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunDual75mmGunTurret);
}

void CShipDual75mmGun::LoadCamoShipDual75mmGunTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunCamoflageDual75mmGunTurret);
}

void CShipDual75mmGun::UnloadCamoShipDual75mmGunTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunCamoflageDual75mmGunTurret);
}

void CShipDual75mmGun::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileAAGunOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iXOffsetForTurret);
	aOutputStream.WriteInt32(iYOffsetForTurret);
	aOutputStream.WriteInt32(iTurrentBaseOffsetAngle);
	iLeftGun->SaveOnDisk(aOutputStream);
	iMiddleGun->SaveOnDisk(aOutputStream);
	iRightGun->SaveOnDisk(aOutputStream);
	iTurretTextureObject->SaveOnDisk(aOutputStream);
}

void CShipDual75mmGun::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileAAGunFromDisk(aReadStream);
	iXOffsetForTurret = aReadStream.ReadInt32();
	iYOffsetForTurret = aReadStream.ReadInt32();
	iTurrentBaseOffsetAngle = aReadStream.ReadInt32();
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iLeftGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iMiddleGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iRightGun->LoadFromDisk(aReadStream);
	iTurretTextureObject->LoadFromDisk(aReadStream);
}

