/*
 * GraphicsMath.cpp
 *
 *  Created on: 24-May-2012
 *      Author: David Stoll
 *      © 2012 HQ Interactive Inc.
 */

#include <math.h>
#include <stdlib.h>
#include "includes/core/standardLibrary/CMath.h"

#define SLOPE_OF_INFINITY 65536000
#define MAX_ANGLE 360
#define MIN_ANGLE 0

TBool CMath::TrigTablesInitialized = false;
TIntFloat CMath::SinSimpleTable[GRAPHICS_DEGREES_PER_CIRCLE];
TIntFloat CMath::CosSimpleTable[GRAPHICS_DEGREES_PER_CIRCLE];

TInt CMath::AngleFromOriginBetweenPoints(TPointIntFloat lOriginPoint, TPointIntFloat lPoint2)
{
	TInt lXDifference = (lPoint2.iX - lOriginPoint.iX).GetIntInBaseInt();
	TInt lYDifference = (lPoint2.iY - lOriginPoint.iY).GetIntInBaseInt();
	TInt lXDiffAbs = CMath::Abs(lXDifference);
	TInt lYDiffAbs = CMath::Abs(lYDifference);
	TDouble lAngel;

	if (lXDiffAbs == 0) //avoid devision by 0
		lAngel = 90;
	else
	{
		lAngel = (TDouble) lYDiffAbs / lXDiffAbs;
		lAngel = atan(lAngel);
	}
	lAngel = (lAngel * 180) / PI; //convert back to degrees

	if (lXDifference < 0 && lYDifference >= 0)
		lAngel = 180 - lAngel;
	else if (lXDifference < 0 && lYDifference < 0)
		lAngel = 180 + lAngel;
	else if (lXDifference >= 0 && lYDifference < 0)
		lAngel = 360 - lAngel;

	return (TInt) lAngel;
}

void CMath::AdjustAngleToUnitCircle(TInt& aAngle)
{
	//check if the flying angel is more than 360 or equal or less than 0
	while (aAngle < MIN_ANGLE || aAngle >= MAX_ANGLE)
	{
		if (aAngle >= MAX_ANGLE)
			aAngle -= MAX_ANGLE;
		else if (aAngle < MIN_ANGLE)
			aAngle += MAX_ANGLE;
	}
}

TDouble CMath::Ceiling(const TDouble& aValue)
{
	return ceil(aValue);
}

TInt CMath::DistanceBetweenPoints(TPointIntFloat lPoint1, TPointIntFloat lPoint2)
{
	TInt lXDifference = (lPoint1.iX - lPoint2.iX).GetIntInBaseInt();
	TInt lYDifference = (lPoint1.iY - lPoint2.iY).GetIntInBaseInt();
	TDouble lResult = lXDifference * lXDifference + lYDifference * lYDifference;
	lResult = CMath::Sqrt(lResult);
	return (TInt) lResult;
}

TPointIntFloat CMath::GetCenterOfRect(const TRect& aRect)
{
	TPointIntFloat lCenterPoint;
	lCenterPoint.iX = TIntFloat::Convert((aRect.iTl.iX + aRect.iBr.iX) / 2);
	lCenterPoint.iY = TIntFloat::Convert((aRect.iTl.iY + aRect.iBr.iY) / 2);
	return lCenterPoint;
}

TPointIntFloat CMath::GetCenterOfRect(const TRectIntFloat& aRect)
{
	TPointIntFloat lCenterPoint;
	lCenterPoint.iX = (aRect.iTl.iX + aRect.iBr.iX) / 2;
	lCenterPoint.iY = (aRect.iTl.iY + aRect.iBr.iY) / 2;
	return lCenterPoint;
}

