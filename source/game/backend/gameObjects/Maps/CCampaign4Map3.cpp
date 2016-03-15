/*
 ============================================================================
 Name		: Campaign4Map3.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map3 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign4Map3.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign4Map3::CCampaign4Map3()
:	CMap(EGameMapCampaign4Mission3,CAMPAIGN4_MAP3_SIZE, CAMPAIGN4_MAP3_INTERVALS, CAMPAIGN4_MAP3_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign4Map3::~CCampaign4Map3()
	{
	}

CCampaign4Map3* CCampaign4Map3::New()
	{
	CCampaign4Map3* self = new CCampaign4Map3();
	self->Construct();
	return self;
	}

void CCampaign4Map3::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQs", CAMPAIGN4_MAP3_CAPTURE_HQ_BONUS_SCORE);
		CGetBonusPointsIfObjectsSurvive* lBonusCondition = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.AllAmericanShipsSurvived", CAMPAIGN4_MAP3_ALL_AMERICAN_SHIPS_SURVIVE_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//all the same, just differ where they are going to wait
		SGroundUnitOrder lJapaneseStormOrder1;
		lJapaneseStormOrder1.DefaultXPosition = 1400;
		lJapaneseStormOrder1.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseStormOrder1.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseStormOrder1.MoveToLocation = false;
		lJapaneseStormOrder1.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseStormOrder2;
		lJapaneseStormOrder2.DefaultXPosition = 1720;
		lJapaneseStormOrder2.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseStormOrder2.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseStormOrder2.MoveToLocation = false;
		lJapaneseStormOrder2.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseStormOrder3;
		lJapaneseStormOrder3.DefaultXPosition = -2500;
		lJapaneseStormOrder3.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseStormOrder3.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseStormOrder3.MoveToLocation = false;
		lJapaneseStormOrder3.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseStormOrder4;
		lJapaneseStormOrder4.DefaultXPosition = -3500;
		lJapaneseStormOrder4.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseStormOrder4.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseStormOrder4.MoveToLocation = false;
		lJapaneseStormOrder4.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseStormOrder5;
		lJapaneseStormOrder5.DefaultXPosition = -4050;
		lJapaneseStormOrder5.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseStormOrder5.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseStormOrder5.MoveToLocation = false;
		lJapaneseStormOrder5.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseStormOrder6;
		lJapaneseStormOrder6.DefaultXPosition = -5400;
		lJapaneseStormOrder6.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseStormOrder6.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseStormOrder6.MoveToLocation = false;
		lJapaneseStormOrder6.ShouldProtectArea = true;
		
		
		//Right ComStation
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostComStationsGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder1,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostComStationsGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder2,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostComStationsGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder3,20);
		CSpawnValIfBuildingGetsCaptured* lValRightMostComStationsGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValRightMostComStationsGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(5,150,EWeaponIdentifier100kgBomb);
		//Right Airport
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightAirportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder4,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightAirportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder5,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightAirportGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder6,20);
		CSpawnBettyIfBuildingGetsCaptured* lBettyRightAirportsGetsCaptured1 = new  CSpawnBettyIfBuildingGetsCaptured(0,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyRightAirportsGetsCaptured2 = new  CSpawnBettyIfBuildingGetsCaptured(10,-150);
		CSpawnValIfBuildingGetsCaptured* lValRightAirportsGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValRightAirportsGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(5,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValRightAirportsGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(5,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValRightAirportsGetsCaptured4 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		//Middle Comstation
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleComStationsGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder1,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleComStationsGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder2,10);
		CSpawnBettyIfBuildingGetsCaptured* lBettyMiddleComStationsGetsCaptured1 = new  CSpawnBettyIfBuildingGetsCaptured(0,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyMiddleComStationsGetsCaptured2 = new  CSpawnBettyIfBuildingGetsCaptured(10,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyMiddleComStationsGetsCaptured3 = new  CSpawnBettyIfBuildingGetsCaptured(20,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyMiddleComStationsGetsCaptured4 = new  CSpawnBettyIfBuildingGetsCaptured(30,-150);
		//Middle Airport
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleAirportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder4,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleAirportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder5,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleAirportGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder6,20);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(0,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(10,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured4 = new  CSpawnValIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValMiddleAirportsGetsCaptured5 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		//Left ComStation
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostComStationsGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder1,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostComStationsGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder2,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostComStationsGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder3,20);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostComStationsGetsCaptured4 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder4,30);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftMostComStationsGetsCaptured1 = new  CSpawnBettyIfBuildingGetsCaptured(0,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftMostComStationsGetsCaptured2 = new  CSpawnBettyIfBuildingGetsCaptured(10,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftMostComStationsGetsCaptured3 = new  CSpawnBettyIfBuildingGetsCaptured(20,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftMostComStationsGetsCaptured4 = new  CSpawnBettyIfBuildingGetsCaptured(30,-150);
		CSpawnValIfBuildingGetsCaptured* lValLeftMostComStationsGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftMostComStationsGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(0,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftMostComStationsGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(10,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftMostComStationsGetsCaptured4 = new  CSpawnValIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftMostComStationsGetsCaptured5 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		//Left Airport
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftAirportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder1,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftAirportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder2,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftAirportGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder3,20);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftAirportGetsCaptured4 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder4,30);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftAirportGetsCaptured5 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder5,40);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftAirportGetsCaptured6 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder6,50);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured1 = new  CSpawnBettyIfBuildingGetsCaptured(0,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured2 = new  CSpawnBettyIfBuildingGetsCaptured(10,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured3 = new  CSpawnBettyIfBuildingGetsCaptured(20,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured4 = new  CSpawnBettyIfBuildingGetsCaptured(30,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured5 = new  CSpawnBettyIfBuildingGetsCaptured(40,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured6 = new  CSpawnBettyIfBuildingGetsCaptured(50,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured7 = new  CSpawnBettyIfBuildingGetsCaptured(60,-150);
		CSpawnBettyIfBuildingGetsCaptured* lBettyLeftAirportGetsCaptured8 = new  CSpawnBettyIfBuildingGetsCaptured(70,-150);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured1 = new  CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured2 = new  CSpawnValIfBuildingGetsCaptured(0,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured3 = new  CSpawnValIfBuildingGetsCaptured(10,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured4 = new  CSpawnValIfBuildingGetsCaptured(20,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured5 = new  CSpawnValIfBuildingGetsCaptured(10,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured6 = new  CSpawnValIfBuildingGetsCaptured(30,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured7 = new  CSpawnValIfBuildingGetsCaptured(30,150,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured8 = new  CSpawnValIfBuildingGetsCaptured(40,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured9 = new  CSpawnValIfBuildingGetsCaptured(50,0,EWeaponIdentifier100kgBomb);
		CSpawnValIfBuildingGetsCaptured* lValLeftAirportGetsCaptured10 = new  CSpawnValIfBuildingGetsCaptured(40,150,EWeaponIdentifier100kgBomb);
		
		//American fleet
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(14000, false);
		lBonusCondition->AddGameObject(lLexington);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//Victory
		CUSSVictory* lHomeCarrier = CUSSVictory::New(12600,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		lLoseEventVictory->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		lBonusCondition->AddGameObject(lHomeCarrier);
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//create troop transporters
		SGroundUnitOrder lOrder;
		lOrder.ShouldProtectArea = false;
		lOrder.MoveToLocation = true;
		lOrder.XLocationDestination = -14400;
		//Troop Transporter 1
		CAmericanTroopTransporter* lTroopTransporter = CAmericanTroopTransporter::New(11750,false,0,EAmericanLCTTypeTankAndTroops,lOrder);
		lBonusCondition->AddGameObject(lTroopTransporter);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//Troop Transporter 2
		lTroopTransporter = CAmericanTroopTransporter::New(11000,false,-60,EAmericanLCTTypeTanksOnly,lOrder);
		lBonusCondition->AddGameObject(lTroopTransporter);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//Troop Transporter 3
		lTroopTransporter = CAmericanTroopTransporter::New(10300,false,(-60) * 2,EAmericanLCTTypeTroopsOnly,lOrder);
		lBonusCondition->AddGameObject(lTroopTransporter);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//create battleship
		CAmericanBattleshipUSSTennessee* lBattleship = CAmericanBattleshipUSSTennessee::New(9100,false);
		lBonusCondition->AddGameObject(lBattleship);
		iGameObjectManager->AddMoveableGameObject(lBattleship);
		
		//--------------------- RIGHT BEACH ---------------------//
		iGameObjectManager->AddGameObject(CTree2::New(6700, ETree2MiddleTreeSmaller)); //width
		iGameObjectManager->AddGameObject(CTree2::New(6600, ETree2MiddleTreeSmallerReflected)); //width
		iGameObjectManager->AddGameObject(CTree2::New(6500, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6460)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6400)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6340)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(6240, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(6140, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6100)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6040)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5980)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(5880, ETree2MiddleTreeSmaller)); //width
		iGameObjectManager->AddGameObject(CTree2::New(5780, ETree2MiddleTreeSmallerReflected)); //width
		iGameObjectManager->AddGameObject(CTree2::New(5680, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5600)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5500)); //width 50
		CJapaneseSquad::CreateJapaneseTankSquad(4900,true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(4800)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(4300,true); //width 430
		iGameObjectManager->AddGameObject(CTree1::New(4300,EBigThinTreeNormal)); // width 32
		CJapaneseSquad::CreateJapaneseSoldierSquad(3900,true); //width 400
		//--------------------- COM STATIONS ---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3800)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3700)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3600)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3500)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3400)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3300)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3200)); //width 80
		iGameObjectManager->AddGameObject(CForest3::New(3200)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(2750,true); //width 430
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2680)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2600)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2540)); //width 32
		CComStation* lComStation = CComStation::New(2440,EConflictSideJapanese,0,0);
		lRightMostComStationsGetsCaptured1->AddBuilding(lComStation);
		lRightMostComStationsGetsCaptured2->AddBuilding(lComStation);
		lRightMostComStationsGetsCaptured3->AddBuilding(lComStation);
		lValRightMostComStationsGetsCaptured1->AddBuilding(lComStation);
		lValRightMostComStationsGetsCaptured2->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2300)); //width 80
		lComStation = CComStation::New(2215,EConflictSideJapanese,10,100);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2100)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2050)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2000)); //width 32
		//--------------------- AIRPORT ---------------------//
		CJapaneseSquad::CreateJapaneseTankSquad(1400,true); //width 560
		CAirport* lAirport = CAirport::New(300,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lRightAirportGetsCaptured1->AddBuilding(lAirport);
		lRightAirportGetsCaptured2->AddBuilding(lAirport);
		lRightAirportGetsCaptured3->AddBuilding(lAirport);
		lBettyRightAirportsGetsCaptured1->AddBuilding(lAirport);
		lBettyRightAirportsGetsCaptured2->AddBuilding(lAirport);
		lValRightAirportsGetsCaptured1->AddBuilding(lAirport);
		lValRightAirportsGetsCaptured2->AddBuilding(lAirport);
		lValRightAirportsGetsCaptured3->AddBuilding(lAirport);
		lValRightAirportsGetsCaptured4->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport); 
		lAirport = CAirport::New(-1000,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport);
		CJapaneseSquad::CreateJapaneseTankSquad(-1720,true); //width 560
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1800)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1880)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2000)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2080)); //width 50
		iGameObjectManager->AddGameObject(CForest3::New(-2400)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierSquad(-2500,true); //width 400
		//--------------------- COM STATION ---------------------//
		iGameObjectManager->AddGameObject(CTree1::New(-2550,EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2600)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2680)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2780)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2880)); //width 80
		lComStation = CComStation::New(-2980,EConflictSideJapanese,-20,200);
		lMiddleComStationsGetsCaptured1->AddBuilding(lComStation);
		lMiddleComStationsGetsCaptured2->AddBuilding(lComStation);
		lBettyMiddleComStationsGetsCaptured1->AddBuilding(lComStation);
		lBettyMiddleComStationsGetsCaptured2->AddBuilding(lComStation);
		lBettyMiddleComStationsGetsCaptured3->AddBuilding(lComStation);
		lBettyMiddleComStationsGetsCaptured4->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-3500,true); //width 430
		//--------------------- Forest Area ---------------------//
		iGameObjectManager->AddGameObject(CBigTree::New(-3630,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-4050,true); //width 430
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4200)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4300)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4380)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-4480)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4550)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4650)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4750)); //width 80
		iGameObjectManager->AddGameObject(CForest3::New(-4800)); //Width width 700
		//--------------------- Airport ---------------------//
		CJapaneseSquad::CreateJapaneseTankSquad(-5400,true); //width 560
		lAirport = CAirport::New(-6500,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lMiddleAirportGetsCaptured1->AddBuilding(lAirport);
		lMiddleAirportGetsCaptured2->AddBuilding(lAirport);
		lMiddleAirportGetsCaptured3->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured1->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured2->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured3->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured4->AddBuilding(lAirport);
		lValMiddleAirportsGetsCaptured5->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		CJapaneseSquad::CreateJapaneseSoldierSquad(-7100,true); //width 400
		iGameObjectManager->AddGameObject(CBigTree::New(-7200,EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CSmallTree::New(-7500,ESmallTreeReflected)); //width 64
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-7600,true); //width 430
		//--------------------- Forest Area ---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-7700)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-7800)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-7900)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8000)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8100)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-8200)); //width 80
		iGameObjectManager->AddGameObject(CForest3::New(-8300)); //Width width 700
		CJapaneseSquad::CreateJapaneseTankSquad(-8900,true); //width 560
		//--------------------- COM STATIONS ---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8960)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-9030)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-9130)); //width 50
		iGameObjectManager->AddGameObject(CTree1::New(-9180,EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-9280)); //width 80
		lComStation = CComStation::New(-9380,EConflictSideJapanese,-20,300);
		lLeftMostComStationsGetsCaptured1->AddBuilding(lComStation);
		lLeftMostComStationsGetsCaptured2->AddBuilding(lComStation);
		lLeftMostComStationsGetsCaptured3->AddBuilding(lComStation);
		lLeftMostComStationsGetsCaptured4->AddBuilding(lComStation);
		lBettyLeftMostComStationsGetsCaptured1->AddBuilding(lComStation);
		lBettyLeftMostComStationsGetsCaptured2->AddBuilding(lComStation);
		lBettyLeftMostComStationsGetsCaptured3->AddBuilding(lComStation);
		lBettyLeftMostComStationsGetsCaptured4->AddBuilding(lComStation);
		lValLeftMostComStationsGetsCaptured1->AddBuilding(lComStation);
		lValLeftMostComStationsGetsCaptured2->AddBuilding(lComStation);
		lValLeftMostComStationsGetsCaptured3->AddBuilding(lComStation);
		lValLeftMostComStationsGetsCaptured4->AddBuilding(lComStation);
		lValLeftMostComStationsGetsCaptured5->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		lComStation = CComStation::New(-9540,EConflictSideJapanese,-30,400);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		CJapaneseSquad::CreateJapaneseTankSquad(-10240,true); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(-10380,EBigTreeReflected)); //width 128
		//--------------------- AIRPORTS ---------------------//
		CJapaneseSquad::CreateJapaneseTankSquad(-10980,true); //width 560
		lAirport = CAirport::New(-12150,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lLeftAirportGetsCaptured1->AddBuilding(lAirport);
		lLeftAirportGetsCaptured2->AddBuilding(lAirport);
		lLeftAirportGetsCaptured3->AddBuilding(lAirport);
		lLeftAirportGetsCaptured4->AddBuilding(lAirport);
		lLeftAirportGetsCaptured5->AddBuilding(lAirport);
		lLeftAirportGetsCaptured6->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured1->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured2->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured3->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured4->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured5->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured6->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured7->AddBuilding(lAirport);
		lBettyLeftAirportGetsCaptured8->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured1->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured2->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured3->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured4->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured5->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured6->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured7->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured8->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured8->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured9->AddBuilding(lAirport);
		lValLeftAirportGetsCaptured10->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		lAirport = CAirport::New(-13400,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport);
		//--------------------- HQ ---------------------//
		CHq* lHq = CHq::New(-14400,EConflictSideJapanese);
		lWinCondition->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670

		iGameObjectManager->AddGameObject(CFloorWater::New(6800, 9450));   //right  side 6000 - 14550
		iGameObjectManager->AddGameObject(CFloorLand::New(-16250, 23050));
		
		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseTankSquadOrder;
		lJapaneseTankSquadOrder.DefaultXPosition = 4900;
		lJapaneseTankSquadOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseTankSquadOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseTankSquadOrder.MoveToLocation = false;
		lJapaneseTankSquadOrder.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder1;
		lJapaneseSoldierAndTankSqaudOrder1.DefaultXPosition = 4300;
		lJapaneseSoldierAndTankSqaudOrder1.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseSoldierAndTankSqaudOrder1.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseSoldierAndTankSqaudOrder1.MoveToLocation = false;	
		lJapaneseSoldierAndTankSqaudOrder1.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder2;
		lJapaneseSoldierAndTankSqaudOrder2.DefaultXPosition = 3900;
		lJapaneseSoldierAndTankSqaudOrder2.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseSoldierAndTankSqaudOrder2.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseSoldierAndTankSqaudOrder2.MoveToLocation = false;
		lJapaneseSoldierAndTankSqaudOrder2.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseSoldieSqaudOrder;
		lJapaneseSoldieSqaudOrder.DefaultXPosition = 2750;
		lJapaneseSoldieSqaudOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-16000);
		lJapaneseSoldieSqaudOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6700);
		lJapaneseSoldieSqaudOrder.MoveToLocation = false;	
		lJapaneseSoldieSqaudOrder.ShouldProtectArea = true;

		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusCondition);
		iEvents->Append(lRightMostComStationsGetsCaptured1);
		iEvents->Append(lRightMostComStationsGetsCaptured2);
		iEvents->Append(lRightMostComStationsGetsCaptured3);
		iEvents->Append(lValRightMostComStationsGetsCaptured1);
		iEvents->Append(lValRightMostComStationsGetsCaptured2);
		iEvents->Append(lRightAirportGetsCaptured1);
		iEvents->Append(lRightAirportGetsCaptured2);
		iEvents->Append(lRightAirportGetsCaptured3);
		iEvents->Append(lBettyRightAirportsGetsCaptured1);
		iEvents->Append(lBettyRightAirportsGetsCaptured2);
		iEvents->Append(lValRightAirportsGetsCaptured1);
		iEvents->Append(lValRightAirportsGetsCaptured2);
		iEvents->Append(lValRightAirportsGetsCaptured3);
		iEvents->Append(lValRightAirportsGetsCaptured4);
		iEvents->Append(lMiddleComStationsGetsCaptured1);
		iEvents->Append(lMiddleComStationsGetsCaptured2);
		iEvents->Append(lBettyMiddleComStationsGetsCaptured1);
		iEvents->Append(lBettyMiddleComStationsGetsCaptured2);
		iEvents->Append(lBettyMiddleComStationsGetsCaptured3);
		iEvents->Append(lBettyMiddleComStationsGetsCaptured4);
		iEvents->Append(lMiddleAirportGetsCaptured1);
		iEvents->Append(lMiddleAirportGetsCaptured2);
		iEvents->Append(lMiddleAirportGetsCaptured3);
		iEvents->Append(lValMiddleAirportsGetsCaptured1);
		iEvents->Append(lValMiddleAirportsGetsCaptured2);
		iEvents->Append(lValMiddleAirportsGetsCaptured3);
		iEvents->Append(lValMiddleAirportsGetsCaptured4);
		iEvents->Append(lValMiddleAirportsGetsCaptured5);
		iEvents->Append(lLeftMostComStationsGetsCaptured1);
		iEvents->Append(lLeftMostComStationsGetsCaptured2);
		iEvents->Append(lLeftMostComStationsGetsCaptured3);
		iEvents->Append(lLeftMostComStationsGetsCaptured4);
		iEvents->Append(lBettyLeftMostComStationsGetsCaptured1);
		iEvents->Append(lBettyLeftMostComStationsGetsCaptured2);
		iEvents->Append(lBettyLeftMostComStationsGetsCaptured3);
		iEvents->Append(lBettyLeftMostComStationsGetsCaptured4);
		iEvents->Append(lValLeftMostComStationsGetsCaptured1);
		iEvents->Append(lValLeftMostComStationsGetsCaptured2);
		iEvents->Append(lValLeftMostComStationsGetsCaptured3);
		iEvents->Append(lValLeftMostComStationsGetsCaptured4);
		iEvents->Append(lValLeftMostComStationsGetsCaptured5);
		iEvents->Append(lLeftAirportGetsCaptured1);
		iEvents->Append(lLeftAirportGetsCaptured2);
		iEvents->Append(lLeftAirportGetsCaptured3);
		iEvents->Append(lLeftAirportGetsCaptured4);
		iEvents->Append(lLeftAirportGetsCaptured5);
		iEvents->Append(lLeftAirportGetsCaptured6);
		iEvents->Append(lBettyLeftAirportGetsCaptured1);
		iEvents->Append(lBettyLeftAirportGetsCaptured2);
		iEvents->Append(lBettyLeftAirportGetsCaptured3);
		iEvents->Append(lBettyLeftAirportGetsCaptured4);
		iEvents->Append(lBettyLeftAirportGetsCaptured5);
		iEvents->Append(lBettyLeftAirportGetsCaptured6);
		iEvents->Append(lBettyLeftAirportGetsCaptured7);
		iEvents->Append(lBettyLeftAirportGetsCaptured8);
		iEvents->Append(lValLeftAirportGetsCaptured1);
		iEvents->Append(lValLeftAirportGetsCaptured2);
		iEvents->Append(lValLeftAirportGetsCaptured3);
		iEvents->Append(lValLeftAirportGetsCaptured4);
		iEvents->Append(lValLeftAirportGetsCaptured5);
		iEvents->Append(lValLeftAirportGetsCaptured6);
		iEvents->Append(lValLeftAirportGetsCaptured7);
		iEvents->Append(lValLeftAirportGetsCaptured8);
		iEvents->Append(lValLeftAirportGetsCaptured9);
		iEvents->Append(lValLeftAirportGetsCaptured10);
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeTankSquad,lJapaneseTankSquadOrder,60*(6),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankSqaudOrder1,60*(6),-60*(1),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeTankSquad,lJapaneseSoldierAndTankSqaudOrder2,60*(6),-60*(2),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierSquad,lJapaneseSoldieSqaudOrder,60*(6),-60*(3),TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign4Map3::InitializeGameObjectSettingsForMap()
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
	CAmericanCarrierLexington::InitilizeAmericanCarrierLexington();
	CAmericanLCT::InitilizeAmericanLCT();
	CAmericanBattleshipUSSTennessee::InitilizeAmericanBattleshipUSSTennessee();
	CAmericanTroopTransporter::InitilizeAmericanTroopTransporter();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign4Map3::RemoveGameObjectSettingsForMap()
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
	CAmericanLCT::RemoveAmericanLCT();
	CAmericanBattleshipUSSTennessee::RemoveAmericanBattleshipUSSTennessee();
	CAmericanTroopTransporter::RemoveAmericanTroopTransporter();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
