/*
 * CCreditsView.cpp
 *
 *  Created on: Sep 20, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CCreditsView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CUserStats.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/views/transitions/CTransitionSlideAngled.h"

#define CREDITS_SCREEN_PIXELS_OFFSET_TITLE 60
#define CREDITS_SCREEN_PIXELS_OFFSET_NAME 20

CCreditsView::CCreditsView()
{
	iYOffsetValue = 660 / 2; //start the middle of the screen
	iMaxYOffsetValue = 660; //we want that we reloop after the last part of the credits vanishes from the screen
}

CCreditsView::~CCreditsView()
{
//	//change back to previous Song
//	if(iWasPlayingAUserSong)
//	{
//		CGame::Game->iAudioEngine->GetMusicPlayer().PlayUserSong(iStringSongName->GetData(), true);
//	}
//	else
//	{
//		CGame::Game->iAudioEngine->GetMusicPlayer().PlayGameSong(iStringSongName->GetData(), true);
//	}

	if(iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}
	if(iGameObjectTexture != NULL)
	{
		delete iGameObjectTexture;
		iGameObjectTexture = NULL;
	}

	delete iStringBackButton;
	delete iStringSongName;
	delete iBackButton;
	delete iScrollSurface;

	for(TInt lIndex = 0; lIndex < iCreditEntries->GetCount(); lIndex++)
	{
		if(iCreditEntries->Get(lIndex)->SectionString != NULL)
		{
			delete iCreditEntries->Get(lIndex)->SectionString;
		}
		if(iCreditEntries->Get(lIndex)->TitleString != NULL)
		{
			delete iCreditEntries->Get(lIndex)->TitleString;
		}
		if(iCreditEntries->Get(lIndex)->NameString != NULL)
		{
			delete iCreditEntries->Get(lIndex)->NameString;
		}
	}
	iCreditEntries->ClearAndDestroy();
	delete iCreditEntries;

}

CCreditsView* CCreditsView::New()
{
	CCreditsView* lSelf = new CCreditsView();
	lSelf->Construct();
	return lSelf;
}

void CCreditsView::Construct()
{
	//change to Credits Music
	//get settings of previous Song
	iStringSongName = new CString(CGame::Game->iAudioEngine->GetMusicPlayer().GetNameOfSong()->GetData());
	iWasPlayingAUserSong = CGame::Game->iAudioEngine->GetMusicPlayer().GetIsPlayingUserSong();
	//play the Song we want
	//CGame::Game->iAudioEngine->GetMusicPlayer().PlayGameSong(SONG_BATTLE_HYMN, true);

	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendCreditsScreenBottomPart), 1024, 85, 100, 0, 1, 0, 1);
	iGameObjectTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendCreditsScreenAmericanDestroyer), 640, 50, 100, 0, 1, 0, 1);

	//Add Entries
	iCreditEntries = CPointerArray<SCreditsEntry>::New();
	//Top Part
	AddSectionAsNextEntry("Frontend.Credits.Title_GameName");
	AddNameAsNextEntry("Frontend.Credits.Title_Whitespace");
	AddNameAsNextEntry("Frontend.Credits.Name_Version");
	AddNameAsNextEntry("Frontend.Credits.Name_CopyRightNotice");
	AddNameAsNextEntry("Frontend.Credits.Name_GameDevelopedAndPublished");
	AddNameAsNextEntry("Frontend.Credits.Name_CompanyWebsite");
	AddNameAsNextEntry("Frontend.Credits.Name_AdditionalSupport");
	AddNameAsNextEntry("Frontend.Credits.Name_Facebook");
	//Add an image
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenMitchellAndBetty);
	//Start of Actual Credits
	AddSectionAsNextEntry("Frontend.Credits.Title_Credits");
	//Developers
	AddTitleAsNextEntry("Frontend.Credits.Title_DevelopersAndDesigners");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Music
	AddTitleAsNextEntry("Frontend.Credits.Title_Music");
	AddNameAsNextEntry("Common.Credits.Tyson_McCarney");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenVictory);
	//Game Design
	AddTitleAsNextEntry("Frontend.Credits.Title_GameDesign");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//All Graphics
	AddTitleAsNextEntry("Frontend.Credits.Title_AllGraphics");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//GameObjects
	AddTitleAsNextEntry("Frontend.Credits.Title_GameObjects");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Dive Bomber AI
	AddTitleAsNextEntry("Frontend.Credits.Title_DiveBomberAI");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenDauntlessAndVal);
	//Other AI
	AddTitleAsNextEntry("Frontend.Credits.Title_OtherAI");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Mission Design And Implementation
	AddTitleAsNextEntry("Frontend.Credits.Title_MissionDesignAndImplementation");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Campaign, Mission and Tutorial Descriptions
	AddTitleAsNextEntry("Frontend.Credits.Title_CampaignMissionTutorialDescriptions");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Proof reader
	AddTitleAsNextEntry("Frontend.Credits.Title_ProofReaderAndEditor");
	AddNameAsNextEntry("Common.Credits.Lukas_Stoll");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenJapanese97ChiHa);
	//Tutorials
	AddTitleAsNextEntry("Frontend.Credits.Title_Tutorials");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Frontend Design
	AddTitleAsNextEntry("Frontend.Credits.Title_FrontEndIntefaceDesign");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Frontend Implementation
	AddTitleAsNextEntry("Frontend.Credits.Title_FrontEndIntefaceImplementation");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenSoldiers);
	//In Game Interface Design
	AddTitleAsNextEntry("Frontend.Credits.Title_InGameIntefaceDesign");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//In Game Interface Implementation
	AddTitleAsNextEntry("Frontend.Credits.Title_InGameIntefaceImplementation");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Sound editing and converting
	AddTitleAsNextEntry("Frontend.Credits.Title_SoundEditionAndConverting");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenShermanTanks);
	//Android Porting
	AddTitleAsNextEntry("Frontend.Credits.Title_AndroidPorting");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Symbian Porting
	AddTitleAsNextEntry("Frontend.Credits.Title_SymbianPorting");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Testing and Debugging
	AddTitleAsNextEntry("Frontend.Credits.Title_TestingAndDebugging");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//User Testing Organizer
	AddTitleAsNextEntry("Frontend.Credits.Title_UserTestingOrganizer");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenShokaku);
	//Game Engine
	AddTitleAsNextEntry("Frontend.Credits.Title_GameEngine");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Graphics Engine
	AddTitleAsNextEntry("Frontend.Credits.Title_GraphicsEngine");
	AddNameAsNextEntry("Common.Credits.David_Stoll");

	//Sound Engine Design
	AddTitleAsNextEntry("Frontend.Credits.Title_SoundEngineDesign");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Sound Engine Implementation
	AddTitleAsNextEntry("Frontend.Credits.Title_SoundEngineImplementation");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	//Collision Engine
	AddTitleAsNextEntry("Frontend.Credits.Title_CollisionEngine");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Font Engine
	AddTitleAsNextEntry("Frontend.Credits.Title_FontEngine");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenJapaneseBattleshipYamato);
	//Marketing
	AddTitleAsNextEntry("Frontend.Credits.Title_Marketing");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	AddNameAsNextEntry("Common.Credits.Adrian_Thorp");
	//French Translation
	AddTitleAsNextEntry("Frontend.Credits.Title_FrenchTranslation");
	AddNameAsNextEntry("Common.Credits.Adrian_Thorp");
	//Chinese Translation
	AddTitleAsNextEntry("Frontend.Credits.Title_ChineseTranslation");
	AddNameAsNextEntry("Common.Credits.Stephen_Cheung");
	//German Translation
	AddTitleAsNextEntry("Frontend.Credits.Title_GermanTranslation");
	AddNameAsNextEntry("Common.Credits.Angela_Stoll");
	AddNameAsNextEntry("Common.Credits.David_Stoll");
	//Company Website
	AddTitleAsNextEntry("Frontend.Credits.Title_CompanyWebsiteDesign");
	AddNameAsNextEntry("Common.Credits.Scott_Fuoco");
	//Images Courtesy Of
	AddTitleAsNextEntry("Frontend.Credits.Title_PublicDomainImagesCourtesyOf");
	AddNameAsNextEntry("Common.Credits.US_National_Archives_And_Records_Administration");
	AddNameAsNextEntry("Common.Credits.US_Marine_Corps");
	AddNameAsNextEntry("Common.Credits.US_Naval_History");
	AddNameAsNextEntry("Common.Credits.US_Air_Force_Museum");
	AddNameAsNextEntry("Common.Credits.Japanese_Self_Defense_Force_Archives");
	AddNameAsNextEntry("Common.Credits.Wikimedia_Commons");
	//Free Type Project
	AddTitleAsNextEntry("Common.Credits.FreeTypeTitle");
	AddNameAsNextEntry("Common.Credits.FreeTypeBodyLine1");
	AddNameAsNextEntry("Common.Credits.FreeTypeBodyLine2");
	//Special Thanks
	AddTitleAsNextEntry("Frontend.Credits.Title_SpecialThanks");
	AddNameAsNextEntry("Common.Credits.Jonas_Stoll");
	AddNameAsNextEntry("Common.Credits.Jonaz_Samson");
	// And...
	// You!
	AddTitleAsNextEntry("Frontend.Credits.Title_And");
	AddTitleAsNextEntry("Frontend.Credits.Title_Whitespace");
	AddSectionAsNextEntry("Frontend.Credits.Title_You");
	AddImageAsNextEntry(EImageIdFrontendCreditsScreenAmericanDestroyer);

	//Strings
	iStringBackButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");

	//Fonts
	iFontForBackButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringBackButton, 190, 60);

	//Find Font For Credit Sections and Entries
	CPointerArray<CString>* lSectionArray = CPointerArray<CString>::New();
	CPointerArray<CString>* lTitleArray = CPointerArray<CString>::New();
	CPointerArray<CString>* lNameArray = CPointerArray<CString>::New();
	for(TInt lIndex = 0; lIndex < iCreditEntries->GetCount(); lIndex++)
	{
		if(iCreditEntries->Get(lIndex)->SectionString!= NULL)
		{
			lSectionArray->Append(iCreditEntries->Get(lIndex)->SectionString);
		}
		else if(iCreditEntries->Get(lIndex)->TitleString != NULL)
		{
			lTitleArray->Append(iCreditEntries->Get(lIndex)->TitleString);
		}
		else if(iCreditEntries->Get(lIndex)->NameString != NULL)
		{
			lNameArray->Append(iCreditEntries->Get(lIndex)->NameString);
		}
	}
	iFontForSectionEntries = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lSectionArray, 1000, 60);
	iFontForTitleEntries = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lTitleArray, 1000, 45);
	iFontForNameEntries= iGraphicsFont->CreateFontThatFitsHorizontalStrings(lNameArray, 1000, 35);
	//clean up
	delete lSectionArray;
	delete lTitleArray;
	delete lNameArray;

	//add MaxY
	iMaxYOffsetValue += GetMaxYOffsetForEntries();
	iYOffsetValue += iMaxYOffsetValue; //need to add MaxY, since we want to start in the middle

	//Back Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(407, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.Label = iStringBackButton;
	lBackButtonArgs.FontForLabel = &iFontForBackButton;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iBackButton = CButton::New(lBackButtonArgs);

	iScrollSurface = CTouchSurface::New(TPoint(0,85), TSize(1024, 575), iYOffsetValue, iMaxYOffsetValue * 2, 2, CTouchSurface::EScrollAxisY);

	CUserStats::CreditsMenuOpenedFrontend();
}

void CCreditsView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//Scroll Surface
				iScrollSurface->CheckIfHit(aTouchEvent);
				//Back Button
				iBackButton->CheckIfHit(aTouchEvent);
				break;
			}

			case ETouchEventHandlerTouchTypeDrag:
			{
				//Scroll Surface
				iScrollSurface->CheckIfHit(aTouchEvent);
				break;
			}

			case ETouchEventHandlerTouchTypeUp:
			{
				//Scroll Surface
				iScrollSurface->CheckIfHit(aTouchEvent);
				//Back Button
				if(iBackButton->GetState() == CButton::EStateSelected && iBackButton->CheckIfHit(aTouchEvent))
				{
					CTransitionSlideAngled::New(&CGame::Game->iView, this, CMainMenuView::New(false), 1, true);
				}
				break;
			}
		}
	}
}

void CCreditsView::ChildViewDrawMethod()
{
	//clear Screen with Blue colour
	iGraphicsEngine->SetClearColourInt(65, 85, 130, 255);
	iGraphicsEngine->ClearScreen();

	//adjust Credits location
	TInt lRealYOffset = iYOffsetValue - iMaxYOffsetValue;
	//increase Y Value to make Credits move up, go back to beginning if we are above MaxY
	if(lRealYOffset > iMaxYOffsetValue)
	{
		iYOffsetValue = iMaxYOffsetValue; //reset back to 0
	}
	//credits are outside the view, move them above the view,so the user can scroll them down
	else if(lRealYOffset < 0)
	{
		iYOffsetValue += iMaxYOffsetValue;
	}
	else
	{
		iYOffsetValue += 1;
	}

	//Draw Credit Entries
	iGraphicsFont->SetColour(KColourWhite);
	TInt lTempYLocation = lRealYOffset;
	TBool lDrawString;

	for(TInt lIndex = 0; lIndex < iCreditEntries->GetCount(); lIndex++)
	{
		SCreditsEntry* lCreditEntry = iCreditEntries->Get(lIndex);
		if(lTempYLocation > -300 && lTempYLocation < 900) //only draw if in visible area
		{
			lDrawString = true;
		}
		else
		{
			lDrawString = false;
		}

		if(lCreditEntry->TitleString != NULL)
		{
			//should be displayed as a title
			lTempYLocation -= CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
			lTempYLocation -= iFontForTitleEntries.HeightAndDescentOfFont.GetIntInBaseInt();
			iGraphicsFont->SetFont(iFontForTitleEntries);
			if(lDrawString)
				DrawCenteredString(TPoint(0,lTempYLocation),iFontForTitleEntries, lCreditEntry->TitleString, 1024, iFontForTitleEntries.HeightAndDescentOfFont.GetIntInBaseInt());
		}
		else if(lCreditEntry->NameString != NULL)
		{
			//should be displayed as a name
			lTempYLocation -= CREDITS_SCREEN_PIXELS_OFFSET_NAME;
			lTempYLocation -= iFontForNameEntries.HeightAndDescentOfFont.GetIntInBaseInt();
			iGraphicsFont->SetFont(iFontForNameEntries);
			if(lDrawString)
				DrawCenteredString(TPoint(0,lTempYLocation),iFontForNameEntries, lCreditEntry->NameString, 1024, iFontForNameEntries.HeightAndDescentOfFont.GetIntInBaseInt());
		}
		else if(lCreditEntry->SectionString != NULL)
		{
			//should be displayed as a section
			lTempYLocation -= CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
			lTempYLocation -= iFontForSectionEntries.HeightAndDescentOfFont.GetIntInBaseInt();
			iGraphicsFont->SetFont(iFontForSectionEntries);
			if(lDrawString)
				DrawCenteredString(TPoint(0,lTempYLocation),iFontForSectionEntries, lCreditEntry->SectionString, 1024, iFontForSectionEntries.HeightAndDescentOfFont.GetIntInBaseInt());
		}
		else
		{
			lTempYLocation -= CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
			lTempYLocation -= iImageStore->GetImageSize(lCreditEntry->ImageId).iHeight;
			iGameObjectTexture->AssignTexture(&iImageStore->GetImage(lCreditEntry->ImageId));
			iGameObjectTexture->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(192),TIntFloat::Convert(lTempYLocation)));
			if(lDrawString)
				iGameObjectTexture->DrawArrays();
			lTempYLocation -= CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
		}
	}

	//draw Bottom Part
	iBackgroundTexture->DrawArrays();

	//draw Controls
	iScrollSurface->Draw();
	iBackButton->Draw();
}

void CCreditsView::AddSectionAsNextEntry(const char* aStringId)
{
	SCreditsEntry lEntry;
	lEntry.NameString = NULL;
	lEntry.TitleString = NULL;//tells it that this not used to display a title
	lEntry.SectionString = iLocalizationEngine->GetString(aStringId);
	iCreditEntries->Append(new SCreditsEntry(lEntry));
}

void CCreditsView::AddTitleAsNextEntry(const char* aStringId)
{
	SCreditsEntry lEntry;
	lEntry.NameString = NULL;
	lEntry.TitleString = iLocalizationEngine->GetString(aStringId);
	lEntry.SectionString = NULL;
	iCreditEntries->Append(new SCreditsEntry(lEntry));
}

void CCreditsView::AddNameAsNextEntry(const char* aStringId)
{
	SCreditsEntry lEntry;
	lEntry.NameString = iLocalizationEngine->GetString(aStringId);
	lEntry.TitleString = NULL;
	lEntry.SectionString = NULL;
	iCreditEntries->Append(new SCreditsEntry(lEntry));
}

void CCreditsView::AddImageAsNextEntry(TImageId aImageId)
{
	SCreditsEntry lEntry;
	lEntry.NameString = NULL;
	lEntry.TitleString = NULL;
	lEntry.SectionString = NULL;
	lEntry.ImageId = aImageId;
	iCreditEntries->Append(new SCreditsEntry(lEntry));
}

TInt CCreditsView::GetMaxYOffsetForEntries()
{
	TInt lMaxYUntilReloop = 0;
	for(TInt lIndex = 0; lIndex < iCreditEntries->GetCount(); lIndex++)
	{
		SCreditsEntry* lCreditEntry = iCreditEntries->Get(lIndex);

		if(lCreditEntry->TitleString != NULL)
		{
			//should be displayed as a title
			lMaxYUntilReloop += CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
			lMaxYUntilReloop += iFontForTitleEntries.HeightAndDescentOfFont.GetIntInBaseInt();;
		}
		else if(lCreditEntry->NameString != NULL)
		{
			//should be displayed as a name
			lMaxYUntilReloop += CREDITS_SCREEN_PIXELS_OFFSET_NAME;
			lMaxYUntilReloop += iFontForNameEntries.HeightAndDescentOfFont.GetIntInBaseInt();
		}
		else if(lCreditEntry->SectionString != NULL)
		{
			//should be displayed as a section
			lMaxYUntilReloop += CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
			lMaxYUntilReloop += iFontForSectionEntries.HeightAndDescentOfFont.GetIntInBaseInt();
		}
		else
		{
			lMaxYUntilReloop += CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
			lMaxYUntilReloop += iImageStore->GetImageSize(lCreditEntry->ImageId).iHeight;
			lMaxYUntilReloop += CREDITS_SCREEN_PIXELS_OFFSET_TITLE;
		}
	}

	return lMaxYUntilReloop;
}



