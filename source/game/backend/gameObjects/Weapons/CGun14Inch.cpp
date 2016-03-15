/*
 ============================================================================
 Name		: Gun14Inch.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGun14Inch implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/core/utility/CRandomGenerator.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CGun14Inch::CGun14Inch(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank) :
	CMobileRotatingWeapon(aLocation, WeaponIdentifierShip14InchGun, aAngel, aNormalPositionAngel, INCH_14_GUN_CENTER_X_POSITION - aXOffset, aYOffset + INCH_14_GUN_CENTER_Y_POSITION, aReflected, aConflictSide, aSpeed, INCH_14_GUN_FRAMES_BETWEEN_FIRE, aMinAngle, aMaxAngle, aTank, INCH_14_GUN_ROTATION_SPEED, 0, INCH_14_GUN_MIN_RANGE,
			INCH_14_GUN_MAX_RANGE)
{
	iXOffsetForTurret = aXOffset;
	iYOffsetForTurret = aYOffset;
	iTurrentBaseOffsetAngle = 0;
	iGunAI = NULL;
}

CGun14Inch::~CGun14Inch()
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

CGun14Inch* CGun14Inch::New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank, TInt aNumberOfMuzzles)
{
	CGun14Inch* self = new CGun14Inch(aLocation, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aMinAngle, aMaxAngle, aTank);
	self->ConstructL(aNumberOfMuzzles);
	return self;
}

void CGun14Inch::ConstructL(TInt aNumberOfMuzzles)
{
	TPointIntFloat lLocation = GetRelativeWeaponPosition();
	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGun14InchGunFire), TIntFloat::Convert(INCH_14_GUN_CANNON_GUN_WIDTH), TIntFloat::Convert(INCH_14_GUN_CANNON_GUN_HEIGHT), INCH_14_GUN_Z_COORDINATE, 0, 1/*Not important*/, 0, 1);
	
	lLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
	iTurretTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdShipGun14InchGunTurret), TIntFloat::Convert(INCH_14_GUN_WIDTH), TIntFloat::Convert(INCH_14_GUN_HEIGHT), INCH_14_GUN_Z_COORDINATE, 0, 0.71875, 0, 0.71875);
	iGun = CExplosive14InchGun::New(false, &iWeaponCoordinate, &iRealAngle,  INCH_14_GUN_PROJECTILE_X_POSITION, INCH_14_GUN_PROJECTILE_Y_POSITION, iWeaponReflectedOverYAxis, iConflictSide);

	//adding turning sound for the turret
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Turret_Turn, GetRelativeWeaponPosition());
	
	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100020;//since we use only one image, only call method once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGun14InchGunFire);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = INCH_14_GUN_CANNON_GUN_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.75;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	SShipArteriallyAIWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = INCH_14_GUN_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH;
	lWaitingCase.FrameDelayAfterShot = INCH_14_GUN_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_AFTER_SHOT;
	iGunAI = CShipArteriallyAI::New(this, lWaitingCase, aNumberOfMuzzles, GFRAMES_PER_SECOND_BACKEND);
}

//---------------------------- functions -----------------------------------//

void CGun14Inch::Draw()
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

void CGun14Inch::Update()
{
	CMobileRotatingWeapon::Update();

	iGun->Update();
	//run AI for AA Gun
	if(iGunAI && iMoveableObject->IsAlive())
		iGunAI->Update();//don't let AI run if aircraft is dead
}

void CGun14Inch::ChangeGunAngle(TInt aAngleChange)
{
		TPointIntFloat lGunLocation = CMath::GetPointRelativeToCoordinates(*iCoordinate, *iWeaponReflectedOverYAxis, *iAngel, iXOffsetForTurret, iYOffsetForTurret);
		iSoundObject->PlayCheckPoint(lGunLocation);
	if(*iWeaponReflectedOverYAxis)
		iOffSetAngleFromNormalPosition -= aAngleChange;
	else
		iOffSetAngleFromNormalPosition += aAngleChange;
}

void CGun14Inch::Rotate(TInt aDegree, TPointIntFloat aLocation)
{
	iTurrentBaseOffsetAngle += aDegree;
	TInt lAngleWithReflection = iTurrentBaseOffsetAngle;
	if(!(*iWeaponReflectedOverYAxis)) //ship faces to the left
		lAngleWithReflection += 180; //need to add since object might be reflected or not
	iTurretTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(lAngleWithReflection, *iWeaponReflectedOverYAxis,aLocation);
}

void CGun14Inch::ShootWithSpeed(TIntFloat aSpeed)
{
	if(iIsDisabled)
	{
		//maybe play a sound
	}else if(iReadyToShoot && iGun->GetIsReadyToShoot())
	{
		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_CONCRETE_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdShipGun14InchGunFire);
		lFiringRequest.TotalImages = 4;
		lFiringRequest.TotalLength = 1024;
		lFiringRequest.Width = INCH_14_GUN_CANNON_GUN_WIDTH;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = 0.75;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));
		//shoot projectile
		iGun->ShootWithSpeed(aSpeed);
	}
}

CShipArteriallyAI* CGun14Inch::GetShipArteriallyAI()
{
	return iGunAI;
}

void CGun14Inch::UpdateAdditionalWeapons()
{
	iGun->Update();
}

void CGun14Inch::InitilizeGun14Inch()
{
	CGun14Inch::LoadGun14InchTextures();
}

void CGun14Inch::RemoveGun14Inch()
{
	CGun14Inch::UnloadGun14InchTextures();
}

void CGun14Inch::LoadGun14InchTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipGun14InchGunFire);
	CGame::Game->iImageStore->LoadImage(EImageIdShipGun14InchGunTurret);
}

void CGun14Inch::UnloadGun14InchTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGun14InchGunFire);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipGun14InchGunTurret);
}

void CGun14Inch::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileRotatingWeaponOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iXOffsetForTurret);
	aOutputStream.WriteInt32(iYOffsetForTurret);
	aOutputStream.WriteInt32(iTurrentBaseOffsetAngle);
	iGun->SaveOnDisk(aOutputStream);
	iGunAI->SaveOnDisk(aOutputStream);
	iTurretTextureObject->SaveOnDisk(aOutputStream);
}

void CGun14Inch::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileRotatingWeaponFromDisk(aReadStream);
	iXOffsetForTurret = aReadStream.ReadInt32();
	iYOffsetForTurret = aReadStream.ReadInt32();
	iTurrentBaseOffsetAngle = aReadStream.ReadInt32();
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iGun->LoadFromDisk(aReadStream);
	iGunAI->LoadFromDisk(aReadStream);
	iTurretTextureObject->LoadFromDisk(aReadStream);
}

