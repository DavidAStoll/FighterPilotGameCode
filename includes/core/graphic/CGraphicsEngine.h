/*
 * CGraphicsEngine.h
 *
 *  Created on: Jun 5, 2012
 *      Author: dstoll
 */

#ifndef CGRAPHICSENGINE_H_
#define CGRAPHICSENGINE_H_

#include "includes/GlobalDefines.h"
#include "includes/core/standardLibrary/StandardLibrary.h"

//For Android
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <EGL/eglplatform.h>

#define KColourBlack 0x000000FF
#define KColourRed 0xFF0000FF
#define KColourDarkRed 0x8B0000FF
#define KColourWhite 0xFFFFFFFF
#define KColourBlue 0x0000FFFF
#define KColourDarkBlue 0x00008BFF
#define KColourGreen 0x00FF00FF
#define KColourLightGray 0xC0C0C0FF
#define KColourGray 0x909090FF
#define KColourDarkGray 0x808080FF
#define KColourSkyBlue 0x8DB6EBFF
#define KColourSmokeWhite 0xF5F5F5FF
#define KColourSmokeGray 0x726E6DFF


typedef TUint TColour;

#if PLATFORM_ANDROID
typedef NativeWindowType NativeWindow;
#else
#endif

#define CGRAPHICS_Z_BUFFER_SIZE 16
#define GRAPHICS_OPEN_GL_CACHE_FRAMES_UNTIL_EXPIRE (GFRAMES_PER_SECOND_BACKEND * 5) //stay caches for 5 seconds,
//for Projection Matrix
#define GRAPHICS_PROJECTION_MATRIX_NEAR_Z (32000)
#define GRAPHICS_PROJECTION_MATRIX_FAR_Z -32000
//clear Colour
#define GRAPHICS_CLEAR_COLOUR_RED 0
#define GRAPHICS_CLEAR_COLOUR_GREEN 0
#define GRAPHICS_CLEAR_COLOUR_BLUE 0
#define GRAPHICS_CLEAR_COLOUR_ALPHA 1
//Alpha value for colour to be drawn
#define GRAPHICS_MINIMUM_ALPHA_VALUE_TO_BE_DRAWN 255
//default values for scanning in bitmaps, when this colour is met, scanning mode will change
#define CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_RED 215
#define CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_GREEN 8
#define CGRAPHICS_DEFAULT_SCAN_LINE_COLOUR_BLUE 234
//anything with this colout will have a alpha value of 0
#define CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_RED 169
#define CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_GREEN 8
#define CGRAPHICS_DEFAULT_DO_NOT_DRAW_COLOUR_BLUE 184
//for find the textureIds
#define GRAPHICS_OPENGL_TEXTURE_HAS_NOT_BE_FOUND 100000
//for Texutre BATCHING
#define GRAPHICS_MAX_NUMBER_OF_BATCHED_TEXTURE_CALLS 42 //since we have to pass a byte, therefore max size is 255 and each texture needs 6 indicies
#define GRAPHICS_TEXTURE_NUMBER_OF_VERTEXES 4
#define GRAPHICS_TEXTURE_NUMBER_OF_VERTEX_COORDINATES_DIMENSIONS 3 //for x, y, and z
#define GRAPHICS_TEXTURE_NUMBER_OF_COLOUR_DIMENSIONS 4
#define GRAPHICS_TEXTURE_NUMBER_OF_TEXTURE_DIMENSIONS 2 //should always be 2, unless you have a 3D texture :)
//needed to know maxium texture width alloed in OpenGL, could be set during time, but right now use a value that always works
#define GRAPHICS_DEFAULT_MAX_TEXTURE_PIXEL_SIZE 1024 //width or height

enum TGraphicsEngineDisplayDepth //tells us how many bits the display can display
{
	EGraphicsEngineDisplayDepth12 = 12,
	EGraphicsEngineDisplayDepth16 = 16,
	EGraphicsEngineDisplayDepth24 = 24,
	EGraphicsEngineDisplayDepth32 = 32
};

enum TGraphicsEngineTextureConfiguration //tells us how many bits the a texture will take in OpenGL memory
{
	EGraphicsEngineTexture2Bytes4BitsEach5BitsForColour1ForAlpha = GL_UNSIGNED_SHORT_5_5_5_1,
	EGraphicsEngineTexture2Bytes4BitsEach = GL_UNSIGNED_SHORT_4_4_4_4,
	EGraphicsEngineTexture2Bytes5BitRed6BitGreeb5BitBlue = GL_UNSIGNED_SHORT_5_6_5,
	EGraphicsEngineTexture4Bytes8BitEach = GL_UNSIGNED_BYTE
};

