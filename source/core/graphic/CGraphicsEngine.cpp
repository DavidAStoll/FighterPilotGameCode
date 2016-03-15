/*
 * CGraphicsEngine.cpp
 *
 *  Created on: Jun 5, 2012
 *      Author: dstoll
 */


#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/utility/CImageStore.h"
#include "includes/core/CGame.h"

TUint64 CGraphicsEngine::GraphicEngineTextureCounter = 0;

CGraphicsEngine::CGraphicsEngine(TGraphicsEngineDisplayDepth aDisplayDepth, TGraphicsEngineTextureConfiguration aTextureConfig, TUint aVirtualScreenWidth, TUint aVirtualScreenHeight)
{
	iDisplayDepth = aDisplayDepth;
	iTextureDepth = aTextureConfig;
	iVirualScreenWidth = aVirtualScreenWidth;
	iVirualScreenHeight = aVirtualScreenHeight;
	iOpenGlCacheArray = NULL;
	iNumberOfTextureCallsBatched = 0;
	iBoundTextureId = 0; //hope 0 is not the current bound texture :)

  //needed to keep track of what images are on GPU memory
	iOpenGlCacheArray = CPointerArray<SOpenGLCacheEntry>::New();
}

CGraphicsEngine::~CGraphicsEngine()
{
	DeleteAllGraphicsTextureIds();

	if(iOpenGlCacheArray != NULL)
	{
		iOpenGlCacheArray->ClearAndDestroy();
		delete iOpenGlCacheArray;
		iOpenGlCacheArray = NULL;
	}

	DestroyOpenGlContext();
}

CGraphicsEngine* CGraphicsEngine::CreateGraphicsEngineToWindow(TGraphicsEngineDisplayDepth aDisplayDepth, TGraphicsEngineTextureConfiguration aTextureConfig, TUint aVirtualScreenWidth, TUint aVirtualScreenHeight)
{
	CGraphicsEngine* lGraphicsEngine = new CGraphicsEngine(aDisplayDepth, aTextureConfig, aVirtualScreenWidth, aVirtualScreenHeight);
	lGraphicsEngine->SetUpWindowDrawingSurface();
	return lGraphicsEngine;
}

void CGraphicsEngine::SetUpWindowDrawingSurface()
{
	EGLint lAttribList[] = //attributes for graphics configurations
				{EGL_BUFFER_SIZE, iDisplayDepth,
				EGL_DEPTH_SIZE, CGRAPHICS_Z_BUFFER_SIZE,
				EGL_NONE};

		EGLint lSurfaceWidth, lSurfaceHeight, lFormat;
		EGLint lNumConfigs;//number of configuratuins returned, can be ignored
		EGLConfig lConfig;
		EGLSurface lSurface;
		EGLContext lContext;
		//need native Window
		NativeWindow lNativeWindow = GetNativeWindow();

		EGLDisplay lDeviceDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		eglInitialize(lDeviceDisplay, 0, 0);
		eglChooseConfig(lDeviceDisplay, lAttribList, &lConfig, 1, &lNumConfigs);//depending on attribute list, sets a framer buffer configuration in EGLConfig type
		lContext = eglCreateContext(lDeviceDisplay, lConfig, NULL, NULL);// creates a context that keeps track of the state information for drawing pipeline, might have several ones
		lSurface = eglCreateWindowSurface(lDeviceDisplay, lConfig, lNativeWindow, NULL);
		eglMakeCurrent(lDeviceDisplay, lSurface, lSurface, lContext); //this function declares on what surface drawing will take place and what context to us for drawing

    eglQuerySurface(lDeviceDisplay, lSurface, EGL_WIDTH, &lSurfaceWidth);
    eglQuerySurface(lDeviceDisplay, lSurface, EGL_HEIGHT, &lSurfaceHeight);

    //keep track of important data
    iDeviceDisplay = lDeviceDisplay;
    iDrawingSurface = lSurface;
    iGraphicsContext = lContext;
    iGraphicsConfiguration = lConfig;
    iActualScreenWidth = lSurfaceWidth;
    iActualScreenHeight = lSurfaceHeight;

    //setup the viewport and project matrix
    AdjustDrawingSurfaceSize(iVirualScreenWidth, iVirualScreenHeight, 0, 0);

    //clear colour
    glClearColor(GRAPHICS_CLEAR_COLOUR_RED, GRAPHICS_CLEAR_COLOUR_GREEN, GRAPHICS_CLEAR_COLOUR_BLUE, GRAPHICS_CLEAR_COLOUR_ALPHA);

    glActiveTexture(GL_TEXTURE0);
  	//disable states, turned to true by OpenGL by default
    glDisable(GL_DEPTH_TEST); //don't need any Z buffering
  	//enable stuff we need
    glEnable(GL_DITHER);
    glEnable(GL_MULTISAMPLE); //to use mutiple fragments to compute final pixel colour
  	glEnable(GL_TEXTURE_2D);
  	glEnableClientState(GL_VERTEX_ARRAY);
  	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  	glEnableClientState(GL_COLOR_ARRAY);
  	glShadeModel(GL_FLAT);
  	glEnable(GL_ALPHA_TEST);
  	glEnable(GL_BLEND);
  	glAlphaFuncx(GL_GEQUAL, 1);
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	//give some hints for rendering to OpenGL
  	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  	glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);
  	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
  	glHint(GL_FOG_HINT, GL_FASTEST);
  	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
  	//set up TextureEnvironment
  	//blend colour together using the texture colour and colour array of the Polygon
  	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
  	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
  	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
  	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
  	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
  	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_ALPHA,  GL_PRIMARY_COLOR);

  	//need to set up the indicies array for texture batching, since indicies used will always be the same
  	TInt lVertexUsedIndex = 0;

  	for(TInt lIndex = 0; lIndex < GRAPHICS_MAX_NUMBER_OF_BATCHED_TEXTURE_CALLS * 6; lIndex = lIndex + 6) //uses 6 indicies per texture
  	{
  		iTextureIndicies [lIndex] = lVertexUsedIndex + 1;
  		iTextureIndicies [lIndex + 1] = lVertexUsedIndex;
  		iTextureIndicies [lIndex + 2] = lVertexUsedIndex + 2;
  		iTextureIndicies [lIndex + 3] = lVertexUsedIndex + 2;
  		iTextureIndicies [lIndex + 4] = lVertexUsedIndex + 3;
  		iTextureIndicies [lIndex + 5] = lVertexUsedIndex + 1;

  		lVertexUsedIndex += 4;
  	}
}

//---------------------------------- functions ---------------------------------------------//

