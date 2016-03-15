/*
 * CGraphicsFont.cpp
 *
 *  Created on: Jul 2, 2012
 *      Author: dstoll
 */

#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/core/utility/CAssetDirectory.h"
#include "includes/core/CGame.h"

CGraphicsFont::CGraphicsFont(CGraphicsEngine& aGraphicsEngineRef)
:iGraphicsEngineRef(aGraphicsEngineRef)
{
	iFontMemory = NULL;

	iDrawColour[0] = GRAPHICS_FONT_DEFAULT_COLOUR_RED;
	iDrawColour[1] = GRAPHICS_FONT_DEFAULT_COLOUR_GREEN;
	iDrawColour[2] = GRAPHICS_FONT_DEFAULT_COLOUR_BLUE;
	iDrawColour[3] = GRAPHICS_FONT_DEFAULT_COLOUR_ALPHA;
}

CGraphicsFont::~CGraphicsFont()
{
	if (iFontMemory != NULL)
	{
		FT_Done_Face(iFontFace);
		FT_Done_FreeType(iFontLibrary);

		delete[] iFontMemory;
		iFontMemory = NULL;

		//update memory stats
		//reset back to 0
		CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForFontFile(-CGame::Game->iMemoryUsageTracker->iKBytesUsedInRAMForFontFile * 1000);
	}

	if(iCharacterToFontMappings != NULL)
	{
		//delete character Font Mapping as well
		RemoveAllCharacterFontMappings();

		delete iCharacterToFontMappings;
		iCharacterToFontMappings = NULL;
	}
}

CGraphicsFont* CGraphicsFont::New(CGraphicsEngine& aGraphicsEngineRef, const char* aFontFileLocation)
{
	CGraphicsFont* lSelf = new CGraphicsFont(aGraphicsEngineRef);
	lSelf->Construct(aFontFileLocation);
	return lSelf;
}

void CGraphicsFont::Construct(const char* aFontFileLocation)
{
	FT_Init_FreeType(&iFontLibrary);

	TInt lFontFileSizeInBytes = CAssetDirectory::GetAssetFileSizeInBytes(aFontFileLocation);
	iFontMemory = CAssetDirectory::ReadAssetFile(aFontFileLocation);
	//update memory stats
	CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForFontFile(lFontFileSizeInBytes);

	FT_New_Memory_Face(iFontLibrary, iFontMemory, lFontFileSizeInBytes, 0, &iFontFace);

	FT_Set_Pixel_Sizes(iFontFace, 0, GRAPHICS_FONT_BASE_HEIGHT_IN_PIXELS);

	iCharacterToFontMappings = CPointerArray<SCharacterToFontMapping>::New();
}

//---------------- functions -------------------//

TColour CGraphicsFont::CreateColour(const TUint8& aRed,const TUint8& aGreen,const TUint8& aBlue,const TUint8& aAlpha)
{
	TColour lColour = 0;
	lColour += aRed << 24; //shift by 2 bytes
	lColour += aGreen << 16; //shift by 1 byte
	lColour += aBlue << 8; //shift by 0 bytes
	lColour += aAlpha;

	return lColour;
}

SFont CGraphicsFont::CreateFont(TInt aHeightAndDescentOfFont)
{
	SFont lFont;
	lFont.HeightAndDescentOfFont = TIntFloat::Convert(aHeightAndDescentOfFont);
	//since MAX height for each character is always the same, this should work to scale the Font to the desired height
	lFont.ScaleFactor =  TIntFloat::Convert(aHeightAndDescentOfFont) / TIntFloat::Convert(GRAPHICS_FONT_BASE_HEIGHT_IN_PIXELS);

	return lFont;
}

SFont CGraphicsFont::CreateFontThatFitsHorizontalString(const CString* aString, TInt aTargetWidth, TInt aTargetHeight)
{
	SFont lBestFont = {TIntFloat::Convert(0), TIntFloat::Convert(0)};

	for(TInt lHeightOfFont = aTargetHeight; lHeightOfFont > 0 ; lHeightOfFont -= 1)
	{
		lBestFont = CreateFont(lHeightOfFont);

		if(GetWidthOfString(lBestFont, aString) <= aTargetWidth)
		{
			//found font that works
			break;
		}
	}

	return lBestFont;
}

