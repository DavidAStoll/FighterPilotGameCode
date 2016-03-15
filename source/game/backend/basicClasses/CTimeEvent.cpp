/*
 ============================================================================
 Name		: TimeEvent.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTimeEvent implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CTimeEvent.h"
#include "includes/game/common/CGameVariables.h"

CTimeEvent::CTimeEvent(TEventIdentifier aEventId, TInt iPeriodInSecs, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeatEvents) :
	CEvent(aEventId)
{
	iPeriodInFrames = iPeriodInSecs * GFRAMES_PER_SECOND_BACKEND;
	iFramesLeft = iPeriodInFrames + (aInitialExtraDelayInSec * GFRAMES_PER_SECOND_BACKEND);
	iNumberOfRepeatEventsLeft = aNumberOfRepeatEvents;
}

CTimeEvent::~CTimeEvent()
{
}

//---------------------------- functions -------------------------------------//

TBool CTimeEvent::CheckIfEventConditionIsMet()
{
	iFramesLeft--;
	TBool lEventCycleShouldStop = false;

	if(iFramesLeft == 0)
	{
		Run();
		iNumberOfRepeatEventsLeft--;
		if(iNumberOfRepeatEventsLeft <= 0)//don't create another event
			lEventCycleShouldStop = true;
		iFramesLeft = iPeriodInFrames;
	}
	return lEventCycleShouldStop;
}

void CTimeEvent::SetNumberRepeatEventsLeft(TInt aValue)
{
	iNumberOfRepeatEventsLeft = aValue;
}

void CTimeEvent::TimeEvenSaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iPeriodInFrames); //used from the outside to know what kind of event to create
	aOutputStream.WriteInt32(iFramesLeft);
	aOutputStream.WriteInt32(iNumberOfRepeatEventsLeft);
}

void CTimeEvent::TimeEventLoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iPeriodInFrames = aReadStream.ReadInt32();
	iFramesLeft = aReadStream.ReadInt32();
	iNumberOfRepeatEventsLeft = aReadStream.ReadInt32();
}

void CTimeEvent::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
}

void CTimeEvent::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
}
