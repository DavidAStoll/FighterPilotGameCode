/*
 * CBasicTransition.h
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#ifndef CBASICTRANSITION_H_
#define CBASICTRANSITION_H_

#include "includes/core/views/CBasicView.h"

class CBasicTransition: public CBasicView
{

public:

	virtual ~CBasicTransition();
	//set if OldView will be deleted at the end of the transition or not
	void SetDeleteOldView(TBool aValue);

protected:

	CBasicTransition(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds);
	void BasicConstruct();
	//makes a call to the transition Draw Method
	virtual void ChildViewDrawMethod();
	//just going to be empty, since transitions usually don't use it
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	//this method should be implemented by the child, it should contain all the draw code
	virtual void DrawTransition() = 0;

	//by default is true, that is it deletes the OldView at the end of the transition
	TBool iDeleteOldView;
	//keep track what Frames per Second the game was running before the transition
	TInt iFramesPerSecondBeforeTransition;
	//keeps track how many times the transition will be called before it is complete
	TInt iUpdatesUntilComplete;
	TInt iUpdateCounter;
	//we use this to know how many bytes the transition takes up on the GPU
	TInt iKiloBytesUsedForTransitionOnGPU;
	//used for textures of the views
	SGraphicsEngineTextureId iOldViewTextureId;
	SGraphicsEngineTextureId iNewViewTextureId;
	CTextureObject* iOldViewTextureObject;
	CTextureObject* iNewViewTextureObject;
	//need these after the transition is done
	CBasicView** iViewObserver; //needs a reference to the Observer that owns this review to update its reference after the transition is done
	CBasicView* iNewView; //this is used during a transition, it hold the new view until the transition is complete
	CBasicView* iOldView;

};

#endif /* CBASICTRANSITION_H_ */
