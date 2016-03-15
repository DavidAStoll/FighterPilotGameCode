/*
 * CButton.cpp
 *
 *  Created on: Jul 14, 2012
 *      Author: dstoll
 */

#include "includes/core/views/components/CButton.h"

CButton::CButton(const SButtonArguments& aArg)
: CBasicComponent(aArg.Location, aArg.TextureIdDefault, aArg.SizeOfTexture.iWidth, aArg.SizeOfTexture.iHeight)
{
	iState = EStateDefault;
	iIdOfFinger = -1;
	iTextureIdSelected = aArg.TextureIdSelected;
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

CButton::~CButton()
{
	if(iButtonLabel != NULL)
	{
		delete iButtonLabel;
		iButtonLabel = NULL;
	}
}

CButton* CButton::New(const SButtonArguments& aArg)
{
	CButton* lSelf = new CButton(aArg);
	return lSelf;
}

void CButton::AssignTextureIds(SGraphicsEngineTextureId* aTextureIdDefault, SGraphicsEngineTextureId* aTextureIdSelected, SGraphicsEngineTextureId* TextureIdDisabled)
{
	iTextureId = aTextureIdDefault;
	iTextureIdSelected = aTextureIdSelected;
	iTextureIdDisabled = TextureIdDisabled;
}

 TBool CButton::CheckIfHit(const STouchEventLocationConverted& aTouchEvent)
 {
	 TInt lX = iTextureObject->ReturnLowerLeftCoordinate().iX.GetIntInBaseInt();
	 TInt lY = iTextureObject->ReturnLowerLeftCoordinate().iY.GetIntInBaseInt();
	 TRect lHitBox = TRect(TPoint(lX,lY + iSizeOfHitBox.iHeight), iSizeOfHitBox.iWidth, iSizeOfHitBox.iHeight); //need to add Height, since we want iY to be the bottom left corner

	 TBool lHasBeenTouched = lHitBox.Contains(aTouchEvent.VirtualLocation);

	 switch(aTouchEvent.Type)
	 {
		 case ETouchEventHandlerTouchTypeDown:
		 {
			 if(lHasBeenTouched && iState != EStateDisabled)
			 {
				 iState = EStateSelected;
				 iIdOfFinger = aTouchEvent.FingerId;
			 }

			 break;
		 }
		 case ETouchEventHandlerTouchTypeUp:
		 {
			 if(iIdOfFinger == aTouchEvent.FingerId)
			 {
				 iState = EStateDefault;
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
		 return lHasBeenTouched;
	 }
 }


void CButton::Draw()
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
		//set Font for label
		iGraphicsFont->SetFont(iFontForLabel);

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

TSize CButton::GetSizeOfHitBox()
{
	return iSizeOfHitBox;
}

CButton::TButtonState CButton::GetState()
{
	return iState;
}

void CButton::ResetButton()
{
	iState = EStateDefault;
	iIdOfFinger = -1;
}

void CButton::SetColourForLabel(const TColour& aColourDefault, const TColour& aColourSelected, const TColour& aColourDisabled)
{
	iColourForLabelDefault = aColourDefault;
	iColourForLabelSelected = aColourSelected;
	iColourForLabelDisabled = aColourDisabled;
}

void CButton::SetLabel(const CString* aLabel)
{
	if(iButtonLabel != NULL)
	{
		delete iButtonLabel;
		iButtonLabel = NULL;
	}
	if(aLabel != NULL)
	{
		iButtonLabel = new CString(aLabel->GetData());
	}
}

void CButton::SetState(CButton::TButtonState aState)
{
	iState = aState;
}

