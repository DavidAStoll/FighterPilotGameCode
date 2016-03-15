/*
 ============================================================================
 Name		: ClockTimerEvent.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CClockTimerEvent implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/Events/CCreateMitchellToAttackTargetClockTimer.h"
#include "includes/game/backend/gameObjects/Events/CClockTimerEvent.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/Aircrafts/CMitchell.h"
#include "includes/game/backend/CClockTimer.h"

CClockTimerEvent::CClockTimerEvent(TInt aSecondsUntilCheckEvent) :
	CEvent(EEventIdentifierClockTimerEvent)
{
	iFramesLeftUntilNextSecond = GFRAMES_PER_SECOND_BACKEND;
	iSecondsUntilChecking = aSecondsUntilCheckEvent;
	ConstructL();
}

CClockTimerEvent::~CClockTimerEvent()
{
}


void CClockTimerEvent::ConstructL()
{
	TInt lMinutes = iSecondsUntilChecking / 60;
	TInt lSeconds = iSecondsUntilChecking % 60;
	iClockTimer = CClockTimer::New(lMinutes, lSeconds, TPoint(CREATE_MITCHELL_TO_ATTACK_TARGET_EVENT_TIMER_X_LOCATION, CREATE_MITCHELL_TO_ATTACK_TARGET_EVENT_TIMER_Y_LOCATION), KColourDarkRed);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddClockTimerObject(iClockTimer);
}

void CClockTimerEvent::TriggerEvent() //calls the method the event will cause, should only be called if the event's conditions have been met
{
  //nothing to do
}

TBool CClockTimerEvent::CheckIfEventConditionIsMet() //checks if the Event Condition has been met
{
	iFramesLeftUntilNextSecond--;
	if(iFramesLeftUntilNextSecond == 0)
	{
		iSecondsUntilChecking--;
		iFramesLeftUntilNextSecond = GFRAMES_PER_SECOND_BACKEND;
		iClockTimer->DecreaseTimerBySecond();
		
		if(iSecondsUntilChecking == 0) // event should be called now
		{
			//all done, nothing to do
			return true;
		}
	}
	return false;
}

void CClockTimerEvent::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iSecondsUntilChecking);
	aOutputStream.WriteInt32(iFramesLeftUntilNextSecond);
	iClockTimer->SaveOnDisk(aOutputStream);
}

void CClockTimerEvent::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iSecondsUntilChecking = aReadStream.ReadInt32();
	iFramesLeftUntilNextSecond = aReadStream.ReadInt32();
	iClockTimer->LoadFromDisk(aReadStream);
}
