/*
 * CCheckBoxButton.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: dstoll
 */

#include "includes/core/views/components/CCheckBoxButton.h"

CCheckBoxButton::CCheckBoxButton(const SCheckBoxButtonArguments& aArg)
: CBasicComponent(aArg.Location, aArg.TextureIdDefault, aArg.SizeOfTexture.iWidth, aArg.SizeOfTexture.iHeight)
{
	iState = EStateDefault;
	iTextureIdSelected =  aArg.TextureIdSelected;
	iTextureIdDisabled = aArg.TextureIdDisabled;
	iSizeOfHitBox = aArg.SizeOfHitbox;

	if(aArg.Label != NULL)
	{
		iButtonLabel = new CString(aArg.Label->GetData());
	}
	else
	{
		iButtonLabel = NULL;
	}
	if(aArg.Label != NULL && aArg.FontForLabel != NULL)
	{
		iFontForLabel = *aArg.FontForLabel;
	}
	else
	{
		iFontForLabel = iGraphicsFont->CreateFont(10);
	}

	iColourForLabelDefault = aArg.ColourForLabelDefault;
	iColourForLabelSelected = aArg.ColourForLabelSelected;
	iColourForLabelDisabled = aArg.ColourForLabelDisabled;
}

CCheckBoxButton::~CCheckBoxButton()
{
	if(iButtonLabel != NULL)
	{
		delete iButtonLabel;
		iButtonLabel = NULL;
	}
}

CCheckBoxButton* CCheckBoxButton::New(const SCheckBoxButtonArguments& aArg)
{
	CCheckBoxButton* lSelf = new CCheckBoxButton(aArg);
	return lSelf;
}

void CCheckBoxButton::AssignTextureIds(SGraphicsEngineTextureId* aTextureIdDefault, SGraphicsEngineTextureId* aTextureIdSelected, SGraphicsEngineTextureId* aTextureIdDisabled)
{
	iTextureId = aTextureIdDefault;
	iTextureIdSelected = aTextureIdSelected;
	iTextureIdDisabled = aTextureIdDisabled;
}

 TBool CCheckBoxButton::CheckIfHit(const STouchEventLocationConverted& aTouchEvent)
 {
	 TInt lX = iTextureObject->ReturnLowerLeftCoordinate().iX.GetIntInBaseInt();
	 TInt lY = iTextureObject->ReturnLowerLeftCoordinate().iY.GetIntInBaseInt();
	 TRect lHitBox = TRect(TPoint(lX,lY + iSizeOfHitBox.iHeight), iSizeOfHitBox.iWidth, iSizeOfHitBox.iHeight); //need to add Height, since we want iY to be the bottom left corner

	 TBool lHasBeenSelected = lHitBox.Contains(aTouchEvent.VirtualLocation);

	 switch(aTouchEvent.Type)
	 {
		 //check Box should changed state from non-selected to Selected or vice versa only if its hitbox has be hit
		 case ETouchEventHandlerTouchTypeDown:
		 {
			 if(lHasBeenSelected && iState != EStateDisabled)
			 {
				 if(iState == EStateSelected)
				 {
					 iState = EStateDefault;
				 }
				 else
				 {
					 iState = EStateSelected;
				 }
			 }
			 break;
		 }
		 default:
		 {
			 break;
		 }
	 }

	 if(iState == EStateDisabled)
	 {
		 return false;
	 }
	 else
	 {
		 return lHasBeenSelected;
	 }
 }


void CCheckBoxButton::Draw()
{
	if(iState == EStateDisabled)
	{
		iTextureObject->AssignTexture(iTextureIdDisabled);
		iTextureObject->DrawArrays();
	}
	else if(iState == EStateSelected)
	{
		iTextureObject->AssignTexture(iTextureIdSelected);
		iTextureObject->DrawArrays();
	}
	else
	{
		iTextureObject->AssignTexture(iTextureId);
		iTextureObject->DrawArrays();
	}

	if(iButtonLabel != NULL)
	{
		//-------------------- draw the String on Top ----------------------//
		iGraphicsFont->SetFont(iFontForLabel);

		//set colour for font
		if(iState == EStateDisabled)
		{
			iGraphicsFont->SetColour(iColourForLabelDisabled);
		}
		else if(iState == EStateSelected)
		{
			iGraphicsFont->SetColour(iColourForLabelSelected);
		}
		else
		{
			iGraphicsFont->SetColour(iColourForLabelDefault);
		}

		//find X Location
		TInt lWidthOfFont = iGraphicsFont->GetWidthOfString(iFontForLabel, iButtonLabel);
		TInt lMaxWidth = iTextureObject->GetWidth().GetIntInBaseInt();
		TInt lMaxHeight = iTextureObject->GetHeight().GetIntInBaseInt();
		TInt lXLocation = iX.GetIntInBaseInt();
		TInt lYLocation = iY.GetIntInBaseInt();
		lXLocation += (lMaxWidth - lWidthOfFont) / 2; //center it on width

		//find Y Location
		lYLocation += iFontForLabel.HeightAndDescentOfFont.GetIntInBaseInt() / 10; //heurastic value to make up for the descent
		lYLocation += (lMaxHeight - iFontForLabel.HeightAndDescentOfFont.GetIntInBaseInt()) / 2; //center it on Height

		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(lXLocation, lYLocation), iButtonLabel);
	}
}

CCheckBoxButton::TCheckBoxButtonState CCheckBoxButton::GetState()
{
	return iState;
}

void CCheckBoxButton::ResetButton()
{
	iState = EStateDefault;
}

void CCheckBoxButton::SetColourForLabel(const TColour& aColourDefault, const TColour& aColourSelected, const TColour& aColourDisabled)
{
	iColourForLabelDefault = aColourDefault;
	iColourForLabelSelected = aColourSelected;
	iColourForLabelDisabled = aColourDisabled;
}

void CCheckBoxButton::SetState(CCheckBoxButton::TCheckBoxButtonState aState)
{
	iState = aState;
}


