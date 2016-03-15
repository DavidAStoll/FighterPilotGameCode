/*
 ============================================================================
 Name		: GroundUnitOverloadAI.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGroundUnitOverlordAI implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CGroundUnit.h"
#include "includes/game/backend/CGameObjectManager.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/core/utility/CRandomGenerator.h"

CGroundUnitOverlordAI::CGroundUnitOverlordAI(SGameObjectAttributes& aAttributes) :
	CMoveableGameObject(GROUND_UNIT_OVERLORD_AI_Z_VALUE, NULL, NULL, TPoint(0, 0), aAttributes, GROUND_UNIT_OVERLORD_AI_DEFAULT_FRAMES_PER_MOVE) //position is not importan, will be updated depending on units that it keeps track of
{
	iCurrentState = EGroundUnitOverlordAIStateWaiting;
	iXTargetLocation = TIntFloat::Convert(0);
	iHaveWalkingTarget = false;
	iCurrentEnemyOverlord = NULL;
	iDistanceUntilEngage = TIntFloat::Convert(GROUND_UNIT_OVERLORD_AI_DEFAULT_DISTANCE_TO_ENGAGE_TARGET);
	iIsInProtectingPositionMode = false;
	iDefaultXPosition = 0;
	iCurrentFrame = 0;
}

CGroundUnitOverlordAI::~CGroundUnitOverlordAI()
{
	if(iGameObjectInControlOf != NULL)
	{
		delete iGameObjectInControlOf;
		iGameObjectInControlOf = NULL;
	}
}

CGroundUnitOverlordAI* CGroundUnitOverlordAI::New(TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = GROUND_UNIT_OVERLORD_AI_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture;
	lAttributes.Reflected = false;
	lAttributes.Armor = GROUND_UNIT_OVERLORD_AI_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierGroundUnitOverlordAI;

	CGroundUnitOverlordAI* self = new CGroundUnitOverlordAI(lAttributes);
	self->ConstructL();
	return self;
}

CGroundUnitOverlordAI* CGroundUnitOverlordAI::New(TConflictSide aConflictSide, SProtectPositionSpan aArea, TInt aDefaultXPosition)
{
	CGroundUnitOverlordAI* self = New(aConflictSide);
	self->ProtectArea(aArea, aDefaultXPosition);
	return self;
}

void CGroundUnitOverlordAI::ConstructL()
{
	iHitBox = NULL; //doesn't have a hit box
	iGameObjectInControlOf = CPointerArray<CGroundUnit>::New();

	TIntFloat lExtraDistance = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(0, GROUND_UNIT_OVERLORD_AI_MAX_EXTRA_DISTANCE_TO_ENGAGE_ENEMY);//give certain randomness to shot
	iDistanceUntilEngage += lExtraDistance;
}

//--------------- functions -------------//

void CGroundUnitOverlordAI::Update()
{

	//remove objects that died, doesn't delete them. Just removes the pointer to them. Deletion will be handeled by the garbage collector
	for(TInt lIndex = 0; lIndex < iGameObjectInControlOf->GetCount(); lIndex++)
		if(!iGameObjectInControlOf->Get(lIndex)->IsAlive() || iGameObjectInControlOf->Get(lIndex)->GetIsLeavingMap())
		{
			iGameObjectInControlOf->Remove(lIndex);
			lIndex--;//since count decreased by 1
		}

	//check if we have any objects left to control, if none, delete this Overlord AI since it has no further use
	if(iGameObjectInControlOf->GetCount() == 0)
	{
		iAlive = false;//will be recycled after some time,since it is a moveable object
		return;
	}

	//Update position, since front most unit might have died or changed because unit turned to walk other side
	UpdatePosition();

	//check for enemies if we are currently not engaged in a battle
	if(iCurrentState != EGroundUnitOverlordAIStateFighting)
	{
		//perform check from time to time if a enemy overlord is present
		iCurrentFrame++;
		if(iCurrentFrame == GROUND_UNIT_OVERLORD_AI_FRAMES_UNTIL_CHECK_FOR_OTHER_ENEMY_OVERLORD)
		{
			iCurrentFrame = 0;
			if(iIsInProtectingPositionMode)
			{
				iCurrentEnemyOverlord = FindSuitableTarget(iProtectedArea); //only look for enemy that are in our area

				if(iCurrentEnemyOverlord) //found a target that is in the area that this overloard is trying to protect
					MoveGroundUnitsToLocation(iCurrentEnemyOverlord->GetCurrentPositionNormilized().iX); //move to the enemy position
				else if(iXTargetLocation != TIntFloat::Convert(iDefaultXPosition))
					MoveGroundUnitsToLocation(iDefaultXPosition);//move back to home position
			}else
			{
				iCurrentEnemyOverlord = FindSuitableTarget(); //find any target that we can attack
				if(iCurrentEnemyOverlord) //make sure to face the enemy overlord
					TurnGroundUnitsToFacePosition(iCurrentEnemyOverlord->GetCurrentPosition().iX);
			}
		}
		if(iCurrentEnemyOverlord)//we have an enemy we can fight
		{
			//check if enemy Overlord is dead
			if(iCurrentEnemyOverlord->IsAlive())
			{
				TIntFloat lDistanceBetweenTargetLocation = CMath::Abs(iCoordinates.iX - iCurrentEnemyOverlord->GetCurrentPosition().iX);
				//keep moving until we are close enough to engage
				if(iDistanceUntilEngage > lDistanceBetweenTargetLocation //target is in reach
						|| (iCurrentState == EGroundUnitOverlordAIStateWaiting && TIntFloat::Convert(GROUND_UNIT_OVERLORD_AI_TOTAL_MAX_DISTANCE_BETWEEN_TARGETS) > lDistanceBetweenTargetLocation))//since we are waiting, we can attack the target at the max distance possible
				{
					//we are close enough
					iCurrentState = EGroundUnitOverlordAIStateFighting;
					//tell all units to start fighting
					for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
						iGameObjectInControlOf->Get(lCurrentIndex)->SwitchToFiringState();
				}
			}else
			{
				iCurrentEnemyOverlord = false; //enemy overlord has been killed
			}
		}
	}

	if(iCurrentState == EGroundUnitOverlordAIStateWalking)
	{
		//check if we reached our destination
		if((iObjectReflected && iCoordinates.iX > iXTargetLocation) || //moving right
				(!iObjectReflected && iCoordinates.iX < iXTargetLocation) //moving left
		)
		{
			//stop all objects
			for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
				iGameObjectInControlOf->Get(lCurrentIndex)->SwitchToStandingState();

			iCurrentState = EGroundUnitOverlordAIStateWaiting;
			iHaveWalkingTarget = false;
		}
	}else if(iCurrentState == EGroundUnitOverlordAIStateFighting)
	{
		if(!iCurrentEnemyOverlord->IsAlive())
		{
			iCurrentEnemyOverlord = NULL; //has been eliminated, fighting is over. Can do something else now
			if(iIsInProtectingPositionMode)
			{
				//check if another enemy is around
				iCurrentEnemyOverlord = FindSuitableTarget(iProtectedArea);
				if(iCurrentEnemyOverlord) //found a target that is in the area that this overloard is trying to protect
				{
					MoveGroundUnitsToLocation(iCurrentEnemyOverlord->GetCurrentPositionNormilized().iX); //move to the enemy position
				}else //nothing to do for now
				{
					iCurrentState = EGroundUnitOverlordAIStateWaiting;
					for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
						iGameObjectInControlOf->Get(lCurrentIndex)->SwitchToStandingState();
				}
			}else if(iHaveWalkingTarget)
			{
				iCurrentState = EGroundUnitOverlordAIStateWalking;
				for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
					iGameObjectInControlOf->Get(lCurrentIndex)->SwitchToMovingState();
			}else
			{
				iCurrentState = EGroundUnitOverlordAIStateWaiting;
				for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
					iGameObjectInControlOf->Get(lCurrentIndex)->SwitchToStandingState();
			}
		}
	}
}

void CGroundUnitOverlordAI::ProtectArea(SProtectPositionSpan& aArea, TInt aDefaultXPosition)
{
	iIsInProtectingPositionMode = true;
	iProtectedArea = aArea;
	iDefaultXPosition = aDefaultXPosition;
}

//this method only looks for targets that are inside the area that this Overlord is trying to protect
CGroundUnitOverlordAI* CGroundUnitOverlordAI::FindSuitableTarget(SProtectPositionSpan& aSpaningArea)
{
	//find a suitable target
	//go through all objects that exists on the map and find one that is closest
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesNoHitBoxNoTexture);
	TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
	CGroundUnitOverlordAI* lCurrentBestTarget = NULL;

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects <lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);
		TPointIntFloat& lCurrentGameObjectPosition = lCurrentGameObject->GetCurrentPosition();

		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierGroundUnitOverlordAI && iConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive()
				&& (aSpaningArea.LeftXPosition < lCurrentGameObjectPosition.iX && aSpaningArea.RightXPosition > lCurrentGameObjectPosition.iX))
		{
			CGroundUnitOverlordAI* lCurrentOverlordAI = static_cast<CGroundUnitOverlordAI*> (lCurrentGameObject);
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentOverlordAI->GetCurrentPosition();
			TInt lDistanceBetweenTargetLocation = CMath::Abs(iCoordinates.iX - lTargetLocation.iX).GetIntInBaseInt();

			if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
			{
				lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
				lCurrentBestTarget = lCurrentOverlordAI;
			}
		}
	}

	//clean up
	delete lGameObjects;

	return lCurrentBestTarget;
}

CGroundUnitOverlordAI* CGroundUnitOverlordAI::FindSuitableTarget()
{
	//find a suitable target
	//go through all objects that exists on the map and find one that is closest
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesNoHitBoxNoTexture);
	TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
	CGroundUnitOverlordAI* lCurrentBestTarget = NULL;

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierGroundUnitOverlordAI && iConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive())
		{
			CGroundUnitOverlordAI* lCurrentOverlordAI = static_cast<CGroundUnitOverlordAI*> (lCurrentGameObject);
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentOverlordAI->GetCurrentPosition();
			TInt lDistanceBetweenTargetLocation = CMath::Abs(iCoordinates.iX - lTargetLocation.iX).GetIntInBaseInt();

			if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
			{
				lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
				lCurrentBestTarget = lCurrentOverlordAI;
			}
		}
	}

	//clean up
	delete lGameObjects;

	return lCurrentBestTarget;
}

void CGroundUnitOverlordAI::MoveGroundUnitsToLocation(TInt aXLocation)
{
	if(iHaveWalkingTarget && iXTargetLocation == TIntFloat::Convert(aXLocation))
		return; //avoid duplicate calls

	iHaveWalkingTarget = true;
	iCurrentState = EGroundUnitOverlordAIStateWalking;//update state
	iXTargetLocation = TIntFloat::Convert(aXLocation);//scale to TIntFloat

	//update position first, to make sure we know where we are standing
	UpdatePosition();

	TurnGroundUnitsToFacePosition(iXTargetLocation);

	//find Speed of slowest unit
	CGroundUnit* lSlowestGroundUnit = NULL;
	for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
	{
		if(lSlowestGroundUnit)
		{
			if(iGameObjectInControlOf->Get(lCurrentIndex)->GetMaxSpeed() < lSlowestGroundUnit->GetMaxSpeed())
				lSlowestGroundUnit = iGameObjectInControlOf->Get(lCurrentIndex);
		}else
			lSlowestGroundUnit = iGameObjectInControlOf->Get(lCurrentIndex);
	}

	//set state to walking and at what speed
	for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
	{
		//set Units speed to max speed in this group
		iGameObjectInControlOf->Get(lCurrentIndex)->SetCurrentSpeed(lSlowestGroundUnit->GetMaxSpeed());
		//set state to moving
		iGameObjectInControlOf->Get(lCurrentIndex)->SwitchToMovingState();
	}
}

void CGroundUnitOverlordAI::TurnGroundUnitsToFacePosition(TIntFloat& aXPosition)
{
	//now check if we have to move right or left
	TBool lMoveRight;
	if(iCoordinates.iX < aXPosition) //is to the right
		lMoveRight = true;
	else
		lMoveRight = false;
	iObjectReflected = lMoveRight;//to keep track what direction we are moving

	//set state to walking, what direction they should face and what speed
	for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
	{
		if(lMoveRight)
			iGameObjectInControlOf->Get(lCurrentIndex)->TurnRight();
		else
			iGameObjectInControlOf->Get(lCurrentIndex)->TurnLeft();
	}
}

void CGroundUnitOverlordAI::GiveOrder(SGroundUnitOrder& aOrder)
{
	if(aOrder.ShouldProtectArea)
	{
		ProtectArea(aOrder.ProtectionArea, aOrder.DefaultXPosition);
	}
	if(aOrder.MoveToLocation)
	{
		MoveGroundUnitsToLocation(aOrder.XLocationDestination);
	}
}

void CGroundUnitOverlordAI::AddGroundUnit(CGroundUnit* aGroundUnit)
{
	iGameObjectInControlOf->Append(aGroundUnit);
	UpdatePosition();//position might have changed
}

void CGroundUnitOverlordAI::UpdatePosition()
{
	//update current position
	CGameObject* lCurrentFurthestObject = NULL;
	if(iObjectReflected) // means it walking to the right side
	{
		//find object that is furthest to the right
		for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
			if(lCurrentFurthestObject)
			{
				if(lCurrentFurthestObject->GetCurrentPosition().iX < iGameObjectInControlOf->Get(lCurrentIndex)->GetCurrentPosition().iX)
					lCurrentFurthestObject = iGameObjectInControlOf->Get(lCurrentIndex); //bigger X, means further to the right
			}else
				lCurrentFurthestObject = iGameObjectInControlOf->Get(lCurrentIndex); //on the first call
	}else //walks to the left
	{
		//find object that is furthest to the left
		for(TInt lCurrentIndex = 0; lCurrentIndex < iGameObjectInControlOf->GetCount(); lCurrentIndex++)
			if(lCurrentFurthestObject)
			{
				if(lCurrentFurthestObject->GetCurrentPosition().iX > iGameObjectInControlOf->Get(lCurrentIndex)->GetCurrentPosition().iX)
					lCurrentFurthestObject = iGameObjectInControlOf->Get(lCurrentIndex); //smaller X, means further to the left
			}else
				lCurrentFurthestObject = iGameObjectInControlOf->Get(lCurrentIndex); //on the first call
	}

	if(lCurrentFurthestObject) // at least has one object
		iCoordinates = lCurrentFurthestObject->GetCurrentPosition();
}

void CGroundUnitOverlordAI::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	MoveableGameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentState);
	aOutputStream.WriteIntFloat(iXTargetLocation);
	aOutputStream.WriteIntFloat(iDistanceUntilEngage);
	aOutputStream.WriteInt32(iHaveWalkingTarget);
	aOutputStream.WriteInt32(iIsInProtectingPositionMode);
	aOutputStream.WriteInt32(iDefaultXPosition);
	aOutputStream.WriteIntFloat(iProtectedArea.LeftXPosition);
	aOutputStream.WriteIntFloat(iProtectedArea.RightXPosition);
	//save pointer to enemy overlord
	aOutputStream.WriteUint32((TUint32)iCurrentEnemyOverlord);
	//save all objects that we are in charge of
	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectInControlOf, aOutputStream);
}

void CGroundUnitOverlordAI::LoadFromDisk(CFileReadStream &aReadStream)
{
	MoveableGameObjectLoadContentFromDisk(aReadStream);
	iCurrentState = static_cast<TGroundUnitOverlordAIStates> (aReadStream.ReadInt32());
	iXTargetLocation = aReadStream.ReadIntFloat();
	iDistanceUntilEngage = aReadStream.ReadIntFloat();
	iHaveWalkingTarget = aReadStream.ReadInt32();
	iIsInProtectingPositionMode = aReadStream.ReadInt32();
	iDefaultXPosition = aReadStream.ReadInt32();
	iProtectedArea.LeftXPosition = aReadStream.ReadIntFloat();
	iProtectedArea.RightXPosition = aReadStream.ReadIntFloat();
	//restore pointer to enemy overlord, if NULL it stays NULL
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iCurrentEnemyOverlord));
	//restore pointers to GroundUnits owned by this overlord
	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectInControlOf, aReadStream);
}
