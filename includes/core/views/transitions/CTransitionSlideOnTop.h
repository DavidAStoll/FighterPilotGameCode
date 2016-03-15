/*
 * CTransitionSlideOnTop.h
 *
 *  Created on: Jul 12, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONSLIDEONTOP_H_
#define CTRANSITIONSLIDEONTOP_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionSlideOnTop : public CBasicTransition
{
public:
	virtual ~CTransitionSlideOnTop();
	static CTransitionSlideOnTop* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight);

protected:
	CTransitionSlideOnTop(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight);
	//update transition progress and draw it
	virtual void DrawTransition();

	TBool iIsSlidingRight;
	TFloat iSlideXPosition; //keeps track of the position the OldView is located
	TFloat iSlideXIncrementValue; //depends on how long the transition should go
};

#endif /* CTRANSITIONSLIDEONTOP_H_ */
