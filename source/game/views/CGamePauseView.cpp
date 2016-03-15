/*
 * CGamePauseView.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CGamePauseView.h"
#include "includes/game/views/CGameSettingsView.h"
#include "includes/game/views/CLoadingScreen.h"
#include "includes/game/common/CUserStats.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

#define NUMBER_OF_LABELS 7
#define NUMBER_OF_SLOTS (NUMBER_OF_LABELS+1) // add one to make look better

CGamePauseView::CGamePauseView(CGameData& aGameData)
:iGameData(aGameData)
{
	iHasSaveGame = false;
	iSelectedPauseMenuLabel = EPauseMenuNoneSelected;
	iDepthLevel = EPauseMenuDepthLevelTopSelection;
	iLoadGameLabel = EPauseMenuLoadGameLabelNo;
	iSaveGameLabel = EPauseMenuSaveGameLabelNo;
	iRestartMissionLabel = EPauseMenuRestartMissionLabelNo;
	iEndMissionLabel = EPauseMenuEndMissionLabelNo;
}

CGamePauseView::~CGamePauseView()
{
	delete iGameView;

	delete iStringReturnToGame;
	delete iStringRestartMission;
	delete iStringExitToMainMenu;
	delete iStringLoadGame;
	delete iStringSaveGame;
	delete iStringShowBattlefieldYes;
	delete iStringShowBattlefieldNo;
	delete iStringGameSettings;
	delete iStringYes;
	delete iStringNo;
	delete iStringConfirmContinue;
	delete iStringSaveGameWarning;
	delete iStringSaveGameSuccessMessage;
	delete iStringLoadGameWarning;
	delete iStringRestartGameWarning;
	delete iStringEndMissionWarning;
	delete iStringEndMissionStatus;
	delete iStringEndMissionFailed;
	delete iStringEndMissionPending;
	delete iStringEndMissionSuccess;
}

CGamePauseView* CGamePauseView::New(CGameData& aGameData)
{
	CGamePauseView* lSelf = new CGamePauseView(aGameData);
	lSelf->Construct();
	return lSelf;
}

void CGamePauseView::Construct()
{
	//pause Music And Sound
	CGame::Game->iAudioEngine->GetMusicPlayer().Pause();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Pause();

	//needed if we don't want to show the background image
	iGameView = CGameView::New(iGameData);

	//check if we have a SaveGame
	iHasSaveGame = iGameData.GetMap()->CheckIfSaveGameExists(iGameData.iMissionData->SaveGameFile->GetData());

	//need to load image from disk
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdHudPauseMenuBackground), 1024, 660, 100, 0, 1, 0, 1);

	//load Strings
	iStringReturnToGame = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.ReturnToGame");
	iStringRestartMission = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.RestartMission");
	iStringExitToMainMenu = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.ExitToMainMenu");
	iStringLoadGame = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.LoadGame");
	iStringSaveGame = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.SaveGame");
	iStringShowBattlefieldYes = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.ViewBattlefieldYes");
	iStringShowBattlefieldNo = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.ViewBattlefieldNo");
	iStringGameSettings = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.GameSettings");
	iStringYes = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.ConfirmYes");
	iStringNo = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.NoLabel");
	iStringConfirmContinue = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.ConfirmContinue");
	iStringSaveGameWarning = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.SaveGameWarning");
	iStringSaveGameSuccessMessage = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.SaveGameSuccessMessage");
	iStringLoadGameWarning = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.LoadGameWarning");
	iStringRestartGameWarning = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.RestartGameWarning");
	iStringEndMissionWarning = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.EndMissionWarning");
	iStringEndMissionStatus = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.EndMissionStatus");
	iStringEndMissionFailed = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.EndMissionStatusFailed");
	iStringEndMissionPending = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.EndMissionStatusPending");
	iStringEndMissionSuccess = iLocalizationEngine->GetString("Frontend.InGamePauseMenu.EndMissionStatusSuccess");

	//setup array
	CString** lStrings = new CString* [20];
	lStrings[0] = iStringReturnToGame;
	lStrings[1] = iStringRestartMission;
	lStrings[2] = iStringExitToMainMenu;
	lStrings[3] = iStringLoadGame;
	lStrings[4] = iStringSaveGame;
	lStrings[5] = iStringShowBattlefieldYes;
	lStrings[6] = iStringShowBattlefieldNo;
	lStrings[7] = iStringGameSettings;
	lStrings[8] = iStringYes;
	lStrings[9] = iStringNo;
	lStrings[10] = iStringConfirmContinue;
	lStrings[11] = iStringSaveGameWarning;
	lStrings[12] = iStringSaveGameSuccessMessage;
	lStrings[13] = iStringLoadGameWarning;
	lStrings[14] = iStringRestartGameWarning;
	lStrings[15] = iStringEndMissionWarning;
	lStrings[16] = iStringEndMissionStatus;
	lStrings[17] = iStringEndMissionFailed;
	lStrings[18] = iStringEndMissionPending;
	lStrings[19] = iStringEndMissionSuccess;

	iFont = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lStrings, 20, 900, 40);
	delete lStrings;

	//need to hardcode because gameView is not created at this point
	TInt lHeight = iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iHeight;
	TInt lWidth = iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iWidth;
	TInt lFontMaxHeight = iFont.HeightAndDescentOfFont.GetIntInBaseInt();
	TInt lOffset = lFontMaxHeight;
	TInt lSectionHeight = (lHeight - (lOffset * NUMBER_OF_SLOTS)) / (NUMBER_OF_SLOTS);

	TInt sizeOfReturnToGameString = iGraphicsFont->GetWidthOfString(iFont, iStringReturnToGame);
	TInt sizeOfRestartMissionString = iGraphicsFont->GetWidthOfString(iFont, iStringRestartMission);
	TInt sizeOfExitToMainMenuString = iGraphicsFont->GetWidthOfString(iFont, iStringExitToMainMenu);
	TInt sizeOfLoadGameString = iGraphicsFont->GetWidthOfString(iFont, iStringLoadGame);
	TInt sizeOfSaveGameString = iGraphicsFont->GetWidthOfString(iFont, iStringSaveGame);
	TInt sizeOfBattleFieldString = iGraphicsFont->GetWidthOfString(iFont, iStringShowBattlefieldYes);
	TInt sizeOfGameSettingsString = iGraphicsFont->GetWidthOfString(iFont, iStringGameSettings);

	//Set location of each Label
	TPoint label1, label2, label3, label4, label5, label6, label7;
	//------------------- Return to Game -----------------------//
	label1 = TPoint((lWidth - sizeOfReturnToGameString) / 2, (lSectionHeight * 7) + lOffset* 7);
	iReturnToGameRect = TRect(label1.iX, label1.iY + lFontMaxHeight, label1.iX + sizeOfReturnToGameString, label1.iY);
	//------------------- Load Game-----------------------//
	label2 = TPoint ((lWidth - sizeOfLoadGameString) / 2, (lSectionHeight * 6) + lOffset * 6);
	iLoadGameRect = TRect(label2.iX, label2.iY + lFontMaxHeight, label2.iX + sizeOfLoadGameString, label2.iY);
	//------------------- Save Game-----------------------//
	label3 = TPoint((lWidth - sizeOfSaveGameString) / 2, (lSectionHeight * 5) + lOffset * 5);
	iSaveGameRect = TRect(label3.iX, label3.iY + lFontMaxHeight, label3.iX + sizeOfSaveGameString, label3.iY);
	//------------------- Show Battlefield-----------------------//
	label4 = TPoint((lWidth - sizeOfBattleFieldString) / 2, (lSectionHeight * 4) + lOffset * 4);
	iShowBattlefieldRect = TRect(label4.iX, label4.iY + lFontMaxHeight, label4.iX + sizeOfBattleFieldString, label4.iY);
	//------------------- Game settins-----------------------//
	label5 = TPoint((lWidth - sizeOfGameSettingsString) / 2, (lSectionHeight * 3) + lOffset * 3);
	iGameSettingsRect = TRect(label5.iX, label5.iY + lFontMaxHeight, label5.iX + sizeOfGameSettingsString, label5.iY);
	//------------------- Restart Game -----------------------//
	label6 = TPoint((lWidth - sizeOfRestartMissionString) / 2, (lSectionHeight * 2) + lOffset * 2);
	iRestartMissionRect = TRect(label6.iX, label6.iY + lFontMaxHeight, label6.iX + sizeOfRestartMissionString, label6.iY);
	//------------------- End Game -----------------------//
	label7 = TPoint((lWidth - sizeOfExitToMainMenuString) / 2, (lSectionHeight * 1) + lOffset * 1);
	iEndMissionRect = TRect(label7.iX, label7.iY + lFontMaxHeight, label7.iX + sizeOfExitToMainMenuString, label7.iY);

	TInt sizeOfYesString = iGraphicsFont->GetWidthOfString(iFont, iStringYes);
	TInt sizeOfNoString = iGraphicsFont->GetWidthOfString(iFont, iStringNo);

	TPoint lYesLoadLabelPoint, lNoLoadLabelPoint;

	//Since these strings are generally very short and thus their hitbox is very small, we make them twice as big
	//---------------------- Load Yes ---------------------//
	lYesLoadLabelPoint = TPoint((lWidth) / 4, (lSectionHeight * 2) + lOffset * 2);// both at at the same height
	iLoadGameYesRect = TRect(lYesLoadLabelPoint.iX - sizeOfYesString, lYesLoadLabelPoint.iY + (lFontMaxHeight * 2), lYesLoadLabelPoint.iX + sizeOfYesString, lYesLoadLabelPoint.iY - lFontMaxHeight);//extend to the Left a bit
	//---------------------- Load No ---------------------//
	lNoLoadLabelPoint = TPoint(((lWidth * 3) / 4) - sizeOfNoString, (lSectionHeight * 2) + lOffset * 2);// both at at the same height
	iLoadGameNoRect = TRect(lNoLoadLabelPoint.iX, lNoLoadLabelPoint.iY + (lFontMaxHeight * 2), lNoLoadLabelPoint.iX + (sizeOfNoString * 2), lNoLoadLabelPoint.iY - lFontMaxHeight);//extend to the Right a bit

	//all have the same data
	iSaveGameYesRect = iLoadGameYesRect;
	iSaveGameNoRect = iLoadGameNoRect;
	iRestartMissionYesRect = iLoadGameYesRect;
	iRestartMissionNoRect = iLoadGameNoRect;

	TInt sizeOfEndMissionYesString = iGraphicsFont->GetWidthOfString(iFont, iStringYes);
	TInt sizeOfEndMissionNoString = iGraphicsFont->GetWidthOfString(iFont, iStringNo);

	TPoint lEndMissionYesPoint, lEndMissionNoPoint;
	lEndMissionYesPoint= TPoint((lWidth) / 4, (lSectionHeight * 2) + lOffset * 2);// both at at the same height
	iEndMissionYesRect = TRect(lEndMissionYesPoint.iX - sizeOfEndMissionYesString, lEndMissionYesPoint.iY + (lFontMaxHeight * 2), lEndMissionYesPoint.iX + sizeOfEndMissionYesString, lEndMissionYesPoint.iY - lFontMaxHeight);
	lEndMissionNoPoint= TPoint(((lWidth * 3) / 4) - sizeOfNoString, (lSectionHeight * 2) + lOffset * 2);// both at at the same height
	iEndMissionNoRect = TRect(lEndMissionNoPoint.iX, lEndMissionNoPoint.iY + (lFontMaxHeight * 2), lEndMissionNoPoint.iX + (sizeOfEndMissionNoString * 2), lEndMissionNoPoint.iY - lFontMaxHeight);
}

//-------------------- functions ---------------------//
void CGamePauseView::LoadImagesNeededForView()
{
	CGame::Game->iImageStore->LoadImage(EImageIdHudPauseMenuBackground);
}

void CGamePauseView::UnloadImagesNeededForView()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdHudPauseMenuBackground);
}

void CGamePauseView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	TPoint lTouchPosition = aTouchEvent.VirtualLocation;

	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			case ETouchEventHandlerTouchTypeDrag:
			{
				//find out what state we are in
				if(iDepthLevel == EPauseMenuDepthLevelTopSelection)
				{
					//check with rects in the top selection
					if(iReturnToGameRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuReturnToGameLabel;
					}else if(iHasSaveGame && iLoadGameRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuLoadGameLabel;
					}else if(iSaveGameRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuSaveGameLabel;
					}else if(iShowBattlefieldRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuShowBattlefieldLabel;
					}else if(iGameSettingsRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuGameSettingsLabel;
					}else if(iRestartMissionRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuRestartMissionLabel;
					}else if(iEndMissionRect.Contains(lTouchPosition))
					{
						iSelectedPauseMenuLabel = EPauseMenuExitToMainMenuLabel;
					}else
					{
						iSelectedPauseMenuLabel = EPauseMenuNoneSelected;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelLoadGameConfirm)
				{
					if(iLoadGameYesRect.Contains(lTouchPosition))
					{
						iLoadGameLabel = EPauseMenuLoadGameLabelYes;
					}else if(iLoadGameNoRect.Contains(lTouchPosition))
					{
						iLoadGameLabel = EPauseMenuLoadGameLabelNo;
					}else
					{
						iLoadGameLabel = EPauseMenuLoadGameLabelNoneSelected;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelSaveGameConfirm)
				{
					if(iSaveGameYesRect.Contains(lTouchPosition))
					{
						iSaveGameLabel = EPauseMenuSaveGameLabelYes;
					}else if(iSaveGameNoRect.Contains(lTouchPosition))
					{
						iSaveGameLabel = EPauseMenuSaveGameLabelNo;
					}else
					{
						iSaveGameLabel = EPauseMenuSaveGameLabelNoneSelected;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelRestartMissionConfirm)
				{
					if(iRestartMissionYesRect.Contains(lTouchPosition))
					{
						iRestartMissionLabel = EPauseMenuRestartMissionLabelYes;
					}else if(iRestartMissionNoRect.Contains(lTouchPosition))
					{
						iRestartMissionLabel = EPauseMenuRestartMissionLabelNo;
					}else
					{
						iRestartMissionLabel = EPauseMenuRestartMissionLabelNoneSelected;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelEndMissionConfirm)
				{
					if(iEndMissionYesRect.Contains(lTouchPosition))
					{
						iEndMissionLabel = EPauseMenuEndMissionLabelYes;
					}else if(iEndMissionNoRect.Contains(lTouchPosition))
					{
						iEndMissionLabel = EPauseMenuEndMissionLabelNo;
					}else
					{
						iEndMissionLabel = EPauseMenuEndMissionLabelNoneSelected;
					}
				}

				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//check if the player selected an option from the menu
				if(iDepthLevel == EPauseMenuDepthLevelTopSelection)
				{
					//check with rects in the top selection
					if(iSelectedPauseMenuLabel == EPauseMenuReturnToGameLabel)
					{
						//resume Music And Sound
						CGame::Game->iAudioEngine->GetMusicPlayer().Resume();
						CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Resume();
						//resume game
						iGameData.SetPaused(false);
						CTransitionHorizontalFlip::New(&CGame::Game->iView, this, CGameView::New(iGameData), 1);
						return;
					}else if(iSelectedPauseMenuLabel == EPauseMenuLoadGameLabel)
					{
						iDepthLevel = EPauseMenuDepthLevelLoadGameConfirm; // will now display the load submenu
						iLoadGameLabel = EPauseMenuLoadGameLabelNoneSelected;
					}else if(iSelectedPauseMenuLabel == EPauseMenuSaveGameLabel)
					{
						iDepthLevel = EPauseMenuDepthLevelSaveGameConfirm; // will now display the load submenu
						iSaveGameLabel = EPauseMenuSaveGameLabelNoneSelected;
					}else if(iSelectedPauseMenuLabel == EPauseMenuShowBattlefieldLabel)
					{
						TBool lDisplayBackgroundImage = CFighterPilotThePacificWar::FighterGame->iGameVariables->iDisplayBackgroundImageInPauseMenu;
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iDisplayBackgroundImageInPauseMenu = !lDisplayBackgroundImage;
					}else if(iSelectedPauseMenuLabel == EPauseMenuGameSettingsLabel)
					{
						//go into the game settings menu
						CGame::Game->iAudioEngine->CheckPlayerProfileIfAudioHasToBeMuted();//maybe settings changed
						CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Resume();
						CGame::Game->iAudioEngine->GetMusicPlayer().Resume();

						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CGameSettingsView::New(true), 2, 2, 2, 45);
					}else if(iSelectedPauseMenuLabel == EPauseMenuExitToMainMenuLabel)
					{
						iDepthLevel = EPauseMenuDepthLevelEndMissionConfirm;
						iEndMissionLabel = EPauseMenuEndMissionLabelNoneSelected;
					}else if(iSelectedPauseMenuLabel == EPauseMenuRestartMissionLabel)
					{
						iDepthLevel = EPauseMenuDepthLevelRestartMissionConfirm;
						iRestartMissionLabel = EPauseMenuRestartMissionLabelNoneSelected;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelLoadGameConfirm)
				{
					if(iLoadGameLabel == EPauseMenuLoadGameLabelYes)
					{
						//user stat
						CUserStats::EndMisssionIncreaseNumberOfLoads();

						//start loading the game for this level
						iGameData.GetMap()->LoadGame(iGameData.iMissionData->SaveGameFile->GetData());
						iDepthLevel = EPauseMenuDepthLevelTopSelection; // go back to top level selection
						//User::ResetInactivityTime();
					}else if(iLoadGameLabel == EPauseMenuLoadGameLabelNo)
					{
						//go back to top selection menu
						iDepthLevel = EPauseMenuDepthLevelTopSelection;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelSaveGameConfirm)
				{
					if(iSaveGameLabel == EPauseMenuSaveGameLabelYes)
					{
						//user stat
						CUserStats::EndMisssionIncreaseNumberOfSaves();

						//save the game
						iGameData.GetMap()->GetPlayer()->SetGameSaved(true);// saving the game means less bonus points
						iGameData.GetMap()->SaveGame(iGameData.iMissionData->SaveGameFile->GetData());
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iLastSaveGame = iGameData.iMissionData->MapId;
						iHasSaveGame = true;
						//all done, tell user that the game has been saved
						iDepthLevel = EPauseMenuDepthLevelSaveGameSuccessMessage;
					}else if(iSaveGameLabel == EPauseMenuSaveGameLabelNo)
					{
						//go back to top selection menu
						iDepthLevel = EPauseMenuDepthLevelTopSelection;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelSaveGameSuccessMessage)
				{
					//just go back to main menu once the player hits the screen
					iDepthLevel = EPauseMenuDepthLevelTopSelection;
				}else if(iDepthLevel == EPauseMenuDepthLevelRestartMissionConfirm)
				{
					if(iRestartMissionLabel == EPauseMenuRestartMissionLabelYes)
					{
						//user stat
						CUserStats::EndMisssionIncreaseNumberOfRestarts();

						//restart the game
						iGameData.GetMap()->RestartMap();
						//all done, map has been restarted
						iDepthLevel = EPauseMenuDepthLevelTopSelection;
						iSelectedPauseMenuLabel = EPauseMenuReturnToGameLabel;
					}else if(iRestartMissionLabel == EPauseMenuRestartMissionLabelNo)
					{
						//go back to top selection menu
						iDepthLevel = EPauseMenuDepthLevelTopSelection;
					}
				}else if(iDepthLevel == EPauseMenuDepthLevelEndMissionConfirm)
				{
					if(iEndMissionLabel == EPauseMenuEndMissionLabelYes)
					{
						//need to make these calls before we end the mission
						iGameData.GetMap()->GetPlayer()->UpdateStatsTrackerTimeValues();
						iGameData.GetMap()->GetPlayer()->SetDeleteStatsTracker(false);
						iGameData.GetMap()->EndMissionPreCall();

						//go back To Frontend
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CLoadingScreen::New(iGameData.iCampaignData, iGameData.iMissionData, ELoadingScreenModeGoToVictoryScreen), 5, 4, 4, 30);
						return;

					}else if(iEndMissionLabel == EPauseMenuEndMissionLabelNo)
					{
						//go back to top selection menu
						iDepthLevel = EPauseMenuDepthLevelTopSelection;
					}
				}

				iSelectedPauseMenuLabel = EPauseMenuNoneSelected;//always reset the top menu selection
				break;
			}
		}
	}
}

void CGamePauseView::ChildViewDrawMethod()
{
	//------------- DRAW BACKGROUND IMAGE ----------------------//
	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDisplayBackgroundImageInPauseMenu)
	{
		iBackgroundTexture->DrawArrays();
	}
	else
	{
		iGameView->Draw();
	}

	//------------- DRAW MENU OPTIONS ----------------------//
	TInt lHeight = iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iHeight;
	TInt lWidth = iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iWidth;

	TInt Offset = iFont.HeightAndDescentOfFont.GetIntInBaseInt();
	TInt sectionHeight = (lHeight - (Offset * NUMBER_OF_SLOTS)) / (NUMBER_OF_SLOTS);
	iGraphicsFont->SetFont(iFont);

	if(iDepthLevel == EPauseMenuDepthLevelTopSelection)
	{
		//LoadStrings
		CString* lStringShowBattleField = NULL;
		if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDisplayBackgroundImageInPauseMenu)
			lStringShowBattleField = iStringShowBattlefieldNo;
		else
			lStringShowBattleField = iStringShowBattlefieldYes;

		TInt sizeOfReturnToGameString = iGraphicsFont->GetWidthOfString(iFont, iStringReturnToGame);
		TInt sizeOfRestartMissionString = iGraphicsFont->GetWidthOfString(iFont, iStringRestartMission);
		TInt sizeOfExitToMainMenuString = iGraphicsFont->GetWidthOfString(iFont, iStringExitToMainMenu);
		TInt sizeOfLoadGameString = iGraphicsFont->GetWidthOfString(iFont, iStringLoadGame);
		TInt sizeOfSaveGameString = iGraphicsFont->GetWidthOfString(iFont, iStringSaveGame);
		TInt sizeOfBattleFieldString = iGraphicsFont->GetWidthOfString(iFont, lStringShowBattleField);
		TInt sizeOfGameSettingsString = iGraphicsFont->GetWidthOfString(iFont, iStringGameSettings);

		//Set location of each Label
		TPoint label1, label2, label3, label4, label5, label6, label7;
		label1 = TPoint((lWidth - sizeOfReturnToGameString) / 2, (sectionHeight * 7) + Offset * 7);
		label2 = TPoint((lWidth - sizeOfLoadGameString) / 2, (sectionHeight * 6) + Offset * 6);
		label3 = TPoint((lWidth - sizeOfSaveGameString) / 2, (sectionHeight * 5) + Offset * 5);
		label4 = TPoint((lWidth - sizeOfBattleFieldString) / 2, (sectionHeight * 4) + Offset * 4);
		label5 = TPoint((lWidth - sizeOfGameSettingsString) / 2, (sectionHeight * 3) + Offset * 3);
		label6 = TPoint((lWidth - sizeOfRestartMissionString) / 2, (sectionHeight * 2) + Offset * 2);
		label7 = TPoint((lWidth - sizeOfExitToMainMenuString) / 2, (sectionHeight * 1) + Offset * 1);

		//draw Text to Screen
		if(iSelectedPauseMenuLabel == EPauseMenuReturnToGameLabel)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label1, iStringReturnToGame);

		if(iSelectedPauseMenuLabel == EPauseMenuLoadGameLabel)
			iGraphicsFont->SetColourBlue();
		else if(iHasSaveGame)
			iGraphicsFont->SetColourBlack();
		else
			iGraphicsFont->SetColour(KColourDarkGray);
		iGraphicsFont->DrawStringToScreenHorizontally(label2, iStringLoadGame);

		if(iSelectedPauseMenuLabel == EPauseMenuSaveGameLabel)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label3, iStringSaveGame);

		if(iSelectedPauseMenuLabel == EPauseMenuShowBattlefieldLabel)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label4, lStringShowBattleField);

		if(iSelectedPauseMenuLabel == EPauseMenuGameSettingsLabel)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label5, iStringGameSettings);

		if(iSelectedPauseMenuLabel == EPauseMenuRestartMissionLabel)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label6, iStringRestartMission);

		if(iSelectedPauseMenuLabel == EPauseMenuExitToMainMenuLabel)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label7, iStringExitToMainMenu);

	}else if(iDepthLevel == EPauseMenuDepthLevelLoadGameConfirm)
	{
		TInt sizeOfLoadGameString = iGraphicsFont->GetWidthOfString(iFont, iStringLoadGameWarning);
		TInt sizeOfNoString = iGraphicsFont->GetWidthOfString(iFont, iStringNo);

		TPoint label1, label2, label3;
		label1 = TPoint((lWidth - sizeOfLoadGameString) / 2, sectionHeight * 6 + Offset * 6);
		label2 = TPoint((lWidth) / 4, (sectionHeight * 2) + Offset * 2);// both at at the same height
		label3 = TPoint(((lWidth * 3) / 4) - sizeOfNoString, (sectionHeight * 2) + Offset * 2);

		//draw warning message
		iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label1, iStringLoadGameWarning);
		//draw choices
		if(iLoadGameLabel == EPauseMenuLoadGameLabelYes)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label2, iStringYes);

		if(iLoadGameLabel == EPauseMenuLoadGameLabelNo)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label3, iStringNo);

	}else if(iDepthLevel == EPauseMenuDepthLevelSaveGameConfirm)
	{
		TInt sizeOfSaveGameString =  iGraphicsFont->GetWidthOfString(iFont, iStringSaveGameWarning);
		TInt sizeOfNoString =  iGraphicsFont->GetWidthOfString(iFont, iStringNo);

		TPoint label1, label2, label3;
		label1 = TPoint((lWidth - sizeOfSaveGameString) / 2, sectionHeight * 6 + Offset * 6);
		label2 = TPoint((lWidth) / 4, (sectionHeight * 2) + Offset * 2);// both at at the same height
		label3 = TPoint(((lWidth * 3) / 4) - sizeOfNoString, (sectionHeight * 2) + Offset * 2);

		//draw warning message
		iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label1, iStringSaveGameWarning);
		//draw choices
		if(iSaveGameLabel == EPauseMenuSaveGameLabelYes)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label2, iStringYes);

		if(iSaveGameLabel == EPauseMenuSaveGameLabelNo)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label3, iStringNo);

	}else if(iDepthLevel == EPauseMenuDepthLevelSaveGameSuccessMessage)
	{
		TInt sizeOfSaveGameSuccessString = iGraphicsFont->GetWidthOfString(iFont, iStringSaveGameSuccessMessage);
		TInt sizeOfContinueString = iGraphicsFont->GetWidthOfString(iFont, iStringConfirmContinue);

		TPoint label1, label2;
		label1 = TPoint((lWidth - sizeOfSaveGameSuccessString) / 2, sectionHeight * 6 + Offset * 6);
		label2 = TPoint((lWidth - sizeOfContinueString) / 2, (sectionHeight * 2) + Offset * 2);

		//draw Success message
		iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label1, iStringSaveGameSuccessMessage);
		//draw Continue Button
		iGraphicsFont->SetColourBlue();
		iGraphicsFont->DrawStringToScreenHorizontally(label2, iStringConfirmContinue);

	}else if(iDepthLevel == EPauseMenuDepthLevelRestartMissionConfirm)
	{
		TInt sizeOfSaveGameString = iGraphicsFont->GetWidthOfString(iFont, iStringRestartMission);
		TInt sizeOfNoString = iGraphicsFont->GetWidthOfString(iFont, iStringNo);

		TPoint label1, label2, label3;
		label1 = TPoint((lWidth - sizeOfSaveGameString) / 2, sectionHeight * 6 + Offset * 6);
		label2 = TPoint((lWidth) / 4, (sectionHeight * 2) + Offset * 2);// both at at the same height
		label3 = TPoint(((lWidth * 3) / 4) - sizeOfNoString, (sectionHeight * 2) + Offset * 2);

		//draw warning message
		iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label1, iStringRestartMission);
		//draw choices
		if(iRestartMissionLabel == EPauseMenuRestartMissionLabelYes)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label2, iStringYes);

		if(iRestartMissionLabel == EPauseMenuRestartMissionLabelNo)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label3, iStringNo);

	}else if(iDepthLevel == EPauseMenuDepthLevelEndMissionConfirm)
	{
		CString* lStringMissionStatus = NULL;

		if(iGameData.GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusFailed)
			lStringMissionStatus = iStringEndMissionFailed;
		else if(iGameData.GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
			lStringMissionStatus = iStringEndMissionPending;
		else if(iGameData.GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusSuccess)
			lStringMissionStatus = iStringEndMissionSuccess;

		CString* lStringMissionStatusComplete = new CString();
		lStringMissionStatusComplete->Append(iStringEndMissionStatus);
		lStringMissionStatusComplete->Append(lStringMissionStatus);

		TInt sizeOfEndMisionWarningString = iGraphicsFont->GetWidthOfString(iFont, iStringEndMissionWarning);
		TInt sizeOfNoString = iGraphicsFont->GetWidthOfString(iFont, iStringNo);
		TInt sizeOfMissionStatusString = iGraphicsFont->GetWidthOfString(iFont, lStringMissionStatusComplete);

		TPoint label1, label2, label3, label4;
		label1 = TPoint((lWidth - sizeOfEndMisionWarningString) / 2, sectionHeight * 6 + Offset * 6);
		label2 = TPoint((lWidth - sizeOfMissionStatusString) / 2, sectionHeight * 4 + Offset * 4);
		label3 = TPoint((lWidth) / 4, (sectionHeight * 2) + Offset * 2);// both at at the same height
		label4 = TPoint(((lWidth * 3) / 4) - sizeOfNoString, (sectionHeight * 2) + Offset * 2);

		//draw warning message
		iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label1, iStringEndMissionWarning);
		iGraphicsFont->DrawStringToScreenHorizontally(label2, lStringMissionStatusComplete);

		//draw choices
		if(iEndMissionLabel == EPauseMenuEndMissionLabelYes)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label3, iStringYes);

		if(iEndMissionLabel == EPauseMenuEndMissionLabelNo)
			iGraphicsFont->SetColourBlue();
		else
			iGraphicsFont->SetColourBlack();
		iGraphicsFont->DrawStringToScreenHorizontally(label4, iStringNo);

		//need to delete composite string
		delete lStringMissionStatusComplete;
	}
}

void CGamePauseView::ContextGained()
{
	//pause Music And Sound
	CGame::Game->iAudioEngine->GetMusicPlayer().Pause();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Pause();
}

void CGamePauseView::ContextLost()
{
	//pause Music And Sound
	CGame::Game->iAudioEngine->GetMusicPlayer().Pause();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Pause();
}