//used mainly by reupload Entry to we know what method we need to use reupload the texture
enum TGraphicsEngineTextureLoadType
{
	EGraphicsEngineTextureLoadTypeBasic,
	EGraphicsEngineTextureLoadTypeWithReadingByDefault,
	EGraphicsEngineTextureLoadTypeWithoutMasking,
	EGraphicsEngineTextureLoadTypePNG
};

struct SVertexCoordinate
{
	TIntFloat x, y;
	TInt z;
};

struct SVertexColour
{
	TUint8 Red, Green, Blue, Alpha;
};

struct SVertexTexture
{
	TIntFloat s, t;
};

struct SVertex
{
	SVertexCoordinate Coordinate;
	SVertexColour Colour;
	SVertexTexture Texture;
};

struct SPolygonSquare
{
	SVertex TopLeftCorner;
	SVertex TopRightCorner;
	SVertex BottomLeftCorner;
	SVertex BottomRightCorner;
};

struct SBitmapColour
{
	TUint8 RedComponent;
	TUint8 GreenComponent;
	TUint8 BlueComponent;
};

struct SBitmap
{
	TUint8* Data;
	TInt WidthInPixels;
	TInt HeightInPixels;
};

struct SGraphicsEngineTextureId
{
	TUint64 GraphicsEngineId;
	GLuint OpenGlId;
	TIntFloat MaxS; //keeps track of the Max S value of the texture, might be less than 1, because the texture is not a power of 2 and thus does not use the entire texture size
	TIntFloat MaxT;
};

//keeps track of OpenGL Id, if it expires it will be removed from OpenGL Id, has all data to add it back in
struct SOpenGLCacheEntry
{
	SBitmap Bitmap;
	TBool ClampImage;
	TInt FramesLeftInCache;
	SGraphicsEngineTextureId& TextureIdRef;
	CString* aImageIdentifier;
};

class CGraphicsEngine
{
public:
	//returns a graphics Engine that draws directly to framebuffer of the application Window
	static CGraphicsEngine* CreateGraphicsEngineToWindow(TGraphicsEngineDisplayDepth aDisplayDepth, TGraphicsEngineTextureConfiguration aTextureConfig, TUint aVirtualScreenWidth, TUint aVirtualScreenHeight);
	~CGraphicsEngine();