SFont CGraphicsFont::CreateFontThatFitsHorizontalStrings(CPointerArray<CString>* aStrings, TInt aTargetWidth, TInt aTargetHeight)
{
	SFont lBestFont = {TIntFloat::Convert(0), TIntFloat::Convert(0)};
	if(aStrings->GetCount() == 0)
		return lBestFont;

	for(TInt lIndex = 0; lIndex < aStrings->GetCount(); lIndex++)
	{
		CString* lString = aStrings->Get(lIndex);
		TBool lWorksWithAllStrings = true;
		lBestFont = CreateFontThatFitsHorizontalString(lString, aTargetWidth, aTargetHeight);

		//check if this font also works with the other Strings
		for(TInt lIndex2 = 0; lIndex2 < aStrings->GetCount(); lIndex2++)
		{
			if(GetWidthOfString(lBestFont, aStrings->Get(lIndex2)) > aTargetWidth)
			{
				lWorksWithAllStrings = false;
				break;
			}
		}

		if(lWorksWithAllStrings == true)
		{
			//found is good enough font that works with all strings
			break;
		}
	}

	return lBestFont;
}

SFont CGraphicsFont::CreateFontThatFitsHorizontalStrings(CString** aStrings, TInt aNumberOfStrings, TInt aTargetWidth, TInt aTargetHeight)
{
	CPointerArray<CString>* lStringArray = CPointerArray<CString>::New();

	for(TInt lIndex = 0; lIndex < aNumberOfStrings; lIndex++)
	{
		lStringArray->Append(aStrings[lIndex]);
	}

	SFont lBestFont = CreateFontThatFitsHorizontalStrings(lStringArray, aTargetWidth, aTargetHeight);
	delete lStringArray;

	return lBestFont;
}

SFont CGraphicsFont::CreateFontThatFitsVerticalString(const CString* aString, TInt aTargetWidth, TInt aTargetHeight)
{
	SFont lBestFont = {TIntFloat::Convert(0), TIntFloat::Convert(0)};

	for(TInt lWidthOfFont = aTargetWidth; lWidthOfFont > 0 ; lWidthOfFont -= 1)
	{
		lBestFont = CreateFont(lWidthOfFont);

		if(GetHeightOfVerticalString(lBestFont, aString) <= aTargetHeight)
		{
			//found font that works
			break;
		}
	}

	return lBestFont;
}

SFont CGraphicsFont::CreateFontThatFitsVerticalStrings(CPointerArray<CString>* aStrings, TInt aTargetWidth, TInt aTargetHeight)
{
	SFont lBestFont = {TIntFloat::Convert(0), TIntFloat::Convert(0)};
	if(aStrings->GetCount() == 0)
		return lBestFont;

	for(TInt lIndex = 0; lIndex < aStrings->GetCount(); lIndex++)
	{
		CString* lString = aStrings->Get(lIndex);
		TBool lWorksWithAllStrings = true;
		lBestFont = CreateFontThatFitsVerticalString(lString, aTargetWidth, aTargetHeight);

		//check if this font also works with the other Strings
		for(TInt lIndex2 = 0; lIndex2 < aStrings->GetCount(); lIndex2++)
		{
			if(GetHeightOfVerticalString(lBestFont, aStrings->Get(lIndex2)) > aTargetHeight)
			{
				lWorksWithAllStrings = false;
				break;
			}
		}

		if(lWorksWithAllStrings == true)
		{
			//found is good enough font that works with all strings
			break;
		}
	}

	return lBestFont;
}

SFont CGraphicsFont::CreateFontThatFitsVerticalStrings(CString** aStrings, TInt aNumberOfStrings, TInt aTargetWidth, TInt aTargetHeight)
{
	CPointerArray<CString>* lStringArray = CPointerArray<CString>::New();

	for(TInt lIndex = 0; lIndex < aNumberOfStrings; lIndex++)
	{
		lStringArray->Append(aStrings[lIndex]);
	}

	SFont lBestFont = CreateFontThatFitsVerticalStrings(lStringArray, aTargetWidth, aTargetHeight);
	delete lStringArray;

	return lBestFont;
}

