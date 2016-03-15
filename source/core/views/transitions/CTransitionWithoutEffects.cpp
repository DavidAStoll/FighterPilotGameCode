/*
 * CTransitionWithoutEffects.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionWithoutEffects.h"

CTransitionWithoutEffects::CTransitionWithoutEffects(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView)
:CBasicTransition(aViewObserver, aOldView, aNewView, 0)
{
	// TODO Auto-generated constructor stub

}

CTransitionWithoutEffects::~CTransitionWithoutEffects()
{
	// TODO Auto-generated destructor stub
}

CTransitionWithoutEffects* CTransitionWithoutEffects::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView)
{
	CTransitionWithoutEffects* lSelf = new CTransitionWithoutEffects(aViewObserver, aOldView, aNewView);
	return lSelf;
}

void  CTransitionWithoutEffects::DrawTransition()
{
	//just draw the old view
	iOldViewTextureObject->DrawArrays();
}
