/*
 * CTransitionCubeRotation.h
 *
 *  Created on: Dec 20, 2012
 *      Author: dstoll
 */

#ifndef CTransitionCubeRotation_H_
#define CTransitionCubeRotation_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionCubeRotation : public CBasicTransition
{
public:
	virtual ~CTransitionCubeRotation();
	static CTransitionCubeRotation* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);

protected:
	CTransitionCubeRotation(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);
	//update transition progress and draw it
	virtual void DrawTransition();
	void Construct();

	TFloat iAngle;
	TFloat iAngleIncreaseValue;
};

#endif /* CTransitionCubeRotation_H_ */