void CGraphicsFont::DrawStringToScreenHorizontally(TPoint aLocation, const CString* aString)
{
	//since state changes
	iGraphicsEngineRef.FlushBatchedTexturesToScreen();

	//need to setup the texture environment
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE); //tell texture environment to use texture Environment colour and texture
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_CONSTANT);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, iDrawColour); //set texture environment colour

	//use a blending mode for the for the Alpha values
	glDisable(GL_ALPHA_TEST);

	//also change the project matrix this way, drawing the String will always be relative to the bottom left corner
	TSizeIntFloat lVirtualDimensions = iGraphicsEngineRef.GetVirtualDimensionsOfScreen();
	glMatrixMode(GL_PROJECTION);
	GLfloat lProjectionMatrixValues [16];
	glGetFloatv(GL_PROJECTION_MATRIX, lProjectionMatrixValues);
	glLoadIdentity();
	glOrthox(0, lVirtualDimensions.iWidth.GetIntInBaseIntFloat(), //left X, right X
					 0, lVirtualDimensions.iHeight.GetIntInBaseIntFloat(), //left Y, right Y
					 TIntFloat::Convert(1).GetIntInBaseIntFloat(),
					 TIntFloat::Convert(-1).GetIntInBaseIntFloat() ); //near Z, far Z

	//draw String
	TPointIntFloat lDrawLocation (TIntFloat::Convert(aLocation.iX), TIntFloat::Convert(aLocation.iY));
	for(TInt lIndex = 0; lIndex < aString->GetLength(); lIndex++)
	{
		TInt64 lCharacterCode = aString->GetCharacterCodeAsUtf32(lIndex);
		TIntFloat lWidthOfScaledCharacter = DrawCharacterToScreen(lDrawLocation, lCharacterCode);
		lDrawLocation.iX += lWidthOfScaledCharacter;
		lDrawLocation.iX += TIntFloat::Convert(GRAPHICS_FONT_EXTRA_WIDTH_BETWEEN_CHARACTERS) * iDrawFont.ScaleFactor;
	}

	//since state changes
	iGraphicsEngineRef.FlushBatchedTexturesToScreen();

	//set back to the default mode
	glLoadMatrixf(lProjectionMatrixValues);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_ALPHA_TEST);

	//set texture environment back to normal
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_ALPHA,  GL_PRIMARY_COLOR);
}

void CGraphicsFont::DrawStringToScreenVertically(TPoint aLocation,const CString* aString)
{
	//since state changes
	iGraphicsEngineRef.FlushBatchedTexturesToScreen();

	//need to setup the texture environment
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE); //tell texture environment to use texture Environment colour and texture
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_CONSTANT);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, iDrawColour); //set texture environment colour

	//use a blending mode for the for the Alpha values
	glDisable(GL_ALPHA_TEST);

	//also change the project matrix this way, drawing the String will always be relative to the bottom left corner
	TSizeIntFloat lVirtualDimensions = iGraphicsEngineRef.GetVirtualDimensionsOfScreen();
	glMatrixMode(GL_PROJECTION);
	GLfloat lProjectionMatrixValues [16];
	glGetFloatv(GL_PROJECTION_MATRIX, lProjectionMatrixValues);
	glLoadIdentity();
	glOrthox(0, lVirtualDimensions.iWidth.GetIntInBaseIntFloat(), //left X, right X
					 0, lVirtualDimensions.iHeight.GetIntInBaseIntFloat(), //left Y, right Y
					 TIntFloat::Convert(1).GetIntInBaseIntFloat(),
					 TIntFloat::Convert(-1).GetIntInBaseIntFloat() ); //near Z, far Z

	//draw String
	TPointIntFloat lDrawLocation (TIntFloat::Convert(aLocation.iX), TIntFloat::Convert(aLocation.iY));
	for(TInt lIndex = 0; lIndex < aString->GetLength(); lIndex++)
	{
		TInt64 lCharacterCode = aString->GetCharacterCodeAsUtf32(lIndex);
		TIntFloat lHeightOfScaledCharacter = DrawCharacterToScreenVertically(lDrawLocation, lCharacterCode);
		lDrawLocation.iY -= lHeightOfScaledCharacter;
		lDrawLocation.iY -= TIntFloat::Convert(GRAPHICS_FONT_EXTRA_WIDTH_BETWEEN_CHARACTERS) * iDrawFont.ScaleFactor;
	}

	//since state changes
	iGraphicsEngineRef.FlushBatchedTexturesToScreen();

	//set back to the default mode
	glLoadMatrixf(lProjectionMatrixValues);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_ALPHA_TEST);

	//set texture environment back to normal
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_ALPHA,  GL_PRIMARY_COLOR);
}

