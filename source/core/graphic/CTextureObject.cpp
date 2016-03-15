/*
 * CTextureObject.cpp
 *
 *  Created on: Jun 11, 2012
 *      Author: dstoll
 */

#include "includes/core/graphic/CTextureObject.h"
#include "includes/core/CGame.h"


//creates a sqaure polygon, should be used with TRIANGLE_STRIP draw method
CTextureObject::CTextureObject(TPoint aLowerLeftCorner,SGraphicsEngineTextureId* aTextureId,TInt aWidth,TInt aHeight,TInt aZorCor,TFloat aTextureSLowerValue,TFloat aTextureSUpperValue,TFloat aTextureTLowerValue,TFloat aTextureTUpperValue)
{
	iPolygon.TopLeftCorner.Coordinate.x=TIntFloat::Convert(aLowerLeftCorner.iX);        			iPolygon.TopLeftCorner.Coordinate.y=TIntFloat::Convert(aLowerLeftCorner.iY+aHeight);     iPolygon.TopLeftCorner.Coordinate.z= aZorCor; //left top corner
	iPolygon.TopRightCorner.Coordinate.x=TIntFloat::Convert(aLowerLeftCorner.iX+aWidth); 		iPolygon.TopRightCorner.Coordinate.y=TIntFloat::Convert(aLowerLeftCorner.iY+aHeight);    iPolygon.TopRightCorner.Coordinate.z=aZorCor; //right top corner
	iPolygon.BottomLeftCorner.Coordinate.x=TIntFloat::Convert(aLowerLeftCorner.iX);        	iPolygon.BottomLeftCorner.Coordinate.y=TIntFloat::Convert(aLowerLeftCorner.iY);          iPolygon.BottomLeftCorner.Coordinate.z=aZorCor; //left bottom corner
	iPolygon.BottomRightCorner.Coordinate.x=TIntFloat::Convert(aLowerLeftCorner.iX+aWidth); 	iPolygon.BottomRightCorner.Coordinate.y=TIntFloat::Convert(aLowerLeftCorner.iY);         iPolygon.BottomRightCorner.Coordinate.z=aZorCor; //right bottom corner

	//save state for unrotated vertexes
	iPolygonUnrotated = iPolygon;

	//setup to default colour
	iPolygon.TopLeftCorner.Colour.Red = 255;
	iPolygon.TopLeftCorner.Colour.Green = 255;
	iPolygon.TopLeftCorner.Colour.Blue = 255;
	iPolygon.TopLeftCorner.Colour.Alpha = 255;
	iPolygon.TopRightCorner.Colour = iPolygon.TopLeftCorner.Colour;
	iPolygon.BottomLeftCorner.Colour = iPolygon.TopLeftCorner.Colour;
	iPolygon.BottomRightCorner.Colour = iPolygon.TopLeftCorner.Colour;

  iTextureId=aTextureId;
  iReflectedOverXAxis=false;
  iReflectedOverYAxis=false;
  iWidth = TIntFloat::Convert(aWidth);
  iHeight = TIntFloat::Convert(aHeight);
  iCurrentAngel=0;
  iOpenGlCacheEntry = NULL;

  //Update Texture values
  iLowerSValue = TIntFloat::Convert(aTextureSLowerValue);
  iUpperSValue = TIntFloat::Convert(aTextureSUpperValue);
  iLowerTValue = TIntFloat::Convert(aTextureTLowerValue);
  iUpperTValue = TIntFloat::Convert(aTextureTUpperValue);
  SetNewSValueIntFloat(iLowerSValue, iUpperSValue);
  SetNewTValueIntFloat(iLowerTValue, iUpperTValue);
}

CTextureObject::CTextureObject(TPointIntFloat aLowerLeftCorner,SGraphicsEngineTextureId* aTextureId,TIntFloat aWidth,TIntFloat aHeight,TInt aZorCor,TFloat aTextureSLowerValue,TFloat aTextureSUpperValue,TFloat aTextureTLowerValue,TFloat aTextureTUpperValue)
{
	iPolygon.TopLeftCorner.Coordinate.x=(aLowerLeftCorner.iX);        			iPolygon.TopLeftCorner.Coordinate.y=(aLowerLeftCorner.iY+aHeight);    iPolygon.TopLeftCorner.Coordinate.z=aZorCor; //left top corner
	iPolygon.TopRightCorner.Coordinate.x=(aLowerLeftCorner.iX+aWidth); 		iPolygon.TopRightCorner.Coordinate.y=(aLowerLeftCorner.iY+aHeight);   iPolygon.TopRightCorner.Coordinate.z=aZorCor; //right top corner
	iPolygon.BottomLeftCorner.Coordinate.x=(aLowerLeftCorner.iX);        	iPolygon.BottomLeftCorner.Coordinate.y=(aLowerLeftCorner.iY);         iPolygon.BottomLeftCorner.Coordinate.z=aZorCor; //left bottom corner
	iPolygon.BottomRightCorner.Coordinate.x=(aLowerLeftCorner.iX+aWidth); 	iPolygon.BottomRightCorner.Coordinate.y=(aLowerLeftCorner.iY);        iPolygon.BottomRightCorner.Coordinate.z=aZorCor; //right bottom corner

	//save state for unrotated vertexes
	iPolygonUnrotated = iPolygon;

	//setup to default colour
	iPolygon.TopLeftCorner.Colour.Red = 255;
	iPolygon.TopLeftCorner.Colour.Green = 255;
	iPolygon.TopLeftCorner.Colour.Blue = 255;
	iPolygon.TopLeftCorner.Colour.Alpha = 255;
	iPolygon.TopRightCorner.Colour = iPolygon.TopLeftCorner.Colour;
	iPolygon.BottomLeftCorner.Colour = iPolygon.TopLeftCorner.Colour;
	iPolygon.BottomRightCorner.Colour = iPolygon.TopLeftCorner.Colour;

  iTextureId=aTextureId;
  iReflectedOverXAxis=false;
  iReflectedOverYAxis=false;
  iWidth = aWidth;
  iHeight = aHeight;
  iCurrentAngel=0;
  iOpenGlCacheEntry = NULL;

  //Update Texture values
  iLowerSValue = TIntFloat::Convert(aTextureSLowerValue);
  iUpperSValue = TIntFloat::Convert(aTextureSUpperValue);
  iLowerTValue = TIntFloat::Convert(aTextureTLowerValue);
  iUpperTValue = TIntFloat::Convert(aTextureTUpperValue);
  SetNewSValueIntFloat(iLowerSValue, iUpperSValue);
  SetNewTValueIntFloat(iLowerTValue, iUpperTValue);

  //iIndicies = new TInt[6];
}

