/*
 * CTransitionHorizontalPeelOff.h
 *
 *  Created on: Jul 12, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONHORIZONTALPEELOFF_H_
#define CTRANSITIONHORIZONTALPEELOFF_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionHorizontalPeelOff : public CBasicTransition
{
public:

	virtual ~CTransitionHorizontalPeelOff();
	static CTransitionHorizontalPeelOff* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight);

protected:
	CTransitionHorizontalPeelOff(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight);
	//update transition progress and draw it
	virtual void DrawTransition();

	TBool iIsSlidingRight;
	TFloat iTextureSPosition;
	TFloat iTextureSPositionIncrementValue;
};

#endif /* CTRANSITIONHORIZONTALPEELOFF_H_ */