TInt CGraphicsFont::GetHeightOfVerticalString(const SFont& aFont, const CString* aString)
{
	TIntFloat lTotalHeight;

	//go through the entire array and add up the width of each character
	for(TInt lIndex = 0; lIndex < aString->GetLength(); lIndex++)
	{
		TInt64 lCharacterCode = aString->GetCharacterCodeAsUtf32(lIndex);
		if(lCharacterCode == GRAPHICS_FONT_CHARACTER_CODE_FOR_WHITESPACE)
		{
			lTotalHeight += aFont.HeightAndDescentOfFont / TIntFloat::Convert(4);
		}
		else
		{
			SCharacterToFontMapping* lCharacterFontMapping = GetCharacterToFontMappingEntry(lCharacterCode);
			lTotalHeight += lCharacterFontMapping->HeightAndDescentOfCharacter * aFont.ScaleFactor;
		}

		//add height between characters if it is not the last character in the string
		if(lIndex + 1 !=  aString->GetLength())
		{
			lTotalHeight += TIntFloat::Convert(GRAPHICS_FONT_EXTRA_WIDTH_BETWEEN_CHARACTERS) * aFont.ScaleFactor;
		}
	}

	return lTotalHeight.GetIntInBaseInt();
}

TInt CGraphicsFont::GetWidthOfString(const SFont& aFont, const CString* aString)
{
	TIntFloat lTotalWidth;

	//go through the entire array and add up the width of each character
	for(TInt lIndex = 0; lIndex < aString->GetLength(); lIndex++)
	{
		TInt64 lCharacterCode = aString->GetCharacterCodeAsUtf32(lIndex);
		if(lCharacterCode == GRAPHICS_FONT_CHARACTER_CODE_FOR_WHITESPACE)
		{
			lTotalWidth += aFont.HeightAndDescentOfFont / TIntFloat::Convert(4);
		}
		else
		{
			SCharacterToFontMapping* lCharacterFontMapping = GetCharacterToFontMappingEntry(lCharacterCode);
			lTotalWidth += lCharacterFontMapping->WidthOfCharacter * aFont.ScaleFactor;
		}

		//add width between characters if it is not the last character in the string
		if(lIndex + 1 !=  aString->GetLength())
		{
			lTotalWidth += TIntFloat::Convert(GRAPHICS_FONT_EXTRA_WIDTH_BETWEEN_CHARACTERS) * aFont.ScaleFactor;
		}
	}

	return lTotalWidth.GetIntInBaseInt();
}

void CGraphicsFont::RemoveAllCharacterFontMappings()
{
	//update memory stats, reset back to 0
	CGame::Game->iMemoryUsageTracker->UpdateGPUUsedForTextRendering(-CGame::Game->iMemoryUsageTracker->iKBytesUsedInGPUForTextRendering * 1000);

	for(TInt lIndex = 0; lIndex < iCharacterToFontMappings->GetCount(); lIndex++)
	{
		if(iCharacterToFontMappings->Get(lIndex)->TextureObject != NULL)
		{
			delete iCharacterToFontMappings->Get(lIndex)->TextureObject;
			iCharacterToFontMappings->Get(lIndex)->TextureObject = NULL;
		}

		GLuint& lOpenGlId = iCharacterToFontMappings->Get(lIndex)->TextureId.OpenGlId;
		glDeleteTextures(1, &lOpenGlId);
		iGraphicsEngineRef.DeleteGraphicsTextureId(iCharacterToFontMappings->Get(lIndex)->TextureId);
	}

	iCharacterToFontMappings->ClearAndDestroy();
}

void CGraphicsFont::SetColour(const TColour& aColour)
{
	SetColourInt((aColour & (0xFF000000)) >> 24,(aColour & (0x00FF0000))  >> 16, (aColour & (0x0000FF00)) >> 8,  aColour & (0x000000FF)); //using different masks to extract the different portions, 1 byte is unused
}

void CGraphicsFont::SetColourBlack()
{
	SetColourFloat(0, 0, 0, 1);
}

void CGraphicsFont::SetColourBlue()
{
	SetColourFloat(0, 0, 1, 1);
}

void CGraphicsFont::SetColourFloat(GLfloat aRedValue,GLfloat aGreenValue,GLfloat aBlueValue, GLfloat aAlphaValue)
{
	iDrawColour[0] = aRedValue;
	iDrawColour[1] = aGreenValue;
	iDrawColour[2] = aBlueValue;
	iDrawColour[3] = aAlphaValue;
}

void CGraphicsFont::SetColourGreen()
{
	SetColourFloat(0, 1, 0, 1);
}

