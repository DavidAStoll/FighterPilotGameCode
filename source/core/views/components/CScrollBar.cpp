/*
 * CScrollBar.cpp
 *
 *  Created on: Jul 17, 2012
 *      Author: dstoll
 */

#include "includes/core/views/components/CScrollBar.h"

CScrollBar::CScrollBar(const SButtonArguments& aButtonArg, const SScrollBarArguments& aScrollBarArg, TFloat& aScrollVariableRef)
:CButton(aButtonArg), iScrollVariableRef(aScrollVariableRef)
{
	iScrollAxis = aScrollBarArg.ScrollAxis;
	iMaxScroll = aScrollBarArg.MaxScroll; //can't scroll more then this
	iScrollSpeedFactor  = aScrollBarArg.ScrollSpeedFactor; //determines how fast the scrolling will be for each virtual pixel

	if(iScrollAxis == EScrollAxisY) //need to substract the area that is already occupied by the texture of the scrollbar
	{
		iMaxScrollHeight = aScrollBarArg.MaxScrollHeight - aScrollBarArg.SizeOfTexture.iHeight;
	}
	else
	{
		iMaxScrollHeight = aScrollBarArg.MaxScrollHeight - aScrollBarArg.SizeOfTexture.iWidth;
	}
	if(iMaxScrollHeight < 0 )
	{
		iMaxScrollHeight = 0;
	}

	UpdateLocation();
}

CScrollBar::~CScrollBar()
{
	// TODO Auto-generated destructor stub
}

CScrollBar* CScrollBar::New(SScrollBarArguments aArg, TFloat& aScrollVariableRef)
{
	CButton::SButtonArguments lButtonArgs;
	lButtonArgs.Location = aArg.Location;
	lButtonArgs.SizeOfHitbox = aArg.SizeOfHitbox;
	lButtonArgs.SizeOfTexture = aArg.SizeOfTexture;
	lButtonArgs.TextureIdDefault = aArg.TextureIdDefault;
	lButtonArgs.TextureIdSelected = aArg.TextureIdSelected;
	lButtonArgs.FontForLabel = NULL;
	lButtonArgs.Label = NULL;
	lButtonArgs.ColourForLabelDefault = 0;

	CScrollBar* lSelf = new CScrollBar(lButtonArgs, aArg, aScrollVariableRef);
	return lSelf;
}

TBool CScrollBar::CheckIfHit(const STouchEventLocationConverted& aTouchEvent)
{
	 TInt lX = iTextureObject->ReturnLowerLeftCoordinate().iX.GetIntInBaseInt();
	 TInt lY = iTextureObject->ReturnLowerLeftCoordinate().iY.GetIntInBaseInt();
	 TRect lHitBox = TRect(TPoint(lX,lY + iSizeOfHitBox.iHeight), iSizeOfHitBox.iWidth, iSizeOfHitBox.iHeight); //need to add Height, since we want iY to be the bottom left corner

	 TBool lHasBeenTouched = lHitBox.Contains(aTouchEvent.VirtualLocation);

	 switch(aTouchEvent.Type)
	 {
		 case ETouchEventHandlerTouchTypeDown:
		 {
			 if(lHasBeenTouched)
			 {
				 iLastPositionDrag = aTouchEvent.VirtualLocation;
				 iState = EStateSelected;
				 iIdOfFinger = aTouchEvent.FingerId;
			 }

			 break;
		 }
		 case ETouchEventHandlerTouchTypeDrag:
		 {
			 if(iState == EStateSelected && iIdOfFinger == aTouchEvent.FingerId)
			 {
				 if(iScrollAxis == EScrollAxisY)
				 {
					 if(iLastPositionDrag.iY > aTouchEvent.VirtualLocation.iY) //move everything down
					 {
						 iScrollVariableRef += (iLastPositionDrag.iY - aTouchEvent.VirtualLocation.iY) * iScrollSpeedFactor;
						 if(iScrollVariableRef > iMaxScroll)
							 iScrollVariableRef = iMaxScroll;
					 }
					 else if(iLastPositionDrag.iY < aTouchEvent.VirtualLocation.iY)//move everything up
					 {
						 iScrollVariableRef -= (aTouchEvent.VirtualLocation.iY - iLastPositionDrag.iY) * iScrollSpeedFactor;
						 if(iScrollVariableRef < 0)
							 iScrollVariableRef = 0;
					 }
				 }
				 else
				 {
						if(iLastPositionDrag.iX < aTouchEvent.VirtualLocation.iX) //move everything to the left
						{
							iScrollVariableRef += (aTouchEvent.VirtualLocation.iX - iLastPositionDrag.iX) * iScrollSpeedFactor;
							if(iScrollVariableRef > iMaxScroll)
								iScrollVariableRef = iMaxScroll;
						}else if(iLastPositionDrag.iX > aTouchEvent.VirtualLocation.iX) //move everything to the right
						{
							iScrollVariableRef -= (iLastPositionDrag.iX - aTouchEvent.VirtualLocation.iX) * iScrollSpeedFactor;
							if(iScrollVariableRef < 0)
								iScrollVariableRef = 0;
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
				 iState = EStateDefault;
			 }

			 break;
		 }
	 }

	 //always update ScrollBar Location, this way even if other objective modify the relative location the scroll bar will be updated as well
	 UpdateLocation();

	 return lHasBeenTouched;
}

void CScrollBar::UpdateLocation()
{
	 TFloat lPercentageScrolled = (TFloat) iScrollVariableRef / iMaxScroll;
	 TInt lOffsetNeeded = iMaxScrollHeight * lPercentageScrolled;

	 if(iScrollAxis == EScrollAxisY)
	 {
		 iTextureObject->SetNewLowerLeftCoordinate(TPointIntFloat(iX, iY - TIntFloat::Convert(lOffsetNeeded)));
	 }
	 else
	 {
		 iTextureObject->SetNewLowerLeftCoordinate(TPointIntFloat(iX + TIntFloat::Convert(lOffsetNeeded), iY));
	 }
}

