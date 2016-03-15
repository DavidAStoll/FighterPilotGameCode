/*
 * CTransitionOpenDoor.h
 *
 *  Created on: Oct 11, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONOPENDOOR_H_
#define CTRANSITIONOPENDOOR_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionOpenDoor : public CBasicTransition
{
public:
	virtual ~CTransitionOpenDoor();
	static CTransitionOpenDoor* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);

protected:
	CTransitionOpenDoor(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);
	//update transition progress and draw it
	virtual void DrawTransition();

	TFloat iAngle;
	TFloat iAngleIncreaseValue;
	TIntFloat iWidthOfScreen;
};

#endif /* CTRANSITIONOPENDOOR_H_ */
