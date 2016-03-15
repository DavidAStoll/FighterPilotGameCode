/*
 ============================================================================
 Name		: ControlsTutorialZoomInAndZoomOut.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialZoomInAndZoomOut implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialZoomInAndZoomOut.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CPlayer.h"

CControlsTutorialZoomInAndZoomOut::CControlsTutorialZoomInAndZoomOut(TTutorialMethodCallIds aMethodCallIfSuccess)
: CBasicTutorialClass(EEventIdentifierZoomInAndOutTutorial, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
	{
	iZoomedOutCompletely = false;
	iZoomedInCompletely = false;
	}

CControlsTutorialZoomInAndZoomOut::~CControlsTutorialZoomInAndZoomOut()
	{
	}

TBool CControlsTutorialZoomInAndZoomOut::TutorialEventLogic() //checks if the Event Condition has been met
{
	GLfloat lZoomFactor =CPlayer::ZoomFactor;
	
	if(lZoomFactor == 2)
		iZoomedOutCompletely = true;
	if(lZoomFactor == 1)
		iZoomedInCompletely = true;
	
	if( iZoomedInCompletely && iZoomedOutCompletely)
		return true;
	else
		return false;
}

void CControlsTutorialZoomInAndZoomOut::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicTutorialSaveOnDisk(aOutputStream);
  aOutputStream.WriteInt32(iZoomedOutCompletely);
  aOutputStream.WriteInt32(iZoomedInCompletely);
}

void CControlsTutorialZoomInAndZoomOut::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicTutorialLoadFromDisk(aReadStream);
	iZoomedOutCompletely = aReadStream.ReadInt32();
	iZoomedInCompletely = aReadStream.ReadInt32();
}
