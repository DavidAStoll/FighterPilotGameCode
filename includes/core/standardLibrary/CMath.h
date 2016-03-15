/*
 * CMath.h
 *
 *  Created on: May 24, 2012
 *      Author: dstoll
 */

#ifndef CMATH_H_
#define CMATH_H_

#include "includes/core/standardLibrary/BasicTypes.h"
#include "includes/core/standardLibrary/TIntFloat.h"
#include "includes/core/standardLibrary/TIntFloat64.h"
#include "includes/core/standardLibrary/RectAndPoints.h"

#define PI 3.141592654
#define GRAPHICS_DEGREES_PER_CIRCLE 360

struct SLinearEquation
{
  TIntFloat64 m;//slope
  TIntFloat64 b;// offset
};

class CMath
{

public:

static inline TInt Abs(TInt aValue)
{
	return (aValue < 0) ? - aValue : aValue;
}

static inline TIntFloat Abs(TIntFloat aValue)
{
	return (aValue.GetIntInBaseIntFloat() < 0) ? aValue *= -1 : aValue;
}

static inline TIntFloat64 Abs(TIntFloat64 aValue)
{
	return (aValue.GetIntInBaseIntFloat64() < 0) ? aValue *= -1 : aValue;
}

//find the angle that two points makes from the origin
static TInt AngleFromOriginBetweenPoints(TPointIntFloat lOriginPoint, TPointIntFloat lPoint2);
//scales down angle between 0 and 360, like 460 would become 100
static void AdjustAngleToUnitCircle(TInt& aAngle);
//return the next biggest integer
static TDouble Ceiling(const TDouble& aValue);
//returns the distance between two points in 2D space
static TInt DistanceBetweenPoints(TPointIntFloat lPoint1,TPointIntFloat lPoint2);
//return the Center of the Rect
static TPointIntFloat GetCenterOfRect(const TRect& aRect);
static TPointIntFloat GetCenterOfRect(const TRectIntFloat& aRect);
//return a Point that lays off from a certain coordiante point at a certain angle, X and Y offset
static TPointIntFloat GetPointRelativeToCoordinates(const TPointIntFloat& aCoordinates,const TBool aReflected,const TInt aAngel,const TInt aXOffset,const TInt aYOffset);
//returns the next member of a power, which basically means it will scale up the number to find the number that is part of the power. For example for the number 14, and the power 2, it will return 16, since 2^4
static TInt GetNextGreatestMemberOfPower(TInt aNumber, TInt aPower);
// returns an mx + b = y linear equation
static SLinearEquation GetLinearEquation(TPointIntFloat& aPoint1, TPointIntFloat& aPoint2);
static SLinearEquation GetLinearEquation(TPoint& aPoint1, TPoint& aPoint2);
//return the TIntfloat value that corresponds to the Cos value of the degree
static TIntFloat GraphicsCosTable(TInt aDegree);
//return the TIntfloat value that corresponds to the Sin value of the degree
static TIntFloat GraphicsSinTable(TInt aDegree);
//checks if the two lines intersect with each other
static TBool LinesIntersect(SLinearEquation& aLine1Eq,TPointIntFloat& aLine1Point1,TPointIntFloat& aLine1Point2,
                            SLinearEquation& aLine2Eq,TPointIntFloat& aLine2Point1,TPointIntFloat& aLine2Point2);
//checks if the two lines intersect with each other and return the intersection point
static TPointIntFloat* LinesIntersectWithIntersectionPoint(SLinearEquation& aLine1Eq,TPointIntFloat& aLine1Point1,TPointIntFloat& aLine1Point2,
                            SLinearEquation& aLine2Eq,TPointIntFloat& aLine2Point1,TPointIntFloat& aLine2Point2);
//compute log of base 10
static TDouble Log(const TDouble& aValue);
//checks if the Point is located within the Rectangle
static TBool PointIsInsideRectangleOpenGlCoordinateSystem (const TPointIntFloat& aPoint,const TRectIntFloat& aRect);
static TBool PointIsInsideRectangleOpenGlCoordinateSystem (const TPoint& aPoint, const TRect& aRect);
static TBool PointIsInsideRectangleWindowCoordinateSystem (const TPointIntFloat& aPoint,const TRectIntFloat& aRect);
static TBool PointIsInsideRectangleWindowCoordinateSystem (const TPoint& aPoint, const TRect& aRect);
//return the value of base^power
static TDouble Power(const TDouble& aBase, const TDouble& aExponent);
//return the Sqrt of a value
static TDouble Sqrt(const TDouble& aValue);

private:

//this function should be called, before making any calls to the Cos or Sin Tables since it will populate them with the correct data
static void InitilizeSimpleTrigTables();
static TDouble GraphicsCos(TDouble aDegrees);
static TDouble GraphicsSin(TDouble aDegrees);

//create as static members for easy reference
static TBool TrigTablesInitialized;
static TIntFloat SinSimpleTable [GRAPHICS_DEGREES_PER_CIRCLE];
static TIntFloat CosSimpleTable [GRAPHICS_DEGREES_PER_CIRCLE];
};

#endif /* CMATH_H_ */
