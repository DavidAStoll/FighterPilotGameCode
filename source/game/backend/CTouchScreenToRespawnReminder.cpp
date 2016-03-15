/*
 * CTouchScreenToRespawnReminder.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/CTouchScreenToRespawnReminder.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/CPlayer.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CTouchScreenToRespawnReminder::CTouchScreenToRespawnReminder(CPlayer* aPlayer)
{
	iCurrentFrame = 0;
	iPlayer = aPlayer;
	iFont = CGame::Game->iGraphicsFont->CreateFont(TOUCHSCREEN_TO_RESPAWN_REMINDER_FONT_SIZE);
	iStringRespawnMessage = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameExtra.TouchScreenToRespawn");
}

CTouchScreenToRespawnReminder::~CTouchScreenToRespawnReminder()
{
	if(iStringRespawnMessage != NULL)
	{
		delete iStringRespawnMessage;
		iStringRespawnMessage = NULL;
	}
}

CTouchScreenToRespawnReminder* CTouchScreenToRespawnReminder::New(CPlayer* aPlayer)
{
	CTouchScreenToRespawnReminder* self = new  CTouchScreenToRespawnReminder(aPlayer);
	return self;
}


//--------------------------- functions --------------------------------------//

void CTouchScreenToRespawnReminder::Draw()
{
	if(!iPlayer->PlayerCanRespawn())//plane crashed and can't spawn another plane or plane did not crash
		return;

	//player's plane must have crashed and player must have at least one life left and player's base is still intact
	iCurrentFrame++;
	if(iCurrentFrame == TOUCHSCREEN_TO_RESPAWN_REMINDER_FRAMES_INTERVAL_BETWEEN_DRAW_AND_NOT_DRAW)
	{
		iCurrentFrame = 0;
		iIsInDrawMode = !iIsInDrawMode;
	}

	if(iIsInDrawMode)
	{
		TInt lLengthOfTotalString = CGame::Game->iGraphicsFont->GetWidthOfString(iFont, iStringRespawnMessage);
		TPoint lDrawLocation;
		lDrawLocation.iX = (1024 - lLengthOfTotalString) / 2;
		lDrawLocation.iY = TOUCHSCREEN_TO_RESPAWN_REMINDER_Y_OFFSET - TOUCHSCREEN_TO_RESPAWN_REMINDER_FONT_SIZE;

		 CGame::Game->iGraphicsFont->SetColourBlack();
		 CGame::Game->iGraphicsFont->SetFont(iFont);
		 CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lDrawLocation,iStringRespawnMessage);
	}
}
