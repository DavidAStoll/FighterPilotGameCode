/*
 ============================================================================
 Name		: Val.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CVal implementation
 ============================================================================
 */
//Weapons
#include "includes/game/backend/gameObjects/Aircrafts/CVal.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb1000Kg.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb500Kg.h"
#include "includes/game/backend/gameObjects/Weapons/CStandardRocketWeapon.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb100Kg.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBombCluster.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponTorpedo.h"
#include "includes/game/backend/gameObjects/Weapons/CBackGunMachineGun8mm.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponAircraftMachineGun8mm.h"
#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CVal::CollisionPoints = NULL;
SBoundingCollisionPoint* CVal::CollisionPointForFrontWheel = NULL;
SBoundingCollisionPoint* CVal::CollisionPointForBackWheel = NULL;

CVal::CVal(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAircraft(VAL_Z_VALUE, NULL, NULL, aLocation, aAttributes, aCurrentAcceleration, TIntFloat::Convert(VAL_MAX_ACCELERATION), aFlying, VAL_TURN_TIME_FACTOR, VAL_ANGLE_CHANGE_TIME_FACTOR)
{
	iFrontMachineGun1 = NULL;
	iFrontMachineGun2 = NULL;
	iSecondaryWeapon = NULL;
	iAircraftType = EAircraftTypeDiveBomber;
}

CVal::~CVal()
{
	if(iFrontMachineGun1)
	{
		delete iFrontMachineGun1;
		iFrontMachineGun1 = NULL;
	}
	if(iFrontMachineGun2)
	{
		delete iFrontMachineGun2;
		iFrontMachineGun2 = NULL;
	}
	if(iSecondaryWeapon)
	{
		delete iSecondaryWeapon;
		iSecondaryWeapon = NULL;
	}
	if(iBackCannon)
	{
		delete iBackCannon;
		iBackCannon = NULL;
	}
}

CVal* CVal::New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = VAL_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesPlane | EObjectTypesSmallPlane;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = VAL_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierVal;
	CVal* self = new CVal(aCurrentAcceleration, aFlying, aLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CVal::ConstructL()
{
	//Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdAircraftValNormalSequence), VAL_WIDTH, VAL_HEIGHT, iZCor, 0, 1, 0, 1);
	//create another texture Object for the Gear
	iLandingGearTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdAircraftValLandingGear), VAL_WIDTH, VAL_HEIGHT, iZCor, 0, 0.6640625, 0, 1);
	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftValNormalSequence);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = VAL_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//create standard weapon
	iFrontMachineGun1 = CWeaponAircraftMachineGun8mm::New(&iCoordinates, &iAngle, VAL_FRONT_8MM_MACHINE_GUN_1_POSITION_X, VAL_FRONT_8MM_MACHINE_GUN_1_POSITION_Y,  &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iFrontMachineGun2 = CWeaponAircraftMachineGun8mm::New(&iCoordinates, &iAngle, VAL_FRONT_8MM_MACHINE_GUN_2_POSITION_X, VAL_FRONT_8MM_MACHINE_GUN_2_POSITION_Y,  &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	//create secondary weapon,might differ depending on payload
	iSecondaryWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Val1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_1000KG_BOMB_POSITION_X, VAL_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	//create back cannon for the Val
	iBackCannon = CBackGunMachineGun8mm::New(&iCoordinates, &iAngle, 180, VAL_BACK_CANNON_POSITION_X, VAL_BACK_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 45, this, 0);
	//create the smoke Creator that makes smoke if the life falls too low
	iSmokeCreator1 = CAircraftSmokeCreator::New(true, this, VAL_SMOKE_CREATOR_POSITION_X, VAL_SMOKE_CREATOR_POSITION_Y, SMALL_AIRCRAFT_FRAMES_UNTIL_SMOKE_DISAPPEARS);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, VAL_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, VAL_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CVal::CollisionPoints);

	//create wheels of the airplane
	iFrontWheel = CPlaneWheels::New(CHitBox::New(&iCoordinates, VAL_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, VAL_LANDING_GEAR_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CVal::CollisionPointForFrontWheel), this);
	iBackWheel = CPlaneWheels::New(CHitBox::New(&iCoordinates, VAL_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, VAL_LANDING_GEAR_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CVal::CollisionPointForBackWheel), this);

	if(iObjectReflected)
	{
		iTextureObject->ReflectOverYAxis();
		iLandingGearTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
	iFighterAI = CFighterAI::New(this);
}

//------------------- functions --------------------


void CVal::DrawAdvancedHitBoxL()
{
	iHitBox->DrawAdvancedHitBoxL();
	iFrontWheel->iHitBox->DrawAdvancedHitBoxL();
	iBackWheel->iHitBox->DrawAdvancedHitBoxL();
}

void CVal::SetPosition(TPointIntFloat& aPoint)
{
	iCoordinates = aPoint;
	if(iTextureObject)
	{
		iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
		iLandingGearTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	}
}

void CVal::SetPosition(TPoint& aPoint)
{
	iCoordinates.iX = TIntFloat::Convert(aPoint.iX);
	iCoordinates.iY = TIntFloat::Convert(aPoint.iY);
	if(iTextureObject)
	{
		iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
		iLandingGearTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	}
}

void CVal::LoadValTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftValNormalSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftValLandingGear);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftValCrashed);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftValFireSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftValTurnSequence);
}

