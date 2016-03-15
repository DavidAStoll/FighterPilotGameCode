/*
 * CMainMenuExitView.cpp
 *
 *  Created on: Jul 8, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CMainMenuExitView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/core/views/transitions/CTransitionSlideOnTop.h"
#include "includes/core/utility/CAssetDirectory.h"
#include "includes/core/utility/CSystem.h"

CMainMenuExitView::CMainMenuExitView()
: CBasicView()
{
	iSelected = ESelectNone;
	iIsPlayingVideo = false;
	iFrame = 0;
}

CMainMenuExitView::~CMainMenuExitView()
{
	if (iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}

	//delete Strings
	delete iYesString;
	delete iNoString;
	delete iConfirmationMessage;
}

CMainMenuExitView* CMainMenuExitView::New()
{
	CMainMenuExitView* lSelf = new CMainMenuExitView();
	lSelf->Construct();
	return lSelf;
}

void CMainMenuExitView::Construct()
{
	//Strings
	iYesString = iLocalizationEngine->GetString("Frontend.MainMenuExit.Yes");
	iNoString = iLocalizationEngine->GetString("Frontend.MainMenuExit.No");
	iConfirmationMessage = iLocalizationEngine->GetString("Frontend.MainMenuExit.ConfirmMessage");

	//find proper Fonts
	//menu selection
	CPointerArray<CString>* lMenuSelectionStrings = CPointerArray<CString>::New();
	lMenuSelectionStrings->Append(iYesString);
	lMenuSelectionStrings->Append(iNoString);
	lMenuSelectionStrings->Append(iConfirmationMessage);
	iFont = CGame::Game->iGraphicsFont->CreateFontThatFitsHorizontalStrings(lMenuSelectionStrings, 900, 40);
	//not needed anymore
	lMenuSelectionStrings->Clear();
	delete lMenuSelectionStrings;

	//create Hitboxes for options
	iYesRect = TRect(TPoint(62, 136), 340, 50);
	iNoRect = TRect(TPoint(600, 136), 340, 50);

	//background Texture
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0),
			&CGame::Game->iImageStore->GetImage(EImageIdFrontendMainMenuBackgroundExitConfirmationScreen), 1024, 660, 100, 0,
			1, 0, 1);
}

//------------------------- functions ------------------------//

void CMainMenuExitView::ChildConsumeTouchEvent(const STouchEventLocationConverted& iTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (iTouchEvent.FingerId == 0)
	{
		switch (iTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			case ETouchEventHandlerTouchTypeDrag:
			{
				if(iIsPlayingVideo)
				{
					if(iFrame > (GTRIALER_CLIP_SECS_UNTIL_PLAYER_CAN_CANCEL * GFRAMES_PER_SECOND_FRONTEND))
					{
						//exit game
						CGame::Game->ExitGame();
					}
					else
					{
						//not allowed to abort yet
						return;
					}
				}
				else
				{
					if (iYesRect.Contains(iTouchEvent.VirtualLocation))
					{
						iSelected = ESelectYes;
					}
					else if (iNoRect.Contains(iTouchEvent.VirtualLocation))
					{
						iSelected = ESelectNo;
					}
					else
					{
						iSelected = ESelectNone;
					}
				}

				break;
			}

			case ETouchEventHandlerTouchTypeUp:
			{
				if (iSelected == ESelectYes && iYesRect.Contains(iTouchEvent.VirtualLocation))
				{

#if GGAME_FULL_VERSION
					//save Game stuff and exit
					CGame::Game->ExitGame();
#else
					CGame::Game->ExitGame();

					/*
					//need to turn off sound
					CGame::Game->iAudioEngine->TurnAudioOff();

					//play trailer
					//Need to find out if we need to silence the video
					TInt lVideoVolume = 100;
					if(CSystem::GetIsSilentModeActivated())
						lVideoVolume = 0;

					CAssetDirectory::PlayVideoInAssetDirectoryNative("Trailer.mp4", lVideoVolume);
					iIsPlayingVideo = true;
					*/
#endif
					return;
				}
				else if (iSelected == ESelectNo && iNoRect.Contains(iTouchEvent.VirtualLocation))
				{
					//go back to main menu
					CTransitionSlideOnTop::New(&CGame::Game->iView, this, CMainMenuView::New(false), 0.5, true);
				}
				else
				{
					iSelected = ESelectNone;
				}

				break;
			}
		}
	}
}

void CMainMenuExitView::ChildViewDrawMethod()
{
	if(iIsPlayingVideo)
		iFrame++;

	iBackgroundTexture->AssignTexture(&iImageStore->GetImage(EImageIdFrontendMainMenuBackgroundExitConfirmationScreen));
	iBackgroundTexture->DrawArrays();

	//draw Menu Selection
	iGraphicsFont->SetFont(iFont);
	TPoint lStartLocation(62, 316);

	//draw Confirmation Message
	iGraphicsFont->SetColourWhite();
	DrawCenteredString(lStartLocation, iFont, iConfirmationMessage, 900, 50);

	//4 bars in between
	lStartLocation.iY -= 60;
	lStartLocation.iY -= 60;
	lStartLocation.iY -= 60;
	lStartLocation.iY -= 60;

	//Yes
	if (iSelected == ESelectYes)
		iGraphicsFont->SetColourSkyBlue();
	else
		iGraphicsFont->SetColourWhite();
	DrawCenteredString(lStartLocation, iFont, iYesString, 340, 50);

	//No
	lStartLocation.iX = 600; //is to the right of the Yes option instead of below
	if (iSelected == ESelectNo)
		iGraphicsFont->SetColourSkyBlue();
	else
		iGraphicsFont->SetColourWhite();
	DrawCenteredString(lStartLocation, iFont, iNoString, 340, 50);

}



