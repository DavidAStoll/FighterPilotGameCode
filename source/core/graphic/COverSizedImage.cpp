/*
 * COverSizedImage.cpp
 *
 *  Created on: Aug 17, 2012
 *      Author: dstoll
 */

#include "includes/core/graphic/COverSizedImage.h"
#include "includes/core/CGame.h"

COverSizedImage::COverSizedImage(const TSize& aCutOutSize, SBitmap& aBitmapData)
:iOriginalBitmap(aBitmapData)
{
	iSubImageSize = aCutOutSize;
	iXSubImageLocation = 0;
	iTextureObject = NULL;
	iGraphicsTexture.GraphicsEngineId = 0;
	iGraphicsTexture.OpenGlId = 0;
}

COverSizedImage::~COverSizedImage()
{
	delete iTextureObject;
	CGame::Game->iGraphicsEngine->DeleteGraphicsTextureId(iGraphicsTexture);
}

COverSizedImage* COverSizedImage::New(const TSize& aCutOutSize, SBitmap& aBitmapData)
{
	COverSizedImage* lSelf = new COverSizedImage(aCutOutSize, aBitmapData);
	lSelf->Construct();
	return lSelf;
}

void COverSizedImage::Construct()
{
	//make sure original image is not too small
	if(iOriginalBitmap.WidthInPixels < iSubImageSize.iWidth)
	{
		iSubImageSize.iWidth = iOriginalBitmap.WidthInPixels;
	}
	if(iOriginalBitmap.HeightInPixels < iSubImageSize.iHeight)
	{
		iSubImageSize.iHeight = iOriginalBitmap.HeightInPixels;
	}

	//find out how much the SubImage can be moved into the X direction
	iMaxXSubImageLocation = iOriginalBitmap.WidthInPixels - iSubImageSize.iWidth;
	if(iMaxXSubImageLocation < 0)
	{
		iMaxXSubImageLocation = 0;
	}

	ReplaceSubImage();
}

void COverSizedImage::ChangeXCutOutLocation(TInt aChange)
{
	iXSubImageLocation = aChange;

	if(iXSubImageLocation < 0)
	{
		iXSubImageLocation = 0;
	}
	else if( iXSubImageLocation > iMaxXSubImageLocation)
	{
		iXSubImageLocation = iMaxXSubImageLocation;
	}

	//call method to copy data of new SubImage
	ReplaceSubImage();
}

void COverSizedImage::ReplaceSubImage()
{
	//clean up
	if(iTextureObject != NULL)
	{
		delete iTextureObject;
		iTextureObject = NULL;
	}
	if(iGraphicsTexture.GraphicsEngineId != 0)
	{
		CGame::Game->iGraphicsEngine->DeleteGraphicsTextureId(iGraphicsTexture);
	}

	//need a new Graphics Engine Id
	iGraphicsTexture = CGame::Game->iGraphicsEngine->CreateGraphicsTextureId();

	//copy data into temp Bitmap and upload image into the Graphics Engine
	SBitmap lSubImage;
	lSubImage.Data = new TUint8[iSubImageSize.iWidth * iSubImageSize.iHeight * 4];
	lSubImage.WidthInPixels = iSubImageSize.iWidth;
	lSubImage.HeightInPixels = iSubImageSize.iHeight;

	for(TInt lYIndex = 0; lYIndex < iSubImageSize.iHeight; lYIndex++)
	{
		memcpy(lSubImage.Data + iSubImageSize.iWidth * lYIndex * 4, iOriginalBitmap.Data + iXSubImageLocation * 4 + iOriginalBitmap.WidthInPixels * lYIndex * 4, iSubImageSize.iWidth * 4);
	}

	CGame::Game->iGraphicsEngine->ConvertBitmapImageTo2DTextureWithoutMasking(iGraphicsTexture, lSubImage, true, "NO Location");
	iTextureObject = CTextureObject::New(TPoint(0,0), &iGraphicsTexture, iSubImageSize.iWidth, iSubImageSize.iHeight, 100, 0, 1, 0, 1);
}

