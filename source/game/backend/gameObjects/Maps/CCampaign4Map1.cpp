/*
 ============================================================================
 Name		: Campaign4Map1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map1 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign4Map1.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign4Map1::CCampaign4Map1()
:	CMap(EGameMapCampaign4Mission1,CAMPAIGN4_MAP1_SIZE, CAMPAIGN4_MAP1_INTERVALS, CAMPAIGN4_MAP1_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign4Map1::~CCampaign4Map1()
	{
	}

CCampaign4Map1* CCampaign4Map1::New()
	{
	CCampaign4Map1* self = new CCampaign4Map1();
	self->Construct();
	return self;
	}

void CCampaign4Map1::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQs", CAMPAIGN4_MAP1_CAPTURE_HQ_BONUS_SCORE);
		CGetBonusPointsIfObjectsSurvive* lBonusCondition = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.AllAmericanShipsSurvived", CAMPAIGN4_MAP1_ALL_AMERICAN_SHIPS_SURVIVE_BONUS_SCORE);
		
		SGroundUnitOrder lComStationStormOrder;
		lComStationStormOrder.DefaultXPosition = 2130;
		lComStationStormOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lComStationStormOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lComStationStormOrder.MoveToLocation = false;
		lComStationStormOrder.ShouldProtectArea = true;
		SGroundUnitOrder lRightAirportStormOrder;
		lRightAirportStormOrder.DefaultXPosition = 1650;
		lRightAirportStormOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lRightAirportStormOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lRightAirportStormOrder.MoveToLocation = false;
		lRightAirportStormOrder.ShouldProtectArea = true;
		SGroundUnitOrder lRightHQStormOrder;
		lRightHQStormOrder.DefaultXPosition = -1000;
		lRightHQStormOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lRightHQStormOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lRightHQStormOrder.MoveToLocation = false;
		lRightHQStormOrder.ShouldProtectArea = true;
		SGroundUnitOrder lComStationMiddleRightStormOrder;
		lComStationMiddleRightStormOrder.DefaultXPosition = -5000;
		lComStationMiddleRightStormOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lComStationMiddleRightStormOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lComStationMiddleRightStormOrder.MoveToLocation = false;
		lComStationMiddleRightStormOrder.ShouldProtectArea = true;
		SGroundUnitOrder lComStationMiddleLeftStormOrder;
		lComStationMiddleLeftStormOrder.DefaultXPosition = -6600;
		lComStationMiddleLeftStormOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lComStationMiddleLeftStormOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lComStationMiddleLeftStormOrder.MoveToLocation = false;
		lComStationMiddleLeftStormOrder.ShouldProtectArea = true;
		SGroundUnitOrder lAirportLeftStormOrder1;
		lAirportLeftStormOrder1.DefaultXPosition = -7100;
		lAirportLeftStormOrder1.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lAirportLeftStormOrder1.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lAirportLeftStormOrder1.MoveToLocation = false;
		lAirportLeftStormOrder1.ShouldProtectArea = true;
		SGroundUnitOrder lAirportLeftStormOrder2;
		lAirportLeftStormOrder2.DefaultXPosition = -11600;
		lAirportLeftStormOrder2.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lAirportLeftStormOrder2.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lAirportLeftStormOrder2.MoveToLocation = false;
		lAirportLeftStormOrder2.ShouldProtectArea = true;
		SGroundUnitOrder lAirportLeftStormOrder3;
		lAirportLeftStormOrder3.DefaultXPosition = -12300;
		lAirportLeftStormOrder3.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lAirportLeftStormOrder3.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lAirportLeftStormOrder3.MoveToLocation = false;
		lAirportLeftStormOrder3.ShouldProtectArea = true;
		SGroundUnitOrder lLeftMostComStationStormOrder;
		lLeftMostComStationStormOrder.DefaultXPosition = -12600;
		lLeftMostComStationStormOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lLeftMostComStationStormOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lLeftMostComStationStormOrder.MoveToLocation = false;
		lLeftMostComStationStormOrder.ShouldProtectArea = true;
		
		//Extra Spawn Events if Buildings get Captured
		//Right ComStation
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostComStationGetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lComStationStormOrder,0);
		//Right Airport
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostAirportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lRightAirportStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostAirportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lRightAirportStormOrder,10);
		//Right HQ
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostHQGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lRightAirportStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostHQGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAATankSquad,lRightAirportStormOrder,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lRightMostHQGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lRightAirportStormOrder,20);
		//Middle Comstations
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleRightComStationGetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lComStationMiddleRightStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleLeftComStationGetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lComStationMiddleLeftStormOrder,10);
		//Left Airports
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostAirportRightGetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lAirportLeftStormOrder1,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostAirportMiddleGetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAATankSquad,lAirportLeftStormOrder2,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostAirportLeftGetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAATankSquad,lAirportLeftStormOrder3,20);
		//Left Most ComStation
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostComStationGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lComStationMiddleLeftStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLeftMostComStationGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lComStationMiddleLeftStormOrder,5);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//American fleet
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(12700, false);
		lBonusCondition->AddGameObject(lLexington);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//Victory
		CUSSVictory* lHomeCarrier = CUSSVictory::New(11300,false);
		lBonusCondition->AddGameObject(lHomeCarrier);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		lLoseEventVictory->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//create troop transporters
		SGroundUnitOrder lOrder;
		lOrder.ShouldProtectArea = false;
		lOrder.MoveToLocation = true;
		lOrder.XLocationDestination = -13500;
		//Troop Transporter 1
		CAmericanTroopTransporter* lTroopTransporter = CAmericanTroopTransporter::New(10450,false,0,EAmericanLCTTypeTankAndTroops,lOrder);
		lBonusCondition->AddGameObject(lTroopTransporter);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//Troop Transporter 2
		lTroopTransporter = CAmericanTroopTransporter::New(9700,false,-60,EAmericanLCTTypeTanksOnly,lOrder);
		lBonusCondition->AddGameObject(lTroopTransporter);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//create battleship
		CAmericanBattleshipUSSTennessee* lBattleship = CAmericanBattleshipUSSTennessee::New(8500,false);
		lBonusCondition->AddGameObject(lBattleship);
		iGameObjectManager->AddMoveableGameObject(lBattleship);
		
		//--------------------- RIGHT BEACH ---------------------//
		iGameObjectManager->AddGameObject(CTree2::New(6400, ETree2MiddleTree)); //width
		iGameObjectManager->AddGameObject(CTree2::New(6300, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(6200, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(6100, ETree2MiddleTreeSmaller)); //width
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(6000)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(5850)); //width 80
		iGameObjectManager->AddGameObject(CTree2::New(5750, ETree2MiddleTree)); //width
		iGameObjectManager->AddGameObject(CTree2::New(5650, ETree2MiddleTreeSmaller)); //width
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5600)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5530)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(5430, ETree2MiddleTreeSmallerReflected)); //width
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5370)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(5270, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(5370, ETree2MiddleTreeSmallerReflected)); //width 64
		CJapaneseSquad::CreateJapaneseTankSquad(4770,true); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(4630,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseTankSquad(4030,true); //width 560
		//-------------------- COM STATION AND AIRPORT---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3950)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3900)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3850)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(3800,EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(3730)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(3650)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3550)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3450)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3350)); //width 80
		CJapaneseSquad::CreateJapaneseSoldierSquad(2900,true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(2900)); //Width width 700
		CComStation* lComStation = CComStation::New(2800,EConflictSideJapanese,0,0);
		lRightMostComStationGetsCaptured->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2620)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2520)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2450)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2400)); //width 32
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(2130,true); //width 244
		iGameObjectManager->AddGameObject(CForest3::New(2100)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierSquad(1650,true); //width 400
		CAirport* lAirport = CAirport::New(500,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lRightMostAirportGetsCaptured1->AddBuilding(lAirport);
		lRightMostAirportGetsCaptured2->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(0,true); //width 244
		//-------------------- MIDDLE PART AND HQ---------------------//
		iGameObjectManager->AddGameObject(CSmallTree::New(-150,ESmallTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-200)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-250)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-300)); //width 32
		iGameObjectManager->AddGameObject(CSmallTree::New(-370,ESmallTreeNormal)); //width 64
		CJapaneseSquad::CreateJapaneseTankSquad(-1000,true); //width 560
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1100)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1200)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1300)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1400)); //width 50
		CJapaneseSquad::CreateJapaneseSoldierSquad(-1800,true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(-1750)); //Width width 700
		CHq* lHq = CHq::New(-2500,EConflictSideJapanese);
		lRightMostHQGetsCaptured1->AddBuilding(lHq);
		lRightMostHQGetsCaptured2->AddBuilding(lHq);
		lRightMostHQGetsCaptured3->AddBuilding(lHq);
		lWinCondition->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670
		iGameObjectManager->AddGameObject(CTree1::New(-3000,EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CForest2::New(-3300)); //Width 271
		//-------------------- MIDDLE PART AND COM STATIONS---------------------//
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-3600,true); //width 244
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3700)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3800)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3900)); //width 80
		iGameObjectManager->AddGameObject(CBigTree::New(-4050,EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4150)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4250)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4350)); //width 80
		CJapaneseSquad::CreateJapaneseTankSquad(-5000,true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(-5100)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5200)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5280)); //width 32
		iGameObjectManager->AddGameObject(CSmallTree::New(-5360,ESmallTreeNormal)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5460)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5560)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5700)); //width 80
		lComStation = CComStation::New(-5800,EConflictSideJapanese,-10,150); //width 64, 53 to left
		lMiddleRightComStationGetsCaptured->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation);
		lComStation = CComStation::New(-5950,EConflictSideJapanese, 10,-150); //width 64, 53 to left
		lMiddleLeftComStationGetsCaptured->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation);
		CJapaneseSquad::CreateJapaneseTankSquad(-6600,true); //width 560
		iGameObjectManager->AddGameObject(CForest2::New(-6900)); //Width 271
		CJapaneseSquad::CreateJapaneseSoldierSquad(-7100,true); //width 400
		//-------------------- AIRPORTS ---------------------//
		lAirport = CAirport::New(-8200,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lLeftMostAirportRightGetsCaptured->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport); 
		lAirport = CAirport::New(-9500,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lLeftMostAirportMiddleGetsCaptured->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport); 
		lAirport = CAirport::New(-10800,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lLeftMostAirportLeftGetsCaptured->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseTankSquad(-11600,true); //width 560
		//-------------------- FINAL HQ ---------------------//
		iGameObjectManager->AddGameObject(CSmallTree::New(-11700,ESmallTreeNormal)); //width 64
		iGameObjectManager->AddGameObject(CBigTree::New(-11900,EBigTreeReflected)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(-12300,true); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-12400)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-12500)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-12600)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-12700)); //width 32
		lComStation = CComStation::New(-12800,EConflictSideJapanese,30,0); //width 64, 53 to left
		lLeftMostComStationGetsCaptured1->AddBuilding(lComStation);
		lLeftMostComStationGetsCaptured2->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation);
		lHq = CHq::New(-13500,EConflictSideJapanese);
		lWinCondition->AddBuilding(lHq);  //width to the left 450, to the right 670
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670
		
		iGameObjectManager->AddGameObject(CFloorWater::New(6500, 8450));   //right  side 6000 - 14550
		iGameObjectManager->AddGameObject(CFloorLand::New(-14950, 21450));
		
		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseTankSquadOrder;
		lJapaneseTankSquadOrder.DefaultXPosition = 4770;
		lJapaneseTankSquadOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lJapaneseTankSquadOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lJapaneseTankSquadOrder.MoveToLocation = false;
		lJapaneseTankSquadOrder.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder;
		lJapaneseSoldierAndTankSqaudOrder.DefaultXPosition = 4030;
		lJapaneseSoldierAndTankSqaudOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lJapaneseSoldierAndTankSqaudOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lJapaneseSoldierAndTankSqaudOrder.MoveToLocation = false;		
		lJapaneseSoldierAndTankSqaudOrder.ShouldProtectArea = true;
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder2;
		lJapaneseSoldierAndTankSqaudOrder2.DefaultXPosition = 2900;
		lJapaneseSoldierAndTankSqaudOrder2.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14550);
		lJapaneseSoldierAndTankSqaudOrder2.ProtectionArea.RightXPosition = TIntFloat::Convert(6400);
		lJapaneseSoldierAndTankSqaudOrder2.MoveToLocation = false;	
		lJapaneseSoldierAndTankSqaudOrder2.ShouldProtectArea = true;

		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusCondition);
		iEvents->Append(lRightMostComStationGetsCaptured);
		iEvents->Append(lRightMostAirportGetsCaptured1);
		iEvents->Append(lRightMostAirportGetsCaptured2);
		iEvents->Append(lRightMostHQGetsCaptured1);
		iEvents->Append(lRightMostHQGetsCaptured2);
		iEvents->Append(lRightMostHQGetsCaptured3);
		iEvents->Append(lMiddleRightComStationGetsCaptured);
		iEvents->Append(lMiddleLeftComStationGetsCaptured);
		iEvents->Append(lLeftMostAirportRightGetsCaptured);
		iEvents->Append(lLeftMostAirportMiddleGetsCaptured);
		iEvents->Append(lLeftMostAirportLeftGetsCaptured);
		iEvents->Append(lLeftMostComStationGetsCaptured1);
		iEvents->Append(lLeftMostComStationGetsCaptured2);
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeTankSquad,lJapaneseTankSquadOrder,60*(6),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankSqaudOrder,60*(6),-60*(1),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankSqaudOrder2,60*(6),-60*(2),TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign4Map1::InitializeGameObjectSettingsForMap()
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

void CCampaign4Map1::RemoveGameObjectSettingsForMap()
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
