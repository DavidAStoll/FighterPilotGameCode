/*
 ============================================================================
 Name		: Campaign3Map3.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map3 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign3Map3.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign3Map3::CCampaign3Map3()
:	CMap(EGameMapCampaign3Mission3,CAMPAIGN3_MAP3_SIZE, CAMPAIGN3_MAP3_INTERVALS, CAMPAIGN3_MAP3_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign3Map3::~CCampaign3Map3()
	{
	}

CCampaign3Map3* CCampaign3Map3::New()
	{
	CCampaign3Map3* self = new  CCampaign3Map3();
	self->Construct();
	return self;
	}

void CCampaign3Map3::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;
		
		CLoseIfObjectsAreDestroyedEvent* lLoseCondition = CLoseIfObjectsAreDestroyedEvent::New();
		CGetBonusPointsIfObjectsSurvive* lBonusEventAllShipsSurvive = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.AllAmericanShipsSurvived", CAMPAIGN3_MAP3_ALL_SHIPS_SURVIVE_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();

		//----------------------------- AMERICAN FLEET -------------------------- //
		
		//Right destroyer
		CShip* lAmericanShip = CAmericanDestroyer::New(7700,false);
		lBonusEventAllShipsSurvive->AddGameObject(lAmericanShip);
		iGameObjectManager->AddMoveableGameObject(lAmericanShip);
		//create home carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(5900,false);
		lBonusEventAllShipsSurvive->AddGameObject(lHomeCarrier);
		lLoseCondition->AddGameObject(lHomeCarrier);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//Middle destroyer
		lAmericanShip = CAmericanDestroyer::New(4600,false);
		lBonusEventAllShipsSurvive->AddGameObject(lAmericanShip);
		iGameObjectManager->AddMoveableGameObject(lAmericanShip);
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(2800, false);
		lBonusEventAllShipsSurvive->AddGameObject(lLexington);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//Left destroyer
		lAmericanShip = CAmericanDestroyer::New(1500,false);
		lBonusEventAllShipsSurvive->AddGameObject(lAmericanShip);
		iGameObjectManager->AddMoveableGameObject(lAmericanShip);
		
		//----------------------------- JAPANESE FLEET -------------------------- //
		
		CShip* lJapaneseShip = CJapaneseDestroyer::New(-1500,true);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(CJapaneseCarrierTaiho::New(-3024,true));
		lJapaneseShip = CJapaneseDestroyer::New(-4024,true);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		iGameObjectManager->AddMoveableGameObject(CJapaneseCarrierShokaku::New(-5551,true));
		lJapaneseShip = CJapaneseDestroyer::New(-6251,true);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		lJapaneseShip = CJapaneseCarrierShoho::New(-7775,true);
		lJapaneseShip->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_SHIP); //planes from this carrier only attack ships
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		lJapaneseShip = CJapaneseDestroyer::New(-8775,true);
		iGameObjectManager->AddMoveableGameObject(lJapaneseShip);
		
		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorWater::New(-12950, 25900));
		
		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseCondition);
		iEvents->Append(lBonusEventAllShipsSurvive);
		iEvents->Append(new  CWinIfAllJapaneseForcesAreDestroyed(CAMPAIGN3_MAP3_DESTROY_ALL_JAPANESE_FORCES_BONUS_SCORE));
		//Mitchell Event
		iEvents->Append(new  CSpawnMitchellEveryTimeInterval(60*5,-60,TIME_EVENT_INFINITE_TIME_EVENTS));
	}
}

void CCampaign3Map3::InitializeGameObjectSettingsForMap()
{
	//Planes
	CDauntless::InitilizeDauntless();
	CMitchell::InitilizeMitchell();
	CVal::InitilizeVal();
	//Buildings
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
	CAmericanDestroyer::InitilizeAmericanDestroyer();
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	CJapaneseCarrierShoho::InitilizeJapaneseCarrierShoho();
	CJapaneseCarrierShokaku::InitilizeJapaneseCarrierShokaku();
	CJapaneseCarrierTaiho::InitilizeJapaneseCarrierTaiho();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign3Map3::RemoveGameObjectSettingsForMap()
{
	//Planes
	CDauntless::RemoveDauntless();
	CMitchell::RemoveMitchell();
	CVal::RemoveVal();
	//Buildings
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
	CAmericanDestroyer::RemoveAmericanDestroyer();
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	CJapaneseCarrierShoho::RemoveJapaneseCarrierShoho();
	CJapaneseCarrierShokaku::RemoveJapaneseCarrierShokaku();
	CJapaneseCarrierTaiho::RemoveJapaneseCarrierTaiho();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
