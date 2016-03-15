/*
 ============================================================================
 Name		: AmericanBattleshipUSSTennessee.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanBattleshipUSSTennessee implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint* CAmericanBattleshipUSSTennessee::CollisionPoints;

CAmericanBattleshipUSSTennessee::CAmericanBattleshipUSSTennessee(TInt aXLocation, SGameObjectAttributes& aAttributes) :
	CShip(AMERICAN_BATTLESHIP_USS_TENNESSEE_Z,aXLocation, AMERICAN_BATTLESHIP_USS_TENNESSEE_KEEL_DEPTH,TIntFloat::Convert(SHIP_BASIC_SPEED), aAttributes)
{
}

CAmericanBattleshipUSSTennessee::~CAmericanBattleshipUSSTennessee()
{
	if(iFront14InchGun)
	{
		delete iFront14InchGun;
		iFront14InchGun = NULL;
	}
	if(iFrontTop14InchGun)
	{
		delete iFrontTop14InchGun;
		iFrontTop14InchGun = NULL;
	}
	if(iFrontMachineGun1)
	{
		delete iFrontMachineGun1;
		iFrontMachineGun1 = NULL;
	}
	if(iFrontMachineGun2)
	{
		delete iFrontMachineGun2;
		iFrontMachineGun2 = NULL;
	}
	if(iFrontMachineGun3)
	{
		delete iFrontMachineGun3;
		iFrontMachineGun3 = NULL;
	}
	if(iTopMachineGun)
	{
		delete iTopMachineGun;
		iTopMachineGun = NULL;
	}
	if(iBack1MachineGun)
	{
		delete iBack1MachineGun;
		iBack1MachineGun = NULL;
	}
	if(iBack2MachineGun)
	{
		delete iBack2MachineGun;
		iBack2MachineGun = NULL;
	}
	if(iBack3MachineGun)
	{
		delete iBack3MachineGun;
		iBack3MachineGun = NULL;
	}
	if(iFront75mmGun)
	{
		delete iFront75mmGun;
		iFront75mmGun = NULL;
	}
	if(iTop75mmGun)
	{
		delete iTop75mmGun;
		iTop75mmGun = NULL;
	}
	if(iBack75mmGun)
	{
		delete iBack75mmGun;
		iBack75mmGun = NULL;
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

CAmericanBattleshipUSSTennessee* CAmericanBattleshipUSSTennessee::LoadAmericanBattleshipUSSTennessee()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanBattleshipUSSTennesseeHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = false;
	lAttributes.Armor = AMERICAN_BATTLESHIP_USS_TENNESSEE_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanBattleshipUSSTennessee;

	CAmericanBattleshipUSSTennessee* self = new CAmericanBattleshipUSSTennessee(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CAmericanBattleshipUSSTennessee* CAmericanBattleshipUSSTennessee::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanBattleshipUSSTennesseeHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_BATTLESHIP_USS_TENNESSEE_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanBattleshipUSSTennessee;

	CAmericanBattleshipUSSTennessee* self = new CAmericanBattleshipUSSTennessee(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanBattleshipUSSTennessee::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanBattleshipUSSTennessee), AMERICAN_BATTLESHIP_USS_TENNESSEE_WIDTH, AMERICAN_BATTLESHIP_USS_TENNESSEE_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanBattleshipUSSTennessee::CollisionPoints);
	
	//14 Inch cannons
	iFront14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_FRONT_GUN_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	iFrontTop14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_TOP_FRONT_GUN_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_TOP_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	//Machine Guns
	iFrontMachineGun1 = CShipDualMachineGun::New(&iCoordinates, &iAngle,110, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_1_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	iFrontMachineGun2 = CShipDualMachineGun::New(&iCoordinates, &iAngle,170, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_2_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iFrontMachineGun3 = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_3_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iTopMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_TOP_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_TOP_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 200, this);
	iBack1MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iBack2MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iBack3MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_3_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	//75 AA Guns
	iFront75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_FRONT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iTop75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_TOP_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_TOP_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iBack75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,170, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_BACK_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_BACK_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 160, this);
	//Smoke Creators
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_LEFT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_LEFT_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_MIDDLE_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_MIDDLE_Y_LOCATION,  false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_RIGHT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_RIGHT_Y_LOCATION,  false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	
	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CAmericanBattleshipUSSTennessee::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipAmericanBattleshipUSSTennessee), AMERICAN_BATTLESHIP_USS_TENNESSEE_WIDTH, AMERICAN_BATTLESHIP_USS_TENNESSEE_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanBattleshipUSSTennessee::CollisionPoints);
	
	//14 Inch cannons
	iFront14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_FRONT_GUN_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	iFrontTop14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_TOP_FRONT_GUN_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_TOP_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	//Machine Guns
	iFrontMachineGun1 = CShipDualMachineGun::New(&iCoordinates, &iAngle,110, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_1_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	iFrontMachineGun2 = CShipDualMachineGun::New(&iCoordinates, &iAngle,170, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_2_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iFrontMachineGun3 = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_3_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iTopMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_TOP_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_TOP_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 200, this);
	iBack1MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iBack2MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iBack3MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_3_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	//75 AA Guns
	iFront75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_FRONT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iTop75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_TOP_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_TOP_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iBack75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,170, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_BACK_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_BACK_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 160, this);
	//FlagPole
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_X_LOCATION;
	lFlagPolePosition.iY += AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_Y_LOCATION;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);
	//Smoke Creators
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_LEFT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_LEFT_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_MIDDLE_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_MIDDLE_Y_LOCATION,  false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_RIGHT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_SMOKE_CREATOR_RIGHT_Y_LOCATION,  false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	
	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CAmericanBattleshipUSSTennessee::Update()
{
	ShipBasicUpdate();
	//14 Inch Guns
	iFront14InchGun->Update();
	iFrontTop14InchGun->Update();
	//Machine Guns
	iFrontMachineGun1->Update();
	iFrontMachineGun2->Update();
	iFrontMachineGun3->Update();
	iTopMachineGun->Update();
	iBack1MachineGun->Update();
	iBack2MachineGun->Update();
	iBack3MachineGun->Update();
	//75 A Guns
	iFront75mmGun->Update();
	iTop75mmGun->Update();
	iBack75mmGun->Update();
	//smoke creator
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorRight->Update();
	//need to relocate flag in case ship moves
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_Y_LOCATION);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);
}

void CAmericanBattleshipUSSTennessee::Draw()
{
	//14 Inch Guns
	iFront14InchGun->Draw();
	iFrontTop14InchGun->Draw();
	//Machine Guns
	iFrontMachineGun1->Draw();
	iFrontMachineGun2->Draw();
	iFrontMachineGun3->Draw();
	iTopMachineGun->Draw();
	iBack1MachineGun->Draw();
	iBack2MachineGun->Draw();
	iBack3MachineGun->Draw();
	//75 A Guns
	iFront75mmGun->Draw();
	iTop75mmGun->Draw();
	iBack75mmGun->Draw();
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CAmericanBattleshipUSSTennessee::UpdateGunPriorities()
{
	iFront14InchGun->GetShipArteriallyAI()->SetTargetPriorities(iTargetPriority);
	iFrontTop14InchGun->GetShipArteriallyAI()->SetTargetPriorities(iTargetPriority);
}

void CAmericanBattleshipUSSTennessee::SinkingFunctionCallWhenShipRotates()
{
	//update flag
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_Y_LOCATION);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
	//update 14 Inch Guns
	TPointIntFloat l14InchGunLocation;
	l14InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_FRONT_GUN_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_FRONT_GUN_Y_LOCATION);
	iFront14InchGun->Rotate(-1,l14InchGunLocation);
	l14InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_TOP_FRONT_GUN_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_14_INCH_TOP_FRONT_GUN_Y_LOCATION);
	iFrontTop14InchGun->Rotate(-1,l14InchGunLocation);
	//MachineGuns
	TPointIntFloat lMachineGunPosition;
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_1_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_1_Y_LOCATION);
	iFrontMachineGun1->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_2_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_2_Y_LOCATION);
	iFrontMachineGun2->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_3_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_FRONT_3_Y_LOCATION);
	iFrontMachineGun3->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_TOP_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_TOP_Y_LOCATION);
	iTopMachineGun->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION);
	iBack1MachineGun->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION);
	iBack2MachineGun->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_3_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_DUAL_MACHINE_GUN_BACK_3_Y_LOCATION);
	iBack3MachineGun->Rotate(-1,lMachineGunPosition);
	//75 mm
	TPointIntFloat l75mmGunPosition;
	l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_FRONT_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_FRONT_Y_LOCATION);
	iFront75mmGun->Rotate(-1,l75mmGunPosition);
	l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_TOP_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_TOP_Y_LOCATION);
	iTop75mmGun->Rotate(-1,l75mmGunPosition);
	l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_BACK_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_75MM_GUN_BACK_Y_LOCATION);
	iBack75mmGun->Rotate(-1,l75mmGunPosition);
}

void CAmericanBattleshipUSSTennessee::SinkingFunctionCallWhenShipMovesDown()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_X_LOCATION, AMERICAN_BATTLESHIP_USS_TENNESSEE_FLAG_Y_LOCATION);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CAmericanBattleshipUSSTennessee::Die(TDamageType aType)
{
  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CAmericanBattleshipUSSTennessee::InitilizeAmericanBattleshipUSSTennessee()
{
	CAmericanBattleshipUSSTennessee::LoadAmericanBattleshipUSSTennesseeTextures();
	CAmericanBattleshipUSSTennessee::CreateAmericanBattleshipUSSTennesseeCollisionPointsL();
}

void CAmericanBattleshipUSSTennessee::RemoveAmericanBattleshipUSSTennessee()
{
	CAmericanBattleshipUSSTennessee::UnloadAmericanBattleshipUSSTennesseeTextures();
	CAmericanBattleshipUSSTennessee::RemoveAmericanBattleshipUSSTennesseeCollisionPoints();
}

void CAmericanBattleshipUSSTennessee::LoadAmericanBattleshipUSSTennesseeTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanBattleshipUSSTennessee);
}

void CAmericanBattleshipUSSTennessee::UnloadAmericanBattleshipUSSTennesseeTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanBattleshipUSSTennessee);
}

void CAmericanBattleshipUSSTennessee::CreateAmericanBattleshipUSSTennesseeCollisionPointsL()
{
	CAmericanBattleshipUSSTennessee::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_BATTLESHIP_USS_TENNESSEE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanBattleshipUSSTennessee::CollisionPoints[0].XOffset = 16;
	CAmericanBattleshipUSSTennessee::CollisionPoints[0].YOffset = 5;
	CAmericanBattleshipUSSTennessee::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CAmericanBattleshipUSSTennessee::CollisionPoints[1].XOffset = 2;
	CAmericanBattleshipUSSTennessee::CollisionPoints[1].YOffset = 60;
	CAmericanBattleshipUSSTennessee::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[2].XOffset = 425;
	CAmericanBattleshipUSSTennessee::CollisionPoints[2].YOffset = 60;
	CAmericanBattleshipUSSTennessee::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[3].XOffset = 430;
	CAmericanBattleshipUSSTennessee::CollisionPoints[3].YOffset = 73;
	CAmericanBattleshipUSSTennessee::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[4].XOffset = 560;
	CAmericanBattleshipUSSTennessee::CollisionPoints[4].YOffset = 73;
	CAmericanBattleshipUSSTennessee::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[5].XOffset = 570;
	CAmericanBattleshipUSSTennessee::CollisionPoints[5].YOffset = 113;
	CAmericanBattleshipUSSTennessee::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[6].XOffset = 630;
	CAmericanBattleshipUSSTennessee::CollisionPoints[6].YOffset = 113;
	CAmericanBattleshipUSSTennessee::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[7].XOffset = 655;
	CAmericanBattleshipUSSTennessee::CollisionPoints[7].YOffset = 103;
	CAmericanBattleshipUSSTennessee::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[8].XOffset = 660;
	CAmericanBattleshipUSSTennessee::CollisionPoints[8].YOffset = 78;
	CAmericanBattleshipUSSTennessee::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[9].XOffset = 707;
	CAmericanBattleshipUSSTennessee::CollisionPoints[9].YOffset = 78;
	CAmericanBattleshipUSSTennessee::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[10].XOffset = 710;
	CAmericanBattleshipUSSTennessee::CollisionPoints[10].YOffset = 70;
	CAmericanBattleshipUSSTennessee::CollisionPoints[10].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[11].XOffset = 835;
	CAmericanBattleshipUSSTennessee::CollisionPoints[11].YOffset = 70;
	CAmericanBattleshipUSSTennessee::CollisionPoints[11].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[12].XOffset = 840;
	CAmericanBattleshipUSSTennessee::CollisionPoints[12].YOffset = 48;
	CAmericanBattleshipUSSTennessee::CollisionPoints[12].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[13].XOffset = 1020;
	CAmericanBattleshipUSSTennessee::CollisionPoints[13].YOffset = 48;
	CAmericanBattleshipUSSTennessee::CollisionPoints[13].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[14].XOffset = 1023;
	CAmericanBattleshipUSSTennessee::CollisionPoints[14].YOffset = 18;
	CAmericanBattleshipUSSTennessee::CollisionPoints[14].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[15].XOffset = 1010;
	CAmericanBattleshipUSSTennessee::CollisionPoints[15].YOffset = 3;
	CAmericanBattleshipUSSTennessee::CollisionPoints[15].iMovementType = EPointMovementTypeNone;
	CAmericanBattleshipUSSTennessee::CollisionPoints[16].XOffset = 16;
	CAmericanBattleshipUSSTennessee::CollisionPoints[16].YOffset = 5;
	CAmericanBattleshipUSSTennessee::CollisionPoints[16].iMovementType = EPointMovementTypeNone;
}

void CAmericanBattleshipUSSTennessee::RemoveAmericanBattleshipUSSTennesseeCollisionPoints()
{
	if(CAmericanBattleshipUSSTennessee::CollisionPoints)
	{
		delete[] CAmericanBattleshipUSSTennessee::CollisionPoints;
		CAmericanBattleshipUSSTennessee::CollisionPoints = NULL;
	}
}

void CAmericanBattleshipUSSTennessee::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	aOutputStream.WriteUint32((TUint32)iFlagPole);
	iFront14InchGun->SaveOnDisk(aOutputStream);
	iFrontTop14InchGun->SaveOnDisk(aOutputStream);
	iFrontMachineGun1->SaveOnDisk(aOutputStream);
	iFrontMachineGun2->SaveOnDisk(aOutputStream);
	iFrontMachineGun3->SaveOnDisk(aOutputStream);
	iTopMachineGun->SaveOnDisk(aOutputStream);
	iBack1MachineGun->SaveOnDisk(aOutputStream);
	iBack2MachineGun->SaveOnDisk(aOutputStream);
	iBack3MachineGun->SaveOnDisk(aOutputStream);
	iFront75mmGun->SaveOnDisk(aOutputStream);
	iTop75mmGun->SaveOnDisk(aOutputStream);
	iBack75mmGun->SaveOnDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
}

void CAmericanBattleshipUSSTennessee::LoadFromDisk(CFileReadStream &aInputStream)
{
	ShipLoadContentFromDisk(aInputStream);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aInputStream.ReadUint32(), (TUint32)(&iFlagPole));
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFront14InchGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontTop14InchGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun1->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun2->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun3->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iTopMachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack1MachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack2MachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack3MachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFront75mmGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iTop75mmGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack75mmGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorLeft->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorMiddle->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorRight->LoadFromDisk(aInputStream);
}
