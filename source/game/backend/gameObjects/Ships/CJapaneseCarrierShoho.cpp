/*
 ============================================================================
 Name		: JapaneseCarrierShoho.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseCarrierShoho implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CJapaneseCarrierShoho::CollisionPoints;

CJapaneseCarrierShoho::CJapaneseCarrierShoho(TInt aXLocation, SGameObjectAttributes& aAttributes) :
CShipCarrier(JAPANESE_CARRIER_SHOHO_Z,aXLocation, JAPANESE_CARRIER_SHOHO_KEEL_DEPTH,TIntFloat::Convert(SHIP_BASIC_SPEED),JAPANESE_CARRIER_SHOHO_FLAG_POLE_X_OFFSET,JAPANESE_CARRIER_SHOHO_FLAG_POLE_Y_OFFSET, aAttributes)
{
}

CJapaneseCarrierShoho::~CJapaneseCarrierShoho()
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
	if(iSmokeCreatorBottomRight)
	{
		delete iSmokeCreatorBottomRight;
		iSmokeCreatorBottomRight = NULL;
	}
}

CJapaneseCarrierShoho* CJapaneseCarrierShoho::LoadJapaneseCarrierShoho()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseCarrierHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_CARRIER_SHOHO_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseCarrierShoho;
	CJapaneseCarrierShoho* self = new CJapaneseCarrierShoho(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CJapaneseCarrierShoho* CJapaneseCarrierShoho::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseCarrierHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_CARRIER_SHOHO_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseCarrierShoho;
	CJapaneseCarrierShoho* self = new CJapaneseCarrierShoho(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseCarrierShoho::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseCarrierShoho), JAPANESE_CARRIER_SHOHO_WIDTH, JAPANESE_CARRIER_SHOHO_HEIGHT, iZCor, 0, 1, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_CARRIER_SHOHO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_CARRIER_SHOHO_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseCarrierShoho::CollisionPoints);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorBottomRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_BOTTOM_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_BOTTOM_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CJapaneseCarrierShoho::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseCarrierShoho), JAPANESE_CARRIER_SHOHO_WIDTH, JAPANESE_CARRIER_SHOHO_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_CARRIER_SHOHO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_CARRIER_SHOHO_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseCarrierShoho::CollisionPoints);
	//create LandingSurface
	TPoint lLandingSurfacePosition = GetCurrentPositionNormilized();
	lLandingSurfacePosition.iY += JAPANESE_CARRIER_SHOHO_LANDING_SURFACE_HEIGHT_OFF_SET;

	//create the Flagpole that shows what side the airport is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += JAPANESE_CARRIER_SHOHO_FLAG_POLE_X_OFFSET;
	lFlagPolePosition.iY += JAPANESE_CARRIER_SHOHO_FLAG_POLE_Y_OFFSET;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	iLandingSurface = CLandingSurface::New(lLandingSurfacePosition, iConflictSide, TIntFloat::Convert(JAPANESE_CARRIER_SHOHO_LANDING_SURFACE_WIDTH), TIntFloat::Convert(JAPANESE_CARRIER_SHOHO_LANDING_SURFACE_HEIGHT));
	//add surface to currentMap
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLandingSurface);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorBottomRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_SHOHO_BOTTOM_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_SHOHO_BOTTOM_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CJapaneseCarrierShoho::Update()
{
	ShipCarrierBasicUpdate();

	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	iSmokeCreatorBottomRight->Update();
}

void CJapaneseCarrierShoho::SinkingFunctionCallWhenShipMovesDown()
{
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_CARRIER_SHOHO_FLAG_POLE_X_OFFSET, JAPANESE_CARRIER_SHOHO_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CJapaneseCarrierShoho::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_CARRIER_SHOHO_FLAG_POLE_X_OFFSET, JAPANESE_CARRIER_SHOHO_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
}

void CJapaneseCarrierShoho::Die(TDamageType aType)
{
	iLandingSurface->SetLandingSurfaceDestroyed();

  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
  iFlagPole->GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

CAircraft* CJapaneseCarrierShoho::CreateNewAircraft()
{

	CVal* lVal = CVal::New(false, TIntFloat::Convert(0), false, TPoint(0, 0));//position does matter since we reposition it anyway
	TPoint lCurrentPosition = GetCurrentPositionNormilized();
	//need to adjust it to be on the landing surface to the landing surface
	lCurrentPosition.iY += JAPANESE_CARRIER_SHOHO_LANDING_SURFACE_HEIGHT;
	lCurrentPosition.iY += JAPANESE_CARRIER_SHOHO_LANDING_SURFACE_HEIGHT_OFF_SET;
	if(iObjectReflected)
		lCurrentPosition.iX -= JAPANESE_CARRIER_SHOHO_AIRPLANE_POSITION_X_OFFSET;
	else
		lCurrentPosition.iX += JAPANESE_CARRIER_SHOHO_AIRPLANE_POSITION_X_OFFSET;
	lVal->SetPosition(lCurrentPosition);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lVal);
	return lVal;//all done
}

void CJapaneseCarrierShoho::InitilizeJapaneseCarrierShoho()
{
	CJapaneseCarrierShoho::LoadJapaneseCarrierShohoTextures();
	CJapaneseCarrierShoho::CreateJapaneseCarrierShohoCollisionPointsL();
}

void CJapaneseCarrierShoho::RemoveJapaneseCarrierShoho()
{
	CJapaneseCarrierShoho::UnloadJapaneseCarrierShohoTextures();
	CJapaneseCarrierShoho::RemoveJapaneseCarrierShohoCollisionPoints();
}

void CJapaneseCarrierShoho::LoadJapaneseCarrierShohoTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseCarrierShoho);
}

void CJapaneseCarrierShoho::UnloadJapaneseCarrierShohoTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseCarrierShoho);
}

void CJapaneseCarrierShoho::CreateJapaneseCarrierShohoCollisionPointsL()
{
	CJapaneseCarrierShoho::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_CARRIER_SHOHO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseCarrierShoho::CollisionPoints[0].XOffset = 40;
	CJapaneseCarrierShoho::CollisionPoints[0].YOffset = 1;
	CJapaneseCarrierShoho::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseCarrierShoho::CollisionPoints[1].XOffset = 3;
	CJapaneseCarrierShoho::CollisionPoints[1].YOffset = 38;
	CJapaneseCarrierShoho::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[2].XOffset = 5;
	CJapaneseCarrierShoho::CollisionPoints[2].YOffset = 53;
	CJapaneseCarrierShoho::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[3].XOffset = 110;
	CJapaneseCarrierShoho::CollisionPoints[3].YOffset = 53;
	CJapaneseCarrierShoho::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[4].XOffset = 115;
	CJapaneseCarrierShoho::CollisionPoints[4].YOffset = 92;
	CJapaneseCarrierShoho::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[5].XOffset = 908;
	CJapaneseCarrierShoho::CollisionPoints[5].YOffset = 92;
	CJapaneseCarrierShoho::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[6].XOffset = 850;
	CJapaneseCarrierShoho::CollisionPoints[6].YOffset = 88;
	CJapaneseCarrierShoho::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[7].XOffset = 855;
	CJapaneseCarrierShoho::CollisionPoints[7].YOffset = 43;
	CJapaneseCarrierShoho::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[8].XOffset = 1020;
	CJapaneseCarrierShoho::CollisionPoints[8].YOffset = 52;
	CJapaneseCarrierShoho::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[9].XOffset = 1005;
	CJapaneseCarrierShoho::CollisionPoints[9].YOffset = 8;
	CJapaneseCarrierShoho::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierShoho::CollisionPoints[10].XOffset = 41;
	CJapaneseCarrierShoho::CollisionPoints[10].YOffset = 0;
	CJapaneseCarrierShoho::CollisionPoints[10].iMovementType = EPointMovementTypeNone;
}

void CJapaneseCarrierShoho::RemoveJapaneseCarrierShohoCollisionPoints()
{
	if(CJapaneseCarrierShoho::CollisionPoints)
	{
		delete[] CJapaneseCarrierShoho::CollisionPoints;
		CJapaneseCarrierShoho::CollisionPoints = NULL;
	}
}

void CJapaneseCarrierShoho::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipCarrierSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iSmokeCreatorBottomRight->SaveOnDisk(aOutputStream);
}

void CJapaneseCarrierShoho::LoadFromDisk(CFileReadStream &aInputStream)
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
	iSmokeCreatorBottomRight->LoadFromDisk(aInputStream);
}
