/*
 ============================================================================
 Name		: Campaign2Map2.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map2 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign2Map2.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign2Map2::CCampaign2Map2()
:CMap(EGameMapCampaign2Mission2,CAMPAIGN2_MAP2_SIZE,CAMPAIGN2_MAP2_INTERVALS, CAMPAIGN2_MAP2_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign2Map2::~CCampaign2Map2()
	{
	}

CCampaign2Map2* CCampaign2Map2::New()
	{
	CCampaign2Map2* self = new CCampaign2Map2();
	self->Construct();
	return self;
	}

void CCampaign2Map2::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectLeavesMap* lLoseCondition = new  CLoseIfObjectLeavesMap(1);
		//Win Conditions
		CWinIfObjectsAreDestroyedEvent* lWinCondition = new  CWinIfObjectsAreDestroyedEvent("Frontend.BonusEvents.DestroyedAllBetties",CAMPAIGN2_MAP2_DESTROY_BETTY_BONUS_SCORE);
		CGetBonusPointsIfObjectsAreDestroyedEvent* lBonusCondition = new  CGetBonusPointsIfObjectsAreDestroyedEvent("Frontend.BonusEvents.DestroyAllJapaneseForces", CAMPAIGN2_MAP2_DESTROY_ALL_JAPANESE_FORCES_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		iPlayer->ChangeLives( -iPlayer->GetLivesLeft()); //no extra lives in this mission
		
		//create American Aircraft - WAVE 1
		//CPU
		//create American Aircraft
		CDauntless* lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-1000,1500));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-900,1600));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-900,1400));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		
		//create American Aircraft - WAVE 2
		//Player Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-700,1500));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		iPlayer->AssignAircarft(lDauntless);
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-600,1600));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-600,1400));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-500,1700));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-500,1300));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//Extra Aircrafts
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-1200,1600));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-1200,1400));
		lDauntless->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		
		//create Japanese Aircraft - WAVE 1
		//CPU
		CVal* lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-3000,1500));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-3100,1600));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-3100,1400));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-3200,1700));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-3200,1300));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//BETTY that is being protected
		CBetty* lBetty = CBetty::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true,TPoint(-3400,1500));
		lBetty->RemoveBombs();
		lLoseCondition->AddGameObject(lBetty);
		lWinCondition->AddGameObject(lBetty);
		lBonusCondition->AddGameObject(lBetty);
		iGameObjectManager->AddMoveableGameObject(lBetty);
		
		//create Japanese Aircraft - WAVE 2
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-4000,1600));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-4100,1600));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//CPU
		lVal = CVal::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-4100,1400));
		lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase();
		lBonusCondition->AddGameObject(lVal);
		iGameObjectManager->AddMoveableGameObject(lVal);
		//BETTY that is being protected
		lBetty = CBetty::New(true,TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true,TPoint(-4400,1500));
		lBetty->RemoveBombs();
		lLoseCondition->AddGameObject(lBetty);
		lWinCondition->AddGameObject(lBetty);
		lBonusCondition->AddGameObject(lBetty);
		iGameObjectManager->AddMoveableGameObject(lBetty);
		
		//add some Forest on the floor
		iGameObjectManager->AddGameObject(CForest3::New(9000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(8000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(7000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(6000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(5000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(4000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(3000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(2000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(1000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(0)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-1000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-2000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-3000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-4000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-5000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-6000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-7000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-8000)); //Width width 700
		iGameObjectManager->AddGameObject(CForest3::New(-9000)); //Width width 700
		
		//Floor
		iGameObjectManager->AddGameObject(CFloorLand::New(-9900, 19800));
		
		//add Events
		iEvents->Append(lLoseCondition);
		iEvents->Append(lWinCondition);
		iEvents->Append(lBonusCondition);
	}
}

void CCampaign2Map2::InitializeGameObjectSettingsForMap()
{
	//Planes
	CDauntless::InitilizeDauntless();
	CVal::InitilizeVal();
	CBetty::InitilizeBetty();
	//Buildings
	CBigTree::InitilizeBigTree();
	CSmallTree::InitilizeSmallTree();
	CTree1::InitilizeTree1();
	CTree2::InitilizeTree2();
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
	//Floor
	CFloorLand::InitilizeFloorLand();
}

void CCampaign2Map2::RemoveGameObjectSettingsForMap()
{
	//Planes
	CDauntless::RemoveDauntless();
	CVal::RemoveVal();
	CBetty::RemoveBetty();
	//Buildings
	CBigTree::RemoveBigTree();
	CSmallTree::RemoveSmallTree();
	CTree1::RemoveTree1();
	CTree2::RemoveTree2();
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
	//Floor
	CFloorLand::RemoveFloorLand();
}
