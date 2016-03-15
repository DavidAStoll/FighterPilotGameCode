/*
 ============================================================================
 Name		: HeightoMeter.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CHeightoMeter implementation
 ============================================================================
 */

#include "includes/game/backend/CHeightoMeter.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

CHeightoMeter::CHeightoMeter(CAircraft* aAircraft)
{
	iOwnedAircraft = aAircraft;
	iHeightString =  CGame::Game->iLocalizationEngine->GetString("Frontend.InGameExtra.AirplaneHeight");
	iFont = CGame::Game->iGraphicsFont->CreateFont(HEIGHTOMETER_FONT_SIZE);
}

CHeightoMeter::~CHeightoMeter()
{
	if(iHeightString != NULL)
	{
		delete iHeightString;
		iHeightString = NULL;
	}
}

CHeightoMeter* CHeightoMeter::New(CAircraft* aAircraft)
{
	CHeightoMeter* self = new CHeightoMeter(aAircraft);
	return self;
}

//---------------------- functions --------------------------//

void CHeightoMeter::AssignAircraft(CAircraft* aNewAircraft)
{
	iOwnedAircraft = aNewAircraft;
}

void CHeightoMeter::Draw()
{
	//need to draw height string
	TInt lHeightOfPlane = 0;
	//get height and scale down into height intervals
	lHeightOfPlane = iOwnedAircraft->GetCurrentPositionNormilized().iY / HEIGHTOMETER_HEIGHT_STEP_INTERVAL;
	lHeightOfPlane = lHeightOfPlane * HEIGHTOMETER_HEIGHT_STEP_INTERVAL;

	//create drawing string
	CString* lStringHeightAndNumber = new CString();
	lStringHeightAndNumber->Append(iHeightString);
	lStringHeightAndNumber->AppendNum(lHeightOfPlane);

	//draw to screen
	TInt lLengthOfTotalString = CGame::Game->iGraphicsFont->GetWidthOfString(iFont, lStringHeightAndNumber);
	TPoint lDrawLocation;
	lDrawLocation.iX = HEIGHTOMETER_X_OFFSET - lLengthOfTotalString;
	lDrawLocation.iY = HEIGHTOMETER_Y_OFFSET;

	CGame::Game->iGraphicsFont->SetColourRed();
	CGame::Game->iGraphicsFont->SetFont(iFont);
	CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lDrawLocation,lStringHeightAndNumber);

	delete lStringHeightAndNumber;
}
