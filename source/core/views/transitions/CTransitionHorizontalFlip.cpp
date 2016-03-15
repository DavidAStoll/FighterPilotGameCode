/*
 * CTransitionHorizontalFlip.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"

CTransitionHorizontalFlip::CTransitionHorizontalFlip(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iAngle = 0;
	iAngleIncreaseValue = (TFloat) 90 / (iUpdatesUntilComplete / 2); //since we rotate to 90 degress and then back to 0
}

CTransitionHorizontalFlip::~CTransitionHorizontalFlip()
{
	// TODO Auto-generated destructor stub
}

CTransitionHorizontalFlip* CTransitionHorizontalFlip::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
{
	CTransitionHorizontalFlip* lSelf = new CTransitionHorizontalFlip(aViewObserver, aOldView, aNewView, aDurationInSeconds);
	return lSelf;
}

void  CTransitionHorizontalFlip::DrawTransition()
{
	if(iUpdateCounter >= iUpdatesUntilComplete / 2)
	{
		//at the halfway point or more
		iGraphicsEngine->RotateMatrix(iAngle, 1, 0, 0);
		iNewViewTextureObject->DrawArrays();
		iAngle -= iAngleIncreaseValue;
	}
	else
	{
		iGraphicsEngine->RotateMatrix(iAngle, 1, 0, 0);
		iOldViewTextureObject->DrawArrays();
		iAngle += iAngleIncreaseValue;
	}
}

