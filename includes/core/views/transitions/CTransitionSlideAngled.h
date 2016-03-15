/*
 * CTransitionSlideAngled.h
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONSLIDEANGLED_H_
#define CTRANSITIONSLIDEANGLED_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionSlideAngled : public CBasicTransition
{
public:

	virtual ~CTransitionSlideAngled();
	static CTransitionSlideAngled* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight);

protected:
	CTransitionSlideAngled(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aSlideRight);
	//update transition progress and draw it
	virtual void DrawTransition();

	TBool iIsSlidingRight;
	TFloat iSlideXPosition; //keeps track of the position the OldView is located
	TFloat iSlideXIncrementValue; //depends on how long the transition should go
	TFloat iScaleValue;
	TFloat iScaleIncrementValue;
	TFloat iAngleIncrementValue;
	TFloat iAngle;
};

#endif /* CTRANSITIONSLIDEANGLED_H_ */
