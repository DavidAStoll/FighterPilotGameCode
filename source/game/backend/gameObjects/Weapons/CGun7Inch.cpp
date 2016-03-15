/*
 ============================================================================
 Name		: Gun7Inch.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGun7Inch implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/core/utility/CRandomGenerator.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CGun7Inch::CGun7Inch(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank,TBool aUseCamoTextures) :
	CMobileRotatingWeapon(aLocation, WeaponIdentifierShip7InchGun, aAngel, aNormalPositionAngel, INCH_7_GUN_CENTER_X_POSITION - aXOffset, aYOffset + INCH_7_GUN_CENTER_Y_POSITION, aReflected, aConflictSide, aSpeed, INCH_7_GUN_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aTank, INCH_7_GUN_ROTATION_SPEED, 0, INCH_7_GUN_MIN_RANGE,
			INCH_7_GUN_MAX_RANGE)
{
	iXOffsetForTurret = aXOffset;
	iYOffsetForTurret = aYOffset;
	iTurrentBaseOffsetAngle = 0;
	iUseCamoTexture = aUseCamoTextures;
	iGunAI = NULL;
}

CGun7Inch::~CGun7Inch()
{
	if(iGun)
	{
		delete iGun;
		iGun = NULL;
	}
	if(iGunAI)
	{
		delete iGunAI;
		iGunAI = NULL;
	}
	if(iTurretTextureObject)
	{
		delete iTurretTextureObject;
		iTurretTextureObject = NULL;
	}
}

CGun7Inch* CGun7Inch::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank, TInt aNumberOfMuzzles, TBool aUseCamoTextures)
{
	CGun7Inch* self = new CGun7Inch(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank, aUseCamoTextures);
	self->ConstructL(aNumberOfMuzzles);
	return self;
}

void CGun7Inch::ConstructL(TInt aNumberOfMuzzles)
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	if(iUseCamoTexture)
		iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunCamoflage7InchGunFire), TIntFloat::Convert(INCH_7_GUN_CANNON_GUN_WIDTH), TIntFloat::Convert(INCH_7_GUN_CANNON_GUN_HEIGHT), INCH_7_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	else
		iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGun7InchGunFire), TIntFloat::Convert(INCH_7_GUN_CANNON_GUN_WIDTH), TIntFloat::Convert(INCH_7_GUN_CANNON_GUN_HEIGHT), INCH_7_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	
	lLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
	if(iUseCamoTexture)
		iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGunCamoflage7InchGunTurret), TIntFloat::Convert(INCH_7_GUN_WIDTH), TIntFloat::Convert(INCH_7_GUN_HEIGHT), INCH_7_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	else
		iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGun7InchGunTurret), TIntFloat::Convert(INCH_7_GUN_WIDTH), TIntFloat::Convert(INCH_7_GUN_HEIGHT), INCH_7_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	iGun = CExplosive7InchGun::New(false, &iWeaponCoordinate, &iRealAngle, INCH_7_GUN_PROJECTILE_X_POSITION,INCH_7_GUN_PROJECTILE_Y_POSITION, iWeaponReflectedOverYAxis, iConflictSide);

	//adding turning sound for the turret
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Turret_Turn, GetRelativeWeaponPosition());
	
	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100020;//since we use only one image, only call method once in a while
	if(iUseCamoTexture)
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGunCamoflage7InchGunFire);
	else
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGun7InchGunFire);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = INCH_7_GUN_CANNON_GUN_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	SShipArteriallyAIWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = INCH_7_GUN_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH;
	lWaitingCase.FrameDelayAfterShot = INCH_7_GUN_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_AFTER_SHOT;
	iGunAI = CShipArteriallyAI::New(this, lWaitingCase, aNumberOfMuzzles, GFRAMES_PER_SECOND_BACKEND);
}

//---------------------------- functions -----------------------------------//

void CGun7Inch::Draw()
{
	if(!iDoNotDraw && !iIsDisabled && iTextureObject)
	{
		//UpdateMobileRotatingWeapon();
		iTextureObject->DrawArrays();
		//draw turret as well
		TPointIntFloat lLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
		Rotate(0, lLocation);
		iTurretTextureObject->DrawArrays();
	}
}

void CGun7Inch::Update()
{
	CMobileRotatingWeapon::Update();
	//also run animation player if possible
	if(iAnimationPlayer)
		iAnimationPlayer->Update();

	iGun->Update();
	//run AI for AA Gun
	if(iGunAI && iMoveableObject->IsAlive())
		iGunAI->Update();//don't let AI run if aircraft is dead
}

void CGun7Inch::ChangeGunAngle(TInt aAngleChange)
{
	TPointIntFloat lGunLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
	iSoundObject->PlayCheckPoint(lGunLocation);
	if(*iWeaponReflectedOverYAxis)
		iOffSetAngleFromNormalPosition -= aAngleChange;
	else
		iOffSetAngleFromNormalPosition += aAngleChange;
}

void CGun7Inch::Rotate(TInt aDegree, TPointIntFloat aLocation)
{
	iTurrentBaseOffsetAngle += aDegree;
	TInt lAngleWithReflection = iTurrentBaseOffsetAngle;
	if(!(*iWeaponReflectedOverYAxis)) //ship faces to the left
		lAngleWithReflection += 180; //need to add since object might be reflected or not
	iTurretTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(lAngleWithReflection, *iWeaponReflectedOverYAxis, aLocation);
}

void CGun7Inch::ShootWithSpeed(TIntFloat aSpeed)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iGun->GetIsReadyToShoot())
	{
		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_CONCRETE_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		if(iUseCamoTexture)
			lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGunCamoflage7InchGunFire);
		else
			lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGun7InchGunFire);
		lFiringRequest.TotalImages = 4;
		lFiringRequest.TotalLength = 512;
		lFiringRequest.Width = INCH_7_GUN_CANNON_GUN_WIDTH;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));
		//shoot projectile
		iGun->ShootWithSpeed(aSpeed);
	}
}

CShipArteriallyAI* CGun7Inch::GetShipArteriallyAI()
{
	return iGunAI;
}

void CGun7Inch::UpdateAdditionalWeapons()
{
	iGun->Update();
}

void CGun7Inch::InitilizeGun7Inch()
{
	CGun7Inch::LoadGun7InchTextures();
}

void CGun7Inch::RemoveGun7Inch()
{
	CGun7Inch::UnloadGun7InchTextures();
}

void CGun7Inch::InitilizeCamoGun7Inch()
{
	CGun7Inch::LoadCamoGun7InchTextures();
}

void CGun7Inch::RemoveCamoGun7Inch()
{
	CGun7Inch::UnloadCamoGun7InchTextures();
}

void CGun7Inch::LoadGun7InchTextures()
{

	CGame::Game->iImageStore->LoadImage(EImageIdShipGun7InchGunFire);
	CGame::Game->iImageStore->LoadImage(EImageIdShipGun7InchGunTurret);
}

void CGun7Inch::UnloadGun7InchTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGun7InchGunFire);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGun7InchGunTurret);
}

void CGun7Inch::LoadCamoGun7InchTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunCamoflage7InchGunFire);
	CGame::Game->iImageStore->LoadImage(EImageIdShipGunCamoflage7InchGunTurret);
}

void CGun7Inch::UnloadCamoGun7InchTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunCamoflage7InchGunFire);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGunCamoflage7InchGunTurret);
}

void CGun7Inch::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileRotatingWeaponOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iXOffsetForTurret);
	aOutputStream.WriteInt32(iYOffsetForTurret);
	aOutputStream.WriteInt32(iTurrentBaseOffsetAngle);
	aOutputStream.WriteInt32(iUseCamoTexture);
	iGun->SaveOnDisk(aOutputStream);
	iGunAI->SaveOnDisk(aOutputStream);
	iTurretTextureObject->SaveOnDisk(aOutputStream);
}

void CGun7Inch::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileRotatingWeaponFromDisk(aReadStream);
	iXOffsetForTurret = aReadStream.ReadInt32();
	iYOffsetForTurret = aReadStream.ReadInt32();
	iTurrentBaseOffsetAngle = aReadStream.ReadInt32();
	iUseCamoTexture = aReadStream.ReadInt32();
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iGun->LoadFromDisk(aReadStream);
	iGunAI->LoadFromDisk(aReadStream);
	iTurretTextureObject->LoadFromDisk(aReadStream);
}

