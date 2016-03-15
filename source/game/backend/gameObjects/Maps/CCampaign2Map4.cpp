/*
 ============================================================================
 Name		: Campaign2Map4.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map4 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign2Map4.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign2Map4::CCampaign2Map4()
:	CMap(EGameMapCampaign2Mission4,CAMPAIGN2_MAP4_SIZE, CAMPAIGN2_MAP4_INTERVALS, CAMPAIGN2_MAP4_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign2Map4::~CCampaign2Map4()
	{
	}

CCampaign2Map4* CCampaign2Map4::New()
	{
	CCampaign2Map4* self = new  CCampaign2Map4();
	self->Construct();
	return self;
	}

void CCampaign2Map4::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CGetBonusPointsIfObjectsSurvive* lBonusCondition = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.AllAmericanShipsSurvived", CAMPAIGN2_MAP4_ALL_AMERICAN_SHIPS_SURVIVE);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//American fleet
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(12100, false);
		lLexington->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_BUILDINGS);
		lBonusCondition->AddGameObject(lLexington);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//Victory
		CUSSVictory* lHomeCarrier = CUSSVictory::New(10800,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		lLoseEventVictory->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		lBonusCondition->AddGameObject(lHomeCarrier);
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//Tennessee
		CAmericanBattleshipUSSTennessee* lTennessee = CAmericanBattleshipUSSTennessee::New(9500,false);
		lTennessee->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_BUILDINGS);
		lBonusCondition->AddGameObject(lTennessee);
		iGameObjectManager->AddMoveableGameObject(lTennessee);
		//Destroyer
		CAmericanDestroyer* lAmericanDestroyer = CAmericanDestroyer::New(8800,false);
		lAmericanDestroyer->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_BUILDINGS);
		lBonusCondition->AddGameObject(lAmericanDestroyer);
		iGameObjectManager->AddMoveableGameObject(lAmericanDestroyer);
		
		//create some Japanese ships on right side
		iGameObjectManager->AddMoveableGameObject(CJapaneseDestroyer::New(6500,true));
		iGameObjectManager->AddMoveableGameObject(CJapaneseOilTanker::New(5700,true));
		SGroundUnitOrder lOrder;
		lOrder.MoveToLocation = false;
		lOrder.ShouldProtectArea = false;
		iGameObjectManager->AddMoveableGameObject(CJapaneseTroopTransporter::New(5100,true,60*60,EJapaneseLCTTypeTroopsOnly,lOrder, false));
		
		//--------------------- RIGHT BEACH ---------------------//
		iGameObjectManager->AddGameObject(CTree2::New(4900, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4800, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(4750)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(4700)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(4620, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(4550)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(4450)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(4390, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4300, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(4200)); //width 80
		//------------------- SMALL FOREST BEFORE AIRPORT -------------------//
		CJapaneseSquad::CreateJapaneseSoldierSquad(3400,true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(3400)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3100)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3000)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2900)); //width 80
		iGameObjectManager->AddGameObject(CSmallTree::New(2800,ESmallTreeNormal)); //width 64
		//------------------- AIRPORT -------------------//
		iGameObjectManager->AddGameObject(CAirport::New(1700,EConflictSideJapanese)); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1400)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1350)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(1250)); //width 50
		iGameObjectManager->AddGameObject(CComStation::New(1150,EConflictSideJapanese,10,0)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(1040)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(950)); //width 32
		//------------------ FOREST ------------------//
		CJapaneseSquad::CreateJapaneseTankSquad(300,true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(200)); //Width width 700
		iGameObjectManager->AddGameObject(CBigTree::New(-150,EBigTreeReflected)); //width 128
		CJapaneseSquad::CreateJapaneseTankSquad(-450,true); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(-550,EBigTreeNormal)); //width 128
		//----------------- AIRPORT -----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-650)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-700)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-800)); //width 50
		iGameObjectManager->AddGameObject(CComStation::New(-900,EConflictSideJapanese,0,200)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1000)); //width 32
		iGameObjectManager->AddGameObject(CAirport::New(-2200,EConflictSideJapanese)); //widht to the left 175, witdh to the right 1050
		//----------------- AIRPORT ----------------------//
		iGameObjectManager->AddGameObject(CAirport::New(-3500,EConflictSideJapanese)); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(CHq::New(-4400,EConflictSideJapanese)); //width to the left 450, to the right 670
		iGameObjectManager->AddGameObject(CTree2::New(-4900, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-5000, ETree2MiddleTreeSmaller)); //width 64
		//------------- LEFT WATER SHIPS -----------------//
		iGameObjectManager->AddMoveableGameObject(CJapaneseDestroyer::New(-5700,true));
		iGameObjectManager->AddMoveableGameObject(CJapaneseOilTanker::New(-6400,true));
		iGameObjectManager->AddMoveableGameObject(CJapaneseTroopTransporter::New(-7200,true,60*60,EJapaneseLCTTypeTroopsOnly,lOrder, false));
		iGameObjectManager->AddMoveableGameObject(CJapaneseDestroyer::New(-8000,false));
		
		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorWater::New(5000, 9950));   //right  side
		iGameObjectManager->AddGameObject(CFloorLand::New(-5000, 10000));
		iGameObjectManager->AddGameObject(CFloorWater::New(-14950, 9950));   //left  side

		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(new  CSpawnMitchellEveryTimeInterval(60*4,0,TIME_EVENT_INFINITE_TIME_EVENTS, GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_BUILDINGS));
		iEvents->Append(new  CSpawnMitchellEveryTimeInterval(60*4,60,TIME_EVENT_INFINITE_TIME_EVENTS, GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_BUILDINGS));
		iEvents->Append(new  CSpawnMitchellEveryTimeInterval(60*4,60 * 2,TIME_EVENT_INFINITE_TIME_EVENTS,  GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_BUILDINGS));
		iEvents->Append(new  CWinIfAllJapaneseForcesAreDestroyed(CAMPAIGN2_MAP4_DESTROY_ALL_JAPANESE_FORCES_BONUS_SCROE));
		iEvents->Append(lBonusCondition);
	}
}

void CCampaign2Map4::InitializeGameObjectSettingsForMap()
{
	//Planes
	CDauntless::InitilizeDauntless();
	CBetty::InitilizeBetty();
	CMitchell::InitilizeMitchell();
	CVal::InitilizeVal();
	//Buildings
	CBigTree::InitilizeBigTree();
	CSmallTree::InitilizeSmallTree();
	CTree1::InitilizeTree1();
	CTree2::InitilizeTree2();
	CJapaneseAAConcrete::InitilizeJapaneseAAConcrete();
	CJapaneseAA75mmType88::InitilizeJapaneseAA75mmType88();
	CJapaneseAA20mmMachineCannon::InitilizeJapaneseAA20mmMachineCannon();
	CFlagPole::InitilizeFlagPole();
	CAirport::InitilizeAirport();
	CHangar::InitilizeHangar();
	CHq::InitilizeHq();
	CComStation::InitilizeComStation();
	//Projectile
	CBullet13mm::InitilizeBullet13mm();
	CExplosive88mmShell::InitilizeExplosive88mmShell();
	CBomb1000kg::InitilizeBomb1000kg();
	CBomb500kg::InitilizeBomb500kg();
	CStandardRocketWeapon::InitilizeStandardRocketWeapon();
	CStandardRocket::InitilizeStandardRocket();
	CBomb100kg::InitilizeBomb100kg();
	CExplosive75mmShell::InitilizeExplosive75mmShell();
	CClusterBomb::InitilizeClusterBomb();
	CTorpedo::InitilizeTorpedo();
	CUnderWaterTorpedo::InitilizeUnderWaterTorpedo();
	//Explosions
	CExplosion1000Kg::InitilizeExplosion1000Kg();
	CExplosion500Kg::InitilizeExplosion500Kg();
	CExplosion1000KgInAir::InitilizeExplosion1000KgInAir();
	CExplosion500KgInAir::InitilizeExplosion500KgInAir();
	CExplosion1000KgInWater::InitilizeExplosion1000KgInWater();
	CExplosion500KgInWater::InitilizeExplosion500KgInWater();
	CExplosion100KgInWater::InitilizeExplosion100KgInWater();
	CExplosion100Kg::InitilizeExplosion100Kg();
	CWaterSplash::InitilizeWaterSplash();
	CExplosion100KgInAir::InitilizeExplosion100KgInAir();
	//Ground Units
	CTankShell80mm::InitilizeTankShell80mm();
	CTankAAGun::InitilizeTankAAGun();
	CBullet8mm::InitilizeBullet8mm();
	CPropelledGrenade::InitilizePropelledGrenade();
	CJapaneseRifleman::InitilizeJapaneseRifleman();
	CJapaneseRocketman::InitilizeJapaneseRocketman();
	CJapanese97ChiHaTank::InitilizeJapanese97ChiHaTank();
	CJapaneseAA97ChiHaTank::InitilizeJapaneseAA97ChiHaTank();
	//Ship Weapons and Projectiles
	CExplosive7InchShell::InitilizeExplosive7InchShell();
	CGun7Inch::InitilizeGun7Inch();
	CGun7Inch::InitilizeCamoGun7Inch();
	CShipDualMachineGun::InitilizeShipDualMachineGun();
	CShipDualMachineGun::InitilizeCamoShipDualMachineGun();
	CShipDual75mmGun::InitilizeShipDual75mmGun();
	CShipDual75mmGun::InitilizeCamoShipDual75mmGun();
	//Extra Battleship objects needed
	CExplosion14InchShellInWater::InitilizeExplosion14InchShellInWater();
	CExplosion14InchShell::InitilizeExplosion14InchShell();
	CExplosive14InchShell::InitilizeExplosive14InchShell();
	CGun14Inch::InitilizeGun14Inch();
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	CAmericanCarrierLexington::InitilizeAmericanCarrierLexington();
	CAmericanDestroyer::InitilizeAmericanDestroyer();
	CAmericanBattleshipUSSTennessee::InitilizeAmericanBattleshipUSSTennessee();
	CJapaneseOilTanker::InitilizeJapaneseOilTanker();
	CJapaneseTroopTransporter::InitilizeJapaneseTroopTransporter();
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign2Map4::RemoveGameObjectSettingsForMap()
{
	//Planes
	CDauntless::RemoveDauntless();
	CBetty::RemoveBetty();
	CMitchell::RemoveMitchell();
	CVal::RemoveVal();
	//Buildings
	CBigTree::RemoveBigTree();
	CSmallTree::RemoveSmallTree();
	CTree1::RemoveTree1();
	CTree2::RemoveTree2();
	CJapaneseAAConcrete::RemoveJapaneseAAConcrete();
	CJapaneseAA75mmType88::RemoveJapaneseAA75mmType88();
	CJapaneseAA20mmMachineCannon::RemoveJapaneseAA20mmMachineCannon();
	CFlagPole::RemoveFlagPole();
	CAirport::RemoveAirport();
	CHangar::RemoveHangar();
	CHq::RemoveHq();
	CComStation::RemoveComStation();
	//Projectile
	CBullet13mm::RemoveBullet13mm();
	CExplosive88mmShell::RemoveExplosive88mmShell();
	CBomb1000kg::RemoveBomb1000kg();
	CBomb500kg::RemoveBomb500kg();
	CStandardRocketWeapon::RemoveStandardRocketWeapon();
	CStandardRocket::RemoveStandardRocket();
	CBomb100kg::RemoveBomb100kg();
	CExplosive75mmShell::RemoveExplosive75mmShell();
	CClusterBomb::RemoveClusterBomb();
	CTorpedo::RemoveTorpedo();
	CUnderWaterTorpedo::RemoveUnderWaterTorpedo();
	//Explosions
	CExplosion1000Kg::RemoveExplosion1000Kg();
	CExplosion500Kg::RemoveExplosion500Kg();
	CExplosion1000KgInAir::RemoveExplosion1000KgInAir();
	CExplosion500KgInAir::RemoveExplosion500KgInAir();
	CExplosion1000KgInWater::RemoveExplosion1000KgInWater();
	CExplosion500KgInWater::RemoveExplosion500KgInWater();
	CExplosion100KgInWater::RemoveExplosion100KgInWater();
	CExplosion100Kg::RemoveExplosion100Kg();
	CWaterSplash::RemoveWaterSplash();
	CExplosion100KgInAir::RemoveExplosion100KgInAir();
	//Ground Units
	CTankShell80mm::RemoveTankShell80mm();
	CTankAAGun::RemoveTankAAGun();
	CBullet8mm::RemoveBullet8mm();
	CPropelledGrenade::RemovePropelledGrenade();
	CJapaneseRifleman::RemoveJapaneseRifleman();
	CJapaneseRocketman::RemoveJapaneseRocketman();
	CAmericanRifleman::RemoveAmericanRifleman();
	CAmericanRocketman::RemoveAmericanRocketman();
	CAmericanShermanTank::RemoveAmericanShermanTank();
	CAmericanAAShermanTank::RemoveAmericanAAShermanTank();
	CJapanese97ChiHaTank::RemoveJapanese97ChiHaTank();
	CJapaneseAA97ChiHaTank::RemoveJapaneseAA97ChiHaTank();
	//Ship Weapons and Projectiles
	CExplosive7InchShell::RemoveExplosive7InchShell();
	CGun7Inch::RemoveGun7Inch();
	CGun7Inch::RemoveCamoGun7Inch();
	CShipDualMachineGun::RemoveShipDualMachineGun();
	CShipDualMachineGun::RemoveCamoShipDualMachineGun();
	CShipDual75mmGun::RemoveShipDual75mmGun();
	CShipDual75mmGun::RemoveCamoShipDual75mmGun();
	//Extra Battleship objects needed
	CExplosion14InchShellInWater::RemoveExplosion14InchShellInWater();
	CExplosion14InchShell::RemoveExplosion14InchShell();
	CExplosive14InchShell::RemoveExplosive14InchShell();
	CGun14Inch::RemoveGun14Inch();
	//Ships
	CUSSVictory::RemoveUSSVictory();
	CAmericanCarrierLexington::RemoveAmericanCarrierLexington();
	CAmericanDestroyer::RemoveAmericanDestroyer();
	CAmericanBattleshipUSSTennessee::RemoveAmericanBattleshipUSSTennessee();
	CJapaneseOilTanker::RemoveJapaneseOilTanker();
	CJapaneseTroopTransporter::RemoveJapaneseTroopTransporter();
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}

