/*
 ============================================================================
 Name		: JapaneseCarrierShokaku.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseCarrierShokaku implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CJapaneseCarrierShokaku::CollisionPoints;

CJapaneseCarrierShokaku::CJapaneseCarrierShokaku(TInt aXLocation, SGameObjectAttributes& aAttributes) :
CShipCarrier(JAPANESE_CARRIER_SHOKAKU_Z,aXLocation, JAPANESE_CARRIER_SHOKAKU_KEEL_DEPTH,TIntFloat::Convert(SHIP_BASIC_SPEED),JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_X_OFFSET,JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_Y_OFFSET, aAttributes)
{
}

CJapaneseCarrierShokaku::~CJapaneseCarrierShokaku()
{
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
	if(iSmokeCreatorTop)
	{
		delete iSmokeCreatorTop;
		iSmokeCreatorTop = NULL;
	}
}

CJapaneseCarrierShokaku* CJapaneseCarrierShokaku::LoadJapaneseCarrierShokaku()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseCarrierHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_CARRIER_SHOKAKU_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseCarrierShokaku;

	CJapaneseCarrierShokaku* self = new CJapaneseCarrierShokaku(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CJapaneseCarrierShokaku* CJapaneseCarrierShokaku::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseCarrierHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_CARRIER_SHOKAKU_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseCarrierShokaku;

	CJapaneseCarrierShokaku* self = new CJapaneseCarrierShokaku(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseCarrierShokaku::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseCarrierShokaku), JAPANESE_CARRIER_SHOKAKU_WIDTH, JAPANESE_CARRIER_SHOKAKU_HEIGHT, iZCor, 0, 1, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_CARRIER_SHOKAKU_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_CARRIER_SHOKAKU_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseCarrierShokaku::CollisionPoints);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_TOP_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CJapaneseCarrierShokaku::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseCarrierShokaku), JAPANESE_CARRIER_SHOKAKU_WIDTH, JAPANESE_CARRIER_SHOKAKU_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_CARRIER_SHOKAKU_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_CARRIER_SHOKAKU_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseCarrierShokaku::CollisionPoints);
	//create LandingSurface
	TPoint lLandingSurfacePosition = GetCurrentPositionNormilized();
	lLandingSurfacePosition.iY += JAPANESE_CARRIER_SHOKAKU_LANDING_SURFACE_HEIGHT_OFF_SET;

	//create the Flagpole that shows what side the airport is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_X_OFFSET;
	lFlagPolePosition.iY += JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_Y_OFFSET;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	iLandingSurface = CLandingSurface::New(lLandingSurfacePosition, iConflictSide, TIntFloat::Convert(JAPANESE_CARRIER_SHOKAKU_LANDING_SURFACE_WIDTH), TIntFloat::Convert(JAPANESE_CARRIER_SHOKAKU_LANDING_SURFACE_HEIGHT));
	//add surface to currentMap
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLandingSurface);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOKAKU_TOP_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CJapaneseCarrierShokaku::Update()
{
	ShipCarrierBasicUpdate();

	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	iSmokeCreatorTop->Update();
}

void CJapaneseCarrierShokaku::SinkingFunctionCallWhenShipMovesDown()
{
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CJapaneseCarrierShokaku::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_X_OFFSET, JAPANESE_CARRIER_SHOKAKU_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
}

void CJapaneseCarrierShokaku::Die(TDamageType aType)
{
	iLandingSurface->SetLandingSurfaceDestroyed();

  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
  iFlagPole->GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

CAircraft* CJapaneseCarrierShokaku::CreateNewAircraft()
{

	CVal* lVal = CVal::New(false, TIntFloat::Convert(0), false, TPoint(0, 0));//position does matter since we reposition it anyway
	TPoint lCurrentPosition = GetCurrentPositionNormilized();
	//need to adjust it to be on the landing surface to the landing surface
	lCurrentPosition.iY += JAPANESE_CARRIER_SHOKAKU_LANDING_SURFACE_HEIGHT;
	lCurrentPosition.iY += JAPANESE_CARRIER_SHOKAKU_LANDING_SURFACE_HEIGHT_OFF_SET;
	if(iObjectReflected)
		lCurrentPosition.iX -= JAPANESE_CARRIER_SHOKAKU_AIRPLANE_POSITION_X_OFFSET;
	else
		lCurrentPosition.iX += JAPANESE_CARRIER_SHOKAKU_AIRPLANE_POSITION_X_OFFSET;
	lVal->SetPosition(lCurrentPosition);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lVal);
	return lVal;//all done
}

void CJapaneseCarrierShokaku::InitilizeJapaneseCarrierShokaku()
{
	CJapaneseCarrierShokaku::LoadJapaneseCarrierShokakuTextures();
	CJapaneseCarrierShokaku::CreateJapaneseCarrierShokakuCollisionPointsL();
}

void CJapaneseCarrierShokaku::RemoveJapaneseCarrierShokaku()
{
	CJapaneseCarrierShokaku::UnloadJapaneseCarrierShokakuTextures();
	CJapaneseCarrierShokaku::RemoveJapaneseCarrierShokakuCollisionPoints();
}

void CJapaneseCarrierShokaku::LoadJapaneseCarrierShokakuTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseCarrierShokaku);
}

void CJapaneseCarrierShokaku::UnloadJapaneseCarrierShokakuTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseCarrierShokaku);
}

void CJapaneseCarrierShokaku::CreateJapaneseCarrierShokakuCollisionPointsL()
{
	CJapaneseCarrierShokaku::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_CARRIER_SHOKAKU_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseCarrierShokaku::CollisionPoints[0].XOffset = 40;
	CJapaneseCarrierShokaku::CollisionPoints[0].YOffset = 3;
	CJapaneseCarrierShokaku::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseCarrierShokaku::CollisionPoints[1].XOffset = 35;
	CJapaneseCarrierShokaku::CollisionPoints[1].YOffset = 33;
	CJapaneseCarrierShokaku::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[2].XOffset = 10;
	CJapaneseCarrierShokaku::CollisionPoints[2].YOffset = 48;
	CJapaneseCarrierShokaku::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[3].XOffset = 1;
	CJapaneseCarrierShokaku::CollisionPoints[3].YOffset = 68;
	CJapaneseCarrierShokaku::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[4].XOffset = 3;
	CJapaneseCarrierShokaku::CollisionPoints[4].YOffset = 84;
	CJapaneseCarrierShokaku::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[5].XOffset = 1017;
	CJapaneseCarrierShokaku::CollisionPoints[5].YOffset = 84;
	CJapaneseCarrierShokaku::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[6].XOffset = 1023;
	CJapaneseCarrierShokaku::CollisionPoints[6].YOffset = 51;
	CJapaneseCarrierShokaku::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[7].XOffset = 1015;
	CJapaneseCarrierShokaku::CollisionPoints[7].YOffset = 1;
	CJapaneseCarrierShokaku::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShokaku::CollisionPoints[8].XOffset = 12;
	CJapaneseCarrierShokaku::CollisionPoints[8].YOffset = 1;
	CJapaneseCarrierShokaku::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
}

void CJapaneseCarrierShokaku::RemoveJapaneseCarrierShokakuCollisionPoints()
{
	if(CJapaneseCarrierShokaku::CollisionPoints)
	{
		delete[] CJapaneseCarrierShokaku::CollisionPoints;
		CJapaneseCarrierShokaku::CollisionPoints = NULL;
	}
}

void CJapaneseCarrierShokaku::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipCarrierSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iSmokeCreatorTop->SaveOnDisk(aOutputStream);
}

void CJapaneseCarrierShokaku::LoadFromDisk(CFileReadStream &aInputStream)
{
	ShipCarrierLoadContentFromDisk(aInputStream);
	//load settings of smoke creators
	TWeaponIdentifier lSmokeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorLeft->LoadFromDisk(aInputStream);
	lSmokeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorMiddle->LoadFromDisk(aInputStream);
	lSmokeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorRight->LoadFromDisk(aInputStream);
	lSmokeCreator = static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorTop->LoadFromDisk(aInputStream);
}

