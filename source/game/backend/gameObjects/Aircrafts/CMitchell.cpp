/*
 ============================================================================
 Name		: Mitchell.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMitchell implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

SBoundingCollisionPoint* CMitchell::CollisionPoints = NULL;

CMitchell::CMitchell(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAircraft(MITCHELL_Z_VALUE, NULL, NULL, aLocation, aAttributes, aCurrentAcceleration, TIntFloat::Convert(MITCHELL_MAX_ACCELERATION), aFlying, MITCHELL_TURN_TIME_FACTOR, MITCHELL_ANGLE_CHANGE_TIME_FACTOR)
{
	iAircraftType = EAircraftTypeHeavyBomber;
	iAircraftChangeDownAngleExtraSpeed = TIntFloat::Convert(HEAVY_BOMBER_AIRCARFT_CHANGE_ANGLE_SPEED_CHANGE_DOWN);//usually is always the same, but for bigger planes like bomber it might be more
}

CMitchell::~CMitchell()
{
	if(iFrontCannon)
		delete iFrontCannon;
	if(iTopCannon)
		delete iTopCannon;
	if(iBackCannon)
		delete iBackCannon;
	if(iBottomCannon)
		delete iBottomCannon;
	if(iSmokeCreatorEngine1)
		delete iSmokeCreatorEngine1;
	if(iSmokeCreatorEngine2)
		delete iSmokeCreatorEngine2;
	if(iBombingAI)
		delete iBombingAI;
	if(iFirstHeavyBombWeapon)
		delete iFirstHeavyBombWeapon;
	if(iSecondHeavyBombWeapon)
		delete iSecondHeavyBombWeapon;
	if(iFirstMediumBombWeapon)
		delete iFirstMediumBombWeapon;
	if(iSecondMediumBombWeapon)
		delete iSecondMediumBombWeapon;
}

CMitchell* CMitchell::New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = MITCHELL_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesPlane | EObjectTypesBigPlane;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = MITCHELL_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierMitchell;

	CMitchell* self = new CMitchell(aCurrentAcceleration, aFlying, aLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CMitchell::ConstructL()
{
	//Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdAircraftMitchellNormalSequence), MITCHELL_WIDTH, MITCHELL_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, 9, 9, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CMitchell::CollisionPoints);

	//create Weapons
	iFirstHeavyBombWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Mitchell1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, MITCHELL_FIRST_1000KG_BOMB_POSITION_X, MITCHELL_FIRST_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iSecondHeavyBombWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Mitchell1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, MITCHELL_SECOND_1000KG_BOMB_POSITION_X, MITCHELL_SECOND_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iFirstMediumBombWeapon = CWeaponBomb500Kg::New(CGameObjectDefaultValues::Mitchell500KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, MITCHELL_FIRST_500KG_BOMB_POSITION_X, MITCHELL_FIRST_500KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iSecondMediumBombWeapon = CWeaponBomb500Kg::New(CGameObjectDefaultValues::Mitchell500KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, MITCHELL_SECOND_500KG_BOMB_POSITION_X, MITCHELL_SECOND_500KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);

	//create cannons to protect the plane
	iFrontCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 80, MITCHELL_FRONT_CANNON_POSITION_X, MITCHELL_FRONT_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);
	iTopCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 170, MITCHELL_TOP_CANNON_POSITION_X, MITCHELL_TOP_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);
	iBackCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 260, MITCHELL_BACK_CANNON_POSITION_X, MITCHELL_BACK_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);
	iBottomCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, -10, MITCHELL_BOTTOM_CANNON_POSITION_X, MITCHELL_BOTTOM_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);

	//create smoke creators
	iSmokeCreatorEngine1 = CAircraftSmokeCreator::New(false, this, MITCHELL_SMOKE_CREATOR_ENGINE1_POSITION_X, MITCHELL_SMOKE_CREATOR_ENGINE1_POSITION_Y, BIG_AIRCRAFT_FRAMES_UNTIL_SMOKE_DISAPPEARS);
	iSmokeCreatorEngine2 = CAircraftSmokeCreator::New(true, this, MITCHELL_SMOKE_CREATOR_ENGINE2_POSITION_X, MITCHELL_SMOKE_CREATOR_ENGINE2_POSITION_Y, BIG_AIRCRAFT_FRAMES_UNTIL_SMOKE_DISAPPEARS);

	//create Bombinb AI for plane
	iBombingAI = new CHeavyBomberAI(this, MITCHELL_MIDDLE_POSITION_OF_BOMBS);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftMitchellNormalSequence);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = MITCHELL_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.7265625;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	if(iObjectReflected)
	{
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
	iSoundObject = CSoundObject::New(ESoundFileId_Airplane_Engine_Middle, iHitBox);
	iSoundObject->PlayRepeatHitBox();
}

//---------------------------------- functions ---------------------------------------//

void CMitchell::LoadMitchellTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftMitchellNormalSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftMitchellCrashed);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftMitchellTurnSequence);
}

void CMitchell::UnloadMitchellTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftMitchellNormalSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftMitchellCrashed);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftMitchellTurnSequence);
}

void CMitchell::CreateMitchellCollisionPointsL()
{
	CMitchell::CollisionPoints = new SBoundingCollisionPoint[9];
	CMitchell::CollisionPoints[0].XOffset = 14;
	CMitchell::CollisionPoints[0].YOffset = 12;
	CMitchell::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[1].XOffset = 180;
	CMitchell::CollisionPoints[1].YOffset = 13;
	CMitchell::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[2].XOffset = 337;
	CMitchell::CollisionPoints[2].YOffset = 37;
	CMitchell::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[3].XOffset = 308;
	CMitchell::CollisionPoints[3].YOffset = 93;
	CMitchell::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[4].XOffset = 292;
	CMitchell::CollisionPoints[4].YOffset = 61;
	CMitchell::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[5].XOffset = 74;
	CMitchell::CollisionPoints[5].YOffset = 63;
	CMitchell::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[6].XOffset = 52;
	CMitchell::CollisionPoints[6].YOffset = 51;
	CMitchell::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[7].XOffset = 3;
	CMitchell::CollisionPoints[7].YOffset = 37;
	CMitchell::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CMitchell::CollisionPoints[8].XOffset = 13;
	CMitchell::CollisionPoints[8].YOffset = 11;
	CMitchell::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
}

void CMitchell::RemoveMitchellCollisionPoints()
{
	if(CMitchell::CollisionPoints)
	{
		delete[] CMitchell::CollisionPoints;
		CMitchell::CollisionPoints = NULL;
	}
}

void CMitchell::InitilizeMitchell()
{
	CMitchell::LoadMitchellTexturesL();
	CMitchell::CreateMitchellCollisionPointsL();
}

void CMitchell::AssignBombingMission(SHeavyBombingMission aMission)
{
	iBombingAI->AssignBombingMission(aMission);
}

CHeavyBomberAI* CMitchell::GetHeavyBomberAI()
{
	return iBombingAI;
}

void CMitchell::RemoveMitchell()
{
	CMitchell::UnloadMitchellTextures();
	CMitchell::RemoveMitchellCollisionPoints();
}

void CMitchell::UpdateAircraft()
{
	UpdateSpeed();
	iFrontCannon->Update();
	iBackCannon->Update();
	iTopCannon->Update();
	iBottomCannon->Update();
	iSmokeCreatorEngine1->Update();
	iSmokeCreatorEngine2->Update();
	iFirstHeavyBombWeapon->Update();
	iSecondHeavyBombWeapon->Update();
	iFirstMediumBombWeapon->Update();
	iSecondMediumBombWeapon->Update();
	iBombingAI->Update();
	iAnimationPlayer->Update();
}

void CMitchell::BeforeTurnSettings()
{
	//disable bombs
	iFirstHeavyBombWeapon->SetDisabled(true);
	iSecondHeavyBombWeapon->SetDisabled(true);
	iFirstMediumBombWeapon->SetDisabled(true);
	iSecondMediumBombWeapon->SetDisabled(true);
	//disable guns
	iFrontCannon->SetDisabled(true);
	iBackCannon->SetDisabled(true);
	iTopCannon->SetDisabled(true);
	iBottomCannon->SetDisabled(true);
}

void CMitchell::AfterTurnSettings()
{
	//disable bombs
	iFirstHeavyBombWeapon->SetDisabled(false);
	iSecondHeavyBombWeapon->SetDisabled(false);
	iFirstMediumBombWeapon->SetDisabled(false);
	iSecondMediumBombWeapon->SetDisabled(false);
	//disable guns
	iFrontCannon->SetDisabled(false);
	iBackCannon->SetDisabled(false);
	iTopCannon->SetDisabled(false);
	iBottomCannon->SetDisabled(false);
}

void CMitchell::Fire1()
{
	iFirstHeavyBombWeapon->Shoot();
	iSecondHeavyBombWeapon->Shoot();
}

void CMitchell::Fire2()
{
	iFirstMediumBombWeapon->Shoot();
	iSecondMediumBombWeapon->Shoot();
}

TBool CMitchell::Fire1HasAmmunitionLeft()
{
	if(iFirstHeavyBombWeapon->GetAmmunitionLeft() > 0 && iSecondHeavyBombWeapon->GetAmmunitionLeft() > 0)
		return true;
	else
		return false;
}

TBool CMitchell::Fire2HasAmmunitionLeft()
{
	if(iFirstMediumBombWeapon->GetAmmunitionLeft() > 0 && iSecondMediumBombWeapon->GetAmmunitionLeft() > 0)
		return true;
	else
		return false;
}

CWeapon* CMitchell::GetSecondaryWeapon()
{
	return iFirstMediumBombWeapon;
}

CWeapon* CMitchell::GetPrimaryWeapon()
{
	return iFirstHeavyBombWeapon;
}

void CMitchell::Draw()
{
	if(!iDoNotDraw)
	{
		if(!iCrashed)
		{
			iFrontCannon->Draw();
			iBackCannon->Draw();
			iTopCannon->Draw();
			iBottomCannon->Draw();
		}
		//draw Plane
		iTextureObject->DrawArrays();//uses the currently assianged texture
	}
}

void CMitchell::PlaneCrash()
{
	//disable cannons
	iFrontCannon->SetDisabled(true);
	iBackCannon->SetDisabled(true);
	iBottomCannon->SetDisabled(true);
	iTopCannon->SetDisabled(true);
	//assign crash image
	iTextureObject->SetHeight(TIntFloat::Convert(MITCHELL_CRASH_STATE_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(MITCHELL_CRASH_STATE_WIDTH));
	iTextureObject->RotateVertexes(iTextureObject->GetRotationalAngel());
	//update hitbox as well
	iHitBox->SetNewPrimitiveHitBox(TSize(MITCHELL_CRASH_STATE_WIDTH, MITCHELL_CRASH_STATE_HEIGHT));
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 10;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftMitchellCrashed);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = MITCHELL_CRASH_STATE_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.5703125;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
	iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);

	//explode
	//since they always explode
	iFirstHeavyBombWeapon->ResetAmmunitionToMaxValue();
	iSecondHeavyBombWeapon->ResetAmmunitionToMaxValue();

	iFirstHeavyBombWeapon->Explode();
	iSecondHeavyBombWeapon->Explode();
	//stops the engine sound from playing
	StopEngineSound();
	//play the collision sound
	iSoundObject->PlayHitBox(ESoundFileId_Plane_Crash_On_Land);
	//play the engine stop sound
	iSoundObject->PlayHitBox(ESoundFileId_Airplane_Engine_End);
}

void CMitchell::PlayTurnAnimation()
{
	//start fire sequence
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = iFramesPerTurnImage;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftMitchellTurnSequence);
	lDefaultRequest.TotalImages = 7;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = 256;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.5;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
}

void CMitchell::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	AircraftGameObjectSaveContentToDisk(aOutputStream);
	iFirstHeavyBombWeapon->SaveOnDisk(aOutputStream);
	iSecondHeavyBombWeapon->SaveOnDisk(aOutputStream);
	iFirstMediumBombWeapon->SaveOnDisk(aOutputStream);
	iSecondMediumBombWeapon->SaveOnDisk(aOutputStream);
	iSmokeCreatorEngine1->SaveOnDisk(aOutputStream);
	iSmokeCreatorEngine2->SaveOnDisk(aOutputStream);
	iFrontCannon->SaveOnDisk(aOutputStream);
	iTopCannon->SaveOnDisk(aOutputStream);
	iBackCannon->SaveOnDisk(aOutputStream);
	iBottomCannon->SaveOnDisk(aOutputStream);
	iBombingAI->SaveOnDisk(aOutputStream);
}

void CMitchell::LoadFromDisk(CFileReadStream &aInputStream)
{
	AircraftGameObjectLoadContentFromDisk(aInputStream);
	TWeaponIdentifier lFirstHeavyBomberWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFirstHeavyBombWeapon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSecondHeavyBomberWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSecondHeavyBombWeapon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lFirstMediumBomberWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFirstMediumBombWeapon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSecondMediumBomberWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSecondMediumBombWeapon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lFirstSomkeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorEngine1->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSecondSomkeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorEngine2->LoadFromDisk(aInputStream);
	TWeaponIdentifier lFrontCannon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontCannon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lTopCannon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iTopCannon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lBackCannon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackCannon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lBottomCannon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBottomCannon->LoadFromDisk(aInputStream);
	iBombingAI->LoadFromDisk(aInputStream);
}
