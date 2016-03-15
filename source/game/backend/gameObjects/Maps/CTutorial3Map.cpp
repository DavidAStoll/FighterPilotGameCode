/*
 * CTutorial3Map.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Maps/CTutorial3Map.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/utility/CLocalizationEngine.h"

#define KTutorial3Section1 "Tutorial3Lesson1"
#define KTutorial3Section2 "Tutorial3Lesson2"
#define KTutorial3Section3 "Tutorial3Lesson3"
#define KTutorial3Section4 "Tutorial3Lesson4"
#define KTutorial3Section5 "Tutorial3Lesson5"
#define KTutorial3Section5Completed "Tutorial3Lesson5Completed"

CTutorial3Map::CTutorial3Map()
:CBasicTutorialMap(EGameMapTutorialMission3,TUTORIAL_MAP3_SIZE, TUTORIAL_MAP3_INTERVALS, TUTORIAL_MAP3_MISSION_TIME_IN_MINS)
	{
	// No implementation required
	}

CTutorial3Map::~CTutorial3Map()
	{
	}

CTutorial3Map* CTutorial3Map::New()
{
	CTutorial3Map* lSelf = new CTutorial3Map();
	lSelf->Construct();
	return lSelf;
}

void CTutorial3Map::TutorialMethodCall(TTutorialMethodCallIds aCallId)
{
	switch(aCallId)
	{
		case ETutorialMethodCallIdTutorial3ExplainHudScreen:
		{
			//user stat, a bit different since it is a special case
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial3_Instructions_GameHud");
			iTutorialScreenData.TutorialImage = EImageIdTutorialExtraTutorialHud;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial3ExplainAAScreen;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial3ExplainAAScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial3ExplainAAScreen:
		{
			//user stat, a bit different since it is a special case
			CUserStats::TutorialSection(KTutorial3Section1);
			CUserStats::TutorialSectionInit();


			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial3_Instructions_AAGuns");
			iTutorialScreenData.TutorialImage = EImageIdTutorialExtraTutorialAAGun;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial3ExplainAirportScreen;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial3ExplainAirportScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial3ExplainAirportScreen:
		{
			//user stat, a bit different since it is a special case
			CUserStats::TutorialSection(KTutorial3Section2);
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial3_Instructions_Airport");
			iTutorialScreenData.TutorialImage = EImageIdTutorialExtraAirport;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial3ExplainComStationScreen;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial3ExplainComStationScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial3ExplainComStationScreen:
		{
			//user stat, a bit different since it is a special case
			CUserStats::TutorialSection(KTutorial3Section3);
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial3_Instructions_ComStation");
			iTutorialScreenData.TutorialImage = EImageIdTutorialExtraComStation;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial3ExplainMusicPlayer;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial3ExplainMusicPlayer;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial3ExplainMusicPlayer:
		{
			//user stat, a bit different since it is a special case
			CUserStats::TutorialSection(KTutorial3Section4);
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial3_Instructions_MusicPlayer");
			iTutorialScreenData.TutorialImage = EImageIdTutorialExtraChangeSongScreen;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorialComplete;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorialComplete;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorialComplete:
		{
			//user stat, a bit different since it is a special case
			CUserStats::TutorialSection(KTutorial3Section5);
			CUserStats::TutorialSectionInit();


			//all done
			GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
			EndMission();
			break;
		}
		default:
		{
			//all done
			GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
			EndMission();
			break;
		}
	}
}

void CTutorial3Map::InitializeMap()
{
  if(!iMapAlreadyInitilized)//too avoid double Initilization
    {
		//user stat, a bit different since it is a special case
		CUserStats::TutorialSectionInit();

		iMapAlreadyInitilized = true;
		//Player Object
		CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(0, 1100));
		iGameObjectManager->AddMoveableGameObject(lPlane);
		iPlayer = CPlayer::New();
		iPlayer->AssignAircarft(lPlane);

		//create home Carrier
		CUSSVictory* lHomeCarrier = CUSSVictory::New(1000,false);
		iPlayer->AssignPlayerBase(lHomeCarrier);
		iGameObjectManager->AddMoveableGameObject(lHomeCarrier);

		//add the floors, should extend to the end of the map otherwise it will crash the game
		iGameObjectManager->AddGameObject(CFloorWater::New(-4900, 9800));

		//Tutorial Section Data
		iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial3_Instructions_GameHud");
		iTutorialScreenData.TutorialImage = EImageIdTutorialExtraTutorialHud;
		iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial3ExplainAAScreen;
		iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial3ExplainAAScreen;
		GoToTutorialView();
    }
}

void CTutorial3Map::InitializeGameObjectSettingsForMap()
{
	//need Tutorial Screen Image
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialScreen);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedDescriptionBackground);
	//Need images For Tutorial Lessons
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialExtraAirport);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialExtraComStation);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialExtraTutorialAAGun);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialExtraChangeSongScreen);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialExtraTutorialHud);

	//Planes
	CDauntless::InitilizeDauntless();
	CVal::InitilizeVal();
	CBetty::InitilizeBetty();
	//Buildings
	CJapaneseAAConcrete::InitilizeJapaneseAAConcrete();
	CJapaneseAA75mmType88::InitilizeJapaneseAA75mmType88();
	CJapaneseAA20mmMachineCannon::InitilizeJapaneseAA20mmMachineCannon();
	CFlagPole::InitilizeFlagPole();
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
	CAmericanCarrierLexington::InitilizeAmericanCarrierLexington();
	CJapaneseCarrierShoho::InitilizeJapaneseCarrierShoho();
	CJapaneseDestroyer::InitilizeJapaneseDestroyer();
	CJapaneseLCT::InitilizeJapaneseLCT();
	//Floor
	CFloorWater::InitilizeFloorWater();
	CFloorLand::InitilizeFloorLand();
}

void CTutorial3Map::RemoveGameObjectSettingsForMap()
{
	//need Tutorial Screen Image
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialScreen);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedDescriptionBackground);
	//Need images For Tutorial Lessons
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialExtraAirport);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialExtraComStation);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialExtraTutorialAAGun);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialExtraChangeSongScreen);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialExtraTutorialHud);

	//Player stuff
	CHeavyBomberLocator::RemoveHeavyBomberLocator();
	CSmallPlaneLocator::RemoveSmallPlaneLocator();
	CBackgroundImage::RemoveBackgroundImage();
	CSpeedoMeter::RemoveSpeedoMeter();
	CHud::RemoveHud();
	//Planes
	CDauntless::RemoveDauntless();
	CVal::RemoveVal();
	CBetty::RemoveBetty();
	//Building
	CJapaneseAAConcrete::RemoveJapaneseAAConcrete();
	CJapaneseAA75mmType88::RemoveJapaneseAA75mmType88();
	CJapaneseAA20mmMachineCannon::RemoveJapaneseAA20mmMachineCannon();
	CFlagPole::RemoveFlagPole();
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
	CAmericanCarrierLexington::RemoveAmericanCarrierLexington();
	CJapaneseCarrierShoho::RemoveJapaneseCarrierShoho();
	CJapaneseDestroyer::RemoveJapaneseDestroyer();
	CJapaneseLCT::RemoveJapaneseLCT();
	//Floor
	CFloorWater::RemoveFloorWater();
	CFloorLand::RemoveFloorLand();
}


