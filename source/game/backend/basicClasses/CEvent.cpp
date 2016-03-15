/*
 ============================================================================
 Name		: Event.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CEvent implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CEvent.h"

CEvent::CEvent(TEventIdentifier aEventId)
	{
		iEventIdentifier = aEventId;
	  iHasHappened=false;
	}

CEvent::~CEvent()
	{
	}

//---------------- functions -------------------------

TBool CEvent::EventHasAlreadyHappened()
{
  return iHasHappened;
}
TEventIdentifier CEvent::GetEventIdentifier()
{
	return iEventIdentifier;
}

TBool CEvent::CheckIfEventConditionIsMet()
{
  //child class needs to implement
  return true;
}

void CEvent::TriggerEvent()
{
  iHasHappened=true;
  //child class needs to implement
}

void CEvent::BasicEvenSaveOnDisk(CFileWriteStream &aOutputStream)
{
 aOutputStream.WriteInt32(iEventIdentifier); //used from the outside to know what kind of event to create
 aOutputStream.WriteInt32(iHasHappened);
}

void CEvent::BasicEventLoadFromDisk(CFileReadStream &aReadStream)
{
  iHasHappened = aReadStream.ReadInt32();
}

void CEvent::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  BasicEvenSaveOnDisk(aOutputStream);
}

void CEvent::LoadFromDisk(CFileReadStream &aReadStream)
{
  BasicEventLoadFromDisk(aReadStream);
}
