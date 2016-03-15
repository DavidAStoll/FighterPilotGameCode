/*
 ============================================================================
 Name		: Map1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCampaign1Map2 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/CCampaign1Map2.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/core/utility/CLocalizationEngine.h"

CCampaign1Map2::CCampaign1Map2() :
	CMap(EGameMapCampaign1Mission2,CAMPAIGN1_MAP2_SIZE, CAMPAIGN1_MAP2_INTERVALS, CAMPAIGN1_MAP2_MISSION_TIME_IN_MINS)
{
	// No implementation required
}

CCampaign1Map2::~CCampaign1Map2()
{
}

CCampaign1Map2* CCampaign1Map2::New()
{
	CCampaign1Map2* self = new CCampaign1Map2();
	self->Construct();
	return self;
}

//-------------functions ---------------

void CCampaign1Map2::InitializeMap()
{
	if(!iMapAlreadyInitilized)//too avoid double Initilization
	{
		iMapAlreadyInitilized = true;

		//Used to check for lose Condition
		CLoseIfObjectsAreDestroyedEvent* lLoseCondition = CLoseIfObjectsAreDestroyedEvent::New();
		CGetBonusPointsIfObjectsAreDestroyedEvent* lBonusEventDestroyAirport = new CGetBonusPointsIfObjectsAreDestroyedEvent("Frontend.BonusEvents.DestroyAirport", CAMPAIGN1_MAP2_DESTROY_AIRPORT_BONUS_SCORE);

		//Player Object
		iPlayer = CPlayer::New();
		
		//create home Carrier    
		CUSSVictory* lHomeCarrier = CUSSVictory::New(3000,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iPlayer->SpawnNewPlayerImmediately(); //creates new aircraft for player
		lLoseCondition->AddGameObject(lHomeCarrier);//if home base is destroyed lose the mission 
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);

		//Create Beach with Palm Trees and some concrete guns
		iGameObjectManager->AddGameObject(CTree2::New(900, ETree2BowedTreeSmaller));
		iGameObjectManager->AddGameObject(CTree2::New(700, ETree2MiddleTree));
		iGameObjectManager->AddGameObject(CTree2::New(500, ETree2BowedTreeReflected));
		iGameObjectManager->AddGameObject(CTree2::New(300, ETree2MiddleTreeReflected));
		iGameObjectManager->AddGameObject(CTree2::New(150, ETree2BowedTreeSmallerReflected));
		iGameObjectManager->AddGameObject(CTree2::New(0, ETree2MiddleTreeSmaller));
		iGameObjectManager->AddGameObject(CTree2::New(-100, ETree2MiddleTree));
		iGameObjectManager->AddGameObject(CTree2::New(-150, ETree2BowedTreeReflected));
		//two Concerete guns are positioned here
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(600));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(280));
		//two machine guns to protect them
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(450));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(100));

		//to fill the emptiness, add a small forest
		iGameObjectManager->AddGameObject(CForest3::New(-900));

		//Concrete Canon at big tree
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-1000));
		iGameObjectManager->AddGameObject(CBigTree::New(-1100, EBigTreeNormal));
		//tpye 88 cannons to protect it
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1150));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-1300));
		//Add forest to the end
		iGameObjectManager->AddGameObject(CForest2::New(-1700));

		//Add an airport
		CAirport* lAirport = CAirport::New(-2900, EConflictSideJapanese);
		lAirport->SetSpawnPlanes(false);
		lBonusEventDestroyAirport->AddGameObject(lAirport);
		iGameObjectManager->AddGameObject(lAirport);
		//do some damage to the airport so that it doesn't have so much live left. Player dones't need to attack it that often
		//8 of them
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2900,FLOOR_TEXTURE_HEIGHT)));
		//also damage hangars a bit
		//Hangar 1
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2500,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2500,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2500,FLOOR_TEXTURE_HEIGHT)));
		//Hangar 2
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2180,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2180,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-2180,FLOOR_TEXTURE_HEIGHT)));

		//Three Big trees with type 88 guns
		iGameObjectManager->AddGameObject(CBigTree::New(-3800, EBigTreeNormal));
		iGameObjectManager->AddGameObject(CBigTree::New(-4030, EBigTreeReflected));
		iGameObjectManager->AddGameObject(CBigTree::New(-4260, EBigTreeNormal));
		//the guns
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3650));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3880));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4100));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-4400));

		//Add Lonely small Tree
		iGameObjectManager->AddGameObject(CSmallTree::New(-4600, ESmallTreeNormal));

		//heavy fortified concrete guns and light machine guns for protection
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-4950));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5070));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5200));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5250));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-5300));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5400));
		iGameObjectManager->AddGameObject(CTree1::New(-5450, EBigThinTreeReflected));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-5600));
		iGameObjectManager->AddGameObject(CSmallTree::New(-5800, ESmallTreeNormal));
		
		//add a litte extra, not part of the real map1 map
		TInt lOffsetForAASquad = CJapaneseSquad::GetWidthOfJapaneseSoldierAATankSquad();
		CJapaneseSquad::CreateJapaneseSoldierAATankSquad(-5600 -50 - lOffsetForAASquad,true);

		//ComStation
		iGameObjectManager->AddGameObject(CForest3::New(-6600));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-6550));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6620));
		iGameObjectManager->AddGameObject(CComStation::New(-6700, EConflictSideJapanese));
		//damage it a bit so it is easier to destroy
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-6750,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-6750,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-6750,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CExplosion1000Kg::New(TPoint(-6750,FLOOR_TEXTURE_HEIGHT)));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-6860));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-6950));
		iGameObjectManager->AddGameObject(CForest1::New(-7300));

		//big Tree and concrete cannons, defended by type 88
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-7600));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-7700));
		iGameObjectManager->AddGameObject(CBigTree::New(-7830, EBigTreeNormal));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-7900));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8100));

		//two big trees, one concrete cannon defended heavily by two machine guns and two type 88
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8500));
		iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-8580));
		iGameObjectManager->AddGameObject(CBigTree::New(-8710, EBigTreeNormal));
		iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-8775));
		iGameObjectManager->AddGameObject(CBigTree::New(-8900, EBigTreeReflected));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-8960));
		iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-9060));

		//to finish it off, add the HQ
		CHq* lHq = CHq::New(-10170, EConflictSideJapanese);
		iGameObjectManager->AddGameObject(lHq);

		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorLand::New(-12950, 14000));
		iGameObjectManager->AddGameObject(CFloorWater::New(1050, 11850));

		//add Events, no events for now
		iEvents->Append(lLoseCondition);
		iEvents->Append(lBonusEventDestroyAirport);
		iEvents->Append(CCreateMitchellToAttackTargetClockTimer::New(lHq, 480));
	}
}

void CCampaign1Map2::InitializeGameObjectSettingsForMap()
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
	//Ships
	CUSSVictory::InitilizeUSSVictory();
	//Floor
	CFloorLand::InitilizeFloorLand();
	CFloorWater::InitilizeFloorWater();
}

void CCampaign1Map2::RemoveGameObjectSettingsForMap()
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
	//Ships
	CUSSVictory::RemoveUSSVictory();
	//Floor
	CFloorLand::RemoveFloorLand();
	CFloorWater::RemoveFloorWater();
}
