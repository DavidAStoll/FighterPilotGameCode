/*
 ============================================================================
 Name		: Campaign1Map3.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCampaign1Map3 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign1Map3.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign1Map3::CCampaign1Map3() :
	CMap(EGameMapCampaign1Mission3,CAMPAIGN1_MAP3_SIZE, CAMPAIGN1_MAP3_INTERVALS, CAMPAIGN1_MAP3_MISSION_TIME_IN_MINS)
{
	// No implementation required
}

CCampaign1Map3::~CCampaign1Map3()
{
}

CCampaign1Map3* CCampaign1Map3::New()
{
	CCampaign1Map3* self = new CCampaign1Map3();
	self->Construct();
	return self;
}

void CCampaign1Map3::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseCondition = CLoseIfObjectsAreDestroyedEvent::New();
		CWinIfObjectsAreCaptured* lWinCondition = new  CWinIfObjectsAreCaptured("Frontend.BonusEvents.CapturedHQ",CAMPAIGN1_MAP3_CAPTURE_HQ_BONUS_SCORE);
		CGetBonusPointsIfObjectsSurvive* lBonusEvent = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.CapturedAirport", CAMPAIGN1_MAP3_CAPTURE_AIRPORT_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		
		//create home carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(3750,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately(); //creates new aircraft for player
		lLoseCondition->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		
		//Create Beach with Palm tree and AA Guns
		iGameObjectManager->AddGameObject(CTree2::New(2350, ETree2BowedTreeSmaller));
		iGameObjectManager->AddGameObject(CTree2::New(2286, ETree2BowedTreeReflected));
		
		//88mm, concrete cannon, tree, concrete cannon and 20mm
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(2180));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(2080));
		iGameObjectManager->AddGameObject(CTree2::New(1990, ETree2BowedTree));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(1889));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1815));
		iGameObjectManager->AddGameObject(CTree2::New(1750, ETree2MiddleTree));
		iGameObjectManager->AddGameObject(CTree2::New(1670, ETree2BowedTreeSmaller));
		
		//create some trees and Soldier and AA Tank squad
		iGameObjectManager->AddGameObject(CBigTree::New(1242,EBigTreeNormal));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(1198));
		iGameObjectManager->AddGameObject(CBigTree::New(1050,EBigTreeReflected));
		iGameObjectManager->AddGameObject(CSmallTree::New(830,ESmallTreeNormal));
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(450,true);
		iGameObjectManager->AddGameObject(CTree1::New(322,EBigThinTreeNormal));
		
		//right most ComStation and AA Guns
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(172));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(95));
		iGameObjectManager->AddGameObject(CComStation::New(-50,EConflictSideJapanese,-10,100));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-220));
		iGameObjectManager->AddGameObject(CBigTree::New(-350,EBigTreeNormal));
		
		//group of soldiers and Forest
		iGameObjectManager->AddGameObject(CForest3::New(-1150));
		CJapaneseSquad::CreateJapaneseSoldierSquad(-1050,true);
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1190));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-1250));
		
		//Airport
		CAirport* lAirport = CAirport::New(-2550,EConflictSideJapanese);
		//lAirport->SetSpawnPlanes(false);
		lBonusEvent->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(CSmallTree::New(-2850,ESmallTreeNormal));
		
		//small Forest section with AA Cannons and Soldiers and AA tank
		iGameObjectManager->AddGameObject(CSmallTree::New(-3150,ESmallTreeReflected));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3200));
		iGameObjectManager->AddGameObject(CTree1::New(-3350,EBigThinTreeReflected));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-3410));
		iGameObjectManager->AddGameObject(CSmallTree::New(-3500,ESmallTreeNormal));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3565));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3750));
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-4050,true);
		iGameObjectManager->AddGameObject(CSmallTree::New(-4130,ESmallTreeNormal));
		
		//two very left ComStation
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4450));
		iGameObjectManager->AddGameObject(CBigTree::New(-4520,EBigTreeNormal));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4650));
		iGameObjectManager->AddGameObject(CComStation::New(-4720,EConflictSideJapanese,10,0));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4785));
		iGameObjectManager->AddGameObject(CComStation::New(-4900,EConflictSideJapanese,0,200));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5010));
		iGameObjectManager->AddGameObject(CTree1::New(-5060,EBigThinTreeReflected));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-5210));
		
		//small forest and HQ
		iGameObjectManager->AddGameObject(CForest2::New(-5700));
		CHq* lHQ = CHq::New(-6550,EConflictSideJapanese);
		lWinCondition->AddBuilding(lHQ);
		iGameObjectManager->AddGameObject(lHQ);//goes around 500 to the left
		

		//add the floors, should extend to the end of the map otherwise it will crash the game,
		iGameObjectManager->AddGameObject(CFloorLand::New(-8450, 10900));
		iGameObjectManager->AddGameObject(CFloorWater::New(2450, 6000));
		
		//create orders for Japanese Squads that spawn on the map
		SGroundUnitOrder lJapaneseSoldierSquad1Order;
		lJapaneseSoldierSquad1Order.DefaultXPosition = 1242;
		lJapaneseSoldierSquad1Order.ProtectionArea.LeftXPosition = TIntFloat::Convert(-8450);
		lJapaneseSoldierSquad1Order.ProtectionArea.RightXPosition = TIntFloat::Convert(2350);
		lJapaneseSoldierSquad1Order.MoveToLocation = false;
		lJapaneseSoldierSquad1Order.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierSquad2Order;
		lJapaneseSoldierSquad2Order.DefaultXPosition = -400;
		lJapaneseSoldierSquad2Order.ProtectionArea.LeftXPosition = TIntFloat::Convert(-8450);
		lJapaneseSoldierSquad2Order.ProtectionArea.RightXPosition = TIntFloat::Convert(2350);
		lJapaneseSoldierSquad2Order.MoveToLocation = false;
		lJapaneseSoldierSquad2Order.ShouldProtectArea = true;	
		SGroundUnitOrder lJapaneseSoldierAndTankOSquadOrder;
		lJapaneseSoldierAndTankOSquadOrder.DefaultXPosition = -5110;
		lJapaneseSoldierAndTankOSquadOrder.ProtectionArea.LeftXPosition = TIntFloat::Convert(-8450);
		lJapaneseSoldierAndTankOSquadOrder.ProtectionArea.RightXPosition = TIntFloat::Convert(2350);
		lJapaneseSoldierAndTankOSquadOrder.MoveToLocation = false;
		lJapaneseSoldierAndTankOSquadOrder.ShouldProtectArea = true;	
		//create American Landing Order
		SGroundUnitOrder lAmericanAttackOrder;
		lAmericanAttackOrder.ShouldProtectArea = false;
		lAmericanAttackOrder.MoveToLocation = true;
		lAmericanAttackOrder.XLocationDestination = -7050;
		
		//Americans start with two LCTs
		iGameObjectManager->AddMoveableGameObject(CAmericanLCT::New(5000,false,EAmericanLCTTypeTroopsOnly,lAmericanAttackOrder));
		iGameObjectManager->AddMoveableGameObject(CAmericanLCT::New(5500,false,EAmericanLCTTypeTanksOnly,lAmericanAttackOrder));
		
		//add Events
		iEvents->Append(lLoseCondition);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusEvent);
		//American Spawn events
		iEvents->Append(new  CSpawnAmericanLCTEveryTimeInterval(EAmericanLCTTypeTankAndTroops,lAmericanAttackOrder,60*(4),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnAmericanLCTEveryTimeInterval(EAmericanLCTTypeTanksOnly,lAmericanAttackOrder,60*(10),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		//Japanese Spawn events
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierSquad,lJapaneseSoldierSquad1Order,60*(4),-60*(2),TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierSquad,lJapaneseSoldierSquad2Order,60*(4),0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAndTankSquad,lJapaneseSoldierAndTankOSquadOrder,60*(8),60*(1),TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}


void CCampaign1Map3::InitializeGameObjectSettingsForMap()
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
	CBackGunMachineGun8mm::InitilizeBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::InitilizeBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::InitilizeBackGunDoubleMachineGun13mm();
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
	CShipDualMachineGun::InitilizeShipDualMachineGun();
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	CAmericanLCT::InitilizeAmericanLCT();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign1Map3::RemoveGameObjectSettingsForMap()
{
	//Planes
	CBackGunMachineGun8mm::RemoveBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::RemoveBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::RemoveBackGunDoubleMachineGun13mm();
	CDauntless::RemoveDauntless();
	CWeaponAircraftMachineGun13mm::RemoveWeaponAircraftMachineGun13mm();
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
	CShipDualMachineGun::RemoveShipDualMachineGun();
	//Ships
	CUSSVictory::RemoveUSSVictory();
	CAmericanLCT::RemoveAmericanLCT();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
