/*
 * CPopupWarning.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: dstoll
 */

#include "includes/core/views/popups/CPopupWarning.h"

void DummyMethod(TBool aValue, void* lArgs)
{
	//do nothing
}


CPopupWarning::CPopupWarning(const SPopupBasicArgs& aArgs)
: CBasicPopup(aArgs)
{
	iStringMessageLine1 = NULL;
	iStringMessageLine2 = NULL;
	iStringMessageLine3 = NULL;
}

CPopupWarning::~CPopupWarning()
{
	delete iWarningSymbol;
	if(iStringMessageLine1 != NULL)
	{
		delete iStringMessageLine1;
	}
	if(iStringMessageLine2 != NULL)
	{
		delete iStringMessageLine2;
	}
	if(iStringMessageLine3 != NULL)
	{
		delete iStringMessageLine3;
	}
}

CPopupWarning* CPopupWarning::New(const SPopupWarningArgs& aArgs)
{

	SPopupBasicArgs lArgsForBasicPopup = {aArgs.OwningView, aArgs.LeftButtonStringId, NULL, aArgs.TitleStringId, DummyMethod, NULL};
	CPopupWarning* lSelf = new CPopupWarning(lArgsForBasicPopup);
	lSelf->Construct(aArgs.MessageLine1StringId, aArgs.MessageLine2StringId, aArgs.MessageLine3StringId, aArgs.NumberOfSecondsBeforeClosing);
	return lSelf;
}

void CPopupWarning::Construct(const char* aMessageLine1StringId, const char* aMessageLine2StringId, const char* aMessageLine3StringId, TInt aSecsBeforeClosing)
{
	//Warning Symbol
	iWarningSymbol = CTextureObject::New(TPoint(790, 480), &iImageStore->GetImage(EImageIdFrontendPopUpWarningSign), 70, 70, 0, 0, 1, 0 ,1);

	//for finding font for lines
	CPointerArray<CString>* lStringArray = CPointerArray<CString>::New();

	if(aMessageLine1StringId != NULL)
	{
		iStringMessageLine1 = iLocalizationEngine->GetString(aMessageLine1StringId);
		lStringArray->Append(iStringMessageLine1);
	}
	if(aMessageLine2StringId != NULL)
	{
		iStringMessageLine2 = iLocalizationEngine->GetString(aMessageLine2StringId);
		lStringArray->Append(iStringMessageLine2);
	}
	if(aMessageLine3StringId != NULL)
	{
		iStringMessageLine3 = iLocalizationEngine->GetString(aMessageLine3StringId);
		lStringArray->Append(iStringMessageLine3);
	}

	iFontForWarning = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lStringArray, 700, 60);
	lStringArray->Clear();
	delete lStringArray;

	//get Frames per Seconds to see how frames we need to wait
  iFramesRemainingBeforeClosing = CGame::Game->iFramesPerSecondCounter->GetFramesPerSecond() * aSecsBeforeClosing;
}

void CPopupWarning::PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent)
{
	//do nothing
}

void CPopupWarning::PopupDrawViewExtraCode()
{
	iWarningSymbol->DrawArrays();

	//draw Warning message
	if(iStringMessageLine1 != NULL)
	{
		iGraphicsFont->SetFont(iFontForWarning);
		iGraphicsFont->SetColourWhite();
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(162, 380), iStringMessageLine1);
	}
	if(iStringMessageLine2 != NULL)
	{
		iGraphicsFont->SetFont(iFontForWarning);
		iGraphicsFont->SetColourWhite();
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(162, 310), iStringMessageLine2);
	}
	if(iStringMessageLine3 != NULL)
	{
		iGraphicsFont->SetFont(iFontForWarning);
		iGraphicsFont->SetColourWhite();
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(162, 240), iStringMessageLine3);
	}


	if(iFramesRemainingBeforeClosing < 0)
	{
		Finish(false);
	}
	else
	{
		iFramesRemainingBeforeClosing--;
	}
}
