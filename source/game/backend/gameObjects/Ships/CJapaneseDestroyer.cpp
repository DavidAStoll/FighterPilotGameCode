/*
 ============================================================================
 Name		: JapaneseDestroyer.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseDestroyer implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CJapaneseDestroyer::CollisionPoints;

CJapaneseDestroyer::CJapaneseDestroyer(TInt aXLocation, SGameObjectAttributes& aAttributes) :
	CShip(JAPANESE_DESTROYER_Z,aXLocation, JAPANESE_DESTROYER_KEEL_DEPTH, TIntFloat::Convert(SHIP_BASIC_SPEED), aAttributes)
{
}

CJapaneseDestroyer::~CJapaneseDestroyer()
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

CJapaneseDestroyer* CJapaneseDestroyer::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseDestroyerHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_DESTROYER_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseDestroyer;

	CJapaneseDestroyer* self = new CJapaneseDestroyer(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseDestroyer::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseDestroyer), JAPANESE_DESTROYER_WIDTH, JAPANESE_DESTROYER_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_DESTROYER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_DESTROYER_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseDestroyer::CollisionPoints);

	i7InchGun = CGun7Inch::New(&iCoordinates, &iAngle,45, JAPANESE_DESTROYER_7_INCH_GUN_X_LOCATION, JAPANESE_DESTROYER_7_INCH_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, JAPANESE_DESTROYER_NUMBER_OF_MUZZELES_PER_7_INCH_TURRENT);
	i75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, JAPANESE_DESTROYER_DUAL_75MM_GUN_FRONT_X_LOCATION, JAPANESE_DESTROYER_DUAL_75MM_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iFrontMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,80, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 100, this);
	iBackMachineGun1 = CShipDualMachineGun::New(&iCoordinates, &iAngle,135, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 135, this);
	iBackMachineGun2 = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 155, this);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_DESTROYER_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_DESTROYER_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_DESTROYER_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_DESTROYER_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_DESTROYER_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_DESTROYER_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CJapaneseDestroyer::Update()
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

void CJapaneseDestroyer::Draw()
{
	iFrontMachineGun->Draw();
	iBackMachineGun1->Draw();
	iBackMachineGun2->Draw();
	i75mmGun->Draw();
	i7InchGun->Draw();
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CJapaneseDestroyer::UpdateGunPriorities()
{
	i7InchGun->GetShipArteriallyAI()->SetTargetPriorities(iTargetPriority);
}

void CJapaneseDestroyer::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat l7InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_DESTROYER_7_INCH_GUN_X_LOCATION, JAPANESE_DESTROYER_7_INCH_GUN_Y_LOCATION);
	TPointIntFloat lFrontMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION);
	TPointIntFloat lBack1MachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION);
	TPointIntFloat lBack2MachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, JAPANESE_DESTROYER_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION);
	TPointIntFloat l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_DESTROYER_DUAL_75MM_GUN_FRONT_X_LOCATION, JAPANESE_DESTROYER_DUAL_75MM_GUN_FRONT_Y_LOCATION);
	i7InchGun->Rotate(-1,l7InchGunLocation);
	iFrontMachineGun->Rotate(-1, lFrontMachineGunPosition);
	iBackMachineGun1->Rotate(-1, lBack1MachineGunPosition);
	iBackMachineGun2->Rotate(-1, lBack2MachineGunPosition);
	i75mmGun->Rotate(-1, l75mmGunPosition);
}

void CJapaneseDestroyer::Die(TDamageType aType)
{
  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CJapaneseDestroyer::InitilizeJapaneseDestroyer()
{
	CJapaneseDestroyer::LoadJapaneseDestroyerTextures();
	CJapaneseDestroyer::CreateJapaneseDestroyerCollisionPointsL();
}

void CJapaneseDestroyer::RemoveJapaneseDestroyer()
{
	CJapaneseDestroyer::UnloadJapaneseDestroyerTextures();
	CJapaneseDestroyer::RemoveJapaneseDestroyerCollisionPoints();
}

void CJapaneseDestroyer::LoadJapaneseDestroyerTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseDestroyer);
}

void CJapaneseDestroyer::UnloadJapaneseDestroyerTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseDestroyer);
}

void CJapaneseDestroyer::CreateJapaneseDestroyerCollisionPointsL()
{
	CJapaneseDestroyer::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_DESTROYER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseDestroyer::CollisionPoints[0].XOffset = 20;
	CJapaneseDestroyer::CollisionPoints[0].YOffset = 1;
	CJapaneseDestroyer::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseDestroyer::CollisionPoints[1].XOffset = 5;
	CJapaneseDestroyer::CollisionPoints[1].YOffset = 38;
	CJapaneseDestroyer::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[2].XOffset = 125;
	CJapaneseDestroyer::CollisionPoints[2].YOffset = 33;
	CJapaneseDestroyer::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[3].XOffset = 116;
	CJapaneseDestroyer::CollisionPoints[3].YOffset = 78;
	CJapaneseDestroyer::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[4].XOffset = 135;
	CJapaneseDestroyer::CollisionPoints[4].YOffset = 81;
	CJapaneseDestroyer::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[5].XOffset = 160;
	CJapaneseDestroyer::CollisionPoints[5].YOffset = 113;
	CJapaneseDestroyer::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[6].XOffset = 175;
	CJapaneseDestroyer::CollisionPoints[6].YOffset = 28;
	CJapaneseDestroyer::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[7].XOffset = 210;
	CJapaneseDestroyer::CollisionPoints[7].YOffset = 33;
	CJapaneseDestroyer::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[8].XOffset = 230;
	CJapaneseDestroyer::CollisionPoints[8].YOffset = 85;
	CJapaneseDestroyer::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[9].XOffset = 260;
	CJapaneseDestroyer::CollisionPoints[9].YOffset = 73;
	CJapaneseDestroyer::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[10].XOffset = 263;
	CJapaneseDestroyer::CollisionPoints[10].YOffset = 38;
	CJapaneseDestroyer::CollisionPoints[10].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[11].XOffset = 290;
	CJapaneseDestroyer::CollisionPoints[11].YOffset = 48;
	CJapaneseDestroyer::CollisionPoints[11].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[12].XOffset = 300;
	CJapaneseDestroyer::CollisionPoints[12].YOffset = 73;
	CJapaneseDestroyer::CollisionPoints[12].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[13].XOffset = 315;
	CJapaneseDestroyer::CollisionPoints[13].YOffset = 68;
	CJapaneseDestroyer::CollisionPoints[13].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[14].XOffset = 320;
	CJapaneseDestroyer::CollisionPoints[14].YOffset = 43;
	CJapaneseDestroyer::CollisionPoints[14].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[15].XOffset = 425;
	CJapaneseDestroyer::CollisionPoints[15].YOffset = 45;
	CJapaneseDestroyer::CollisionPoints[15].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[16].XOffset = 430;
	CJapaneseDestroyer::CollisionPoints[16].YOffset = 25;
	CJapaneseDestroyer::CollisionPoints[16].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[17].XOffset = 508;
	CJapaneseDestroyer::CollisionPoints[17].YOffset = 25;
	CJapaneseDestroyer::CollisionPoints[17].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[18].XOffset = 490;
	CJapaneseDestroyer::CollisionPoints[18].YOffset = 0;
	CJapaneseDestroyer::CollisionPoints[18].iMovementType = EPointMovementTypeNone;
	CJapaneseDestroyer::CollisionPoints[19].XOffset = 20;
	CJapaneseDestroyer::CollisionPoints[19].YOffset = 1;
	CJapaneseDestroyer::CollisionPoints[19].iMovementType = EPointMovementTypeNone;
}

void CJapaneseDestroyer::RemoveJapaneseDestroyerCollisionPoints()
{
	if(CJapaneseDestroyer::CollisionPoints)
	{
		delete[] CJapaneseDestroyer::CollisionPoints;
		CJapaneseDestroyer::CollisionPoints = NULL;
	}
}

void CJapaneseDestroyer::SaveOnDisk(CFileWriteStream &aOutputStream)
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

void CJapaneseDestroyer::LoadFromDisk(CFileReadStream &aInputStream)
{
	ShipLoadContentFromDisk(aInputStream);
	//load settings of smoke creators
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorLeft->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorMiddle->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorRight->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	i7InchGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	i75mmGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackMachineGun1->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackMachineGun2->LoadFromDisk(aInputStream);
}
