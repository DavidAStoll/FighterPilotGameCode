/*
 ============================================================================
 Name		: HeavyBomberAI.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHeavyBomberAI implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/AI/CHeavyBomberAI.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/CGameObjectManager.h"

CHeavyBomberAI::CHeavyBomberAI(CAircraft* aBomber, TInt aMiddlePositionOfBombs, SHeavyBombingMission aMission)
{
	iTargetPriority = GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP;
	iBombingMission = aMission;
	iMiddlePositionOfBombsOffset = aMiddlePositionOfBombs;
	iHasBombingMissionAssigned = true;
	iHasPriotizedTargetAssigned = true;
	iStartBombing = false;
	iBomber = aBomber;
	iNeededDistanceForDrop = 0;
	iExtraWidthOffset = 0;
	iCurrentFrameForClosestTargetCheck = 0;
	CalculateNeededDistanceForDrop(iBombingMission.Target);
}
CHeavyBomberAI::CHeavyBomberAI(CAircraft* aBomber, TInt aMiddlePositionOfBombs)
{
	iTargetPriority = GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP;
	iMiddlePositionOfBombsOffset = aMiddlePositionOfBombs;
	iHasBombingMissionAssigned = false;
	iStartBombing = false;
	iHasPriotizedTargetAssigned = false;
	iBomber = aBomber;
	iNeededDistanceForDrop = 0;
	iExtraWidthOffset = 0;
	iCurrentFrameForClosestTargetCheck = 0;
	iBombingMission.Target = NULL;
}

CHeavyBomberAI::~CHeavyBomberAI()
{
}

//--------------------- functions ----------------------//

void CHeavyBomberAI::Update()
{
	iCurrentFrameForClosestTargetCheck++;
	if(!iStartBombing && !iBomber->IsAlive())//if dead start bombing drops
	{
		iStartBombing = true;
	}
	if(iBombingMission.Target && iBombingMission.Target->GetIsMoveableGameObject() && static_cast<CMoveableGameObject*>(iBombingMission.Target)->GetIsLeavingMap())
	{
		//to avoid crashing the game when the target leaves the map, happens :)
		iHasBombingMissionAssigned = false;
		iHasPriotizedTargetAssigned = false;
	}
	//update the distance we need, might have changed because object is moving or bomber slowed down
	if(!iStartBombing && iHasBombingMissionAssigned)
		CalculateNeededDistanceForDrop(iBombingMission.Target);

	if(!iStartBombing && iHasBombingMissionAssigned)
	{
		CGameObject* lTarget = iBombingMission.Target;
		if(lTarget && lTarget->IsAlive() && iBomber->GetConflictSide() != lTarget->GetConflictSide())//might have been captured by friendly units
		{
			TPointIntFloat lTargetLocation = lTarget->GetCurrentPosition();
			TPointIntFloat lBomberLocation = iBomber->GetCurrentPosition();
			TInt lDistanceBetweenTargetLocation = CMath::Abs(lBomberLocation.iX - lTargetLocation.iX).GetIntInBaseInt();

			if((lDistanceBetweenTargetLocation + iExtraWidthOffset) - iNeededDistanceForDrop <= 0) //release bombs as soon as possible when we are in range
			{
				iStartBombing = true;
			}
		}else
		{
			iHasBombingMissionAssigned = false;
			iHasPriotizedTargetAssigned = false;
		}
	}

	if(!iStartBombing && !iHasPriotizedTargetAssigned && (!iHasBombingMissionAssigned || iCurrentFrameForClosestTargetCheck >= HEAVY_BOMBER_AI_FRAMES_UNTIL_RECHECK_ON_CLOSEST_TARGET))
	{
		iCurrentFrameForClosestTargetCheck = 0;
		//find a suitable target
		//go through all objects that exists on the map and find one that is closest
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(iTargetPriority);//only recycle troops and planes
		TConflictSide lBomberConflictSide = iBomber->GetConflictSide();
		TBool lLiesInTheCorrectDirection = false;
		TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
		CGameObject* lCurrentBestTarget = NULL;

		for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

			if(lBomberConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive())
			{
				//check if the target is in the correct direction
				if(iBomber->IsReflected()) // flies to the right
				{
					if(iBomber->GetCurrentPosition().iX < lCurrentGameObject->GetCurrentPosition().iX) // is to the left of the target object
						lLiesInTheCorrectDirection = true;
					else
						lLiesInTheCorrectDirection = false;
				}else
				{
					if(iBomber->GetCurrentPosition().iX > lCurrentGameObject->GetCurrentPosition().iX) // is to the right of the target object
						lLiesInTheCorrectDirection = true;
					else
						lLiesInTheCorrectDirection = false;
				}
				if(lLiesInTheCorrectDirection)
				{
					//check if the object is not too close to drop bombs on it properly
					TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
					TPointIntFloat lBomberLocation = iBomber->GetCurrentPosition();
					TInt lDistanceBetweenTargetLocation = CMath::Abs(lBomberLocation.iX - lTargetLocation.iX).GetIntInBaseInt();
					CalculateNeededDistanceForDrop(lCurrentGameObject);

					if((lDistanceBetweenTargetLocation + iExtraWidthOffset) - iNeededDistanceForDrop > 0) // target is good to go
					{
						if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
						{
							lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
							lCurrentBestTarget = lCurrentGameObject;
						}
					}
				}
			}
		}
		//clean up
		delete lGameObjects;

		//check if we found a valid target
		if(lCurrentBestTarget)
		{
			iHasBombingMissionAssigned = true;
			iBombingMission.Target = lCurrentBestTarget;
			CalculateNeededDistanceForDrop(iBombingMission.Target);
		}
	}

	if(iStartBombing)
	{
		//try to release bombs, will block when weapon has been just used up and stop releasing bombs once the weapons are empty
		if(iBomber->Fire1HasAmmunitionLeft())
			iBomber->Fire1();
		if(iBomber->Fire2HasAmmunitionLeft())
			iBomber->Fire2();
	}
}

void CHeavyBomberAI::AssignBombingMission(SHeavyBombingMission aMission)
{
	iHasBombingMissionAssigned = true;
	iHasPriotizedTargetAssigned = true;
	iBombingMission = aMission;
	CalculateNeededDistanceForDrop(iBombingMission.Target);
}

void CHeavyBomberAI::SetTargetPriority(TUint aTargetTypes)
{
	iTargetPriority = aTargetTypes;
}

void CHeavyBomberAI::CalculateNeededDistanceForDrop(CGameObject* aTarget)
{
	TInt lFramesNeededToHitGround;
	TPointIntFloat lBomberLocation = iBomber->GetCurrentPosition();

	//Newton formula
	//Vf^2 = Vi^2 + 2AD
	//Vf = velocity final, Vi = velocity initial, A = total acceleration, D = distance travelled
	//find final velocity, used to find time

	TIntFloat lAcceleration = ((TIntFloat::Convert(1)) * (GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE)) / GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE;
	TIntFloat lOldVelocitySquared = ((*iBomber->GetPixelsPerMoveY()) * (*iBomber->GetPixelsPerMoveY()));
	TIntFloat lAggTimesDistance = TIntFloat::Convert(2) * (lAcceleration * lBomberLocation.iY);
	TInt lSum = (lOldVelocitySquared + lAggTimesDistance).GetIntInBaseInt();
	TIntFloat lFinalVelocity = TIntFloat::Convert(CMath::Sqrt(lSum));

	//Newton formula
	//Vf = Vi + AT
	//Vf = velocity final, Vi = velocity initial, A = total acceleration, T = time
	//find time

	TIntFloat lDiffBetweenFinalAndStartingVelocity = lFinalVelocity - (*iBomber->GetPixelsPerMoveY());
	lFramesNeededToHitGround = (lDiffBetweenFinalAndStartingVelocity / lAcceleration).GetIntInBaseInt();
	//might need to add extra distance if the object can move
	TIntFloat lTargetSpeed = TIntFloat::Convert(0);
	if(aTarget->GetIsMoveableGameObject())
	{
		lTargetSpeed = *static_cast<CMoveableGameObject*>(aTarget)->GetPixelsPerMoveX();
		lTargetSpeed = CMath::Abs(lTargetSpeed);
		if(aTarget->IsReflected() == iBomber->IsReflected()) //both move into the same direction, then we need to drop the bombs later, thus less distance
			lTargetSpeed *= -1;
		//otherwise, we add it postively, which means to drop the bombs earlier, from a further distance since the target will move towards the hit location of the bombs
	}
	//at this distance to the target we should drop the bomb
	iNeededDistanceForDrop = (CMath::Abs((*iBomber->GetPixelsPerMoveX() + lTargetSpeed) * lFramesNeededToHitGround)).GetIntInBaseInt();

	//calculate how much we need we be off the target because of the position of bombs on the plane and to hit traget right in the middle
	//add also where first bomb is mounted on the airplane
	iExtraWidthOffset = 0;
	iExtraWidthOffset += iMiddlePositionOfBombsOffset;
	//need to check if we approach it from a side where the fixpoint is at the end of the object
	TInt lWidthOfTargetObject = aTarget->GetWidth().GetIntInBaseInt();
	if((!aTarget->IsReflected() && !iBomber->IsReflected()) || (aTarget->IsReflected() && iBomber->IsReflected()))
	{
		iExtraWidthOffset -= (lWidthOfTargetObject) / 2; //not a problem as below, since added distance is negative
	}
	else
		iExtraWidthOffset += (lWidthOfTargetObject) / 2;
	{
		if(CMath::Abs(iExtraWidthOffset) > iNeededDistanceForDrop) //it could be that the extra offset is bigger than the drop distance, causing it not to drop at all
		{
			TInt lSpeedOfBomber = CMath::Abs(*iBomber->GetPixelsPerMoveX()).GetIntInBaseInt();
			iExtraWidthOffset = (iNeededDistanceForDrop - (lSpeedOfBomber) * 2); //depends on how fast the bomber flies per frame
		}
	}
}

void CHeavyBomberAI::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iHasBombingMissionAssigned);
	aOutputStream.WriteInt32(iHasPriotizedTargetAssigned);
	aOutputStream.WriteInt32(iCurrentFrameForClosestTargetCheck);
	aOutputStream.WriteInt32(iExtraWidthOffset);
	aOutputStream.WriteInt32(iNeededDistanceForDrop);
	aOutputStream.WriteInt32(iMiddlePositionOfBombsOffset);
	aOutputStream.WriteInt32(iStartBombing);
	aOutputStream.WriteUint32(iTargetPriority);
	if(iHasBombingMissionAssigned)
	{
		TUint32 lBombinbMissionTargetPointerMemoryAddress = (TUint32)iBombingMission.Target;
		aOutputStream.WriteUint32(lBombinbMissionTargetPointerMemoryAddress);
	}
}

void CHeavyBomberAI::LoadFromDisk(CFileReadStream &aReadStream)
{
	iHasBombingMissionAssigned = aReadStream.ReadInt32();
	iHasPriotizedTargetAssigned = aReadStream.ReadInt32();
	iCurrentFrameForClosestTargetCheck = aReadStream.ReadInt32();
	iExtraWidthOffset = aReadStream.ReadInt32();
	iNeededDistanceForDrop = aReadStream.ReadInt32();
	iMiddlePositionOfBombsOffset = aReadStream.ReadInt32();
	iStartBombing = aReadStream.ReadInt32();
	iTargetPriority = aReadStream.ReadUint32();
	if(iHasBombingMissionAssigned)
	{
		TUint32 lOldBombinbMissionTargetPointerMemoryAddress = aReadStream.ReadUint32();
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lOldBombinbMissionTargetPointerMemoryAddress, (TUint32)(&iBombingMission.Target));
	}
}
