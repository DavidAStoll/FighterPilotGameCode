/*
 ============================================================================
 Name		: ControlsTutorialSlowDown.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialSlowDown implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialSlowDown.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CControlsTutorialSlowDown::CControlsTutorialSlowDown(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfPlaneCrashed, TTutorialMethodCallIds TTutorialMethodCallIfPlaneDidNotCrash) :
	CBasicAircraftNeededTutorialClass(EEventIdentifierSlowDownTutorial, aAircraft, CONTROLS_TUTORIAL_LOOP_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfPlaneCrashed)
{
	iMethodCallIfNotCrashed = TTutorialMethodCallIfPlaneDidNotCrash;
	iSlowedDown = false;
	iPlayerDied = false;
	iSpeedUpAgain= false;
}

CControlsTutorialSlowDown::~CControlsTutorialSlowDown()
{
}

TBool CControlsTutorialSlowDown::TutorialEventLogic()
{
	//not needed
	return false;
}

TBool CControlsTutorialSlowDown::CheckIfEventConditionIsMet() //checks if the Event Condition has been met() //checks if the Event Condition has been met
{
	if(!iSlowedDown && iAircraft->IsAlive() && iAircraft->GetCurrentEngineAcceleration() == 0) //player slowed down completly
	{
		iSlowedDown = true;
		//need to remove restart when die event otherwise this event gets messed up
		CPointerArray<CEvent>* lEvents= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetEvents();
		for(TInt lIndex = 0; lIndex < lEvents->GetCount(); lIndex++)
		{
			if(lEvents->Get(lIndex)->GetEventIdentifier() == EEventIdentifierRestartLastTutorialIfDied)
			{
				lEvents->RemoveAndDestroy(lIndex);
				lIndex--;// since we removed one object
			}
		}
	}
	if(iSlowedDown)
	{
		//we have two options to advance to next tutorial, either the player crashes the plane, or he speeds up again and saves the plane
		if(iAircraft->AirplaneCrashed())
		{
			iReadyToMakeMethodCall = true;
			iPlayerDied = true;
			iFramesUntilMethodCall = CONTROLS_TUTORIAL_SLOW_DOWN_SECS_UNTIL_SCUCCESS_MESSAGE_IF_CRASHED * GFRAMES_PER_SECOND_BACKEND;
		}
		if(iAircraft->GetCurrentEngineAcceleration() >= TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING))
		{
			iSpeedUpAgain = true;//player speed up the plane again to avoid dropping
		}
		if(iSpeedUpAgain)
		{
			TInt lPlaneFlyingAngle = *iAircraft->GetFlyingAngel();
			if(lPlaneFlyingAngle <= 180) //means is pointing up again
			{
				iReadyToMakeMethodCall = true; //player will most likely save the plane from crashing
				iFramesUntilMethodCall = CONTROLS_TUTORIAL_SLOW_DOWN_SECS_UNTIL_SCUCCESS_MESSAGE_IF_SPEED_UP_AGAIN * GFRAMES_PER_SECOND_BACKEND;
			}
		}
	}
	
	if(iReadyToMakeMethodCall)
	{
		iCurrentFrame++;
		if(iCurrentFrame >= iFramesUntilMethodCall)
		{
			if(iPlayerDied)
				((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iMethodCall);
			else
				((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iMethodCallIfNotCrashed); //better ending, if player speed up again and saved the plane
		}
	}
	
	return false;
}

void CControlsTutorialSlowDown::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicAircraftNeededTutorialSaveOnDisk(aOutputStream);
  aOutputStream.WriteInt32(iSpeedUpAgain); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iSlowedDown); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iPlayerDied); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iMethodCallIfNotCrashed); //used from the outside to know what kind of event to create
}

void CControlsTutorialSlowDown::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicAircraftNeededTutorialLoadFromDisk(aReadStream);
	iSpeedUpAgain = aReadStream.ReadInt32();
	iSlowedDown = aReadStream.ReadInt32();
	iPlayerDied = aReadStream.ReadInt32();
	iMethodCallIfNotCrashed = static_cast<TTutorialMethodCallIds>(aReadStream.ReadInt32());
}
