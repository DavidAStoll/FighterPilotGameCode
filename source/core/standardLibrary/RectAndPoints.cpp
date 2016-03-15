/*
 * RectAndPoints.cpp
 *
 *  Created on: May 26, 2012
 *      Author: dstoll
 */

#include "includes/core/standardLibrary/CMath.h"
#include "includes/core/standardLibrary/RectAndPoints.h"

//------------------------------------------ TPoint -------------------------------------//

TPoint::TPoint()
{
	iX = 0;
	iY = 0;
}

TPoint::TPoint(TInt aX, TInt aY)
{
	iX = aX;
	iY = aY;
}

//------------------------------------------ TPointFloat -------------------------------------//

TPointIntFloat::TPointIntFloat()
{
	iX = TIntFloat::Convert(0);
	iY = TIntFloat::Convert(0);
}

TPointIntFloat::TPointIntFloat(TIntFloat aX, TIntFloat aY)
{
	iX = aX;
	iY = aY;
}

//------------------------------------------ TPointFloat64 -------------------------------------//

TPointIntFloat64::TPointIntFloat64()
{
	iX = TIntFloat64::Convert(0);
	iY = TIntFloat64::Convert(0);
}

TPointIntFloat64::TPointIntFloat64(TIntFloat64 aX, TIntFloat64 aY)
{
	iX = aX;
	iY = aY;
}

//------------------------------------------ TRECT -------------------------------------//

TRect::TRect()
{
	iTl.iX = 0; 	iTl.iY = 0;
	iBr.iX = 0; 	iBr.iY = 0;
	iWidth = 0;
	iHeight = 0;
}

TRect::TRect(const TPoint& aTlCorner, const TPoint& aBrCorner)
{
	iTl = aTlCorner;
	iBr = aBrCorner;
	iWidth = CMath::Abs(aBrCorner.iX - aTlCorner.iX);
	iHeight = CMath::Abs(aTlCorner.iY - aBrCorner.iY);
}

TRect::TRect(const TInt& aTlX, const TInt& aTlY, const TInt& aBrX, const TInt& aBrY)
{
	iTl.iX = aTlX;
	iTl.iY = aTlY;
	iBr.iX = aBrX;
	iBr.iY = aBrY;
	iWidth = CMath::Abs(iBr.iX - iTl.iX);
	iHeight = CMath::Abs(iTl.iY - iBr.iY);
}

TRect::TRect(const TPoint& aTlCorner, const TInt& aWidth, const TInt& aHeight)
{
	iTl = aTlCorner;
	iBr.iX = iTl.iX + aWidth;
	iBr.iY = iTl.iY - aHeight;
}

TBool TRect::Intersects(const TRect& aOtherRect) const
{
	if (aOtherRect.iTl.iY < iBr.iY)
		return false;
	else if (aOtherRect.iBr.iY > iTl.iY)
		return false;
	else if (aOtherRect.iTl.iX > iBr.iX)
		return false;
	else if (aOtherRect.iBr.iX < iTl.iX)
		return false;
	return true;
}

