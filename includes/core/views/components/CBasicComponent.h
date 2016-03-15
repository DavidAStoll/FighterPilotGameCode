/*
 * CBasicComponent.h
 *
 *  Created on: Jul 14, 2012
 *      Author: dstoll
 */

#ifndef CBASICCOMPONENT_H_
#define CBASICCOMPONENT_H_

#include "includes/core/graphic/CTextureObject.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/CGame.h"
#include "includes/core/utility/CImageStore.h"
#include "includes/core/utility/CTouchEventHandler.h"

class CBasicComponent
{
public:
	virtual ~CBasicComponent();
	//Draws the Component
	virtual void Draw();
	//returns the Lower Left Location of the component
	TPoint GetLowerLeftLocation();
	//changes the Location of the component to the new Location
	void SetLowerLeftLocation(const TPoint& aLocation);

protected:
	CBasicComponent(const TPoint& aLocation, SGraphicsEngineTextureId* aTextureId, TInt aWidth, TInt aHeight);

	TIntFloat iX;
	TIntFloat iY;
	CTextureObject* iTextureObject;
	SGraphicsEngineTextureId* iTextureId;
	CGraphicsFont* iGraphicsFont;
};

#endif /* CBASICCOMPONENT_H_ */
