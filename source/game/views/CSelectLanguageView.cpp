/*
 * CSelectLanguageView.cpp
 *
 *  Created on: Sep 22, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CSelectLanguageView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CSelectLanguageView::CSelectLanguageView(TBool aFirstTimeSelectingLanguage)
{
	iLanguageSelected = ELanguageSelectedNone;
	iFirstTimeSelectingLanguage = aFirstTimeSelectingLanguage;
}

CSelectLanguageView::~CSelectLanguageView()
{
	if(iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}

	delete iStringBackButton;
	delete iStringEnglish;
	delete iStringGerman;
	delete iStringFrench;
	delete iStringChineseSimplified;
	delete iStringChineseTraditional;
	delete iBackButton;
}

CSelectLanguageView* CSelectLanguageView::New(TBool aFirstTimeSelectingLanguage)
{
	CSelectLanguageView* lSelf = new CSelectLanguageView(aFirstTimeSelectingLanguage);
	lSelf->Construct();
	return lSelf;
}

void CSelectLanguageView::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendSelectLanguageScreen), 1024, 660, 100, 0, 1, 0, 1);

	//Strings
	iStringBackButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");
	iStringEnglish = iLocalizationEngine->GetString("Common.SelectLanguage.English");
	iStringGerman = iLocalizationEngine->GetString("Common.SelectLanguage.German");
	iStringFrench = iLocalizationEngine->GetString("Common.SelectLanguage.French");
	iStringChineseSimplified = iLocalizationEngine->GetString("Common.SelectLanguage.SimplifiedChinese");
	iStringChineseTraditional = iLocalizationEngine->GetString("Common.SelectLanguage.TraditionalChinese");

	iRectEnglish = TRect(TPoint(112, 630), 800, 100);
	iRectGerman = TRect(TPoint(112, 530), 800, 100);;
	iRectFrench = TRect(TPoint(112, 430), 800, 100);;
	iRectChineseTraditional = TRect(TPoint(112, 330), 800, 100);;
	iRectChineseSimplified = TRect(TPoint(112, 230), 800, 100);;

	//Fonts
	iFontForBackButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringBackButton, 190, 60);

	CPointerArray<CString>* lLanguageArray = CPointerArray<CString>::New();
	lLanguageArray->Append(iStringEnglish);
	lLanguageArray->Append(iStringGerman);
	lLanguageArray->Append(iStringFrench);
	lLanguageArray->Append(iStringChineseSimplified);
	lLanguageArray->Append(iStringChineseTraditional);
	iFontForLanguages = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lLanguageArray, 800, 50);
	delete lLanguageArray;

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

	CUserStats::SelectLanguageMenuOpenedFrontend();
}

void CSelectLanguageView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			case ETouchEventHandlerTouchTypeDrag:
			{
				//Back Button
				iBackButton->CheckIfHit(aTouchEvent);

				if(!iBackButton->GetState() == CButton::EStateSelected)
				{
					if(iRectEnglish.Contains(aTouchEvent.VirtualLocation))
					{
						iLanguageSelected =  ELanguageSelectedEnglish;
					}
					else if(iRectGerman.Contains(aTouchEvent.VirtualLocation))
					{
						iLanguageSelected = ELanguageSelectedGerman;
					}
					else if(iRectFrench.Contains(aTouchEvent.VirtualLocation))
					{
						iLanguageSelected = ELanguageSelectedFrench;
					}
					else if(iRectChineseTraditional.Contains(aTouchEvent.VirtualLocation))
					{
						iLanguageSelected = ELanguageSelectedChineseTraditional;
					}
					else if(iRectChineseSimplified.Contains(aTouchEvent.VirtualLocation))
					{
						iLanguageSelected = ELanguageSelectedChineseSimplified;
					}
					else
					{
						iLanguageSelected = ELanguageSelectedNone;
					}
				}
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//Back Button
				if(iBackButton->GetState() == CButton::EStateSelected)
				{
					if(iBackButton->CheckIfHit(aTouchEvent))
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(iFirstTimeSelectingLanguage), 2, 1, 40);
					}
				}
				else
				{
					if(iLanguageSelected == ELanguageSelectedEnglish && iRectEnglish.Contains(aTouchEvent.VirtualLocation))
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iLanguage = ELanguageEnglish;
						iLocalizationEngine->SetLanguage(ELanguageEnglish);
						CUserStats::LanguageSelected("English");
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(iFirstTimeSelectingLanguage), 2, 1, 40);
					}
					else if(iLanguageSelected == ELanguageSelectedGerman && iRectGerman.Contains(aTouchEvent.VirtualLocation))
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iLanguage = ELanguageGerman;
						iLocalizationEngine->SetLanguage(ELanguageGerman);
						CUserStats::LanguageSelected("German");
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(iFirstTimeSelectingLanguage), 2, 1, 40);
					}
					else if(iLanguageSelected == ELanguageSelectedFrench && iRectFrench.Contains(aTouchEvent.VirtualLocation))
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iLanguage = ELanguageFrench;
						iLocalizationEngine->SetLanguage(ELanguageFrench);
						CUserStats::LanguageSelected("French");
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(iFirstTimeSelectingLanguage), 2, 1, 40);
					}
					else if(iLanguageSelected == ELanguageSelectedChineseTraditional && iRectChineseTraditional.Contains(aTouchEvent.VirtualLocation))
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iLanguage = ELanguageChineseTraditional;
						iLocalizationEngine->SetLanguage(ELanguageChineseTraditional);
						CUserStats::LanguageSelected("ChineseTraditional");
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(iFirstTimeSelectingLanguage), 2, 1, 40);
					}
					else if(iLanguageSelected == ELanguageSelectedChineseSimplified && iRectChineseSimplified.Contains(aTouchEvent.VirtualLocation))
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iLanguage = ELanguageChineseSimplified;
						iLocalizationEngine->SetLanguage(ELanguageChineseSimplified);
						CUserStats::LanguageSelected("ChineseSimplified");
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(iFirstTimeSelectingLanguage), 2, 1, 40);
					}
					else
					{
						iLanguageSelected = ELanguageSelectedNone;
					}
				}
				break;
			}
		}
	}
}

void CSelectLanguageView::ChildViewDrawMethod()
{
	iBackgroundTexture->DrawArrays();

	//Draw Options
	iGraphicsFont->SetFont(iFontForLanguages);

	//English
	if(iLanguageSelected == ELanguageSelectedEnglish) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
	DrawCenteredString(TPoint(112, 530), iFontForLanguages, iStringEnglish, 800, 100);
	//German
	if(iLanguageSelected == ELanguageSelectedGerman) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
	DrawCenteredString(TPoint(112, 430), iFontForLanguages, iStringGerman, 800, 100);
	//French
	if(iLanguageSelected == ELanguageSelectedFrench) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
	DrawCenteredString(TPoint(112, 330), iFontForLanguages, iStringFrench, 800, 100);
	//Chinese Trad
	if(iLanguageSelected == ELanguageSelectedChineseTraditional) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
	DrawCenteredString(TPoint(112, 230), iFontForLanguages, iStringChineseTraditional, 800, 100);
	//Chinese Sim
	if(iLanguageSelected == ELanguageSelectedChineseSimplified) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
	DrawCenteredString(TPoint(112, 130), iFontForLanguages, iStringChineseSimplified, 800, 100);

	//draw Controls
	iBackButton->Draw();
}

