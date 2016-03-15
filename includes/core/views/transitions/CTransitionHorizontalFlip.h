/*
 * CTransitionHorizontalFlip.h
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONHORIZONTALFLIP_H_
#define CTRANSITIONHORIZONTALFLIP_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionHorizontalFlip : public CBasicTransition
{
public:
	virtual ~CTransitionHorizontalFlip();
	static CTransitionHorizontalFlip* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);

protected:
	CTransitionHorizontalFlip(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);
	//update transition progress and draw it
	virtual void DrawTransition();

	TFloat iAngle;
	TFloat iAngleIncreaseValue;
};

#endif /* CTRANSITIONHORIZONTALFLIP_H_ */
