/*
 ============================================================================
 Name		: CreateMitchellToAttackTargetClockTimer.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCreateMitchellToAttackTargetClockTimer implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Maps/MapDefaultValues.h"
#include "includes/game/backend/gameObjects/Events/CCreateMitchellToAttackTargetClockTimer.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/Aircrafts/CMitchell.h"
#include "includes/game/backend/CClockTimer.h"
#include "includes/game/backend/gameObjects/Events/CWinIfObjectsLeaveMapAlive.h"

CCreateMitchellToAttackTargetClockTimer::CCreateMitchellToAttackTargetClockTimer(CGameObject* aTarget, TInt aSecondsUntilSpawnEvent) :
	CEvent(EEventIdentifierSpawnMitchellToAttackTargetClockTimer)
{
	iFramesLeftUntilNextSecond = GFRAMES_PER_SECOND_BACKEND;
	iTargetObject = aTarget;
	iSecondsUntilSpawnEvent = aSecondsUntilSpawnEvent;
}

CCreateMitchellToAttackTargetClockTimer::~CCreateMitchellToAttackTargetClockTimer()
{
}

CCreateMitchellToAttackTargetClockTimer* CCreateMitchellToAttackTargetClockTimer::New(CGameObject* aTarget, TInt aSecondsUntilSpawnEvent)
{
	CCreateMitchellToAttackTargetClockTimer* self = new CCreateMitchellToAttackTargetClockTimer(aTarget, aSecondsUntilSpawnEvent);
	self->ConstructL();
	return self;
}

void CCreateMitchellToAttackTargetClockTimer::ConstructL()
{

	TInt lMinutes = iSecondsUntilSpawnEvent / 60;
	TInt lSeconds = iSecondsUntilSpawnEvent % 60;
	iClockTimer = CClockTimer::New(lMinutes, lSeconds, TPoint(CREATE_MITCHELL_TO_ATTACK_TARGET_EVENT_TIMER_X_LOCATION, CREATE_MITCHELL_TO_ATTACK_TARGET_EVENT_TIMER_Y_LOCATION), KColourDarkRed);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddClockTimerObject(iClockTimer);
}

void CCreateMitchellToAttackTargetClockTimer::TriggerEvent() //calls the method the event will cause, should only be called if the event's conditions have been met
{
	//create bonus score if object leave map, create seperate new classs with extra points ------------- do later --------------
	CWinIfObjectsLeaveMapAlive* lWinEvent = new CWinIfObjectsLeaveMapAlive("Frontend.BonusEvents.OneB25Leaves", CAMPAIGN1_MAP2_MITCHELL_LEAVES_MAP_BONUS_SCORE, 1);

	SHeavyBombingMission lMission;
	lMission.Target = iTargetObject;
	//create three planes
	TInt lXSpawningLocationPlane1 = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(MITCHELL_WIDTH * 7);
	TInt lXSpawningLocationPlane2 = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(MITCHELL_WIDTH);
	TInt lXSpawningLocationPlane3 = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(MITCHELL_WIDTH * 4);

	CMitchell* lMitchell = CMitchell::New(false, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocationPlane1, COMSTATION_BOMBER_SPAWNING_HEIGHT));
	lMitchell->AssignBombingMission(lMission);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lMitchell);
	lWinEvent->AddGameObject(lMitchell);

	lMitchell = CMitchell::New(false, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocationPlane2, COMSTATION_BOMBER_SPAWNING_HEIGHT));
	lMitchell->AssignBombingMission(lMission);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lMitchell);
	lWinEvent->AddGameObject(lMitchell);

	lMitchell = CMitchell::New(false, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocationPlane3, COMSTATION_BOMBER_SPAWNING_HEIGHT + MITCHELL_HEIGHT));
	lMitchell->AssignBombingMission(lMission);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lMitchell);
	lWinEvent->AddGameObject(lMitchell);

	//add event to event list
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddEventObject(lWinEvent);
}

TBool CCreateMitchellToAttackTargetClockTimer::CheckIfEventConditionIsMet() //checks if the Event Condition has been met
{
	iFramesLeftUntilNextSecond--;
	if(iFramesLeftUntilNextSecond == 0)
	{
		iSecondsUntilSpawnEvent--;
		iFramesLeftUntilNextSecond = GFRAMES_PER_SECOND_BACKEND;
		iClockTimer->DecreaseTimerBySecond();

		if(iSecondsUntilSpawnEvent == 0) // event should be called now
			return true;
	}

	return false;
}

void CCreateMitchellToAttackTargetClockTimer::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);

	aOutputStream.WriteInt32(iSecondsUntilSpawnEvent);
	aOutputStream.WriteInt32(iFramesLeftUntilNextSecond);
	iClockTimer->SaveOnDisk(aOutputStream);
	aOutputStream.WriteUint32((TUint32)iTargetObject);
}

void CCreateMitchellToAttackTargetClockTimer::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);

	iSecondsUntilSpawnEvent = aReadStream.ReadInt32();
	iFramesLeftUntilNextSecond = aReadStream.ReadInt32();
	iClockTimer->LoadFromDisk(aReadStream);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iTargetObject));
}

