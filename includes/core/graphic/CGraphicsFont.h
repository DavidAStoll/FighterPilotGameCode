/*
 * CGraphicsFont.h
 *
 *  Created on: Jul 2, 2012
 *      Author: dstoll
 */

#ifndef CGRAPHICSFONT_H_
#define CGRAPHICSFONT_H_

#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/graphic/CTextureObject.h"
#include <includes/core/standardLibrary/StandardLibrary.h>

//free type headers
#include <ft2build.h>
#include FT_FREETYPE_H

struct SFont
{
	TIntFloat ScaleFactor; //the scale factor is used to determine how much the desired characters are scaled to match the desired height
	TIntFloat HeightAndDescentOfFont; //units are in Virtual OpenGl coordinates, scaled down from TIntFloats for convience
};

struct SCharacterToFontMapping
{
	TInt64 CharacterCode;
	CTextureObject* TextureObject;
	SGraphicsEngineTextureId TextureId;
	TIntFloat WidthOfCharacter;
	TIntFloat HeightAndDescentOfCharacter;
	TIntFloat DescentOfCharacter;
};

#define GRAPHICS_FONT_BASE_HEIGHT_IN_PIXELS 128 //each character will be scaled using this height as a base
#define GRAPHICS_FONT_CHARACTER_CODE_FOR_WHITESPACE 32 //free type does not allocate an image for it, so we do a little hack to get spacing working
#define GRAPHICS_FONT_EXTRA_WIDTH_BETWEEN_CHARACTERS 6 //by default some characters are a bit too close to each other
#define GRAPHICS_FONT_DEFAULT_COLOUR_RED 0
#define GRAPHICS_FONT_DEFAULT_COLOUR_GREEN 0
#define GRAPHICS_FONT_DEFAULT_COLOUR_BLUE 0
#define GRAPHICS_FONT_DEFAULT_COLOUR_ALPHA 1

class CGraphicsFont
{
public:

	static CGraphicsFont* New(CGraphicsEngine& aGraphicsEngineRef,const char* aFontFileLocation);
	~CGraphicsFont();

	//creates a Colour object using the three given components
	TColour CreateColour(const TUint8& aRed,const TUint8& aGreen,const TUint8& aBlue,const TUint8& aAlpha);
	//returns a Font object with the desired max HeightAndDescent for the current loaded font, in OpenGl virtual units
	SFont CreateFont(TInt aHeightAndDescentOfFont);
	//returns a Font that fits required specifications with the biggest size possible
	SFont CreateFontThatFitsHorizontalString(const CString* aString, TInt aTargetWidth, TInt aTargetHeight);
	//returns a Font that works for all Strings and makes sure that all Strings fit the required Specifications
	SFont CreateFontThatFitsHorizontalStrings(CPointerArray<CString>* aStrings, TInt aTargetWidth, TInt aTargetHeight);
	SFont CreateFontThatFitsHorizontalStrings(CString** aStrings, TInt aNumberOfStrings, TInt aTargetWidth, TInt aTargetHeight);
	//returns a Font that fits required specifications with the biggest size possible, assumes String will be drawn Vertically
	SFont CreateFontThatFitsVerticalString(const CString* aString, TInt aTargetWidth, TInt aTargetHeight);
	//returns a Font that works for all Strings and makes sure that all Strings fit the required Specifications
	SFont CreateFontThatFitsVerticalStrings(CPointerArray<CString>* aStrings, TInt aTargetWidth, TInt aTargetHeight);
	SFont CreateFontThatFitsVerticalStrings(CString** aStrings, TInt aNumberOfStrings, TInt aTargetWidth, TInt aTargetHeight);
	//draw a String relative to the bottom left corner of the screen, using the current colour and Font
	void DrawStringToScreenHorizontally(TPoint aLocation,const CString* aString);
	//draw a String relative to the Top Left Corner, is drawn Vertically downwards, centered
	void DrawStringToScreenVertically(TPoint aLocation,const CString* aString);
	//returns the Height of the String with given Font if drawn Vertically, units are in Virtual OpenGl coordinates, scaled down from TIntFloats for convience
	TInt GetHeightOfVerticalString(const SFont& aFont, const CString* aString);
	//returns the width of the String with given Font, units are in Virtual OpenGl coordinates, scaled down from TIntFloats for convience
	TInt GetWidthOfString(const SFont& aFont, const CString* aString);
	//deletes all cached characters
	void RemoveAllCharacterFontMappings();
	//set Colour using the lower 3 bytes of the Int value
	void SetColour(const TColour& aColour);
	//changes the font colour to black, 0,0,0
	void SetColourBlack();
	//changes the font colour to blue 0,0,1
	void SetColourBlue();
	//changes the current font colour to the new values
	void SetColourFloat(GLfloat aRedValue,GLfloat aGreenValue,GLfloat aBlueValue, GLfloat aAlphaValue);
	 //changes the font to green 0,1,0
	void SetColourGreen();
	//changes the font colour to grey  0.6666,0.6666,0.6666
	void SetColourGrey();
	//changes the font colour to skyBlue 141, 182, 235
	void SetColourSkyBlue();
	 //changes the current font colour usings Ints, does internally a conversion to floats
	void SetColourInt(TInt aRedValue,TInt aGreenValue,TInt aBlueValue, TInt aAlphaValue);
	//changes the font to red 1,0,0
	void SetColourRed();
	//changes the font colour to white, 1,1,1
	void SetColourWhite();
	//sets what font is used for drawing
	void SetFont(const SFont& aFontForDrawing);

protected:

	CGraphicsFont(CGraphicsEngine& aGraphicsEngineRef);
	void Construct(const char* aFontFileLocation);

	SCharacterToFontMapping* AddSingleCharacter(const TInt64& aCharacterCode);
	TIntFloat DrawCharacterToScreen(TPointIntFloat aLocation, const TInt64& aCharacterCode);
	TIntFloat DrawCharacterToScreenVertically(TPointIntFloat aLocation, const TInt64& aCharacterCode);
	SCharacterToFontMapping* GetCharacterToFontMappingEntry(const TInt64& aCharacterCode);

	FT_Library iFontLibrary;
	FT_Face iFontFace;
	TUint8* iFontMemory;
	TFloat iDrawColour [4]; //3 indexes for RGB, and 1 for A, so A is not used. But OpenGL needs for indexes
	SFont iDrawFont;
	CGraphicsEngine& iGraphicsEngineRef;
	CPointerArray<SCharacterToFontMapping>* iCharacterToFontMappings;
};

#endif /* CGRAPHICSFONT_H_ */
