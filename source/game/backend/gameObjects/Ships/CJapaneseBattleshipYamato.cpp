/*
 ============================================================================
 Name		: JapaneseBattleshipYamato.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseBattleshipYamato implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

SBoundingCollisionPoint* CJapaneseBattleshipYamato::CollisionPoints;

CJapaneseBattleshipYamato::CJapaneseBattleshipYamato(TInt aXLocation, SGameObjectAttributes& aAttributes) :
	CShip(JAPANESE_BATTLESHIP_YAMATO_Z,aXLocation, JAPANESE_BATTLESHIP_YAMATO_KEEL_DEPTH, TIntFloat::Convert(SHIP_BASIC_SPEED), aAttributes)
{
}

CJapaneseBattleshipYamato::~CJapaneseBattleshipYamato()
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
	if(i7InchGun)
	{
		delete i7InchGun;
		i7InchGun = NULL;
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
	if(iSmokeCreatorTopMiddle)
	{
		delete iSmokeCreatorTopMiddle;
		iSmokeCreatorTopMiddle  = NULL;
	}
	if(iSmokeCreatorRight)
	{
		delete iSmokeCreatorRight;
		iSmokeCreatorRight = NULL;
	}
}

CJapaneseBattleshipYamato* CJapaneseBattleshipYamato::LoadJapaneseBattleshipYamato()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseBattleshipYamatoHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_BATTLESHIP_YAMATO_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseBattleshipYamato;

	CJapaneseBattleshipYamato* self = new CJapaneseBattleshipYamato(0, lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CJapaneseBattleshipYamato* CJapaneseBattleshipYamato::New(TInt aXLocation,TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseBattleshipYamatoHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_BATTLESHIP_YAMATO_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseBattleshipYamato;

	CJapaneseBattleshipYamato* self = new CJapaneseBattleshipYamato(aXLocation, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseBattleshipYamato::ConstructWithoutCreatingOtherObjectsL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseBattleshipYamato), JAPANESE_BATTLESHIP_YAMATO_WIDTH, JAPANESE_BATTLESHIP_YAMATO_HEIGHT, iZCor, 0, 1, 0, 0.625);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseBattleshipYamato::CollisionPoints);
	
	//14 Inch cannons
	iFront14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, JAPANESE_BATTLESHIP_YAMATO_14_INCH_FRONT_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_14_INCH_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	iFrontTop14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, JAPANESE_BATTLESHIP_YAMATO_14_INCH_TOP_FRONT_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_14_INCH_TOP_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	//7 Inch Cannon
	i7InchGun = CGun7Inch::New(&iCoordinates, &iAngle,45, JAPANESE_BATTLESHIP_YAMATO_7_INCH_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_7_INCH_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this,JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_MUZZELES_PER_7_INCH_TURRENT);
	//Machine Guns
	iFrontMachineGun1 = CShipDualMachineGun::New(&iCoordinates, &iAngle,110, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_1_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	iFrontMachineGun2 = CShipDualMachineGun::New(&iCoordinates, &iAngle,170, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_2_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iFrontMachineGun3 = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_3_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iBack1MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iBack2MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 120, this);
	iBack3MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_3_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	//75 AA Guns
	iFront75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_FRONT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iBack75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,170, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_BACK_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_BACK_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 160, this);
	//Smoke Creators
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_LEFT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_LEFT_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_MIDDLE_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_MIDDLE_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTopMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_TOP_MIDDLE_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_TOP_MIDDLE_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_RIGHT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_RIGHT_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	
	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

void CJapaneseBattleshipYamato::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseBattleshipYamato), JAPANESE_BATTLESHIP_YAMATO_WIDTH, JAPANESE_BATTLESHIP_YAMATO_HEIGHT, iZCor, 0, 1, 0, 0.625);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseBattleshipYamato::CollisionPoints);
	
	//14 Inch cannons
	iFront14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, JAPANESE_BATTLESHIP_YAMATO_14_INCH_FRONT_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_14_INCH_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	iFrontTop14InchGun = CGun14Inch::New(&iCoordinates, &iAngle,45, JAPANESE_BATTLESHIP_YAMATO_14_INCH_TOP_FRONT_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_14_INCH_TOP_FRONT_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this, JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_MUZZELES_PER_14_INCH_TURRENT);
	//7 Inch Cannon
	i7InchGun = CGun7Inch::New(&iCoordinates, &iAngle,45, JAPANESE_BATTLESHIP_YAMATO_7_INCH_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_7_INCH_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 45, this,JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_MUZZELES_PER_7_INCH_TURRENT);
	//Machine Guns
	iFrontMachineGun1 = CShipDualMachineGun::New(&iCoordinates, &iAngle,110, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_1_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	iFrontMachineGun2 = CShipDualMachineGun::New(&iCoordinates, &iAngle,170, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_2_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 190, this);
	iFrontMachineGun3 = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_3_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iBack1MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,180, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 180, this);
	iBack2MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 120, this);
	iBack3MachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,200, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_3_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_3_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 130, this);
	//75 AA Guns
	iFront75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,160, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_FRONT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_FRONT_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 140, this);
	iBack75mmGun = CShipDual75mmGun::New(&iCoordinates, &iAngle,170, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_BACK_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_BACK_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 160, this);
	//FlagPole
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += JAPANESE_BATTLESHIP_YAMATO_FLAG_X_LOCATION;
	lFlagPolePosition.iY += JAPANESE_BATTLESHIP_YAMATO_FLAG_Y_LOCATION;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);
	//Smoke Creators
	iSmokeCreatorLeft = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_LEFT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_LEFT_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_MIDDLE_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_MIDDLE_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorTopMiddle = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_TOP_MIDDLE_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_TOP_MIDDLE_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iSmokeCreatorRight = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_RIGHT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_SMOKE_CREATOR_RIGHT_Y_LOCATION, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	
	//used when the ship starts sinking
	iSoundObject = CSoundObject::New(ESoundFileId_Ship_Creak_Middle, iHitBox);
	BasicShipPostConstructCall();
}

//------------------ functions ---------------------------//


void CJapaneseBattleshipYamato::Update()
{
	ShipBasicUpdate();
	//14 Inch Guns
	iFront14InchGun->Update();
	iFrontTop14InchGun->Update();
	//7 Inch Gun
	i7InchGun->Update();
//	//Machine Guns
	iFrontMachineGun1->Update();
	iFrontMachineGun2->Update();
	iFrontMachineGun3->Update();
	iBack1MachineGun->Update();
	iBack2MachineGun->Update();
	iBack3MachineGun->Update();
//	//75 A Guns
	iFront75mmGun->Update();
	iBack75mmGun->Update();
//	//smoke creator
	iSmokeCreatorLeft->Update();
	iSmokeCreatorMiddle->Update();
	iSmokeCreatorTopMiddle->Update();
	iSmokeCreatorRight->Update();
	//need to relocate flag in case ship moves
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_FLAG_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_FLAG_Y_LOCATION);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);
}

void CJapaneseBattleshipYamato::Draw()
{
	//14 Inch Guns
	iFront14InchGun->Draw();
	iFrontTop14InchGun->Draw();
	//7 Inch Gun
	i7InchGun->Draw();
//	//Machine Guns
	iFrontMachineGun1->Draw();
	iFrontMachineGun2->Draw();
	iFrontMachineGun3->Draw();
	iBack1MachineGun->Draw();
	iBack2MachineGun->Draw();
	iBack3MachineGun->Draw();
//	//75 A Guns
	iFront75mmGun->Draw();
	iBack75mmGun->Draw();
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CJapaneseBattleshipYamato::UpdateGunPriorities()
{
	iFront14InchGun->GetShipArteriallyAI()->SetTargetPriorities(iTargetPriority);
	iFrontTop14InchGun->GetShipArteriallyAI()->SetTargetPriorities(iTargetPriority);
}

void CJapaneseBattleshipYamato::SinkingFunctionCallWhenShipRotates()
{
	//update flag
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_FLAG_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_FLAG_Y_LOCATION);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(-1);
	//update 14 Inch Guns
	TPointIntFloat l14InchGunLocation;
	l14InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_14_INCH_FRONT_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_14_INCH_FRONT_GUN_Y_LOCATION);
	iFront14InchGun->Rotate(-1,l14InchGunLocation);
	l14InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_14_INCH_TOP_FRONT_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_14_INCH_TOP_FRONT_GUN_Y_LOCATION);
	iFrontTop14InchGun->Rotate(-1,l14InchGunLocation);
	//Update 7 Inch Gun
	TPointIntFloat l7InchGunLocation = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_7_INCH_GUN_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_7_INCH_GUN_Y_LOCATION);
	i7InchGun->Rotate(-1,l7InchGunLocation);
	//MachineGuns
	TPointIntFloat lMachineGunPosition;
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_1_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_1_Y_LOCATION);
	iFrontMachineGun1->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_2_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_2_Y_LOCATION);
	iFrontMachineGun2->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_3_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_FRONT_3_Y_LOCATION);
	iFrontMachineGun3->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_1_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_1_Y_LOCATION);
	iBack1MachineGun->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_2_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_2_Y_LOCATION);
	iBack2MachineGun->Rotate(-1,lMachineGunPosition);
	lMachineGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_3_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_DUAL_MACHINE_GUN_BACK_3_Y_LOCATION);
	iBack3MachineGun->Rotate(-1,lMachineGunPosition);
	//75 mm
	TPointIntFloat l75mmGunPosition;
	l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_FRONT_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_FRONT_Y_LOCATION);
	iFront75mmGun->Rotate(-1,l75mmGunPosition);
	l75mmGunPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_BACK_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_75MM_GUN_BACK_Y_LOCATION);
	iBack75mmGun->Rotate(-1,l75mmGunPosition);
}

void CJapaneseBattleshipYamato::SinkingFunctionCallWhenShipMovesDown()
{
	//need to rotate all attached objects as well
	TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, JAPANESE_BATTLESHIP_YAMATO_FLAG_X_LOCATION, JAPANESE_BATTLESHIP_YAMATO_FLAG_Y_LOCATION);
	iFlagPole->SetPosition(lNewPositionOfFlag);
	iFlagPole->Rotate(0);//little trick to reallign the flag and flagpole
}

void CJapaneseBattleshipYamato::Die(TDamageType aType)
{
  MoveableObjectBasicDieRoutine();
  GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CJapaneseBattleshipYamato::InitilizeJapaneseBattleshipYamato()
{
	CJapaneseBattleshipYamato::LoadJapaneseBattleshipYamatoTextures();
	CJapaneseBattleshipYamato::CreateJapaneseBattleshipYamatoCollisionPointsL();
}

void CJapaneseBattleshipYamato::RemoveJapaneseBattleshipYamato()
{
	CJapaneseBattleshipYamato::UnloadJapaneseBattleshipYamatoTextures();
	CJapaneseBattleshipYamato::RemoveJapaneseBattleshipYamatoCollisionPoints();
}

void CJapaneseBattleshipYamato::LoadJapaneseBattleshipYamatoTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseBattleshipYamato);
}

void CJapaneseBattleshipYamato::UnloadJapaneseBattleshipYamatoTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseBattleshipYamato);
}

void CJapaneseBattleshipYamato::CreateJapaneseBattleshipYamatoCollisionPointsL()
{
	CJapaneseBattleshipYamato::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_BATTLESHIP_YAMATO_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseBattleshipYamato::CollisionPoints[0].XOffset = 18;
	CJapaneseBattleshipYamato::CollisionPoints[0].YOffset = 2;
	CJapaneseBattleshipYamato::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseBattleshipYamato::CollisionPoints[1].XOffset = 30;
	CJapaneseBattleshipYamato::CollisionPoints[1].YOffset = 30;
	CJapaneseBattleshipYamato::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[2].XOffset = 5;
	CJapaneseBattleshipYamato::CollisionPoints[2].YOffset = 65;
	CJapaneseBattleshipYamato::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[3].XOffset = 267;
	CJapaneseBattleshipYamato::CollisionPoints[3].YOffset = 61;
	CJapaneseBattleshipYamato::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[4].XOffset = 265;
	CJapaneseBattleshipYamato::CollisionPoints[4].YOffset = 76;
	CJapaneseBattleshipYamato::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[5].XOffset = 400;
	CJapaneseBattleshipYamato::CollisionPoints[5].YOffset = 76;
	CJapaneseBattleshipYamato::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[6].XOffset = 398;
	CJapaneseBattleshipYamato::CollisionPoints[6].YOffset = 93;
	CJapaneseBattleshipYamato::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[7].XOffset = 535;
	CJapaneseBattleshipYamato::CollisionPoints[7].YOffset = 93;
	CJapaneseBattleshipYamato::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[8].XOffset = 534;
	CJapaneseBattleshipYamato::CollisionPoints[8].YOffset = 103;
	CJapaneseBattleshipYamato::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[9].XOffset = 610;
	CJapaneseBattleshipYamato::CollisionPoints[9].YOffset = 103;
	CJapaneseBattleshipYamato::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[10].XOffset = 633;
	CJapaneseBattleshipYamato::CollisionPoints[10].YOffset = 134;
	CJapaneseBattleshipYamato::CollisionPoints[10].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[11].XOffset = 635;
	CJapaneseBattleshipYamato::CollisionPoints[11].YOffset = 159;
	CJapaneseBattleshipYamato::CollisionPoints[11].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[12].XOffset = 700;
	CJapaneseBattleshipYamato::CollisionPoints[12].YOffset = 159;
	CJapaneseBattleshipYamato::CollisionPoints[12].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[13].XOffset = 704;
	CJapaneseBattleshipYamato::CollisionPoints[13].YOffset = 107;
	CJapaneseBattleshipYamato::CollisionPoints[13].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[14].XOffset = 723;
	CJapaneseBattleshipYamato::CollisionPoints[14].YOffset = 107;
	CJapaneseBattleshipYamato::CollisionPoints[14].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[15].XOffset = 812;
	CJapaneseBattleshipYamato::CollisionPoints[15].YOffset = 105;
	CJapaneseBattleshipYamato::CollisionPoints[15].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[16].XOffset = 817;
	CJapaneseBattleshipYamato::CollisionPoints[16].YOffset = 81;
	CJapaneseBattleshipYamato::CollisionPoints[16].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[17].XOffset = 940;
	CJapaneseBattleshipYamato::CollisionPoints[17].YOffset = 81;
	CJapaneseBattleshipYamato::CollisionPoints[17].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[18].XOffset = 944;
	CJapaneseBattleshipYamato::CollisionPoints[18].YOffset = 60;
	CJapaneseBattleshipYamato::CollisionPoints[18].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[19].XOffset = 965;
	CJapaneseBattleshipYamato::CollisionPoints[19].YOffset = 55;
	CJapaneseBattleshipYamato::CollisionPoints[19].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[20].XOffset = 1020;
	CJapaneseBattleshipYamato::CollisionPoints[20].YOffset = 55;
	CJapaneseBattleshipYamato::CollisionPoints[20].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[21].XOffset = 1015;
	CJapaneseBattleshipYamato::CollisionPoints[21].YOffset = 25;
	CJapaneseBattleshipYamato::CollisionPoints[21].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[22].XOffset = 1010;
	CJapaneseBattleshipYamato::CollisionPoints[22].YOffset = 4;
	CJapaneseBattleshipYamato::CollisionPoints[22].iMovementType = EPointMovementTypeNone;
	CJapaneseBattleshipYamato::CollisionPoints[23].XOffset = 18;
	CJapaneseBattleshipYamato::CollisionPoints[23].YOffset = 2;
	CJapaneseBattleshipYamato::CollisionPoints[23].iMovementType = EPointMovementTypeNone;
}

void CJapaneseBattleshipYamato::RemoveJapaneseBattleshipYamatoCollisionPoints()
{
	if(CJapaneseBattleshipYamato::CollisionPoints)
	{
		delete[] CJapaneseBattleshipYamato::CollisionPoints;
		CJapaneseBattleshipYamato::CollisionPoints = NULL;
	}
}

void CJapaneseBattleshipYamato::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	aOutputStream.WriteUint32((TUint32)iFlagPole);
	iFront14InchGun->SaveOnDisk(aOutputStream);
	iFrontTop14InchGun->SaveOnDisk(aOutputStream);
	i7InchGun->SaveOnDisk(aOutputStream);
	iFrontMachineGun1->SaveOnDisk(aOutputStream);
	iFrontMachineGun2->SaveOnDisk(aOutputStream);
	iFrontMachineGun3->SaveOnDisk(aOutputStream);
	iBack1MachineGun->SaveOnDisk(aOutputStream);
	iBack2MachineGun->SaveOnDisk(aOutputStream);
	iBack3MachineGun->SaveOnDisk(aOutputStream);
	iFront75mmGun->SaveOnDisk(aOutputStream);
	iBack75mmGun->SaveOnDisk(aOutputStream);
	iSmokeCreatorLeft->SaveOnDisk(aOutputStream);
	iSmokeCreatorMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorTopMiddle->SaveOnDisk(aOutputStream);
	iSmokeCreatorRight->SaveOnDisk(aOutputStream);
}

void CJapaneseBattleshipYamato::LoadFromDisk(CFileReadStream &aInputStream)
{
	ShipLoadContentFromDisk(aInputStream);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aInputStream.ReadUint32(), (TUint32)(&iFlagPole));
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFront14InchGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontTop14InchGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	i7InchGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun1->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun2->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFrontMachineGun3->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack1MachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack2MachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack3MachineGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iFront75mmGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iBack75mmGun->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorLeft->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorMiddle->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorTopMiddle->LoadFromDisk(aInputStream);
	static_cast<TWeaponIdentifier> (aInputStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreatorRight->LoadFromDisk(aInputStream);
}

