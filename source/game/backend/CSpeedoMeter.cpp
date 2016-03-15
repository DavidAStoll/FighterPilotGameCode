/*
 ============================================================================
 Name		: SpeedoMeter.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CSpeedoMeter implementation
 ============================================================================
 */

#include "includes/game/backend/CSpeedoMeter.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CSpeedoMeter::CSpeedoMeter(TIntFloat *aObjectAcceleration,TIntFloat aMaxAcceleration)
	{
	iCurrentAcceleration = aObjectAcceleration;
	iMaxAcceleration = aMaxAcceleration;
	}

CSpeedoMeter::~CSpeedoMeter()
	{
  if(iTextureObject)
    {
    delete iTextureObject;
    iTextureObject = NULL;
    }
	}

CSpeedoMeter* CSpeedoMeter::New(TIntFloat *aObjectAcceleration,TIntFloat aMaxAcceleration)
	{
	CSpeedoMeter* self = new CSpeedoMeter(aObjectAcceleration,aMaxAcceleration);
	self->ConstructL();
	return self;
	}

void CSpeedoMeter::ConstructL()
	{                                      //location not important since it will be updated everytime we draw
  iTextureObject = CTextureObject::New(TPoint(SPEEDOMETER_X_OFFSET_ON_SCREEN, SPEEDOMETER_Y_OFFSET_ON_SCREEN),
  																			&CFighterPilotThePacificWar::Game->iImageStore->GetImage(EImageIdHudSpeedometer),
  																			SPEEDOMETER_WIDTH,SPEEDOMETER_HEIGHT_FOR_TEXTURE_OBJECT,0,0,0.78125,0,1);
  iTextureObject->ReflectOverXAxis();
	}
//------------------- functions -----------------------------//

void CSpeedoMeter::Draw()
{
  TIntFloat lSpeedPercentage = ((*iCurrentAcceleration) / iMaxAcceleration);
  TIntFloat lHeightOfTextureUsed =  lSpeedPercentage * SPEEDOMETER_ACTUAL_HEIGHT;
  TIntFloat lHeightOfSpeedoMeter = lSpeedPercentage * SPEEDOMETER_HEIGHT_FOR_TEXTURE_OBJECT;
  iTextureObject->SetHeight(lHeightOfSpeedoMeter);
  TIntFloat lTUpperValue = lHeightOfTextureUsed / TIntFloat::Convert(SPEEDOMETER_HEIGHT_TEXTURE);
  iTextureObject->SetNewTValueIntFloat(TIntFloat::Convert(0), -lTUpperValue);

  iTextureObject->DrawArrays();
}

void CSpeedoMeter::UpdateObjectProperties(TIntFloat *aObjectAcceleration,TIntFloat aMaxAcceleration)
{
  iCurrentAcceleration = aObjectAcceleration;
  iMaxAcceleration = aMaxAcceleration;
}


void CSpeedoMeter::LoadSpeedoMeterTextures()
{
  CFighterPilotThePacificWar::Game->iImageStore->LoadImage(EImageIdHudSpeedometer);
}

void CSpeedoMeter::UnloadSpeedoMeterTextures()
{
	CFighterPilotThePacificWar::Game->iImageStore->UnloadImage(EImageIdHudSpeedometer);
}

void CSpeedoMeter::InitilizeSpeedoMeter()
{
  CSpeedoMeter::LoadSpeedoMeterTextures();
}

void CSpeedoMeter::RemoveSpeedoMeter()
{
  CSpeedoMeter::UnloadSpeedoMeterTextures();
}
