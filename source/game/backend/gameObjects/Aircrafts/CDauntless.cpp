/*
 ============================================================================
 Name		: Dauntless.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CDauntless implementation
 ============================================================================
 */

//Weapons
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/backend/gameObjects/Aircrafts/CDauntless.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb1000Kg.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb500Kg.h"
#include "includes/game/backend/gameObjects/Weapons/CStandardRocketWeapon.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb100Kg.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponBombCluster.h"
#include "includes/game/backend/gameObjects/Weapons/CWeaponTorpedo.h"
#include "includes/game/backend/gameObjects/Weapons/CBackGunMachineGun8mm.h"
#include "includes/game/backend/gameObjects/Weapons/CBackGunDoubleMachineGun8mm.h"

SBoundingCollisionPoint* CDauntless::CollisionPoints = NULL;
SBoundingCollisionPoint* CDauntless::CollisionPointForFrontWheel = NULL;
SBoundingCollisionPoint* CDauntless::CollisionPointForBackWheel = NULL;

CDauntless::CDauntless(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAircraft(DAUNTLESS_Z_VALUE, NULL, NULL, aLocation, aAttributes, aCurrentAcceleration, TIntFloat::Convert(DAUNTLESS_MAX_ACCELERATION), aFlying, DAUNTLESS_TURN_TIME_FACTOR, DAUNTLESS_ANGLE_CHANGE_TIME_FACTOR)
{
	iPrimaryWeaponMachineGun1 = NULL;
	iPrimaryWeaponMachineGun2 = NULL;
	iSecondaryWeapon = NULL;
	iAircraftType = EAircraftTypeDiveBomber;
}

CDauntless::~CDauntless()
{
	if(iPrimaryWeaponMachineGun1)
	{
		delete iPrimaryWeaponMachineGun1;
		iPrimaryWeaponMachineGun1 = NULL;
	}
	if(iPrimaryWeaponMachineGun2)
	{
		delete iPrimaryWeaponMachineGun2;
		iPrimaryWeaponMachineGun2 = NULL;
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

CDauntless* CDauntless::New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = DAUNTLESS_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesPlane | EObjectTypesSmallPlane;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = DAUNTLESS_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierDauntless;
	CDauntless* self = new CDauntless(aCurrentAcceleration, aFlying, aLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CDauntless::ConstructL()
{
	//Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdAircraftDauntlessNormalSequence), DAUNTLESS_WIDTH, DAUNTLESS_HEIGHT, iZCor, 0, 1, 0, 1);
	//create another texture Object for the Gear
	iLandingGearTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdAircraftDauntlessLandingGear), DAUNTLESS_WIDTH, DAUNTLESS_HEIGHT, iZCor, 0, 0.625, 0, 1);
	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 1;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftDauntlessNormalSequence);
	lDefaultRequest.TotalImages = 3;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = 80;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//create standard weapon
	iPrimaryWeaponMachineGun1 = CWeaponAircraftMachineGun13mm::New(&iCoordinates, &iAngle, DAUNTLESS_FRONT_13MM_MACHINE_GUN_1_POSITION_X, DAUNTLESS_FRONT_13MM_MACHINE_GUN_1_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	iPrimaryWeaponMachineGun2 = CWeaponAircraftMachineGun13mm::New(&iCoordinates, &iAngle, DAUNTLESS_FRONT_13MM_MACHINE_GUN_2_POSITION_X, DAUNTLESS_FRONT_13MM_MACHINE_GUN_2_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	//create secondary weapon,might differ depending on payload
	iSecondaryWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_1000KG_BOMB_POSITION_X, DAUNTLESS_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
	//create back cannon for the dauntless
	iBackCannon = CBackGunDoubleMachineGun8mm::New(&iCoordinates, &iAngle, 180, DAUNTLESS_BACK_CANNON_POSITION_X, DAUNTLESS_BACK_CANNON_POSITION_Y, &iObjectReflected, &iConflictSide, &iTotalSpeed, 0, 45, this, 0);
	//create the smoke Creator that makes smoke if the life falls too low
	iSmokeCreator1 = CAircraftSmokeCreator::New(true, this, DAUNTLESS_SMOKE_CREATOR_POSITION_X, DAUNTLESS_SMOKE_CREATOR_POSITION_Y, SMALL_AIRCRAFT_FRAMES_UNTIL_SMOKE_DISAPPEARS);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, DAUNTLESS_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, DAUNTLESS_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CDauntless::CollisionPoints);

	//create wheels of the airplane
	iFrontWheel = CPlaneWheels::New(CHitBox::New(&iCoordinates, DAUNTLESS_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, DAUNTLESS_LANDING_GEAR_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CDauntless::CollisionPointForFrontWheel), this);
	iBackWheel = CPlaneWheels::New(CHitBox::New(&iCoordinates, DAUNTLESS_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, DAUNTLESS_LANDING_GEAR_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CDauntless::CollisionPointForBackWheel), this);

	if(iObjectReflected)
	{
		iTextureObject->ReflectOverYAxis();
		iLandingGearTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

//------------------- functions --------------------

void CDauntless::SetPosition(TPointIntFloat& aPoint)
{
	iCoordinates = aPoint;
	if(iTextureObject)
	{
		iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
		iLandingGearTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	}
}

void CDauntless::SetPosition(TPoint& aPoint)
{
	iCoordinates.iX = TIntFloat::Convert(aPoint.iX);
	iCoordinates.iY = TIntFloat::Convert(aPoint.iY);
	if(iTextureObject)
	{
		iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
		iLandingGearTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	}
}

void CDauntless::LoadDauntlessTexturesL()
{
	//load images that dauntless needs
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftDauntlessNormalSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftDauntlessFireSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftDauntlessCrashed);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftDauntlessLandingGear);
	CGame::Game->iImageStore->LoadImage(EImageIdAircraftDauntlessTurnSequence);
}

void CDauntless::UnloadDauntlessTextures()
{
	//load images that dauntless needs
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftDauntlessNormalSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftDauntlessFireSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftDauntlessCrashed);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftDauntlessLandingGear);
	CGame::Game->iImageStore->UnloadImage(EImageIdAircraftDauntlessTurnSequence);
}