TPointIntFloat CMath::GetPointRelativeToCoordinates(const TPointIntFloat& aCoordinates, const TBool aReflected,
		const TInt aAngel, const TInt aXOffset, const TInt aYOffset)
{
	TPointIntFloat lResult = aCoordinates;
	if (aReflected)
	{
		lResult.iX += (CMath::GraphicsCosTable(aAngel) * (-aXOffset)) - (CMath::GraphicsSinTable(aAngel) * aYOffset);
		lResult.iY += (CMath::GraphicsCosTable(aAngel) * (+aYOffset)) - (CMath::GraphicsSinTable(aAngel) * aXOffset);
	}
	else
	{
		lResult.iX += (CMath::GraphicsCosTable(aAngel) * (-aXOffset)) + (CMath::GraphicsSinTable(aAngel) * aYOffset);
		lResult.iY += (CMath::GraphicsCosTable(aAngel) * (-aYOffset)) - (CMath::GraphicsSinTable(aAngel) * aXOffset);
	}
	return lResult;
}

TInt CMath::GetNextGreatestMemberOfPower(TInt aNumber, TInt aPower)
{
	//find out to what power of two the width has to be
	TDouble lReturnValue = aNumber;
	TDouble lLogOfPower = aPower;

	lLogOfPower = CMath::Log(lLogOfPower);
	lReturnValue = CMath::Log(lReturnValue);
	lReturnValue = lReturnValue / lLogOfPower; //this is basically Log(Number)basePower
	lReturnValue = CMath::Ceiling(lReturnValue); //go the next bigger Power interval
	lReturnValue = CMath::Power(aPower, lReturnValue); //get the number of this power interval
	return lReturnValue;
}

SLinearEquation CMath::GetLinearEquation(TPointIntFloat& aPoint1, TPointIntFloat& aPoint2)
{
	SLinearEquation lResult;
	//convert points to 64 bit ones
	TPointIntFloat64 lPoint1, lPoint2;
	lPoint1.iX = aPoint1.iX;
	lPoint1.iY = aPoint1.iY;
	lPoint2.iX = aPoint2.iX;
	lPoint2.iY = aPoint2.iY;

	if (lPoint2.iX - lPoint1.iX != 0)
		lResult.m = (lPoint2.iY - lPoint1.iY) / (lPoint2.iX - lPoint1.iX);
	else
		lResult.m = TIntFloat64::Convert(SLOPE_OF_INFINITY);

	lResult.b = lPoint1.iY - (lPoint1.iX * lResult.m);

	return lResult;
}

SLinearEquation CMath::GetLinearEquation(TPoint& aPoint1, TPoint& aPoint2)
{
	TPointIntFloat64 lPoint1, lPoint2;
	lPoint1.iX = TIntFloat64::Convert(aPoint1.iX);
	lPoint1.iY = TIntFloat64::Convert(aPoint1.iY);
	lPoint2.iX = TIntFloat64::Convert(aPoint2.iX);
	lPoint2.iY = TIntFloat64::Convert(aPoint2.iY);

	SLinearEquation lResult;
	if (lPoint2.iX - lPoint1.iX != 0)
		lResult.m = ((lPoint2.iY - lPoint1.iY) / (lPoint2.iX - lPoint1.iX));
	else
		lResult.m = TIntFloat64::Convert(SLOPE_OF_INFINITY);

	lResult.b = lPoint1.iY - (lPoint1.iX * lResult.m);

	return lResult;
}

TIntFloat CMath::GraphicsCosTable(TInt aDegree)
{
	if(!CMath::TrigTablesInitialized)
	{
		InitilizeSimpleTrigTables();
		CMath::TrigTablesInitialized = true;
	}

	aDegree = aDegree % GRAPHICS_DEGREES_PER_CIRCLE;

	//makes sure that Degree is between 0 and 360
	if (aDegree < 0)
		return CMath::CosSimpleTable[360 - CMath::Abs(aDegree)];
	else
		return CMath::CosSimpleTable[aDegree];
}

TIntFloat CMath::GraphicsSinTable(TInt aDegree)
{
	if(!CMath::TrigTablesInitialized)
	{
		InitilizeSimpleTrigTables();
		CMath::TrigTablesInitialized = true;
	}

	aDegree = aDegree % GRAPHICS_DEGREES_PER_CIRCLE;

	//makes sure that Degree is between 0 and 360
	if (aDegree < 0)
		return CMath::SinSimpleTable[360 - CMath::Abs(aDegree)];
	else
		return CMath::SinSimpleTable[aDegree];
}