CTextureObject::~CTextureObject()
{
}

CTextureObject* CTextureObject::New(TPoint aLowerLeftCorner,SGraphicsEngineTextureId* aTextureId,TInt aWidth,TInt aHeight,TInt aZorCor,TFloat aTextureSLowerValue,TFloat aTextureSUpperValue,TFloat aTextureTLowerValue,TFloat aTextureTUpperValue)
{
	return new CTextureObject(aLowerLeftCorner, aTextureId, aWidth, aHeight, aZorCor, aTextureSLowerValue, aTextureSUpperValue, aTextureTLowerValue, aTextureTUpperValue);
}

CTextureObject* CTextureObject::New(TPointIntFloat aLowerLeftCorner,SGraphicsEngineTextureId* aTextureId,TIntFloat aWidth,TIntFloat aHeight,TInt aZorCor,TFloat aTextureSLowerValue,TFloat aTextureSUpperValue,TFloat aTextureTLowerValue,TFloat aTextureTUpperValue)
{
	return new CTextureObject(aLowerLeftCorner, aTextureId, aWidth, aHeight, aZorCor, aTextureSLowerValue, aTextureSUpperValue, aTextureTLowerValue, aTextureTUpperValue);
}


//--------------------------functions-----------------------------------

//assigns a new texture Handle
void CTextureObject::AssignTexture(SGraphicsEngineTextureId* aTextureId)
{
	iTextureId = aTextureId;

	//need to adjust S and T values since MaxS and MaxT values might be different from precious texture
	SetNewSValueIntFloat(iLowerSValue, iUpperSValue);
	SetNewTValueIntFloat(iLowerTValue, iUpperTValue);
}

void CTextureObject::ChangeHeight(const TIntFloat& aChange)
{
  iHeight += aChange;
  TIntFloat lAdjustedChange = aChange / 2;
  TIntFloat lYChange = lAdjustedChange * CMath::GraphicsCosTable(iCurrentAngel); //should always be positive
  TIntFloat lXChange = lAdjustedChange * CMath::GraphicsSinTable(iCurrentAngel); //should always be positive

  iPolygon.TopLeftCorner.Coordinate.y 		+= 	lYChange; //Top corner
  iPolygon.TopRightCorner.Coordinate.y 		+= 	lYChange; //Top corner
  iPolygon.BottomLeftCorner.Coordinate.y 	-= 	lYChange; //Bottom corner
  iPolygon.BottomRightCorner.Coordinate.y -= 	lYChange; //Bottom corner

  //also need to adjust the X axis if rotate
  iPolygon.TopLeftCorner.Coordinate.x 		-= 	lXChange; //Top corner
  iPolygon.TopRightCorner.Coordinate.x 		-= 	lXChange; //Top corner
  iPolygon.BottomLeftCorner.Coordinate.x 	+= 	lXChange; //Bottom corner
  iPolygon.BottomRightCorner.Coordinate.x += 	lXChange; //Bottom corner

  //update unrotated vertexes
  iPolygonUnrotated.TopLeftCorner.Coordinate.y 			+= lAdjustedChange; //Top corner
  iPolygonUnrotated.TopRightCorner.Coordinate.y 		+= lAdjustedChange; //Top corner
  iPolygonUnrotated.BottomLeftCorner.Coordinate.y 	-= lAdjustedChange; //Bottom corner
  iPolygonUnrotated.BottomRightCorner.Coordinate.y 	-= lAdjustedChange; //Bottom corner
}

void CTextureObject::ChangeWidth(const TIntFloat& aChange)
{
  iWidth += aChange;
  TIntFloat lAdjustedChange = aChange / 2;
  TIntFloat lXChange = lAdjustedChange * CMath::GraphicsCosTable(iCurrentAngel); //should always be positive
  TIntFloat lYChange = lAdjustedChange * CMath::GraphicsSinTable(iCurrentAngel); //should always be positive

  //make adjustments to x
  iPolygon.TopRightCorner.Coordinate.x    += 	lXChange;//right corner
  iPolygon.BottomRightCorner.Coordinate.x += 	lXChange;//right corner
  iPolygon.TopLeftCorner.Coordinate.x			-= 	lXChange;//left corner
  iPolygon.BottomLeftCorner.Coordinate.x	-= 	lXChange;//left corner

  //also need to scale y because of possible rotation
  iPolygon.TopRightCorner.Coordinate.y		+= 	lYChange;//right corner
  iPolygon.BottomRightCorner.Coordinate.y	+= 	lYChange;//right corner
  iPolygon.TopLeftCorner.Coordinate.y			-= 	lYChange;//left corner
  iPolygon.BottomLeftCorner.Coordinate.y	-= 	lYChange;//left corner

  //these are unrotated, so angle does not matter
  iPolygonUnrotated.TopRightCorner.Coordinate.x 		+= 	lAdjustedChange;//right corner
  iPolygonUnrotated.BottomRightCorner.Coordinate.x 	+=  lAdjustedChange;//right corner
  iPolygonUnrotated.TopLeftCorner.Coordinate.x 			-= 	lAdjustedChange;//left corner
  iPolygonUnrotated.BottomLeftCorner.Coordinate.x 	-= 	lAdjustedChange;//left corner
}

