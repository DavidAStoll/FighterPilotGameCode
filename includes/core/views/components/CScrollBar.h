/*
 * CScrollBar.h
 *
 *  Created on: Jul 17, 2012
 *      Author: dstoll
 */

#ifndef CSCROLLBAR_H_
#define CSCROLLBAR_H_

#include "includes/core/views/components/CButton.h"

class CScrollBar : public CButton
{
	public:

	enum TScrollAxis
	{
		EScrollAxisY,
		EScrollAxisX
	};

	struct SScrollBarArguments
	{
		TPoint Location;
		TSize SizeOfHitbox;
		SGraphicsEngineTextureId* TextureIdDefault;
		SGraphicsEngineTextureId* TextureIdSelected;
		TSize SizeOfTexture;
		TScrollAxis ScrollAxis;
		TFloat MaxScroll;
		TFloat MaxScrollHeight; //how much the scroll bar can move in the interface
		TFloat ScrollSpeedFactor;
	};

	static CScrollBar* New(SScrollBarArguments aArg, TFloat& aScrollVariableRef);
	virtual ~CScrollBar();

	//checks if the ScrollBar has been touched or released
	virtual TBool CheckIfHit(const STouchEventLocationConverted& aTouchEvent);

protected:
	CScrollBar(const SButtonArguments& aButtonArg, const SScrollBarArguments& aScrollBarArg, TFloat& aScrollVariableRef);
	//relocates the ScrollBar Location depending on the ScrollVariableRef value and its MaxScrollHeight
	void UpdateLocation();

	TScrollAxis iScrollAxis;
	TFloat& iScrollVariableRef; //modifies this value when surface scrolls
	TFloat iMaxScroll; //can't scroll more then this
	TFloat iMaxScrollHeight;
	TFloat iScrollSpeedFactor; //determines how fast the scrolling will be for each virtual pixel
	TPoint iLastPositionDrag; //keep track of where the last drag has been, so we can calculate the speed of scrolling with it
};

#endif /* CSCROLLBAR_H_ */