TBool CMath::LinesIntersect(SLinearEquation& aLine1Eq, TPointIntFloat& aLine1Point1, TPointIntFloat& aLine1Point2,
		SLinearEquation& aLine2Eq, TPointIntFloat& aLine2Point1, TPointIntFloat& aLine2Point2)
{
	TIntFloat aLine1BiggerX, aLine1SmallerX, aLine2BiggerX, aLine2SmallerX;

	if (aLine1Point1.iX < aLine1Point2.iX)
	{
		aLine1BiggerX = aLine1Point2.iX;
		aLine1SmallerX = aLine1Point1.iX;
	}
	else
	{
		aLine1BiggerX = aLine1Point1.iX;
		aLine1SmallerX = aLine1Point2.iX;
	}
	if (aLine2Point1.iX < aLine2Point2.iX)
	{
		aLine2BiggerX = aLine2Point2.iX;
		aLine2SmallerX = aLine2Point1.iX;
	}
	else
	{
		aLine2BiggerX = aLine2Point1.iX;
		aLine2SmallerX = aLine2Point2.iX;
	}

	//check condition if slopes are the same
	if (aLine1Eq.m == aLine2Eq.m)
	{
		//lines are parallel, only if Bs are the same they could intersect
		if (aLine1Eq.b == aLine2Eq.b)
		{
			//intersection still possible
			if (aLine1SmallerX < aLine2SmallerX) // aLine1Point1 lies to the left of aLine2Point1
			{
				if (aLine1BiggerX >= aLine2SmallerX)
					return true; //if the right most point is beyound the left most point of Line2
				else
					return false;
			}
			else
			{
				if (aLine2BiggerX >= aLine1SmallerX)
					return true;
				else
					return false;
			}
		}
		else
			return false; // can't intersect
	}

	//lines are not parallel, so they must intersect at some point in space
	TIntFloat64 lIntersectionPointX;
	lIntersectionPointX = (aLine2Eq.b - aLine1Eq.b) / (aLine1Eq.m - aLine2Eq.m);

	//check if the intersection point is on both lines
	if ((lIntersectionPointX >= aLine1SmallerX && lIntersectionPointX <= aLine1BiggerX) && // the intesection point is on Line1
			(lIntersectionPointX >= aLine2SmallerX && lIntersectionPointX <= aLine2BiggerX)) // the intersection point is on Line2
		return true;
	else
		return false;
}

//checks if the two lines intersect with each other and return the intersection point
TPointIntFloat* CMath::LinesIntersectWithIntersectionPoint(SLinearEquation& aLine1Eq, TPointIntFloat& aLine1Point1,
		TPointIntFloat& aLine1Point2, SLinearEquation& aLine2Eq, TPointIntFloat& aLine2Point1, TPointIntFloat& aLine2Point2)
{
	TPointIntFloat* lCollisionPoint = NULL;
	TIntFloat aLine1BiggerX, aLine1SmallerX, aLine2BiggerX, aLine2SmallerX;

	if (aLine1Point1.iX < aLine1Point2.iX)
	{
		aLine1BiggerX = aLine1Point2.iX;
		aLine1SmallerX = aLine1Point1.iX;
	}
	else
	{
		aLine1BiggerX = aLine1Point1.iX;
		aLine1SmallerX = aLine1Point2.iX;
	}
	if (aLine2Point1.iX < aLine2Point2.iX)
	{
		aLine2BiggerX = aLine2Point2.iX;
		aLine2SmallerX = aLine2Point1.iX;
	}
	else
	{
		aLine2BiggerX = aLine2Point1.iX;
		aLine2SmallerX = aLine2Point2.iX;
	}

	//check condition if slopes are the same
	if (aLine1Eq.m == aLine2Eq.m)
	{
		//lines are parallel, only if Bs are the same they could intersect
		if (aLine1Eq.b == aLine2Eq.b)
		{
			//intersection still possible, however, we will have an infinite amount of intersection points so we just need to pick one
			if (aLine1SmallerX < aLine2SmallerX) // aLine1Point1 lies to the left of aLine2Point1
			{
				if (aLine1BiggerX >= aLine2SmallerX)
				{
					TIntFloat64 lIntersectionPointX = aLine2SmallerX;
					lCollisionPoint = new TPointIntFloat;
					lCollisionPoint->iX = lIntersectionPointX;
					lCollisionPoint->iY = (aLine2Eq.m * lIntersectionPointX) + aLine2Eq.b;
					return lCollisionPoint;
				}
				else
					return NULL;
			}
			else
			{
				if (aLine2BiggerX >= aLine1SmallerX)
				{
					TIntFloat64 lIntersectionPointX = aLine2SmallerX;
					lCollisionPoint = new TPointIntFloat;
					lCollisionPoint->iX = lIntersectionPointX;
					lCollisionPoint->iY = (aLine1Eq.m * lIntersectionPointX) + aLine1Eq.b;
					return lCollisionPoint;
				}
				else
					return NULL;
			}
		}
		else
			return NULL;
	}

	//lines are not parallel, so they must intersect at some point in space
	TIntFloat64 lIntersectionPointX;
	lIntersectionPointX = (aLine2Eq.b - aLine1Eq.b) / (aLine1Eq.m - aLine2Eq.m);

	//check if the intersection point is on both lines
	if ((lIntersectionPointX >= aLine1SmallerX && lIntersectionPointX <= aLine1BiggerX) && // the intesection point is on Line1
			(lIntersectionPointX >= aLine2SmallerX && lIntersectionPointX <= aLine2BiggerX)) // the intersection point is on Line2
	{
		lCollisionPoint = new TPointIntFloat;
		lCollisionPoint->iX = lIntersectionPointX;
		lCollisionPoint->iY = (aLine1Eq.m * lIntersectionPointX) + aLine1Eq.b;
		return lCollisionPoint;
	}
	else
		return NULL;
}