//moves the all vertexes by the given Change on the X axis
void CTextureObject::ChangeXCoordinate(TIntFloat aChange)
{
	iPolygon.TopRightCorner.Coordinate.x			+= aChange;
	iPolygon.TopLeftCorner.Coordinate.x			+= aChange;
	iPolygon.BottomRightCorner.Coordinate.x 	+= aChange;
	iPolygon.BottomLeftCorner.Coordinate.x 	+= aChange;

	iPolygonUnrotated.TopRightCorner.Coordinate.x			+= aChange;
	iPolygonUnrotated.TopLeftCorner.Coordinate.x				+= aChange;
	iPolygonUnrotated.BottomRightCorner.Coordinate.x 	+= aChange;
	iPolygonUnrotated.BottomLeftCorner.Coordinate.x 		+= aChange;
}

//moves the all vertexes by the given change on the Y axis
void CTextureObject::ChangeYCoordinate(TIntFloat aChange)
{
	iPolygon.TopRightCorner.Coordinate.y			+= aChange;
	iPolygon.TopLeftCorner.Coordinate.y			+= aChange;
	iPolygon.BottomRightCorner.Coordinate.y 	+= aChange;
	iPolygon.BottomLeftCorner.Coordinate.y 	+= aChange;

	iPolygonUnrotated.TopRightCorner.Coordinate.y			+= aChange;
	iPolygonUnrotated.TopLeftCorner.Coordinate.y				+= aChange;
	iPolygonUnrotated.BottomRightCorner.Coordinate.y 	+= aChange;
	iPolygonUnrotated.BottomLeftCorner.Coordinate.y 		+= aChange;
}

void CTextureObject::ChangeSCoordinate(TFloat aChange)
{
	iPolygon.TopRightCorner.Texture.s += TIntFloat::Convert(aChange);
	iPolygon.TopLeftCorner.Texture.s += TIntFloat::Convert(aChange);
	iPolygon.BottomRightCorner.Texture.s += TIntFloat::Convert(aChange);
	iPolygon.BottomLeftCorner.Texture.s += TIntFloat::Convert(aChange);
}

void CTextureObject::ChangeTCoordinate(TFloat aChange)
{
	iPolygon.TopRightCorner.Texture.t += TIntFloat::Convert(aChange);
	iPolygon.TopLeftCorner.Texture.t += TIntFloat::Convert(aChange);
	iPolygon.BottomRightCorner.Texture.t += TIntFloat::Convert(aChange);
	iPolygon.BottomLeftCorner.Texture.t += TIntFloat::Convert(aChange);
}

void CTextureObject::DrawArrays()
{
	iOpenGlCacheEntry = CGame::Game->iGraphicsEngine->RefreshOpenGlCacheEntry(*iTextureId, iOpenGlCacheEntry); //make sure texture is ready to be drawn
	CGame::Game->iGraphicsEngine->DrawTextureBatched(iPolygon, iTextureId->OpenGlId);
}

void CTextureObject::DrawArraysWithoutAlpha()
{
	CGame::Game->iGraphicsEngine->SetGraphicsEngineSettings(GL_ALPHA_TEST, false);
	iOpenGlCacheEntry = CGame::Game->iGraphicsEngine->RefreshOpenGlCacheEntry(*iTextureId, iOpenGlCacheEntry);
	CGame::Game->iGraphicsEngine->DrawTextureImmediately(iPolygon, iTextureId->OpenGlId);
	CGame::Game->iGraphicsEngine->SetGraphicsEngineSettings(GL_ALPHA_TEST, true);
}

void CTextureObject::DrawAtPositionWithValuesWithDrawArrays(TPointIntFloat& aPosition,TIntFloat lWidth,TIntFloat lHeight)
{
	SPolygonSquare lTempPolygon, lTempPolygonUnrotated;

  TInt lOldAngle = iCurrentAngel;
  lTempPolygon = iPolygon;
  lTempPolygonUnrotated = iPolygonUnrotated;
  TIntFloat lTempWidth = iWidth;
  TIntFloat lTempHeight = iHeight;

  //set new values
  SetNewLowerLeftCoordinate(aPosition);
  SetHeight(lHeight);
  SetWidth(lWidth);
  //draw with new settings
  DrawArrays();

  //reset settings to old values
  iPolygon = lTempPolygon;
  iPolygonUnrotated = lTempPolygonUnrotated;
  iWidth = lTempWidth;
  iHeight = lTempHeight;
  iCurrentAngel = lOldAngle;
}

const SGraphicsEngineTextureId& CTextureObject::GetTextureRef()
{
  return *iTextureId;
}

TIntFloat CTextureObject::GetHeight()
{
  return iHeight;
}

const SPolygonSquare& CTextureObject::GetPolygon()
{
	return iPolygon;
}

TInt CTextureObject::GetRotationalAngel()
{
  return iCurrentAngel;
}