void CGraphicsEngine::AdjustDrawingSurfaceSize(TInt aWidthInPixels, TInt aHeightInPixels, TInt aXOffset, TInt aYOffset)
{
	iVirualScreenWidth = aWidthInPixels;
	iVirualScreenHeight = aHeightInPixels;

	SetMatrixMode(GL_PROJECTION);
	LoadIdentityMatrix(); //reset matrix to identity
	glOrthox(0, TIntFloat::Convert(iVirualScreenWidth).GetIntInBaseIntFloat(), //left X, right X
					 0, TIntFloat::Convert(iVirualScreenHeight).GetIntInBaseIntFloat(), //left Y, right Y
					 TIntFloat::Convert(GRAPHICS_PROJECTION_MATRIX_NEAR_Z).GetIntInBaseIntFloat(),
					 TIntFloat::Convert(GRAPHICS_PROJECTION_MATRIX_FAR_Z).GetIntInBaseIntFloat()); //near Z, far Z
	SetMatrixMode(GL_MODELVIEW);

	//change screen resolution to match world coordinates
	glViewport(aXOffset, aYOffset, iActualScreenWidth - aXOffset, iActualScreenHeight - aYOffset);
}

void CGraphicsEngine::CenterViewToLocation(TPointIntFloat aLocation)
{
	CenterViewToLocation(aLocation, 1, 1);
}

void CGraphicsEngine::CenterViewToLocation(TPointIntFloat aLocation, TFloat aZoomFactorHorizontal, TFloat aZoomFactorVertical)
{
	TInt lScreenWidth = iVirualScreenWidth * aZoomFactorHorizontal;
	TInt lScreenHeight = iVirualScreenHeight * aZoomFactorVertical;

	SetMatrixMode(GL_PROJECTION);
	LoadIdentityMatrix(); //reset matrix to identity
	glOrthox(0, TIntFloat::Convert(lScreenWidth).GetIntInBaseIntFloat(), //left X, right X
					 0, TIntFloat::Convert(lScreenHeight).GetIntInBaseIntFloat(), //left Y, right Y
					 TIntFloat::Convert(GRAPHICS_PROJECTION_MATRIX_NEAR_Z).GetIntInBaseIntFloat(),
					 TIntFloat::Convert(GRAPHICS_PROJECTION_MATRIX_FAR_Z).GetIntInBaseIntFloat()); //near Z, far Z

	TIntFloat lTransletX = aLocation.iX - (TIntFloat::Convert(lScreenWidth) / 2);
	TIntFloat lTransletY = aLocation.iY - (TIntFloat::Convert(lScreenHeight) / 2);
	TranslateMatrix(-lTransletX, -lTransletY, TIntFloat::Convert(0));
	SetMatrixMode(GL_MODELVIEW);
}

//checks all OpenGl Entries and removes those that have expired
void CGraphicsEngine::CheckOpenGlCacheForExpiredEntries()
{
	for(TInt lEntryIndex = 0; lEntryIndex < iOpenGlCacheArray->GetCount(); lEntryIndex++)
	{
		if((*iOpenGlCacheArray)[lEntryIndex]->FramesLeftInCache == 0)
		{
			GLuint& lOpenGlId = (*iOpenGlCacheArray)[lEntryIndex]->TextureIdRef.OpenGlId;
			if(lOpenGlId != 0)
			{
				DeleteTextureFromGPU(lOpenGlId);
			}
		}
		else //still has some time left in the cache
		{
			(*iOpenGlCacheArray)[lEntryIndex]->FramesLeftInCache--; //decrement count left
		}
	}
}

void CGraphicsEngine::ClearScreen()
{
	iNumberOfTextureCallsBatched = 0; //since we clear the screen, we can cancell all pending draw calls
	glClear(GL_COLOR_BUFFER_BIT);
}

void CGraphicsEngine::ConvertBitmapImageTo2DTextureWithReadingStartingByDefault(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId)
{
	SBitmapColour lScanLineColour;
	SBitmapColour lDoNotDrawColour;

	lScanLineColour.RedComponent = CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_RED;
	lScanLineColour.GreenComponent = CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_GREEN;
	lScanLineColour.BlueComponent = CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_BLUE;

	lDoNotDrawColour.RedComponent = CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_RED;
	lDoNotDrawColour.GreenComponent = CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_GREEN;
	lDoNotDrawColour.BlueComponent = CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_BLUE;

	ConvertBitmapImageTo2DTextureWithReadingStartingByDefault(aTextureId, aBitmap, aClampImage, lScanLineColour, lDoNotDrawColour, aImageId);
}

void CGraphicsEngine::ConvertBitmapImageTo2DTexture(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId)
{
	SBitmapColour lScanLineColour;
	SBitmapColour lDoNotDrawColour;

	lScanLineColour.RedComponent = CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_RED;
	lScanLineColour.GreenComponent = CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_GREEN;
	lScanLineColour.BlueComponent = CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_BLUE;

	lDoNotDrawColour.RedComponent = CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_RED;
	lDoNotDrawColour.GreenComponent = CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_GREEN;
	lDoNotDrawColour.BlueComponent = CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_BLUE;

	ConvertBitmapImageTo2DTexture(aTextureId, aBitmap, aClampImage, lScanLineColour, lDoNotDrawColour, aImageId);
}

