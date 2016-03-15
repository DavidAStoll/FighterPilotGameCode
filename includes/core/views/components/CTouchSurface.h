/*
 * CTouchSurface.h
 *
 *  Created on: Jul 16, 2012
 *      Author: dstoll
 */

#ifndef CTOUCHSURFACE_H_
#define CTOUCHSURFACE_H_

#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/views/components/CBasicComponent.h"

class CTouchSurface : public CBasicComponent
{
public:

	enum TScrollAxis
	{
		EScrollAxisY,
		EScrollAxisX
	};

	static CTouchSurface* New(const TPoint& aBottomLeftCorner, const TSize& aSizeOfSurface, TFloat& aScrollRef, TInt aMaxScroll, TInt aScrollSpeedFactor, TScrollAxis aScrollAxis);
	virtual ~CTouchSurface();

	//checks if the surface has been touched
	TBool CheckIfHit(const STouchEventLocationConverted& aTouchEvent);
	//just not gonna draw anything
	virtual void Draw();
	//returns weither the surface is Touched or not
	TBool GetIsTouched();

protected:
	CTouchSurface(const TPoint& aBottomLeftCorner, const TSize& aSizeOfSurface, TFloat& aScrollRef, TInt aMaxScroll, TInt aScrollSpeedFactor, TScrollAxis aScrollAxis);

	TSize iSizeOfSurface;
	TScrollAxis iScrollAxis;
	TBool iTouched;
	TInt iIdOfFinger;
	TFloat& iScrollVariableRef; //modifies this value when surface scrolls
	TFloat iMaxScroll; //can't scroll more then this
	TFloat iScrollSpeedFactor; //determines how fast the scrolling will be for each virtual pixel
	TPoint iLastPositionDrag; //keep track of where the last drag has been, so we can calculate the speed of scrolling with it
};

#endif /* CTOUCHSURFACE_H_ */