	//resizes the drawing surface of the OpenGL instance, the X and Y offset can be used to move the drawing location around on the destination surface
	//also adjust the projection to the width and height converted to TIntFloat and centers it to the 0,0 location
	void AdjustDrawingSurfaceSize(TInt aWidthInPixels, TInt aHeightInPixels, TInt aXOffset, TInt aYOffset);
	//centers the View to the given location, view can also be zoomed out or zoomed in
	void CenterViewToLocation(TPointIntFloat aLocation);
	void CenterViewToLocation(TPointIntFloat aLocation, TFloat aZoomFactorHorizontal, TFloat aZoomFactorVertical);
	//checks all OpenGl Entries and removes those that have expired
	void CheckOpenGlCacheForExpiredEntries();
	//clears the entire screen with the current colour
	void ClearScreen();
	//converts a bitmap to a OpenGL2D texture, performs masking by using default aScanLine Colour and don't draw Pixel Colour
	void ConvertBitmapImageTo2DTexture(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId);
	//converts a bitmap to a OpenGL2D texture, performs masking by using aScanLine Colour and don't draw Pixel Colour
	void ConvertBitmapImageTo2DTexture(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, SBitmapColour aScanLineColour, SBitmapColour aDontDrawPixelColour, const char* aImageId);
	//starts reading the bitmap values from line 1, using default values
	void ConvertBitmapImageTo2DTextureWithReadingStartingByDefault(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageLocation);
	//starts reading the bitmap values from line 1
	void ConvertBitmapImageTo2DTextureWithReadingStartingByDefault(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, SBitmapColour aScanLineColour, SBitmapColour aDontDrawPixelColour, const char* aImageId);
	//transforms a Bitmap to an OpenGl2D texture without alpha masking, all pixels will have a valid alpha value
	void ConvertBitmapImageTo2DTextureWithoutMasking(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId);
	//just convert a PNG image to texture without any major modifications
	void ConvertPNGTo2DTexture(SGraphicsEngineTextureId& aTextureId, SBitmap& aBitmap, TBool aClampImage, const char* aImageId);
	//copies the OpenGL surface to the destination surface
	void CopyDrawingSurfaceToDestinationSurface();
	//creates an GraphicsTextureId for use with the Graphics Environment texture functions
	SGraphicsEngineTextureId CreateGraphicsTextureId();
	//deletes all GraphicTexture Ids that have been loaded into the Graphics Engine that been loaded through the texture load functions
	void DeleteAllGraphicsTextureIds();
	//should be called if a Texture Is not Needed anymore, frees it from the OpenGL context, reupload entries and OpenGL cache
	void DeleteGraphicsTextureId(SGraphicsEngineTextureId& aTextureIdRef);
	//destroys the current OpenGl context, the TextureReupload array will stay intact and might be used in the new context to reupload the lost textures
	void DestroyOpenGlContext();
	//Draws a line in OpenGL coordinate space, uses the current OpenGL colour
	void DrawLine(const TPointIntFloat& aPoint1, const TPointIntFloat& aPoint2);
	//Draws Several connected lines, starting with the first point in the array
	void DrawLines(CPointerArray<TPointIntFloat>& aDrawPoints);
	//draw a Rect with the given Coordinates, outlines it
	void DrawRect(const TRectIntFloat& aRect);
	//draws a Texture with the given Vertex, Text coordniates, colours(Including Alpha, NOT YEY) and textureId
	//should pass an array of 4 vertexes each. Actually delays drawing to batch more calls with the same texture
	void DrawTextureBatched(const SPolygonSquare& aSquarePolygon, GLuint aTextureId);
	//makes the draw call to the GPU immedieatly and does not try to batch several calls together, less efficient but avoids problems with state changes
	void DrawTextureImmediately(const SPolygonSquare& aSquarePolygon, GLuint aTextureId);
	//draw a Rect with the given Coordinates, fills it with the current colour
	void FillRect(const TRectIntFloat& aRect);
	//flushes the batched textures to screen using the current bound TextureId
	void FlushBatchedTexturesToScreen();
	//returns the ImageIdentifier for the given Texture cached
	const CString* GetImageIdentifier(SGraphicsEngineTextureId& aTextureId);
	//returns the real dimensions of the Screen In Pixel units, this device dependent
	TSize GetRealDimensionsOfScreenInPixels() const;
	//returns the TextureId for the given ImadeIdentifier
	SGraphicsEngineTextureId* GetTextureId(const char* aImageIdentifier);
	//return the virtual dimensions of the Screen, virtual means that it is always the same no matter what the real pixel resolution of the Screen is
	TSizeIntFloat GetVirtualDimensionsOfScreen() const;
	//return the virtual dimensions but scaled down to TInts, virtual means that it is always the same no matter what the real pixel resolution of the Screen is
	TSize GetVirtualDimensionsOfScreenAsInts() const;
	//loads the identity matrix for the currently bound matrix
	void LoadIdentityMatrix();
	//--------------------------- NATIVE METHOD ------------------------//
	//Should load the given bitmap from Disk and return a structer to it
	static SBitmap LoadBitmapImageFromDisk(const char* aPath);
	//Pops the current Matrix from the stack
	void PopMatrix();
	//Pushes the current Matrix to the stack
	void PushMatrix();
	//Recreates the OpenGL context with the settings from a previous context
	void RecreateOpenGlContext();
	//Rotates the current bound matrix by the given angle in degrees over the given axis
	void RotateMatrix(TFloat aAngle, TFloat aX, TFloat aY, TFloat aZ);
	//updates the Expire counter again, so that texture will not be removed from OpenGL memory
	SOpenGLCacheEntry* RefreshOpenGlCacheEntry(SGraphicsEngineTextureId& TextureIdRef, SOpenGLCacheEntry* aLikelyEntry);
	//removes all textures from the GPU that have been cached in main memory
	void RemoveTexturesFromGPUThatHaveBeenCached();
	//scales current bound matrix over the given axis
	void ScaleMatrix(TFloat aX, TFloat aY, TFloat aZ);
	//set the current Clear colour in the graphics environment
	void SetClearColour(const TColour& aColour); //uses always full alpha Channel
	void SetClearColourInt(TInt aRedValue, TInt aGreenValue, TInt aBlueValue, TInt aAlphaValue);
	void SetClearColourFloat(TFloat aRedValue, TFloat aGreenValue, TFloat aBlueValue, TFloat aAlphaValue);
	//set the current Draw colour in the graphics environment
	void SetDrawColour(const TColour& aColour); //uses always full alpha Channel
	void SetDrawColourInt(TInt aRedValue, TInt aGreenValue, TInt aBlueValue, TInt aAlphaValue);
	void SetDrawColourFloat(TFloat aRedValue, TFloat aGreenValue, TFloat aBlueValue, TFloat aAlphaValue);
	//sets graphics Settings changes, such as Alpha text and cull face
	void SetGraphicsEngineSettings(GLenum aSetting, TBool aEnable);
	//sets the current Matrix Mode, either Modelview or Projection that will be modified by subsequent transformation calls
	void SetMatrixMode(GLenum aMatrixMode);
	//translate the matrix by the given x, y and z coordinates
	void TranslateMatrix(TIntFloat aX, TIntFloat aY, TIntFloat aZ);

protected:

