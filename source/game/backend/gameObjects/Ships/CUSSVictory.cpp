/*
 ============================================================================
 Name		: USSVictory.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CUSSVictory implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CUSSVictory::CollisionPoints;

CUSSVictory::CUSSVictory(TInt aXLocation, SGameObjectAttributes& aAttributes) :
	CShipCarrier(USS_VICTORY_Z, aXLocation, USS_VICTORY_KEEL_DEPTH, TIntFloat::Convert(SHIP_BASIC_SPEED), USS_VICTORY_FLAG_POLE_X_OFFSET, USS_VICTORY_FLAG_POLE_Y_OFFSET, aAttributes)
{
	//little trick to avoid that it spawns planes
	iDoesNotSpawnPlanes = true;
	iNumberOfAircraftsLeft = 0;
}

CUSSVictory::~CUSSVictory()
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

CUSSVictory* CUSSVictory::LoadUSSVictory()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::USSVictoryHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = USS_VICTORY_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierUSSVictory;
	CUSSVictory* self = new  CUSSVictory(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CUSSVictory* CUSSVictory::New(TInt aXLocation, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::USSVictoryHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = USS_VICTORY_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierUSSVictory;
	CUSSVictory* self = new  CUSSVictory(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CUSSVictory::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanCarrierUSSVictory), USS_VICTORY_WIDTH, USS_VICTORY_HEIGHT, iZCor, 0, 1, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, USS_VICTORY_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, USS_VICTORY_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CUSSVictory::CollisionPoints);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_LEFT_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_MIDDLE_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_RIGHT_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_TOP_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CUSSVictory::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanCarrierUSSVictory), USS_VICTORY_WIDTH, USS_VICTORY_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, USS_VICTORY_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, USS_VICTORY_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CUSSVictory::CollisionPoints);
	//create LandingSurface
	TPoint lLandingSurfacePosition = GetCurrentPositionNormilized();
	lLandingSurfacePosition.iY += USS_VICTORY_LANDING_SURFACE_HEIGHT_OFF_SET;

	//create the Flagpole that shows what side the airport is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += USS_VICTORY_FLAG_POLE_X_OFFSET;
	lFlagPolePosition.iY += USS_VICTORY_FLAG_POLE_Y_OFFSET;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	iLandingSurface = CLandingSurface::New(lLandingSurfacePosition, iConflictSide, TIntFloat::Convert(USS_VICTORY_LANDING_SURFACE_WIDTH), TIntFloat::Convert(USS_VICTORY_LANDING_SURFACE_HEIGHT));
	//add surface to currentMap
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLandingSurface);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_LEFT_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_MIDDLE_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_RIGHT_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, USS_VICTORY_TOP_SMOKE_CREATOR_X_OFFSET, USS_VICTORY_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CUSSVictory::Update()
{
	ShipCarrierBasicUpdate();

	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	iSmokeCreatorTop->Update();
}

void CUSSVictory::SinkingFunctionCallWhenShipMovesDown()
{
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, USS_VICTORY_FLAG_POLE_X_OFFSET, USS_VICTORY_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CUSSVictory::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, USS_VICTORY_FLAG_POLE_X_OFFSET, USS_VICTORY_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
}

void CUSSVictory::Die(TDamageType aType)
{
	iLandingSurface->SetLandingSurfaceDestroyed();

	MoveableObjectBasicDieRoutine();
	GameObjectMoveIntoBackground();
	iFlagPole->GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

CAircraft* CUSSVictory::CreateNewAircraft()
{
	CDauntless* lDauntless = CDauntless::New(false, TIntFloat::Convert(0), true, TPoint(0, 0));//position does matter since we reposition it anyway
	TPoint lCurrentPosition = GetCurrentPositionNormilized();
	//need to adjust it to be on the landing surface to the landing surface
	lCurrentPosition.iY += USS_VICTORY_LANDING_SURFACE_HEIGHT;
	lCurrentPosition.iY += USS_VICTORY_LANDING_SURFACE_HEIGHT_OFF_SET;
	if(iObjectReflected)
		lCurrentPosition.iX -= USS_VICTORY_AIRPLANE_POSITION_X_OFFSET;
	else
		lCurrentPosition.iX += USS_VICTORY_AIRPLANE_POSITION_X_OFFSET;
	lDauntless->SetPosition(lCurrentPosition);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lDauntless);
	return lDauntless;//all done
}

void CUSSVictory::InitilizeUSSVictory()
{
	CUSSVictory::LoadUSSVictoryTextures();
	CUSSVictory::CreateUSSVictoryCollisionPointsL();
}

void CUSSVictory::RemoveUSSVictory()
{
	CUSSVictory::UnloadUSSVictoryTextures();
	CUSSVictory::RemoveUSSVictoryCollisionPoints();
}

void CUSSVictory::LoadUSSVictoryTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanCarrierUSSVictory);
}

void CUSSVictory::UnloadUSSVictoryTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanCarrierUSSVictory);
}

void CUSSVictory::CreateUSSVictoryCollisionPointsL()
{
	CUSSVictory::CollisionPoints = new SBoundingCollisionPoint[USS_VICTORY_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CUSSVictory::CollisionPoints[0].XOffset = 55;
	CUSSVictory::CollisionPoints[0].YOffset = 18;
	CUSSVictory::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CUSSVictory::CollisionPoints[1].XOffset = 30;
	CUSSVictory::CollisionPoints[1].YOffset = 28;
	CUSSVictory::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CUSSVictory::CollisionPoints[2].XOffset = 10;
	CUSSVictory::CollisionPoints[2].YOffset = 65;
	CUSSVictory::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CUSSVictory::CollisionPoints[3].XOffset = 1007;
	CUSSVictory::CollisionPoints[3].YOffset = 65;
	CUSSVictory::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CUSSVictory::CollisionPoints[4].XOffset = 975;
	CUSSVictory::CollisionPoints[4].YOffset = 28;
	CUSSVictory::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CUSSVictory::CollisionPoints[5].XOffset = 970;
	CUSSVictory::CollisionPoints[5].YOffset = 3;
	CUSSVictory::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CUSSVictory::CollisionPoints[6].XOffset = 55;
	CUSSVictory::CollisionPoints[6].YOffset = 18;
	CUSSVictory::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
}

void CUSSVictory::RemoveUSSVictoryCollisionPoints()
{
	if(CUSSVictory::CollisionPoints)
	{
		delete[] CUSSVictory::CollisionPoints;
		CUSSVictory::CollisionPoints = NULL;
	}
}

void CUSSVictory::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipCarrierSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iSmokeCreatorTop->SaveOnDisk(aOutputStream);
}

void CUSSVictory::LoadFromDisk(CFileReadStream &aInputStream)
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
