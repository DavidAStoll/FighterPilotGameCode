/*
 * CTouchEventHandler.h
 *
 *  Created on: Jul 7, 2012
 *      Author: dstoll
 */

#ifndef CTOUCHEVENTHANDLER_H_
#define CTOUCHEVENTHANDLER_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/utility/IPC.h"

class CBasicView;

//this class checks the platform if any touch Events have happened
//it will then take these touch Events and hand them over to the view given to it as an argument

enum TTouchEventHandlerTouchType
{
	ETouchEventHandlerTouchTypeDown,
	ETouchEventHandlerTouchTypeDrag,
	ETouchEventHandlerTouchTypeUp,
};

struct STouchEventLocationRaw
{
	TInt XPixelLocation; //dependent on screen size, should be TopLeft relative to the View
	TInt YPixelLocation; //dependent on screen size, should be TopLeft relative to the View
	TTouchEventHandlerTouchType Type;
	TInt FingerId; //should stay constant throughout a sequence of TouchEvents caused by one particular finger, always starts with 0 up to Max
};

struct STouchEventLocationConverted //is device independent
{
	TPoint VirtualLocation; //independent of screen size, should be BottomLeft relative to the View
	TTouchEventHandlerTouchType Type;
	TInt FingerId; //should stay constant throughout a sequence of TouchEvents caused by one particular finger, always starts with 0 up to Max
};

class CTouchEventHandler
{
public:
	static CTouchEventHandler* New(TInt aMaxUniqueFingers);
	~CTouchEventHandler();

	//this method should be called by the platform's framework to deliver raw Events that will then in turn be added and converted
	//----------- NATIVE METHOD------------//
	void AddRawEvent(const STouchEventLocationRaw& aRawEvent);

	//flushes its converted TouchEvents to the specified view(after this call its Event list will be empty), it performs concurrency control to avoid that events are added while perform this call
	void DeliverTouchEventsToView(CBasicView* aView);


protected:
	CTouchEventHandler(TInt aMaxUniqueFingers);
	void Construct();

	//Adds a new RawEvent to the TouchEvent array and also converts it to the internal format, this method also checks if the TouchEvent is in the correct range
	//in addition, it performs concurrency control. Thus the calling function does not have to worry about these things
	void AddAndConvertRawEvent(const STouchEventLocationRaw& aRawTouchEvent);

	TInt iMaxUniqueFingers; //this is just the upper bound, the lower bound is dependent on the platform
	CNativeMutex* iTouchEvenArrayLock; //this lock will be used to prevent that touch Events are modified, while they are consumed or added. To prevent that they are lost
	CPointerArray<STouchEventLocationConverted>* iTouchEvents;
};

#endif /* CTOUCHEVENTHANDLER_H_ */
