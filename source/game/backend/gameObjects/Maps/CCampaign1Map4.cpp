/*
 ============================================================================
 Name		: Campaign1Map4.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign1Map4 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign1Map4.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign1Map4::CCampaign1Map4()
:	CMap(EGameMapCampaign1Mission4,CAMPAIGN1_MAP4_SIZE, CAMPAIGN1_MAP4_INTERVALS, CAMPAIGN1_MAP4_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign1Map4::~CCampaign1Map4()
	{
	}

CCampaign1Map4* CCampaign1Map4::New()
	{
	CCampaign1Map4* self = new CCampaign1Map4();
	self->Construct();
	return self;
	}

void CCampaign1Map4::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CLoseIfObjectsAreCaptured* lLoseEventHq = new  CLoseIfObjectsAreCaptured();
		CGetBonusPointsIfObjectsSurvive* lBonusEventAirportSurvives = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.AirportSurvived", CAMPAIGN1_MAP4_AIRPORT_SURVIVES_BONUS_SCORE);
		//spawn american troop transporter
		SGroundUnitOrder lAmericanAttackOrder;
		lAmericanAttackOrder.ShouldProtectArea = false;
		lAmericanAttackOrder.MoveToLocation = true;
		lAmericanAttackOrder.XLocationDestination = -4100;
		CSpawnAmericanTroopTransporterIfObjectsDestroyed* lSpawnAmericanTroopTransporterEvent = new  CSpawnAmericanTroopTransporterIfObjectsDestroyed (9000,EAmericanLCTTypeTroopsOnly,lAmericanAttackOrder);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//create home carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(6700,false);
		lLoseEventVictory->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		
		//------------------------ RIGHT BEACH --------------------------------//
		iGameObjectManager->AddGameObject(CTree2::New(5340, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5300,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(5220, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(5155, ETree2BowedTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5100,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(5035, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5000,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(4920,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(4850, ETree2BowedTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4765, ETree2BowedTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4700, ETree2MiddleTree)); //width 64
		//------------------------ HQ --------------------------------//
		SGroundUnitOrder lAmericanProtectHQOrder;
		lAmericanProtectHQOrder.DefaultXPosition = 4650;
		lAmericanProtectHQOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(2200);
		lAmericanProtectHQOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(5340);
		lAmericanProtectHQOrder.MoveToLocation = false;
		CAmericanSquad::CreateAmericanSoldierAndTankSquad(4650,false)->GiveOrder(lAmericanProtectHQOrder); //width 244
		iGameObjectManager->AddGameObject(CBigTree::New(4270,EBigTreeNormal)); //width 128
		CHq* lHq = CHq::New(3600,EConflictSideAmerican);
		lLoseEventHq->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670
		//------------------------ AIRPORT --------------------------------//
		SGroundUnitOrder lAmericanProtectAirport;
		lAmericanProtectAirport.DefaultXPosition = 2950;
		lAmericanProtectAirport.ProtectionArea.LeftXPosition = TIntFloat::Convert(730);
		lAmericanProtectAirport.ProtectionArea.RightXPosition = TIntFloat::Convert(5340);
		lAmericanProtectAirport.MoveToLocation = false;
		CAmericanSquad::CreateAmericanSoldierSquad(2950,false)->GiveOrder(lAmericanProtectAirport); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(2400)); // width 692
		iGameObjectManager->AddGameObject(CComStation::New(2330,EConflictSideAmerican,-10,100)); //width 110
		iGameObjectManager->AddGameObject(CSmallTree::New(2200, ESmallTreeReflected)); //width 64
		CAirport* lAirport = CAirport::New(1050,EConflictSideAmerican);
		lBonusEventAirportSurvives->AddGameObject(lAirport);
		iPlayer->AssignPlayerBase(lAirport);
		iPlayer->SpawnNewPlayerImmediately();
		iGameObjectManager->AddGameObject(lAirport); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(CSmallTree::New(730, ESmallTreeReflected)); //width 64
		//------------------------ FOREST AREA BEFORE AIRPORT --------------------------------//
		CAmericanSquad::CreateAmericanSoldierSquad(550,false); //width 400
		iGameObjectManager->AddGameObject(CBigTree::New(450,EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CForest2::New(-150)); //Width 271
		iGameObjectManager->AddGameObject(CTree1::New(-200,EBigThinTreeNormal)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-500,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-850,EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-950,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CForest4::New(-1100)); //Width  849
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1200,EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1350,EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1500,EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CBigTree::New(-1600,EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1680,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1730,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CSmallTree::New(-1800, ESmallTreeNormal)); //width 64
		//------------------------ COM STATION --------------------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1870,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(-1950,EConflictSideAmerican,10,0)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2150,EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2200,EConflictSideAmerican)); //width 32
		CAmericanSquad::CreateAmericanSoldierAndTankSquad(-2270,false); //width 244
		iGameObjectManager->AddGameObject(CTree1::New(-2550,EBigThinTreeReflected)); // width 32
		//------------------------ FOREST AREA BEFORE COM STATION --------------------------------//
		iGameObjectManager->AddGameObject(CForest2::New(-2830)); //Width 271
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2850,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CSmallTree::New(-2950, ESmallTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2985,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3100,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CSmallTree::New(-3150, ESmallTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3200,EConflictSideAmerican)); //width 32
		CAmericanSquad::CreateAmericanSoldierSquad(-3250,false); //width 400
		iGameObjectManager->AddGameObject(CForest2::New(-3600)); //Width 271
		//------------------------ COM STATION --------------------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3700,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(-3780,EConflictSideAmerican,0,200)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3930,EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4000,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CBigTree::New(-4200,EBigTreeReflected)); //width 128
		CAmericanSquad::CreateAmericanSoldierSquad(-4100,false); //width 400
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4850,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4750,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CForest3::New(-4900)); // width 692
		//------------------------ LEFT BEACH --------------------------------//
		iGameObjectManager->AddGameObject(CTree2::New(-4980, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-5050, ETree2BowedTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5060,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(-5120, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5170,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5230,EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(-5300, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5350,EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(-5430, ETree2MiddleTreeSmaller)); //width 64
		
		//------------------------ JAPANESE NAVAL FORCE --------------------------------//
		CJapaneseDestroyer* lDestroyer = CJapaneseDestroyer::New(-8500,true);
		lDestroyer->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_TROOPS);
		lSpawnAmericanTroopTransporterEvent->AddGameObject(lDestroyer);
		iGameObjectManager->AddMoveableGameObject(lDestroyer);
		//Landing force
		SGroundUnitOrder lJapaneseAttackOrder;
		lJapaneseAttackOrder.MoveToLocation = true;
		lJapaneseAttackOrder.XLocationDestination = 5340;
		lJapaneseAttackOrder.ShouldProtectArea = false;
		//create some LCTs that are going to land
		iGameObjectManager->AddMoveableGameObject(CJapaneseLCT::New(-8000,true,EJapaneseLCTTypeTankAndTroops,lJapaneseAttackOrder));
		iGameObjectManager->AddMoveableGameObject(CJapaneseLCT::New(-9000,true,EJapaneseLCTTypeTankAndTroops,lJapaneseAttackOrder));
		iGameObjectManager->AddMoveableGameObject(CJapaneseTroopTransporter::New(-10000,true,0,EJapaneseLCTTypeTroopsOnly,lJapaneseAttackOrder));
		iGameObjectManager->AddMoveableGameObject(CJapaneseTroopTransporter::New(-11000,true,-60,EJapaneseLCTTypeTroopsOnly,lJapaneseAttackOrder));
		
		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorWater::New(5450, 9000));   //American water
		iGameObjectManager->AddGameObject(CFloorLand::New(-5450, 10900));
		iGameObjectManager->AddGameObject(CFloorWater::New(-14450, 9000)); //Japanese water
		//home base is still carrier
		iPlayer->AssignPlayerBase(lHomeCarrier);
		
		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(lLoseEventHq);
		iEvents->Append(lSpawnAmericanTroopTransporterEvent);
		iEvents->Append(new CWinIfAllJapaneseMoveableObjectsAreDestroyed(CAMPAIGN1_MAP1_DESTROY_JAPANESE_FORCES_BONUS_SCORE));
		iEvents->Append(lBonusEventAirportSurvives);
	}
}


void CCampaign1Map4::InitializeGameObjectSettingsForMap()
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
	CAmericanRifleman::InitilizeAmericanRifleman();
	CAmericanRocketman::InitilizeAmericanRocketman();
	CAmericanShermanTank::InitilizeAmericanShermanTank();
	CAmericanAAShermanTank::InitilizeAmericanAAShermanTank();
	CJapanese97ChiHaTank::InitilizeJapanese97ChiHaTank();
	CJapaneseAA97ChiHaTank::InitilizeJapaneseAA97ChiHaTank();
	//Ship Weapons and Projectiles
	CExplosive7InchShell::InitilizeExplosive7InchShell();
	CGun7Inch::InitilizeGun7Inch();
	CShipDualMachineGun::InitilizeShipDualMachineGun();
	CShipDual75mmGun::InitilizeShipDual75mmGun();
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	CAmericanLCT::InitilizeAmericanLCT();
	CAmericanTroopTransporter::InitilizeAmericanTroopTransporter();
	CJapaneseLCT::InitilizeJapaneseLCT();
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	CJapaneseTroopTransporter::InitilizeJapaneseTroopTransporter();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign1Map4::RemoveGameObjectSettingsForMap()
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
	CShipDualMachineGun::RemoveShipDualMachineGun();
	CShipDual75mmGun::RemoveShipDual75mmGun();
	//Ships
	CUSSVictory::RemoveUSSVictory();
	CAmericanLCT::RemoveAmericanLCT();
	CAmericanTroopTransporter::RemoveAmericanTroopTransporter();
	CJapaneseLCT::RemoveJapaneseLCT();
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	CJapaneseTroopTransporter::RemoveJapaneseTroopTransporter();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