void CGraphicsFont::SetColourGrey()
{
	SetColourFloat(0.6666f, 0.6666f, 0.6666f, 1);
}

void CGraphicsFont::SetColourSkyBlue()
{
	SetColourInt(141, 182, 235, 1);
}

void CGraphicsFont::SetColourInt(TInt aRedValue,TInt aGreenValue,TInt aBlueValue, TInt aAlphaValue)
{
	iDrawColour[0] = (GLfloat)aRedValue / 255;
	iDrawColour[1] = (GLfloat)aGreenValue / 255;
	iDrawColour[2] = (GLfloat)aBlueValue / 255;
	iDrawColour[3] = (GLfloat)aAlphaValue / 255;
}

void CGraphicsFont::SetColourRed()
{
	SetColourFloat(1, 0, 0, 1);
}

void CGraphicsFont::SetColourWhite()
{
	SetColourFloat(1, 1, 1, 1);
}

//sets what font is used for drawing
void CGraphicsFont::SetFont(const SFont& aFontForDrawing)
{
	iDrawFont = aFontForDrawing;
}

SCharacterToFontMapping* CGraphicsFont::AddSingleCharacter(const TInt64& aCharacterCode)
{
	//find glyph and convert to anti-aliased bitmap
	FT_Load_Char(iFontFace, aCharacterCode, FT_LOAD_RENDER);

	//get reference to the bitmap
	FT_Bitmap& lBitmapForGlyph = iFontFace->glyph->bitmap;

	//now need to load the bitmap for the glyph into OpenGL
	TInt lWidthOfTexture = CMath::GetNextGreatestMemberOfPower(lBitmapForGlyph.width, 2);
	TInt lHeightOfTexture = CMath::GetNextGreatestMemberOfPower(lBitmapForGlyph.rows, 2);

	//allocate memory for the Character
	TUint8* lTextureMemory = new TUint8[lWidthOfTexture * lHeightOfTexture]; //need 1 byte for the alpha Value

	//copy over the values of GlyphBitmap into the Texture Memory
	for(TInt lHeightIndex = 0; lHeightIndex < lHeightOfTexture; lHeightIndex++)
	{
		for(TInt lWidthIndex = 0; lWidthIndex < lWidthOfTexture; lWidthIndex++)
		{
			TInt lIndexLocation = lWidthIndex + (lHeightIndex * lWidthOfTexture);

			//find out the alpha value
			if(lBitmapForGlyph.width <= lWidthIndex || lBitmapForGlyph.rows <= lHeightIndex)
			{
				lTextureMemory[lIndexLocation] = 0; //the luminance value is always full on
			}
			else
			{
				lTextureMemory[lIndexLocation] = lBitmapForGlyph.buffer[lWidthIndex + lHeightIndex * lBitmapForGlyph.width];
			}
		}
	}

	//texture contains now the data of the glyph, now we just need to load the texutre into OpenGL memory
	SCharacterToFontMapping* lNewMapping = new SCharacterToFontMapping;
	lNewMapping->CharacterCode = aCharacterCode;
	lNewMapping->TextureId = iGraphicsEngineRef.CreateGraphicsTextureId();
	lNewMapping->WidthOfCharacter = TIntFloat::Convert(lBitmapForGlyph.width);
	lNewMapping->HeightAndDescentOfCharacter =  TIntFloat::Convert(lBitmapForGlyph.rows);

	//need to find the decent as well
	TInt lDescentFreeTypeUnits = iFontFace->glyph->metrics.height - iFontFace->glyph->metrics.horiBearingY;
	lNewMapping->DescentOfCharacter = TIntFloat::Convert(lDescentFreeTypeUnits) /  TIntFloat::Convert(64); //convert to pixels, freetype uses 1/64th of pixels units)

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lNewMapping->TextureId.OpenGlId);///all 2D texture operations will affect this texture object
	//loads the it into in internal OpenGl Texture attached to the textureId
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, lWidthOfTexture, lHeightOfTexture, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid*) lTextureMemory);
	//update memory stats
	CGame::Game->iMemoryUsageTracker->UpdateGPUUsedForTextRendering(lWidthOfTexture * lHeightOfTexture * 4); //assume that OpenGL uses 4Bytes internally, instead of just 1

	//set up how the texturing will be done for this texture
	//need to clamp otherwise GL_Linea(linear interpolation) causes problems with some characters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//use linear interpolation for best quality
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete[] lTextureMemory; //not needed anymore

	//create a texture object for drawing this character, position, width and height don't matter since it changes everytime we do a draw
	lNewMapping->TextureObject = CTextureObject::New(TPoint(0,0), &lNewMapping->TextureId, lBitmapForGlyph.width, lBitmapForGlyph.rows, 0,
																									0, (TFloat) lBitmapForGlyph.width / lWidthOfTexture, 0, (TFloat) lBitmapForGlyph.rows / lHeightOfTexture);
	//add the mapping to end of the list
	iCharacterToFontMappings->Append(lNewMapping);

	//return the new mapping, so we can use it right away
	return lNewMapping;
}