void CDauntless::CreateDauntlessCollisionPointsL()
{
	CDauntless::CollisionPoints = new SBoundingCollisionPoint[DAUNTLESS_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CDauntless::CollisionPoints[0].XOffset = 3;
	CDauntless::CollisionPoints[0].YOffset = 4;
	CDauntless::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPoints[1].XOffset = 80;
	CDauntless::CollisionPoints[1].YOffset = 13;
	CDauntless::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPoints[2].XOffset = 78;
	CDauntless::CollisionPoints[2].YOffset = 22;
	CDauntless::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPoints[3].XOffset = 43;
	CDauntless::CollisionPoints[3].YOffset = 23;
	CDauntless::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPoints[4].XOffset = 30;
	CDauntless::CollisionPoints[4].YOffset = 26;
	CDauntless::CollisionPoints[4].iMovementType = EPointMovementTypeConnectionStartPoint;
	CDauntless::CollisionPoints[5].XOffset = 19;
	CDauntless::CollisionPoints[5].YOffset = 22;
	CDauntless::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPoints[6].XOffset = 7;
	CDauntless::CollisionPoints[6].YOffset = 20;
	CDauntless::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPoints[7].XOffset = 5;
	CDauntless::CollisionPoints[7].YOffset = 4;
	CDauntless::CollisionPoints[7].iMovementType = EPointMovementTypeConnectionEndPoint; //repeat first point

	//create collisionPoints for wheels
	CDauntless::CollisionPointForFrontWheel = new SBoundingCollisionPoint[DAUNTLESS_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS]; //don't really need a moving hitbox, are accurate enough
	CDauntless::CollisionPointForFrontWheel[0].XOffset = 0;
	CDauntless::CollisionPointForFrontWheel[0].YOffset = 6;
	CDauntless::CollisionPointForFrontWheel[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CDauntless::CollisionPointForFrontWheel[1].XOffset = 2;
	CDauntless::CollisionPointForFrontWheel[1].YOffset = -2;
	CDauntless::CollisionPointForFrontWheel[1].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPointForFrontWheel[2].XOffset = 18;
	CDauntless::CollisionPointForFrontWheel[2].YOffset = 1;
	CDauntless::CollisionPointForFrontWheel[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CDauntless::CollisionPointForFrontWheel[3].XOffset = 22;
	CDauntless::CollisionPointForFrontWheel[3].YOffset = 8;
	CDauntless::CollisionPointForFrontWheel[3].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPointForFrontWheel[4].XOffset = 0;
	CDauntless::CollisionPointForFrontWheel[4].YOffset = 6;
	CDauntless::CollisionPointForFrontWheel[4].iMovementType = EPointMovementTypeNone;

	CDauntless::CollisionPointForBackWheel = new SBoundingCollisionPoint[DAUNTLESS_LANDING_GEAR_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CDauntless::CollisionPointForBackWheel[0].XOffset = 65;
	CDauntless::CollisionPointForBackWheel[0].YOffset = 6;
	CDauntless::CollisionPointForBackWheel[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CDauntless::CollisionPointForBackWheel[1].XOffset = 62;
	CDauntless::CollisionPointForBackWheel[1].YOffset = -2;
	CDauntless::CollisionPointForBackWheel[1].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPointForBackWheel[2].XOffset = 72;
	CDauntless::CollisionPointForBackWheel[2].YOffset = 0;
	CDauntless::CollisionPointForBackWheel[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CDauntless::CollisionPointForBackWheel[3].XOffset = 75;
	CDauntless::CollisionPointForBackWheel[3].YOffset = 8;
	CDauntless::CollisionPointForBackWheel[3].iMovementType = EPointMovementTypeNone;
	CDauntless::CollisionPointForBackWheel[4].XOffset = 62;
	CDauntless::CollisionPointForBackWheel[4].YOffset = 6;
	CDauntless::CollisionPointForBackWheel[4].iMovementType = EPointMovementTypeNone;

}

void CDauntless::RemoveDauntlessCollisionPoints()
{
	if(CDauntless::CollisionPoints)
	{
		delete[] CDauntless::CollisionPoints;
		CDauntless::CollisionPoints = NULL;
	}
	if(CDauntless::CollisionPointForBackWheel)
	{
		delete[] CDauntless::CollisionPointForBackWheel;
		CDauntless::CollisionPointForBackWheel = NULL;
	}
	if(CDauntless::CollisionPointForFrontWheel)
	{
		delete[] CDauntless::CollisionPointForFrontWheel;
		CDauntless::CollisionPointForFrontWheel = NULL;
	}
}

void CDauntless::InitilizeDauntless()
{
	CDauntless::LoadDauntlessTexturesL();
	CDauntless::CreateDauntlessCollisionPointsL();
}

void CDauntless::RemoveDauntless()
{
	CDauntless::UnloadDauntlessTextures();
	CDauntless::RemoveDauntlessCollisionPoints();
}

void CDauntless::DrawAdvancedHitBoxL()
{
	iHitBox->DrawAdvancedHitBoxL();
	iFrontWheel->iHitBox->DrawAdvancedHitBoxL();
	iBackWheel->iHitBox->DrawAdvancedHitBoxL();
}

void CDauntless::UpdateAircraft()
{

	iAnimationPlayer->Update();
	iSmokeCreator1->Update();

	if(iPrimaryWeaponMachineGun1)
	{
		iPrimaryWeaponMachineGun1->Update();
	}
	if(iPrimaryWeaponMachineGun2)
	{
		iPrimaryWeaponMachineGun2->Update();
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

void CDauntless::BeforeTurnSettings()
{
	iPrimaryWeaponMachineGun1->SetDisabled(true);
	iPrimaryWeaponMachineGun2->SetDisabled(true);
	iSecondaryWeapon->SetDisabled(true);
	iBackCannon->SetDisabled(true);
}

void CDauntless::AfterTurnSettings()
{
	iPrimaryWeaponMachineGun1->SetDisabled(false);
	iPrimaryWeaponMachineGun2->SetDisabled(false);
	iSecondaryWeapon->SetDisabled(false);
	iBackCannon->SetDisabled(false);
}

void CDauntless::Draw()
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
	if(iPrimaryWeaponMachineGun2)
		iPrimaryWeaponMachineGun2->Draw();

	//draw Plane
	if(!iDoNotDraw)
		iTextureObject->DrawArrays();//uses the currently assianged texture

	//infront of plane
	if(iPrimaryWeaponMachineGun1)
		iPrimaryWeaponMachineGun1->Draw();
}

void CDauntless::Fire1()
{
	if(!iPrimaryWeaponMachineGun1->IsDisabled())
	{
		iPrimaryWeaponMachineGun1->Shoot();
	}
	if(!iPrimaryWeaponMachineGun2->IsDisabled())
	{
		iPrimaryWeaponMachineGun2->Shoot();
	}
}

void CDauntless::Fire2()
{
	iSecondaryWeapon->Shoot();
}

void CDauntless::PlayTurnAnimation()
{
	//start fire sequence
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = iFramesPerTurnImage;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftDauntlessTurnSequence);
	lDefaultRequest.TotalImages = 7;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = DAUNTLESS_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
}

TBool CDauntless::Fire1HasAmmunitionLeft()
{
	return true;
}

TBool CDauntless::Fire2HasAmmunitionLeft()
{
	if(iSecondaryWeapon->GetAmmunitionLeft() > 0)
		return true;
	else
		return false;
}

void CDauntless::PlaneCrash()
{
	//create big explosion when plane crashes
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
	iTextureObject->SetHeight(TIntFloat::Convert(DAUNTLESS_CRASH_STATE_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(DAUNTLESS_CRASH_STATE_WIDTH));
	iTextureObject->RotateVertexes(iTextureObject->GetRotationalAngel());
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 10;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdAircraftDauntlessCrashed);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = 80;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.59375;
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
	iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);

	CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision, iHitBox);
	lSoundObject->PlayHitBox();
	delete lSoundObject;//need to clean it up
}

CWeapon* CDauntless::GetSecondaryWeapon()
{
	return iSecondaryWeapon;
}

CWeapon* CDauntless::GetPrimaryWeapon()
{
	return iPrimaryWeaponMachineGun1;
}

TInt CDauntless::GetAmmunitionLeftOfSecondaryWeapon()
{
	if(iSecondaryWeapon)
	{
		return iSecondaryWeapon->GetAmmunitionLeft();
	}else
		return 0;
}

void CDauntless::RearmAircraft()
{
	if(iSecondaryWeapon)
		iSecondaryWeapon->ResetAmmunitionToMaxValue();

	//also repair the aircraft
	iHealth = CGameObjectDefaultValues::DauntlessHealth;
	iMaxEngineAcceleration = iNormalMaxEngineAcceleration;
}

void CDauntless::EquipWithNewSecondaryWeapon(TWeaponIdentifier aWeaponIdendifier)
{
	switch(aWeaponIdendifier)
	{
		case EWeaponIdentifier1000kgBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBomb1000Kg::New(CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_1000KG_BOMB_POSITION_X, DAUNTLESS_1000KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		case EWeaponIdentifier500kgBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBomb500Kg::New(CGameObjectDefaultValues::Dauntless500KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_500KG_BOMB_POSITION_X, DAUNTLESS_500KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		case EWeaponIdentifier100kgBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBomb100Kg::New(CGameObjectDefaultValues::Dauntless100KgBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_100KG_BOMB_POSITION_X, DAUNTLESS_100KG_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		case EWeaponIdentifierClusterBomb:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponBombCluster::New(CGameObjectDefaultValues::DauntlessClusterBombMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_CLUSTER_BOMB_POSITION_X, DAUNTLESS_CLUSTER_BOMB_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed,
					DAUNTLESS_CLUSTER_BOMB_TIMER_FRAMES);
			return;
		}
		case EWeaponStandardRocket:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CStandardRocketWeapon::New(CGameObjectDefaultValues::DauntlessRocketMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_STANDARD_ROCKET_POSITION_X, DAUNTLESS_STANDARD_ROCKET_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed, this);
			return;
		}
		case EWeaponIdentifierTorpedo:
		{
			if(iSecondaryWeapon)
				delete iSecondaryWeapon;
			iSecondaryWeapon = CWeaponTorpedo::New(CGameObjectDefaultValues::DauntlessTorpedoMaxLoad, &iCoordinates, &iAngle, &iIsUpsideDown, DAUNTLESS_TORPEDO_POSITION_X, DAUNTLESS_TORPEDO_POSITION_Y, &iObjectReflected, &iConflictSide, false, &iTotalSpeed);
			return;
		}
		default:
			return;
	}
}

void CDauntless::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	AircraftGameObjectSaveContentToDisk(aOutputStream);
	iPrimaryWeaponMachineGun1->SaveOnDisk(aOutputStream);
	iPrimaryWeaponMachineGun2->SaveOnDisk(aOutputStream);
	iSecondaryWeapon->SaveOnDisk(aOutputStream);
	iBackCannon->SaveOnDisk(aOutputStream);
	iSmokeCreator1->SaveOnDisk(aOutputStream);
}

void CDauntless::LoadFromDisk(CFileReadStream &aInputStream)
{
	AircraftGameObjectLoadContentFromDisk(aInputStream);

	TWeaponIdentifier lPrimaryWeapon1 = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iPrimaryWeaponMachineGun1->LoadFromDisk(aInputStream);
	TWeaponIdentifier lPrimaryWeapon2 = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());
	iPrimaryWeaponMachineGun2->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSecondaryWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//can be different weapons
	EquipWithNewSecondaryWeapon(lSecondaryWeapon);//equip with weapon that has been saved
	iSecondaryWeapon->LoadFromDisk(aInputStream);//set its data
	TWeaponIdentifier lBackCannon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackCannon->LoadFromDisk(aInputStream);
	TWeaponIdentifier lSmokeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreator1->LoadFromDisk(aInputStream);
}