void CGraphicsEngine::ConvertBitmapImageTo2DTextureWithReadingStartingByDefault(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, SBitmapColour aScanLineColour, SBitmapColour aDontDrawPixelColour, const char* aImageId)
{
	//OpenGL texture have to be off Power of 2
	TSize lSizeOfBitmap = TSize(aBitmap.WidthInPixels, aBitmap.HeightInPixels);
	TSize lSizeOfTexture = TSize(CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iWidth, 2), CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iHeight, 2));
	//find out the real Max T and S values for this texture
	aTextureId.MaxS = TIntFloat::Convert(lSizeOfBitmap.iWidth) / TIntFloat::Convert(lSizeOfTexture.iWidth);
	aTextureId.MaxT = TIntFloat::Convert(lSizeOfBitmap.iHeight) / TIntFloat::Convert(lSizeOfTexture.iHeight);

	TInt lNumberOfPixelsInTexture = lSizeOfTexture.iWidth * lSizeOfTexture.iHeight;//Pixels in the image
	TUint8* lSourceBytes = aBitmap.Data;//returns address of first Byte in the texture
	TUint8* lDestinationBytes = new TUint8[lNumberOfPixelsInTexture * 4]; // each pixel has 4 bytes:1 Red,1 Green,1 Blue,1 Alpha
	TUint8* lDataBytes = lDestinationBytes;
	TBool lDrawModeTurnedOn = true;//used to set what Pixels get an Alpha value of GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN
	SBitmapColour lSourceBitmapColour;

	//this loop transforms the bitmap colour format into internal OpenGl colour format
	for(TInt lHeightIndex = 0; lHeightIndex < lSizeOfTexture.iHeight; lHeightIndex++)
	{
		for(TInt lWidthIndex = 0; lWidthIndex < lSizeOfTexture.iWidth; lWidthIndex++)
		{
			if(lWidthIndex >= lSizeOfBitmap.iWidth || lHeightIndex >= lSizeOfBitmap.iHeight)
			{
				//in this case Pixel is not located in the bitmap and thus will not be drawn in its entirety
				lDestinationBytes[0] = 0; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
				lDestinationBytes[1] = 0; //therefore, need to switch the B byte with the R byte's position
				lDestinationBytes[2] = 0;
				lDestinationBytes[3] = 0; //don't draw Pixel
				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
			}
			else // inside original Bitmap
			{
				lSourceBitmapColour.BlueComponent = lSourceBytes[0];
				lSourceBitmapColour.GreenComponent = lSourceBytes[1];
				lSourceBitmapColour.RedComponent = lSourceBytes[2];

				if (lWidthIndex == 0)
					lDrawModeTurnedOn = true; //by default if we start on a new line drawing will be turned on

				//check if any changes to the drawing mode need to happen
				if (lSourceBitmapColour.BlueComponent == aScanLineColour.BlueComponent
						&& lSourceBitmapColour.GreenComponent == aScanLineColour.GreenComponent
						&& lSourceBitmapColour.RedComponent == aScanLineColour.RedComponent)
				{
					lDrawModeTurnedOn = !lDrawModeTurnedOn;
					//since it is justa  signal pixel, don't draw it
					lDestinationBytes[0] = 0;
					lDestinationBytes[1] = 0;
					lDestinationBytes[2] = 0;
					lDestinationBytes[3] = 0;
				}
				else if (!lDrawModeTurnedOn ||
									(lSourceBitmapColour.BlueComponent == aDontDrawPixelColour.BlueComponent
								&& lSourceBitmapColour.GreenComponent == aDontDrawPixelColour.GreenComponent
								&& lSourceBitmapColour.RedComponent == aDontDrawPixelColour.RedComponent))
				{
					//don't draw this pixel
					lDestinationBytes[0] = 0;
					lDestinationBytes[1] = 0;
					lDestinationBytes[2] = 0;
					lDestinationBytes[3] = 0;
				}
				else
				{
					//this pixel should be drawn
					lDestinationBytes[0] = lSourceBytes[2]; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
					lDestinationBytes[1] = lSourceBytes[1]; //therefore, need to switch the B byte with the R byte's position
					lDestinationBytes[2] = lSourceBytes[0];
					lDestinationBytes[3] = GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN;
				}

				lDestinationBytes += 4; //since we have an Alpha byte we need to advance by 4
				lSourceBytes += 4; //bitmaps don't have an alpha channel
			}
		}
	}

	//need to go through the texture again and fix up some of the border cases, since it causes with the linear mapping in OpenGL
	lDestinationBytes = lDataBytes; //reset
	AdjustBorderPixels(lDestinationBytes, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight);

	delete[] aBitmap.Data;// free up memory, since bitmap will be in OpenGL memory after the few statements

	//add entry to cache list, still need to keep data main memory in case it expires and we need to load it back in
	AddOpenGlCacheEntry(lDataBytes, aTextureId, aClampImage, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight, aImageId);
}

//converts a bitmap to a OpenGL2D texture, performs masking by using aScanLine Colour and don't draw Pixel Colour
void CGraphicsEngine::ConvertBitmapImageTo2DTexture(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, SBitmapColour aScanLineColour, SBitmapColour aDontDrawPixelColour, const char* aImageId)
{
	//OpenGL texture have to be off Power of 2
	TSize lSizeOfBitmap = TSize(aBitmap.WidthInPixels, aBitmap.HeightInPixels);
	TSize lSizeOfTexture = TSize(CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iWidth, 2), CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iHeight, 2));
	//find out the real Max T and S values for this texture
	aTextureId.MaxS = TIntFloat::Convert(lSizeOfBitmap.iWidth) / TIntFloat::Convert(lSizeOfTexture.iWidth);
	aTextureId.MaxT = TIntFloat::Convert(lSizeOfBitmap.iHeight) / TIntFloat::Convert(lSizeOfTexture.iHeight);

	TInt lNumberOfPixelsInTexture = lSizeOfTexture.iWidth * lSizeOfTexture.iHeight;//Pixels in the image
	TUint8* lSourceBytes = aBitmap.Data;//returns address of first Byte in the texture
	TUint8* lDestinationBytes = new TUint8[lNumberOfPixelsInTexture * 4]; // each pixel has 4 bytes:1 Red,1 Green,1 Blue,1 Alpha
	TUint8* lDataBytes = lDestinationBytes;
	TBool lDrawModeTurnedOn = false;//used to set what Pixels get an Alpha value of GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN
	SBitmapColour lSourceBitmapColour;

	//this loop transforms the bitmap colour format into internal OpenGl colour format
	for(TInt lHeightIndex = 0; lHeightIndex < lSizeOfTexture.iHeight; lHeightIndex++)
	{
		for(TInt lWidthIndex = 0; lWidthIndex < lSizeOfTexture.iWidth; lWidthIndex++)
		{
			if(lWidthIndex >= lSizeOfBitmap.iWidth || lHeightIndex >= lSizeOfBitmap.iHeight)
			{
				//in this case Pixel is not located in the bitmap and thus will not be drawn in its entirety
				lDestinationBytes[0] = 0; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
				lDestinationBytes[1] = 0; //therefore, need to switch the B byte with the R byte's position
				lDestinationBytes[2] = 0;
				lDestinationBytes[3] = 0; //don't draw Pixel
				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
			}
			else // inside original Bitmap
			{
				lSourceBitmapColour.BlueComponent = lSourceBytes[0];
				lSourceBitmapColour.GreenComponent = lSourceBytes[1];
				lSourceBitmapColour.RedComponent = lSourceBytes[2];

				//check if any changes to the drawing mode need to happen
				if(lSourceBitmapColour.BlueComponent == aScanLineColour.BlueComponent && lSourceBitmapColour.GreenComponent == aScanLineColour.GreenComponent && lSourceBitmapColour.RedComponent == aScanLineColour.RedComponent)
				{
					lDrawModeTurnedOn = !lDrawModeTurnedOn;
					//since it is just signal pixel, don't draw it
					lDestinationBytes[0] = 0;
					lDestinationBytes[1] = 0;
					lDestinationBytes[2] = 0;
					lDestinationBytes[3] = 0;
				}else if(!lDrawModeTurnedOn || (lSourceBitmapColour.BlueComponent == aDontDrawPixelColour.BlueComponent && lSourceBitmapColour.GreenComponent == aDontDrawPixelColour.GreenComponent && lSourceBitmapColour.RedComponent == aDontDrawPixelColour.RedComponent))
				{
					//this pixel should not be drawn
					lDestinationBytes[0] = 0;
					lDestinationBytes[1] = 0;
					lDestinationBytes[2] = 0;
					lDestinationBytes[3] = 0;//don't draw this pixel
				}else
				{
					//this pixel should be drawn
					lDestinationBytes[0] = lSourceBytes[2]; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
					lDestinationBytes[1] = lSourceBytes[1]; //therefore, need to switch the B byte with the R byte's position
					lDestinationBytes[2] = lSourceBytes[0];
					lDestinationBytes[3] = GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN;
				}

				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
				lSourceBytes += 4;//bitmaps don't have an alpha channel
			}
		}
	}

	//need to go through the texture again and fix up some of the border cases, since it causes with the linear mapping in OpenGL
	lDestinationBytes = lDataBytes; //reset
	AdjustBorderPixels(lDestinationBytes, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight);

	delete[] aBitmap.Data;// free up memory, since bitmap will be in OpenGL memory after the next few statements

	//add entry to cache list, still need to keep data main memory in case it expires and we need to load it back in
	AddOpenGlCacheEntry(lDataBytes, aTextureId, aClampImage, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight, aImageId);
}

