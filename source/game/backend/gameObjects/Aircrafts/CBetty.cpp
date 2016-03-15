/*
 ============================================================================
 Name		: Betty.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBetty implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

SBoundingCollisionPoint* CBetty::CollisionPoints = NULL;

CBetty::CBetty(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAircraft(BETTY_Z_VALUE, NULL, NULL, aLocation, aAttributes, aCurrentAcceleration, TIntFloat::Convert(BETTY_MAX_ACCELERATION), aFlying, BETTY_TURN_TIME_FACTOR, BETTY_ANGLE_CHANGE_TIME_FACTOR)
{
	iAircraftType = EAircraftTypeHeavyBomber;
	iAircraftChangeDownAngleExtraSpeed = TIntFloat::Convert(HEAVY_BOMBER_AIRCARFT_CHANGE_ANGLE_SPEED_CHANGE_DOWN);//usually is always the same, but for bigger planes like bomber it might be more
}

CBetty::~CBetty()
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

CBetty* CBetty::New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BETTY_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesPlane | EObjectTypesBigPlane;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BETTY_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierBetty;

	CBetty* self = new CBetty(aCurrentAcceleration, aFlying, aLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CBetty::ConstructL()
{
	//Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdAircraftBettyNormalSequence), BETTY_WIDTH, BETTY_HEIGHT, iZCor, 0, 0.68359375, 0, 0.765625);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, 8, 8, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBetty::CollisionPoints);

	//create Weapons
	iFirstHeavyBombWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Betty1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, BETTY_FIRST_1000KG_BOMB_POSITION_X, BETTY_FIRST_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iSecondHeavyBombWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Betty1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, BETTY_SECOND_1000KG_BOMB_POSITION_X, BETTY_SECOND_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iFirstMediumBombWeapon = CWeaponBomb500Kg::New(CGameObjectDefaultValues::Betty500KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, BETTY_FIRST_500KG_BOMB_POSITION_X, BETTY_FIRST_500KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iSecondMediumBombWeapon = CWeaponBomb500Kg::New(CGameObjectDefaultValues::Betty500KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, BETTY_SECOND_500KG_BOMB_POSITION_X, BETTY_SECOND_500KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);

	//create cannons to protect the plane
	iFrontCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 80, BETTY_FRONT_CANNON_POSITION_X, BETTY_FRONT_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);
	iTopCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 170, BETTY_TOP_CANNON_POSITION_X, BETTY_TOP_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);
	iBackCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 260, BETTY_BACK_CANNON_POSITION_X, BETTY_BACK_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);
	iBottomCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, -10, BETTY_BOTTOM_CANNON_POSITION_X, BETTY_BOTTOM_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 160, this);

	//create smoke creators
	iSmokeCreatorEngine1 = CAircraftSmokeCreator::New(false, this, BETTY_SMONE_CREATOR_ENGINE1_POSITION_X, BETTY_SMONE_CREATOR_ENGINE1_POSITION_Y, BIG_AIRCRAFT_FRAMES_UNTIL_SMOKE_DISAPPEARS);
	iSmokeCreatorEngine2 = CAircraftSmokeCreator::New(true, this, BETTY_SMONE_CREATOR_ENGINE2_POSITION_X, BETTY_SMONE_CREATOR_ENGINE2_POSITION_Y, BIG_AIRCRAFT_FRAMES_UNTIL_SMOKE_DISAPPEARS);

	//create Bombinb AI for plane
	iBombingAI = new CHeavyBomberAI(this, BETTY_MIDDLE_POSITION_OF_BOMBS);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftBettyNormalSequence);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = BETTY_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.765625;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	if(iObjectReflected)
	{
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
	iSoundObject = CSoundObject::New(ESoundFileId_Airplane_Engine_Middle, iHitBox);
	iSoundObject->PlayRepeatHitBox();
}

void CBetty::RemoveBombs()
{
	iFirstHeavyBombWeapon->SetAmmunitionToValue(0);
	iFirstMediumBombWeapon->SetAmmunitionToValue(0);
	iSecondHeavyBombWeapon->SetAmmunitionToValue(0);
	iSecondMediumBombWeapon->SetAmmunitionToValue(0);
}

void CBetty::LoadBettyTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftBettyNormalSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftBettyCrashed);
}

void CBetty::UnloadBettyTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftBettyNormalSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftBettyCrashed);
}

void CBetty::CreateBettyCollisionPointsL()
{
	CBetty::CollisionPoints = new SBoundingCollisionPoint[8];
	CBetty::CollisionPoints[0].XOffset = 1;
	CBetty::CollisionPoints[0].YOffset = 25;
	CBetty::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[1].XOffset = 33;
	CBetty::CollisionPoints[1].YOffset = 10;
	CBetty::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[2].XOffset = 280;
	CBetty::CollisionPoints[2].YOffset = 12;
	CBetty::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[3].XOffset = 337;
	CBetty::CollisionPoints[3].YOffset = 22;
	CBetty::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[4].XOffset = 330;
	CBetty::CollisionPoints[4].YOffset = 55;
	CBetty::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[5].XOffset = 60;
	CBetty::CollisionPoints[5].YOffset = 60;
	CBetty::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[6].XOffset = 2;
	CBetty::CollisionPoints[6].YOffset = 36;
	CBetty::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CBetty::CollisionPoints[7].XOffset = 3;
	CBetty::CollisionPoints[7].YOffset = 25;
	CBetty::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
}

void CBetty::RemoveBettyCollisionPoints()
{
	if(CBetty::CollisionPoints)
	{
		delete[] CBetty::CollisionPoints;
		CBetty::CollisionPoints = NULL;
	}
}

void CBetty::AssignBombingMission(SHeavyBombingMission aMission)
{
	iBombingAI->AssignBombingMission(aMission);
}

void CBetty::InitilizeBetty()
{
	CBetty::LoadBettyTexturesL();
	CBetty::CreateBettyCollisionPointsL();
}

void CBetty::RemoveBetty()
{
	CBetty::UnloadBettyTextures();
	CBetty::RemoveBettyCollisionPoints();
}

void CBetty::UpdateAircraft()
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

void CBetty::Fire1()
{
	iFirstHeavyBombWeapon->Shoot();
	iSecondHeavyBombWeapon->Shoot();
}

void CBetty::Fire2()
{
	iFirstMediumBombWeapon->Shoot();
	iSecondMediumBombWeapon->Shoot();
}

TBool CBetty::Fire1HasAmmunitionLeft()
{
	if(iFirstHeavyBombWeapon->GetAmmunitionLeft() > 0 && iSecondHeavyBombWeapon->GetAmmunitionLeft() > 0)
		return true;
	else
		return false;
}

TBool CBetty::Fire2HasAmmunitionLeft()
{
	if(iFirstMediumBombWeapon->GetAmmunitionLeft() > 0 && iSecondMediumBombWeapon->GetAmmunitionLeft() > 0)
		return true;
	else
		return false;
}

CWeapon* CBetty::GetSecondaryWeapon()
{
	return iFirstMediumBombWeapon;
}

CWeapon* CBetty::GetPrimaryWeapon()
{
	return iFirstHeavyBombWeapon;
}

void CBetty::Draw()
{
	//draw Plane
	if(!iDoNotDraw)
	{
		if(!iCrashed)
		{
			iFrontCannon->Draw();
			iBackCannon->Draw();
			iTopCannon->Draw();
			iBottomCannon->Draw();
		}
		iTextureObject->DrawArrays();//uses the currently assianged texture
	}
}

void CBetty::PlaneCrash()
{
	//disable cannons
	iFrontCannon->SetDisabled(true);
	iBackCannon->SetDisabled(true);
	iBottomCannon->SetDisabled(true);
	iTopCannon->SetDisabled(true);
	//assign crash image
	iTextureObject->SetHeight(TIntFloat::Convert(BETTY_CRASH_STATE_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(BETTY_CRASH_STATE_WIDTH));
	iTextureObject->RotateVertexes(iTextureObject->GetRotationalAngel());
	//update hitbox as well
	iHitBox->SetNewPrimitiveHitBox(TSize(BETTY_CRASH_STATE_WIDTH, BETTY_CRASH_STATE_HEIGHT));
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 10;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftBettyCrashed);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = BETTY_CRASH_STATE_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.5859375;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
	iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);

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

void CBetty::SaveOnDisk(CFileWriteStream &aOutputStream)
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

void CBetty::LoadFromDisk(CFileReadStream &aInputStream)
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
