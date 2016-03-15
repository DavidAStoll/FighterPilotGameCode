/*
 * CTutorial1Map.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Maps/CTutorial1Map.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/common/CUserStats.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
#include "includes/core/utility/CLocalizationEngine.h"

#define KTutorial1Section1 "Tutorial1Lesson1"
#define KTutorial1Section2 "Tutorial1Lesson2"
#define KTutorial1Section3 "Tutorial1Lesson3"
#define KTutorial1Section4 "Tutorial1Lesson4"
#define KTutorial1Section5 "Tutorial1Lesson5"
#define KTutorial1Section6 "Tutorial1Lesson6"
#define KTutorial1Section7 "Tutorial1Lesson7"
#define KTutorial1Section8 "Tutorial1Lesson8"
#define KTutorial1Section8Completed "Tutorial1Lesson8Completed"

CTutorial1Map::CTutorial1Map()
:CBasicTutorialMap(EGameMapTutorialMission1,TUTORIAL_MAP1_SIZE, TUTORIAL_MAP1_INTERVALS, TUTORIAL_MAP1_MISSION_TIME_IN_MINS)
	{
		iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1UpNavPractice;
	}

CTutorial1Map::~CTutorial1Map()
	{
	}

CTutorial1Map* CTutorial1Map::New()
{
	CTutorial1Map* lSelf = new CTutorial1Map();
	lSelf->Construct();
	return lSelf;
}

void CTutorial1Map::TutorialMethodCall(TTutorialMethodCallIds aCallId)
{
	switch(aCallId)
	{
		case ETutorialMethodCallIdTutorial1UpNavScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall == ETutorialMethodCallIdTutorial1UpNavScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section1);
			}
			else
			{
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1UpNavScreen;
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_Up");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsTurn360Degree;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1UpNavPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1TurnScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1UpNavPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(0, 1100));
			BasicSetup(lPlane,ETutorialMethodCallIdTutorial1UpNavScreen);
			//if complete go this screen
			iEvents->Append(new CControlsTutorialLooping(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial1TurnScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1TurnScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1TurnScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section1);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1TurnScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section2);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_LeftRight");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsTurnLeftAndRight;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1TurnPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1IncreaseSpeedScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1TurnPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(0, 1100));
			BasicSetup(lPlane,ETutorialMethodCallIdTutorial1TurnScreen);
			//if complete go this screen
			iEvents->Append(new CControlsTutorialTurns(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial1IncreaseSpeedScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1IncreaseSpeedScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1IncreaseSpeedScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section2);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1IncreaseSpeedScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section3);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_SpeedUp");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsSpeedUp;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1IncreaseSpeedPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1SlowDownScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1IncreaseSpeedPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(0, 900));
			BasicSetup(lPlane,ETutorialMethodCallIdTutorial1IncreaseSpeedScreen);
			//if complete go this screen
			iEvents->Append(new  CControlsTutorialSpeedUp(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial1SlowDownScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1SlowDownScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1SlowDownScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section3);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1SlowDownScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section4);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_SlowDown");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsSlowDown;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1SlowDownPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1SlowDownPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(-1000, 200));
			BasicSetup(lPlane,ETutorialMethodCallIdTutorial1SlowDownScreen);
			//if complete go this screen
			iEvents->Append(new  CControlsTutorialSlowDown(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed,ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownSuccess));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed://both the same, only description is slightly different
		case ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownSuccess:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed
					&& iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownSuccess)
			{
				CUserStats::TutorialSection(KTutorial1Section4);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section5);
			}
			CUserStats::TutorialSectionInit();

			if(aCallId == ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed)
			{
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed;
				iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_SlowDown_Failed");
			}
			else
			{
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownSuccess;
				iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_SlowDown_Success");
			}

			//Tutorial Section Data
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsZoomInAndOut;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1ZoomInAndOutPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1AvoidAAScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1ZoomInAndOutPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(2000, 400));
			BasicSetup(lPlane, iPreviousTutorialMethodCall);
			//if complete go this screen
			iEvents->Append(new  CControlsTutorialZoomInAndZoomOut(ETutorialMethodCallIdTutorial1AvoidAAScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1AvoidAAScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1AvoidAAScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section5);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1AvoidAAScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section6);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_AvoidAA");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsAvoidAA;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1AvoidAAPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1StartFromCarrierScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1AvoidAAPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-2010, 200));
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000,false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add AA Guns -------------------
			iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2000));
			iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2050));
			iGameObjectManager->AddGameObject(CJapaneseAA20mmMachineCannon::New(-2185));
			iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-2300));
			//add some 88s
			iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2500));
			iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2700));
			iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-2900));
			iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3100));
			//some concrete and 88s to finish it up
			iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3300));
			iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3500));
			iGameObjectManager->AddGameObject(CJapaneseAAConcrete::New(-3600));
			iGameObjectManager->AddGameObject(CJapaneseAA75mmType88::New(-3800));

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new  CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial1AvoidAAScreen));
			iEvents->Append(new  CControlsTutorialMoveToEndOfMap(iPlayer->GetCurrentAircaft(),ETutorialMethodCallIdTutorial1StartFromCarrierScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1StartFromCarrierScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1StartFromCarrierScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section6);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1StartFromCarrierScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section7);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_StartFromCarrier");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsStartFromCarrier;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1StartFromCarrierPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1LandOnCarrierScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1StartFromCarrierPractice:
		{
			ClearMap();
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000,false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			iPlayer->SpawnNewPlayerDelayed();

			//add floor
			iGameObjectManager->AddGameObject(CFloorWater::New(-4900, 9800));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new  CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial1StartFromCarrierScreen));
			iEvents->Append(new  CControlsTutorialStartFromCarrier(iPlayer->GetCurrentAircaft(),ETutorialMethodCallIdTutorial1LandOnCarrierScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial1LandOnCarrierScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial1LandOnCarrierScreen)
			{
				CUserStats::TutorialSection(KTutorial1Section7);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial1LandOnCarrierScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial1Section8);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_LandOnCarrier");
			iTutorialScreenData.TutorialImage = EImageIdTutorialControlsLanding;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1LandOnCarrierPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorialComplete;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial1LandOnCarrierPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(8), true, TPoint(3000, 300));
			BasicSetup(lPlane,ETutorialMethodCallIdTutorial1LandOnCarrierScreen);

			//if complete go this screen
			iEvents->Append(new  CControlsTutorialLandOnCarrier(iPlayer->GetCurrentAircaft(),ETutorialMethodCallIdTutorialComplete));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorialComplete:
		{
			CUserStats::TutorialSection(KTutorial1Section8);
			//all done
			GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
			EndMission();
			break;
		}
		default:
		{
			EndMission();
			break;
		}

	}
}

void CTutorial1Map::InitializeMap()
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

		//Start up with a Tutorial Section
		iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial1_Instructions_Up");
		iTutorialScreenData.TutorialImage = EImageIdTutorialControlsTurn360Degree;
		iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial1UpNavPractice;
		iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial1IncreaseSpeedScreen;
		GoToTutorialView();
    }
}

void CTutorial1Map::InitializeGameObjectSettingsForMap()
{
	//need Tutorial Screen Image
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialScreen);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedDescriptionBackground);
	//Need images For Tutorial Lessons
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsAvoidAA);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsLanding);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsSlowDown);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsSpeedUp);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsStartFromCarrier);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsTurn360Degree);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsTurnLeftAndRight);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialControlsZoomInAndOut);

	//Planes
	CBackGunMachineGun8mm::InitilizeBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::InitilizeBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::InitilizeBackGunDoubleMachineGun13mm();
	CSmoke::InitilizeSmoke();
	CDauntless::InitilizeDauntless();
	CVal::InitilizeVal();
	//Buildings
	CJapaneseAAConcrete::InitilizeJapaneseAAConcrete();
	CJapaneseAA75mmType88::InitilizeJapaneseAA75mmType88();
	CJapaneseAA20mmMachineCannon::InitilizeJapaneseAA20mmMachineCannon();
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
	CFloorLand::InitilizeFloorLand();
}

void CTutorial1Map::RemoveGameObjectSettingsForMap()
{
	//need Tutorial Screen Image
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialScreen);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedDescriptionBackground);
	//Need images For Tutorial Lessons
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsAvoidAA);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsLanding);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsSlowDown);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsSpeedUp);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsStartFromCarrier);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsTurn360Degree);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsTurnLeftAndRight);
	CGame::Game->iImageStore->UnloadImage(EImageIdTutorialControlsZoomInAndOut);

	//Player stuff
	CHeavyBomberLocator::RemoveHeavyBomberLocator();
	CSmallPlaneLocator::RemoveSmallPlaneLocator();
	CBackgroundImage::RemoveBackgroundImage();
	CSpeedoMeter::RemoveSpeedoMeter();
	CHud::RemoveHud();
	//Planes
	CBackGunMachineGun8mm::RemoveBackGunMachineGun8mm();
	CBackGunDoubleMachineGun8mm::RemoveBackGunDoubleMachineGun8mm();
	CBackGunDoubleMachineGun13mm::RemoveBackGunDoubleMachineGun13mm();
	CSmoke::RemoveSmoke();
	CDauntless::RemoveDauntless();
	CVal::RemoveVal();
	//Building
	CJapaneseAAConcrete::RemoveJapaneseAAConcrete();
	CJapaneseAA75mmType88::RemoveJapaneseAA75mmType88();
	CJapaneseAA20mmMachineCannon::RemoveJapaneseAA20mmMachineCannon();
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
	CFloorLand::RemoveFloorLand();
}

