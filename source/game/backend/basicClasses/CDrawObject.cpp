/*
 ============================================================================
 Name		: DrawObject.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CDrawObject implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CDrawObject.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"

TInt CDrawObject::CurrentZValueOffset = 0;

CDrawObject::CDrawObject(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer)
        {
          iZCor = AdjustZCoordinate(aZCor);
          iTextureObject=aTextureObject;
          iAnimationPlayer=aAnimationPlayer;
          iDoNotDraw=false;
        }

CDrawObject::~CDrawObject()
	{
	  if(iTextureObject)
	    {
	      delete iTextureObject;
	      iTextureObject=NULL;
	    }
	  if(iAnimationPlayer)
	    {
	      delete iAnimationPlayer;
	      iAnimationPlayer=NULL;
	    }
	}

//------------------------ functions ---------------------------

TInt CDrawObject::AdjustZCoordinate(TInt aZValue)
{
	CDrawObject::CurrentZValueOffset++;
	if(CDrawObject::CurrentZValueOffset == Z_VALUE_CATEGORY_RANGE)
		CDrawObject::CurrentZValueOffset = 0;

	return aZValue+CDrawObject::CurrentZValueOffset;
}

TUint CDrawObject::GetAlpha()
{
	TUint lAlphaValue = 0;

	if(iTextureObject != NULL)
	{
		//since all vertexes use the same alpha, which vertex we use does not matter
		lAlphaValue = iTextureObject->GetPolygon().TopLeftCorner.Colour.Alpha;
	}

	return lAlphaValue;
}

CTextureObject* CDrawObject::GetTextureObject()
{
	return iTextureObject;
}

void CDrawObject::SetDoNotDraw(TBool aValue)
{
  iDoNotDraw=aValue;
}

void CDrawObject::Draw()
{
  if(!iDoNotDraw && iTextureObject)
    {
      iTextureObject->DrawArrays();//uses the currently assianged texture
    }
}

void CDrawObject::ChangeZValue(TInt aChange)
{
  iZCor+=aChange;
}

void CDrawObject::DrawAlways()
{
  iTextureObject->DrawArrays();
}

void CDrawObject::SetAlpha(TInt aAlphaValue)
{
	if(iTextureObject != NULL)
	{
		iTextureObject->SetAlpha((aAlphaValue * 255) / 100);
	}
}

void CDrawObject::DrawAtPositionWithValues(TPointIntFloat& aPosition,TIntFloat lWidth,TIntFloat lHeight)
{
  iTextureObject->DrawAtPositionWithValuesWithDrawArrays(aPosition,lWidth,lHeight);
}

void  CDrawObject::DrawObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
  aOutputStream.WriteInt32(iZCor);
  aOutputStream.WriteInt32(iDoNotDraw);
  //save texture object if is exists
 if(iTextureObject != NULL)
    {
    aOutputStream.WriteInt32(1);
    iTextureObject->SaveOnDisk(aOutputStream);
    }
  else
    aOutputStream.WriteInt32(0);
  //save animation player if it exists
  if(iAnimationPlayer != NULL)
    {
    aOutputStream.WriteInt32(1);
    iAnimationPlayer->SaveOnDisk(aOutputStream);
    }
  else
    aOutputStream.WriteInt32(0);
}

void CDrawObject::DrawObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
  iZCor = aReadStream.ReadInt32();
  iDoNotDraw = aReadStream.ReadInt32();

  TInt lTextureObjectHasBeenSaved = aReadStream.ReadInt32();
  if(lTextureObjectHasBeenSaved == 1)
    iTextureObject->LoadFromDisk(aReadStream);

  TInt lAnimationPlayerExists = aReadStream.ReadInt32();
  if(lAnimationPlayerExists == 1)
    iAnimationPlayer->LoadFromDisk(aReadStream,iTextureObject);
}
