/*
 ============================================================================
 Name		: Campaign2Map1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map1 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign2Map1.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign2Map1::CCampaign2Map1()
:CMap(EGameMapCampaign2Mission1,CAMPAIGN2_MAP1_SIZE,CAMPAIGN2_MAP1_INTERVALS, CAMPAIGN2_MAP1_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign2Map1::~CCampaign2Map1()
	{
	}

CCampaign2Map1* CCampaign2Map1::New()
	{
	CCampaign2Map1* self = new  CCampaign2Map1();
	self->Construct();
	return self;
	}

void CCampaign2Map1::InitializeMap()
{
  if(!iMapAlreadyInitilized)//too avoid double Initilization
    {
		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseCondition = CLoseIfObjectsAreDestroyedEvent::New();
		CLoseIfObjectLeavesMap* lLoseConditionIfTransportLeaves = new  CLoseIfObjectLeavesMap(1);
		CWinIfObjectsAreDestroyedEvent* lWinCondition = new  CWinIfObjectsAreDestroyedEvent("Frontend.BonusEvents.DestroyedAllTransport",CAMPAIGN2_MAP1_DESTROY_TRANSPORTS_BONUS_SCORE);
		CGetBonusPointsIfObjectsAreDestroyedEvent* lBonusCondition = new  CGetBonusPointsIfObjectsAreDestroyedEvent("Frontend.BonusEvents.DestroyedEntireConvoy", CAMPAIGN2_MAP1_DESTROY_ENTIRE_CONVOY_BONUS_SCORE);
		
		iMapAlreadyInitilized = true;
		iPlayer = CPlayer::New();
		//---------- American Forces ------------///
		TIntFloat lAmericanShipMoveLocation = TIntFloat::Convert(0); //location should be outsite the map
		//create home Carrier    
		CUSSVictory* lHomeCarrier = CUSSVictory::New(12000,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		lHomeCarrier->MoveToLocation(lAmericanShipMoveLocation);
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		lLoseCondition->AddGameObject(lHomeCarrier);
		//Lexington
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(10500,false);
		lLexington->MoveToLocation(lAmericanShipMoveLocation);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		
		//---------- Japanese Convoy ------------//
		//create some air cover
		CVal* lValPlane = CVal::New(true, TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(6000,200));
		lValPlane->GetSecondaryWeapon()->SetAmmunitionToValue(0); //should attack aircrafts only
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		lValPlane = CVal::New(true, TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(5000,200));
		lValPlane->GetSecondaryWeapon()->SetAmmunitionToValue(0); //should attack aircrafts only
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase();  //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		lValPlane = CVal::New(true, TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(2000,200));
		lValPlane->GetSecondaryWeapon()->SetAmmunitionToValue(0); //should attack aircrafts only
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase();  //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		
		SGroundUnitOrder lOrder;
		CShip* lJapaneseShip;
		TIntFloat lJapaneseShipMoveLocation = TIntFloat::Convert(-12000); //location should be outsite the map
		//Destroyer
		lJapaneseShip = CJapaneseDestroyer::New(8000,false);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lBonusCondition->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Transport Ship
		lJapaneseShip = CJapaneseTroopTransporter::New(7000,false,0,EJapaneseLCTTypeTroopsOnly,lOrder);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lWinCondition->AddGameObject(lJapaneseShip);
		lBonusCondition->AddGameObject(lJapaneseShip);
		lLoseConditionIfTransportLeaves->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Oil Tanker
		lJapaneseShip = CJapaneseOilTanker::New(6000,false);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lWinCondition->AddGameObject(lJapaneseShip);
		lBonusCondition->AddGameObject(lJapaneseShip);
		lLoseConditionIfTransportLeaves->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Transport Ship
		lJapaneseShip = CJapaneseTroopTransporter::New(5000,false,0,EJapaneseLCTTypeTroopsOnly,lOrder);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lWinCondition->AddGameObject(lJapaneseShip);
		lBonusCondition->AddGameObject(lJapaneseShip);
		lLoseConditionIfTransportLeaves->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Destroyer
		lJapaneseShip = CJapaneseDestroyer::New(4000,false);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lBonusCondition->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Transport Ship
		lJapaneseShip = CJapaneseTroopTransporter::New(3000,false,0,EJapaneseLCTTypeTroopsOnly,lOrder);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lWinCondition->AddGameObject(lJapaneseShip);
		lBonusCondition->AddGameObject(lJapaneseShip);
		lLoseConditionIfTransportLeaves->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Transport Ship
		lJapaneseShip = CJapaneseTroopTransporter::New(2000,false,0,EJapaneseLCTTypeTroopsOnly,lOrder);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lWinCondition->AddGameObject(lJapaneseShip);
		lBonusCondition->AddGameObject(lJapaneseShip);
		lLoseConditionIfTransportLeaves->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		//Destroyer
		lJapaneseShip = CJapaneseDestroyer::New(1000,false);
		lJapaneseShip->MoveToLocation(lJapaneseShipMoveLocation);
		lBonusCondition->AddGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		
		//add floor - just a water map
		iGameObjectManager->AddGameObject(CFloorWater::New(-14900, 29800));
		
		//Events
		iEvents->Append(lLoseCondition);
		iEvents->Append(lLoseConditionIfTransportLeaves);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusCondition);
		iEvents->Append(new  CSpawnMitchellEveryTimeInterval(60*4,0,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnMitchellEveryTimeInterval(60*4,60,TIME_EVENT_INFINITE_TIME_EVENTS));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*3,0,TIME_EVENT_INFINITE_TIME_EVENTS, GAMEOBJECT_TYPE_GROUP_PLANES, true));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*3,60,TIME_EVENT_INFINITE_TIME_EVENTS, GAMEOBJECT_TYPE_GROUP_PLANES, true));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*3,60*2,TIME_EVENT_INFINITE_TIME_EVENTS, GAMEOBJECT_TYPE_GROUP_PLANES, true));
    }
}

void CCampaign2Map1::InitializeGameObjectSettingsForMap()
{
	//Planes
	CDauntless::InitilizeDauntless();
	CVal::InitilizeVal();
	CMitchell::InitilizeMitchell();
	//Building
	CFlagPole::InitilizeFlagPole();
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
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	CJapaneseOilTanker::InitilizeJapaneseOilTanker();
	CJapaneseTroopTransporter::InitilizeJapaneseTroopTransporter();
	//Floor
	CFloorWater::InitilizeFloorWater();
}

void CCampaign2Map1::RemoveGameObjectSettingsForMap()
{
	//Planes
	CDauntless::RemoveDauntless();
	CVal::RemoveVal();
	CMitchell::RemoveMitchell();
	//Building
	CFlagPole::RemoveFlagPole();
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
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	CJapaneseOilTanker::RemoveJapaneseOilTanker();
	CJapaneseTroopTransporter::RemoveJapaneseTroopTransporter();
	//Floor
	CFloorWater::RemoveFloorWater();
}
