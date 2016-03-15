/*
 * COverSizedImage.h
 *
 *  Created on: Aug 17, 2012
 *      Author: dstoll
 */

#ifndef COVERSIZEDIMAGE_H_
#define COVERSIZEDIMAGE_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CTextureObject.h"

class COverSizedImage
{
public:

	static COverSizedImage* New(const TSize& aCutOutSize, SBitmap& aBitmapData);
	virtual ~COverSizedImage();
	void ChangeXCutOutLocation(TInt aChange);

	CTextureObject* iTextureObject;

protected:
	COverSizedImage(const TSize& aCutOutSize, SBitmap& aBitmapData);
	void Construct();

	//uses the current X and Y cutout location to extract the new subimage, it replaces its current one in the Graphics Engine
	void ReplaceSubImage();

	SBitmap& iOriginalBitmap;

	TInt iXSubImageLocation; //left most location in the bitmap from which will we start cutting
	TInt iMaxXSubImageLocation;

	TSize iSubImageSize;
	SGraphicsEngineTextureId iGraphicsTexture;

};

#endif /* COVERSIZEDIMAGE_H_ */
