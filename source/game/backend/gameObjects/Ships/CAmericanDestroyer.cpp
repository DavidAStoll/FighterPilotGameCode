/*
 ============================================================================
 Name		: AmericanDestroyer.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanDestroyer implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CAmericanDestroyer::CollisionPoints;

CAmericanDestroyer::CAmericanDestroyer(TInt aXLocation, SGameObjectAttributes& aAttributes) :
	CShip(AMERICAN_DESTROYER_Z,aXLocation, AMERICAN_DESTROYER_KEEL_DEPTH,TIntFloat::Convert(SHIP_BASIC_SPEED), aAttributes)
{
}

CAmericanDestroyer::~CAmericanDestroyer()
{
	if(i7InchGun)
	{
		delete i7InchGun;
		i7InchGun = NULL;
	}
	if(i75mmGun)
	{
		delete i75mmGun;
		i75mmGun = NULL;
	}
	if(iFrontMachineGun)
	{
		delete iFrontMachineGun;
		iFrontMachineGun = NULL;
	}
	if(iBackMachineGun1)
	{
		delete iBackMachineGun1;
		iBackMachineGun1 = NULL;
	}
	if(iBackMachineGun2)
	{
		delete iBackMachineGun2;
		iBackMachineGun2 = NULL;
	}
	if(iSmokeCreatorLeft)
	{
		delete iSmokeCreatorLeft;
		iSmokeCreatorLeft = NULL;
	}
	if(iSmokeCreatorMiddle)
	{
		delete iSmokeCreatorMiddle;
		iSmokeCreatorMiddle = NULL;
	}
	if(iSmokeCreatorRight)
	{
		delete iSmokeCreatorRight;
		iSmokeCreatorRight = NULL;
	}
}

CAmericanDestroyer* CAmericanDestroyer::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanDestroyerHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_DESTROYER_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanDestroyer;

	CAmericanDestroyer* self = new CAmericanDestroyer(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanDestroyer::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanDestroyer), AMERICAN_DESTROYER_WIDTH, AMERICAN_DESTROYER_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_DESTROYER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_DESTROYER_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanDestroyer::CollisionPoints);

	i7InchGun = CGun7Inch::New(&iCoordinates, &iAngle,45, AMERICAN_DESTROYER_7_INCH_GUN_X_LOCATION, AMERICAN_DESTROYER_7_INCH_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, AMERICAN_DESTROYER_NUMBER_OF_MUZZELES_PER_7_INCH_TURRENT, true);
	i75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, AMERICAN_DESTROYER_DUAL_75MM_GUN_FRONT_X_LOCATION, AMERICAN_DESTROYER_DUAL_75MM_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this, true);
	iFrontMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,80, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 100, this, true);
	iBackMachineGun1 = CShipDualMachineGun::New(&iCoordinates, &iAngle,135, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 135, this, true);
	iBackMachineGun2 = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 155, this, true);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_DESTROYER_LEFT_SMOKE_CREATOR_X_OFFSET, AMERICAN_DESTROYER_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_DESTROYER_MIDDLE_SMOKE_CREATOR_X_OFFSET, AMERICAN_DESTROYER_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_DESTROYER_RIGHT_SMOKE_CREATOR_X_OFFSET, AMERICAN_DESTROYER_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CAmericanDestroyer::Update()
{
	ShipBasicUpdate();
	iFrontMachineGun->Update();
	iBackMachineGun1->Update();
	iBackMachineGun2->Update();
	i75mmGun->Update();
	i7InchGun->Update();
	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
}

void CAmericanDestroyer::Draw()
{
	iFrontMachineGun->Draw();
	iBackMachineGun1->Draw();
	iBackMachineGun2->Draw();
	i75mmGun->Draw();
	i7InchGun->Draw();
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CAmericanDestroyer::UpdateGunPriorities()
{
	i7InchGun->GetShipArteriallyAI()->SetTargetPriorities(iTargetPriority);
}

void CAmericanDestroyer::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat l7InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_DESTROYER_7_INCH_GUN_X_LOCATION, AMERICAN_DESTROYER_7_INCH_GUN_Y_LOCATION);
	TPointIntFloat lFrontMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION);
	TPointIntFloat lBack1MachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION);
	TPointIntFloat lBack2MachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, AMERICAN_DESTROYER_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION);
	TPointIntFloat l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_DESTROYER_DUAL_75MM_GUN_FRONT_X_LOCATION, AMERICAN_DESTROYER_DUAL_75MM_GUN_FRONT_Y_LOCATION);
	i7InchGun->Rotate(-1,l7InchGunLocation);
	iFrontMachineGun->Rotate(-1, lFrontMachineGunPosition);
	iBackMachineGun1->Rotate(-1, lBack1MachineGunPosition);
	iBackMachineGun2->Rotate(-1, lBack2MachineGunPosition);
	i75mmGun->Rotate(-1, l75mmGunPosition);
}

void CAmericanDestroyer::Die(TDamageType aType)
{
  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CAmericanDestroyer::InitilizeAmericanDestroyer()
{
	CAmericanDestroyer::LoadAmericanDestroyerTextures();
	CAmericanDestroyer::CreateAmericanDestroyerCollisionPointsL();
}

void CAmericanDestroyer::RemoveAmericanDestroyer()
{
	CAmericanDestroyer::UnloadAmericanDestroyerTextures();
	CAmericanDestroyer::RemoveAmericanDestroyerCollisionPoints();
}

void CAmericanDestroyer::LoadAmericanDestroyerTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanDestroyer);
}

void CAmericanDestroyer::UnloadAmericanDestroyerTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanDestroyer);
}

void CAmericanDestroyer::CreateAmericanDestroyerCollisionPointsL()
{
	CAmericanDestroyer::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_DESTROYER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanDestroyer::CollisionPoints[0].XOffset = 20;
	CAmericanDestroyer::CollisionPoints[0].YOffset = 1;
	CAmericanDestroyer::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CAmericanDestroyer::CollisionPoints[1].XOffset = 5;
	CAmericanDestroyer::CollisionPoints[1].YOffset = 38;
	CAmericanDestroyer::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[2].XOffset = 125;
	CAmericanDestroyer::CollisionPoints[2].YOffset = 33;
	CAmericanDestroyer::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[3].XOffset = 116;
	CAmericanDestroyer::CollisionPoints[3].YOffset = 78;
	CAmericanDestroyer::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[4].XOffset = 135;
	CAmericanDestroyer::CollisionPoints[4].YOffset = 81;
	CAmericanDestroyer::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[5].XOffset = 160;
	CAmericanDestroyer::CollisionPoints[5].YOffset = 113;
	CAmericanDestroyer::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[6].XOffset = 175;
	CAmericanDestroyer::CollisionPoints[6].YOffset = 28;
	CAmericanDestroyer::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[7].XOffset = 210;
	CAmericanDestroyer::CollisionPoints[7].YOffset = 33;
	CAmericanDestroyer::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[8].XOffset = 230;
	CAmericanDestroyer::CollisionPoints[8].YOffset = 85;
	CAmericanDestroyer::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[9].XOffset = 260;
	CAmericanDestroyer::CollisionPoints[9].YOffset = 73;
	CAmericanDestroyer::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[10].XOffset = 263;
	CAmericanDestroyer::CollisionPoints[10].YOffset = 38;
	CAmericanDestroyer::CollisionPoints[10].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[11].XOffset = 290;
	CAmericanDestroyer::CollisionPoints[11].YOffset = 48;
	CAmericanDestroyer::CollisionPoints[11].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[12].XOffset = 300;
	CAmericanDestroyer::CollisionPoints[12].YOffset = 73;
	CAmericanDestroyer::CollisionPoints[12].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[13].XOffset = 315;
	CAmericanDestroyer::CollisionPoints[13].YOffset = 68;
	CAmericanDestroyer::CollisionPoints[13].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[14].XOffset = 320;
	CAmericanDestroyer::CollisionPoints[14].YOffset = 43;
	CAmericanDestroyer::CollisionPoints[14].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[15].XOffset = 425;
	CAmericanDestroyer::CollisionPoints[15].YOffset = 45;
	CAmericanDestroyer::CollisionPoints[15].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[16].XOffset = 430;
	CAmericanDestroyer::CollisionPoints[16].YOffset = 25;
	CAmericanDestroyer::CollisionPoints[16].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[17].XOffset = 508;
	CAmericanDestroyer::CollisionPoints[17].YOffset = 25;
	CAmericanDestroyer::CollisionPoints[17].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[18].XOffset = 490;
	CAmericanDestroyer::CollisionPoints[18].YOffset = 0;
	CAmericanDestroyer::CollisionPoints[18].iMovementType = EPointMovementTypeNone;
	CAmericanDestroyer::CollisionPoints[19].XOffset = 20;
	CAmericanDestroyer::CollisionPoints[19].YOffset = 1;
	CAmericanDestroyer::CollisionPoints[19].iMovementType = EPointMovementTypeNone;
}

void CAmericanDestroyer::RemoveAmericanDestroyerCollisionPoints()
{
	if(CAmericanDestroyer::CollisionPoints)
	{
		delete[] CAmericanDestroyer::CollisionPoints;
		CAmericanDestroyer::CollisionPoints = NULL;
	}
}

void CAmericanDestroyer::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	i7InchGun->SaveOnDisk(aOutputStream);
	i75mmGun->SaveOnDisk(aOutputStream);
	iFrontMachineGun->SaveOnDisk(aOutputStream);
	iBackMachineGun1->SaveOnDisk(aOutputStream);
	iBackMachineGun2->SaveOnDisk(aOutputStream);
}

void CAmericanDestroyer::LoadFromDisk(CFileReadStream &aInputStream)
{
	ShipLoadContentFromDisk(aInputStream);
	TWeaponIdentifier lWeapon;
	//load settings of smoke creators
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorLeft->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorMiddle->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorRight->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	i7InchGun->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	i75mmGun->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackMachineGun1->LoadFromDisk(aInputStream);
	lWeapon = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackMachineGun2->LoadFromDisk(aInputStream);
}
