/*
 * CBasicTutorialMap.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/basicClasses/CBasicTutorialMap.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
#include "includes/game/views/CTutorialView.h"
#include "includes/game/views/CGameView.h"
#include "includes/game/views/CLoadingScreen.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CBasicTutorialMap::CBasicTutorialMap(TGameMap aGameMapId, TUint aSize, TInt aNumberOfIntervals, TInt aExpectedMissionTimeInMins)
:CMap(aGameMapId, aSize, aNumberOfIntervals, aExpectedMissionTimeInMins)
{
	iTutorialScreenData.DescriptionStringId = NULL;
}

CBasicTutorialMap::~CBasicTutorialMap()
{
	if(iTutorialScreenData.DescriptionStringId != NULL)
	{
		delete iTutorialScreenData.DescriptionStringId;
		iTutorialScreenData.DescriptionStringId = NULL;
	}
}

const STutorialScreenData& CBasicTutorialMap::GetTutorialScreenData()
{
	return iTutorialScreenData;
}

void CBasicTutorialMap::TutorialMethodCall(TTutorialMethodCallIds aCallId)
{
	//stub
}

void CBasicTutorialMap::GoToTutorialView()
{
	CTransitionHorizontalFlip::New(&CGame::Game->iView, CGame::Game->iView, CTutorialView::New(), 1);
}

void CBasicTutorialMap::GoToGameView()
{
	PostMapInit();
	CTransitionHorizontalFlip::New(&CGame::Game->iView, CGame::Game->iView, CGameView::New(*CFighterPilotThePacificWar::FighterGame->iGameData), 1);
}

void CBasicTutorialMap::EndMission()
{
	CGameData* lGameData = CFighterPilotThePacificWar::FighterGame->iGameData;

	//need to make these calls before we end the mission
	GetPlayer()->UpdateStatsTrackerTimeValues();
	GetPlayer()->SetDeleteStatsTracker(false);
	EndMissionPreCall();

	//go back To Frontend
	CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, CGame::Game->iView, CLoadingScreen::New(lGameData->iCampaignData, lGameData->iMissionData, ELoadingScreenModeGoToVictoryScreen), 5, 4, 4, 30);
}

void CBasicTutorialMap::BasicSetup(CDauntless* aPlayerAircraft, TTutorialMethodCallIds aCallIdIfFailed)
{
	ClearMap();
	iGameObjectManager->AddMoveableGameObject(aPlayerAircraft);
	iMapAlreadyInitilized = true;
	iPlayer = CPlayer::New();
	iPlayer->AssignAircarft(aPlayerAircraft);
	//create home Carrier
	CUSSVictory* lHomeCarrier = CUSSVictory::New(1000,false);
	iPlayer->AssignPlayerBase(lHomeCarrier);
	iGameObjectManager->AddMoveableGameObject(lHomeCarrier);
	//add floor
	iGameObjectManager->AddGameObject(CFloorWater::New(-4900, 9800));
	//adjust zoom to center view
	iEvents->Append(new CRestartLastTutorialScreenIfDiedEvent(iPlayer->GetCurrentAircaft(), aCallIdIfFailed));
}

void CBasicTutorialMap::InitializeMap()
{
	//stub
}

void CBasicTutorialMap::InitializeGameObjectSettingsForMap()
{
	//stub
}

void CBasicTutorialMap::RemoveGameObjectSettingsForMap()
{
	//stub
}

