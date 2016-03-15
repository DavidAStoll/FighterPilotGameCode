/*
 ============================================================================
 Name		: Campaign3Map2.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map2 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign3Map2.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign3Map2::CCampaign3Map2()
:	CMap(EGameMapCampaign3Mission2,CAMPAIGN3_MAP2_SIZE, CAMPAIGN3_MAP2_INTERVALS, CAMPAIGN3_MAP2_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CCampaign3Map2::~CCampaign3Map2()
	{
	}

CCampaign3Map2* CCampaign3Map2::New()
	{
	CCampaign3Map2* self = new  CCampaign3Map2();
	self->Construct();
	return self;
	}

void CCampaign3Map2::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;
		
		CLoseIfObjectsAreDestroyedEvent* lLoseCondition = CLoseIfObjectsAreDestroyedEvent::New();
		CGetBonusPointsIfObjectsSurvive* lBonusEventLexigtonSurvives = new  CGetBonusPointsIfObjectsSurvive("Frontend.BonusEvents.LexingtonSurvived", CAMPAIGN3_MAP2_LEXINGTON_SURVIVES_BONUS_SCORE);
		
		//Player Object
		iPlayer = CPlayer::New();
		//------------------------ AMERICAN ATTACK PLANES -------------------------------//
		//create American Aircrafts, need 6, since we already start with 4
		//create American Aircraft - WAVE 1
		//CPU
		//create American Aircraft
		CDauntless* lDauntless = CDauntless::New(true,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-6100,400));
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(true,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-6000,300));
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(true,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-6100,200));
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		
		//create American Aircraft - WAVE 2
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(true,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-6700,400));
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(true,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-6600,300));
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		//CPU
		//create American Aircraft
		lDauntless = CDauntless::New(true,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(-6700,200));
		lDauntless->SetAIControlled();
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase();
		iGameObjectManager->AddMoveableGameObject(lDauntless);
		
		//------------------------ JAPANESE ATTACK PLANES -------------------------------//
		//------------------ Wave 1 --------------//
		//Val
		CVal* lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(14200,1000));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(14100,900));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(14000,800));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(14100,700));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(14200,600));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		
		//------------------ Wave 2 --------------//
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(15200,1000));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(15100,900));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(15000,800));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(15100,700));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		//Val
		lValPlane = CVal::New(false,TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED),true,TPoint(15200,600));
		lValPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
		lValPlane->GetFighterAI()->SetTargetAttackType(GAMEOBJECT_TYPE_GROUP_SHIP);
		lValPlane->GetFighterAI()->SetFighterAIToRetreatToBase(); //plane will figure out now base exists and start attacking planes
		iGameObjectManager->AddMoveableGameObject(lValPlane);
		
		//----------------------------- AMERICAN FLEET -------------------------- //
		
		//Right destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(900,true));
		//Lexginton
		CAmericanCarrierLexington* lLexington = CAmericanCarrierLexington::New(-900, false);
		lBonusEventLexigtonSurvives->AddGameObject(lLexington);
		iGameObjectManager->AddMoveableGameObject(lLexington);
		//Middle destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(-2200,false));
		//create home carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(-4000,false);
		lLoseCondition->AddGameObject(lHomeCarrier);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately();
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
		//Left destroyer
		iGameObjectManager->AddMoveableGameObject(CAmericanDestroyer::New(-5300,false));
		
		//--------------------- RIGHT BEACH ---------------------//
		iGameObjectManager->AddGameObject(CTree2::New(-8100, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8140, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8200, EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(-8270, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8310, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8370, EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(-8450, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-8520, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8560, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8620, EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CTree2::New(-8720, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-8820, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-8920, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9020, ETree2MiddleTreeSmaller)); //width 64
		CAmericanSquad::CreateAmericanSoldierAndTankSquad(-9100,true); //width 244
		iGameObjectManager->AddGameObject(CTree2::New(-9120, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9220, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9320, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9420, ETree2MiddleTreeSmallerReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9520, ETree2MiddleTree)); //width 64
		CAmericanSquad::CreateAmericanSoldierAATankSquad(-9600,true); //width 430
		iGameObjectManager->AddGameObject(CTree2::New(-9620, ETree2MiddleTreeReflected)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9720, ETree2MiddleTree)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9820, ETree2MiddleTreeSmaller)); //width 64
		iGameObjectManager->AddGameObject(CTree2::New(-9920, ETree2MiddleTree)); //width 64
		CAmericanSquad::CreateAmericanSoldierSquad(-10010,true); //width 400
		//----------------------- COM STATION AND AIRPORT-----------------------//
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-10200, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-10250, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-10350, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-10450, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-10550, EConflictSideAmerican)); //width 80
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-10580, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-10640, EConflictSideAmerican)); //width 32
		iGameObjectManager->AddGameObject(CComStation::New(-10750,EConflictSideAmerican,10,0)); //width 64, 53 to left
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-10900, EConflictSideAmerican)); //width 50
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-11050, EConflictSideAmerican)); //width 50
		CAirport* lAirport = CAirport::New(-12300,EConflictSideAmerican); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport); 
		CAmericanSquad::CreateAmericanTankSquad(-13100,true); //width 560
		lAirport = CAirport::New(-14200,EConflictSideAmerican); //widht to the left 175, witdh to the right 1050
		iGameObjectManager->AddGameObject(lAirport);
		
		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorWater::New(-8200, 24150));   //right  side 5000 - 13000
		iGameObjectManager->AddGameObject(CFloorLand::New(-15950, 7950));
		
		//---------------------- ADD EVENTS ---------------------//
		iEvents->Append(lLoseCondition);
		iEvents->Append(lBonusEventLexigtonSurvives);
		iEvents->Append(new  CWinIfJapaneseDestroyedAfterACertainTime(60*15, CAMPAIGN3_MAP2_DESTROY_ALL_JAPANESE_FORCES_BONUS_SCORE));
		//First Wave
		//Group 1
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,10,950,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,5,850,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,0,750,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,5,650,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,10,550,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		//Group 2
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,10,500,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,5,400,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,0,300,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,5,200,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,10,100,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		//Second Wave
		//Group 1
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 10,950,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 5,850,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30,750,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 5,650,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 10,550,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		//Group 2
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 10,500,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 5,400,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30,300,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 5,200,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,30 + 10,100,false, 7, GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP));
		//Third Wave
		//Group 1
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,50 + 10,950,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,50 + 5,850,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,50,750,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,50 + 5,650,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		iEvents->Append(new  CSpawnValEveryTimeInterval(60*2,50 + 10,550,false, 7, GAMEOBJECT_TYPE_GROUP_SHIP));
		//Betty Event
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(60*5,10,100,false, 3));
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(60*5,-1,0,false, 3));
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(60*5,20,0,false, 3));
		iEvents->Append(new  CSpawnBettyEveryTimeInterval(60*5,10,-100,false, 3));
		//Spawns dauntless
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60*2,0,0,7, GAMEOBJECT_TYPE_GROUP_PLANES, false));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60*2,10,0,7, GAMEOBJECT_TYPE_GROUP_PLANES, false));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60*2,20,0,7, GAMEOBJECT_TYPE_GROUP_PLANES, false));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60*2,30,0,7, GAMEOBJECT_TYPE_GROUP_PLANES, false));
		iEvents->Append(new  CSpawnDauntlessEveryTimeInterval(60*2,40,0,7, GAMEOBJECT_TYPE_GROUP_PLANES, false));
	}
}

void CCampaign3Map2::InitializeGameObjectSettingsForMap()
{
	//new
	CWeaponAircraftCannon20mm::InitilizeWeaponAircraftCannon20mm();
	CWeaponAircraftMachineGun13mm::InitilizeWeaponAircraftMachineGun13mm();
	CWeaponAircraftMachineGun8mm::InitilizeWeaponAircraftMachineGun8mm();
	CSmoke::InitilizeSmoke();
	CBackGunMachineGun8mm::InitilizeBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::InitilizeBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::InitilizeBackGunDoubleMachineGun13mm();
	CBulletCannon20mm::InitilizeBulletCannon20mm();
	CBulletRifle::InitilizeBulletRifle();

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
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign3Map2::RemoveGameObjectSettingsForMap()
{
	//new
	CWeaponAircraftCannon20mm::RemoveWeaponAircraftCannon20mm();
	CWeaponAircraftMachineGun13mm::RemoveWeaponAircraftMachineGun13mm();
	CWeaponAircraftMachineGun8mm::RemoveWeaponAircraftMachineGun8mm();
	CSmoke::RemoveSmoke();
	CBackGunMachineGun8mm::RemoveBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::RemoveBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::RemoveBackGunDoubleMachineGun13mm();
	CBulletCannon20mm::RemoveBulletCannon20mm();
	CBulletRifle::RemoveBulletRifle();

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
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
