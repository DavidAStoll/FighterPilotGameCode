/*
 * CTransitionOpenDoor.cpp
 *
 *  Created on: Oct 11, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionOpenDoor.h"

CTransitionOpenDoor::CTransitionOpenDoor(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iAngle = 0;
	iAngleIncreaseValue = (TFloat) 90 / iUpdatesUntilComplete; //since we rotate to 90 degress and then back to 0
	iWidthOfScreen = iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
}

CTransitionOpenDoor::~CTransitionOpenDoor()
{
	// TODO Auto-generated destructor stub
}

CTransitionOpenDoor* CTransitionOpenDoor::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
{
	CTransitionOpenDoor* lSelf = new CTransitionOpenDoor(aViewObserver, aOldView, aNewView, aDurationInSeconds);
	return lSelf;
}

void  CTransitionOpenDoor::DrawTransition()
{
	//draw new View
	iNewViewTextureObject->DrawArrays();

	//draw left door
	iGraphicsEngine->PushMatrix();
	iOldViewTextureObject->SetNewSValue(0, 0.5);
	iOldViewTextureObject->SetWidth(iWidthOfScreen / 2);
	iOldViewTextureObject->ReflectOverYAxis();
	iGraphicsEngine->TranslateMatrix(-iWidthOfScreen / 2, TIntFloat::Convert(0), TIntFloat::Convert(0));
	iGraphicsEngine->RotateMatrix(180 - iAngle, 0, 1, 0);
	iOldViewTextureObject->DrawArrays();
	iGraphicsEngine->PopMatrix();

	//draw right door
	iGraphicsEngine->PushMatrix();
	iOldViewTextureObject->SetNewSValue(0.5, 1);
	iOldViewTextureObject->SetWidth(iWidthOfScreen / 2);
	iOldViewTextureObject->ReflectOverYAxis();
	iGraphicsEngine->TranslateMatrix(iWidthOfScreen / 2, TIntFloat::Convert(0), TIntFloat::Convert(0));
	iGraphicsEngine->RotateMatrix(iAngle, 0, 1, 0);
	iOldViewTextureObject->DrawArrays();
	iGraphicsEngine->PopMatrix();

	iAngle += iAngleIncreaseValue;
}

