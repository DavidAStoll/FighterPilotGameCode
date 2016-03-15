/*
 ============================================================================
 Name		: BasicAircraftNeededTutorialClass.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CBasicAircraftNeededTutorialClass implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CBasicAircraftNeededTutorialClass.h"

CBasicAircraftNeededTutorialClass::CBasicAircraftNeededTutorialClass(TEventIdentifier EEventIdentifierSpeedUpTutorial, CAircraft* aAircraft, TInt aSecsOfCallDelay, TTutorialMethodCallIds aMethodCallIfSuccess)
: CBasicTutorialClass(EEventIdentifierSpeedUpTutorial, aSecsOfCallDelay, aMethodCallIfSuccess)
	{
		iAircraft = aAircraft;
	}

CBasicAircraftNeededTutorialClass::~CBasicAircraftNeededTutorialClass()
	{
	}

void CBasicAircraftNeededTutorialClass::BasicAircraftNeededTutorialSaveOnDisk(CFileWriteStream &aOutputStream)
{
  BasicTutorialSaveOnDisk(aOutputStream);
	aOutputStream.WriteUint32((TUint32)iAircraft);
}

void CBasicAircraftNeededTutorialClass::BasicAircraftNeededTutorialLoadFromDisk(CFileReadStream &aReadStream)
{
	BasicTutorialLoadFromDisk(aReadStream);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iAircraft));
}

void CBasicAircraftNeededTutorialClass::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicAircraftNeededTutorialSaveOnDisk(aOutputStream);
}

void CBasicAircraftNeededTutorialClass::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicAircraftNeededTutorialLoadFromDisk(aReadStream);
}
