/*
 * CBasicComponent.cpp
 *
 *  Created on: Jul 14, 2012
 *      Author: dstoll
 */

#include "includes/core/views/components/CBasicComponent.h"

CBasicComponent::CBasicComponent(const TPoint& aLocation, SGraphicsEngineTextureId* aTextureId, TInt aWidth, TInt aHeight)
{
	iX = TIntFloat::Convert(aLocation.iX);
	iY = TIntFloat::Convert(aLocation.iY);

	if(aTextureId != NULL)
	{
		iTextureId = aTextureId;
		iTextureObject = CTextureObject::New(aLocation, aTextureId, aWidth, aHeight, 0, 0, 1, 0, 1);
		iGraphicsFont = CGame::Game->iGraphicsFont; // for convience
	}
	else
	{
		iTextureObject = NULL;
	}
}

CBasicComponent::~CBasicComponent()
{
	if(iTextureObject != NULL)
	{
		delete iTextureObject;
		iTextureObject = NULL;
	}
}

 void CBasicComponent::Draw()
{
	iTextureObject->DrawArrays();
}

TPoint CBasicComponent::GetLowerLeftLocation()
{
	TPoint lLowerLeft;
	lLowerLeft.iX = iX.GetIntInBaseInt();
	lLowerLeft.iY = iY.GetIntInBaseInt();
	return lLowerLeft;
}

void CBasicComponent::SetLowerLeftLocation(const TPoint& aLocation)
{
	iX = TIntFloat::Convert(aLocation.iX);
	iY = TIntFloat::Convert(aLocation.iY);
	iTextureObject->SetNewLowerLeftCoordinate(TPointIntFloat(iX, iY));
}