TDouble CMath::Log(const TDouble& aValue)
{
	return log10(aValue);
}

TBool CMath::PointIsInsideRectangleOpenGlCoordinateSystem (const TPointIntFloat& aPoint,const TRectIntFloat& aRect)
{
	if(aRect.iTl.iY >= aPoint.iY && aRect.iTl.iX <= aPoint.iX && aRect.iBr.iY <= aPoint.iY && aRect.iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

TBool CMath::PointIsInsideRectangleOpenGlCoordinateSystem (const TPoint& aPoint, const TRect& aRect)
{
	if(aRect.iTl.iY >= aPoint.iY && aRect.iTl.iX <= aPoint.iX && aRect.iBr.iY <= aPoint.iY && aRect.iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

TBool CMath::PointIsInsideRectangleWindowCoordinateSystem (const TPointIntFloat& aPoint,const TRectIntFloat& aRect)
{
	if(aRect.iTl.iY <= aPoint.iY && aRect.iTl.iX <= aPoint.iX && aRect.iBr.iY >= aPoint.iY && aRect.iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

TBool CMath::PointIsInsideRectangleWindowCoordinateSystem (const TPoint& aPoint, const TRect& aRect)
{
	if(aRect.iTl.iY <= aPoint.iY && aRect.iTl.iX <= aPoint.iX && aRect.iBr.iY >= aPoint.iY && aRect.iBr.iX >= aPoint.iX)
		return true;
	else
		return false;
}

TDouble CMath::Power(const TDouble& aBase, const TDouble& aExponent)
{
	return pow(aBase, aExponent);
}

TDouble CMath::Sqrt(const TDouble& aValue)
{
	return sqrt(aValue);
}

void CMath::InitilizeSimpleTrigTables()
{
	for (TInt lIndex = 0; lIndex < GRAPHICS_DEGREES_PER_CIRCLE; lIndex++)
	{
		CMath::SinSimpleTable[lIndex] = TIntFloat::Convert((CMath::GraphicsSin(lIndex))); //can't use shift on floats
		CMath::CosSimpleTable[lIndex] = TIntFloat::Convert((CMath::GraphicsCos(lIndex)));
	}
}

TDouble CMath::GraphicsCos(TDouble aDegrees)
{
	TDouble lResult;
	lResult = cos((aDegrees / 180) * PI); //convert to Radians, since function expects radians
	return lResult;
}

TDouble CMath::GraphicsSin(TDouble aDegrees)
{
	TDouble lResult;
	lResult = sin((aDegrees / 180) * PI); //convert to Radians, since function expects radians
	return lResult;
}