TIntFloat CTextureObject::GetWidth()
{
  return iWidth;
}

TBool CTextureObject::IsReflectedOverYAxis()
{
  return iReflectedOverYAxis;
}

TBool CTextureObject::IsReflectedOverXAxis()
{
  return iReflectedOverXAxis;
}

void CTextureObject::ReflectOverYAxis()
{
  //To Reflect Over the Y Axis we just have to change the S coordinates to their opposite vertex value
  TIntFloat lTemp;
  lTemp = iPolygon.TopLeftCorner.Texture.s;
  iPolygon.TopLeftCorner.Texture.s = iPolygon.TopRightCorner.Texture.s;
  iPolygon.TopRightCorner.Texture.s = lTemp;
  lTemp = iPolygon.BottomLeftCorner.Texture.s;
  iPolygon.BottomLeftCorner.Texture.s = iPolygon.BottomRightCorner.Texture.s;
  iPolygon.BottomRightCorner.Texture.s =lTemp;
  iReflectedOverYAxis = !iReflectedOverYAxis;
}

void CTextureObject::ReflectOverXAxis()
{
  //To Reflect Over the X Axis we just have to change the T coordinates to their opposite
  TIntFloat lTemp;
  lTemp = iPolygon.TopLeftCorner.Texture.t;
  iPolygon.TopLeftCorner.Texture.t = iPolygon.BottomLeftCorner.Texture.t;
  iPolygon.BottomLeftCorner.Texture.t=lTemp;
  lTemp= iPolygon.TopRightCorner.Texture.t;
  iPolygon.TopRightCorner.Texture.t = iPolygon.BottomRightCorner.Texture.t;
  iPolygon.BottomRightCorner.Texture.t =lTemp;
  iReflectedOverXAxis = !iReflectedOverXAxis;
}

TPointIntFloat CTextureObject::ReturnCurrentFixPoint()
{
  if(iReflectedOverYAxis)
    {
			return ReturnLowerRightCoordinate();
    }
  else
    {
      return ReturnLowerLeftCoordinate();
    }
}

TPointIntFloat CTextureObject::ReturnLowerLeftCoordinate()
{
  TPointIntFloat lResult;
  lResult.iX = iPolygon.BottomLeftCorner.Coordinate.x; lResult.iY = iPolygon.BottomLeftCorner.Coordinate.y;//left bottom corner
  return lResult;
}

TPointIntFloat CTextureObject::ReturnLowerRightCoordinate()
{
  TPointIntFloat lResult;
  lResult.iX = iPolygon.BottomRightCorner.Coordinate.x; lResult.iY = iPolygon.BottomRightCorner.Coordinate.y;
  return lResult;
}

TPointIntFloat CTextureObject::ReturnTopLeftCoordinate()
{
  TPointIntFloat lResult;
  lResult.iX = iPolygon.TopLeftCorner.Coordinate.x; lResult.iY = iPolygon.TopLeftCorner.Coordinate.y;
  return lResult;
}

TPointIntFloat CTextureObject::ReturnTopRightCoordinate()
{
  TPointIntFloat lResult;
  lResult.iX = iPolygon.TopRightCorner.Coordinate.x; lResult.iY= iPolygon.TopRightCorner.Coordinate.y;//left bottom corner
  return lResult;
}

void CTextureObject::RotateTextureObjectToAngleAndAdjustToPoint(const TInt& aAngel,const TBool& aObjectReflected,const TPointIntFloat& aCoordinates)
{
	SetNewLowerLeftCoordinate(aCoordinates);
	if(aObjectReflected)
	{
		if(!IsReflectedOverYAxis())
			ReflectOverYAxis();

		RotateVertexes(aAngel);
		//need to reset the image back to the original position, since after rotating the image, the image will not be at its default position
		TPointIntFloat lTextureLowerCorner = ReturnCurrentFixPoint();
		lTextureLowerCorner.iX = -lTextureLowerCorner.iX + aCoordinates.iX;
		lTextureLowerCorner.iY = -lTextureLowerCorner.iY + aCoordinates.iY;
		ChangeXCoordinate(lTextureLowerCorner.iX);
		ChangeYCoordinate(lTextureLowerCorner.iY);
	}else
	{
		if(IsReflectedOverYAxis())
			ReflectOverYAxis();

		RotateVertexes(-180 + aAngel);//since 180 degrees is the normal orientation of the image
		//need to reset the image back to the original position, since after rotating the image, the image will not be at its default position
		TPointIntFloat lTextureLowerCorner = ReturnCurrentFixPoint();
		lTextureLowerCorner.iX = -lTextureLowerCorner.iX + aCoordinates.iX;
		lTextureLowerCorner.iY = -lTextureLowerCorner.iY + aCoordinates.iY;
		ChangeXCoordinate(lTextureLowerCorner.iX);
		ChangeYCoordinate(lTextureLowerCorner.iY);
	}
}

