#ifndef RECTANDPOINTS_H_
#define RECTANDPOINTS_H_

#include "includes/core/standardLibrary/BasicTypes.h"
#include "includes/core/standardLibrary/TIntFloat.h"
#include "includes/core/standardLibrary/TIntFloat64.h"

class TPoint
{
public:
	TPoint();
	TPoint(TInt aX, TInt aY);

	//data members
	TInt iX;
	TInt iY;
};

class TPointIntFloat
{
public:
	TPointIntFloat();
	TPointIntFloat(TIntFloat aX, TIntFloat aY);

	//data members
	TIntFloat iX;
	TIntFloat iY;
};

class TPointIntFloat64
{
public:
	TPointIntFloat64();
	TPointIntFloat64(TIntFloat64 aX, TIntFloat64 aY);

	//data members
	TIntFloat64 iX;
	TIntFloat64 iY;
};

class TRect
{
public:
	TRect();
	TRect(const TPoint& aTlCorner, const TPoint& aBrCorner);
	TRect(const TInt& aTlX, const TInt& aTlY, const TInt& aBrX, const TInt& aBrY);
	TRect(const TPoint& aTlCorner, const TInt& aWidth, const TInt& aHeight);
	TBool Intersects(const TRect& aOtherRect) const;
	TBool Contains(const TPoint& aPoint) const;

	//data members
	TPoint iTl;
	TPoint iBr;
	TInt iWidth;
	TInt iHeight;
};

class TRectIntFloat
{
public:
	TRectIntFloat();
	TRectIntFloat(const TPointIntFloat& aTlCorner, const TPointIntFloat& aBrCorner);
	TRectIntFloat(const TIntFloat& aTlX, const TIntFloat& aTlY, const TIntFloat& aBrX, const TIntFloat& aBrY);
	TRectIntFloat(const TPointIntFloat& aTlCorner, const TIntFloat& aWidth, const TIntFloat& aHeight);
	TBool Intersects(const TRectIntFloat& aOtherRect) const;
	TBool Contains(const TPointIntFloat& aPoint) const;

	//data members
	TPointIntFloat iTl;
	TPointIntFloat iBr;
	TIntFloat iWidth;
	TIntFloat iHeight;
};

class TRectIntFloat64
{
public:
	TRectIntFloat64();
	TRectIntFloat64(const TPointIntFloat64& aTlCorner, const TPointIntFloat64& aBrCorner);
	TRectIntFloat64(const TIntFloat64& aTlX, const TIntFloat64& aTlY, const TIntFloat64& aBrX, const TIntFloat64& aBrY);
	TRectIntFloat64(const TPointIntFloat64& aTlCorner, const TIntFloat64& aWidth, const TIntFloat64& aHeight);
	TBool Intersects(const TRectIntFloat64& aOtherRect) const;
	TBool Contains(const TPointIntFloat64& aPoint) const;

	//data members
	TPointIntFloat64 iTl;
	TPointIntFloat64 iBr;
	TIntFloat64 iWidth;
	TIntFloat64 iHeight;
};

class TSize
{
public:
	TSize();
	TSize(const TInt& aWidht, const TInt& aHeight);

	//data members
	TInt iWidth;
	TInt iHeight;
};

class TSizeIntFloat
{
public:
	TSizeIntFloat();
	TSizeIntFloat(const TIntFloat& aWidht, const TIntFloat& aHeight);

	//data members
	TIntFloat iWidth;
	TIntFloat iHeight;
};

class TSizeIntFloat64
{
public:
	TSizeIntFloat64();
	TSizeIntFloat64(const TIntFloat64& aWidht, const TIntFloat64& aHeight);

	//data members
	TIntFloat64 iWidth;
	TIntFloat64 iHeight;
};



#endif
