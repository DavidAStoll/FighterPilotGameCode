/*
 * CTextureObject.h
 *
 *  Created on: Jun 11, 2012
 *      Author: dstoll
 */

#ifndef CTEXTUREOBJECT_H_
#define CTEXTUREOBJECT_H_

#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

class CTextureObject
{
public:

	static CTextureObject* New(TPoint aLowerLeftCorner, SGraphicsEngineTextureId* aTextureId, TInt aWidth, TInt aHeight, TInt aZCor, TFloat aTextureSLowerValue, TFloat aTextureSUpperValue, TFloat aTextureTLowerValue, TFloat aTextureTUpperValue);
	static CTextureObject* New(TPointIntFloat aLowerLeftCorner, SGraphicsEngineTextureId* aTextureId, TIntFloat aWidth, TIntFloat aHeight, TInt aZCor, TFloat aTextureSLowerValue, TFloat aTextureSUpperValue, TFloat aTextureTLowerValue, TFloat aTextureTUpperValue);
	~CTextureObject();

	//assigns a new texture Handle
	void AssignTexture(SGraphicsEngineTextureId* aTextureId);
	//changes the Height of the texture Object, also changes its location
	void ChangeHeight(const TIntFloat& aChange);
	//changes the Width of the texture Object, also changes its location
	void ChangeWidth(const TIntFloat& aChange);
	//moves the all vertexes by the given Change on the X axis, internal format
	void ChangeXCoordinate(TIntFloat aChange);
	//moves the all vertexes by the given change on the Y axis, internal format
	void ChangeYCoordinate(TIntFloat aChange);
	//moves the the overall S texturing, by moving the upper and lower bound
	//NOTE!! Only works well if the width is power of 2
	void ChangeSCoordinate(TFloat aChange);
	//moves the the overall S texturing, by moving the upper and lower bound
	//NOTE!! Only works well if the height is power of 2
	void ChangeTCoordinate(TFloat aChange);
	//draws the Texture Object with Triangle_STRIP and the associated Texturem without using indicies
	void DrawArrays();
	void DrawArraysWithoutAlpha();
	//Draws at the specified location with a given width and height
	void DrawAtPositionWithValuesWithDrawArrays(TPointIntFloat& aPosition, TIntFloat lWidth, TIntFloat lHeight);
	//returns the current Texture as a Ref
	const SGraphicsEngineTextureId& GetTextureRef();
	//returns Height of TextureObject
	TIntFloat GetHeight();
	//returns ref to rotated polygon object of the texture object
	const SPolygonSquare& GetPolygon();
	//Returns by how much Degree the TextureObject has been rotated from the origin
	TInt GetRotationalAngel();
	//returns Width of TextureObject
	TIntFloat GetWidth();
	//if the Texture Object has been reflected over the Y axis
	TBool IsReflectedOverYAxis();
	//if the texture has been reflected over the X axis
	TBool IsReflectedOverXAxis();
	//changes the S and T coordinates such that the Texture looks like it has been reflected over the Y axis
	void ReflectOverYAxis();
	//changes the S and T coordinates such that the Texture looks like it has been reflected over the X axis
	void ReflectOverXAxis();
	//return current fix point, the point that is seen as the lower corner of the object
	TPointIntFloat ReturnCurrentFixPoint();
	//return lower left corner, in internal format
	TPointIntFloat ReturnLowerLeftCoordinate();
	//return lower right corner, in internal format
	TPointIntFloat ReturnLowerRightCoordinate();
	//return top left corner, in internal format
	TPointIntFloat ReturnTopLeftCoordinate();
	//return top right corner, in internal format
	TPointIntFloat ReturnTopRightCoordinate();
	//rotates the TextureObject by a certain angle and fixes it back to a certain point in space
	void RotateTextureObjectToAngleAndAdjustToPoint(const TInt& aAngel,const TBool& aObjectReflected,const TPointIntFloat& aCoordinates);
	//rotatests the vertexes by the given angel
	void RotateVertexes(TInt aAngle);
	//just changes the Alpha value of the TextureObject, between 0 - 255
	void SetAlpha(TUint8 aAlpha);
	//sets the Colour and Alpha for the TextureObject, between 0 - 255
	void SetColourAndAlpha(TUint8 aRed, TUint8 aGreen, TUint8 aBlue, TUint8 aAlpha);
	//sets new Lower Left Corner and the associated other corner relative
	void SetNewLowerLeftCoordinate(TPointIntFloat aLocation);
	//Sets new T and S values, used to cut out a different region from the Texture
	void SetNewSandTValues(TFloat aSLowerValue, TFloat aSUpperValue, TFloat aTLowerValue, TFloat aTUpperValue);
	//changes the cutting region only on the X plane
	void SetNewSValue(TFloat aSLowerValue, TFloat aSValue);
	void SetNewSValueIntFloat(TIntFloat aSLowerValue, TIntFloat aSValue);
	//changes the cutting region only on the Y plane
	void SetNewTValue(TFloat aTLowerValue, TFloat aTValue);
	void SetNewTValueIntFloat(TIntFloat aTLowerValue, TIntFloat aTValue);
	//sets a new Height, preserves rotation and location
	void SetHeight(const TIntFloat& aHeight);
	//sets a new Width, preserves rotation and location
	void SetWidth(const TIntFloat& aWidth);
	//saves the entire textureObject on disk, textureID tells us what current Texture has been assigned
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	//loads this TextureObject with the data saved on disk
	void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	CTextureObject(TPoint aLowerLeftCorner, SGraphicsEngineTextureId* aTextureId, TInt aWidth, TInt aHeight, TInt aZCor, TFloat aTextureSLowerValue, TFloat aTextureSUpperValue, TFloat aTextureTLowerValue, TFloat aTextureTUpperValue);
	CTextureObject(TPointIntFloat aLowerLeftCorner, SGraphicsEngineTextureId* aTextureId, TIntFloat aWidth, TIntFloat aHeight, TInt aZCor, TFloat aTextureSLowerValue, TFloat aTextureSUpperValue, TFloat aTextureTLowerValue, TFloat aTextureTUpperValue);

	SPolygonSquare iPolygon;
	SPolygonSquare iPolygonUnrotated;//these vertexes are not rotated and don't suffer from any round off error due to rotations. Their only purpose is to be used in the rotation calculation to avoid round offs
	SGraphicsEngineTextureId* iTextureId;// the texture associated with this object
	TBool iReflectedOverYAxis;//used to know how S and T values must be mapped to the 4 vertexes
	TBool iReflectedOverXAxis;
	TIntFloat iWidth;
	TIntFloat iHeight;
	TIntFloat iLowerSValue;
	TIntFloat iUpperSValue;
	TIntFloat iLowerTValue;
	TIntFloat iUpperTValue;
	TInt iCurrentAngel;//to keep track what the current rotation of the texture Object is relative to the origin
	SOpenGLCacheEntry* iOpenGlCacheEntry;
};

#endif /* CTEXTUREOBJECT_H_ */
