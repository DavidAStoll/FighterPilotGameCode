/*
 ============================================================================
 Name		: Campaign2Map3.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map3 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign2Map3.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"


CCampaign2Map3::CCampaign2Map3()
:	CMap(EGameMapCampaign2Mission3,CAMPAIGN2_MAP3_SIZE, CAMPAIGN2_MAP3_INTERVALS, CAMPAIGN2_MAP3_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign2Map3::~CCampaign2Map3()
	{
	}

CCampaign2Map3* CCampaign2Map3::New()
	{
	CCampaign2Map3* self = new  CCampaign2Map3();
	self->Construct();
	return self;
	}

void CCampaign2Map3::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQ",CAMPAIGN2_MAP3_CAPTURE_HQ_BONUS_SCORE);
		CGetBonusPointsIfObjectsSurvive* lBonusCondition = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.CapturedAirports", CAMPAIGN2_MAP3_CAPTURE_AIRPORT_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//create home carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(7000,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		lLoseEventVictory->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//create troop transporters
		SGroundUnitOrder lOrder;
		lOrder.ShouldProtectArea = false;
		lOrder.MoveToLocation = true;
		lOrder.XLocationDestination = -14000;
		//Troop Transporter 1
		CAmericanTroopTransporter* lTroopTransporter = CAmericanTroopTransporter::New(6300,false,0,EAmericanLCTTypeTankAndTroops,lOrder);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//Troop Transporter 2
		lTroopTransporter = CAmericanTroopTransporter::New(5500,false,-60,EAmericanLCTTypeTroopsOnly,lOrder);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//create destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(4500,false));
		
		//------------------------ RIGHT BEACH --------------------------------//
		iGameObjectManager->AddGameObject(CTree2::New(2900, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2860)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(2820, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(2750, ETree2BowedTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2680)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(2630, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2566)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2500)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(2380, ETree2BowedTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(2316, ETree2BowedTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(2252, ETree2MiddleTree)); //width 64
		//----------------------- Small Tree Section -----------------------//
		iGameObjectManager->AddGameObject(CBigTree::New(2000,EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1820)); //width 32
		iGameObjectManager->AddGameObject(CBigTree::New(1850,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(1555,true); //width 244
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1430)); //width 32
		iGameObjectManager->AddGameObject(CBigTree::New(1450,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(900,true); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(860)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(750)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(630)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(670)); //Width width 700
		//----------------------- COM STATION -----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(500)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(400)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(300)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(230)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(150,EConflictSideJapanese,10,0)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(70)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(30)); //width 32
		CJapaneseSquad::CreateJapaneseSoldierSquad(-700,true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(-700)); //Width width 700
		//----------------------- AIRPORT --------------------------//
		iGameObjectManager->AddGameObject(CBigTree::New(-1000,EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1150)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1350)); //width 50
		iGameObjectManager->AddGameObject(CBigTree::New(-1500,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-2000,true); //width 244
		CAirport* lAirport = CAirport::New(-3200,EConflictSideJapanese);
		lBonusCondition->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(lAirport); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3900)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3600)); //width 50
		iGameObjectManager->AddGameObject(CForest3::New(-4200)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierSquad(-4600,true); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4650)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(-4700,EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4800)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5200)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5600)); //width 80
		iGameObjectManager->AddGameObject(CForest3::New(-5700)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-6000,true); //width 244
		//----------------------- COM STATION -----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6200)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6400)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6600)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(-6800)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-6900)); //width 80
		iGameObjectManager->AddGameObject(CComStation::New(-7000,EConflictSideJapanese,0,200)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-7100)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-7200)); //width 50
		iGameObjectManager->AddGameObject(CSmallTree::New(-7300,ESmallTreeNormal)); //width 64
		CJapaneseSquad::CreateJapaneseSoldierSquad(-7700,true); //width 400
		//----------------------- COM STATION -----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8000)); //width 32
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-8300,true); //width 244
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8400)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(-8400)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-8750)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-8900)); //width 80
		iGameObjectManager->AddGameObject(CComStation::New(-9000,EConflictSideJapanese,-10,100)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-9100)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-9200)); //width 32
		iGameObjectManager->AddGameObject(CBigTree::New(-9400,EBigTreeReflected)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(-9700,true); //width 400
		//---------------------- AIRPORT-------------------//
		lAirport = CAirport::New(-10900,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		lBonusCondition->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(lAirport); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(-11600,true); //width 400
		//--------------------- HQ -----------------------//
		CHq* lHq = CHq::New(-12300,EConflictSideJapanese);
		lWinCondition->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670
		
		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorLand::New(-14450, 17450));
		iGameObjectManager->AddGameObject(CFloorWater::New(3000, 11450));   //American water
		
		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseSoldierSquad1Order;
		lJapaneseSoldierSquad1Order.DefaultXPosition = 1555;
		lJapaneseSoldierSquad1Order.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14400);
		lJapaneseSoldierSquad1Order.ProtectionArea.RightXPosition = TIntFloat::Convert(2900);
		lJapaneseSoldierSquad1Order.MoveToLocation = false;
		lJapaneseSoldierSquad1Order.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierSquad2Order;
		lJapaneseSoldierSquad2Order.DefaultXPosition = 900;
		lJapaneseSoldierSquad2Order.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14400);
		lJapaneseSoldierSquad2Order.ProtectionArea.RightXPosition =TIntFloat::Convert(2900);
		lJapaneseSoldierSquad2Order.MoveToLocation = false;
		lJapaneseSoldierSquad2Order.ShouldProtectArea = true;	
		
		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusCondition);
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierSquad1Order,60*(4),-60*(1),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierSquad,lJapaneseSoldierSquad2Order,60*(4),0,TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign2Map3::InitializeGameObjectSettingsForMap()
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
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	CAmericanLCT::InitilizeAmericanLCT();
	CAmericanDestroyer::InitilizeAmericanDestroyer();
	CAmericanTroopTransporter::InitilizeAmericanTroopTransporter();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign2Map3::RemoveGameObjectSettingsForMap()
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
	//Ships
	CUSSVictory::RemoveUSSVictory();
	CAmericanLCT::RemoveAmericanLCT();
	CAmericanDestroyer::RemoveAmericanDestroyer();
	CAmericanTroopTransporter::RemoveAmericanTroopTransporter();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
