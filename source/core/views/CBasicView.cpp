/*
 * CBasicView.cpp
 *
 *  Created on: Jul 5, 2012
 *      Author: dstoll
 */

#include "includes/core/views/CBasicView.h"

CBasicView::CBasicView()
{
	iIsDoingTransition = false;
	iIsReadyToHandleInput = false;
	iGraphicsFont = CGame::Game->iGraphicsFont;
	iImageStore = CGame::Game->iImageStore;
	iGraphicsEngine = CGame::Game->iGraphicsEngine;
	iLocalizationEngine = CGame::Game->iLocalizationEngine;
}

CBasicView::~CBasicView()
{

}

void CBasicView::ConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	if(iIsReadyToHandleInput)
	{
		ChildConsumeTouchEvent(aTouchEvent);
	}
}

void CBasicView::ContextGained()
{
	//resume Music And Sound
	CGame::Game->iAudioEngine->GetMusicPlayer().Resume();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Resume();
}

void CBasicView::ContextLost()
{
	//pause Music And Sound
	CGame::Game->iAudioEngine->GetMusicPlayer().Pause();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Pause();
}

void CBasicView::Draw()
{
	iIsReadyToHandleInput = true; //since draw the view, the view can handle input now
	ChildViewDrawMethod(); //will be implemented by child
}

void CBasicView::RunLogicCode()
{
	ChildRunLogicCode();
}

void  CBasicView::SetIsDoingTransition(TBool aValue)
{
	iIsDoingTransition = aValue;
}

void CBasicView::ChildRunLogicCode()
{
	//just empty
}

void CBasicView::DrawCenteredString(TPoint aLeftBottomCorner, const SFont& aDrawFont, const CString* aString, TInt aMaxWidth, TInt aMaxHeight)
{
	//find X Location
	TInt lWidthOfFont = iGraphicsFont->GetWidthOfString(aDrawFont, aString);
	aLeftBottomCorner.iX += (aMaxWidth - lWidthOfFont) / 2;

	//find Y Location
	aLeftBottomCorner.iY += aDrawFont.HeightAndDescentOfFont.GetIntInBaseInt() / 10; //heurastic value to make up for the descent
	aLeftBottomCorner.iY += (aMaxHeight - aDrawFont.HeightAndDescentOfFont.GetIntInBaseInt()) / 2;

	iGraphicsFont->DrawStringToScreenHorizontally(aLeftBottomCorner, aString);
}

void CBasicView::DrawStringIntoBox(TPoint aTopRightStartLocation, TInt aLineWidth, TRect aBoxOutline, SFont& aFont, const CString* aString)
{
	iGraphicsFont->SetFont(aFont);
	TPoint lTextLocation = aTopRightStartLocation;
	TInt lXStartLocation = aTopRightStartLocation.iX;
	TInt lLengthOfText = 0;
	TInt lHeightOfFont = aFont.HeightAndDescentOfFont.GetIntInBaseInt();
	TBool lOnlyCutOneCharacterAtATime = iLocalizationEngine->GetLanguage() == ELanguageChineseSimplified || iLocalizationEngine->GetLanguage() == ELanguageChineseTraditional;

	//find out where the first new line is
	TInt lNextNewLineCharacterLocation = aString->FindString(0, "|");
	//makes space for the first line
	lTextLocation.iY -= lHeightOfFont;

	for(TInt lIndex = 0; lIndex < aString->GetLength();)
	{
		TInt lNextWhitespaceLocation = aString->FindString(lIndex, " ");

		CString* lNextWholeWord = NULL;

		//check if a new Line Character
		if(lNextNewLineCharacterLocation == lIndex)
		{
			//find out where the next new line is
			lNextNewLineCharacterLocation = aString->FindString(lIndex + 1, "|"); //need to skip current character
			lLengthOfText = 0;
			lTextLocation.iY -= lHeightOfFont;
			lTextLocation.iX = lXStartLocation;

			//skip white space that follows directly after new line
			if(lNextWhitespaceLocation == lIndex + 1)
				lIndex += 2;
			else
				lIndex += 1;
		}
		else if(lNextWhitespaceLocation == -1)
		{
			//end of String
			break;
		}
		else
		{
			if(lOnlyCutOneCharacterAtATime) //only cut out one character
			{
				lNextWholeWord = aString->GetSubString(lIndex, 1);
			}
			else //cut out the entire word until the next whitespace
			{
				lNextWholeWord = aString->GetSubString(lIndex, (lNextWhitespaceLocation - lIndex)+ 1); //include whitespace
			}

			TInt lWidthOfWordInVirtualPixels = iGraphicsFont->GetWidthOfString(aFont, lNextWholeWord);
			lLengthOfText += lWidthOfWordInVirtualPixels;

			if(lLengthOfText > aLineWidth)
			{
				//character is outside the box, need to start on a new line
				lTextLocation.iY -= lHeightOfFont;
				lTextLocation.iX = lXStartLocation;
				lLengthOfText = lWidthOfWordInVirtualPixels;
			}

			//draw character and increment position
			//only draw if Y coordinate is still inside the draw box
			if(aBoxOutline.iTl.iY > lTextLocation.iY && lTextLocation.iY > aBoxOutline.iBr.iY - lHeightOfFont/*add one extra row to bottom, makes it look smoother*/)
			{
				iGraphicsFont->DrawStringToScreenHorizontally(lTextLocation, lNextWholeWord);
			}

			lTextLocation.iX += lWidthOfWordInVirtualPixels;
			lIndex += lNextWholeWord->GetLength();
		}

		//all done with this character
		if(lNextWholeWord != NULL)
		{
			delete lNextWholeWord;
		}
	}
}