//transforms a Bitmap to an OpenGl texture without masking
void CGraphicsEngine::ConvertBitmapImageTo2DTextureWithoutMasking(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId)
{
	//OpenGL texture have to be off Power of 2
	TSize lSizeOfBitmap = TSize(aBitmap.WidthInPixels, aBitmap.HeightInPixels);
	TSize lSizeOfTexture = TSize(CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iWidth, 2), CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iHeight, 2));
	//find out the real Max T and S values for this texture
	aTextureId.MaxS = TIntFloat::Convert(lSizeOfBitmap.iWidth) / TIntFloat::Convert(lSizeOfTexture.iWidth);
	aTextureId.MaxT = TIntFloat::Convert(lSizeOfBitmap.iHeight) / TIntFloat::Convert(lSizeOfTexture.iHeight);

	TUint8* lSourceBytes = aBitmap.Data;//returns address of first Byte in the texture
	TInt lNumberOfPixelsInTexture = lSizeOfTexture.iWidth * lSizeOfTexture.iHeight;//Pixels in the image
	TUint8* lDestinationBytes = new TUint8[lNumberOfPixelsInTexture * 4]; // each pixel has 4 bytes:1 Red,1 Green,1 Blue,1 Alpha
	TUint8* lDataBytes = lDestinationBytes;

	for(TInt lHeightIndex = 0; lHeightIndex < lSizeOfTexture.iHeight; lHeightIndex++)
	{
		for(TInt lWidthIndex = 0; lWidthIndex < lSizeOfTexture.iWidth; lWidthIndex++)
		{
			if(lWidthIndex >= lSizeOfBitmap.iWidth || lHeightIndex >= lSizeOfBitmap.iHeight)
			{
				//in this case Pixel is not located in the bitmap and thus will not be drawn in its entirety
				lDestinationBytes[0] = 0; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
				lDestinationBytes[1] = 0; //therefore, need to switch the B byte with the R byte's position
				lDestinationBytes[2] = 0;
				lDestinationBytes[3] = 0; //don't draw Pixel
				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
			}
			else
			{
				//this transforms the bitmap colour format into internal OpenGl colour format
				lDestinationBytes[0] = lSourceBytes[2]; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
				lDestinationBytes[1] = lSourceBytes[1]; //therefore, need to switch the B byte with the R byte's position
				lDestinationBytes[2] = lSourceBytes[0];
				lDestinationBytes[3] = GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN;//all pixels will be drawn

				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
				lSourceBytes += 4;//bitmaps don't have an alpha channel
			}
		}
	}

	//need to go through the texture again and fix up some of the border cases, since it causes problems with the linear mapping in OpenGL
	lDestinationBytes = lDataBytes; //reset
	AdjustBorderPixels(lDestinationBytes, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight);

	delete[] aBitmap.Data;// free up memory, since bitmap will be in OpenGL memory after the few statements
	aBitmap.Data = NULL;

	//add entry to cache list, still need to keep data main memory in case it expires and we need to load it back in
	AddOpenGlCacheEntry(lDataBytes, aTextureId, aClampImage, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight, aImageId);
}

void CGraphicsEngine::ConvertPNGTo2DTexture(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId)
{
	//OpenGL texture have to be off Power of 2
	TSize lSizeOfBitmap = TSize(aBitmap.WidthInPixels, aBitmap.HeightInPixels);
	TSize lSizeOfTexture = TSize(CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iWidth, 2), CMath::GetNextGreatestMemberOfPower(lSizeOfBitmap.iHeight, 2));
	//find out the real Max T and S values for this texture
	aTextureId.MaxS = TIntFloat::Convert(lSizeOfBitmap.iWidth) / TIntFloat::Convert(lSizeOfTexture.iWidth);
	aTextureId.MaxT = TIntFloat::Convert(lSizeOfBitmap.iHeight) / TIntFloat::Convert(lSizeOfTexture.iHeight);

	TUint8* lSourceBytes = aBitmap.Data;//returns address of first Byte in the texture
	TInt lNumberOfPixelsInTexture = lSizeOfTexture.iWidth * lSizeOfTexture.iHeight;//Pixels in the image
	TUint8* lDestinationBytes = new TUint8[lNumberOfPixelsInTexture * 4]; // each pixel has 4 bytes:1 Red,1 Green,1 Blue,1 Alpha
	TUint8* lDataBytes = lDestinationBytes;

	for(TInt lHeightIndex = 0; lHeightIndex < lSizeOfTexture.iHeight; lHeightIndex++)
	{
		for(TInt lWidthIndex = 0; lWidthIndex < lSizeOfTexture.iWidth; lWidthIndex++)
		{
			if(lWidthIndex >= lSizeOfBitmap.iWidth || lHeightIndex >= lSizeOfBitmap.iHeight)
			{
				//in this case Pixel is not located in the bitmap and thus will not be drawn in its entirety
				lDestinationBytes[0] = 0; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
				lDestinationBytes[1] = 0; //therefore, need to switch the B byte with the R byte's position
				lDestinationBytes[2] = 0;
				lDestinationBytes[3] = 0; //don't draw Pixel
				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
			}
			else
			{
				//this transforms the bitmap colour format into internal OpenGl colour format
				lDestinationBytes[0] = lSourceBytes[2]; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
				lDestinationBytes[1] = lSourceBytes[1]; //therefore, need to switch the B byte with the R byte's position
				lDestinationBytes[2] = lSourceBytes[0];
				lDestinationBytes[3] = lSourceBytes[3];//pixel will be drawn if Alpha is large enough

				lDestinationBytes += 4;//since we have an Alpha byte we need to advance by 4
				lSourceBytes += 4;
			}
		}
	}

	//need to go through the texture again and fix up some of the border cases, since it causes problems with the linear mapping in OpenGL
	lDestinationBytes = lDataBytes; //reset
	AdjustBorderPixels(lDestinationBytes, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight);

	delete[] aBitmap.Data;// free up memory, since bitmap will be in OpenGL memory after the few statements
	aBitmap.Data = NULL;

	//add entry to cache list, still need to keep data main memory in case it expires and we need to load it back in
	AddOpenGlCacheEntry(lDataBytes, aTextureId, aClampImage, lSizeOfTexture.iWidth, lSizeOfTexture.iHeight, aImageId);
}

