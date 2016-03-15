/*
 ============================================================================
 Name		: AmericanCarrierLexington.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanCarrierLexington implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint * CAmericanCarrierLexington::CollisionPoints;

CAmericanCarrierLexington::CAmericanCarrierLexington(TInt aXLocation, SGameObjectAttributes& aAttributes) :
CShipCarrier(AMERICAN_CARRIER_LEXINGTON_Z, aXLocation, AMERICAN_CARRIER_LEXINGTON_KEEL_DEPTH, TIntFloat::Convert(SHIP_BASIC_SPEED), AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_Y_OFFSET, aAttributes)
{
}

CAmericanCarrierLexington::~CAmericanCarrierLexington()
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

CAmericanCarrierLexington* CAmericanCarrierLexington::LoadAmericanCarrierLexington()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanCarrierHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = AMERICAN_CARRIER_LEXINGTON_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanCarrierLexington;

	CAmericanCarrierLexington* self = new CAmericanCarrierLexington(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CAmericanCarrierLexington* CAmericanCarrierLexington::New(TInt aXLocation, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanCarrierHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_CARRIER_LEXINGTON_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanCarrierLexington;

	CAmericanCarrierLexington* self = new CAmericanCarrierLexington(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanCarrierLexington::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanCarrierUSSLexington), AMERICAN_CARRIER_LEXINGTON_WIDTH, AMERICAN_CARRIER_LEXINGTON_HEIGHT, iZCor, 0, 1, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_CARRIER_LEXINGTON_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_CARRIER_LEXINGTON_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanCarrierLexington::CollisionPoints);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_LEFT_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_MIDDLE_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_RIGHT_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_TOP_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CAmericanCarrierLexington::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanCarrierUSSLexington), AMERICAN_CARRIER_LEXINGTON_WIDTH, AMERICAN_CARRIER_LEXINGTON_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_CARRIER_LEXINGTON_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_CARRIER_LEXINGTON_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanCarrierLexington::CollisionPoints);
	//create LandingSurface
	TPoint lLandingSurfacePosition = GetCurrentPositionNormilized();
	lLandingSurfacePosition.iY += AMERICAN_CARRIER_LEXINGTON_LANDING_SURFACE_HEIGHT_OFF_SET;
	iLandingSurface = CLandingSurface::New(lLandingSurfacePosition, iConflictSide, TIntFloat::Convert(AMERICAN_CARRIER_LEXINGTON_LANDING_SURFACE_WIDTH), TIntFloat::Convert(AMERICAN_CARRIER_LEXINGTON_LANDING_SURFACE_HEIGHT));
	//add surface to currentMap
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLandingSurface);

	//create the Flagpole that shows what side the airport is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_X_OFFSET;
	lFlagPolePosition.iY += AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_Y_OFFSET;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	//create some creators that will be triggered when the ship gets Damaged
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_LEFT_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_LEFT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_MIDDLE_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_MIDDLE_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_RIGHT_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_RIGHT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTop = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_CARRIER_LEXINGTON_TOP_SMOKE_CREATOR_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_TOP_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);

	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CAmericanCarrierLexington::Update()
{
	ShipCarrierBasicUpdate();

	//let smoke creators run as well
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	iSmokeCreatorTop->Update();
}

void CAmericanCarrierLexington::SinkingFunctionCallWhenShipMovesDown()
{
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CAmericanCarrierLexington::SinkingFunctionCallWhenShipRotates()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_X_OFFSET, AMERICAN_CARRIER_LEXINGTON_FLAG_POLE_Y_OFFSET);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
}

void CAmericanCarrierLexington::Die(TDamageType aType)
{
	iLandingSurface->SetLandingSurfaceDestroyed();

	MoveableObjectBasicDieRoutine();
	GameObjectMoveIntoBackground();
	iFlagPole->GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

CAircraft* CAmericanCarrierLexington::CreateNewAircraft()
{
	CDauntless* lDauntless = CDauntless::New(false, TIntFloat::Convert(0), false, TPoint(0, 0));//position does matter since we reposition it anyway
	TPoint lCurrentPosition = GetCurrentPositionNormilized();
	//need to adjust it to be on the landing surface to the landing surface
	lCurrentPosition.iY += AMERICAN_CARRIER_LEXINGTON_LANDING_SURFACE_HEIGHT;
	lCurrentPosition.iY += AMERICAN_CARRIER_LEXINGTON_LANDING_SURFACE_HEIGHT_OFF_SET;
	if(iObjectReflected)
		lCurrentPosition.iX -= AMERICAN_CARRIER_LEXINGTON_AIRPLANE_POSITION_X_OFFSET;
	else
		lCurrentPosition.iX += AMERICAN_CARRIER_LEXINGTON_AIRPLANE_POSITION_X_OFFSET;
	lDauntless->SetPosition(lCurrentPosition);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lDauntless);
	return lDauntless;//all done
}

void CAmericanCarrierLexington::InitilizeAmericanCarrierLexington()
{
	CAmericanCarrierLexington::LoadAmericanCarrierLexingtonTextures();
	CAmericanCarrierLexington::CreateAmericanCarrierLexingtonCollisionPointsL();
}

void CAmericanCarrierLexington::RemoveAmericanCarrierLexington()
{
	CAmericanCarrierLexington::UnloadAmericanCarrierLexingtonTextures();
	CAmericanCarrierLexington::RemoveAmericanCarrierLexingtonCollisionPoints();
}

void CAmericanCarrierLexington::LoadAmericanCarrierLexingtonTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanCarrierUSSLexington);
}

void CAmericanCarrierLexington::UnloadAmericanCarrierLexingtonTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanCarrierUSSLexington);
}

void CAmericanCarrierLexington::CreateAmericanCarrierLexingtonCollisionPointsL()
{
	CAmericanCarrierLexington::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_CARRIER_LEXINGTON_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanCarrierLexington::CollisionPoints[0].XOffset = 30;
	CAmericanCarrierLexington::CollisionPoints[0].YOffset = 1;
	CAmericanCarrierLexington::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CAmericanCarrierLexington::CollisionPoints[1].XOffset = 25;
	CAmericanCarrierLexington::CollisionPoints[1].YOffset = 28;
	CAmericanCarrierLexington::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanCarrierLexington::CollisionPoints[2].XOffset = 2;
	CAmericanCarrierLexington::CollisionPoints[2].YOffset = 40;
	CAmericanCarrierLexington::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanCarrierLexington::CollisionPoints[3].XOffset = 1015;
	CAmericanCarrierLexington::CollisionPoints[3].YOffset = 40;
	CAmericanCarrierLexington::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanCarrierLexington::CollisionPoints[4].XOffset = 1010;
	CAmericanCarrierLexington::CollisionPoints[4].YOffset = 8;
	CAmericanCarrierLexington::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanCarrierLexington::CollisionPoints[5].XOffset = 1000;
	CAmericanCarrierLexington::CollisionPoints[5].YOffset = 0;
	CAmericanCarrierLexington::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CAmericanCarrierLexington::CollisionPoints[6].XOffset = 30;
	CAmericanCarrierLexington::CollisionPoints[6].YOffset = 1;
	CAmericanCarrierLexington::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
}

void CAmericanCarrierLexington::RemoveAmericanCarrierLexingtonCollisionPoints()
{
	if(CAmericanCarrierLexington::CollisionPoints)
	{
		delete[] CAmericanCarrierLexington::CollisionPoints;
		CAmericanCarrierLexington::CollisionPoints = NULL;
	}
}

void CAmericanCarrierLexington::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipCarrierSaveContentToDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
	iSmokeCreatorTop->SaveOnDisk(aOutputStream);
}

void CAmericanCarrierLexington::LoadFromDisk(CFileReadStream &aInputStream)
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
