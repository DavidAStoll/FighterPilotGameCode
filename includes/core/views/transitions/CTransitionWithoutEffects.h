/*
 * CTransitionWithoutEffects.h
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONWITHOUTEFFECTS_H_
#define CTRANSITIONWITHOUTEFFECTS_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionWithoutEffects : public CBasicTransition
{
public:
	virtual ~CTransitionWithoutEffects();
	static CTransitionWithoutEffects* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView);

protected:
	CTransitionWithoutEffects(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView);
	//update transition progress and draw it
	virtual void DrawTransition();
};

#endif /* CTRANSITIONWITHOUTEFFECTS_H_ */
