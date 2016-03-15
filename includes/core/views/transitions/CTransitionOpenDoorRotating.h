/*
 * CTransitionOpenDoorRotating.h
 *
 *  Created on: Oct 17, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONOPENDOORROTATING_H_
#define CTRANSITIONOPENDOORROTATING_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionOpenDoorRotating : public CBasicTransition
{
public:
	virtual ~CTransitionOpenDoorRotating();
	static CTransitionOpenDoorRotating* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aIsClosing);

protected:
	CTransitionOpenDoorRotating(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TBool aIsClosing);
	//update transition progress and draw it
	virtual void DrawTransition();

	TFloat iAngle;
	TFloat iAngleIncreaseValue;
	TIntFloat iWidthOfScreen;
	TBool iIsClosing;
};

#endif /* CTRANSITIONOPENDOORROTATING_H_ */
