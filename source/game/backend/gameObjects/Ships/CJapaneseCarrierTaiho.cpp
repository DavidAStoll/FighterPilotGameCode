/*
 ============================================================================
 Name		: JapaneseCarrierTaiho.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseCarrierTaiho implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CJapaneseCarrierTaiho::CollisionPoints;

CJapaneseCarrierTaiho::CJapaneseCarrierTaiho(TInt aXLocation, SGameObjectAttributes& aAttributes) :
CShipCarrier(JAPANESE_CARRIER_TAIHO_Z,aXLocation, JAPANESE_CARRIER_TAIHO_KEEL_DEPTH,TIntFloat::Convert(SHIP_BASIC_SPEED),JAPANESE_CARRIER_TAIHO_FLAG_POLE_X_OFFSET,JAPANESE_CARRIER_TAIHO_FLAG_POLE_Y_OFFSET, aAttributes)
{
}

CJapaneseCarrierTaiho::~CJapaneseCarrierTaiho()
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

CJapaneseCarrierTaiho* CJapaneseCarrierTaiho::LoadJapaneseCarrierTaiho()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseCarrierHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_CARRIER_TAIHO_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseCarrierTaiho;

	CJapaneseCarrierTaiho* self = new CJapaneseCarrierTaiho(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CJapaneseCarrierTaiho* CJapaneseCarrierTaiho::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseCarrierHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_CARRIER_TAIHO_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseCarrierTaiho;

	CJapaneseCarrierTaiho* self = new CJapaneseCarrierTaiho(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseCarrierTaiho::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseCarrierTaiho), JAPANESE_CARRIER_TAIHO_WIDTH, JAPANESE_CARRIER_TAIHO_HEIGHT, iZCor, 0, 1, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_CARRIER_TAIHO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_CARRIER_TAIHO_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseCarrierTaiho::CollisionPoints);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_TOP_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CJapaneseCarrierTaiho::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseCarrierTaiho), JAPANESE_CARRIER_TAIHO_WIDTH, JAPANESE_CARRIER_TAIHO_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_CARRIER_TAIHO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_CARRIER_TAIHO_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseCarrierTaiho::CollisionPoints);
	//create LandingSurface
	TPoint lLandingSurfacePosition = GetCurrentPositionNormilized();
	lLandingSurfacePosition.iY += JAPANESE_CARRIER_TAIHO_LANDING_SURFACE_HEIGHT_OFF_SET;

	//create the Flagpole that shows what side the airport is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += JAPANESE_CARRIER_TAIHO_FLAG_POLE_X_OFFSET;
	lFlagPolePosition.iY += JAPANESE_CARRIER_TAIHO_FLAG_POLE_Y_OFFSET;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	iLandingSurface = CLandingSurface::New(lLandingSurfacePosition, iConflictSide, TIntFloat::Convert(JAPANESE_CARRIER_TAIHO_LANDING_SURFACE_WIDTH), TIntFloat::Convert(JAPANESE_CARRIER_TAIHO_LANDING_SURFACE_HEIGHT));
	//add surface to currentMap
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLandingSurface);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_LEFT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_MIDDLE_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_RIGHT_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_CARRIER_TAIHO_TOP_SMOKE_CREATOR_X_OFFSET, JAPANESE_CARRIER_TAIHO_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CJapaneseCarrierTaiho::Update()
{
	ShipCarrierBasicUpdate();

	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	iSmokeCreatorTop->Update();
}

void CJapaneseCarrierTaiho::SinkingFunctionCallWhenShipMovesDown()
{
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_CARRIER_TAIHO_FLAG_POLE_X_OFFSET, JAPANESE_CARRIER_TAIHO_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CJapaneseCarrierTaiho::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_CARRIER_TAIHO_FLAG_POLE_X_OFFSET, JAPANESE_CARRIER_TAIHO_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
}

void CJapaneseCarrierTaiho::Die(TDamageType aType)
{
	iLandingSurface->SetLandingSurfaceDestroyed();

  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
  iFlagPole->GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

CAircraft* CJapaneseCarrierTaiho::CreateNewAircraft()
{

	CVal* lVal = CVal::New(false, TIntFloat::Convert(0), false, TPoint(0, 0));//position does matter since we reposition it anyway
	TPoint lCurrentPosition = GetCurrentPositionNormilized();
	//need to adjust it to be on the landing surface to the landing surface
	lCurrentPosition.iY += JAPANESE_CARRIER_TAIHO_LANDING_SURFACE_HEIGHT;
	lCurrentPosition.iY += JAPANESE_CARRIER_TAIHO_LANDING_SURFACE_HEIGHT_OFF_SET;
	if(iObjectReflected)
		lCurrentPosition.iX -= JAPANESE_CARRIER_TAIHO_AIRPLANE_POSITION_X_OFFSET;
	else
		lCurrentPosition.iX += JAPANESE_CARRIER_TAIHO_AIRPLANE_POSITION_X_OFFSET;
	lVal->SetPosition(lCurrentPosition);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lVal);
	return lVal;//all done
}

void CJapaneseCarrierTaiho::InitilizeJapaneseCarrierTaiho()
{
	CJapaneseCarrierTaiho::LoadJapaneseCarrierTaihoTextures();
	CJapaneseCarrierTaiho::CreateJapaneseCarrierTaihoCollisionPointsL();
}

void CJapaneseCarrierTaiho::RemoveJapaneseCarrierTaiho()
{
	CJapaneseCarrierTaiho::UnloadJapaneseCarrierTaihoTextures();
	CJapaneseCarrierTaiho::RemoveJapaneseCarrierTaihoCollisionPoints();
}

void CJapaneseCarrierTaiho::LoadJapaneseCarrierTaihoTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseCarrierTaiho);
}

void CJapaneseCarrierTaiho::UnloadJapaneseCarrierTaihoTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseCarrierTaiho);
}

void CJapaneseCarrierTaiho::CreateJapaneseCarrierTaihoCollisionPointsL()
{
	CJapaneseCarrierTaiho::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_CARRIER_TAIHO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseCarrierTaiho::CollisionPoints[0].XOffset = 40;
	CJapaneseCarrierTaiho::CollisionPoints[0].YOffset = 1;
	CJapaneseCarrierTaiho::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseCarrierTaiho::CollisionPoints[1].XOffset = 35;
	CJapaneseCarrierTaiho::CollisionPoints[1].YOffset = 33;
	CJapaneseCarrierTaiho::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[2].XOffset = 3;
	CJapaneseCarrierTaiho::CollisionPoints[2].YOffset = 71;
	CJapaneseCarrierTaiho::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[3].XOffset = 1015;
	CJapaneseCarrierTaiho::CollisionPoints[3].YOffset = 71;
	CJapaneseCarrierTaiho::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[4].XOffset = 1017;
	CJapaneseCarrierTaiho::CollisionPoints[4].YOffset = 84;
	CJapaneseCarrierTaiho::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[5].XOffset = 1022;
	CJapaneseCarrierTaiho::CollisionPoints[5].YOffset = 48;
	CJapaneseCarrierTaiho::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[6].XOffset = 1015;
	CJapaneseCarrierTaiho::CollisionPoints[6].YOffset = 20;
	CJapaneseCarrierTaiho::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[7].XOffset = 995;
	CJapaneseCarrierTaiho::CollisionPoints[7].YOffset = 3;
	CJapaneseCarrierTaiho::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseCarrierTaiho::CollisionPoints[8].XOffset = 40;
	CJapaneseCarrierTaiho::CollisionPoints[8].YOffset = 1;
	CJapaneseCarrierTaiho::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
}

void CJapaneseCarrierTaiho::RemoveJapaneseCarrierTaihoCollisionPoints()
{
	if(CJapaneseCarrierTaiho::CollisionPoints)
	{
		delete[] CJapaneseCarrierTaiho::CollisionPoints;
		CJapaneseCarrierTaiho::CollisionPoints = NULL;
	}
}

void CJapaneseCarrierTaiho::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipCarrierSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iSmokeCreatorTop->SaveOnDisk(aOutputStream);
}

void CJapaneseCarrierTaiho::LoadFromDisk(CFileReadStream &aInputStream)
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