void CGraphicsEngine::CopyDrawingSurfaceToDestinationSurface()
{
	//make sure all calls are completed before swaping
	FlushBatchedTexturesToScreen();

	//swaps the frame buffer to the window frame buffer
	eglSwapBuffers(iDeviceDisplay, iDrawingSurface);
}

//creates an GraphicsTextureId for use with the Graphics Environment texture functions
SGraphicsEngineTextureId CGraphicsEngine::CreateGraphicsTextureId()
{
	SGraphicsEngineTextureId lGraphicTextureId;

	lGraphicTextureId.GraphicsEngineId = ++CGraphicsEngine::GraphicEngineTextureCounter;
	lGraphicTextureId.MaxS = TIntFloat::Convert(1);
	lGraphicTextureId.MaxT = TIntFloat::Convert(1);
	lGraphicTextureId.OpenGlId = 0;
	glGenTextures(1, &lGraphicTextureId.OpenGlId); //create also texture Id in OpenGl

	return lGraphicTextureId;
}

void CGraphicsEngine::DeleteAllGraphicsTextureIds()
{
	for(TInt lIndex = 0; lIndex < iOpenGlCacheArray->GetCount(); lIndex++)
	{
		DeleteGraphicsTextureId(iOpenGlCacheArray->Get(lIndex)->TextureIdRef);
		//need to decrement to avoid that a Index gets skipped
		lIndex--;
	}
}

void CGraphicsEngine::DeleteGraphicsTextureId(SGraphicsEngineTextureId& aTextureIdRef)
{
	//delete Texture From GPU
	DeleteTextureFromGPU(aTextureIdRef.OpenGlId);

	//remove first from OpenGl cache and reupload array to avoid that it gets reuploaded at some point
	RemoveOpenGlCacheEntry(aTextureIdRef.GraphicsEngineId);

	aTextureIdRef.GraphicsEngineId = 0; //signals that this image is not loaded into OpenGl
	aTextureIdRef.OpenGlId = 0; //signals that this image is not loaded into OpenGl
}

void CGraphicsEngine::DeleteTextureFromGPU(GLuint& aOpenGLId)
{
	if(aOpenGLId != 0)
	{
		for(TInt lEntryIndex = 0; lEntryIndex < iOpenGlCacheArray->GetCount(); lEntryIndex++)
		{
			if((*iOpenGlCacheArray)[lEntryIndex]->TextureIdRef.OpenGlId == aOpenGLId)
			{
				glDeleteTextures(1, &aOpenGLId);
				aOpenGLId = 0; //to signal that it is currently not in OpenGL memory

				//update Memory stats
				SBitmap& lBitmap = (*iOpenGlCacheArray)[lEntryIndex]->Bitmap;
				CGame::Game->iMemoryUsageTracker->UpdateGPUUsedForTextures(-lBitmap.WidthInPixels * lBitmap.HeightInPixels * 4);
				return;
			}
		}
	}
}

