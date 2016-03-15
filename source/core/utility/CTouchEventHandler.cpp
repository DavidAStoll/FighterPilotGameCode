/*
 * CTouchEventHandler.cpp
 *
 *  Created on: Jul 7, 2012
 *      Author: dstoll
 */

#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/views/CBasicView.h"
#include "includes/core/CGame.h"


CTouchEventHandler::CTouchEventHandler(TInt aMaxUniqueFingers)
{
	iMaxUniqueFingers = aMaxUniqueFingers;
}

CTouchEventHandler::~CTouchEventHandler()
{
	if(iTouchEvents != NULL)
	{
		iTouchEvents->ClearAndDestroy();
		delete iTouchEvents;
		iTouchEvents = NULL;
	}
}

CTouchEventHandler* CTouchEventHandler::New(TInt aMaxUniqueFingers)
{
	CTouchEventHandler* lSelf = new CTouchEventHandler(aMaxUniqueFingers);
	lSelf->Construct();
	return lSelf;
}

void CTouchEventHandler::Construct()
{
	iTouchEvents = CPointerArray<STouchEventLocationConverted>::New();
	iTouchEvenArrayLock = new CNativeMutex();
}

//---------------------- functions ------------------------//

void CTouchEventHandler::DeliverTouchEventsToView(CBasicView* aView)
{
	//we don't want that new events are added while we consume them, otherwise Events might get lost
	iTouchEvenArrayLock->Lock();

	if(aView != NULL)
	{
		for(TInt lIndex = 0; lIndex < iTouchEvents->GetCount(); lIndex++)
		{
			aView->ConsumeTouchEvent(*iTouchEvents->Get(lIndex));
		}
	}

	//these events have been consumed
	iTouchEvents->ClearAndDestroy();
	iTouchEvenArrayLock->Unlock(); //critical section is over
}

void CTouchEventHandler::AddAndConvertRawEvent(const STouchEventLocationRaw& aRawTouchEvent)
{
	//find out if we need to add this event at all
	if(aRawTouchEvent.FingerId < iMaxUniqueFingers)
	{
		//going to lock here, since this is called during the adding stage, thus don't need to add it in the platform dependent code
		iTouchEvenArrayLock->Lock();

		//has a valid FingerId, add it
		STouchEventLocationConverted lConvertedTouchEvent;
		lConvertedTouchEvent.Type = aRawTouchEvent.Type;
		lConvertedTouchEvent.FingerId = aRawTouchEvent.FingerId;

		//need to find proper virtual Locations
		TSize lRealSizeOfScreen = CGame::Game->iGraphicsEngine->GetRealDimensionsOfScreenInPixels();
		TSize lVirtualSizeOfScreen  =  CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreenAsInts();

		TInt lVirtualXLocation = ((TFloat) aRawTouchEvent.XPixelLocation / lRealSizeOfScreen.iWidth) * lVirtualSizeOfScreen.iWidth;
		TInt lVirtualYLocation = ((TFloat) aRawTouchEvent.YPixelLocation / lRealSizeOfScreen.iHeight) * lVirtualSizeOfScreen.iHeight;
		lVirtualYLocation = lVirtualSizeOfScreen.iHeight - lVirtualYLocation; //RawTouchEvent uses a TopLeft Window System, while ours assumes a BottomLeft one

		lConvertedTouchEvent.VirtualLocation = TPoint(lVirtualXLocation, lVirtualYLocation);
		iTouchEvents->Append(new STouchEventLocationConverted(lConvertedTouchEvent));

		//TouchEvent can be consumed again
		iTouchEvenArrayLock->Unlock();
	}
}