void CTextureObject::RotateVertexes(TInt aAngle)
{
  //adjust currentAngel
  iCurrentAngel += aAngle;
  while(iCurrentAngel<0) iCurrentAngel+=360;
  while(iCurrentAngel>=360) iCurrentAngel-=360;

  //reset vertex rotation
  iPolygon.TopLeftCorner.Coordinate = iPolygonUnrotated.TopLeftCorner.Coordinate;
  iPolygon.TopRightCorner.Coordinate = iPolygonUnrotated.TopRightCorner.Coordinate;
  iPolygon.BottomLeftCorner.Coordinate = iPolygonUnrotated.BottomLeftCorner.Coordinate;
  iPolygon.BottomRightCorner.Coordinate = iPolygonUnrotated.BottomRightCorner.Coordinate;

  //find the center point of the square
  TIntFloat lCenterPoint [] = {((iPolygon.TopLeftCorner.Coordinate.x + iPolygon.BottomRightCorner.Coordinate.x) / 2),//top left corner and bottom right corner of square
															 ((iPolygon.TopLeftCorner.Coordinate.y + iPolygon.BottomRightCorner.Coordinate.y) / 2)};

  //now, move the square to the origin, by substracting the centerPoint
  iPolygon.TopLeftCorner.Coordinate.x -= lCenterPoint[0]; iPolygon.TopLeftCorner.Coordinate.y -= lCenterPoint[1];
  iPolygon.TopRightCorner.Coordinate.x -= lCenterPoint[0]; iPolygon.TopRightCorner.Coordinate.y -= lCenterPoint[1];
  iPolygon.BottomLeftCorner.Coordinate.x -= lCenterPoint[0]; iPolygon.BottomLeftCorner.Coordinate.y -= lCenterPoint[1];
  iPolygon.BottomRightCorner.Coordinate.x -= lCenterPoint[0]; iPolygon.BottomRightCorner.Coordinate.y -= lCenterPoint[1];

  //rotation matrix
  TIntFloat lRotationMatrix[] = {(CMath::GraphicsCosTable(iCurrentAngel)),(-CMath::GraphicsSinTable(iCurrentAngel)),
																 (CMath::GraphicsSinTable(iCurrentAngel)),(CMath::GraphicsCosTable(iCurrentAngel))};

  //mulitply vertexes positioned at origin by rotation matrix
  TIntFloat lTempX,lTempY;
  lTempX = iPolygon.TopLeftCorner.Coordinate.x; lTempY = iPolygon.TopLeftCorner.Coordinate.y;//to avoid to use the modified data in the second calculation to get Y
  iPolygon.TopLeftCorner.Coordinate.x = (lRotationMatrix[0] * lTempX) + (lRotationMatrix[1] * lTempY);
  iPolygon.TopLeftCorner.Coordinate.y = (lRotationMatrix[2] * lTempX) + (lRotationMatrix[3] * lTempY);

  lTempX = iPolygon.TopRightCorner.Coordinate.x; lTempY = iPolygon.TopRightCorner.Coordinate.y;
  iPolygon.TopRightCorner.Coordinate.x = (lRotationMatrix[0] * lTempX) + (lRotationMatrix[1] * lTempY);
  iPolygon.TopRightCorner.Coordinate.y = (lRotationMatrix[2] * lTempX) + (lRotationMatrix[3] * lTempY);

  lTempX = iPolygon.BottomLeftCorner.Coordinate.x; lTempY = iPolygon.BottomLeftCorner.Coordinate.y;
  iPolygon.BottomLeftCorner.Coordinate.x =(lRotationMatrix[0] * lTempX) + (lRotationMatrix[1] * lTempY);
  iPolygon.BottomLeftCorner.Coordinate.y =(lRotationMatrix[2] * lTempX) + (lRotationMatrix[3] * lTempY);

  lTempX = iPolygon.BottomRightCorner.Coordinate.x; lTempY = iPolygon.BottomRightCorner.Coordinate.y;
  iPolygon.BottomRightCorner.Coordinate.x= (lRotationMatrix[0] * lTempX) + (lRotationMatrix[1] * lTempY);
  iPolygon.BottomRightCorner.Coordinate.y= (lRotationMatrix[2] * lTempX) + (lRotationMatrix[3] * lTempY);

  //move Vertexes back to their original position
  iPolygon.TopLeftCorner.Coordinate.x += lCenterPoint[0]; iPolygon.TopLeftCorner.Coordinate.y += lCenterPoint[1];
  iPolygon.TopRightCorner.Coordinate.x += lCenterPoint[0]; iPolygon.TopRightCorner.Coordinate.y += lCenterPoint[1];
  iPolygon.BottomLeftCorner.Coordinate.x += lCenterPoint[0]; iPolygon.BottomLeftCorner.Coordinate.y += lCenterPoint[1];
  iPolygon.BottomRightCorner.Coordinate.x += lCenterPoint[0]; iPolygon.BottomRightCorner.Coordinate.y += lCenterPoint[1];
}

void CTextureObject::SetAlpha(TUint8 aAlpha)
{
	iPolygon.TopLeftCorner.Colour.Alpha = aAlpha;
	iPolygon.TopRightCorner.Colour.Alpha = aAlpha;
	iPolygon.BottomLeftCorner.Colour.Alpha = aAlpha;
	iPolygon.BottomRightCorner.Colour.Alpha = aAlpha;
}

void CTextureObject::SetColourAndAlpha(TUint8 aRed, TUint8 aGreen, TUint8 aBlue, TUint8 aAlpha)
{
	iPolygon.TopLeftCorner.Colour.Red = aRed;
	iPolygon.TopLeftCorner.Colour.Green = aGreen;
	iPolygon.TopLeftCorner.Colour.Blue = aBlue;
	iPolygon.TopLeftCorner.Colour.Alpha = aAlpha;

	//use all the same colour components
	iPolygon.TopRightCorner.Colour = iPolygon.TopLeftCorner.Colour;
	iPolygon.BottomLeftCorner.Colour = iPolygon.TopLeftCorner.Colour;
	iPolygon.BottomRightCorner.Colour = iPolygon.TopLeftCorner.Colour;
}

