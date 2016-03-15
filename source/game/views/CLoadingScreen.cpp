/*
 * CLoadingScreen.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CLoadingScreen.h"
#include "includes/game/views/CGameView.h"
#include "includes/game/views/CMissionVictoryView.h"
#include "includes/game/views/CTutorialView.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/views/CEndMissionTempTransitionView.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"
#include "includes/core/views/transitions/CTransitionCubeRotation.h"

CLoadingScreen::CLoadingScreen(SCampaign* aCampaign, SMission* aMission, TLoadingScreenMode aMode)
{
	iInitDone = false;
	iReady = false;
	iCampaign = aCampaign;
	iMission = aMission;
	iMode = aMode;
}

CLoadingScreen::~CLoadingScreen()
{
	delete iBackgroundTexture;
	iImageStore->UnloadImage(iRandomLoadImage);
}

CLoadingScreen* CLoadingScreen::New(SCampaign* aCampaign, SMission*aMission, TLoadingScreenMode aMode)
{
	CLoadingScreen* lSelf = new CLoadingScreen(aCampaign, aMission, aMode);
	lSelf->Construct();
	return lSelf;
}

void CLoadingScreen::Construct()
{
	//range is over all Loading screens
	TInt lUpperBound = EImageIdLoadingScreenPhilippinesShermanTank - EImageIdLoadingScreenAmericansOnGilberyBigExplosion;
	lUpperBound += EImageIdLoadingScreenAmericansOnGilberyBigExplosion; //add lower Bound
	iRandomLoadImage = static_cast<TImageId> (CGame::Game->iRandomGenerator->GetRandomNumberTInt(EImageIdLoadingScreenAmericansOnGilberyBigExplosion, lUpperBound + 1));

	iImageStore->LoadImage(iRandomLoadImage);
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(iRandomLoadImage), 1024, 660, 100, 0, 1, 0, 1);
}

void CLoadingScreen::ChildConsumeTouchEvent(const STouchEventLocationConverted& iTouchEvent)
{

}

void CLoadingScreen::ChildViewDrawMethod()
{
	iBackgroundTexture->DrawArrays();

	if(iReady)
	{
		if(!iInitDone)
		{
			iInitDone = true;

			if(iMode == ELoadingScreenModeStartMap || iMode == ELoadingScreenModeLoadMap)
			{
				//Change into Backend
				CGame::Game->iFramesPerSecondCounter->SetFramesPerSecond(GFRAMES_PER_SECOND_BACKEND);
				iImageStore->UnloadAllFrontendImages();

				if(iCampaign->TutorialCampaign)
				{
					CFighterPilotThePacificWar::FighterGame->iGameData->StartMap(iCampaign, iMission);
				}
				else
				{
					if(iMode == ELoadingScreenModeLoadMap)
					{
						CFighterPilotThePacificWar::FighterGame->iGameData->LoadGame(iCampaign, iMission);
					}
					else
					{
						CFighterPilotThePacificWar::FighterGame->iGameData->StartMap(iCampaign, iMission);
					}
					CTransitionHorizontalFlip::New(&CGame::Game->iView, this, CGameView::New(*CFighterPilotThePacificWar::FighterGame->iGameData), 1);
				}
			}
			else
			{
				//change Frames Per Second
				CGame::Game->iFramesPerSecondCounter->SetFramesPerSecond(GFRAMES_PER_SECOND_FRONTEND);

				//get data from Mission For Stats
				CGameData* lGameData = CFighterPilotThePacificWar::FighterGame->iGameData;
				TBool lMissionWon = lGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusSuccess;
				TBool lGameSave = lGameData->GetMap()->GetPlayer()->GetGameSaved();
				TInt lLivesLeft = lGameData->GetMap()->GetPlayer()->GetLivesLeft();
				TInt lScore = lGameData->GetMap()->GetPlayer()->GetPoints();
				TInt lExpectedMissionTimeInMins = lGameData->GetMap()->GetExpectedNumberOfMinutesForMap();
				CStatsTracker* lStatsTracker = lGameData->GetMap()->GetPlayer()->GetStatsTracker();
				//got everything we need, delete the map and unload its textures
				lGameData->DeleteMap();

				//load Frontend Images
				iImageStore->LoadAllFrontendImages();
				//go to Victory Screen
				CTransitionCubeRotation::New(&CGame::Game->iView, this, CEndMissionTempTransitionView::New(lMissionWon, lGameSave, lLivesLeft, lScore, lExpectedMissionTimeInMins, lStatsTracker), 1);
			}
			return;
		}
	}

	if(iIsReadyToHandleInput)
	{
		//to avoid that it loads it twice
		iReady = true;
		//load stuff
	}
}

