/*
 ============================================================================
 Name		: BasicTutorialClass.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CBasicTutorialClass implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CBasicTutorialClass.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CBasicTutorialClass::CBasicTutorialClass(TEventIdentifier aIdentifier, TInt aSecsUntilMessageCall, TTutorialMethodCallIds aMethodCall) :
	CEvent(aIdentifier)
{
	iReadyToMakeMethodCall = false;
	iCurrentFrame = 0;
	iFramesUntilMethodCall = aSecsUntilMessageCall * GFRAMES_PER_SECOND_BACKEND;
	iMethodCall = aMethodCall;
}

CBasicTutorialClass::~CBasicTutorialClass()
{
}

TBool CBasicTutorialClass::CheckIfEventConditionIsMet()
{
	if(iReadyToMakeMethodCall)
	{
		iCurrentFrame++;
		if(iCurrentFrame >= iFramesUntilMethodCall) //wait a bit until we interrupt the player
			((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iMethodCall);
	}else
	{
		if(TutorialEventLogic())
			iReadyToMakeMethodCall = true;
	}

	return false; //can't be used otherwise we have a double deletion problem
}

void CBasicTutorialClass::BasicTutorialSaveOnDisk(CFileWriteStream &aOutputStream)
{
  BasicEvenSaveOnDisk(aOutputStream);
  aOutputStream.WriteInt32(iReadyToMakeMethodCall); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iCurrentFrame); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iFramesUntilMethodCall); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iMethodCall); //used from the outside to know what kind of event to create
}

void CBasicTutorialClass::BasicTutorialLoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iReadyToMakeMethodCall = aReadStream.ReadInt32();
	iCurrentFrame = aReadStream.ReadInt32();
	iFramesUntilMethodCall = aReadStream.ReadInt32();
	iMethodCall = static_cast<TTutorialMethodCallIds>(aReadStream.ReadInt32());
}

void CBasicTutorialClass::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicTutorialSaveOnDisk(aOutputStream);
}

void CBasicTutorialClass::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicTutorialLoadFromDisk(aReadStream);
}