	CGraphicsEngine(TGraphicsEngineDisplayDepth aDisplayDepth, TGraphicsEngineTextureConfiguration aTextureConfig, TUint aVirtualScreenWidth, TUint aVirtualScreenHeight);
	void SetUpWindowDrawingSurface();
	//Adds an Entry into the OpenGL cache array
	void AddOpenGlCacheEntry(TUint8* aData, SGraphicsEngineTextureId& aTextureId, TBool aClampImage, TInt aWidthInPixels, TInt aHeightInPixels, const char* aImageIdentifier);
	//scans through the Bitmap to adjust the colour value at borders of drawable pixels, this is done remove problems with OpenGLs Linear mapping method
	//if this is not done images can have black borders
	void AdjustBorderPixels(TUint8* aData, TInt aWidthInPixels, TInt aHeightInPixels);
	//deletes the OpenGlTexture From the GPU if it resides on the GPU, if not it does nothing. Also resets the OpenGLId back to 0
	void DeleteTextureFromGPU(GLuint& aOpenGLId);
	//loads an array of pre-converted OpenGL bytes into OpenGL memory bound to the TextureId with given dimensions
	void LoadTextureDataIntoOpenGlMemory(TUint8* aData, GLuint aTextureId, TBool aClampImage, TInt aWidth, TInt aHeight);
	//----------------NATIVE METHOD --------------------------//
	static NativeWindow GetNativeWindow(); //has to be implemetned on the native side of the platform
	//destroys a particular OpenGlCacheEntry with the given OpenGlTextureId
	void RemoveOpenGlCacheEntry(const TUint64& aGraphicsEngineId);

	EGLDisplay iDeviceDisplay; //keeps track of the device the device display the application is running on
	EGLConfig iGraphicsConfiguration; //keeps track of the graphcis configuration
	EGLSurface iDrawingSurface; //surface is used by openGl to draw to
	EGLContext iGraphicsContext;//context that keeps track of the state information for drawing pipeline, might have several ones
	TGraphicsEngineDisplayDepth iDisplayDepth;
	TGraphicsEngineTextureConfiguration iTextureDepth;
	//these keep track of the actual Screen Width and Height, dependent on the device running
	TInt iActualScreenWidth;
	TInt iActualScreenHeight;
	//these are virtual Width and Height measurements, independent of the device. An Application will make use of these without the need to know the actual dimensions
	TInt iVirualScreenWidth;
	TInt iVirualScreenHeight;
	//keeps track of what textures are currently in OpenGL memory
	CPointerArray<SOpenGLCacheEntry>* iOpenGlCacheArray;
	//used to give away new unique GraphicTexture Ids, will be incremented for every new GraphicsEngineTextureId
	static TUint64 GraphicEngineTextureCounter;

	//needed for texture batching
	SPolygonSquare iPolygonSquareBatch [GRAPHICS_MAX_NUMBER_OF_BATCHED_TEXTURE_CALLS]; //can't have more than 255, it is the limit enforced by OpenGL
	GLushort iTextureIndicies [255];
	GLuint iBoundTextureId;
	TInt iNumberOfTextureCallsBatched; //keeps track how many texture calls have been batched to avoid overflow(Too many batched) and unessary drawing if none have been batched


};

#endif /* CGRAPHICSENGINE_H_ */
