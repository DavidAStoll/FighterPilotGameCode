/*
 * CCompanyImageView.cpp
 *
 *  Created on: Sep 12, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CCompanyImageView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/game/views/CAgreeToTermsScreen.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionOpenDoor.h"

#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"
CCompanyImageView::CCompanyImageView()
{
	iInitDone = false;
	iHasBeenDrawnOnce = false;

}

CCompanyImageView::~CCompanyImageView()
{
	if(iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}
	iImageStore->UnloadImage(EImageIdCompanyImage);
}

CCompanyImageView* CCompanyImageView::New()
{
	CCompanyImageView* lSelf = new CCompanyImageView();
	lSelf->Construct();
	return lSelf;
}

void CCompanyImageView::Construct()
{
	iImageStore->LoadImage(EImageIdCompanyImage);
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdCompanyImage), 1024, 660, 0, 0, 1, 0, 1);
}

void CCompanyImageView::ChildConsumeTouchEvent(const STouchEventLocationConverted& iTouchEvent)
{
	//no need to handle input
}

void CCompanyImageView::ChildViewDrawMethod()
{
	iBackgroundTexture->DrawArrays();

	if(iIsReadyToHandleInput) //to avoid that it loads it twice
	{
		if(!iHasBeenDrawnOnce) //need to draw the view at least once
		{
			iHasBeenDrawnOnce = true;
		}
		else if(!iInitDone)
		{
			iInitDone = true;

			//start playing music
			CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->SetVolume(CFighterPilotThePacificWar::FighterGame->iGameVariables->iSoundEffectsPercentage);
			CGame::Game->iAudioEngine->GetMusicPlayer().SetVolume(CFighterPilotThePacificWar::FighterGame->iGameVariables->iMusicPercentage);
			CGame::Game->iAudioEngine->GetMusicPlayer().PlayGameSong(SONG_MENU_LOOP, true);
			//user Stats
			CUserStats::GameStarted();
			CUserStats::GameStartedTime(); //to record what time the game started
			CUserStats::RecordPhoneModel();
			CUserStats::UsesSilentProfile(); //find out if User has a silent profile enabeled
			//load Images
			iImageStore->LoadAllFrontendImages();

			if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iFirstTimeStartingUp)
			{
				CTransitionOpenDoor::New(&CGame::Game->iView, this, CAgreeToTermsScreen::New(false), 0.5);
			}
			else
			{
				CTransitionOpenDoor::New(&CGame::Game->iView, this,CMainMenuView::New(true), 0.5);
			}
			return;
		}
	}
}