TIntFloat CGraphicsFont::DrawCharacterToScreen(TPointIntFloat aDrawLocation, const TInt64& aCharacterCode)
{
	if(GRAPHICS_FONT_CHARACTER_CODE_FOR_WHITESPACE == aCharacterCode)
	{
		//since white space does not have a texture or any width by default, we need to add this special case
		return iDrawFont.HeightAndDescentOfFont / TIntFloat::Convert(4); //fourth looks around right
	}
	else
	{
		SCharacterToFontMapping* lMapping = GetCharacterToFontMappingEntry(aCharacterCode);

		//make adjustment for descent, that way all characters are all on the same baseline
		aDrawLocation.iY -= lMapping->DescentOfCharacter * iDrawFont.ScaleFactor;
		lMapping->TextureObject->SetNewLowerLeftCoordinate(aDrawLocation);
		//need to adjust the width and height of the character for this particular font
		TIntFloat lWidthOfCharacter = lMapping->WidthOfCharacter * iDrawFont.ScaleFactor;
		TIntFloat lHeightOfCharacter = lMapping->HeightAndDescentOfCharacter * iDrawFont.ScaleFactor;
		lMapping->TextureObject->SetWidth(lWidthOfCharacter);
		lMapping->TextureObject->SetHeight(lHeightOfCharacter);
		lMapping->TextureObject->DrawArrays();

		//return width so caller can adjust its next draw location
		return lWidthOfCharacter;
	}
}

TIntFloat CGraphicsFont::DrawCharacterToScreenVertically(TPointIntFloat aDrawLocation, const TInt64& aCharacterCode)
{
	if(GRAPHICS_FONT_CHARACTER_CODE_FOR_WHITESPACE == aCharacterCode)
	{
		//since white space does not have a texture or any width by default, we need to add this special case
		return iDrawFont.HeightAndDescentOfFont / TIntFloat::Convert(4); //fourth looks around right
	}
	else
	{
		SCharacterToFontMapping* lMapping = GetCharacterToFontMappingEntry(aCharacterCode);

		//need to adjust the width and height of the character for this particular font
		TIntFloat lWidthOfCharacter = lMapping->WidthOfCharacter * iDrawFont.ScaleFactor;
		TIntFloat lHeightOfCharacter = lMapping->HeightAndDescentOfCharacter * iDrawFont.ScaleFactor;
		//make adjustment for height
		aDrawLocation.iY -= lHeightOfCharacter;
		//make adjustment to center the String on the horizontal axis
		aDrawLocation.iX += (iDrawFont.HeightAndDescentOfFont -  lWidthOfCharacter) / 2;
		lMapping->TextureObject->SetNewLowerLeftCoordinate(aDrawLocation);
		lMapping->TextureObject->SetWidth(lWidthOfCharacter);
		lMapping->TextureObject->SetHeight(lHeightOfCharacter);
		lMapping->TextureObject->DrawArrays();

		//return height that needs to be skipped over to draw the next character
		return lHeightOfCharacter;
	}
}

SCharacterToFontMapping* CGraphicsFont::GetCharacterToFontMappingEntry(const TInt64& aCharacterCode)
{
	SCharacterToFontMapping* lRequiredMapping = NULL;

	for(TInt lIndex = 0; lIndex < iCharacterToFontMappings->GetCount(); lIndex++)
	{
		if(iCharacterToFontMappings->Get(lIndex)->CharacterCode == aCharacterCode)
		{
			lRequiredMapping = iCharacterToFontMappings->Get(lIndex);
			break;
		}
	}

	if(lRequiredMapping == NULL)
	{
		//was not found, need to add it
		lRequiredMapping = AddSingleCharacter(aCharacterCode);
	}

	return lRequiredMapping;
}
