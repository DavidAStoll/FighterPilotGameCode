/*
 * CImageStore.cpp
 *
 *  Created on: Jun 12, 2012
 *      Author: dstoll
 */

#include "includes/core/utility/CImageStore.h"
#include "includes/core/CGame.h"

CImageStore::CImageStore()
{
	iImageStoreEntries = NULL;
}

CImageStore::~CImageStore()
{
	//remove all loaded images from OpenGL
	UnloadAllImages();

	if(iImageStoreEntries != NULL)
	{
		for(TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
		{
			if(iImageStoreEntries->Get(lIndex)->ImageFileLocation != NULL)
			{
				delete  iImageStoreEntries->Get(lIndex)->ImageFileLocation;
				iImageStoreEntries->Get(lIndex)->ImageFileLocation = NULL;
			}
		}

		iImageStoreEntries->ClearAndDestroy();
		delete iImageStoreEntries;
		iImageStoreEntries = NULL;
	}
}

CImageStore* CImageStore::New()
{
	CImageStore* lSelf = new CImageStore();
	lSelf->InitializeImageStore();
	return lSelf;
}

SGraphicsEngineTextureId& CImageStore::GetImage(TImageId aImageId)
{
	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		if (iImageStoreEntries->Get(lIndex)->ImageId == aImageId)
		{
			return iImageStoreEntries->Get(lIndex)->TextureId;
		}
	}

	//should never really happen
	return iImageStoreEntries->Get(0)->TextureId;
}

void CImageStore::LoadAllFrontendImages()
{
	for (TInt lIndex = 0; lIndex < EImageIdEndOfFrontEndImages; lIndex++)
	{
		CImageStore::LoadImage(static_cast<TImageId>(lIndex));
	}
}

void CImageStore::LoadImage(TImageId aImageId)
{
	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		SImageStoreEntry* lEntry = iImageStoreEntries->Get(lIndex);

		if (lEntry->ImageId == aImageId)
		{
			//check if the image has not been loaded previously
			if(lEntry->TextureId.GraphicsEngineId == 0)
			{
				//found the correct entry
				lEntry->TextureId = CGame::Game->iGraphicsEngine->CreateGraphicsTextureId();
				//load Bitmap from disk
				const char* lFileLocation = lEntry->ImageFileLocation->GetData();
				SBitmap lBitmap = 	CGame::Game->iGraphicsEngine->LoadBitmapImageFromDisk(lFileLocation);
				lEntry->SizeInPixels = TSize(lBitmap.WidthInPixels, lBitmap.HeightInPixels);
				lEntry->Bitmap.Data = NULL;

				switch (lEntry->LoadMethod)
				{
					case EGraphicsEngineTextureLoadTypeBasic:
					{
						CGame::Game->iGraphicsEngine->ConvertBitmapImageTo2DTexture(lEntry->TextureId, lBitmap, lEntry->ClampImage, lFileLocation);
						break;
					}
					case EGraphicsEngineTextureLoadTypeWithReadingByDefault:
					{
						CGame::Game->iGraphicsEngine->ConvertBitmapImageTo2DTextureWithReadingStartingByDefault(lEntry->TextureId,lBitmap, lEntry->ClampImage, lFileLocation);
						break;
					}
					case EGraphicsEngineTextureLoadTypeWithoutMasking:
					{
						CGame::Game->iGraphicsEngine->ConvertBitmapImageTo2DTextureWithoutMasking(lEntry->TextureId, lBitmap, lEntry->ClampImage, lFileLocation);
						break;
					}
					case EGraphicsEngineTextureLoadTypePNG:
					{
						CGame::Game->iGraphicsEngine->ConvertPNGTo2DTexture(lEntry->TextureId, lBitmap, lEntry->ClampImage, lFileLocation);
						break;
					}

				}
			}

			//all done
			return;
		}
	}
}

void CImageStore::LoadOverSizedImage(TImageId aImageId)
{
	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		SImageStoreEntry* lEntry = iImageStoreEntries->Get(lIndex);

		if (lEntry->ImageId == aImageId)
		{
			//found the correct entry
			//load Bitmap from disk
			const char* lFileLocation = lEntry->ImageFileLocation->GetData();
			lEntry->Bitmap = 	CGame::Game->iGraphicsEngine->LoadBitmapImageFromDisk(lFileLocation);
			lEntry->SizeInPixels = TSize(lEntry->Bitmap.WidthInPixels, lEntry->Bitmap.HeightInPixels);
			lEntry->TextureId.GraphicsEngineId = 0;

			//all done
			return;
		}
	}
}

TSize CImageStore::GetImageSize(TImageId aImageId)
{
	TSize lSizeOfImage = TSize(1,1);

	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		SImageStoreEntry* lEntry = iImageStoreEntries->Get(lIndex);

		if (lEntry->ImageId == aImageId)
		{
			lSizeOfImage = lEntry->SizeInPixels;
			break;
		}
	}

	return lSizeOfImage;
}

SBitmap* CImageStore::GetOverSizeImage(TImageId aImageId)
{
	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		SImageStoreEntry* lEntry = iImageStoreEntries->Get(lIndex);

		if (lEntry->ImageId == aImageId)
		{
			return &lEntry->Bitmap;
		}
	}

	return NULL;
}

void CImageStore::UnloadImage(TImageId aImageId)
{
	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		SImageStoreEntry* lEntry = iImageStoreEntries->Get(lIndex);

		if(lEntry->ImageId == aImageId)
		{
			if(lEntry->TextureId.GraphicsEngineId != 0) //means it has been loaded into OpenGL
			{
				//delete texture from OpenGl
				if(CGame::Game->iGraphicsEngine != NULL)
				{
					//delete Graphics Engine texture
					CGame::Game->iGraphicsEngine->DeleteGraphicsTextureId(lEntry->TextureId);
				}
			}
			if(lEntry->Bitmap.Data != NULL)
			{
				delete[] lEntry->Bitmap.Data;
				lEntry->Bitmap.Data = NULL;
			}
			else
			{
				//image has not been loaded as texture, nothing needs to be done
				return;
			}
		}
	}
}

void CImageStore::UnloadAllFrontendImages()
{
	for (TInt lIndex = 0; lIndex < EImageIdEndOfFrontEndImages; lIndex++)
	{
		UnloadImage(static_cast<TImageId>(lIndex));
	}
}

void CImageStore::UnloadAllImages()
{
	for (TInt lIndex = 0; lIndex < iImageStoreEntries->GetCount(); lIndex++)
	{
		UnloadImage(iImageStoreEntries->Get(lIndex)->ImageId);
	}
}

