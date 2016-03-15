/*
 * CTransitionCubeRotation.cpp
 *
 *  Created on: Dec 20, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionCubeRotation.h"

CTransitionCubeRotation::CTransitionCubeRotation(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iAngle = 0;
	iAngleIncreaseValue = (TFloat) 90 / (iUpdatesUntilComplete); //since we rotate to 90 degress and then back to 0
}

CTransitionCubeRotation::~CTransitionCubeRotation()
{
}

CTransitionCubeRotation* CTransitionCubeRotation::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
{
	CTransitionCubeRotation* lSelf = new CTransitionCubeRotation(aViewObserver, aOldView, aNewView, aDurationInSeconds);
	lSelf->Construct();
	return lSelf;
}

void CTransitionCubeRotation::Construct()
{
	//need to change rotation point
	iOldViewTextureObject->ChangeXCoordinate(TIntFloat::Convert(512));
	iOldViewTextureObject->ChangeYCoordinate(TIntFloat::Convert(330));
	iNewViewTextureObject->ChangeXCoordinate(TIntFloat::Convert(512));
	iNewViewTextureObject->ChangeYCoordinate(TIntFloat::Convert(330));
}

void CTransitionCubeRotation::DrawTransition()
{
	iAngle += iAngleIncreaseValue;

	//move to Rotation Point to Left Corner
	iGraphicsEngine->TranslateMatrix(-TIntFloat::Convert(512), -TIntFloat::Convert(330), TIntFloat::Convert(0));

	//old View
	iGraphicsEngine->RotateMatrix(-iAngle, 0, 1, 0);
	iOldViewTextureObject->DrawArrays();

	//New View
	iGraphicsEngine->TranslateMatrix(TIntFloat::Convert(1024), TIntFloat::Convert(0), TIntFloat::Convert(0));
	iGraphicsEngine->RotateMatrix(90, 0, 1, 0);
	iNewViewTextureObject->DrawArrays();
}


