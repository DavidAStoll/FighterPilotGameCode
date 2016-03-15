/*
 ============================================================================
 Name		: Campaign4Map2.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map2 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign4Map2.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign4Map2::CCampaign4Map2() :
	CMap(EGameMapCampaign4Mission2, CAMPAIGN4_MAP2_SIZE, CAMPAIGN4_MAP2_INTERVALS, CAMPAIGN4_MAP2_MISSION_TIME_IN_MINS)
{
	// No implementation required
}

CCampaign4Map2::~CCampaign4Map2()
{
}

CCampaign4Map2* CCampaign4Map2::New()
{
	CCampaign4Map2* self = new  CCampaign4Map2();
	self->Construct();
	return self;
}

void CCampaign4Map2::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreCaptured* lLoseEventIfAirportGetsCaptured = new  CLoseIfObjectsAreCaptured();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQ", CAMPAIGN4_MAP2_CAPTURE_HQ_BONUS_SCORE);
		CGetBonusPointsIfObjectsSurvive* lBonusEventAllAirportSurvive = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.AllAirportsSurvived", CAMPAIGN4_MAP2_ALL_AIRPORT_SURVIVE_BONUS_SCORE);

		//Player Object
		iPlayer = CPlayer::New();
		CAirport* lAirport = NULL;
		CComStation* lComStation = NULL;
		CHq* lHq = NULL;
		
		SGroundUnitOrder lJapaneseStormOrder;
		lJapaneseStormOrder.MoveToLocation = true;
		lJapaneseStormOrder.XLocationDestination = 10600;
		lJapaneseStormOrder.ShouldProtectArea = false;
		
		//Extra Spawn Events if Buildings get Captured
		//Right Airport
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lFirstAirportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lFirstAirportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeTankSquad,lJapaneseStormOrder,10);
		//Middle ComStation
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleComStationGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleComStationGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder,10);
		//Middle Airport
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleAiportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleAiportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lMiddleAiportGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,20);
		//Last Two ComStations
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastComStation1GetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastComStation2GetsCaptured = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseStormOrder,10);
		//Last Airport Gets Captured - Super banzai charge!!!
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured1 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,0);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured2 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,10);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured3 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,20);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured4 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,30);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured5 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,40);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured6 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,50);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured7 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,60);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured8 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,70);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured9 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,80);
		CSpawnJapaneseSquadIfBuildingGetsCaptured* lLastAirportGetsCaptured10 = new  CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierSquad,lJapaneseStormOrder,90);
		
		//--------------------- American Base ---------------------//
		lAirport = CAirport::New(9500, EConflictSideAmerican); //widht to the left 175, witdh to the right 1050
		lLoseEventIfAirportGetsCaptured->AddBuilding(lAirport);
		lBonusEventAllAirportSurvive->AddGameObject(lAirport);
		iPlayer->AssignPlayerBase(lAirport);
		//iPlayer->SpawnNewPlayerImmediately();

		//test
		CMitchell* lValPlane = CMitchell::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(9500,500));
		iPlayer->AssignAircarft(lValPlane);
		iGameObjectManager->AddMoveableGameObject(lValPlane);

		iGameObjectManager->AddGameObject(lAirport);
		CAmericanSquad::CreateAmericanTankSquad(9300, false); //width 560
		iGameObjectManager->AddGameObject(CSmallTree::New(8650, ESmallTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CSmallTree::New(8500, ESmallTreeNormal)); //width 64
		CAmericanSquad::CreateAmericanSoldierSquad(8750, false); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(8250, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(8150, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(8050, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(8000, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(7950, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(7850, EConflictSideAmerican)); //width 50
		lComStation = CComStation::New(7750, EConflictSideAmerican, 0, 0);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(7670, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(7600, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CBigTree::New(7250, EBigTreeReflected)); //width 128
		CAmericanSquad::CreateAmericanSoldierSquad(7550, false); //width 400
		//---------------------- Forest Area, Com Station and Airport----------------------------//
		iGameObjectManager->AddGameObject(CForest3::New(6900)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(6900)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(6800)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6700)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(6600)); //width 32
		CJapaneseSquad::CreateJapaneseSoldierSquad(6150, true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(6100)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(6050)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5950)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5850)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5750)); //width 50
		CJapaneseSquad::CreateJapaneseTankSquad(5200, true); //width 560
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5100)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5040)); //width 32
		lComStation = CComStation::New(4940, EConflictSideJapanese, 10, 150);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(4740)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(4640)); //width 80
		iGameObjectManager->AddGameObject(CBigTree::New(4570, EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(4550)); //width 32
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(4050, true); //width 430
		lAirport = CAirport::New(2950, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lBonusEventAllAirportSurvive->AddGameObject(lAirport);
		lFirstAirportGetsCaptured1->AddBuilding(lAirport);
		lFirstAirportGetsCaptured2->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		//---------------------- Forest, Comstation, Forest, Airport --------------------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2700)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2650)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2400)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2300)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2200)); //width 80
		CJapaneseSquad::CreateJapaneseTankSquad(1600, true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(2000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest2::New(1500)); //Width 271
		lComStation = CComStation::New(1400, EConflictSideJapanese, -10, -150);
		lMiddleComStationGetsCaptured1->AddBuilding(lComStation);
		lMiddleComStationGetsCaptured2->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1330)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1250)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(1200, EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(1130)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(941)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(850)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(750)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(-200)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierSquad(250, true); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(100)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(0)); //width 80
		iGameObjectManager->AddGameObject(CForest2::New(-100)); //Width 271
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-600, true); //width 430
		lAirport = CAirport::New(-1800, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lBonusEventAllAirportSurvive->AddGameObject(lAirport);
		lMiddleAiportGetsCaptured1->AddBuilding(lAirport);
		lMiddleAiportGetsCaptured2->AddBuilding(lAirport);
		lMiddleAiportGetsCaptured3->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(CSmallTree::New(-2150, ESmallTreeNormal)); //width 64
		//---------------------- Small Forest, ComStations And Big Airport --------------------------------//
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2400)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2500)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2600)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2800)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2900)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3000)); //width 50
		iGameObjectManager->AddGameObject(CForest3::New(-3000)); //Width width 700
		CJapaneseSquad::CreateJapaneseTankSquad(-3650, true); //width 560
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3750)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3830)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3930)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4030)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4130)); //width 80
		lComStation = CComStation::New(-4230, EConflictSideJapanese, 20, -150);
		lLastComStation1GetsCaptured->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		lComStation = CComStation::New(-4430, EConflictSideJapanese, -20, 150);
		lLastComStation2GetsCaptured->AddBuilding(lComStation);
		iGameObjectManager->AddGameObject(lComStation); //width 64, 53 to left
		CJapaneseSquad::CreateJapaneseTankSquad(-5080, true); //width 560
		lAirport = CAirport::New(-6280, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050 ,,
		lBonusEventAllAirportSurvive->AddGameObject(lAirport);
		lLastAirportGetsCaptured1->AddBuilding(lAirport);
		lLastAirportGetsCaptured2->AddBuilding(lAirport);
		lLastAirportGetsCaptured3->AddBuilding(lAirport);
		lLastAirportGetsCaptured4->AddBuilding(lAirport);
		lLastAirportGetsCaptured5->AddBuilding(lAirport);
		lLastAirportGetsCaptured6->AddBuilding(lAirport);
		lLastAirportGetsCaptured7->AddBuilding(lAirport);
		lLastAirportGetsCaptured8->AddBuilding(lAirport);
		lLastAirportGetsCaptured9->AddBuilding(lAirport);
		lLastAirportGetsCaptured10->AddBuilding(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		lAirport = CAirport::New(-7600, EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lBonusEventAllAirportSurvive->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		CJapaneseSquad::CreateJapaneseTankSquad(-8400, true); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(-8350, EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-9000, true); //width 430
		lHq = CHq::New(-9650, EConflictSideJapanese);
		lWinCondition->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670

		//Floor
		iGameObjectManager->AddGameObject(CFloorLand::New(-11490, 22980));

		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseSquadOrder;
		lJapaneseSquadOrder.MoveToLocation = true;
		lJapaneseSquadOrder.XLocationDestination = 10600;
		lJapaneseSquadOrder.ShouldProtectArea = false;
		//create orders for American Squads that spawn on the map
		SGroundUnitOrder lAmericanSquadOrder;
		lAmericanSquadOrder.MoveToLocation = true;
		lAmericanSquadOrder.XLocationDestination = -9650;
		lAmericanSquadOrder.ShouldProtectArea = false;

		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventIfAirportGetsCaptured);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusEventAllAirportSurvive);
		//extra charge events
		iEvents->Append(lFirstAirportGetsCaptured1);
		iEvents->Append(lFirstAirportGetsCaptured2);
		iEvents->Append(lMiddleComStationGetsCaptured1);
		iEvents->Append(lMiddleComStationGetsCaptured2);
		iEvents->Append(lMiddleAiportGetsCaptured1);
		iEvents->Append(lMiddleAiportGetsCaptured2);
		iEvents->Append(lMiddleAiportGetsCaptured3);
		iEvents->Append(lLastComStation1GetsCaptured);
		iEvents->Append(lLastComStation2GetsCaptured);
		iEvents->Append(lLastAirportGetsCaptured1);
		iEvents->Append(lLastAirportGetsCaptured2);
		iEvents->Append(lLastAirportGetsCaptured3);
		iEvents->Append(lLastAirportGetsCaptured4);
		iEvents->Append(lLastAirportGetsCaptured5);
		iEvents->Append(lLastAirportGetsCaptured6);
		iEvents->Append(lLastAirportGetsCaptured7);
		iEvents->Append(lLastAirportGetsCaptured8);
		iEvents->Append(lLastAirportGetsCaptured9);
		iEvents->Append(lLastAirportGetsCaptured10);
		//American Squads
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeTankSquad, lAmericanSquadOrder, 60 * (5), 0, TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeSoldierAndTankSquad, lAmericanSquadOrder, 60 * (5), -60 * (1), TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeSoldierAndTankSquad, lAmericanSquadOrder, 60 * (5), -60 * (2), TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeSoldierSquad, lAmericanSquadOrder, 60 * (5), -60 * (3), TIME_EVENT_INFINITE_TIME_EVENTS));
		//Japanese Squads
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeTankSquad, lJapaneseSquadOrder, 60 * (5), 0, TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad, lJapaneseSquadOrder, 60 * (5), -60 * (1), TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad, lJapaneseSquadOrder, 60 * (5), -60 * (2), TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierSquad, lJapaneseSquadOrder, 60 * (5), -60 * (3), TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign4Map2::InitializeGameObjectSettingsForMap()
{
	//new
	CWeaponAircraftCannon20mm::InitilizeWeaponAircraftCannon20mm();
	CWeaponAircraftMachineGun13mm::InitilizeWeaponAircraftMachineGun13mm();
	CWeaponAircraftMachineGun8mm::InitilizeWeaponAircraftMachineGun8mm();
	CSmoke::InitilizeSmoke();
	CBackGunMachineGun8mm::InitilizeBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::InitilizeBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::InitilizeBackGunDoubleMachineGun13mm();
	CBulletRifle::InitilizeBulletRifle();
	CBullet8mm::InitilizeBullet8mm();
	CBullet13mm::InitilizeBullet13mm();
	CBulletCannon20mm::InitilizeBulletCannon20mm();


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
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign4Map2::RemoveGameObjectSettingsForMap()
{
	//new
	CWeaponAircraftCannon20mm::RemoveWeaponAircraftCannon20mm();
	CWeaponAircraftMachineGun13mm::RemoveWeaponAircraftMachineGun13mm();
	CWeaponAircraftMachineGun8mm::RemoveWeaponAircraftMachineGun8mm();
	CSmoke::RemoveSmoke();
	CBackGunMachineGun8mm::RemoveBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::RemoveBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::RemoveBackGunDoubleMachineGun13mm();
	CBulletRifle::RemoveBulletRifle();
	CBullet8mm::RemoveBullet8mm();
	CBullet13mm::RemoveBullet13mm();
	CBulletCannon20mm::RemoveBulletCannon20mm();

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
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}