void CTextureObject::SetNewLowerLeftCoordinate(TPointIntFloat aLocation)
{
	iPolygon.TopLeftCorner.Coordinate.x = aLocation.iX;        iPolygon.TopLeftCorner.Coordinate.y= aLocation.iY + iHeight; //left Top corner
	iPolygon.TopRightCorner.Coordinate.x = aLocation.iX+iWidth; iPolygon.TopRightCorner.Coordinate.y = aLocation.iY +iHeight; //right top corner
	iPolygon.BottomLeftCorner.Coordinate.x = aLocation.iX;        iPolygon.BottomLeftCorner.Coordinate.y = aLocation.iY;//left bottom corner
	iPolygon.BottomRightCorner.Coordinate.x=aLocation.iX+iWidth; iPolygon.BottomRightCorner.Coordinate.y = aLocation.iY;//right bottom corner

  //update original vertexes as well
	iPolygonUnrotated.TopLeftCorner.Coordinate.x = aLocation.iX;        iPolygonUnrotated.TopLeftCorner.Coordinate.y= aLocation.iY + iHeight; //left Top corner
	iPolygonUnrotated.TopRightCorner.Coordinate.x = aLocation.iX+iWidth; iPolygonUnrotated.TopRightCorner.Coordinate.y = aLocation.iY +iHeight; //right top corner
	iPolygonUnrotated.BottomLeftCorner.Coordinate.x = aLocation.iX;        iPolygonUnrotated.BottomLeftCorner.Coordinate.y = aLocation.iY;//left bottom corner
	iPolygonUnrotated.BottomRightCorner.Coordinate.x=aLocation.iX+iWidth; iPolygonUnrotated.BottomRightCorner.Coordinate.y = aLocation.iY;//right bottom corner

	iCurrentAngel = 0;
}

void CTextureObject::SetNewSandTValues(TFloat aSLowerValue,TFloat aSUpperValue,TFloat aTLowerValue,TFloat aTUpperValue)
{
  SetNewSValue(aSLowerValue,aSUpperValue);//takes TFloat as argument
  SetNewTValue(aTLowerValue,aTUpperValue);
}

void CTextureObject::SetNewSValue(TFloat aSLowerValue,TFloat aSUpperValue)
{
	SetNewSValueIntFloat(TIntFloat::Convert(aSLowerValue), TIntFloat::Convert(aSUpperValue));
}

void CTextureObject::SetNewSValueIntFloat(TIntFloat aSLowerValue,TIntFloat aSUpperValue)
{
	//update real S value
  iLowerSValue = aSLowerValue;
  iUpperSValue = aSUpperValue;

	if (iTextureId != NULL)
	{
		if (iReflectedOverYAxis)
		{
			//check if the s or t value is not negative, if negative give it the negative new value, to avoid reflection over the X and Y axis
			//in this way we can also have normal behaviour with negative texture values
			iPolygon.TopLeftCorner.Texture.s = aSUpperValue * iTextureId->MaxS;
			iPolygon.TopRightCorner.Texture.s = aSLowerValue * iTextureId->MaxS;
			iPolygon.BottomLeftCorner.Texture.s = aSUpperValue * iTextureId->MaxS;
			iPolygon.BottomRightCorner.Texture.s = aSLowerValue * iTextureId->MaxS;
		}
		else
		{
			//check if the s or t value is not negative, if negative give it the negative new value, to avoid reflection over the X and Y axis
			iPolygon.TopLeftCorner.Texture.s = aSLowerValue * iTextureId->MaxS;
			iPolygon.TopRightCorner.Texture.s = aSUpperValue * iTextureId->MaxS;
			iPolygon.BottomLeftCorner.Texture.s = aSLowerValue * iTextureId->MaxS;
			iPolygon.BottomRightCorner.Texture.s = aSUpperValue * iTextureId->MaxS;
		}
	}
}

void CTextureObject::SetNewTValue(TFloat aTLowerValue,TFloat aTUpperValue)
{
	SetNewTValueIntFloat(TIntFloat::Convert(aTLowerValue), TIntFloat::Convert(aTUpperValue));
}

void CTextureObject::SetNewTValueIntFloat(TIntFloat aTLowerValue, TIntFloat aTUpperValue)
{
	//update real T value
  iLowerTValue = aTLowerValue;
  iUpperTValue = aTUpperValue;

	if (iTextureId != NULL)
	{
		if (iReflectedOverXAxis)
		{
			//check if the s or t value is not negative, if negative give it the negative new value, to avoid reflection over the X and Y axis
			iPolygon.TopLeftCorner.Texture.t = aTUpperValue * iTextureId->MaxT;
			iPolygon.TopRightCorner.Texture.t = aTUpperValue * iTextureId->MaxT;
			iPolygon.BottomLeftCorner.Texture.t = aTLowerValue * iTextureId->MaxT;
			iPolygon.BottomRightCorner.Texture.t = aTLowerValue * iTextureId->MaxT;
		}
		else
		{
			//check if the s or t value is not negative, if negative give it the negative new value, to avoid reflection over the X and Y axis
			iPolygon.TopLeftCorner.Texture.t = aTLowerValue * iTextureId->MaxT;
			iPolygon.TopRightCorner.Texture.t = aTLowerValue * iTextureId->MaxT;
			iPolygon.BottomLeftCorner.Texture.t = aTUpperValue * iTextureId->MaxT;
			iPolygon.BottomRightCorner.Texture.t = aTUpperValue * iTextureId->MaxT;
		}
	}
}

