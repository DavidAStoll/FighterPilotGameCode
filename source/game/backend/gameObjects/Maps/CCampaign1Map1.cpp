/*
 ============================================================================
 Name		: Campaign1Map1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CCampaign1Map1 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign1Map1.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign1Map1::CCampaign1Map1()
:CMap(EGameMapCampaign1Mission1,CAMPAIGN1_MAP1_SIZE,CAMPAIGN1_MAP1_INTERVALS, CAMPAIGN1_MAP1_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign1Map1::~CCampaign1Map1()
	{
	}

CCampaign1Map1* CCampaign1Map1::New()
{
	CCampaign1Map1* lSelf = new CCampaign1Map1();
	lSelf->Construct();
	return lSelf;
}

void CCampaign1Map1::InitializeMap()
{
  if(!iMapAlreadyInitilized)//too avoid double Initilization
    {
		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseCondition = CLoseIfObjectsAreDestroyedEvent::New();
		//Bonus
		CGetBonusPointsIfObjectsSurvive* lBonusEventLexigtonSurvives = new CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.LexingtonSurvived", CAMPAIGN1_MAP1_LEXINGTON_SURVIVED_BONUS_SCORE);

		iMapAlreadyInitilized = true;
		iPlayer = CPlayer::New();
		//---------- American Forces ------------///
		//create home Carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(3000,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		lLoseCondition->AddGameObject(lHomeCarrier);
		//Lexington
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(500,false);
		iGameObjectManager->AddMoveableGameObject(lLexington);

		//---------- Japanese Forces ------------//
		CJapaneseDestroyer* lDestroyerFront = CJapaneseDestroyer::New(-2800,true);
		lDestroyerFront->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_BUILDINGS);//little trick, don't want that it attacks ships
		iGameObjectManager->AddMoveableGameObject(lDestroyerFront);
		CJapaneseCarrierShoho* lShoho = CJapaneseCarrierShoho::New(-4000,true);
		lShoho->SetTarget(lLexington);
		iGameObjectManager->AddMoveableGameObject(lShoho);
		CJapaneseDestroyer* lDestroyerBack = CJapaneseDestroyer::New(-4700,true);
		lDestroyerBack->SetTargetPriority(GAMEOBJECT_TYPE_GROUP_BUILDINGS);//little trick, don't want that it attacks ships
		iGameObjectManager->AddMoveableGameObject(lDestroyerBack);

		//add floor - just a water map
		iGameObjectManager->AddGameObject(CFloorWater::New(-9900, 19800));

		//Events
		iEvents->Append(lLoseCondition);
		iEvents->Append(new CWinIfAllJapaneseForcesAreDestroyed(CAMPAIGN1_MAP1_DESTROY_JAPANESE_FORCES_BONUS_SCORE));
		iEvents->Append(lBonusEventLexigtonSurvives);
    }
}

void CCampaign1Map1::InitializeGameObjectSettingsForMap()
{
	//Planes
	CDauntless::InitilizeDauntless();
	CVal::InitilizeVal();
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
	CShipDualMachineGun::InitilizeShipDualMachineGun();
	CShipDual75mmGun::InitilizeShipDual75mmGun();
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	CAmericanCarrierLexington::InitilizeAmericanCarrierLexington();
	CJapaneseCarrierShoho::InitilizeJapaneseCarrierShoho();
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	//Floor
	CFloorWater::InitilizeFloorWater();
}

void CCampaign1Map1::RemoveGameObjectSettingsForMap()
{
	//Planes
	CDauntless::RemoveDauntless();
	CVal::RemoveVal();
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
	CShipDualMachineGun::RemoveShipDualMachineGun();
	CShipDual75mmGun::RemoveShipDual75mmGun();
	//Ships
	CUSSVictory::RemoveUSSVictory();
	CAmericanCarrierLexington::RemoveAmericanCarrierLexington();
	CJapaneseCarrierShoho::RemoveJapaneseCarrierShoho();
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	//Floor
	CFloorWater::RemoveFloorWater();
}
