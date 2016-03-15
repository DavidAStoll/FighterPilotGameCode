/*
 ============================================================================
 Name		: SpawnAmericanTroopTransporterIfObjectDestroyed.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnAmericanTroopTransporterIfObjectsDestroyed implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CSpawnAmericanTroopTransporterIfObjectsDestroyed.h"
#include "includes/game/backend/gameObjects/Ships/CAmericanTroopTransporter.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CSpawnAmericanTroopTransporterIfObjectsDestroyed::CSpawnAmericanTroopTransporterIfObjectsDestroyed(TInt aXMoveToLocation, TAmericanLCTType aLctType, SGroundUnitOrder aOrder) :
	CEvent(EEventIdentifierSpawnAmericanTroopTransporterIfObjectsAreDestroyed)
{
	iXMoveToLocation = TIntFloat::Convert(aXMoveToLocation);
	iOrder = aOrder;
	iLctType = aLctType;
	iObjectsThatNeedToBeDestroyed = CPointerArray<CGameObject>::New();
}

CSpawnAmericanTroopTransporterIfObjectsDestroyed::~CSpawnAmericanTroopTransporterIfObjectsDestroyed()
{
	if(iObjectsThatNeedToBeDestroyed != NULL)
	{
		delete iObjectsThatNeedToBeDestroyed;
		iObjectsThatNeedToBeDestroyed = NULL;
	}
}

void CSpawnAmericanTroopTransporterIfObjectsDestroyed::TriggerEvent()
{
	//set game state to lost
	TInt lShipWidth = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(AMERICAN_TROOP_TRANSPORTER_WIDTH);
	CAmericanTroopTransporter* lShip = CAmericanTroopTransporter::New(lShipWidth,false,0,iLctType,iOrder);
	lShip->MoveToLocation(iXMoveToLocation);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lShip);
}

TBool CSpawnAmericanTroopTransporterIfObjectsDestroyed::CheckIfEventConditionIsMet()
{
	//go through all objects in the array and check if they have all been destroyed and if mission is not failed yet
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		//mission can still be won, check if all objects have been destroyed that need to be destroyed
		for(TInt lCurrentObject = 0; lCurrentObject < iObjectsThatNeedToBeDestroyed->GetCount(); lCurrentObject++)
		{
			if(iObjectsThatNeedToBeDestroyed->Get(lCurrentObject)->IsAlive())
				return false; //if any object has not been destroyed yet
		}
		//check all objets, all of them are destroyed, so we can return true
		return true;
	}
	return false; //Mission status has already been set, can't change it now
}

void CSpawnAmericanTroopTransporterIfObjectsDestroyed::AddGameObject(CGameObject* aGameObject)
{
	iObjectsThatNeedToBeDestroyed->Append(aGameObject);
}

void CSpawnAmericanTroopTransporterIfObjectsDestroyed::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteIntFloat(iXMoveToLocation);
	aOutputStream.WriteInt32(iLctType);
	aOutputStream.WriteInt32(iOrder.DefaultXPosition);	aOutputStream.WriteInt32(iOrder.MoveToLocation);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.LeftXPosition);	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.RightXPosition);
	aOutputStream.WriteInt32(iOrder.ShouldProtectArea);	aOutputStream.WriteInt32(iOrder.XLocationDestination);
	CMap::SaveDynamicArray((CPointerArray<void>*) iObjectsThatNeedToBeDestroyed, aOutputStream);
}

void CSpawnAmericanTroopTransporterIfObjectsDestroyed::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iXMoveToLocation = aReadStream.ReadIntFloat();
	iLctType = static_cast<TAmericanLCTType> (aReadStream.ReadInt32());
	iOrder.DefaultXPosition = aReadStream.ReadInt32(); iOrder.MoveToLocation = aReadStream.ReadInt32();
	iOrder.ProtectionArea.LeftXPosition = aReadStream.ReadIntFloat(); iOrder.ProtectionArea.RightXPosition = aReadStream.ReadIntFloat();
	iOrder.ShouldProtectArea = aReadStream.ReadInt32(); iOrder.XLocationDestination = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iObjectsThatNeedToBeDestroyed, aReadStream);
}
