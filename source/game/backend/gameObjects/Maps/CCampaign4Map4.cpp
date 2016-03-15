/*
 ============================================================================
 Name		: Campaign4Map4.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map4 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign4Map4.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign4Map4::CCampaign4Map4()
:	CMap(EGameMapCampaign4Mission4,CAMPAIGN4_MAP4_SIZE, CAMPAIGN4_MAP4_INTERVALS, CAMPAIGN4_MAP4_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign4Map4::~CCampaign4Map4()
	{
	}

CCampaign4Map4* CCampaign4Map4::New()
	{
	CCampaign4Map4* self = new  CCampaign4Map4();
	self->Construct();
	return self;
	}

void CCampaign4Map4::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization,
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreCaptured* lLoseEventAirportGotCaptured = new  CLoseIfObjectsAreCaptured();
		CWinIfObjectsAreDestroyedEvent* lWinCondition = new  CWinIfObjectsAreDestroyedEvent("Frontend.BonusEvents.DestroyedJapaneseFleet",CAMPAIGN4_MAP4_DESTROY_JAPANESE_FLEET_BONUS_SCORE);
		CGetBonusPointsIfObjectsSurvive* lBonusCondition = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.CaptainBrotherSurvived", CAMPAIGN4_MAP4_CAPTAIN_BROTHER_SURVIVED_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		CAirport* lAirport;
		CComStation* lComStation;
		
		//Captue Spawn Events
		//Middle Airport
		CSpawnBettyIfBuildingGetsCaptured* lBettyMiddleAirportsGetsCaptured1 = new  CSpawnBettyIfBuildingGetsCaptured(0,300);
		CSpawnBettyIfBuildingGetsCaptured* lBettyMiddleAirportsGetsCaptured2 = new  CSpawnBettyIfBuildingGetsCaptured(10,300);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		//Japanese ComStation
		CSpawnBettyIfBuildingGetsCaptured* lBettyComStationGetsCaptured1 = new  CSpawnBettyIfBuildingGetsCaptured(0,300);
		CSpawnBettyIfBuildingGetsCaptured* lBettyComStationGetsCaptured2 = new  CSpawnBettyIfBuildingGetsCaptured(10,300);
		CSpawnValIfBuildingGetsCaptured* lValComStationGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValComStationGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValComStationGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValComStationGetsCaptured4 = new  CSpawnValIfBuildingGetsCaptured(30,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValComStationGetsCaptured5 = new  CSpawnValIfBuildingGetsCaptured(40,0,EWeaponIdentifier100kgBomb);
		//Left Airport
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportsGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportsGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportsGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportsGetsCaptured4 = new  CSpawnValIfBuildingGetsCaptured(30,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportsGetsCaptured5 = new  CSpawnValIfBuildingGetsCaptured(40,0,EWeaponIdentifier100kgBomb);
		//Left Most Airport, help the Americans to sink the fleet
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured1 = new  CSpawnDauntlessIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured2 = new  CSpawnDauntlessIfBuildingGetsCaptured(10,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured3 = new  CSpawnDauntlessIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured4 = new  CSpawnDauntlessIfBuildingGetsCaptured(30,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured5 = new  CSpawnDauntlessIfBuildingGetsCaptured(40,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured6 = new  CSpawnDauntlessIfBuildingGetsCaptured(50,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured7 = new  CSpawnDauntlessIfBuildingGetsCaptured(60,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured8 = new  CSpawnDauntlessIfBuildingGetsCaptured(70,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured9 = new  CSpawnDauntlessIfBuildingGetsCaptured(80,0,EWeaponIdentifier100kgBomb);
		CSpawnDauntlessIfBuildingGetsCaptured* lDauntlessLeftMostAirportGetsCaptured10 = new  CSpawnDauntlessIfBuildingGetsCaptured(90,0,EWeaponIdentifier100kgBomb);
		
		
		//--------------------- Home Airport ---------------------//
		CAirport* lHomeAirport;
		lHomeAirport = CAirport::New(14150, EConflictSideAmerican); //widht to the left 175, witdh to the right 1050
		lLoseEventAirportGotCaptured->AddBuilding(lHomeAirport);
		iGameObjectManager->AddGameObject(lHomeAirport);
		lAirport = CAirport::New(12850, EConflictSideAmerican); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport);
		lBonusCondition->AddGameObject(CAmericanSquad::CreateAmericanSoldierAndTankSquad(12500,false)); //width 244
		iGameObjectManager->AddGameObject(CSmallTree::New(12150, ESmallTreeNormal)); //width ??
		CAmericanSquad::CreateAmericanTankSquad(12150, false); //width 560
		//--------------------- ComStation ----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(11540, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(11470, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(11350, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(11250,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(11100, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(11020, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(10940, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(10930)); //Width width 700
		lComStation = CComStation::New(10860, EConflictSideAmerican, 0, 200);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(10710, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(10600, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(10500,EConflictSideAmerican)); //width 50
		CAmericanSquad::CreateAmericanSoldierSquad(10500, false); //width 400
		iGameObjectManager->AddGameObject(CBigTree::New(10300, EBigTreeReflected)); //width 128
		//--------------------- Forest Area ----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(9950, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(9850, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(9750, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(9650, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(9550, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(9350)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(9300,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(9200,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(9100,EConflictSideAmerican)); //width 50
		CAmericanSquad::CreateAmericanTankSquad(9000, false); //width 560
		//--------------------- ComStation ----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(8400, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(8330, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(8280, EBigThinTreeNormal)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(8180, EConflictSideAmerican)); //width 80
		lComStation = CComStation::New(8100, EConflictSideAmerican, 10, 100);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(8000, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(7930, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(7830, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(7730, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(7630,EConflictSideAmerican)); //width 50
		CAmericanSquad::CreateAmericanTankSquad(7400, false); //width 560
		CAmericanSquad::CreateAmericanTankSquad(7600, false)->MoveGroundUnitsToLocation(-4400); //width 560
		CAmericanSquad::CreateAmericanTankSquad(7000, false)->MoveGroundUnitsToLocation(-4200); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(7700, EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CBigTree::New(7500, EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CForest3::New(6800)); //Width width 700
		//--------------------- JAPANESE SIDE STARTS HERE ----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(6700)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(6600)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(6500)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6400)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6340)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6280)); //width 32
		CJapaneseSquad::CreateJapaneseSoldierSquad(5850, true); //width 400
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(5400, true); //width 430
		iGameObjectManager->AddGameObject(CForest3::New(5300)); //Width width 700
		//------------------------- Airport ---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5300)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5240)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(5140)); //width 80
		CJapaneseSquad::CreateJapaneseTankSquad(4500, true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(4500)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierSquad(4100, true); //width 400
		lAirport = CAirport::New(3000, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lBettyMiddleAirportsGetsCaptured1->AddBuilding(lAirport);
		lBettyMiddleAirportsGetsCaptured2->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured1->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured2->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured3->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2700)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2600)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2500)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2400)); //width 80
		iGameObjectManager->AddGameObject(CTree1::New(2330, EBigThinTreeReflected)); // width 32
		//----------------------- Com Station  AND Airport ----------------//
		CJapaneseSquad::CreateJapaneseSoldierSquad(1900, true); //width 400
		CJapaneseSquad::CreateJapaneseTankSquad(1200, true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(1100)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1000)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(930)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(830)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(730)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(630)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(570)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(470)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(340)); //width 80
		iGameObjectManager->AddGameObject(CForest3::New(300)); //Width width 700
		lComStation = CComStation::New(200, EConflictSideJapanese, 20, 0);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		lComStation = CComStation::New(50, EConflictSideJapanese, 30, -100);
		lBettyComStationGetsCaptured1->AddBuilding(lComStation);
		lBettyComStationGetsCaptured2->AddBuilding(lComStation);
		lValComStationGetsCaptured1->AddBuilding(lComStation);
		lValComStationGetsCaptured2->AddBuilding(lComStation);
		lValComStationGetsCaptured3->AddBuilding(lComStation);
		lValComStationGetsCaptured4->AddBuilding(lComStation);
		lValComStationGetsCaptured5->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-50)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-130)); //width 32
		iGameObjectManager->AddGameObject(CBigTree::New(-400, EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(-560, true); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-600)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-700)); //width 50
		CJapaneseSquad::CreateJapaneseTankSquad(-1300, true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(-1300)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierSquad(-1700, true); //width 400
		lAirport = CAirport::New(-2800, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lValLeftAirportsGetsCaptured1->AddBuilding(lAirport);
		lValLeftAirportsGetsCaptured2->AddBuilding(lAirport);
		lValLeftAirportsGetsCaptured3->AddBuilding(lAirport);
		lValLeftAirportsGetsCaptured4->AddBuilding(lAirport);
		lValLeftAirportsGetsCaptured5->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		lAirport = CAirport::New(-4100, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lDauntlessLeftMostAirportGetsCaptured1->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured2->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured3->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured4->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured5->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured6->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured7->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured8->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured9->AddBuilding(lAirport);
		lDauntlessLeftMostAirportGetsCaptured10->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(CTree2::New(-4500, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-4600, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-4750, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-4850, ETree2MiddleTreeReflected)); //width 64
		CJapaneseSquad::CreateJapaneseTankSquad(-4910, true); //width 560
		iGameObjectManager->AddGameObject(CTree2::New(-4950, ETree2MiddleTreeSmallerReflected)); //width 64

		//----------------- LEFT WATER ------------------------//	
		//------ American forces ----------//
		//Victory
		CUSSVictory* lHomeCarrier = CUSSVictory::New(-6500,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		iPlayer->AssignPlayerBase(lHomeAirport); //next time spawn at the your new home base, bye bye Victory!
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//American Destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(-7300,false));
		
		//-----Japanese forces -----------//
		//Destroyer
		CJapaneseDestroyer* lJapaneseDestroyer;
		lJapaneseDestroyer = CJapaneseDestroyer::New(-8500,true);
		lJapaneseDestroyer->MoveToLocation(TIntFloat::Convert(-7300));
		lWinCondition->AddGameObject(lJapaneseDestroyer);
		iGameObjectManager->AddMoveableGameObject(lJapaneseDestroyer);
		lJapaneseDestroyer = CJapaneseDestroyer::New(-9300,true);
		lJapaneseDestroyer->MoveToLocation(TIntFloat::Convert(-8100));
		lWinCondition->AddGameObject(lJapaneseDestroyer);
		iGameObjectManager->AddMoveableGameObject(lJapaneseDestroyer);
		//Landing Ships
		//create troop transporters
		//order
		SGroundUnitOrder lJapaneseOrder;
		lJapaneseOrder.ShouldProtectArea = false;
		lJapaneseOrder.MoveToLocation = true;
		lJapaneseOrder.XLocationDestination = 14500;
		//actual ships
		CJapaneseTroopTransporter* lJapaneseTransporter;
		lJapaneseTransporter = CJapaneseTroopTransporter::New(-10100,true,60,EJapaneseLCTTypeTanksOnly,lJapaneseOrder);
		lJapaneseTransporter->MoveToLocation(TIntFloat::Convert(-8900));
		lWinCondition->AddGameObject(lJapaneseTransporter);
		iGameObjectManager->AddMoveableGameObject(lJapaneseTransporter);
		lJapaneseTransporter = CJapaneseTroopTransporter::New(-10900,true,0,EJapaneseLCTTypeTankAndTroops,lJapaneseOrder);
		lJapaneseTransporter->MoveToLocation(TIntFloat::Convert(-9700));
		lWinCondition->AddGameObject(lJapaneseTransporter);
		iGameObjectManager->AddMoveableGameObject(lJapaneseTransporter);
		lJapaneseTransporter = CJapaneseTroopTransporter::New(-11700,true,-60,EJapaneseLCTTypeTanksOnly,lJapaneseOrder);
		lJapaneseTransporter->MoveToLocation(TIntFloat::Convert(-10500));
		lWinCondition->AddGameObject(lJapaneseTransporter);
		iGameObjectManager->AddMoveableGameObject(lJapaneseTransporter);
		//Japanese Battleship Yamato
		CJapaneseBattleshipYamato* lYamato = CJapaneseBattleshipYamato::New(-13000,true);
		lYamato->MoveToLocation(TIntFloat::Convert(-11300));
		lWinCondition->AddGameObject(lYamato);
		iGameObjectManager->AddMoveableGameObject(lYamato);
		//Destroyer
		lJapaneseDestroyer = CJapaneseDestroyer::New(-13800,true);
		lJapaneseDestroyer->MoveToLocation(TIntFloat::Convert(-12600));
		lWinCondition->AddGameObject(lJapaneseDestroyer);
		iGameObjectManager->AddMoveableGameObject(lJapaneseDestroyer);
		//Destroyer
		lJapaneseDestroyer = CJapaneseDestroyer::New(-14600,true);
		lJapaneseDestroyer->MoveToLocation(TIntFloat::Convert(-13400));
		lWinCondition->AddGameObject(lJapaneseDestroyer);
		iGameObjectManager->AddMoveableGameObject(lJapaneseDestroyer);
		
		//Add in some bombers to destroy the American fleet
		//Wave 1
		iGameObjectManager->AddMoveableGameObject(CBetty::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-14600,1700)));
		iGameObjectManager->AddMoveableGameObject(CBetty::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-14800,1600)));
		iGameObjectManager->AddMoveableGameObject(CBetty::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-15000,1500)));

		//Add FLOOR
		iGameObjectManager->AddGameObject(CFloorLand::New(-5000, 21250));
		iGameObjectManager->AddGameObject(CFloorWater::New(-16250, 11250));
		

		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventAirportGotCaptured);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusCondition);
		//Add Betties that destroy the American ships
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(30,10,100,true, 1));
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(30,0,0,true, 1));
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(30,10,-100,true, 1));
		//some initial ones as well
		iEvents->Append(new  CSpawnValEveryTimeInterval(10,0,800,true, 1,  GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnValEveryTimeInterval(15,0,950,true, 1,  GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnValEveryTimeInterval(20,0,1050,true, 1,GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		//add Some Vals as well
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*5,0,800,true, 5,  GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*5,5,950,true, 5,  GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*5,10,1050,true, 5,GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		//American Squads
		SGroundUnitOrder lAmericanOrder1;
		lAmericanOrder1.ShouldProtectArea = false;
		lAmericanOrder1.MoveToLocation = true;
		lAmericanOrder1.XLocationDestination = -4100;
		SGroundUnitOrder lAmericanOrder2;
		lAmericanOrder2.ShouldProtectArea = false;
		lAmericanOrder2.MoveToLocation = true;
		lAmericanOrder2.XLocationDestination = -3800;
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder1, 60 * (4), 0, TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeSoldierAndTankSquad, lAmericanOrder2, 60 * (4), -60 * (1), TIME_EVENT_INFINITE_TIME_EVENTS));
		//Create Reinforcement event after 20 mins, this should help the player to win the battle
		iEvents->Append(new  CClockTimerEvent(60 * 20));
		//spawns dauntless
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,0,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,10,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,20,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,30,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,40,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,50,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,60,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,70,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,80,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60 * 20,90,0,1, GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS));
		//add extra American reinforcement as well
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder1, 60 * (20), 0, 1));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder2, 60 * (20), 15, 1));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder1, 60 * (20), 30, 1));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder2, 60 * (20), 45, 1));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder1, 60 * (20), 60, 1));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanOrder2, 60 * (20), 75, 1));
		//Capture Events
		iEvents->Append(lBettyMiddleAirportsGetsCaptured1);
		iEvents->Append(lBettyMiddleAirportsGetsCaptured2);
		iEvents->Append(lValMiddleAirportsGetsCaptured1);
		iEvents->Append(lValMiddleAirportsGetsCaptured2);
		iEvents->Append(lValMiddleAirportsGetsCaptured3);
		iEvents->Append(lBettyComStationGetsCaptured1);
		iEvents->Append(lBettyComStationGetsCaptured2);
		iEvents->Append(lValComStationGetsCaptured1);
		iEvents->Append(lValComStationGetsCaptured2);
		iEvents->Append(lValComStationGetsCaptured3);
		iEvents->Append(lValComStationGetsCaptured4);
		iEvents->Append(lValComStationGetsCaptured5);
		iEvents->Append(lValLeftAirportsGetsCaptured1);
		iEvents->Append(lValLeftAirportsGetsCaptured2);
		iEvents->Append(lValLeftAirportsGetsCaptured3);
		iEvents->Append(lValLeftAirportsGetsCaptured4);
		iEvents->Append(lValLeftAirportsGetsCaptured5);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured1);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured2);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured3);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured4);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured5);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured6);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured7);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured8);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured9);
		iEvents->Append(lDauntlessLeftMostAirportGetsCaptured10);
	}
}

void CCampaign4Map4::InitializeGameObjectSettingsForMap()
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
	CAmericanRifleman::InitilizeAmericanRifleman();
	CAmericanRocketman::InitilizeAmericanRocketman();
	CAmericanShermanTank::InitilizeAmericanShermanTank();
	CAmericanAAShermanTank::InitilizeAmericanAAShermanTank();
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
	CAmericanDestroyer::InitilizeAmericanDestroyer();
	CJapaneseBattleshipYamato::InitilizeJapaneseBattleshipYamato();
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	CJapaneseTroopTransporter::InitilizeJapaneseTroopTransporter();
	CJapaneseLCT::InitilizeJapaneseLCT();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign4Map4::RemoveGameObjectSettingsForMap()
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
	//Ships,
	CUSSVictory::RemoveUSSVictory();
	CAmericanDestroyer::RemoveAmericanDestroyer();
	CJapaneseBattleshipYamato::RemoveJapaneseBattleshipYamato();
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	CJapaneseTroopTransporter::RemoveJapaneseTroopTransporter();
	CJapaneseLCT::RemoveJapaneseLCT();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}

