/*
 * ChangeSongView.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: dstoll
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/views/CChangeSongView.h"
#include "includes/game/views/CGameSettingsView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"

CChangeSongView::CChangeSongView(TBool aReturnToGameView)
{
	iSongTilesTopYLocation = 375;
	iLastSongTileSelected = -1;
	iSongTilesYOffset = 0;
	iReturnToGameView = aReturnToGameView;
}

CChangeSongView::~CChangeSongView()
{
	if(iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}

	delete iStringSongPlaying;
	delete iStringAvailableSongs;
	delete iStringCancelButton;
	delete iStringSaveButton;

	delete iCancelButton;
	delete iSaveButton;
	delete iScrollBar;
	delete iSongTilesButtons;
	delete iOriginalSongPlaying.SongName;
	//delete structer and Pointer Array
	for(TInt lIndex = 0; lIndex < iSongTilesNames->GetCount(); lIndex++)
	{
		delete iSongTilesNames->Get(lIndex)->SongName;
	}
	iSongTilesNames->ClearAndDestroy();
	delete iSongTilesNames;
}

CChangeSongView* CChangeSongView::New(TBool aReturnToGameView)
{
	CChangeSongView* lSelf = new CChangeSongView(aReturnToGameView);
	lSelf->Construct();
	return lSelf;
}

void CChangeSongView::Construct()
{
	//create texture Object
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendChangeSongScreen), 1024, 660, 100, 0, 1, 0, 1);

	//---------------- Strings ---------------------//
	iStringSongPlaying = iLocalizationEngine->GetString("Frontend.ChangeSong.SongPlaying");
	iStringAvailableSongs = iLocalizationEngine->GetString("Frontend.ChangeSong.AvailableSongs");
	iStringCancelButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Cancel");
	iStringSaveButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Save");

	//----------------- Fonts -----------------//
	iFontForSongNames = iGraphicsFont->CreateFont(30);

	CPointerArray<CString>* lLabelStringArray = CPointerArray<CString>::New();
	lLabelStringArray->Append(iStringAvailableSongs);
	lLabelStringArray->Append(iStringSongPlaying);
	iFontForLabels = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lLabelStringArray, 850, 50);
	//clean up
	delete lLabelStringArray;
	lLabelStringArray = NULL;

	//ForButtons
	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	lButtonStringArray->Append(iStringCancelButton);
	lButtonStringArray->Append(iStringSaveButton);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 190, 60);
	//clean up
	delete lButtonStringArray;
	lButtonStringArray = NULL;

	//--------------- Buttons -------------//

	//Cancel Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(151, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.Label = iStringCancelButton;
	lBackButtonArgs.FontForLabel = &iFontForButtons;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iCancelButton = CButton::New(lBackButtonArgs);

	//Save Button
	CButton::SButtonArguments lStartButtonArgs;
	lStartButtonArgs.Location = TPoint(663, 0);
	lStartButtonArgs.SizeOfHitbox = TSize(210, 85);
	lStartButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lStartButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lStartButtonArgs.SizeOfTexture = TSize(210, 85);
	lStartButtonArgs.Label = iStringSaveButton;
	lStartButtonArgs.FontForLabel = &iFontForButtons;
	lStartButtonArgs.ColourForLabelDefault = KColourBlack;
	lStartButtonArgs.ColourForLabelSelected = KColourBlack;
	iSaveButton = CButton::New(lStartButtonArgs);

	//--------------- Songs -------------//

	iOriginalSongPlaying.SongName = new CString(CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetNameOfSong()->GetData());
	iOriginalSongPlaying.IsUserSong = CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetIsPlayingUserSong();

	iSongTilesNames = CPointerArray<SSongEntry>::New();
	iSongTilesButtons = CRadioButtonGroup::New();


	//---------------------- SONGS -------------------//
	TInt lMaxYOffsetForSongs = -301; //by default we can display 301 pixels of data in the song box

	//add GameSongs
	//AddSongTile(SONG_BATTLE_HYMN, false);
	//AddSongTile(SONG_BIG_HITS, false);
	AddSongTile(SONG_DARK_MARCH, false);
	//AddSongTile(SONG_DEATH_FROM_ABOVE, false);
//	AddSongTile(SONG_DISTO_MARCH, false);
	AddSongTile(SONG_MENU_LOOP, false);
	lMaxYOffsetForSongs += 46 * 2; //space used by game songs

	//add User Songs
	CPointerArray<CString>* lUserFiles = CDirectory::GetFilesInDirectory(CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetUserSongPath()->GetData());
	//add Files if they are valid mp3 songs
	for(TInt lIndex = 0; lIndex < lUserFiles->GetCount(); lIndex++)
	{
		//get file and extension
		CString* lFileName = lUserFiles->Get(lIndex);
		//make sure file name is long enough to have a valid file extension
		if(lFileName->GetLength() >= 4)
		{
			CString* lFileExtension = lFileName->GetSubString(lFileName->GetLength() - 4,4); //extract the file name

			//check if it is a mp3 file
			if(lFileExtension->EqualTo(".mp3"))
			{
				AddSongTile(lFileName->GetData(), true);
				lMaxYOffsetForSongs += 46;
			}

			//clean up
			delete lFileExtension;
		}
	}

	//clean up
	lUserFiles->ClearAndDestroy();
	delete lUserFiles;
	lUserFiles = NULL;

	//make sure it is not negative
	if(lMaxYOffsetForSongs < 0)
	{
		lMaxYOffsetForSongs = 0;
	}

	//ScrollBar for Songs
	CScrollBar::SScrollBarArguments lScrollBarArgs;
	lScrollBarArgs.Location = TPoint(960, 331);
	lScrollBarArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarArgs.MaxScroll = lMaxYOffsetForSongs;
	lScrollBarArgs.MaxScrollHeight = 302;
	lScrollBarArgs.ScrollSpeedFactor = 4;
	lScrollBarArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBar = CScrollBar::New(lScrollBarArgs, iSongTilesYOffset);

	if(iReturnToGameView)
	{
		CUserStats::ChangeSongMenuOpenedInGame();
	}
	else
	{
		CUserStats::ChangeSongMenuOpenedFrontend();
	}
}

//-------------------- functions ---------------------//
void CChangeSongView::LoadImagesNeededForView()
{
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedMenuNavigationButton);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendChangeSongScreenSongListTile);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendChangeSongScreenSongListTileSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendChangeSongScreen);
}

void CChangeSongView::UnloadImagesNeededForView()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedMenuNavigationButton);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendChangeSongScreenSongListTile);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendChangeSongScreenSongListTileSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendChangeSongScreen);
}

void CChangeSongView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//Song Tiles
				if(iSongTilesButtons->CheckIfHit(aTouchEvent))
				{
					TInt lSelectedButton = iSongTilesButtons->GetValueOfSelectedButton();

					if(iLastSongTileSelected != lSelectedButton)
					{
						//check if the button is in the visible range
						TInt lButtonLowerLeftY = iSongTilesButtons->GetRadioButton(lSelectedButton)->GetLowerLeftLocation().iY;

						if(lButtonLowerLeftY < 421 && lButtonLowerLeftY > 80)
						{
							//Song has been selected
							iLastSongTileSelected = lSelectedButton;//avoid that the same song gets started again
							CString* lNameOfSong = iSongTilesNames->Get(iSongTilesButtons->GetValueOfSelectedButton())->SongName;
							TBool lUserSong = iSongTilesNames->Get(iSongTilesButtons->GetValueOfSelectedButton())->IsUserSong;

							if(lUserSong)
							{
								CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().PlayUserSong(lNameOfSong->GetData(), true);
							}
							else
							{
								CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().PlayGameSong(lNameOfSong->GetData(), true);
							}
						}
						else
						{
							//deselect the button again, since it was out of range
							iSongTilesButtons->GetRadioButton(lSelectedButton)->SetState(CRadioButton::EStateDefault);
							//select the old Button again, unless none was selected
							if(iLastSongTileSelected > 0)
							{
								iSongTilesButtons->GetRadioButton(iLastSongTileSelected)->SetState(CRadioButton::EStateSelected);
							}
						}
					}
				}

				//ScrollBar
				iScrollBar->CheckIfHit(aTouchEvent);

				//Buttons
				iCancelButton->CheckIfHit(aTouchEvent);
				iSaveButton->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				//Song Tiles
				if(iSongTilesButtons->CheckIfHit(aTouchEvent))
				{
					TInt lSelectedButton = iSongTilesButtons->GetValueOfSelectedButton();

					if(iLastSongTileSelected != lSelectedButton)
					{
						//check if the button is in the visible range
						TInt lButtonLowerLeftY = iSongTilesButtons->GetRadioButton(lSelectedButton)->GetLowerLeftLocation().iY;

						if(lButtonLowerLeftY < 421 && lButtonLowerLeftY > 80)
						{
							//Song has been selected
							iLastSongTileSelected = lSelectedButton;//avoid that the same song gets started again
							CString* lNameOfSong = iSongTilesNames->Get(iSongTilesButtons->GetValueOfSelectedButton())->SongName;
							TBool lUserSong = iSongTilesNames->Get(iSongTilesButtons->GetValueOfSelectedButton())->IsUserSong;

							if(lUserSong)
							{
								CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().PlayUserSong(lNameOfSong->GetData(), true);
							}
							else
							{
								CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().PlayGameSong(lNameOfSong->GetData(), true);
							}
						}
						else
						{
							//deselect the button again, since it was out of range
							iSongTilesButtons->GetRadioButton(lSelectedButton)->SetState(CRadioButton::EStateDefault);
							//select the old Button again, unless none was selected
							if(iLastSongTileSelected > 0)
							{
								iSongTilesButtons->GetRadioButton(iLastSongTileSelected)->SetState(CRadioButton::EStateSelected);
							}
						}
					}
				}

				//ScrollBar
				iScrollBar->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//ScrollBar
				iScrollBar->CheckIfHit(aTouchEvent);

				//go back to GameSettings, reset the old Song
				if (iCancelButton->GetState() == CButton::EStateSelected && iCancelButton->CheckIfHit(aTouchEvent))
				{
					const CString* lSongPlaying = CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetNameOfSong();
					if(!iOriginalSongPlaying.SongName->EqualTo(lSongPlaying))
					{
						//set back to Original
						if(iOriginalSongPlaying.IsUserSong)
						{
							CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().PlayUserSong(iOriginalSongPlaying.SongName->GetData(), true);
						}
						else
						{
							CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().PlayGameSong(iOriginalSongPlaying.SongName->GetData(), true);
						}
					}

					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CGameSettingsView::New(iReturnToGameView), 0.5, false);
				}
				//go back to GameSettings, keep playing the current Song
				if (iSaveButton->GetState() == CButton::EStateSelected && iSaveButton->CheckIfHit(aTouchEvent))
				{
					//user Stat
					if(CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetIsPlayingUserSong())
					{
						CUserStats::ChangeSong(!iReturnToGameView, "UserSong");
					}
					else
					{
						CUserStats::ChangeSong(!iReturnToGameView, CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetNameOfSong()->GetData());
					}

					//was this song changed in the frontend?
					if(!iReturnToGameView)
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iIsUserSong = CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetIsPlayingUserSong();
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iFrontendSong->Replace(CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetNameOfSong());
					}

					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CGameSettingsView::New(iReturnToGameView), 0.5, false);
				}
				break;
			}
		}
	}
}

void CChangeSongView::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//---------------- Draw Song Tiles --------------------//
	iGraphicsFont->SetFont(iFontForSongNames);
	iGraphicsFont->SetColourBlack();

	//Need to reallocate Song Tiles since they might have moved
	for(TInt lIndex = 0; lIndex < iSongTilesButtons->GetCount(); lIndex++)
	{
		if(lIndex == 0)
		{
			iSongTilesButtons->GetRadioButton(lIndex)->SetLowerLeftLocation(TPoint(80, iSongTilesTopYLocation + iSongTilesYOffset));
		}
		else
		{
			TInt lPreviousSongTileYLocation = iSongTilesButtons->GetRadioButton(lIndex -1)->GetLowerLeftLocation().iY;
			iSongTilesButtons->GetRadioButton(lIndex)->SetLowerLeftLocation(TPoint(80, lPreviousSongTileYLocation - 46));
		}
	}

	//draw Tiles
	iSongTilesButtons->Draw();
	//draw Names for Songs
	for(TInt lIndex = 0; lIndex < iSongTilesNames->GetCount(); lIndex++)
	{
		//draw Song Name
		TPoint lLocation = iSongTilesButtons->GetRadioButton(lIndex)->GetLowerLeftLocation();
		lLocation.iX += 20; //default Offset
		lLocation.iY += 10; //default Offset
		iGraphicsFont->DrawStringToScreenHorizontally(lLocation, iSongTilesNames->Get(lIndex)->SongName);
	}

	//--------------- draw Background ----------------//
	iBackgroundTexture->DrawArrays();

	//--------------- Draw Current Song Playing -------------------//
	iGraphicsFont->SetFont(iFontForSongNames);
	iGraphicsFont->SetColourBlack();
	//draw Song Playing
	const CString* lSongPlaying = CFighterPilotThePacificWar::FighterGame->iAudioEngine->GetMusicPlayer().GetNameOfSong();
	iGraphicsFont->DrawStringToScreenHorizontally(TPoint(100, 530), lSongPlaying);

	//--------- draw Labels ------------//
	iGraphicsFont->SetFont(iFontForLabels);
	iGraphicsFont->SetColourWhite();
	//Song Playing
	iGraphicsFont->DrawStringToScreenHorizontally(TPoint(80, 585), iStringSongPlaying);
	//Available Songs
	iGraphicsFont->DrawStringToScreenHorizontally(TPoint(80, 440), iStringAvailableSongs);

	//----------- Controls ------------//
	iScrollBar->Draw();
	iCancelButton->Draw();
	iSaveButton->Draw();
}

void CChangeSongView::AddSongTile(const char* aSongName, TBool aUserSong)
{
	//add GameSongs
	SSongEntry lSongEntry;
	TInt lYLocationOfTile = 0;

	if(iSongTilesNames->GetCount() == 0)
	{
		//the first entry
		lYLocationOfTile = iSongTilesTopYLocation;
	}
	else
	{
		lYLocationOfTile = iSongTilesButtons->GetRadioButton(iSongTilesNames->GetCount() - 1)->GetLowerLeftLocation().iY;
		lYLocationOfTile -= 46; //height of a tile, since we need to move below the last one
	}

	CRadioButton::SRadioButtonArguments lSongTileButton;
	lSongTileButton.Location = TPoint(80, lYLocationOfTile);
	lSongTileButton.SizeOfHitbox = TSize(864, 46);
	lSongTileButton.SizeOfTexture = TSize(864, 46);
	lSongTileButton.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendChangeSongScreenSongListTile);
	lSongTileButton.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendChangeSongScreenSongListTileSelected);
	lSongTileButton.TextureIdDisabled = &iImageStore->GetImage(EImageIdFrontendChangeSongScreenSongListTileSelected);
	lSongTileButton.Label = NULL;

	lSongEntry.IsUserSong = aUserSong;
	lSongEntry.SongName = new CString(aSongName);

	iSongTilesNames->Append(new SSongEntry(lSongEntry));
	iSongTilesButtons->AddRadioButton(CRadioButton::New(lSongTileButton), iSongTilesNames->GetCount() - 1);
}

