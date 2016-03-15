/*
 * CTransitionOpenDoorRotatingRotating.cpp
 *
 *  Created on: Oct 17, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionOpenDoorRotating.h"

CTransitionOpenDoorRotating::CTransitionOpenDoorRotating(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aIsClosing)
:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iAngle = 0;
	iAngleIncreaseValue = (TFloat) 90 / iUpdatesUntilComplete; //since we rotate to 90 degress and then back to 0
	iWidthOfScreen = iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
	iIsClosing = aIsClosing;

	//need to move the transformation point back to the bottom left corner
	if(iIsClosing)
	{
		iNewViewTextureObject->ChangeXCoordinate(TIntFloat::Convert(512));
		iNewViewTextureObject->ChangeYCoordinate(TIntFloat::Convert(330));
	}
	else
	{
		iOldViewTextureObject->ChangeXCoordinate(TIntFloat::Convert(512));
		iOldViewTextureObject->ChangeYCoordinate(TIntFloat::Convert(330));
	}
}

CTransitionOpenDoorRotating::~CTransitionOpenDoorRotating()
{
	// TODO Auto-generated destructor stub
}

CTransitionOpenDoorRotating* CTransitionOpenDoorRotating::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aIsClosing)
{
	CTransitionOpenDoorRotating* lSelf = new CTransitionOpenDoorRotating(aViewObserver, aOldView, aNewView, aDurationInSeconds, aIsClosing);
	return lSelf;
}

void  CTransitionOpenDoorRotating::DrawTransition()
{
	if(iIsClosing)
	{
		//draw new View
		iOldViewTextureObject->DrawArrays();

		//move transition point to bottom of screen
		iGraphicsEngine->TranslateMatrix(TIntFloat::Convert(0), -TIntFloat::Convert(330), TIntFloat::Convert(0));

		//draw Left door
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix((90 - iAngle), 0, 0, 1);
		iNewViewTextureObject->ChangeXCoordinate(-TIntFloat::Convert(512));
		iNewViewTextureObject->SetWidth(iWidthOfScreen / 2);
		iNewViewTextureObject->SetNewSValue(0, 0.5);
		iNewViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();

		//draw Right door
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix(-(90 - iAngle), 0, 0, 1);
		iNewViewTextureObject->ChangeXCoordinate(TIntFloat::Convert(512));
		iNewViewTextureObject->SetWidth(iWidthOfScreen / 2);
		iNewViewTextureObject->SetNewSValue(0.5, 1);
		iNewViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();
	}
	else
	{
		//draw new View
		iNewViewTextureObject->DrawArrays();

		//move transition point to bottom of screen
		iGraphicsEngine->TranslateMatrix(TIntFloat::Convert(0), -TIntFloat::Convert(330), TIntFloat::Convert(0));

		//draw Left door
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix((iAngle), 0, 0, 1);
		iOldViewTextureObject->ChangeXCoordinate(-TIntFloat::Convert(512));
		iOldViewTextureObject->SetWidth(iWidthOfScreen / 2);
		iOldViewTextureObject->SetNewSValue(0, 0.5);
		iOldViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();

		//draw Right door
		iGraphicsEngine->PushMatrix();
		iGraphicsEngine->RotateMatrix(-(iAngle), 0, 0, 1);
		iOldViewTextureObject->ChangeXCoordinate(TIntFloat::Convert(512));
		iOldViewTextureObject->SetWidth(iWidthOfScreen / 2);
		iOldViewTextureObject->SetNewSValue(0.5, 1);
		iOldViewTextureObject->DrawArrays();
		iGraphicsEngine->PopMatrix();
	}

	iAngle += iAngleIncreaseValue;
}

