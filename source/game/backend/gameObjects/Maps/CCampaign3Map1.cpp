/*
 ============================================================================
 Name		: Campaign3Map1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map1 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign3Map1.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign3Map1::CCampaign3Map1()
:	CMap(EGameMapCampaign3Mission1,CAMPAIGN3_MAP1_SIZE, CAMPAIGN3_MAP1_INTERVALS, CAMPAIGN3_MAP1_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign3Map1::~CCampaign3Map1()
	{
	}

CCampaign3Map1* CCampaign3Map1::New()
	{
	CCampaign3Map1* self = new  CCampaign3Map1();
	self->Construct();
	return self;
	}

void CCampaign3Map1::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization,
	{
		iMapAlreadyInitilized = true;
		//EVENTS
		CLoseIfObjectsAreDestroyedEvent* lLoseEventVictory = CLoseIfObjectsAreDestroyedEvent::New();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQ", CAMPAIGN3_MAP1_CAPTURE_HQ_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//American fleet
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(12100, false);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//create home carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(10800,false);
		lLoseEventVictory->AddGameObject(lHomeCarrier);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//create troop transporters
		SGroundUnitOrder lOrder;
		lOrder.ShouldProtectArea = false;
		lOrder.MoveToLocation = true;
		lOrder.XLocationDestination = -13500;
		//Troop Transporter 1
		CAmericanTroopTransporter* lTroopTransporter = CAmericanTroopTransporter::New(9800,false,0,EAmericanLCTTypeTankAndTroops,lOrder);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//Troop Transporter 2
		lTroopTransporter = CAmericanTroopTransporter::New(9000,false,-60,EAmericanLCTTypeTroopsOnly,lOrder);
		iGameObjectManager->AddMoveableGameObject(lTroopTransporter);
		//create destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(8300,false));
		
		
		//--------------------- RIGHT BEACH ---------------------//
		iGameObjectManager->AddGameObject(CTree2::New(5900, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5860)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5800)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(5730, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5690)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5630)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(5550, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(5480, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(5440)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(5380)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(5280, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(5180, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(5080, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4980, ETree2MiddleTreeSmaller)); //width 64
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(4900,true); //width 244
		iGameObjectManager->AddGameObject(CTree2::New(4880, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4780, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4680, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4580, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4480, ETree2MiddleTree)); //width 64
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(4400,true); //width 430
		iGameObjectManager->AddGameObject(CTree2::New(4380, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4280, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4180, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(4080, ETree2MiddleTree)); //width 64
		CJapaneseSquad::CreateJapaneseSoldierSquad(3990,true); //width 400
		//----------------------- COM STATION AND AIRPORT-----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3800)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3750)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3650)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3550)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(3450)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3420)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(3360)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(3250,EConflictSideJapanese,10,0)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(3100)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2950)); //width 50
		CAirport* lAirport = CAirport::New(1700,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseTankSquad(900,true); //width 560
		lAirport = CAirport::New(-200,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseTankSquad(-1000,true); //width 560
		//----------------------- FOREST AREA -----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1100)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1180)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1280)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1380)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1480)); //width 80
		CJapaneseSquad::CreateJapaneseSoldierSquad(-2000,true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(-2000)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2070)); //width 50
		iGameObjectManager->AddGameObject(CBigTree::New(-2200,EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2270)); //width 50
		iGameObjectManager->AddGameObject(CBigTree::New(-2400,EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2470)); //width 50
		iGameObjectManager->AddGameObject(CBigTree::New(-2600,EBigTreeReflected)); //width 128
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2670)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2750)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(-2800,EBigThinTreeReflected)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2880)); //width 32
		//----------------------- COM STATION -----------------------//
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-3400,true); //width 430
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3500)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3600)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3700)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3780)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(-3900,EConflictSideJapanese,0,200)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4000)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(-4070,EBigThinTreeNormal)); // width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-4150)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4250)); //width 80
		//----------------------- FOREST AREA -----------------------//
		CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-4600,true); //width 244
		iGameObjectManager->AddGameObject(CForest3::New(-5000)); //Width width 700
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-5100,true); //width 430
		CJapaneseSquad::CreateJapaneseSoldierSquad(-5550,true); //width 400
		iGameObjectManager->AddGameObject(CForest3::New(-6000)); //Width width 700
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-6100)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6200)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6300)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-6400)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-6500)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6600)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6700)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-6800)); //width 80
		iGameObjectManager->AddGameObject(CForest3::New(-7000)); //Width width 700
		//----------------------- COM STATION -----------------------//
		CJapaneseSquad::CreateJapaneseTankSquad(-7650,true); //width 560
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-7700)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-7780)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-7860)); //width 32
		iGameObjectManager->AddGameObject(CTree1::New(-7920,EBigThinTreeNormal)); // width 32
		iGameObjectManager->AddGameObject(CComStation::New(-8020,EConflictSideJapanese,-10,100)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8150)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-8300)); //width 80
		iGameObjectManager->AddGameObject(CBigTree::New(-8600,EBigTreeNormal)); //width 128
		CJapaneseSquad::CreateJapaneseSoldierSquad(-8800,true); //width 400
		//----------------------- AIRPORTS -----------------------//
		lAirport = CAirport::New(-10100,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		lAirport = CAirport::New(-11400,EConflictSideJapanese); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		CJapaneseSquad::CreateJapaneseTankSquad(-12300,true); //width 560
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-12800,true); //width 430
		//--------------------- HQ -----------------------//
		CHq* lHq = CHq::New(-13500,EConflictSideJapanese);
		lWinCondition->AddBuilding(lHq);
		iGameObjectManager->AddGameObject(lHq); //width to the left 450, to the right 670
		
		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseTankSquadOrder;
		lJapaneseTankSquadOrder.DefaultXPosition = 900;
		lJapaneseTankSquadOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14400);
		lJapaneseTankSquadOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(5900);
		lJapaneseTankSquadOrder.MoveToLocation = false;
		lJapaneseTankSquadOrder.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierAndTankSqaudOrder;
		lJapaneseSoldierAndTankSqaudOrder.DefaultXPosition = 4400;
		lJapaneseSoldierAndTankSqaudOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14400);
		lJapaneseSoldierAndTankSqaudOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(5900);
		lJapaneseSoldierAndTankSqaudOrder.MoveToLocation = false;		
		lJapaneseSoldierAndTankSqaudOrder.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierSquadOrder;
		lJapaneseSoldierSquadOrder.DefaultXPosition = 3990;
		lJapaneseSoldierSquadOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-14400);
		lJapaneseSoldierSquadOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(5900);
		lJapaneseSoldierSquadOrder.MoveToLocation = false;
		lJapaneseSoldierSquadOrder.ShouldProtectArea = true;	
		
		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorWater::New(6000, 8950));   //right  side 6000 - 14550
		iGameObjectManager->AddGameObject(CFloorLand::New(-14950, 20950));
		
		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseEventVictory);
		iEvents->Append(lWinCondition);
		iEvents->Append(new  CGetBonusPointsIfLessTroopsGetKilled("Frontend.BonusEvents.LostLessThan80SoldiersAnd15Tanks", CAMPAIGN3_MAP1_LOSS_LESS_TROOPS_SCORE, CAMPAIGN3_MAP1_MAX_SOLDIER_LOSS,CAMPAIGN3_MAP1_MAX_TANK_LOSS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeTankSquad,lJapaneseTankSquadOrder,60*(6),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankSqaudOrder,60*(6),-60*(1),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierSquad,lJapaneseSoldierSquadOrder,60*(6),-60*(2),TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign3Map1::InitializeGameObjectSettingsForMap()
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
	CAmericanRifleman::InitilizeAmericanRifleman();
	CAmericanRocketman::InitilizeAmericanRocketman();
	CAmericanShermanTank::InitilizeAmericanShermanTank();
	CAmericanAAShermanTank::InitilizeAmericanAAShermanTank();
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
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	CAmericanLCT::InitilizeAmericanLCT();
	CAmericanCarrierLexington::InitilizeAmericanCarrierLexington();
	CAmericanTroopTransporter::InitilizeAmericanTroopTransporter();
	CAmericanDestroyer::InitilizeAmericanDestroyer();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign3Map1::RemoveGameObjectSettingsForMap()
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
	CAmericanRifleman::RemoveAmericanRifleman();
	CAmericanRocketman::RemoveAmericanRocketman();
	CAmericanShermanTank::RemoveAmericanShermanTank();
	CAmericanAAShermanTank::RemoveAmericanAAShermanTank();
	CJapaneseRifleman::RemoveJapaneseRifleman();
	CJapaneseRocketman::RemoveJapaneseRocketman();
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
	CAmericanCarrierLexington::RemoveAmericanCarrierLexington();
	CAmericanTroopTransporter::RemoveAmericanTroopTransporter();
	CAmericanDestroyer::RemoveAmericanDestroyer();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
