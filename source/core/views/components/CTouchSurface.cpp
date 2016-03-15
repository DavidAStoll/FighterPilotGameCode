/*
 * CTouchSurface.cpp
 *
 *  Created on: Jul 16, 2012
 *      Author: dstoll
 */

#include "includes/core/views/components/CTouchSurface.h"

CTouchSurface::CTouchSurface(const TPoint& aBottomLeftCorner, const TSize& aSizeOfSurface, TFloat& aScrollRef, TInt aMaxScroll, TInt aScrollSpeedFactor, TScrollAxis aScrollAxis)
:CBasicComponent(aBottomLeftCorner, NULL, 0, 0),iScrollVariableRef(aScrollRef)
{
	iSizeOfSurface = aSizeOfSurface;
	iTouched = false;
	iIdOfFinger = 0;
	iMaxScroll = aMaxScroll;
	iScrollSpeedFactor = aScrollSpeedFactor;
	iScrollAxis = aScrollAxis;
}

CTouchSurface::~CTouchSurface()
{
	// TODO Auto-generated destructor stub
}

CTouchSurface* CTouchSurface::New(const TPoint& aBottomLeftCorner, const TSize& aSizeOfSurface, TFloat& aScrollRef, TInt aMaxScroll, TInt aScrollSpeedFactor, TScrollAxis aScrollAxis)
{
	CTouchSurface* lSelf = new CTouchSurface(aBottomLeftCorner, aSizeOfSurface, aScrollRef, aMaxScroll, aScrollSpeedFactor, aScrollAxis);
	return lSelf;
}

TBool CTouchSurface::CheckIfHit(const STouchEventLocationConverted& aTouchEvent)
{
	 TInt lX = iX.GetIntInBaseInt();
	 TInt lY = iY.GetIntInBaseInt();
	 TRect lHitBox = TRect(TPoint(lX,lY + iSizeOfSurface.iHeight), iSizeOfSurface.iWidth, iSizeOfSurface.iHeight); //need to add Height, since we want iY to be the bottom left corner

	 TBool lHasBeenTouched = lHitBox.Contains(aTouchEvent.VirtualLocation);

	 switch(aTouchEvent.Type)
	 {
		 case ETouchEventHandlerTouchTypeDown:
		 {
			 if(lHasBeenTouched)
			 {
				 iLastPositionDrag = aTouchEvent.VirtualLocation;
				 iTouched = true;
				 iIdOfFinger = aTouchEvent.FingerId;
			 }

			 break;
		 }
		 case ETouchEventHandlerTouchTypeDrag:
		 {
			 if(iTouched && iIdOfFinger == aTouchEvent.FingerId)
			 {
				 if(iScrollAxis == EScrollAxisY)
				 {
					 if(iLastPositionDrag.iY > aTouchEvent.VirtualLocation.iY) //move everything up
					 {
						 iScrollVariableRef -= (iLastPositionDrag.iY - aTouchEvent.VirtualLocation.iY) * iScrollSpeedFactor;
						 if(iScrollVariableRef < 0)
							 iScrollVariableRef = 0;
					 }
					 else if(iLastPositionDrag.iY < aTouchEvent.VirtualLocation.iY)//move everything down
					 {
						 iScrollVariableRef += (aTouchEvent.VirtualLocation.iY - iLastPositionDrag.iY) * iScrollSpeedFactor;
						 if(iScrollVariableRef > iMaxScroll)
							 iScrollVariableRef = iMaxScroll;
					 }
				 }
				 else
				 {
						if(iLastPositionDrag.iX < aTouchEvent.VirtualLocation.iX) //move everything to the right
						{
							iScrollVariableRef -= (aTouchEvent.VirtualLocation.iX - iLastPositionDrag.iX) * iScrollSpeedFactor;
							if(iScrollVariableRef < 0)
								iScrollVariableRef = 0;
						}else if(iLastPositionDrag.iX > aTouchEvent.VirtualLocation.iX) //move everything to the left
						{
							iScrollVariableRef += (iLastPositionDrag.iX - aTouchEvent.VirtualLocation.iX) * iScrollSpeedFactor;
							if(iScrollVariableRef > iMaxScroll)
								iScrollVariableRef = iMaxScroll;
						}
				 }

				 iLastPositionDrag = aTouchEvent.VirtualLocation;
			 }

			 break;
		 }
		 case ETouchEventHandlerTouchTypeUp:
		 {
			 if(iIdOfFinger == aTouchEvent.FingerId)
			 {
				 iTouched = false;
			 }

			 break;
		 }
	 }

	 return lHasBeenTouched;
}

void CTouchSurface::Draw()
{
	//do nothing
}

TBool CTouchSurface::GetIsTouched()
{
	return iTouched;
}

