/*
 ============================================================================
 Name		: JapaneseOilTanker.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseOilTanker implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CJapaneseOilTanker::CollisionPoints;

CJapaneseOilTanker::CJapaneseOilTanker(TInt aXLocation, SGameObjectAttributes& aAttributes) :
	CShip(JAPANESE_OIL_TANKER_Z, aXLocation, JAPANESE_OIL_TANKER_KEEL_DEPTH, TIntFloat::Convert(SHIP_BASIC_SPEED), aAttributes)
{
}

CJapaneseOilTanker::~CJapaneseOilTanker()
{
	if(iFrontMachineGun)
	{
		delete iFrontMachineGun;
		iFrontMachineGun = NULL;
	}
	if(iBackMachineGun)
	{
		delete iBackMachineGun;
		iBackMachineGun = NULL;
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

CJapaneseOilTanker* CJapaneseOilTanker::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseOilTankerHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_OIL_TANKER_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseOilTanker;

	CJapaneseOilTanker* self = new CJapaneseOilTanker(aXLocation,lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseOilTanker::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseOilTanker), JAPANESE_OIL_TANKER_WIDTH, JAPANESE_OIL_TANKER_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_OIL_TANKER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_OIL_TANKER_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseOilTanker::CollisionPoints);

	//weapons
	iFrontMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iBackMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_BACK_X_LOCATION, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_BACK_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_OIL_TANKER_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_OIL_TANKER_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_OIL_TANKER_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_OIL_TANKER_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_OIL_TANKER_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_OIL_TANKER_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CJapaneseOilTanker::Update()
{
	ShipBasicUpdate();
	iFrontMachineGun->Update();
	iBackMachineGun->Update();
	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
}

void CJapaneseOilTanker::Draw()
{
	iFrontMachineGun->Draw();
	iBackMachineGun->Draw();
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CJapaneseOilTanker::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lFrontMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION);
	TPointIntFloat lBackMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_BACK_X_LOCATION, JAPANESE_OIL_TANKER_DUAL_MACHINE_GUN_BACK_Y_LOCATION);
	iFrontMachineGun->Rotate(-1, lFrontMachineGunPosition);
	iBackMachineGun->Rotate(-1, lBackMachineGunPosition);
}

void CJapaneseOilTanker::Die(TDamageType aType)
{
  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CJapaneseOilTanker::InitilizeJapaneseOilTanker()
{
	CJapaneseOilTanker::LoadJapaneseOilTankerTextures();
	CJapaneseOilTanker::CreateJapaneseOilTankerCollisionPointsL();
}

void CJapaneseOilTanker::RemoveJapaneseOilTanker()
{
	CJapaneseOilTanker::UnloadJapaneseOilTankerTextures();
	CJapaneseOilTanker::RemoveJapaneseOilTankerCollisionPoints();
}

void CJapaneseOilTanker::LoadJapaneseOilTankerTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseOilTanker);
}

void CJapaneseOilTanker::UnloadJapaneseOilTankerTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseOilTanker);
}

void CJapaneseOilTanker::CreateJapaneseOilTankerCollisionPointsL()
{
	CJapaneseOilTanker::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_OIL_TANKER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseOilTanker::CollisionPoints[0].XOffset = 18;
	CJapaneseOilTanker::CollisionPoints[0].YOffset = 3;
	CJapaneseOilTanker::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseOilTanker::CollisionPoints[1].XOffset = 3;
	CJapaneseOilTanker::CollisionPoints[1].YOffset = 10;
	CJapaneseOilTanker::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[2].XOffset = 1;
	CJapaneseOilTanker::CollisionPoints[2].YOffset = 58;
	CJapaneseOilTanker::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[3].XOffset = 60;
	CJapaneseOilTanker::CollisionPoints[3].YOffset = 50;
	CJapaneseOilTanker::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[4].XOffset = 95;
	CJapaneseOilTanker::CollisionPoints[4].YOffset = 50;
	CJapaneseOilTanker::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[5].XOffset = 100;
	CJapaneseOilTanker::CollisionPoints[5].YOffset = 78;
	CJapaneseOilTanker::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[6].XOffset = 190;
	CJapaneseOilTanker::CollisionPoints[6].YOffset = 78;
	CJapaneseOilTanker::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[7].XOffset = 195;
	CJapaneseOilTanker::CollisionPoints[7].YOffset = 34;
	CJapaneseOilTanker::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[8].XOffset = 335;
	CJapaneseOilTanker::CollisionPoints[8].YOffset = 43;
	CJapaneseOilTanker::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[9].XOffset = 355;
	CJapaneseOilTanker::CollisionPoints[9].YOffset = 78;
	CJapaneseOilTanker::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[10].XOffset = 470;
	CJapaneseOilTanker::CollisionPoints[10].YOffset = 78;
	CJapaneseOilTanker::CollisionPoints[10].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[11].XOffset = 473;
	CJapaneseOilTanker::CollisionPoints[11].YOffset = 63;
	CJapaneseOilTanker::CollisionPoints[11].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[12].XOffset = 505;
	CJapaneseOilTanker::CollisionPoints[12].YOffset = 63;
	CJapaneseOilTanker::CollisionPoints[12].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[13].XOffset = 485;
	CJapaneseOilTanker::CollisionPoints[13].YOffset = 38;
	CJapaneseOilTanker::CollisionPoints[13].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[14].XOffset = 480;
	CJapaneseOilTanker::CollisionPoints[14].YOffset = 13;
	CJapaneseOilTanker::CollisionPoints[14].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[15].XOffset = 460;
	CJapaneseOilTanker::CollisionPoints[15].YOffset = 3;
	CJapaneseOilTanker::CollisionPoints[15].iMovementType = EPointMovementTypeNone;
	CJapaneseOilTanker::CollisionPoints[16].XOffset = 18;
	CJapaneseOilTanker::CollisionPoints[16].YOffset = 3;
	CJapaneseOilTanker::CollisionPoints[16].iMovementType = EPointMovementTypeNone;
}

void CJapaneseOilTanker::RemoveJapaneseOilTankerCollisionPoints()
{
	if(CJapaneseOilTanker::CollisionPoints)
	{
		delete[] CJapaneseOilTanker::CollisionPoints;
		CJapaneseOilTanker::CollisionPoints = NULL;
	}
}

void CJapaneseOilTanker::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iFrontMachineGun->SaveOnDisk(aOutputStream);
	iBackMachineGun->SaveOnDisk(aOutputStream);
}

void CJapaneseOilTanker::LoadFromDisk(CFileReadStream &aInputStream)
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
	iFrontMachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBackMachineGun->LoadFromDisk(aInputStream);
}
