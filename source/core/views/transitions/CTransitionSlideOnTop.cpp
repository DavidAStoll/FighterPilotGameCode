/*
 * CTransitionSlideOnTop.cpp
 *
 *  Created on: Jul 12, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionSlideOnTop.h"

CTransitionSlideOnTop::CTransitionSlideOnTop(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight)
		:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iIsSlidingRight = aSlideRight;
	iSlideXPosition = 0;
	iSlideXIncrementValue = (TFloat) iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iWidth / iUpdatesUntilComplete;
}

CTransitionSlideOnTop::~CTransitionSlideOnTop()
{
	// TODO Auto-generated destructor stub
}

CTransitionSlideOnTop* CTransitionSlideOnTop::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight)
{
	CTransitionSlideOnTop* lSelf = new CTransitionSlideOnTop(aViewObserver, aOldView, aNewView, aDurationInSeconds, aSlideRight);
	return lSelf;
}

void CTransitionSlideOnTop::DrawTransition()
{
	if(iIsSlidingRight) //views slides out to the right
	{
		iOldViewTextureObject->DrawArrays();

		TIntFloat lTotalHoriontalSlide = iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth - TIntFloat::Convert(iSlideXPosition);
		iGraphicsEngine->TranslateMatrix(lTotalHoriontalSlide, TIntFloat::Convert(0), TIntFloat::Convert(0));
		iNewViewTextureObject->DrawArrays();
	}
	else //view slides out to the left
	{
		iOldViewTextureObject->DrawArrays();

		TIntFloat lTotalHoriontalSlide = iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth - TIntFloat::Convert(iSlideXPosition);
		iGraphicsEngine->TranslateMatrix(-lTotalHoriontalSlide, TIntFloat::Convert(0), TIntFloat::Convert(0));
		iNewViewTextureObject->DrawArrays();
	}

	iSlideXPosition += iSlideXIncrementValue; //increment XPosition
}
