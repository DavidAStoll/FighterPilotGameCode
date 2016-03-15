/*
 ============================================================================
 Name		: ShipCarrier.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipCarrier implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CShipCarrier.h"
#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/backend/gameObjects/Buildings/CFlagPole.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CShipCarrier::CShipCarrier(TInt aZValue, TInt aXLocation, TInt aKeelDepth, TIntFloat aShipMaxSpeed, TInt aFlagPoleXOffset, TInt aFlagPoleYOffset, SGameObjectAttributes& aAttributes) :
	CShip(aZValue, aXLocation, aKeelDepth, aShipMaxSpeed, aAttributes)
{
	iNumberOfAircraftsLeft = CARRIER_DEFAULT_NUMBER_OF_AIRCRAFTS;
	iFramesLeftUntilSpawnPlane = 0;
	iDoesNotSpawnPlanes = false;
	iFlagPoleXOffset = aFlagPoleXOffset;
	iFlagPoleYOffset = aFlagPoleYOffset;
	iTarget = NULL;
	iFlagPole = NULL;
	iLandingSurface = NULL;
	iFirstAircraft = NULL;
	iSecondAircraft = NULL;
	iTargetPriority = GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS; // include planes as well
}

CShipCarrier::~CShipCarrier()
{
}

//------- functions ------------//

void CShipCarrier::Update()
{
	ShipCarrierBasicUpdate();
}

void CShipCarrier::SetTarget(CGameObject* aTarget)
{
	iTarget = aTarget;
}

void CShipCarrier::ShipCarrierBasicUpdate()
{
	ShipBasicUpdate();
	if(iAlive)
	{
		if(iTarget && !iTarget->IsAlive())//make sure target has not been destroyed yet
			iTarget = NULL;

		if(iFlagPole)
		{ //in case the ship is moving, set the flagpole back to its default position
			TPointIntFloat lNewPositionOfFlag = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, iFlagPoleXOffset, iFlagPoleYOffset);
			iFlagPole->SetPosition(lNewPositionOfFlag);
			iFlagPole->Rotate(0);
		}
		if(iLandingSurface)
		{
			iLandingSurface->UpdateXLocation(iPixelsPerMoveX); //need to make sure that landing surface moves along with the carrier
		}

		if(iDoesNotSpawnPlanes)
		{
			return; //all done
		}else
		{
			//check if a plane died
			if(iFirstAircraft && !iFirstAircraft->IsAlive())
				iFirstAircraft = NULL;
			if(iSecondAircraft && !iSecondAircraft->IsAlive())
				iSecondAircraft = NULL;

			if(iFramesLeftUntilSpawnPlane == 0)
			{
				iNumberOfAircraftsLeft--;
				if(iNumberOfAircraftsLeft > 0)
					iFramesLeftUntilSpawnPlane = CARRIER_DEFAULT_SECS_UNTIL_SPAWN_AIRCRAFT;
				else
					//still spawn, but a lot slower
					iFramesLeftUntilSpawnPlane = CARRIER_DEFAULT_SECS_UNTIL_SPAWN_AIRCRAFT * CARRIER_DEFAULT_EXTRA_SPAWN_TIME_MULTIPLIER_IF_NO_PLANES_LEFT;

				if(!iFirstAircraft)//either first aircraft or aircraft just died
				{
					iFirstAircraft = CreateNewAircraft();
					if(!iFirstAircraft->GetFighterAI())
						iFirstAircraft->SetAIControlled();
					iFirstAircraft->GetFighterAI()->SetTargetAttackType(iTargetPriority);
					iFirstAircraft->GetFighterAI()->SetTargetToAttack(iTarget);//doesn't matter if NULL
				}else if(!iSecondAircraft)
				{
					iSecondAircraft = CreateNewAircraft();
					if(!iSecondAircraft->GetFighterAI())
						iSecondAircraft->SetAIControlled();
					iSecondAircraft->GetFighterAI()->SetTargetAttackType(iTargetPriority);
					iSecondAircraft->GetFighterAI()->SetTargetToAttack(iTarget);//doesn't matter if NULL
				}
			}else if(!iFirstAircraft || !iSecondAircraft) //at least one slot is unused
			{
				iFramesLeftUntilSpawnPlane--;
			}
		}
	}
}

void CShipCarrier::ShipCarrierSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iNumberOfAircraftsLeft);
	aOutputStream.WriteInt32(iDoesNotSpawnPlanes);
	aOutputStream.WriteInt32(iFramesLeftUntilSpawnPlane);
	aOutputStream.WriteInt32(iFlagPoleXOffset);
	aOutputStream.WriteInt32(iFlagPoleYOffset);
	//save Pointers
	aOutputStream.WriteUint32((TUint32)iTarget);
	aOutputStream.WriteUint32((TUint32)iFlagPole);
	aOutputStream.WriteUint32((TUint32)iLandingSurface);
	aOutputStream.WriteUint32((TUint32)iFirstAircraft);
	aOutputStream.WriteUint32((TUint32)iSecondAircraft);
}
void CShipCarrier::ShipCarrierLoadContentFromDisk(CFileReadStream &aReadStream)
{
	ShipLoadContentFromDisk(aReadStream);
	iNumberOfAircraftsLeft = aReadStream.ReadInt32();
	iDoesNotSpawnPlanes = aReadStream.ReadInt32();
	iFramesLeftUntilSpawnPlane = aReadStream.ReadInt32();
	iFlagPoleXOffset = aReadStream.ReadInt32();
	iFlagPoleYOffset = aReadStream.ReadInt32();
	//reset pointers
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iTarget));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iFlagPole));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iLandingSurface));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iFirstAircraft));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iSecondAircraft));
}
