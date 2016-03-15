/*
 * CDrawObject.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CDRAWOBJECT_H_
#define CDRAWOBJECT_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CTextureObject.h"
#include "includes/core/graphic/CAnimationPlayer.h"
#include "includes/core/utility/CImageStore.h"
#include "includes/core/utility/CFileStream.h"

class CDrawObject
{

public:

	virtual ~CDrawObject();

	void ChangeZValue(TInt aChange);
	virtual void Draw();
	virtual void DrawAlways();
	//returns the Alpha of DrawObject that the texture object is currently set to, range 0 - 255
	virtual TUint GetAlpha();
	CTextureObject* GetTextureObject();
	void SetDoNotDraw(TBool aValue);
	virtual void SetAlpha(TInt aAlphaValue); //should be between 0 - 100, where value defines opacity of drawing. Needs to be overriden if object has more than one textureObject
	virtual void DrawAtPositionWithValues(TPointIntFloat& aPosition,TIntFloat lWidth,TIntFloat lHeight);
	void DrawObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void DrawObjectLoadContentFromDisk(CFileReadStream &aReadStream);
	//public data members needed by the draw function
	TInt iZCor;//0 is drawn first, then 1... the bigger Z the later it is draw. Therefore, more in the foregrounds

protected:

	CTextureObject* iTextureObject;//has function to draw the associated Texture
	CAnimationPlayer* iAnimationPlayer;//used to play animations, must be assigned an value in the deriving class if it should be used
	TBool iDoNotDraw;

	CDrawObject(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer);
	static TInt AdjustZCoordinate(TInt aZValue);

	static 	TInt CurrentZValueOffset;
};

#endif /* CDRAWOBJECT_H_ */
