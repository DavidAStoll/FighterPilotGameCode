/*
 ============================================================================
 Name		: Tutorial1Map.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTutorial2Map implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Maps/CTutorial2Map.h"
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/utility/CLocalizationEngine.h"

#define KTutorial2Section1 "Tutorial2Lesson1"
#define KTutorial2Section2 "Tutorial2Lesson2"
#define KTutorial2Section3 "Tutorial2Lesson3"
#define KTutorial2Section4 "Tutorial2Lesson4"
#define KTutorial2Section5 "Tutorial2Lesson5"
#define KTutorial2Section6 "Tutorial2Lesson6"
#define KTutorial2Section7 "Tutorial2Lesson7"
#define KTutorial2Section8 "Tutorial2Lesson8"
#define KTutorial2Section9 "Tutorial2Lesson9"
#define KTutorial2Section10 "Tutorial2Lesson10"
#define KTutorial2Section11 "Tutorial2Lesson11"
#define KTutorial2Section11Completed "Tutorial2Lesson11Completed"

CTutorial2Map::CTutorial2Map()
:CBasicTutorialMap(EGameMapTutorialMission2,TUTORIAL_MAP2_SIZE, TUTORIAL_MAP2_INTERVALS, TUTORIAL_MAP2_MISSION_TIME_IN_MINS)
	{
		iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2UseFireButtonsPractice;
	}

CTutorial2Map::~CTutorial2Map()
	{
	}

CTutorial2Map* CTutorial2Map::New()
{
	CTutorial2Map* lSelf = new CTutorial2Map();
	lSelf->Construct();
	return lSelf;
}

void CTutorial2Map::TutorialMethodCall(TTutorialMethodCallIds aCallId)
{
	switch(aCallId)
	{
		case ETutorialMethodCallIdTutorial2UseFireButtonsScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall == ETutorialMethodCallIdTutorial2UseFireButtonsScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section1);
			}
			else
			{
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2UseFireButtonsScreen;
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
		case ETutorialMethodCallIdTutorial2UseFireButtonsPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(0, 250));
			BasicSetup(lPlane, ETutorialMethodCallIdTutorial2UseFireButtonsScreen);
			//if complete go this screen
			iEvents->Append(new CWeaponsTutorialFireButtons(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2Use1000KgBombScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2Use1000KgBombScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2Use1000KgBombScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section1);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2Use1000KgBombScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section2);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_Use1000KgBomb");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsUse1000Kg;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2Use1000KgBombPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2Use500KgBombScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2Use1000KgBombPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-1500, 200));
			lPlane->GetSecondaryWeapon()->SetAmmunitionToValue(6);
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add several Objects that can be used for target practice -------------------
			//some Guns
			CGameObject* lGameObject = CJapaneseAAConcrete::New(-2000);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2050);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2185);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAAConcrete::New(-2300);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			//add some squads
			TInt lSquadOffset = 0;
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
			CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-2350 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2400 - lSquadOffset, true);
			//add COmStation and 88s
			lGameObject = CJapaneseAA75mmType88::New(-2550 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			iGameObjectManager->AddGameObject(CComStation::New(-2650 - lSquadOffset, EConflictSideJapanese));
			lGameObject = CJapaneseAA75mmType88::New(-2800 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2850 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2900 - lSquadOffset, true);

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2Use1000KgBombScreen));
			iEvents->Append(new CWeaponsTutorialBomb1000kg(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2Use500KgBombScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2Use500KgBombScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2Use500KgBombScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section2);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2Use500KgBombScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section3);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_Use500KgBomb");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsUse500Kg;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2Use500KgBombPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2Use100KgBombScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2Use500KgBombPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-1500, 200));
			lPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifier500kgBomb);
			lPlane->GetSecondaryWeapon()->SetAmmunitionToValue(6);
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add several Objects that can be used for target practice -------------------
			//some Guns
			CGameObject* lGameObject = CJapaneseAAConcrete::New(-2000);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2050);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2185);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAAConcrete::New(-2300);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			//add some squads
			TInt lSquadOffset = 0;
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
			CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-2350 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2400 - lSquadOffset, true);
			//add COmStation and 88s
			lGameObject = CJapaneseAA75mmType88::New(-2550 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			iGameObjectManager->AddGameObject(CComStation::New(-2650 - lSquadOffset, EConflictSideJapanese));
			lGameObject = CJapaneseAA75mmType88::New(-2800 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2850 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2900 - lSquadOffset, true);

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2Use500KgBombScreen));
			iEvents->Append(new CWeaponsTutorialBomb1000kg(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2Use100KgBombScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2Use100KgBombScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2Use100KgBombScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section3);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2Use100KgBombScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section4);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_Use100KgBomb");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsUse100Kg;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2Use100KgBombPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2UseClusterBombScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2Use100KgBombPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-1500, 200));
			lPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifier100kgBomb);
			lPlane->GetSecondaryWeapon()->SetAmmunitionToValue(20);
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add several Objects that can be used for target practice -------------------
			//some Guns
			CGameObject* lGameObject = CJapaneseAAConcrete::New(-2000);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2050);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2185);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAAConcrete::New(-2300);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			//add some squads
			TInt lSquadOffset = 0;
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
			CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-2350 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2400 - lSquadOffset, true);
			//add COmStation and 88s
			lGameObject = CJapaneseAA75mmType88::New(-2550 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			iGameObjectManager->AddGameObject(CComStation::New(-2650 - lSquadOffset, EConflictSideJapanese));
			lGameObject = CJapaneseAA75mmType88::New(-2800 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2850 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2900 - lSquadOffset, true);

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2Use100KgBombScreen));
			iEvents->Append(new  CWeaponsTutorialBomb1000kg(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2UseClusterBombScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2UseClusterBombScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2UseClusterBombScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section4);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2UseClusterBombScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section5);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_UseClusterBomb");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsUseClusterBomb;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2UseClusterBombPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2UseClusterBombPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-1500, 200));
			lPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierClusterBomb);
			lPlane->GetSecondaryWeapon()->SetAmmunitionToValue(6);
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add several Objects that can be used for target practice -------------------
			//some Guns
			CGameObject* lGameObject = CJapaneseAAConcrete::New(-2000);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2050);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAA20mmMachineCannon::New(-2185);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lGameObject = CJapaneseAAConcrete::New(-2300);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			//add some squads
			TInt lSquadOffset = 0;
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
			CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(-2350 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2400 - lSquadOffset, true);
			//add COmStation and 88s
			lGameObject = CJapaneseAA75mmType88::New(-2550 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			iGameObjectManager->AddGameObject(CComStation::New(-2650 - lSquadOffset, EConflictSideJapanese));
			lGameObject = CJapaneseAA75mmType88::New(-2800 - lSquadOffset);
			lGameObject->AssignConflictSide(EConflictSideAmerican);
			iGameObjectManager->AddGameObject(lGameObject);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2850 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-2900 - lSquadOffset, true);

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new  CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2UseClusterBombScreen));
			iEvents->Append(new  CWeaponsTutorialBomb1000kg(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section5);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section6);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_UseMachineGunToKillSoldiers");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsKillSoldiersUseMachineGun;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2ProtectSoldiersScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-1500, 200));
			lPlane->GetSecondaryWeapon()->SetAmmunitionToValue(0);
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add Squads to be used for practice -------------------
			//add some squads
			TInt lSquadOffset = 0;
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-1700 - lSquadOffset, true);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-1750 - lSquadOffset, true);

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new  CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen));
			iEvents->Append(new  CWeaponsTutorialKillSoldiersUsingMachineGun(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2ProtectSoldiersScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2ProtectSoldiersScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2ProtectSoldiersScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section6);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2ProtectSoldiersScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section7);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_ProtectSoldiers");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsProtectSoldiers;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2ProtectSoldiersPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2DiveBombingScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2ProtectSoldiersPractice:
		{
			ClearMap();
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(400, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iPlayer->SpawnNewPlayerDelayed();
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add Squads to be used for practice -------------------
			//create American Landing Order
			SGroundUnitOrder lJapaneseAttackOrder;
			lJapaneseAttackOrder.ShouldProtectArea = false;
			lJapaneseAttackOrder.MoveToLocation = true;
			lJapaneseAttackOrder.XLocationDestination = -50;

			//add Japanese attack squads
			TInt lSquadOffset = 0;
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-1300 - lSquadOffset, true)->GiveOrder(lJapaneseAttackOrder);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-1350 - lSquadOffset, true)->GiveOrder(lJapaneseAttackOrder);
			lSquadOffset += CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
			CJapaneseSquad::CreateJapaneseSoldierSquad(-1400 - lSquadOffset, true)->GiveOrder(lJapaneseAttackOrder);
			//add American defence sqaud
			CAmericanSquad::CreateAmericanSoldierSquad(-50, false)->GiveOrder(lJapaneseAttackOrder);

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new  CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2ProtectSoldiersScreen));
			iEvents->Append(new  CWeaponsTutorialLoseIfAllAmericanSoldiersDied(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2ProtectSoldiersScreen));
			iEvents->Append(new  CWeaponsTutorialKillSoldiersUsingMachineGun(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2DiveBombingScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2DiveBombingScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2DiveBombingScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section7);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2DiveBombingScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section8);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_DiveBombing");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsDiveBombing;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2DiveBombingPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2AttackBomberScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2DiveBombingPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(6), true, TPoint(-1100, 1300));
			lPlane->GetSecondaryWeapon()->SetAmmunitionToValue(1);
			ClearMap();
			iGameObjectManager->AddMoveableGameObject(lPlane);
			iMapAlreadyInitilized = true;
			iPlayer = CPlayer::New();
			iPlayer->AssignAircarft(lPlane);
			//create home Carrier
			CUSSVictory* lHomeCarrier = CUSSVictory::New(1000, false);
			iPlayer->AssignPlayerBase(lHomeCarrier);
			iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
			//----------------------- add Squads to be used for practice -------------------
			//add some squads
			iGameObjectManager->AddMoveableGameObject(CJapanese97ChiHaTank::New(-2000, TIntFloat::Convert(0), true));

			//add floor
			iGameObjectManager->AddGameObject(CFloorLand::New(-4900, 4900));
			iGameObjectManager->AddGameObject(CFloorWater::New(0, 4900));
			//adjust zoom to center view
			iPlayer->ChangeZoomMode(0);
			iEvents->Append(new  CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2DiveBombingScreen));
			iEvents->Append(new  CWeaponsTutorialLoseIfFailedToKillAnything(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2DiveBombingScreen));
			iEvents->Append(new  CWeaponsTutorialDiveBombingKill(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2AttackBomberScreen, ETutorialMethodCallIdTutorial2DiveBombingScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2AttackBomberScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2AttackBomberScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section8);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2AttackBomberScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section9);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_AttackBomber");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsAttackBomber;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2AttackBomberPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2AttackFighterScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2AttackBomberPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(-2000, 1700));
			lPlane->EquipWithNewSecondaryWeapon(EWeaponStandardRocket);
			BasicSetup(lPlane, ETutorialMethodCallIdTutorial2AttackBomberScreen);
			iGameObjectManager->AddMoveableGameObject(CBetty::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-2500, 1500)));
			//if complete go this screen
			iEvents->Append(new  CWeaponsTutorialAttackBomber(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2AttackFighterScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorial2AttackFighterScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2AttackFighterScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section9);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2AttackFighterScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section10);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_AttackDiveBomber");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsAttackFighter;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2AttackFighterPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2AttackShipScreen;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2AttackFighterPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(12), true, TPoint(-2000, 1000));
			lPlane->EquipWithNewSecondaryWeapon(EWeaponStandardRocket);
			BasicSetup(lPlane, ETutorialMethodCallIdTutorial2AttackFighterScreen);
			CVal* lEnemyPlane = CVal::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(-2500, 900));
			lEnemyPlane->GetSecondaryWeapon()->SetAmmunitionToValue(0); //no secondary weapons in this level
			lEnemyPlane->GetFighterAI()->SetFighterAIToRetreatToBase();
			iGameObjectManager->AddMoveableGameObject(lEnemyPlane);
			//if complete go this screen
			iEvents->Append(new  CWeaponsTutorialAttackFighter(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2AttackShipScreen));
			GoToGameView();
			break;
		}

		case ETutorialMethodCallIdTutorial2AttackShipScreen:
		{
			//user Stat
			if(iPreviousTutorialMethodCall != ETutorialMethodCallIdTutorial2AttackShipScreen)
			{
				CUserStats::TutorialSection(KTutorial2Section10);
				iPreviousTutorialMethodCall = ETutorialMethodCallIdTutorial2AttackShipScreen;
			}
			else
			{
				CUserStats::TutorialSection(KTutorial2Section11);
			}
			CUserStats::TutorialSectionInit();

			//Tutorial Section Data
			iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_TorpedoAttack");
			iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsTorpedoAttack;
			iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2AttackShipPractice;
			iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorialComplete;
			GoToTutorialView();
			break;
		}
		case ETutorialMethodCallIdTutorial2AttackShipPractice:
		{
			CDauntless* lPlane = CDauntless::New(false, TIntFloat::Convert(8), true, TPoint(-200, 150));
			lPlane->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
			BasicSetup(lPlane, ETutorialMethodCallIdTutorial2AttackShipScreen);
			SGroundUnitOrder lDummyOrder;
			CJapaneseLCT* lLCT = CJapaneseLCT::New(-2000, false, EJapaneseLCTTypeTankAndTroops, lDummyOrder);
			lLCT->SetSpeed(TIntFloat::Convert(0));
			iGameObjectManager->AddMoveableGameObject(lLCT);
			//if complete go this screen
			iEvents->Append(new  CWeaponsTutorialAttackShip(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorialComplete));
			iEvents->Append(new  CWeaponsTutorialLoseIfFailedToKillAnything(iPlayer->GetCurrentAircaft(), ETutorialMethodCallIdTutorial2AttackShipScreen));
			GoToGameView();
			break;
		}
		case ETutorialMethodCallIdTutorialComplete:
		{
			//user Stat
			CUserStats::TutorialSection(KTutorial2Section11);
			CUserStats::TutorialSectionInit();

			//all done
			GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
			EndMission();
			break;
		}
		default:
		{
			GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
			EndMission();
			break;
		}
	}
}

void CTutorial2Map::InitializeMap()
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
		iTutorialScreenData.DescriptionStringId = new CString("Frontend.Tutorials.Tutorial2_Instructions_FireButtons");
		iTutorialScreenData.TutorialImage = EImageIdTutorialWeaponsFireButtons;
		iTutorialScreenData.MethodCallIdIfContinue = ETutorialMethodCallIdTutorial2UseFireButtonsPractice;
		iTutorialScreenData.MethodCallIdIfSkip = ETutorialMethodCallIdTutorial2Use1000KgBombScreen;
		GoToTutorialView();
    }
}

void CTutorial2Map::InitializeGameObjectSettingsForMap()
{
	//need Tutorial Screen Image
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialScreen);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedDescriptionBackground);
	//Need images For Tutorial Lessons
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsAttackBomber);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsAttackFighter);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsDiveBombing);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsKillSoldiersUseMachineGun);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsProtectSoldiers);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsTorpedoAttack);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsUse100Kg);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsUse500Kg);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsUse1000Kg);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsUseClusterBomb);
	CGame::Game->iImageStore->LoadImage(EImageIdTutorialWeaponsFireButtons);

	//Planes
	CSmoke::InitilizeSmoke();
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

void CTutorial2Map::RemoveGameObjectSettingsForMap()
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
	CSmoke::RemoveSmoke();
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