void CVal::UnloadValTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftValNormalSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftValLandingGear);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftValCrashed);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftValFireSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftValTurnSequence);
}

void CVal::CreateValCollisionPointsL()
{
	CVal::CollisionPoints = new SBoundingCollisionPoint[VAL_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CVal::CollisionPoints[0].XOffset = 3;
	CVal::CollisionPoints[0].YOffset = 4;
	CVal::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPoints[1].XOffset = 80;
	CVal::CollisionPoints[1].YOffset = 13;
	CVal::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPoints[2].XOffset = 78;
	CVal::CollisionPoints[2].YOffset = 22;
	CVal::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPoints[3].XOffset = 43;
	CVal::CollisionPoints[3].YOffset = 23;
	CVal::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPoints[4].XOffset = 30;
	CVal::CollisionPoints[4].YOffset = 26;
	CVal::CollisionPoints[4].iMovementType = EPointMovementTypeConnectionStartPoint;
	CVal::CollisionPoints[5].XOffset = 19;
	CVal::CollisionPoints[5].YOffset = 22;
	CVal::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPoints[6].XOffset = 7;
	CVal::CollisionPoints[6].YOffset = 20;
	CVal::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPoints[7].XOffset = 5;
	CVal::CollisionPoints[7].YOffset = 4;
	CVal::CollisionPoints[7].iMovementType = EPointMovementTypeConnectionEndPoint; //repeat first point

	//create collisionPoints for wheels
	CVal::CollisionPointForFrontWheel = new SBoundingCollisionPoint[VAL_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS]; //don't really need a moving hitbox, are accurate enough
	CVal::CollisionPointForFrontWheel[0].XOffset = 0;
	CVal::CollisionPointForFrontWheel[0].YOffset = 6;
	CVal::CollisionPointForFrontWheel[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CVal::CollisionPointForFrontWheel[1].XOffset = 2;
	CVal::CollisionPointForFrontWheel[1].YOffset = -2;
	CVal::CollisionPointForFrontWheel[1].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPointForFrontWheel[2].XOffset = 18;
	CVal::CollisionPointForFrontWheel[2].YOffset = 1;
	CVal::CollisionPointForFrontWheel[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CVal::CollisionPointForFrontWheel[3].XOffset = 22;
	CVal::CollisionPointForFrontWheel[3].YOffset = 8;
	CVal::CollisionPointForFrontWheel[3].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPointForFrontWheel[4].XOffset = 0;
	CVal::CollisionPointForFrontWheel[4].YOffset = 6;
	CVal::CollisionPointForFrontWheel[4].iMovementType = EPointMovementTypeNone;

	CVal::CollisionPointForBackWheel = new SBoundingCollisionPoint[VAL_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CVal::CollisionPointForBackWheel[0].XOffset = 74;
	CVal::CollisionPointForBackWheel[0].YOffset = 6;
	CVal::CollisionPointForBackWheel[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CVal::CollisionPointForBackWheel[1].XOffset = 72;
	CVal::CollisionPointForBackWheel[1].YOffset = -2;
	CVal::CollisionPointForBackWheel[1].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPointForBackWheel[2].XOffset = 82;
	CVal::CollisionPointForBackWheel[2].YOffset = 0;
	CVal::CollisionPointForBackWheel[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CVal::CollisionPointForBackWheel[3].XOffset = 85;
	CVal::CollisionPointForBackWheel[3].YOffset = 8;
	CVal::CollisionPointForBackWheel[3].iMovementType = EPointMovementTypeNone;
	CVal::CollisionPointForBackWheel[4].XOffset = 74;
	CVal::CollisionPointForBackWheel[4].YOffset = 6;
	CVal::CollisionPointForBackWheel[4].iMovementType = EPointMovementTypeNone;

}

void CVal::RemoveValCollisionPoints()
{
	if(CVal::CollisionPoints)
	{
		delete[] CVal::CollisionPoints;
		CVal::CollisionPoints = NULL;
	}
	if(CVal::CollisionPointForBackWheel)
	{
		delete[] CVal::CollisionPointForBackWheel;
		CVal::CollisionPointForBackWheel = NULL;
	}
	if(CVal::CollisionPointForFrontWheel)
	{
		delete[] CVal::CollisionPointForFrontWheel;
		CVal::CollisionPointForFrontWheel = NULL;
	}
}

void CVal::InitilizeVal()
{
	CVal::LoadValTexturesL();
	CVal::CreateValCollisionPointsL();
}

void CVal::RemoveVal()
{
	CVal::UnloadValTextures();
	CVal::RemoveValCollisionPoints();
}

void CVal::UpdateAircraft()
{
	iAnimationPlayer->Update();
	iSmokeCreator1->Update();
	if(iFrontMachineGun1)
	{
		iFrontMachineGun1->Update();
	}
	if(iFrontMachineGun2)
	{
		iFrontMachineGun2->Update();
	}
	if(iSecondaryWeapon)
	{
		iSecondaryWeapon->Update();
	}
	if(iBackCannon)
	{
		iBackCannon->Update();
	}
	if(iFighterAI)
	{
		iFighterAI->Update();
	}
	UpdateSpeed();
}

void CVal::BeforeTurnSettings()
{
	iFrontMachineGun1->SetDisabled(true);
	iFrontMachineGun2->SetDisabled(true);
	iSecondaryWeapon->SetDisabled(true);
	iBackCannon->SetDisabled(true);
}

void CVal::AfterTurnSettings()
{
	iFrontMachineGun1->SetDisabled(false);
	iFrontMachineGun2->SetDisabled(false);
	iSecondaryWeapon->SetDisabled(false);
	iBackCannon->SetDisabled(false);
}

void CVal::Draw()
{
	//draw Secondary Weapon
	if(iSecondaryWeapon)
		iSecondaryWeapon->Draw();

	//draw Gear
	if(!iGearUp && !iCrashed && !iIsTurning)
		iLandingGearTextureObject->DrawArrays();
	
	//draw BackCannon
	if(!iDoNotDraw && !iCrashed)
		iBackCannon->Draw();

	//behind plane
	if(iFrontMachineGun2)
		iFrontMachineGun2->Draw();

	//draw Plane
	if(!iDoNotDraw)
		iTextureObject->DrawArrays();//uses the currently assianged texture

	//behind plane
	if(iFrontMachineGun1)
		iFrontMachineGun1->Draw();
}

void CVal::Fire1()
{
	if(!iFrontMachineGun1->IsDisabled())
	{
		iFrontMachineGun1->Shoot();
	}
	if(!iFrontMachineGun2->IsDisabled())
	{
		iFrontMachineGun2->Shoot();
	}
}

void CVal::Fire2()
{
	iSecondaryWeapon->Shoot();
}

void CVal::PlayTurnAnimation()
{
	//start fire sequence
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = iFramesPerTurnImage;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftValTurnSequence);
	lDefaultRequest.TotalImages = 7;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = VAL_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
}

TBool CVal::Fire1HasAmmunitionLeft()
{
	return true;
}

TBool CVal::Fire2HasAmmunitionLeft()
{
	if(iSecondaryWeapon->GetAmmunitionLeft() > 0)
		return true;
	else
		return false;
}

void CVal::PlaneCrash()
{
	if(iSecondaryWeapon)
	{
		TWeaponIdentifier lPreviousWeapon = iSecondaryWeapon->GetWeaponIdentifier();
		EquipWithNewSecondaryWeapon(EWeaponIdentifier1000kgBomb);
		iSecondaryWeapon->Explode();
		//need to do this otherwise the player will see that we changed the actual weapon to create this explosion
		EquipWithNewSecondaryWeapon(lPreviousWeapon);
		iSecondaryWeapon->SetAmmunitionToValue(0);
	}

	//assign crash image
	iTextureObject->SetHeight(TIntFloat::Convert(VAL_CRASH_STATE_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(VAL_CRASH_STATE_WIDTH));
	iTextureObject->RotateVertexes(iTextureObject->GetRotationalAngel());
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 10;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftValCrashed);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = VAL_CRASH_STATE_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.59375;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
	iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);

	CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision, iHitBox);
	lSoundObject->PlayHitBox();
	delete lSoundObject;//need to clean it up
}

CWeapon* CVal::GetSecondaryWeapon()
{
	return iSecondaryWeapon;
}

CWeapon* CVal::GetPrimaryWeapon()
{
	return iFrontMachineGun1;
}

TInt CVal::GetAmmunitionLeftOfSecondaryWeapon()
{
	if(iSecondaryWeapon)
	{
		return iSecondaryWeapon->GetAmmunitionLeft();
	}else
		return 0;
}

void CVal::RearmAircraft()
{
	if(iSecondaryWeapon)
		iSecondaryWeapon->ResetAmmunitionToMaxValue();

	//also repair the aircraft
	iHealth = CGameObjectDefaultValues::ValHealth;
	iMaxEngineAcceleration = iNormalMaxEngineAcceleration;
}

void CVal::EquipWithNewSecondaryWeapon(TWeaponIdentifier aWeaponIdendifier)
{
	switch(aWeaponIdendifier)
	{
		case EWeaponIdentifier1000kgBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Val1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_1000KG_BOMB_POSITION_X, VAL_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		case EWeaponIdentifier500kgBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBomb500Kg::New(CGameObjectDefaultValues::Val500KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_500KG_BOMB_POSITION_X, VAL_500KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		case EWeaponIdentifier100kgBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBomb100Kg::New(CGameObjectDefaultValues::Val100KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_100KG_BOMB_POSITION_X, VAL_100KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		case EWeaponIdentifierClusterBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBombCluster::New(CGameObjectDefaultValues::ValClusterBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_CLUSTER_BOMB_POSITION_X, VAL_CLUSTER_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed,
					VAL_CLUSTER_BOMB_TIMER_FRAMES);
			return;
		}
		case EWeaponStandardRocket:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CStandardRocketWeapon::New(CGameObjectDefaultValues::ValRocketMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_STANDARD_ROCKET_POSITION_X, VAL_STANDARD_ROCKET_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed, this);
			return;
		}
		case EWeaponIdentifierTorpedo:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponTorpedo::New(CGameObjectDefaultValues::ValTorpedoMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, VAL_TORPEDO_POSITION_X, VAL_TORPEDO_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		default:
			return;
	}
}

void CVal::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	AircraftGameObjectSaveContentToDisk(aOutputStream);
	iFrontMachineGun1->SaveOnDisk(aOutputStream);
	iFrontMachineGun2->SaveOnDisk(aOutputStream);
	iSecondaryWeapon->SaveOnDisk(aOutputStream);
	iBackCannon->SaveOnDisk(aOutputStream);
	iSmokeCreator1->SaveOnDisk(aOutputStream);
}

void CVal::LoadFromDisk(CFileReadStream &aInputStream)
{
	AircraftGameObjectLoadContentFromDisk(aInputStream);
	TWeaponIdentifier lFrontWeapon1 = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun1->LoadFromDisk(aInputStream);
	TWeaponIdentifier lFrontWeapon2 = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun2->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSecondaryWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//can be different weapons
	EquipWithNewSecondaryWeapon(lSecondaryWeapon);//equip with weapon that has been saved
	iSecondaryWeapon->LoadFromDisk(aInputStream);//set its data
	TWeaponIdentifier lBackCannon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackCannon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSmokeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreator1->LoadFromDisk(aInputStream);
}