void CTextureObject::SetHeight(const TIntFloat& aHeight)
{
  iHeight = aHeight;
  iPolygon.TopLeftCorner.Coordinate.y= iPolygon.BottomLeftCorner.Coordinate.y + iHeight; //left Top corner
  iPolygon.TopRightCorner.Coordinate.y= iPolygon.BottomLeftCorner.Coordinate.y + iHeight; //right top corner

  //update unrotated vertexes
  iPolygonUnrotated.TopLeftCorner.Coordinate.y = iPolygonUnrotated.BottomLeftCorner.Coordinate.y + iHeight; //left Top corner
  iPolygonUnrotated.TopRightCorner.Coordinate.y = iPolygonUnrotated.BottomLeftCorner.Coordinate.y + iHeight; //right top corner

  RotateVertexes(0); //rotation has been lost, redue it if any
}

void CTextureObject::SetWidth(const TIntFloat& aWidth)
{
  iWidth=aWidth;
  iPolygon.TopRightCorner.Coordinate.x =  iPolygon.BottomLeftCorner.Coordinate.x + iWidth;
  iPolygon.BottomRightCorner.Coordinate.x =  iPolygon.BottomLeftCorner.Coordinate.x + iWidth;

  //update unrotated vertexes
  iPolygonUnrotated.TopRightCorner.Coordinate.x = iPolygonUnrotated.BottomLeftCorner.Coordinate.x + iWidth;
  iPolygonUnrotated.BottomRightCorner.Coordinate.x = iPolygonUnrotated.BottomLeftCorner.Coordinate.x + iWidth;

  RotateVertexes(0); //rotation has been lost, redue it if any
}

void CTextureObject::SaveOnDisk(CFileWriteStream& aOutputStream)
{
  aOutputStream.WriteIntFloat(iWidth);
  aOutputStream.WriteIntFloat(iHeight);
  aOutputStream.WriteInt32(iCurrentAngel);
  aOutputStream.WriteInt32(iReflectedOverYAxis);
  aOutputStream.WriteInt32(iReflectedOverXAxis);
  aOutputStream.WriteIntFloat(iLowerSValue);
  aOutputStream.WriteIntFloat(iUpperSValue);
  aOutputStream.WriteIntFloat(iLowerTValue);
  aOutputStream.WriteIntFloat(iUpperTValue);
  //location vertexes
  aOutputStream.WriteIntFloat(iPolygon.TopLeftCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygon.TopLeftCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygon.TopLeftCorner.Coordinate.z);
  aOutputStream.WriteIntFloat(iPolygon.TopRightCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygon.TopRightCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygon.TopRightCorner.Coordinate.z);
  aOutputStream.WriteIntFloat(iPolygon.BottomLeftCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygon.BottomLeftCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygon.BottomLeftCorner.Coordinate.z);
  aOutputStream.WriteIntFloat(iPolygon.BottomRightCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygon.BottomRightCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygon.BottomRightCorner.Coordinate.z);
  //unrotated location vertexes
  //location vertexes
  aOutputStream.WriteIntFloat(iPolygonUnrotated.TopLeftCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygonUnrotated.TopLeftCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygonUnrotated.TopLeftCorner.Coordinate.z);
  aOutputStream.WriteIntFloat(iPolygonUnrotated.TopRightCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygonUnrotated.TopRightCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygonUnrotated.TopRightCorner.Coordinate.z);
  aOutputStream.WriteIntFloat(iPolygonUnrotated.BottomLeftCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygonUnrotated.BottomLeftCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygonUnrotated.BottomLeftCorner.Coordinate.z);
  aOutputStream.WriteIntFloat(iPolygonUnrotated.BottomRightCorner.Coordinate.x);aOutputStream.WriteIntFloat(iPolygonUnrotated.BottomRightCorner.Coordinate.y);aOutputStream.WriteInt32(iPolygonUnrotated.BottomRightCorner.Coordinate.z);
  //texture vertexes
  aOutputStream.WriteIntFloat(iPolygon.TopLeftCorner.Texture.s);aOutputStream.WriteIntFloat(iPolygon.TopLeftCorner.Texture.t);
  aOutputStream.WriteIntFloat(iPolygon.TopRightCorner.Texture.s);aOutputStream.WriteIntFloat(iPolygon.TopRightCorner.Texture.t);
  aOutputStream.WriteIntFloat(iPolygon.BottomLeftCorner.Texture.s);aOutputStream.WriteIntFloat(iPolygon.BottomLeftCorner.Texture.t);
  aOutputStream.WriteIntFloat(iPolygon.BottomRightCorner.Texture.s);aOutputStream.WriteIntFloat(iPolygon.BottomRightCorner.Texture.t);
  //colour vertexes
  aOutputStream.WriteInt32(iPolygon.TopLeftCorner.Colour.Red);aOutputStream.WriteInt32(iPolygon.TopLeftCorner.Colour.Green);aOutputStream.WriteInt32(iPolygon.TopLeftCorner.Colour.Blue);aOutputStream.WriteInt32(iPolygon.TopLeftCorner.Colour.Alpha);
  aOutputStream.WriteInt32(iPolygon.TopRightCorner.Colour.Red);aOutputStream.WriteInt32(iPolygon.TopRightCorner.Colour.Green);aOutputStream.WriteInt32(iPolygon.TopRightCorner.Colour.Blue);aOutputStream.WriteInt32(iPolygon.TopRightCorner.Colour.Alpha);
  aOutputStream.WriteInt32(iPolygon.BottomLeftCorner.Colour.Red);aOutputStream.WriteInt32(iPolygon.BottomLeftCorner.Colour.Green);aOutputStream.WriteInt32(iPolygon.BottomLeftCorner.Colour.Blue);aOutputStream.WriteInt32(iPolygon.BottomLeftCorner.Colour.Alpha);
  aOutputStream.WriteInt32(iPolygon.BottomRightCorner.Colour.Red);aOutputStream.WriteInt32(iPolygon.BottomRightCorner.Colour.Green);aOutputStream.WriteInt32(iPolygon.BottomRightCorner.Colour.Blue);aOutputStream.WriteInt32(iPolygon.BottomRightCorner.Colour.Alpha);

  //save the TextureIdentifier
  if(iTextureId != NULL)
  {
  	aOutputStream.WriteInt32(1); //signal that we had a vaild TextureId
  	aOutputStream.WriteString(CGame::Game->iGraphicsEngine->GetImageIdentifier(*iTextureId)->GetData());
  }
  else
  {
  	aOutputStream.WriteInt32(0); //signal that we don't have a textureId
  }
}

