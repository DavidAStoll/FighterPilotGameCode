/*
 ============================================================================
 Name		: BackgroundImage.cpp
 Author	  :
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBackgroundImage implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CBackgroundImage.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/CHud.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/backend/CPlayer.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CImageStore.h"


CBackgroundImage::CBackgroundImage(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer)
:CDrawObject(aZCor,aTextureObject,aAnimationPlayer)
	{
	iBackgroundTextureWidth = TIntFloat::Convert(BACKGROUND_BASIC_WIDTH);
	iBackgroundTextureHeight = TIntFloat::Convert(BACKGROUND_BASIC_HEIGHT);
	iBackgroundUpperTValue = BACKGROUND_BASIC_DEFAULT_UPPER_T_VALUE;
	}

CBackgroundImage::~CBackgroundImage()
	{
	}

CBackgroundImage* CBackgroundImage::New()
	{
	CBackgroundImage* self = new CBackgroundImage(BACKGROUND_BASIC_Z_VALUE,NULL,NULL);
	self->ConstructL();
	return self;
	}

void CBackgroundImage::ConstructL()
	{
  iTextureObject = CTextureObject::New(
      TPoint(0,0),&CGame::Game->iImageStore->GetImage(EImageIdBackgroundBlueDay),iBackgroundTextureWidth.GetIntInBaseInt(),
      iBackgroundTextureHeight.GetIntInBaseInt(),
      iZCor,
      0,(TFloat) iBackgroundTextureWidth.GetIntInBaseInt() / BACKGROUND_TEXTURE_SIZE,
      BACKGROUND_BASIC_DEFAULT_LOWER_T_VALUE,iBackgroundUpperTValue);//will be changed anyway when the hud is displayed

  iTextureObject->ReflectOverXAxis();//need to rotate otherwise image will be upside down
	}

//----------------------- functions -----------------------------


void CBackgroundImage::InitilizeBackgroundImage()
{
  CBackgroundImage::LoadBackgroundImageTextures();
}


void CBackgroundImage::RemoveBackgroundImage()
{
  CBackgroundImage::UnloadBackgroundImageTextures();
}

void CBackgroundImage::LoadBackgroundImageTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBackgroundBlueDay);
}

void CBackgroundImage::UnloadBackgroundImageTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBackgroundBlueDay);
}

void CBackgroundImage::Draw()
{
  iTextureObject->DrawArrays();
}

void CBackgroundImage::UpdatePosition(TPointIntFloat aPosition,TBool aIsInGroundView,TInt aCurrentAircraftSpeed)
{
  //update S position of the bitmap
	iTextureObject->ChangeSCoordinate(-BACKGROUND_BASIC_CLOUD_MOVE_SPEED + (GLfloat)aCurrentAircraftSpeed*BACKGROUND_BASIC_CLOUD_MOVE_SPEED);

  if(aIsInGroundView)
    {
    //set back to original position
    iTextureObject->SetNewTValue(BACKGROUND_BASIC_DEFAULT_LOWER_T_VALUE,iBackgroundUpperTValue);
    }
  else
    {
    TInt lPlayerPositionView = aPosition.iY.GetIntInBaseInt();
    if(lPlayerPositionView < GMAX_HEIGHT_IN_GAME_LEVEL)
      {
    	TFloat lAdditionalTValueNeeded = (TFloat)lPlayerPositionView / MAX_HEIGHT_OF_BITMAP;
      iTextureObject->SetNewTValue(BACKGROUND_BASIC_DEFAULT_LOWER_T_VALUE-lAdditionalTValueNeeded,iBackgroundUpperTValue-lAdditionalTValueNeeded);
      }
    }
}

void CBackgroundImage::UpdateForZoomFactor()
{
	iBackgroundUpperTValue = BACKGROUND_BASIC_DEFAULT_UPPER_T_VALUE * CPlayer::ZoomFactor;
	iTextureObject->SetNewSValue(0, ((TFloat) iBackgroundTextureWidth.GetIntInBaseInt() / BACKGROUND_TEXTURE_SIZE) * CPlayer::ZoomFactor);
}
