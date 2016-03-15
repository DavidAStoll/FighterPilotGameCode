/*
 ============================================================================
 Name		: Campaign3Map4.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map4 implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Maps/CCampaign3Map4.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign3Map4::CCampaign3Map4()
:	CMap(EGameMapCampaign3Mission4,CAMPAIGN3_MAP4_SIZE, CAMPAIGN3_MAP4_INTERVALS, CAMPAIGN3_MAP4_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign3Map4::~CCampaign3Map4()
	{
	}

CCampaign3Map4* CCampaign3Map4::New()
	{
	CCampaign3Map4* self = new  CCampaign3Map4();
	self->Construct();
	return self;
	}

void CCampaign3Map4::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQ", CAMPAIGN3_MAP4_CAPTURE_HQ_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//American fleet
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(10700, false);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//Victory
		CUSSVictory* lHomeCarrier = CUSSVictory::New(9300,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		lLoseEventVictory->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//create troop transporters
		SGroundUnitOrder lOrder;
		lOrder.ShouldProtectArea = false;
		lOrder.MoveToLocation = true;
		lOrder.XLocationDestination = -12100;
		//Troop Transporter 1
		CAmericanTroopTransporter* lTroopTransporter = CAmericanTroopTransporter::New(8000,false,0,EAmericanLCTTypeTankAndTroops,lOrder);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//Troop Transporter 2
		lTroopTransporter = CAmericanTroopTransporter::New(7100,false,-60,EAmericanLCTTypeTroopsOnly,lOrder);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//create destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(6500,false));
		
		
		//--------------------- RIGHT BEACH ---------------------//
		iGameObjectManager->AddGameObject(CTree2::New(4900, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4800, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4720, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(4680)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(4600)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(4530, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(4450)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(4370)); //width 32
		iGameObjectManager->AddGameObject(CTree2::New(4280, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4216, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(4116)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(4016)); //width 80
		iGameObjectManager->AddGameObject(CTree2::New(3940, ETree2MiddleTreeSmaller)); //width 64
		CJapaneseSquad::CreateJapaneseTankSquad(3380,true); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(3330,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(2900,true); //width 430
		iGameObjectManager->AddGameObject(CBigTree::New(3100,EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2800)); //width 80
		iGameObjectManager->AddGameObject(CBigTree::New(2700,EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2600)); //width 80
		//--------------------- COM STATIONS AND AIRPORT---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2500)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2320)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(2240)); //width 32
		iGameObjectManager->AddGameObject(CSmallTree::New(2160,ESmallTreeNormal)); //width 64
		iGameObjectManager->AddGameObject(CComStation::New(2070,EConflictSideJapanese,-10,150)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CComStation::New(1940,EConflictSideJapanese, 10,-150)); //width 64, 53 to left
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(1360,true); //width 430
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(800)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(700)); //width 32
		CJapaneseSquad::CreateJapaneseTankSquad(100,true); //width 560
		iGameObjectManager->AddGameObject(CForest3::New(600)); //Width width 700
		CAirport* lAirport = CAirport::New(-1000,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseSoldierSquad(-1600,true); //width 400
		iGameObjectManager->AddGameObject(CBigTree::New(-1750,EBigTreeNormal)); //width 128
		//--------------------- FOREST AND GUNS---------------------//
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1900)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2000)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2100)); //width 50
		iGameObjectManager->AddGameObject(CTree1::New(-2200,EBigThinTreeNormal)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2250)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2350)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2450)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2550)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2650)); //width 50
		iGameObjectManager->AddGameObject(CForest3::New(-3500)); //Width width 700
		CJapaneseSquad::CreateJapaneseTankSquad(-3300,true); //width 560
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3400)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3500)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3600)); //width 50
		iGameObjectManager->AddGameObject(CSmallTree::New(-3620,ESmallTreeNormal)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3720)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3820)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3920)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-4020)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-4120)); //width 50
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-4620,true); //width 430
		iGameObjectManager->AddGameObject(CForest3::New(-4920)); //Width width 700
		CJapaneseSquad::CreateJapaneseTankSquad(-5520,true); //width 560
		//------------------------- COM STATIONS -------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5620)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5720)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5820)); //width 80
		iGameObjectManager->AddGameObject(CBigTree::New(-5870,EBigTreeNormal)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5920)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(-6020,EConflictSideJapanese,30,150)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CComStation::New(-6160,EConflictSideJapanese, 40,-150)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6250)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-6350)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6430)); //width 32
		CJapaneseSquad::CreateJapaneseTankSquad(-7100,true); //width 560
		iGameObjectManager->AddGameObject(CBigTree::New(-7150,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(-7600,true); //width 400
		//----------------------- AIRPORTS -----------------------//
		lAirport = CAirport::New(-8600,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		lAirport = CAirport::New(-9900,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseTankSquad(-10650,true); //width 560
		iGameObjectManager->AddGameObject(CSmallTree::New(-10700,ESmallTreeReflected)); //width 64
		CJapaneseSquad::CreateJapaneseTankSquad(-11300,true); //width 560
		//--------------------- HQ -----------------------//
		CHq* lHq = CHq::New(-12100,EConflictSideJapanese);
		lWinCondition->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670
		
		iGameObjectManager->AddGameObject(CFloorWater::New(5000, 8950));   //right  side 6000 - 14550
		iGameObjectManager->AddGameObject(CFloorLand::New(-13950, 18950));
		
		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseTankSquadOrder;
		lJapaneseTankSquadOrder.DefaultXPosition = 3380;
		lJapaneseTankSquadOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-12500);
		lJapaneseTankSquadOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(4900);
		lJapaneseTankSquadOrder.MoveToLocation = false;
		lJapaneseTankSquadOrder.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder;
		lJapaneseSoldierAndTankSqaudOrder.DefaultXPosition = 2900;
		lJapaneseSoldierAndTankSqaudOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-12500);
		lJapaneseSoldierAndTankSqaudOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(4900);
		lJapaneseSoldierAndTankSqaudOrder.MoveToLocation = false;		
		lJapaneseSoldierAndTankSqaudOrder.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder2;
		lJapaneseSoldierAndTankSqaudOrder2.DefaultXPosition = 1360;
		lJapaneseSoldierAndTankSqaudOrder2.ProtectionArea.LeftXPosition = TIntFloat::Convert(-12500);
		lJapaneseSoldierAndTankSqaudOrder2.ProtectionArea.RightXPosition = TIntFloat::Convert(4900);
		lJapaneseSoldierAndTankSqaudOrder2.MoveToLocation = false;	
		lJapaneseSoldierAndTankSqaudOrder2.ShouldProtectArea = true;	

		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(lWinCondition);
		iEvents->Append(new  CGetBonusPointsIfLessTroopsGetKilled("Frontend.BonusEvents.LostLessThan80SoldiersAnd15Tanks", CAMPAIGN3_MAP4_LOSS_LESS_TROOPS_SCORE, CAMPAIGN3_MAP4_MAX_SOLDIER_LOSS,CAMPAIGN3_MAP4_MAX_TANK_LOSS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeTankSquad,lJapaneseTankSquadOrder,60*(6),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankSqaudOrder,60*(6),-60*(1),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankSqaudOrder2,60*(6),-60*(2),TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign3Map4::InitializeGameObjectSettingsForMap()
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
	CAmericanCarrierLexington::InitilizeAmericanCarrierLexington();
	CAmericanLCT::InitilizeAmericanLCT();
	CAmericanDestroyer::InitilizeAmericanDestroyer();
	CAmericanTroopTransporter::InitilizeAmericanTroopTransporter();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign3Map4::RemoveGameObjectSettingsForMap()
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
	CAmericanCarrierLexington::RemoveAmericanCarrierLexington();
	CAmericanLCT::RemoveAmericanLCT();
	CAmericanDestroyer::RemoveAmericanDestroyer();
	CAmericanTroopTransporter::RemoveAmericanTroopTransporter();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