void CImageStore::Add(TImageId aId, const char* aImageLocation, TGraphicsEngineTextureLoadType aLoadMethod, TBool aClampImage)
{
	SImageStoreEntry lNewEntry;
	lNewEntry.ImageId = aId;
	lNewEntry.ImageFileLocation = new CString(aImageLocation);
	lNewEntry.LoadMethod = aLoadMethod;
	lNewEntry.TextureId.GraphicsEngineId = 0;
	lNewEntry.TextureId.OpenGlId = 0;
	lNewEntry.ClampImage = aClampImage;
	lNewEntry.Bitmap.Data = NULL;

	iImageStoreEntries->Append(new SImageStoreEntry(lNewEntry));
}

void CImageStore::InitializeImageStore()
{
	if (iImageStoreEntries == NULL)
	{
		//image has not been created already

		//create the pointer array
		iImageStoreEntries = CPointerArray<SImageStoreEntry>::New();

		//add Entries
		//Title
		Add(EImageIdFrontendTileScreen, "Images/Frontend/TitleScreen/TitleScreen.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMainMenuBackgroundScreen, "Images/Frontend/TitleScreen/RisingSunTitleScaledDownWithBoxes.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMainMenuBackgroundExitConfirmationScreen, "Images/Frontend/TitleScreen/RisingSunTitleScaledDownWithBoxesExitConfirm.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Choose Campaign
		Add(EImageIdFrontendChooseCampaignScreenBackgroundScreen, "Images/Frontend/ChooseCampaignScreen/ChooseCampaignScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdFrontendChooseCampaignScreenBackgroundScreenSelectionBackground, "Images/Frontend/ChooseCampaignScreen/ChooseCampaignScreenSelectionBackground.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignScreenSelectionBox, "Images/Frontend/ChooseCampaignScreen/ChooseCampaignScreenSelectionBox.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignScreenSelectionBoxButton, "Images/Frontend/ChooseCampaignScreen/ChooseCampaignScreenSelectionBoxButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignScreenSelectionBoxButtonSelected, "Images/Frontend/ChooseCampaignScreen/ChooseCampaignScreenSelectionBoxButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignImageDauntlessTakeOfFromCarrier, "Images/Frontend/ChooseCampaignScreen/CampaignImages/DauntlessTakeOffFromCarrierColorConverted.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignImageIwoJimaLanding, "Images/Frontend/ChooseCampaignScreen/CampaignImages/IwoJimaLandingColorConverted.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignImageJapaneseSurrender, "Images/Frontend/ChooseCampaignScreen/CampaignImages/JapaneseSurrenderColorConverted.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignImageMitchellAttackShip, "Images/Frontend/ChooseCampaignScreen/CampaignImages/MitchellAttacksShipColorConverted.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChooseCampaignImageSingleDauntless, "Images/Frontend/ChooseCampaignScreen/CampaignImages/SingleDauntlessColorConverted.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Campaign
		Add(EImageIdFrontendCampaignScreen, "Images/Frontend/CampaignScreen/CampaignScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdFrontendCampaignScreenStartLoadButton, "Images/Frontend/CampaignScreen/CampaignScreenSelectionBoxButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenStartLoadButtonSelected, "Images/Frontend/CampaignScreen/CampaignScreenSelectionBoxButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenBabySymbol, "Images/Frontend/CampaignScreen/CampaignScreenBabyLevelSymbol.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenBabySymbolGreyedOut, "Images/Frontend/CampaignScreen/CampaignScreenBabyLevelSymbolGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenEasySymbol, "Images/Frontend/CampaignScreen/CampaignScreenEasyLevelSymbol.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenEasySymbolGreyedOut, "Images/Frontend/CampaignScreen/CampaignScreenEasyLevelSymbolGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenMediumSymbol, "Images/Frontend/CampaignScreen/CampaignScreenMediumLevelSymbol.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenMediumSymbolGreyedOut, "Images/Frontend/CampaignScreen/CampaignScreenMediumLevelSymbolGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenHardSymbol, "Images/Frontend/CampaignScreen/CampaignScreenHardLevelSymbol.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignScreenHardSymbolGreyedOut, "Images/Frontend/CampaignScreen/CampaignScreenHardLevelSymbolGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//MissionBriefing
		Add(EImageIdFrontendMissionBriefingScreen, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyBaby, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenBaby.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyBabySelected, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenBabySelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyBabyGreyedOut, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenBabyGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyEasy, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenEasy.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyEasySelected, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenEasySelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyEasyGreyedOut, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenEasyGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyMedium, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenMedium.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyMediumSelected, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenMediumSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyMediumGreyedOut, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenMediumGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyHard, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenHard.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyHardSelected, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenHardSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionBriefingScreenDifficultyHardGreyedOut, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenHardGreyedOut.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//GameSettingsScreen
		Add(EImageIdFrontendGameSettingsScreen, "Images/Frontend/GameSettingsScreen/GameSettingsScreen.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendGameSettingsScreenChangeSongButton, "Images/Frontend/GameSettingsScreen/ChangeSongButtonNotSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendGameSettingsScreenChangeSongButtonSelected, "Images/Frontend/GameSettingsScreen/ChangeSongButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendGameSettingsScreenScrollBarButton, "Images/Frontend/GameSettingsScreen/ScrollBarButtonNotSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendGameSettingsScreenScrollBarButtonSelected, "Images/Frontend/GameSettingsScreen/ScrollBarButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendGameSettingsScreenUseButtonIcon, "Images/Frontend/GameSettingsScreen/UseButtonIconNotChecked.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendGameSettingsScreenUseButtonIconChecked, "Images/Frontend/GameSettingsScreen/UseButtonIconChecked.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//ChangeSongScreen
		Add(EImageIdFrontendChangeSongScreen, "Images/Frontend/GameSettingsScreen/ChangeSongScreen/ChangeSongScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdFrontendChangeSongScreenSongListTile, "Images/Frontend/GameSettingsScreen/ChangeSongScreen/SongListTileNotSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendChangeSongScreenSongListTileSelected, "Images/Frontend/GameSettingsScreen/ChangeSongScreen/SongListTileSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//SelectLanguageScreen
		Add(EImageIdFrontendSelectLanguageScreen, "Images/Frontend/SelectLanguageScreen/SelectLanguageScreen.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//CreditsScreen
		Add(EImageIdFrontendCreditsScreenBottomPart, "Images/Frontend/CreditsScreen/CreditsScreenBottomPart.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenAmericanDestroyer, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsAmericanDestroyer.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenDauntlessAndVal, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsDauntlessAndVal.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenJapanese97ChiHa, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsJapanese97ChiHa.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenJapaneseBattleshipYamato, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsJapaneseBattleshipYamato.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenMitchellAndBetty, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsMitchellAndBetty.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenShermanTanks, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsShermanTanks.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenShokaku, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsShokaku.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenSoldiers, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsSoldiers.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCreditsScreenVictory, "Images/Frontend/CreditsScreen/CreditsScreenImages/CreditsVictory.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//MissionVictoryScreen
		Add(EImageIdFrontendMissionVictoryScreen, "Images/Frontend/MissionVictoryScreen/MissionVictoryScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		//MissionImages
		Add(EImageIdFrontendMissionImageDauntlessDropsBomb, "Images/Frontend/CampaignScreen/MissionImages/Tutorial/DauntlessBombDrop.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageDauntlessFlyInFormation, "Images/Frontend/CampaignScreen/MissionImages/Tutorial/DauntlessFlyInFormation.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageDauntlessOnAirport, "Images/Frontend/CampaignScreen/MissionImages/Tutorial/DauntlessOnAirport.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageDauntlessTakesOff, "Images/Frontend/CampaignScreen/MissionImages/Campaign1/DauntlessTakesOffFromCarrier.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageOkinawaCaveDemolition, "Images/Frontend/CampaignScreen/MissionImages/Campaign1/OkinawaMarineCaveDemolition.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageWaterBuffaloLoaded, "Images/Frontend/CampaignScreen/MissionImages/Campaign1/WaterBuffaloLoadedWithMarines.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageLexingtionBurns, "Images/Frontend/CampaignScreen/MissionImages/Campaign1/USSLexingtonBurns.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageG4MBetty, "Images/Frontend/CampaignScreen/MissionImages/Campaign2/G4MBetty.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageJapaneseBattleshipAttacked, "Images/Frontend/CampaignScreen/MissionImages/Campaign2/JapaneseBattleshipHarunaAttacked.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageTarawaOnHill, "Images/Frontend/CampaignScreen/MissionImages/Campaign2/TarawaOnHill.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageTransporterAttack, "Images/Frontend/CampaignScreen/MissionImages/Campaign2/TransporterAttack.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageAmericansLandeAtLosNegros, "Images/Frontend/CampaignScreen/MissionImages/Campaign3/AmericansLandedAtLosNegros.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageShermanTankFlamethrower, "Images/Frontend/CampaignScreen/MissionImages/Campaign3/ShermanTankFlamethrower.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageShootDownBurningJapanesePlane, "Images/Frontend/CampaignScreen/MissionImages/Campaign3/ShootDownBuringJapanesePlane.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageBunkerHitTwoKamikazes, "Images/Frontend/CampaignScreen/MissionImages/Campaign3/USSBunkerHillHitTwoKamikazes.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageIwoJimaFlagRising, "Images/Frontend/CampaignScreen/MissionImages/Campaign4/BattleIwoJimaVolcano.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageBattleMarianas, "Images/Frontend/CampaignScreen/MissionImages/Campaign4/BattleMarianas.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageOkinawaBeach, "Images/Frontend/CampaignScreen/MissionImages/Campaign4/BattleOkinawaBeach.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageYamato, "Images/Frontend/CampaignScreen/MissionImages/Campaign4/YamatoDuringTrialService.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendMissionImageCrewBeforeWildCat, "Images/Frontend/CampaignScreen/MissionImages/Extra/CrewBeforeWildCat.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//PopUps
		Add(EImageIdFrontendPopUpBackground, "Images/Frontend/Popup/PopUpBackground.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendPopUpButton, "Images/Frontend/Popup/PopUpButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendPopUpButtonSelected, "Images/Frontend/Popup/PopUpButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendPopUpProgressBar, "Images/Frontend/Popup/PopUpProgressBar.png", EGraphicsEngineTextureLoadTypeWithoutMasking, false);
		Add(EImageIdFrontendPopUpTextBackground, "Images/Frontend/Popup/PopUpTextBackground.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendPopUpWarningSign, "Images/Frontend/Popup/PopUpWarningSign.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Keyboard
		Add(EImageIdFrontendKeyboardAcceptButton, "Images/Frontend/KeyboardScreen/KeyboardAcceptButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardAcceptButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardAcceptButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardBackspaceButton, "Images/Frontend/KeyboardScreen/KeyboardBackspaceButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardBackspaceButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardBackspaceButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardCapsButton, "Images/Frontend/KeyboardScreen/KeyboardCapsButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardCapsButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardCapsButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardDefaultBigButton, "Images/Frontend/KeyboardScreen/KeyboardDefaultBigButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardDefaultBigButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardDefaultBigButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardDefaultButton, "Images/Frontend/KeyboardScreen/KeyboardDefaultButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardDefaultButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardDefaultButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardNewLineButton, "Images/Frontend/KeyboardScreen/KeyboardNewLineButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardNewLineButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardNewLineButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardSpacebarButton, "Images/Frontend/KeyboardScreen/KeyboardSpacebarButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendKeyboardSpacebarButtonSelected, "Images/Frontend/KeyboardScreen/KeyboardSpacebarButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Campaign Briefing Screen
		Add(EImageIdFrontendCampaignBriefingScreen, "Images/Frontend/CampaignScreen/CampaignBriefingScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		//Campaign Briefing Maps
		Add(EImageIdFrontendCampaignBriefingScreenMap1941, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/CampaignBriefingMap1941PreWar.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1942, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/CampaignBriefingMap1942Jun.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1943, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/CampaignBriefingMap1943Jul.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1944, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/CampaignBriefingMap1944Jun.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1945, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/CampaignBriefingMap1945May.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1941Small, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/SmallVersions/SmallCampaignBriefingMap1941PreWar.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1942Small, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/SmallVersions/SmallCampaignBriefingMap1942Jun.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1943Small, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/SmallVersions/SmallCampaignBriefingMap1943Jul.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1944Small, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/SmallVersions/SmallCampaignBriefingMap1944Jun.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignBriefingScreenMap1945Small, "Images/Frontend/CampaignScreen/CampaignBriefingMaps/SmallVersions/SmallCampaignBriefingMap1945May.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Campaign DebriefingImages
		Add(EImageIdFrontendCampaignDebriefingImageAmericanDisplayFlag, "Images/Frontend/CampaignScreen/DebriefingImages/AmericanDisplayCapturedAmericanFlag.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageGilbertArtillery, "Images/Frontend/CampaignScreen/DebriefingImages/GilbertArtillery.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageHighscoolWaveKamikaze, "Images/Frontend/CampaignScreen/DebriefingImages/ChiranHighSchoolGirlsWaveKamikazePilot.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageJapanSurrender, "Images/Frontend/CampaignScreen/DebriefingImages/JapanSurrender.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageAmericanDisplayFlagSmall, "Images/Frontend/CampaignScreen/DebriefingImages/SmallVersions/SmallAmericanDisplayCapturedAmericanFlag.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageGilbertArtillerySmall, "Images/Frontend/CampaignScreen/DebriefingImages/SmallVersions/SmallGilbertArtillery.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageHighscoolWaveKamikazeSmall, "Images/Frontend/CampaignScreen/DebriefingImages/SmallVersions/SmallChiranHighSchoolGirlsWaveKamikazePilot.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendCampaignDebriefingImageJapanSurrenderSmall, "Images/Frontend/CampaignScreen/DebriefingImages/SmallVersions/SmallJapanSurrender.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Shared
		Add(EImageIdFrontendSharedMenuNavigationButton, "Images/Frontend/Shared/MenuNavigationBlankButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendSharedMenuNavigationButtonSelected, "Images/Frontend/Shared/MenuNavigationBlankButtonSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendSharedVerticalScrollBarSmall, "Images/Frontend/Shared/VerticalScrollBarSmall.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendSharedVerticalScrollBarSmallSelected, "Images/Frontend/Shared/VerticalScrollBarSmallSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendSharedHorizontalScrollBarSmall, "Images/Frontend/Shared/HorizontalScrollBarSmall.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendSharedHorizontalScrollBarSmallSelected, "Images/Frontend/Shared/HorizontalScrollBarSmallSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdFrontendSharedDescriptionBackground, "Images/Frontend/MissionBriefingScreen/MissionBriefingScreenInstructionBackground.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Company Image
		Add(EImageIdCompanyImage, "Images/Frontend/CompanyImageScreen/CompanySymbol.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Loading Screen Images
		Add(EImageIdLoadingScreenAmericansOnGilberyBigExplosion, "Images/Frontend/LoadingImages/AmericansOnGilberyBigExplosion.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdLoadingScreenBunkerHillHitByTwoKamikazes, "Images/Frontend/LoadingImages/USSBunkerHillHitTwoKamikazes.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdLoadingScreenBattleIwoJimaLanding, "Images/Frontend/LoadingImages/BattleIwoJimaLanding.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdLoadingScreenPhilippinesShermanTank, "Images/Frontend/LoadingImages/PhilippinesShermanTank.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdLoadingScreenAmericanFleetAndHellDiver, "Images/Frontend/LoadingImages/AmericanFleetAndHellDiver.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdLoadingScreenParachuteBombAttack, "Images/Frontend/LoadingImages/ParachuteBombAttack.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Mission Map Images
		Add(EImageIdMapTutorial1, "Images/Frontend/GameMaps/TutorialMap1.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapTutorial2, "Images/Frontend/GameMaps/TutorialMap2.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapTutorial3, "Images/Frontend/GameMaps/TutorialMap3.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign1Mission1, "Images/Frontend/GameMaps/Campaign1Map1.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign1Mission2, "Images/Frontend/GameMaps/Campaign1Map2.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign1Mission3, "Images/Frontend/GameMaps/Campaign1Map3.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign1Mission4, "Images/Frontend/GameMaps/Campaign1Map4.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign2Mission1, "Images/Frontend/GameMaps/Campaign2Map1.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign2Mission2, "Images/Frontend/GameMaps/Campaign2Map2.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign2Mission3, "Images/Frontend/GameMaps/Campaign2Map3.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign2Mission4, "Images/Frontend/GameMaps/Campaign2Map4.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign3Mission1, "Images/Frontend/GameMaps/Campaign3Map1.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign3Mission2, "Images/Frontend/GameMaps/Campaign3Map2.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign3Mission3, "Images/Frontend/GameMaps/Campaign3Map3.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign3Mission4, "Images/Frontend/GameMaps/Campaign3Map4.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign4Mission1, "Images/Frontend/GameMaps/Campaign4Map1.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign4Mission2, "Images/Frontend/GameMaps/Campaign4Map2.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign4Mission3, "Images/Frontend/GameMaps/Campaign4Map3.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdMapCampaign4Mission4, "Images/Frontend/GameMaps/Campaign4Map4.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//Tutorial Images
		Add(EImageIdTutorialScreen, "Images/Backend/Tutorial/TutorialScreen.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdTutorialControlsAvoidAA, "Images/Backend/Tutorial/TutorialAvoidAA.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsLanding, "Images/Backend/Tutorial/TutorialLandingOnShip.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsSlowDown, "Images/Backend/Tutorial/TutorialSlowDown.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsSpeedUp, "Images/Backend/Tutorial/TutorialSpeedUp.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsStartFromCarrier, "Images/Backend/Tutorial/TutorialStartFromCarrier.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsTurn360Degree, "Images/Backend/Tutorial/TutorialTurn360DegreeImage.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsTurnLeftAndRight, "Images/Backend/Tutorial/TutorialTurnLeftAndRight.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialControlsZoomInAndOut, "Images/Backend/Tutorial/TutorialZoomInAndOutButtons.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsAttackBomber, "Images/Backend/Tutorial/WeaponsTutorialAttackBomber.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsAttackFighter, "Images/Backend/Tutorial/WeaponsTutorialAttackFighter.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsDiveBombing, "Images/Backend/Tutorial/WeaponsTutorialDiveBombing.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsKillSoldiersUseMachineGun, "Images/Backend/Tutorial/WeaponsTutorialKillSoldiersUseMachineGun.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsProtectSoldiers, "Images/Backend/Tutorial/WeaponsTutorialProtectSoldiers.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsTorpedoAttack, "Images/Backend/Tutorial/WeaponsTutorialTorpedoAttack.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsUse100Kg, "Images/Backend/Tutorial/WeaponsTutorialUse100Kg.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsUse500Kg, "Images/Backend/Tutorial/WeaponsTutorialUse500Kg.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsUse1000Kg, "Images/Backend/Tutorial/WeaponsTutorialUse1000Kg.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsUseClusterBomb, "Images/Backend/Tutorial/WeaponsTutorialUseClusterBomb.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialWeaponsFireButtons, "Images/Backend/Tutorial/WeaponsTutorialUseFireButtons.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialExtraAirport, "Images/Backend/Tutorial/ExtraAirport.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialExtraComStation, "Images/Backend/Tutorial/ExtraComStation.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialExtraTutorialAAGun, "Images/Backend/Tutorial/ExtraTutorialAAGuns.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialExtraChangeSongScreen, "Images/Backend/Tutorial/ExtraTutorialChangeSongScreen.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdTutorialExtraTutorialHud, "Images/Backend/Tutorial/ExtraTutorialHud.png", EGraphicsEngineTextureLoadTypeWithoutMasking);

		//--------------------------- In GAME IMAGES ------------------------//
				//-----------------------------------------------------------//
		//------ Aircraft ----//
		//Dauntless
		Add(EImageIdAircraftDauntlessNormalSequence, "Images/Backend/Airplanes/Dauntless/DauntlessNormalSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftDauntlessFireSequence, "Images/Backend/Airplanes/Dauntless/DauntlessFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftDauntlessTurnSequence, "Images/Backend/Airplanes/Dauntless/DauntlessTurnSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftDauntlessCrashed, "Images/Backend/Airplanes/Dauntless/DauntlessCrashed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftDauntlessLandingGear, "Images/Backend/Airplanes/Dauntless/DauntlessLandingGear.png", EGraphicsEngineTextureLoadTypeBasic);
		//Mitchell
		Add(EImageIdAircraftMitchellNormalSequence, "Images/Backend/Airplanes/Mitchell/MitchellNormalSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftMitchellCrashed, "Images/Backend/Airplanes/Mitchell/MitchellCrash.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftMitchellTurnSequence, "Images/Backend/Airplanes/Mitchell/MitchellTurnSequence.png", EGraphicsEngineTextureLoadTypePNG);
		//Val
		Add(EImageIdAircraftValNormalSequence, "Images/Backend/Airplanes/Val/ValNormalSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftValFireSequence, "Images/Backend/Airplanes/Val/ValFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftValTurnSequence, "Images/Backend/Airplanes/Val/ValTurnSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftValCrashed, "Images/Backend/Airplanes/Val/ValCrashed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftValLandingGear, "Images/Backend/Airplanes/Val/ValLandingGear.png", EGraphicsEngineTextureLoadTypeBasic);
		//Betty
		Add(EImageIdAircraftBettyNormalSequence, "Images/Backend/Airplanes/Betty/BettyNormalSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdAircraftBettyCrashed, "Images/Backend/Airplanes/Betty/BettyCrash.png", EGraphicsEngineTextureLoadTypeBasic);
		//BackCannon
		Add(EImageIdAircraftBackCannon, "Images/Backend/Airplanes/BackCannons/AircraftBackCannonSequence.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdAircraftDoubleBackCannon, "Images/Backend/Airplanes/BackCannons/DoubleBackCannonSequence.png", EGraphicsEngineTextureLoadTypePNG);
		//------ Backgrounds ----//
		Add(EImageIdBackgroundBlueDay, "Images/Backend/Backgrounds/NormalBlueBackground.png", EGraphicsEngineTextureLoadTypeWithoutMasking, false);
		//------ AircraftWeapons ------//
		Add(EImageIdAircraftWeaponCannon20mm, "Images/Backend/AircraftWeapons/AircraftCannon20mmFireSequence.png", EGraphicsEngineTextureLoadTypePNG);
		Add(EImageIdAircraftWeaponMachineGun13mm, "Images/Backend/AircraftWeapons/AircraftMachineGun13mmFireSequence.png", EGraphicsEngineTextureLoadTypePNG);
		Add(EImageIdAircraftWeaponMachineGun8mm, "Images/Backend/AircraftWeapons/AircraftMachineGun8mmFireSequence.png", EGraphicsEngineTextureLoadTypePNG);
		//------ Bombs ----//
		Add(EImageIdBomb100KgBomb, "Images/Backend/Bombs/100KgBomb.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBomb500KgBomb, "Images/Backend/Bombs/500KgBomb.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBomb1000KgBomb, "Images/Backend/Bombs/1000KgBomb.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBombClusterBomb, "Images/Backend/Bombs/ClusterBomb.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBombTorpedo, "Images/Backend/Bombs/Torpedo.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		//------ Buildings ----//
		//Airport
		Add(EImageIdBuildingAirportMainBuilding, "Images/Backend/Buildings/Airport/AirportMainBuilding.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAirportMainBuildingBroken1, "Images/Backend/Buildings/Airport/AirportMainBuildingBroken1.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAirportMainBuildingBroken2, "Images/Backend/Buildings/Airport/AirportMainBuildingBroken2.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAirportHangar, "Images/Backend/Buildings/Airport/AirportHangar.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAirportHangarBroken1, "Images/Backend/Buildings/Airport/AirportHangarBroke1.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAirportHangarBroken2, "Images/Backend/Buildings/Airport/AirportHangarBroke2.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAirportRunway, "Images/Backend/Buildings/Airport/AirportRunway.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//ComStation
		Add(EImageIdBuildingComStation, "Images/Backend/Buildings/ComStation/ComStation.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingComStationBroken1, "Images/Backend/Buildings/ComStation/ComStationBroken1.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingComStationBroken2, "Images/Backend/Buildings/ComStation/ComStationBroken2.png", EGraphicsEngineTextureLoadTypeBasic);
		//FlagPole
		Add(EImageIdBuildingFlagPole, "Images/Backend/Buildings/FlagPole/FlagPole.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdBuildingFlagPoleAmericanFlagSequence, "Images/Backend/Buildings/FlagPole/SequenceAmericanFlagBig.png", EGraphicsEngineTextureLoadTypeBasic, false);
		Add(EImageIdBuildingFlagPoleJapaneseFlagSequence, "Images/Backend/Buildings/FlagPole/SequenceJapaneseFlagBig.png", EGraphicsEngineTextureLoadTypePNG, false);
		//HQ
		Add(EImageIdBuildingHQ, "Images/Backend/Buildings/HQ/HQnormal.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingHQBroken1, "Images/Backend/Buildings/HQ/HQbroken1.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingHQBroken2, "Images/Backend/Buildings/HQ/HQbroken2.png", EGraphicsEngineTextureLoadTypeBasic);
		//AA20mmMachineCannon
		Add(EImageIdBuildingAA20mmMachineCannon, "Images/Backend/Buildings/AA20mmMachineCannon/AA20mmMachineCannon.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBuildingAA20mmMachineCannonBroken, "Images/Backend/Buildings/AA20mmMachineCannon/AA20mmMachineCannonBroken.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAA20mmMachineCannonGun, "Images/Backend/Buildings/AA20mmMachineCannon/AA20mmMachineCannonGun.png", EGraphicsEngineTextureLoadTypeBasic);
		//AA75mmType88
		Add(EImageIdBuildingAA75mmType88, "Images/Backend/Buildings/AA75mmType88/AA75mmType88.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAA75mmType88Broken, "Images/Backend/Buildings/AA75mmType88/AA75mmType88Broken.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAA75mmType88Gun, "Images/Backend/Buildings/AA75mmType88/AA75mmType88Gun.png", EGraphicsEngineTextureLoadTypeBasic);
		//AA Concrete
		Add(EImageIdBuildingAAConcrete, "Images/Backend/Buildings/AAConcrete/AAConcrete.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBuildingAAConcreteBroken, "Images/Backend/Buildings/AAConcrete/AAConcreteBroken.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBuildingAAConcreteGun, "Images/Backend/Buildings/AAConcrete/AAConcreteCanon.png", EGraphicsEngineTextureLoadTypeBasic);
		//------ Bullets ----//
		Add(EImageIdBullet7InchExplosiveShell, "Images/Backend/Bullets/7InchExplosiveShell.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet14InchExplosiveShell, "Images/Backend/Bullets/14InchExplosiveShell.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet75mmExplosiveShell, "Images/Backend/Bullets/75mmExplosiveShell.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet88mmExplosiveShell, "Images/Backend/Bullets/88mmExplosiveShell.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet80mmTankShell, "Images/Backend/Bullets/80mmTankShell.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBulletBulletRifleAmerican, "Images/Backend/Bullets/RifleBulletAmerican.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet8mmBulletAmerican, "Images/Backend/Bullets/8mmBulletAmerican.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet13mmBulletAmerican, "Images/Backend/Bullets/13mmBulletAmerican.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBulletCannon20mmBulletAmerican, "Images/Backend/Bullets/20mmBulletCannonAmerican.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBulletBulletRifleJapanese, "Images/Backend/Bullets/RifleBulletJapanese.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet8mmBulletJapanese, "Images/Backend/Bullets/8mmBulletJapanese.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdBullet13mmBulletJapanese, "Images/Backend/Bullets/13mmBulletJapanese.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBulletCannon20mmBulletJapanese, "Images/Backend/Bullets/20mmBulletCannonJapanese.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdBulletPropelledGrenade, "Images/Backend/Bullets/PropelledGrenade.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		//------ Clouds ----//
		Add(EImageIdCloudAirplaneSmokeSequence, "Images/Backend/Clouds/AirplaneSmokeSequence.png", EGraphicsEngineTextureLoadTypePNG);
		Add(EImageIdCloudSmokeSequence, "Images/Backend/Clouds/SequenceSmoke.png", EGraphicsEngineTextureLoadTypePNG);
		//-------------------------------- Explosions --------------------------//
		Add(EImageIdExplosion14InchShell, "Images/Backend/Explosions/14InchShellExplosionSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion14InchShellInWater, "Images/Backend/Explosions/14InchShellExplosionWaterSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion100KgShell, "Images/Backend/Explosions/100kgExplosionSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion100KgShellInAir, "Images/Backend/Explosions/100kgExplosionInAirSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion100KgShellInWater, "Images/Backend/Explosions/100kgExplosionWaterSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion500KgShell, "Images/Backend/Explosions/500kgExplosionSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion500KgShellInAir, "Images/Backend/Explosions/500kgExplosionInAirSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion500KgShellInWater, "Images/Backend/Explosions/500kgExplosionWaterSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion1000KgShell, "Images/Backend/Explosions/1000kgExplosionSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion1000KgShellInWater, "Images/Backend/Explosions/1000kgExplosionWaterSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosion1000KgShellInAir, "Images/Backend/Explosions/1000kgExplosionInAirSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdExplosionWaterSplash, "Images/Backend/Explosions/WaterSplashSequence.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdExplosionTorpedoGas, "Images/Backend/Explosions/TorpedoGasSequence.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		//-------------------------------- Ground --------------------------//
		Add(EImageIdGroundSandDirtFloor, "Images/Backend/Ground/SandDirtFloor.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdGroundWater1, "Images/Backend/Ground/WaterSequence1.png", EGraphicsEngineTextureLoadTypeWithoutMasking, false);
		Add(EImageIdGroundWater2, "Images/Backend/Ground/WaterSequence2.png", EGraphicsEngineTextureLoadTypeWithoutMasking, false);
		Add(EImageIdGroundWater3, "Images/Backend/Ground/WaterSequence3.png", EGraphicsEngineTextureLoadTypeWithoutMasking, false);
		Add(EImageIdGroundWater4, "Images/Backend/Ground/WaterSequence4.png", EGraphicsEngineTextureLoadTypeWithoutMasking, false);
		//-------------------------------- Ground Units --------------------------//
		//AmericanAAShermanTank
		Add(EImageIdGroundUnitAAShermanTankDriving, "Images/Backend/GroundUnits/AmericanAAShermanTank/AAShermanTankDrivingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAAShermanTankDestroyed, "Images/Backend/GroundUnits/AmericanAAShermanTank/ShermanAATankDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAAShermanTankAAGun, "Images/Backend/GroundUnits/AmericanAAShermanTank/TankAAGun.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		//American ShermanTank
		Add(EImageIdGroundUnitShermanTankDriving, "Images/Backend/GroundUnits/AmericanShermanTank/ShermanTankDrivingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitShermanTankDestroyed, "Images/Backend/GroundUnits/AmericanShermanTank/ShermanTankDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitShermanTankShooting, "Images/Backend/GroundUnits/AmericanShermanTank/ShermanTankShootingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		//American Rifleman
		Add(EImageIdGroundUnitAmericanRiflemanStanding, "Images/Backend/GroundUnits/AmericanRifleman/AmericanRiflemanStanding.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRiflemanWalking, "Images/Backend/GroundUnits/AmericanRifleman/AmericanRiflemanWalkingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRiflemanShooting, "Images/Backend/GroundUnits/AmericanRifleman/AmericanRiflemanShootingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRiflemanFalling, "Images/Backend/GroundUnits/AmericanRifleman/AmericanRiflemanFalling.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRiflemanDeadOnFloor, "Images/Backend/GroundUnits/AmericanRifleman/AmericanRiflemanDeadOnFloor.png", EGraphicsEngineTextureLoadTypeBasic);
		//American Rocketman
		Add(EImageIdGroundUnitAmericanRocketmanStanding, "Images/Backend/GroundUnits/AmericanRocketman/AmericanRocketmanStanding.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRocketmanWalking, "Images/Backend/GroundUnits/AmericanRocketman/AmericanRocketmanWalkingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRocketmanShooting, "Images/Backend/GroundUnits/AmericanRocketman/AmericanRocketmanShootingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRocketmanFalling, "Images/Backend/GroundUnits/AmericanRocketman/AmericanRocketmanFalling.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitAmericanRocketmanDeadOnFloor, "Images/Backend/GroundUnits/AmericanRocketman/AmericanRocketmanDeadOnFloor.png", EGraphicsEngineTextureLoadTypeBasic);
		//Japanese 97 Chi-Ha Tank
		Add(EImageIdGroundUnitJapanese97ChiHaTankDriving, "Images/Backend/GroundUnits/Japanese97ChiHaTank/Japanese97ChiHaTankDrivingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapanese97ChiHaTankDestroyed, "Images/Backend/GroundUnits/Japanese97ChiHaTank/Japanese97ChiHaTankDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapanese97ChiHaTankShooting, "Images/Backend/GroundUnits/Japanese97ChiHaTank/Japanese97ChiHaTankShootingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		//Japanese AA 97 Chi-Ha Tank
		Add(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving, "Images/Backend/GroundUnits/JapaneseAA97ChiHaTank/JapaneseAA97ChiHaTankDrivingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseAA97ChiHaTankDestroyed, "Images/Backend/GroundUnits/JapaneseAA97ChiHaTank/JapaneseAA97ChiHaTankDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		//Japanese Rifleman
		Add(EImageIdGroundUnitJapaneseRiflemanStanding, "Images/Backend/GroundUnits/JapaneseRifleman/JapaneseRiflemanStanding.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRiflemanWalking, "Images/Backend/GroundUnits/JapaneseRifleman/JapaneseRiflemanWalkingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRiflemanShooting, "Images/Backend/GroundUnits/JapaneseRifleman/JapaneseRiflemanShootingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRiflemanFalling, "Images/Backend/GroundUnits/JapaneseRifleman/JapaneseRiflemanFalling.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRiflemanDeadOnFloor, "Images/Backend/GroundUnits/JapaneseRifleman/JapaneseRiflemanDeadOnFloor.png", EGraphicsEngineTextureLoadTypeBasic);
		//Japanese Rocketman
		Add(EImageIdGroundUnitJapaneseRocketmanStanding, "Images/Backend/GroundUnits/JapaneseRocketman/JapaneseRocketmanStanding.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRocketmanWalking, "Images/Backend/GroundUnits/JapaneseRocketman/JapaneseRocketmanWalkingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRocketmanShooting, "Images/Backend/GroundUnits/JapaneseRocketman/JapaneseRocketmanShootingSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRocketmanFalling, "Images/Backend/GroundUnits/JapaneseRocketman/JapaneseRocketmanFalling.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdGroundUnitJapaneseRocketmanDeadOnFloor, "Images/Backend/GroundUnits/JapaneseRocketman/JapaneseRocketmanDeadOnFloor.png", EGraphicsEngineTextureLoadTypeBasic);
		//-------------------------------- HUD --------------------------//
		Add(EImageIdHudPauseMenuBackground, "Images/Backend/Hud/PauseMenuBackground.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudTopHud, "Images/Backend/Hud/Hud.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdHudSpeedometer, "Images/Backend/Hud/Speedometer.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault, false);
		Add(EImageIdHudEnemyBomberLocationArrow, "Images/Backend/Hud/EnemyBomberLocationArrow.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdHudEnemySmallPlaneLocationArrow, "Images/Backend/Hud/EnemySmallPlaneLocationArrow.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdHudArmentHudBombTypes100KgSelected, "Images/Backend/Hud/Arment_Hud_Bomb_Types_100kgSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudBombTypes500KgSelected, "Images/Backend/Hud/Arment_Hud_Bomb_Types_500kgSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudBombTypes1000KgSelected, "Images/Backend/Hud/Arment_Hud_Bomb_Types_1000kgSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudBombTypesClusterSelected, "Images/Backend/Hud/Arment_Hud_Bomb_Types_ClusterSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudBombTypesNoneSelected, "Images/Backend/Hud/Arment_Hud_Bomb_Types_NoneSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudSelectWeaponBombSelected, "Images/Backend/Hud/Arment_Hud_Select_Weapon_Type_BombSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudSelectWeaponRocketSelected, "Images/Backend/Hud/Arment_Hud_Select_Weapon_Type_RocketSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudSelectWeaponTorpedoSelected, "Images/Backend/Hud/Arment_Hud_Select_Weapon_Type_TorpedoSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudArmentHudSelectWeaponNoneSelected, "Images/Backend/Hud/Arment_Hud_Select_Weapon_Type_NoneSelected.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonBlankGreen, "Images/Backend/Hud/BlankGreenButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonBlankRed, "Images/Backend/Hud/BlankRedButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonBlankPushed, "Images/Backend/Hud/BlankPushedButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonBlankDisabled, "Images/Backend/Hud/BlankDisabledButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonDecreasedSpeed, "Images/Backend/Hud/DecreaseSpeedButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonDecreasedSpeedPushed, "Images/Backend/Hud/DecreaseSpeedButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonDecreasedSpeedDisabled, "Images/Backend/Hud/DecreaseSpeedButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonIncreaseSpeed, "Images/Backend/Hud/IncreaseSpeedButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonIncreaseSpeedPushed, "Images/Backend/Hud/IncreaseSpeedButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonIncreaseSpeedDisabled, "Images/Backend/Hud/IncreaseSpeedButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonDown, "Images/Backend/Hud/DownButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonDownPushed, "Images/Backend/Hud/DownButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonDownDisabled, "Images/Backend/Hud/DownButtonControlVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonUp, "Images/Backend/Hud/UpButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonUpPushed, "Images/Backend/Hud/UpButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonUpDisabled, "Images/Backend/Hud/UpButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonLeft, "Images/Backend/Hud/LeftButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonLeftPushed, "Images/Backend/Hud/LeftButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonLeftDisabled, "Images/Backend/Hud/LeftButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonRight, "Images/Backend/Hud/RightButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonRightPushed, "Images/Backend/Hud/RightButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonRightDisabled, "Images/Backend/Hud/RightButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonPrimaryFire, "Images/Backend/Hud/PrimaryFireButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonPrimaryFirePushed, "Images/Backend/Hud/PrimaryFireButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonPrimaryFireDisabled, "Images/Backend/Hud/PrimaryFireButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonSecondaryFire, "Images/Backend/Hud/SecondaryFireButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonSecondaryFirePushed, "Images/Backend/Hud/SecondaryFireButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonSecondaryFireDisabled, "Images/Backend/Hud/SecondaryFireButtonLongVDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonZoomIn, "Images/Backend/Hud/ZoomInButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonZoomInPushed, "Images/Backend/Hud/ZoomInButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonZoomInDisabled, "Images/Backend/Hud/ZoomInButtonLongVDisabeled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonZoomOut, "Images/Backend/Hud/ZoomOutButtonLongV.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonZoomOutPushed, "Images/Backend/Hud/ZoomOutButtonLongVPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonZoomOutDisabled, "Images/Backend/Hud/ZoomOutButtonLongVDisabeled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonMenu, "Images/Backend/Hud/MenuButton.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonMenuPushed, "Images/Backend/Hud/MenuButtonPushed.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		Add(EImageIdHudButtonMenuDisabled, "Images/Backend/Hud/MenuButtonDisabled.png", EGraphicsEngineTextureLoadTypeWithoutMasking);
		//-------------------------------- Rocket --------------------------//
		Add(EImageIdRocketStandardRocketNormalView, "Images/Backend/Rockets/StandardRocket/StandardRocketNormalView.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdRocketStandardRocketFireSequence, "Images/Backend/Rockets/StandardRocket/StandardRocketFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);

		//-------------------------------- Ship --------------------------//
		//American
		Add(EImageIdShipAmericanBattleshipUSSTennessee, "Images/Backend/Ships/AmericanBattleshipUSSTennessee/AmericanBattleshipUSSTennessee.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanCarrierUSSLexington, "Images/Backend/Ships/AmericanCarrierUSSLexington/AmericanCarrierUSSLexington.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanCarrierUSSVictory, "Images/Backend/Ships/Victory/Victory.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanDestroyer, "Images/Backend/Ships/AmericanDestroyer/AmericanDestroyer.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanTroopTransporter, "Images/Backend/Ships/AmericanTroopTransporter/AmericanTroopTransporter.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanLCTEmpty, "Images/Backend/Ships/AmericanLCT/AmericanLCTEmpty.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanLCTLoadedWithSoldiers, "Images/Backend/Ships/AmericanLCT/AmericanLCTLoadedWithTroops.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanLCTLoadedWithTanks, "Images/Backend/Ships/AmericanLCT/AmericanLCTWithTanks.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipAmericanLCTLoadedWithSoldiersAndTanks, "Images/Backend/Ships/AmericanLCT/AmericanLCTLoadedWithTroopsAndTank.png", EGraphicsEngineTextureLoadTypeBasic);
		//Japanese
		Add(EImageIdShipJapaneseBattleshipYamato, "Images/Backend/Ships/JapaneseBattleshipYamato/JapaneseBattleshipYamato.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseCarrierShoho, "Images/Backend/Ships/JapaneseCarrierShoho/JapaneseCarrierShoho.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseCarrierShokaku, "Images/Backend/Ships/JapaneseCarrierShokaku/JapaneseCarrierShokaku.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseCarrierTaiho, "Images/Backend/Ships/JapaneseCarrierTaiho/JapaneseCarrierTaiho.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseDestroyer, "Images/Backend/Ships/JapaneseDestroyer/JapaneseDestroyer.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseTroopTransporter, "Images/Backend/Ships/JapaneseTroopTransporter/JapaneseTroopTransporter.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseOilTanker, "Images/Backend/Ships/JapaneseOilTanker/JapaneseOilTanker.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseLCTEmpty, "Images/Backend/Ships/JapaneseLCT/JapaneseLCTEmpty.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseLCTLoadedWithSoldiers, "Images/Backend/Ships/JapaneseLCT/JapaneseLCTLoadedWithTroops.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseLCTLoadedWithTanks, "Images/Backend/Ships/JapaneseLCT/JapaneseLCTWithTanks.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipJapaneseLCTLoadedWithSoldiersAndTanks, "Images/Backend/Ships/JapaneseLCT/JapaneseLCTLoadedWithTroopsAndTank.png", EGraphicsEngineTextureLoadTypeBasic);
		//Ship Guns
		Add(EImageIdShipGun7InchGunFire, "Images/Backend/Ships/ShipGuns/7InchGun/7InchGunFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGun7InchGunTurret, "Images/Backend/Ships/ShipGuns/7InchGun/7InchGunTurret.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGun14InchGunFire, "Images/Backend/Ships/ShipGuns/14InchGun/14InchGunFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGun14InchGunTurret, "Images/Backend/Ships/ShipGuns/14InchGun/14InchGunTurret.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdShipGunDual75mmGunFire, "Images/Backend/Ships/ShipGuns/Dual75mmGunTurret/Dual75mmGunFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunDual75mmGunTurret, "Images/Backend/Ships/ShipGuns/Dual75mmGunTurret/Dual75mmGunTurret.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunDualMachineGunFire, "Images/Backend/Ships/ShipGuns/DualMachineGunTurret/DualMachineGunFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunDualMachineGunTurret, "Images/Backend/Ships/ShipGuns/DualMachineGunTurret/DualMachineGunTurret.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunCamoflage7InchGunFire, "Images/Backend/Ships/ShipGuns/Camouflage7InchGun/Camoflage7InchGunFireSequence.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunCamoflage7InchGunTurret, "Images/Backend/Ships/ShipGuns/Camouflage7InchGun/Camoflage7InchGunTurret.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunCamoflageDual75mmGunTurret, "Images/Backend/Ships/ShipGuns/CamouflageDual75mmGunTurret/CamoflageDual75mmGunTurret.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdShipGunCamoflageDualMachineGunTurret, "Images/Backend/Ships/ShipGuns/CamouflageDualMachineGunTurret/CamoflageDualMachineGunTurret.png", EGraphicsEngineTextureLoadTypeBasic);
		//-------------------------------- Trees --------------------------//
		Add(EImageIdTreeBigTree, "Images/Backend/Trees/BigTree.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreeBigTreeDestroyed, "Images/Backend/Trees/BigTreeDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreePalmTreeBowed, "Images/Backend/Trees/PalmTreeBowed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreePalmTreeBowedDestroyed, "Images/Backend/Trees/PalmTreeBowedDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreePalmTreeMiddle, "Images/Backend/Trees/PalmTreeMiddle.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreePalmTreeMiddleDestroyed, "Images/Backend/Trees/PalmTreeMiddleDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreeSmallTree, "Images/Backend/Trees/SmallTree.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreeSmallTreeDestroyed, "Images/Backend/Trees/SmallTreeDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);
		Add(EImageIdTreeTallThinTree, "Images/Backend/Trees/TallThinTree.png", EGraphicsEngineTextureLoadTypeWithReadingByDefault);
		Add(EImageIdTreeTallThinTreeDestroyed, "Images/Backend/Trees/TallThinTreeDestroyed.png", EGraphicsEngineTextureLoadTypeBasic);

	}
}
