/*
 ============================================================================
 Name		: AmericanTroopTransporter.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanTroopTransporter implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CAmericanTroopTransporter::CollisionPoints;

CAmericanTroopTransporter::CAmericanTroopTransporter(TInt aXLocation, SGameObjectAttributes& aAttributes, TInt aSecOffset, TAmericanLCTType aLCTType, SGroundUnitOrder& aOrder) :
	CShip(AMERICAN_TROOP_TRANSPORTER_Z, aXLocation, AMERICAN_TROOP_TRANSPORTER_KEEL_DEPTH, TIntFloat::Convert(SHIP_BASIC_SPEED), aAttributes)
{
	iCurrentFrame = 0;
	iExtraFrameOffset = aSecOffset * GFRAMES_PER_SECOND_BACKEND;
	iLCTSpawnType = aLCTType;
	iOrder = aOrder;
}

CAmericanTroopTransporter::~CAmericanTroopTransporter()
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

CAmericanTroopTransporter* CAmericanTroopTransporter::New(TInt aXLocation,TBool aReflected, TInt aSecOffset, TAmericanLCTType aLCTType, SGroundUnitOrder& aOrder)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanTroopTransporterHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_TROOP_TRANSPORTER_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanTroopTransporter;

	CAmericanTroopTransporter* self = new CAmericanTroopTransporter(aXLocation,lAttributes,aSecOffset,aLCTType,aOrder);
	self->ConstructL();
	return self;
}

void CAmericanTroopTransporter::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanTroopTransporter), AMERICAN_TROOP_TRANSPORTER_WIDTH, AMERICAN_TROOP_TRANSPORTER_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_TROOP_TRANSPORTER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_TROOP_TRANSPORTER_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanTroopTransporter::CollisionPoints);

	//weapons
	iFrontMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iBackMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_BACK_X_LOCATION, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_BACK_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_TROOP_TRANSPORTER_LEFT_SMOKE_CREATOR_X_OFFSET, AMERICAN_TROOP_TRANSPORTER_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_TROOP_TRANSPORTER_MIDDLE_SMOKE_CREATOR_X_OFFSET, AMERICAN_TROOP_TRANSPORTER_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_TROOP_TRANSPORTER_RIGHT_SMOKE_CREATOR_X_OFFSET, AMERICAN_TROOP_TRANSPORTER_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CAmericanTroopTransporter::Update()
{
	ShipBasicUpdate();
	iFrontMachineGun->Update();
	iBackMachineGun->Update();
	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	
	if(iAlive && iSpeed == TIntFloat::Convert(0))
	{
		iCurrentFrame++;
		if(iCurrentFrame == (TROOPTRANSPORTER_FRAMES_UNTIL_LCT_SPAWN + iExtraFrameOffset))
		{
			//create a new bomber
			iCurrentFrame = iExtraFrameOffset; // otherwise spawns will not be in sync, and it will always longer between spawns
			TInt lXSpawnLocation;
			if(iObjectReflected)
				lXSpawnLocation = GetCurrentPositionNormilized().iX - (AMERICAN_LCT_WIDTH/2);
			else
				lXSpawnLocation = GetCurrentPositionNormilized().iX + (AMERICAN_LCT_WIDTH/2);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CAmericanLCT::New(lXSpawnLocation,iObjectReflected,iLCTSpawnType,iOrder));
		}
	}
}

void CAmericanTroopTransporter::Draw()
{
	iFrontMachineGun->Draw();
	iBackMachineGun->Draw();
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CAmericanTroopTransporter::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lFrontMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_FRONT_X_LOCATION, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_FRONT_Y_LOCATION);
	TPointIntFloat lBackMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_BACK_X_LOCATION, AMERICAN_TROOP_TRANSPORTER_DUAL_MACHINE_GUN_BACK_Y_LOCATION);
	iFrontMachineGun->Rotate(-1, lFrontMachineGunPosition);
	iBackMachineGun->Rotate(-1, lBackMachineGunPosition);
}

void CAmericanTroopTransporter::Die(TDamageType aType)
{
  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CAmericanTroopTransporter::InitilizeAmericanTroopTransporter()
{
	CAmericanTroopTransporter::LoadAmericanTroopTransporterTextures();
	CAmericanTroopTransporter::CreateAmericanTroopTransporterCollisionPointsL();
}

void CAmericanTroopTransporter::RemoveAmericanTroopTransporter()
{
	CAmericanTroopTransporter::UnloadAmericanTroopTransporterTextures();
	CAmericanTroopTransporter::RemoveAmericanTroopTransporterCollisionPoints();
}

void CAmericanTroopTransporter::LoadAmericanTroopTransporterTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanTroopTransporter);
}

void CAmericanTroopTransporter::UnloadAmericanTroopTransporterTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanTroopTransporter);
}

void CAmericanTroopTransporter::CreateAmericanTroopTransporterCollisionPointsL()
{
	CAmericanTroopTransporter::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_TROOP_TRANSPORTER_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanTroopTransporter::CollisionPoints[0].XOffset = 20;
	CAmericanTroopTransporter::CollisionPoints[0].YOffset = 1;
	CAmericanTroopTransporter::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CAmericanTroopTransporter::CollisionPoints[1].XOffset = 2;
	CAmericanTroopTransporter::CollisionPoints[1].YOffset = 60;
	CAmericanTroopTransporter::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[2].XOffset = 200;
	CAmericanTroopTransporter::CollisionPoints[2].YOffset = 58;
	CAmericanTroopTransporter::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[3].XOffset = 205;
	CAmericanTroopTransporter::CollisionPoints[3].YOffset = 98;
	CAmericanTroopTransporter::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[4].XOffset = 320;
	CAmericanTroopTransporter::CollisionPoints[4].YOffset = 98;
	CAmericanTroopTransporter::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[5].XOffset = 325;
	CAmericanTroopTransporter::CollisionPoints[5].YOffset = 58;
	CAmericanTroopTransporter::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[6].XOffset = 510;
	CAmericanTroopTransporter::CollisionPoints[6].YOffset = 58;
	CAmericanTroopTransporter::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[7].XOffset = 496;
	CAmericanTroopTransporter::CollisionPoints[7].YOffset = 8;
	CAmericanTroopTransporter::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[8].XOffset = 460;
	CAmericanTroopTransporter::CollisionPoints[8].YOffset = 2;
	CAmericanTroopTransporter::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CAmericanTroopTransporter::CollisionPoints[9].XOffset = 20;
	CAmericanTroopTransporter::CollisionPoints[9].YOffset = 0;
	CAmericanTroopTransporter::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
}

void CAmericanTroopTransporter::RemoveAmericanTroopTransporterCollisionPoints()
{
	if(CAmericanTroopTransporter::CollisionPoints)
	{
		delete[] CAmericanTroopTransporter::CollisionPoints;
		CAmericanTroopTransporter::CollisionPoints = NULL;
	}
}

void CAmericanTroopTransporter::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iExtraFrameOffset);
	aOutputStream.WriteInt32(iLCTSpawnType);
	aOutputStream.WriteInt32(iOrder.DefaultXPosition);	aOutputStream.WriteInt32(iOrder.MoveToLocation);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.LeftXPosition);	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.RightXPosition);
	aOutputStream.WriteInt32(iOrder.ShouldProtectArea);	aOutputStream.WriteInt32(iOrder.XLocationDestination);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iFrontMachineGun->SaveOnDisk(aOutputStream);
	iBackMachineGun->SaveOnDisk(aOutputStream);
}

void CAmericanTroopTransporter::LoadFromDisk(CFileReadStream &aInputStream)
{
	ShipLoadContentFromDisk(aInputStream);
	iCurrentFrame = aInputStream.ReadInt32();
	iExtraFrameOffset = aInputStream.ReadInt32();
	iLCTSpawnType = static_cast<TAmericanLCTType> (aInputStream.ReadInt32());
	iOrder.DefaultXPosition = aInputStream.ReadInt32(); iOrder.MoveToLocation = aInputStream.ReadInt32();
	iOrder.ProtectionArea.LeftXPosition = aInputStream.ReadIntFloat(); iOrder.ProtectionArea.RightXPosition = aInputStream.ReadIntFloat();
	iOrder.ShouldProtectArea = aInputStream.ReadInt32(); iOrder.XLocationDestination = aInputStream.ReadInt32();
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
