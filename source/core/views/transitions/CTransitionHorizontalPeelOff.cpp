/*
 * CTransitionHorizontalPeelOff.cpp
 *
 *  Created on: Jul 12, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"

CTransitionHorizontalPeelOff::CTransitionHorizontalPeelOff(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight)
		:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iIsSlidingRight = aSlideRight;
	iTextureSPosition = 0;
	iTextureSPositionIncrementValue = (TFloat) 1 / iUpdatesUntilComplete;
}

CTransitionHorizontalPeelOff::~CTransitionHorizontalPeelOff()
{
	// TODO Auto-generated destructor stub
}

CTransitionHorizontalPeelOff* CTransitionHorizontalPeelOff::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight)
{
	CTransitionHorizontalPeelOff* lSelf = new CTransitionHorizontalPeelOff(aViewObserver, aOldView, aNewView, aDurationInSeconds, aSlideRight);
	return lSelf;
}

void CTransitionHorizontalPeelOff::DrawTransition()
{
	if(iIsSlidingRight) //views slides out to the right
	{
		iOldViewTextureObject->DrawArrays();

		iNewViewTextureObject->SetNewSValue(0, iTextureSPosition);
		TIntFloat lWidthOfTextureObject = TIntFloat::Convert(iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iWidth * iTextureSPosition);
		iNewViewTextureObject->SetWidth(lWidthOfTextureObject);
		iNewViewTextureObject->DrawArrays();
	}
	else //view slides out to the left
	{
		iOldViewTextureObject->DrawArrays();

		iNewViewTextureObject->SetNewSValue((TFloat) 1 - iTextureSPosition, 1);
		TIntFloat lWidthOfTextureObject = TIntFloat::Convert(iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iWidth * iTextureSPosition);
		iNewViewTextureObject->SetWidth(lWidthOfTextureObject);
		//in this case we also need to change its location so that it starts at right side of the view
		iGraphicsEngine->TranslateMatrix((iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth - lWidthOfTextureObject), TIntFloat::Convert(0), TIntFloat::Convert(0));
		iNewViewTextureObject->DrawArrays();
	}

	iTextureSPosition += iTextureSPositionIncrementValue;
}