TBool TRect::Contains(const TPoint& aPoint) const
{
	if(iTl.iY >= aPoint.iY && iTl.iX <= aPoint.iX && iBr.iY <= aPoint.iY && iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

//------------------------------------------ TRECT_INTFLOAT-------------------------------------//

TRectIntFloat::TRectIntFloat()
{
	iTl.iX = TIntFloat::Convert(0); 	iTl.iY = TIntFloat::Convert(0);
	iBr.iX = TIntFloat::Convert(0); 	iBr.iY = TIntFloat::Convert(0);
	iWidth = TIntFloat::Convert(0);
	iHeight = TIntFloat::Convert(0);
}

TRectIntFloat::TRectIntFloat(const TPointIntFloat& aTlCorner, const TPointIntFloat& aBrCorner)
{
	iTl = aTlCorner;
	iBr = aBrCorner;
	iWidth = CMath::Abs(aBrCorner.iX - aTlCorner.iX);
	iHeight = CMath::Abs(aTlCorner.iY - aBrCorner.iY);
}

TRectIntFloat::TRectIntFloat(const TIntFloat& aTlX, const TIntFloat& aTlY, const TIntFloat& aBrX, const TIntFloat& aBrY)
{
	iTl.iX = aTlX;
	iTl.iY = aTlY;
	iBr.iX = aBrX;
	iBr.iY = aBrY;
	iWidth = CMath::Abs(iBr.iX - iTl.iX);
	iHeight = CMath::Abs(iTl.iY - iBr.iY);
}

TRectIntFloat::TRectIntFloat(const TPointIntFloat& aTlCorner, const TIntFloat& aWidth, const TIntFloat& aHeight)
{
	iTl = aTlCorner;
	iBr.iX = iTl.iX + aWidth;
	iBr.iY = iTl.iY - aHeight;
}

TBool TRectIntFloat::Intersects(const TRectIntFloat& aOtherRect) const
{
	if (aOtherRect.iTl.iY < iBr.iY)
		return false;
	else if (aOtherRect.iBr.iY > iTl.iY)
		return false;
	else if (aOtherRect.iTl.iX > iBr.iX)
		return false;
	else if (aOtherRect.iBr.iX < iTl.iX)
		return false;
	return true;
}

TBool TRectIntFloat::Contains(const TPointIntFloat& aPoint) const
{
	if(iTl.iY >= aPoint.iY && iTl.iX <= aPoint.iX && iBr.iY <= aPoint.iY && iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

//------------------------------------------ TRECT_INTFLOAT_64-------------------------------------//

TRectIntFloat64::TRectIntFloat64()
{
	iTl.iX = TIntFloat64::Convert(0); 	iTl.iY = TIntFloat64::Convert(0);
	iBr.iX = TIntFloat64::Convert(0); 	iBr.iY = TIntFloat64::Convert(0);
	iWidth = TIntFloat64::Convert(0);
	iHeight = TIntFloat64::Convert(0);
}

TRectIntFloat64::TRectIntFloat64(const TPointIntFloat64& aTlCorner, const TPointIntFloat64& aBrCorner)
{
	iTl = aTlCorner;
	iBr = aBrCorner;
	iWidth = CMath::Abs(aBrCorner.iX - aTlCorner.iX);
	iHeight = CMath::Abs(aTlCorner.iY - aBrCorner.iY);
}

TRectIntFloat64::TRectIntFloat64(const TIntFloat64& aTlX, const TIntFloat64& aTlY, const TIntFloat64& aBrX, const TIntFloat64& aBrY)
{
	iTl.iX = aTlX;
	iTl.iY = aTlY;
	iBr.iX = aBrX;
	iBr.iY = aBrY;
	iWidth = CMath::Abs(iBr.iX - iTl.iX);
	iHeight = CMath::Abs(iTl.iY - iBr.iY);
}

TRectIntFloat64::TRectIntFloat64(const TPointIntFloat64& aTlCorner, const TIntFloat64& aWidth, const TIntFloat64& aHeight)
{
	iTl = aTlCorner;
	iBr.iX = iTl.iX + aWidth;
	iBr.iY = iTl.iY - aHeight;
}

TBool TRectIntFloat64::Intersects(const TRectIntFloat64& aOtherRect) const
{
	if (aOtherRect.iTl.iY < iBr.iY)
		return false;
	else if (aOtherRect.iBr.iY > iTl.iY)
		return false;
	else if (aOtherRect.iTl.iX > iBr.iX)
		return false;
	else if (aOtherRect.iBr.iX < iTl.iX)
		return false;
	return true;
}

TBool TRectIntFloat64::Contains(const TPointIntFloat64& aPoint) const
{
	if(iTl.iY >= aPoint.iY && iTl.iX <= aPoint.iX && iBr.iY <= aPoint.iY && iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

//------------------------------------------ TSize -------------------------------------//

TSize::TSize()
{
	iWidth = 0;
	iHeight = 0;
}

TSize::TSize(const TInt& aWidth, const TInt& aHeight)
{
	iWidth = aWidth;
	iHeight = aHeight;
}

//------------------------------------------ TSize IntFloat -------------------------------------//

TSizeIntFloat::TSizeIntFloat()
{
	iWidth = TIntFloat::Convert(0);
	iHeight = TIntFloat::Convert(0);
}

TSizeIntFloat::TSizeIntFloat(const TIntFloat& aWidth, const TIntFloat& aHeight)
{
	iWidth = aWidth;
	iHeight = aHeight;
}

//------------------------------------------ TSize IntFloat64  -------------------------------------//

TSizeIntFloat64::TSizeIntFloat64()
{
	iWidth = TIntFloat64::Convert(0);
	iHeight = TIntFloat64::Convert(0);
}

TSizeIntFloat64::TSizeIntFloat64(const TIntFloat64& aWidth, const TIntFloat64& aHeight)
{
	iWidth = aWidth;
	iHeight = aHeight;
}



