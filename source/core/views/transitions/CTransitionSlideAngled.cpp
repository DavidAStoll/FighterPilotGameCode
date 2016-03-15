/*
 * CTransitionSlideAngled.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionSlideAngled.h"

#define WIDTH_BETWEEN_VIEWS 0 //need some certain amount of width of the views, just gonna hardcode it here, so it is bad practice

CTransitionSlideAngled::CTransitionSlideAngled(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight)
		:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iIsSlidingRight = aSlideRight;
	iAngle = 0;
	iSlideXPosition = 0;
	iScaleValue = 1;
	iSlideXIncrementValue = (TFloat) ((iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts().iWidth / 2) + WIDTH_BETWEEN_VIEWS)/ iUpdatesUntilComplete;
	iAngleIncrementValue = (TFloat) 90 / (iUpdatesUntilComplete);
	iScaleIncrementValue = (TFloat) 0.5 / (iUpdatesUntilComplete);
}

CTransitionSlideAngled::~CTransitionSlideAngled()
{
	// TODO Auto-generated destructor stub
}

CTransitionSlideAngled* CTransitionSlideAngled::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight)
{
	CTransitionSlideAngled* lSelf = new CTransitionSlideAngled(aViewObserver, aOldView, aNewView, aDurationInSeconds, aSlideRight);
	return lSelf;
}

void CTransitionSlideAngled::DrawTransition()
{
	if(iUpdateCounter >= iUpdatesUntilComplete / 2)
	{
		//at the halfway point or more
		iGraphicsEngine->ScaleMatrix(iScaleValue, iScaleValue, 1);
		iScaleValue += iScaleIncrementValue;
	}
	else
	{
		iGraphicsEngine->ScaleMatrix(iScaleValue, iScaleValue, 1);
		iScaleValue -= iScaleIncrementValue;
	}


	if(iIsSlidingRight) //views slides out to the right
	{
		//rotation for old view
		iGraphicsEngine->TranslateMatrix(TIntFloat::Convert(iSlideXPosition), TIntFloat::Convert(0), TIntFloat::Convert(0));
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix(-iAngle, 0, 1, 0);
		iOldViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();

		//move to the location of the new view
		TIntFloat lWidthOfOldView = CMath::GraphicsCosTable(iAngle) * iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
		TIntFloat lWidthOfNewView = CMath::GraphicsCosTable(90 - iAngle) * iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
		TIntFloat lDistanceFromOldView = (lWidthOfOldView + lWidthOfNewView) / TIntFloat::Convert(2);
		lDistanceFromOldView += TIntFloat::Convert(WIDTH_BETWEEN_VIEWS); //have a constant distance of 200 between both view
		iGraphicsEngine->TranslateMatrix(-lDistanceFromOldView, TIntFloat::Convert(0), TIntFloat::Convert(0));

		//rotation for new view
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix((90 - iAngle), 0, 1, 0);
		iNewViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();
	}
	else //view slides out to the left
	{
		//rotation for old view
		iGraphicsEngine->TranslateMatrix(-TIntFloat::Convert(iSlideXPosition), TIntFloat::Convert(0), TIntFloat::Convert(0));
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix(iAngle, 0, 1, 0);
		iOldViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();

		//move to the location of the new view
		TIntFloat lWidthOfOldView = CMath::GraphicsCosTable(iAngle) * iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
		TIntFloat lWidthOfNewView = CMath::GraphicsCosTable(90 - iAngle) * iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
		TIntFloat lDistanceFromOldView = (lWidthOfOldView + lWidthOfNewView) / TIntFloat::Convert(2);
		lDistanceFromOldView += TIntFloat::Convert(WIDTH_BETWEEN_VIEWS); //have a constant distance of 200 between both view
		iGraphicsEngine->TranslateMatrix(lDistanceFromOldView, TIntFloat::Convert(0), TIntFloat::Convert(0));

		//rotation for new view
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix(-(90 - iAngle), 0, 1, 0);
		iNewViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();
	}

	iSlideXPosition += iSlideXIncrementValue; //increment XPosition
	iAngle += iAngleIncrementValue; //increment angle
}