TInt CBasicView::GetMaxHeightOfTextInBox(TInt aLineWidth, SFont& aFont, const CString* aString)
{
	iGraphicsFont->SetFont(aFont);
	TInt lLengthOfText = 0;
	TInt lHeightOfFont = aFont.HeightAndDescentOfFont.GetIntInBaseInt();
	TInt lNumberOfLinesUsed = 3; //add 2 extra lines just to make sure there is enough space
	TBool lOnlyCutOneCharacterAtATime = iLocalizationEngine->GetLanguage() == ELanguageChineseSimplified || iLocalizationEngine->GetLanguage() == ELanguageChineseTraditional;

	//find out where the first new line is
	TInt lNextNewLineCharacterLocation = aString->FindString(0, "|");

	for(TInt lIndex = 0; lIndex < aString->GetLength();)
	{
		TInt lNextWhitespaceLocation = aString->FindString(lIndex, " ");

		CString* lNextWholeWord = NULL;

		//check if a new Line Character
		if(lNextNewLineCharacterLocation == lIndex)
		{
			//find out where the next new line is
			lNextNewLineCharacterLocation = aString->FindString(lIndex + 1, "|"); //need to skip current character
			lLengthOfText = 0;
			lNumberOfLinesUsed += 1;

			//skip white space that follows directly after new line
			if(lNextWhitespaceLocation == lIndex + 1)
				lIndex += 2;
			else
				lIndex += 1;
		}
		else if(lNextWhitespaceLocation == -1)
		{
			//end of String
			break;
		}
		else
		{
			if(lOnlyCutOneCharacterAtATime) //only cut out one character
			{
				lNextWholeWord = aString->GetSubString(lIndex, 1);
			}
			else //cut out the entire word until the next whitespace
			{
				lNextWholeWord = aString->GetSubString(lIndex, (lNextWhitespaceLocation - lIndex)+ 1); //include whitespace
			}

			TInt lWidthOfWordInVirtualPixels = iGraphicsFont->GetWidthOfString(aFont, lNextWholeWord);
			lLengthOfText += lWidthOfWordInVirtualPixels;

			if(lLengthOfText > aLineWidth)
			{
				//character is outside the box, need to start on a new line
				lLengthOfText = lWidthOfWordInVirtualPixels;
				lNumberOfLinesUsed += 1;
			}

			//update index for how many characters we are going to skip
			lIndex += lNextWholeWord->GetLength();
		}

		//all done with this character
		if(lNextWholeWord != NULL)
		{
			delete lNextWholeWord;
		}
	}

	return lNumberOfLinesUsed * lHeightOfFont;
}