void CGraphicsEngine::DestroyOpenGlContext()
{
	RemoveTexturesFromGPUThatHaveBeenCached();

	//------clean up EGL stuff--------
	//set context to nothing
	eglMakeCurrent(iDeviceDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	//destroy old context
	eglDestroyContext(iDeviceDisplay, iGraphicsContext);//to avoid memory leak, we need to destroy the context at the end of its lifetime
	//destroy surface used
	eglDestroySurface(iDeviceDisplay, iDrawingSurface);
	//terminate handle to display
	eglTerminate(iDeviceDisplay);//destroys EGL instance, since we don't share contextes between instances we can terminate the display
}

void CGraphicsEngine::DrawLine(const TPointIntFloat& aPoint1, const TPointIntFloat& aPoint2)
{
	//since we have a state change
	FlushBatchedTexturesToScreen();

	//disable the use of texture
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GLfixed lVertexes[] =
	{aPoint1.iX.GetIntInBaseIntFloat(), aPoint1.iY.GetIntInBaseIntFloat(),
   aPoint2.iX.GetIntInBaseIntFloat(), aPoint2.iY.GetIntInBaseIntFloat()};
	glVertexPointer(2, GL_FIXED, 0, lVertexes);
	glDrawArrays(GL_LINES, 0, 2);

	//enable the use of textures again
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void CGraphicsEngine::DrawLines(CPointerArray<TPointIntFloat>& aDrawPoints)
{
	//since we have a state change
	FlushBatchedTexturesToScreen();

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GLfixed* lVertexes = new GLfixed[aDrawPoints.GetCount() * 2];
	TInt lYVertexOffset = 0;// since X and Y are in different locations

	for(TInt lCurrentPoint = 0; lCurrentPoint < aDrawPoints.GetCount(); lCurrentPoint++)
	{
		lVertexes[lCurrentPoint + lYVertexOffset] = aDrawPoints [lCurrentPoint]->iX.GetIntInBaseIntFloat();
		lVertexes[lCurrentPoint + lYVertexOffset + 1] = aDrawPoints[lCurrentPoint]->iY.GetIntInBaseIntFloat();
		lYVertexOffset++;
	}

	glVertexPointer(2, GL_FIXED, 0, lVertexes);
	glDrawArrays(GL_LINE_STRIP, 0, aDrawPoints.GetCount());

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	delete[] lVertexes; //don't forget to clean up
}

void CGraphicsEngine::DrawRect(const TRectIntFloat& aRect)
{
	//since we have a state change
	FlushBatchedTexturesToScreen();

	glDisable(GL_TEXTURE_2D);//textures must be turned off, otherwise a texture will be applied to the line
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GLfixed lVertexes[] =
	{
			aRect.iTl.iX.GetIntInBaseIntFloat(), aRect.iTl.iY.GetIntInBaseIntFloat(),//top left
			aRect.iBr.iX.GetIntInBaseIntFloat(), aRect.iTl.iY.GetIntInBaseIntFloat(),//top right
			aRect.iBr.iX.GetIntInBaseIntFloat(), aRect.iBr.iY.GetIntInBaseIntFloat(),//bottom right
			aRect.iTl.iX.GetIntInBaseIntFloat(), aRect.iBr.iY.GetIntInBaseIntFloat(),//bottom left
			aRect.iTl.iX.GetIntInBaseIntFloat(), aRect.iTl.iY.GetIntInBaseIntFloat() //top left, again, to complete Rectangle
	};
	glVertexPointer(2, GL_FIXED, 0, &lVertexes[0]);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void CGraphicsEngine::DrawTextureBatched(const SPolygonSquare& aSquarePolygon, GLuint aTextureId)
{
	if(aTextureId != iBoundTextureId || iNumberOfTextureCallsBatched == GRAPHICS_MAX_NUMBER_OF_BATCHED_TEXTURE_CALLS)
	{
		//make call draw batched texture
		FlushBatchedTexturesToScreen();

		iBoundTextureId = aTextureId;
	}

	iPolygonSquareBatch[iNumberOfTextureCallsBatched] = aSquarePolygon;

	//increase Count
	iNumberOfTextureCallsBatched++;
}

void CGraphicsEngine::DrawTextureImmediately(const SPolygonSquare& aSquarePolygon, GLuint aTextureId)
{
	if(iNumberOfTextureCallsBatched > 0)
	{
		FlushBatchedTexturesToScreen(); //make sure to finish up any pending calls
		iBoundTextureId = aTextureId;
	}

	//draw right away
	glBindTexture(GL_TEXTURE_2D, iBoundTextureId);
	glVertexPointer(GRAPHICS_TEXTURE_NUMBER_OF_VERTEX_COORDINATES_DIMENSIONS,GL_FIXED, sizeof(SVertex),&aSquarePolygon.TopLeftCorner.Coordinate);
	glColorPointer(GRAPHICS_TEXTURE_NUMBER_OF_COLOUR_DIMENSIONS, GL_UNSIGNED_BYTE, sizeof(SVertex), &aSquarePolygon.TopLeftCorner.Colour);
	glTexCoordPointer(GRAPHICS_TEXTURE_NUMBER_OF_TEXTURE_DIMENSIONS,GL_FIXED,sizeof(SVertex),&aSquarePolygon.TopLeftCorner.Texture);
	glDrawArrays(GL_TRIANGLE_STRIP,0,GRAPHICS_TEXTURE_NUMBER_OF_VERTEXES);
}

void CGraphicsEngine::FillRect(const TRectIntFloat& aRect)
{
	//since we have a state change
	FlushBatchedTexturesToScreen();

	glDisable(GL_TEXTURE_2D);//textures must be turned off, otherwise a texture will be applied to the line
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GLfixed lVertexes[] =
	{
			aRect.iTl.iX.GetIntInBaseIntFloat(), aRect.iTl.iY.GetIntInBaseIntFloat(),//top left
			aRect.iBr.iX.GetIntInBaseIntFloat(), aRect.iTl.iY.GetIntInBaseIntFloat(),//top right
			aRect.iTl.iX.GetIntInBaseIntFloat(), aRect.iBr.iY.GetIntInBaseIntFloat(),//bottom left
			aRect.iBr.iX.GetIntInBaseIntFloat(), aRect.iBr.iY.GetIntInBaseIntFloat() //bottom right
	};
	glVertexPointer(2, GL_FIXED, 0, &lVertexes[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

}

void CGraphicsEngine::FlushBatchedTexturesToScreen()
{
	if(iNumberOfTextureCallsBatched > 0)
	{
		glBindTexture(GL_TEXTURE_2D, iBoundTextureId);
		glVertexPointer(GRAPHICS_TEXTURE_NUMBER_OF_VERTEX_COORDINATES_DIMENSIONS, GL_FIXED, sizeof(SVertex), &iPolygonSquareBatch[0].TopLeftCorner.Coordinate);
		glColorPointer(GRAPHICS_TEXTURE_NUMBER_OF_COLOUR_DIMENSIONS, GL_UNSIGNED_BYTE, sizeof(SVertex), &iPolygonSquareBatch[0].TopLeftCorner.Colour);
		glTexCoordPointer(GRAPHICS_TEXTURE_NUMBER_OF_TEXTURE_DIMENSIONS, GL_FIXED, sizeof(SVertex), &iPolygonSquareBatch[0].TopLeftCorner.Texture);
		glDrawElements(GL_TRIANGLES, iNumberOfTextureCallsBatched * 6, GL_UNSIGNED_SHORT, &iTextureIndicies[0]);
	}

	iNumberOfTextureCallsBatched = 0;
}

const CString* CGraphicsEngine::GetImageIdentifier(SGraphicsEngineTextureId& aTextureId)
{
	CString* lImageId = NULL;

	for(TInt lIndex = 0; lIndex < iOpenGlCacheArray->GetCount(); lIndex++)
	{
		if(iOpenGlCacheArray->Get(lIndex)->TextureIdRef.GraphicsEngineId == aTextureId.GraphicsEngineId)
		{
			lImageId = iOpenGlCacheArray->Get(lIndex)->aImageIdentifier;
			break;
		}
	}

	return lImageId;
}

TSize CGraphicsEngine::GetRealDimensionsOfScreenInPixels() const
{
	TSize lRealDimensionOfScreen(iActualScreenWidth, iActualScreenHeight);
	return lRealDimensionOfScreen;
}

SGraphicsEngineTextureId* CGraphicsEngine::GetTextureId(const char* aImageIdentifier)
{
	for(TInt lIndex = 0; lIndex < iOpenGlCacheArray->GetCount(); lIndex++)
	{
		if(iOpenGlCacheArray->Get(lIndex)->aImageIdentifier->EqualTo(aImageIdentifier))
		{
			return &iOpenGlCacheArray->Get(lIndex)->TextureIdRef;
		}
	}

	return NULL; //was not found
}

TSizeIntFloat CGraphicsEngine::GetVirtualDimensionsOfScreen()  const
{
	TSizeIntFloat lVirtualDimensionsOfScreen(TIntFloat::Convert(iVirualScreenWidth), TIntFloat::Convert(iVirualScreenHeight));
	return lVirtualDimensionsOfScreen;
}

TSize CGraphicsEngine::GetVirtualDimensionsOfScreenAsInts()  const
{
	TSize lVirtualDimensionsOfScreen(iVirualScreenWidth, iVirualScreenHeight);
	return lVirtualDimensionsOfScreen;
}

void CGraphicsEngine::LoadIdentityMatrix()
{
	FlushBatchedTexturesToScreen(); //finish any drawing before Matrix changes
	glLoadIdentity();
}

void CGraphicsEngine::PopMatrix()
{
	FlushBatchedTexturesToScreen(); //finish any drawing before Matrix changes
	glPopMatrix();
}

void CGraphicsEngine::PushMatrix()
{
	FlushBatchedTexturesToScreen(); //finish any drawing before Matrix changes
	glPushMatrix();
}

void CGraphicsEngine::RecreateOpenGlContext()
{
	SetUpWindowDrawingSurface();
}

void CGraphicsEngine::RotateMatrix(TFloat aAngle, TFloat aX, TFloat aY, TFloat aZ)
{
	FlushBatchedTexturesToScreen(); //finish any drawing before Matrix changes
	glRotatef(aAngle, aX, aY, aZ);
}

SOpenGLCacheEntry* CGraphicsEngine::RefreshOpenGlCacheEntry(SGraphicsEngineTextureId& TextureIdRef, SOpenGLCacheEntry* aLikelyEntry)
{
	SOpenGLCacheEntry* lNeededEntry = NULL;

	for(TInt lEntryIndex = 0; lEntryIndex < iOpenGlCacheArray->GetCount(); lEntryIndex++)
	{
		SOpenGLCacheEntry* lEntry = iOpenGlCacheArray->Get(lEntryIndex);
		if(lEntry->TextureIdRef.GraphicsEngineId == TextureIdRef.GraphicsEngineId)
		{
			//found the entry needed for this texture Id
			lNeededEntry = lEntry;
			break;
		}
	}

	if(lNeededEntry != NULL)
	{
		//update the entry
		lNeededEntry->FramesLeftInCache = GRAPHICS_OPEN_GL_CACHE_FRAMES_UNTIL_EXPIRE;
		if(lNeededEntry->TextureIdRef.OpenGlId == 0)
		{
			//has been deleted from OpenGl memory, we need to load it back in
			GLuint& lOpenGlId = lNeededEntry->TextureIdRef.OpenGlId;
			glGenTextures(1, &lOpenGlId);
			LoadTextureDataIntoOpenGlMemory(lNeededEntry->Bitmap.Data, lOpenGlId, lNeededEntry->ClampImage, lNeededEntry->Bitmap.WidthInPixels, lNeededEntry->Bitmap.HeightInPixels);
		}
	}
	return lNeededEntry; //next time when calling this function use this pointer so we don't need to look through the entire array
}

void CGraphicsEngine::RemoveTexturesFromGPUThatHaveBeenCached()
{
	if(iOpenGlCacheArray != NULL)
	{
		for(TInt lIndex = 0; lIndex < iOpenGlCacheArray->GetCount(); lIndex++)
		{
			GLuint& lOpenGlIdRef = iOpenGlCacheArray->Get(lIndex)->TextureIdRef.OpenGlId;
			if(lOpenGlIdRef != 0)
			{
				DeleteTextureFromGPU(lOpenGlIdRef);
			}
		}
	}
}

void CGraphicsEngine::ScaleMatrix(TFloat aX, TFloat aY, TFloat aZ)
{
	FlushBatchedTexturesToScreen(); //finish up draw calls before matrix changes
	glScalef(aX, aY, aZ);
}

void CGraphicsEngine::SetClearColour(const TColour& aColour)
{
	SetClearColourInt((aColour & (0xFF000000)) >> 24,(aColour & (0x00FF0000))  >> 16, (aColour & (0x0000FF00)) >> 8,  aColour & (0x000000FF));
}

void CGraphicsEngine::SetClearColourInt(TInt aRedValue, TInt aGreenValue, TInt aBlueValue, TInt aAlphaValue)
{
	glClearColor((TFloat) aRedValue / 255, (TFloat) aGreenValue / 255,  (TFloat) aBlueValue / 255, (TFloat) aAlphaValue / 255);
}

void CGraphicsEngine::SetClearColourFloat(TFloat aRedValue, TFloat aGreenValue, TFloat aBlueValue, TFloat aAlphaValue)
{
	glClearColor(aRedValue, aGreenValue, aBlueValue, aAlphaValue);
}

void CGraphicsEngine::SetDrawColour(const TColour& aColour)
{
	SetDrawColourInt((aColour & (0xFF000000)) >> 24,(aColour & (0x00FF0000))  >> 16, (aColour & (0x0000FF00)) >> 8,  aColour & (0x000000FF));
}

void CGraphicsEngine::SetDrawColourInt(TInt aRedValue, TInt aGreenValue, TInt aBlueValue, TInt aAlphaValue)
{
	glColor4f((TFloat) aRedValue / 255, (TFloat) aGreenValue / 255,  (TFloat) aBlueValue / 255, (TFloat) aAlphaValue / 255);
}

void CGraphicsEngine::SetDrawColourFloat(TFloat aRedValue, TFloat aGreenValue, TFloat aBlueValue, TFloat aAlphaValue)
{
	glColor4f(aRedValue, aGreenValue, aBlueValue, aAlphaValue);
}

void CGraphicsEngine::SetGraphicsEngineSettings(GLenum aSetting, TBool aEnable)
{
	//make sure all drawing is done
	FlushBatchedTexturesToScreen();

	if(aEnable)
	{
		glEnable(aSetting);
	}
	else
	{
		glDisable(aSetting);
	}
}

void CGraphicsEngine::SetMatrixMode(GLenum aMatrixMode)
{
	glMatrixMode(aMatrixMode);
}

void CGraphicsEngine::TranslateMatrix(TIntFloat aX, TIntFloat aY, TIntFloat aZ)
{
	FlushBatchedTexturesToScreen(); //make sure all drawing is completed before changing the current matrix
	glTranslatex(aX.GetIntInBaseIntFloat(), aY.GetIntInBaseIntFloat(), aZ.GetIntInBaseIntFloat());
}

void CGraphicsEngine::AddOpenGlCacheEntry(TUint8* aData, SGraphicsEngineTextureId& aTextureId, TBool aClampImage, TInt aWidthInPixels, TInt aHeightInPixels,  const char* aImageIdentifier)
{
	SBitmap lBitmap = {aData, aWidthInPixels, aHeightInPixels};
	CString* lImageId = new CString(aImageIdentifier);

	SOpenGLCacheEntry lEntry = {lBitmap, aClampImage, 1, aTextureId, lImageId};

	//Add to cache
	iOpenGlCacheArray->Append(new SOpenGLCacheEntry(lEntry));

	//delete entry from OpenGL cache, since we might not need it right away
	if(aTextureId.OpenGlId != 0)
	{
		glDeleteTextures(1, &aTextureId.OpenGlId);
		aTextureId.OpenGlId = 0;
	}

	//update stats
	CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForTextureCache(aWidthInPixels * aHeightInPixels * 4);
}

void CGraphicsEngine::AdjustBorderPixels(TUint8* aData, TInt aWidthInPixels, TInt aHeightInPixels)
{
	for(TInt lHeightIndex = 0; lHeightIndex < aHeightInPixels; lHeightIndex++)
	{
		for(TInt lWidthIndex = 0; lWidthIndex < aWidthInPixels; lWidthIndex++)
		{
			TInt lNumberOfPixelsAreValid = 0;
			TInt lRedChannel = 0;
			TInt lGreenChannel = 0;
			TInt lBlueChannel = 0;
			TInt lTopPixelOffset, lBottomPixelOffset, lLeftPixelOffset, lRightPixelOffset;

			//top
			if(lHeightIndex > 0)
			{
				lTopPixelOffset = -aWidthInPixels * 4; //go one line above
			}
			//Bottom
			if(lHeightIndex + 1 < aHeightInPixels)
			{
				lBottomPixelOffset = aWidthInPixels * 4; //go one line below
			}
			//Left
			if(lWidthIndex > 0)
			{
				lLeftPixelOffset = -4; //go one pixel to the left
			}
			//Right
			if(lWidthIndex + 1 < aWidthInPixels)
			{
				lRightPixelOffset = 4; //go one pixel to the right
			}

			if(aData[3] == 0) //only modift pixel data for pixels that will not be drawn
			{
				//top
				if(lHeightIndex > 0 && aData[lTopPixelOffset + 3] == GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN)
				{
					lRedChannel += aData[lTopPixelOffset];
					lGreenChannel += aData[lTopPixelOffset + 1];
					lBlueChannel += aData[lTopPixelOffset + 2];
					lNumberOfPixelsAreValid++;
				}
				//Bottom
				if(lHeightIndex + 1 < aHeightInPixels && aData[lBottomPixelOffset + 3] == GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN)
				{
					lRedChannel += aData[lBottomPixelOffset];
					lGreenChannel += aData[lBottomPixelOffset + 1];
					lBlueChannel += aData[lBottomPixelOffset + 2];
					lNumberOfPixelsAreValid++;
				}
				//Left
				if(lWidthIndex > 0 && aData[lLeftPixelOffset + 3] == GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN)
				{
					lRedChannel += aData[lLeftPixelOffset];
					lGreenChannel += aData[lLeftPixelOffset + 1];
					lBlueChannel += aData[lLeftPixelOffset + 2];
					lNumberOfPixelsAreValid++;
				}
				//Right
				if(lWidthIndex + 1 < aWidthInPixels && aData[lRightPixelOffset + 3] == GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN)
				{
					lRedChannel += aData[lRightPixelOffset];
					lGreenChannel += aData[lRightPixelOffset + 1];
					lBlueChannel += aData[lRightPixelOffset + 2];
					lNumberOfPixelsAreValid++;
				}

				if(lNumberOfPixelsAreValid > 0)
				{
					//this transforms the bitmap colour format into internal OpenGl colour format
					aData[0] = lRedChannel / lNumberOfPixelsAreValid; //bitmaps have a BGR sorting of bytes, openGL uses RGBA
					aData[1] = lGreenChannel / lNumberOfPixelsAreValid; //therefore, need to switch the B byte with the R byte's position
					aData[2] = lBlueChannel / lNumberOfPixelsAreValid;
					aData[3] = 1;
				}
			}

			aData += 4;//since we have an Alpha byte we need to advance by 4
		}
	}
}

void CGraphicsEngine::RemoveOpenGlCacheEntry(const TUint64& aGraphicsEngineId)
{
	if(iOpenGlCacheArray != NULL)
	{
		for(TInt lIndex = 0; lIndex < iOpenGlCacheArray->GetCount(); lIndex++)
		{
			SOpenGLCacheEntry* lOpenGlCacheEntry = iOpenGlCacheArray->Get(lIndex);

			if(lOpenGlCacheEntry->TextureIdRef.GraphicsEngineId == aGraphicsEngineId)
			{
				//remove it from GPU if it is loaded
				if(lOpenGlCacheEntry->TextureIdRef.OpenGlId != 0)
				{
					DeleteTextureFromGPU(iOpenGlCacheArray->Get(lIndex)->TextureIdRef.OpenGlId);
				}

				//delete String
				if(lOpenGlCacheEntry->aImageIdentifier != NULL)
				{
					delete iOpenGlCacheArray->Get(lIndex)->aImageIdentifier;
					iOpenGlCacheArray->Get(lIndex)->aImageIdentifier = NULL;
				}

				//delete Bitmap that holds data
				if(lOpenGlCacheEntry->Bitmap.Data != NULL)
				{
					//update memory stats
					SBitmap& lBitmapRef = lOpenGlCacheEntry->Bitmap;
					CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForTextureCache(-lBitmapRef.HeightInPixels * lBitmapRef.WidthInPixels * 4);

					delete[] lOpenGlCacheEntry->Bitmap.Data;
					lOpenGlCacheEntry->Bitmap.Data = NULL;
				}

				iOpenGlCacheArray->RemoveAndDestroy(lIndex);
				return;
			}
		}
	}
}

void CGraphicsEngine::LoadTextureDataIntoOpenGlMemory(TUint8* aData, GLuint aTextureId, TBool aClampImage, TInt aWidth, TInt aHeight)
{
	//update stats
	CGame::Game->iMemoryUsageTracker->UpdateGPUUsedForTextures(aWidth * aHeight * 4);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, aTextureId);///all 2D texture operations will affect this texture object

	//loads the it into in internal OpenGl Texture attached to the textureId
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth, aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) aData);
	//set up how the texturing will be done for this texture
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//cheapest method available, takes the nearest pixel, Linear interpolation is too expensive
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(aClampImage)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

