/*
 * CBasicView.h
 *
 *  Created on: Jul 5, 2012
 *      Author: dstoll
 */

#ifndef CBASICVIEW_H_
#define CBASICVIEW_H_

#include "includes/core/CGame.h"
#include "includes/core/graphic/CTextureObject.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CImageStore.h"
#include "includes/core/utility/CTouchEventHandler.h"

#define PREFERRED_FONT_SIZE_FOR_TEXT_IN_BOX 40

class CBasicView
{
	friend class CBasicTransition; //not very nice, but basic transition should be friend otherwise it can't call the childdrawMethods for its views

public:
	virtual ~CBasicView();

	//this method is called from the framework if a new TouchEvent is available to be consumed
	void ConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	//this method is called when the context is gained for the view, application resumed from background
	//by defauly it will Resume all Audio
	virtual void ContextGained();
	//this method is called the context is lost for the view, application is put into background
	//by defauly it stop all Audio
	virtual void ContextLost();
	//this method should be called to draw the view, will do some book keeping before calling the child draw method
	void Draw();
	//this method contains all the Logic code that runs to update the state of the view, most views don't use it
	void RunLogicCode();
	//Sets if the View undergoing a Transition
	void SetIsDoingTransition(TBool aValue);


protected:

	CBasicView();
	//this TouchEvent method should be overriden by the child view
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent) = 0;
	//this draw Method should be overriden by the child view
	virtual void ChildViewDrawMethod() = 0;
	//this  method should be overriden by the child view and should contain any code if any that should run before the Draw calls
	virtual void ChildRunLogicCode();
	//just a convience method returns the Point where the String should be drawn with the given requirements
	void DrawCenteredString(TPoint aLeftBottomCorner, const SFont& aDrawFont, const CString* aString, TInt aMaxWidth, TInt aMaxHeight);
	//just a convient method that draws a text string into a Box with the given restrictoms
	void DrawStringIntoBox(TPoint aTopRightStartLocation, TInt aLineWidth, TRect aBoxOutline, SFont& aFont, const CString* aString);
	//returns how many virtual Pixels in Height(Y) will be needed to display the entire text in the box with the given font and line width and string
	TInt GetMaxHeightOfTextInBox(TInt aLineWidth, SFont& aFont, const CString* aString);

	//tells us if the View is Ready to Handle Input, since creation of the new View might take some time, this avoid that the view gets modified before it is displayed at least once
	TBool iIsDoingTransition;
	TBool iIsReadyToHandleInput;
	CGraphicsFont* iGraphicsFont; //for convience
	CImageStore* iImageStore; //for convience
	CGraphicsEngine* iGraphicsEngine; //for convience
	CLocalizationEngine* iLocalizationEngine; //for convience
};

#endif /* CBASICVIEW_H_ */