void CTextureObject::LoadFromDisk(CFileReadStream& aReadStream)
{
  iWidth = aReadStream.ReadIntFloat();
  iHeight = aReadStream.ReadIntFloat();
  iCurrentAngel = aReadStream.ReadInt32();
  iReflectedOverYAxis = aReadStream.ReadInt32();
  iReflectedOverXAxis = aReadStream.ReadInt32();
	iLowerSValue = aReadStream.ReadIntFloat();
	iUpperSValue = aReadStream.ReadIntFloat();
	iLowerTValue = aReadStream.ReadIntFloat();
	iUpperTValue = aReadStream.ReadIntFloat();
  //location vertexes
	iPolygon.TopLeftCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygon.TopLeftCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygon.TopLeftCorner.Coordinate.z=aReadStream.ReadInt32();
	iPolygon.TopRightCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygon.TopRightCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygon.TopRightCorner.Coordinate.z=aReadStream.ReadInt32();
	iPolygon.BottomLeftCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygon.BottomLeftCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygon.BottomLeftCorner.Coordinate.z=aReadStream.ReadInt32();
	iPolygon.BottomRightCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygon.BottomRightCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygon.BottomRightCorner.Coordinate.z=aReadStream.ReadInt32();
  //unrotated location vertexes
	iPolygonUnrotated.TopLeftCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygonUnrotated.TopLeftCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygonUnrotated.TopLeftCorner.Coordinate.z=aReadStream.ReadInt32();
	iPolygonUnrotated.TopRightCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygonUnrotated.TopRightCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygonUnrotated.TopRightCorner.Coordinate.z=aReadStream.ReadInt32();
	iPolygonUnrotated.BottomLeftCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygonUnrotated.BottomLeftCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygonUnrotated.BottomLeftCorner.Coordinate.z=aReadStream.ReadInt32();
	iPolygonUnrotated.BottomRightCorner.Coordinate.x=aReadStream.ReadIntFloat();iPolygonUnrotated.BottomRightCorner.Coordinate.y=aReadStream.ReadIntFloat();iPolygonUnrotated.BottomRightCorner.Coordinate.z=aReadStream.ReadInt32();
  //texture vertexes
	iPolygon.TopLeftCorner.Texture.s=aReadStream.ReadIntFloat();iPolygon.TopLeftCorner.Texture.t=aReadStream.ReadIntFloat();
	iPolygon.TopRightCorner.Texture.s=aReadStream.ReadIntFloat();iPolygon.TopRightCorner.Texture.t=aReadStream.ReadIntFloat();
	iPolygon.BottomLeftCorner.Texture.s=aReadStream.ReadIntFloat();iPolygon.BottomLeftCorner.Texture.t=aReadStream.ReadIntFloat();
	iPolygon.BottomRightCorner.Texture.s=aReadStream.ReadIntFloat();iPolygon.BottomRightCorner.Texture.t=aReadStream.ReadIntFloat();
  //colour vertexes
  iPolygon.TopLeftCorner.Colour.Red = aReadStream.ReadInt32();iPolygon.TopLeftCorner.Colour.Green = aReadStream.ReadInt32();iPolygon.TopLeftCorner.Colour.Blue = aReadStream.ReadInt32();iPolygon.TopLeftCorner.Colour.Alpha = aReadStream.ReadInt32();
  iPolygon.TopRightCorner.Colour.Red = aReadStream.ReadInt32();iPolygon.TopRightCorner.Colour.Green = aReadStream.ReadInt32();iPolygon.TopRightCorner.Colour.Blue = aReadStream.ReadInt32();iPolygon.TopRightCorner.Colour.Alpha = aReadStream.ReadInt32();
  iPolygon.BottomLeftCorner.Colour.Red = aReadStream.ReadInt32();iPolygon.BottomLeftCorner.Colour.Green = aReadStream.ReadInt32();iPolygon.BottomLeftCorner.Colour.Blue = aReadStream.ReadInt32();iPolygon.BottomLeftCorner.Colour.Alpha = aReadStream.ReadInt32();
  iPolygon.BottomRightCorner.Colour.Red = aReadStream.ReadInt32();iPolygon.BottomRightCorner.Colour.Green = aReadStream.ReadInt32();iPolygon.BottomRightCorner.Colour.Blue = aReadStream.ReadInt32();iPolygon.BottomRightCorner.Colour.Alpha = aReadStream.ReadInt32();

  //get textureId
  TInt lHaveTextureId = aReadStream.ReadInt32();
  if(lHaveTextureId == 1)
  {
		CString* lFileLocation = new CString();
		aReadStream.ReadString(lFileLocation);
		AssignTexture(CGame::Game->iGraphicsEngine->GetTextureId(lFileLocation->GetData()));
		delete lFileLocation;
  }
  else
  {
  	AssignTexture(NULL);
  }
}
