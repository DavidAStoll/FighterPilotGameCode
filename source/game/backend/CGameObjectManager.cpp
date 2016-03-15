/*
 * CGameObjectManager.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CProjectile.h"
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CGameObjectManager.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/backend/CGameData.h"
#include "includes/game/backend/CPlayer.h"


CGameObjectManager::CGameObjectManager(TUint aWidth, TInt aNumberOfIntervals, CPlayer*& aPlayer)
:iPlayer(aPlayer)
{
	iWidth = TIntFloat::Convert((TInt)aWidth);
	iNumberOfIntervals = aNumberOfIntervals;
	iSizeOfEachInterval = iWidth / iNumberOfIntervals;

	iIntervals = NULL;
	iFramesUntilNextPossibleRecycle = 0;
	iMaxNumberOfObjectsRecycleLimit = GNUMBER_OF_MOVEABLE_OBJECTS_GET_RECYCLED_AT_A_TIME;
}

CGameObjectManager::~CGameObjectManager()
{
	iMoveableGameObjects->Clear();//don't want to destroy Objects owned
	iObjectsNeedInterval->Clear();
	iMoveableGameObjectsSlowRecycle->Clear();
	delete iMoveableGameObjects;
	delete iObjectsNeedInterval;
	delete iMoveableGameObjectsSlowRecycle;

	if(iIntervals)
	{
		for(TInt lCurrentInterval = 0; lCurrentInterval < iNumberOfIntervals; lCurrentInterval++)
		{
			delete iIntervals[lCurrentInterval];
		}
		delete[] iIntervals;
		iIntervals = NULL;
	}
	if(iGameObjectsByType)
	{
		//since structer contains array, we need remove array first
		for(TInt lIndex = 0; lIndex < iGameObjectsByType->GetCount(); lIndex++)
		{
			delete iGameObjectsByType->Get(lIndex)->GameObjects;
		}

		delete iGameObjectsByType;
		iGameObjectsByType = NULL;
	}

	iGameObjects->ClearAndDestroy();//here we actually destory the owned members because every object is located here
	delete iGameObjects;
}

CGameObjectManager* CGameObjectManager::New(TUint aWidth, TInt aNumberOfIntervals, CPlayer*& aPlayer)
{
	CGameObjectManager* self = new CGameObjectManager(aWidth, aNumberOfIntervals, aPlayer);
	self->ConstructL();
	return self;
}

void CGameObjectManager::ConstructL()
{
	iMoveableGameObjects = CPointerArray<CMoveableGameObject>::New();//keeps track of all objects that can move and will call their move function each frame
	iMoveableGameObjectsSlowRecycle = CPointerArray<CMoveableGameObject>::New();
	iGameObjects = CPointerArray<CGameObject>::New();
	iObjectsNeedInterval = CPointerArray<CGameObject>::New();
	iGameObjectsByType = CPointerArray<SGameObjectTypeArray>::New();
	iIntervals = new CInterval* [iNumberOfIntervals];

	for(TInt lIndex = 0; lIndex < iNumberOfIntervals; lIndex++)
	{
		iIntervals[lIndex] = CInterval::New();
	}
}

//------------------ functions -----------------------

void CGameObjectManager::AddGameObject(CGameObject* aGameObject)
{
	iGameObjects->Append(aGameObject);
	iObjectsNeedInterval->Append(aGameObject);

	for(TInt lIndex = 0; lIndex < iGameObjectsByType->GetCount(); lIndex++)
	{
		if(aGameObject->GetGameObjectType() == iGameObjectsByType->Get(lIndex)->ObjectType)
		{
			//found the correct Array to add it in
			iGameObjectsByType->Get(lIndex)->GameObjects->Append(aGameObject);

			//no need to look further, can return from here
			return;
		}
	}

	//must be a new game Object type that has no been added yet
	SGameObjectTypeArray lNewGameObjectCategory;
	lNewGameObjectCategory.ObjectType = aGameObject->GetGameObjectType();
	lNewGameObjectCategory.GameObjects = CPointerArray<CGameObject>::New();
	lNewGameObjectCategory.GameObjects->Append(aGameObject); //first member

	//add to master Array
	iGameObjectsByType->Append(new SGameObjectTypeArray(lNewGameObjectCategory));
}

void CGameObjectManager::AddMoveableGameObject(CMoveableGameObject* aMoveableGameObject)
{
	iMoveableGameObjects->Append(aMoveableGameObject);

	//is by definition also gameObject
	AddGameObject(aMoveableGameObject);
}

void CGameObjectManager::CheckDeadMoveableGameObjectsForExpiring()
{
	if(iFramesUntilNextPossibleRecycle <= 0) //are waiting for last objects to get recycled?
	{
		//only check if performance is degraded due to overload, so we can remove some objects to make run more smoothly
		if(GMAX_WORK_LOAD_LIMIT_UNTIL_RECYCLING_STARTS < CGame::Game->iFramesPerSecondCounter->GetPercentageOfFrameUsed())
		{
			CPointerArray<CGameObject>* lGameObjects = GetGameObjectsByType(GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS);//only recycle troops and planes
			TInt lNumberOfObjectsAddedToRecycling = 0;

			for (TInt lIndex = 0; lIndex < lGameObjects->GetCount(); lIndex++)
			{
				CMoveableGameObject* lMoveableObject = static_cast<CMoveableGameObject*>(lGameObjects->Get(lIndex));

				if (!lMoveableObject->IsAlive() && !lMoveableObject->GetIsMoving() && lMoveableObject != iPlayer->GetCurrentAircaft()) //don't do anything if player own the object otherwise the object might become NULL while the player still owns it
				{
					TBool& lIsSlowlyRecycledRef = lMoveableObject->GetIsSlowlyRecycledRef();

					if(!lIsSlowlyRecycledRef) //avoid that object gets added several times
					{
						lIsSlowlyRecycledRef = true;
						iMoveableGameObjectsSlowRecycle->Append(lMoveableObject);
						lNumberOfObjectsAddedToRecycling++;

						if(lNumberOfObjectsAddedToRecycling >= iMaxNumberOfObjectsRecycleLimit)
						{
							iMaxNumberOfObjectsRecycleLimit *= 2; //increase limit, incase we have congestion going on
							iFramesUntilNextPossibleRecycle = GFRAMES_UNTIL_NEXT_RECYCLE_CIRCLE;
							break;
						}
					}
				}
			}

			//clean up
			delete lGameObjects;
		}
		else
		{
			iMaxNumberOfObjectsRecycleLimit = GNUMBER_OF_MOVEABLE_OBJECTS_GET_RECYCLED_AT_A_TIME; //work has decreased, set to back to normal
		}
	}
	else
	{
		iFramesUntilNextPossibleRecycle--;
	}

	//fade out objects that have been determined to be recycled
	for(TInt lIndex = 0; lIndex < iMoveableGameObjectsSlowRecycle->GetCount(); lIndex++)
	{
		CMoveableGameObject* lMoveableObject = iMoveableGameObjectsSlowRecycle->Get(lIndex);
		TInt& lFramesLeft = lMoveableObject->GetFramesToLive();

		if(lMoveableObject->RecycleObject()) //make sure that object did not get recycled by something else
		{
			iMoveableGameObjectsSlowRecycle->Remove(lIndex);
			lIndex--; //to avoid that next object gets skipped
		}
		else if(lFramesLeft <= 0)
		{
			lMoveableObject->SetRecycleObject(true);
			iMoveableGameObjectsSlowRecycle->Remove(lIndex);
			lIndex--; //to avoid that next object gets skipped
		}
		else
		{
			lFramesLeft--;

			//change fade of object
			lMoveableObject->SetAlpha((lFramesLeft * 100) / (GFRAMES_PER_SECOND_BACKEND * GSECONDS_UNTIL_DEAD_MOVEABLE_GAME_OBJECT_GETS_RECYCLED));
		}
	}
}

void CGameObjectManager::DrawIntervals()
{
	CFighterPilotThePacificWar::FighterGame->iGraphicsEngine->SetDrawColourFloat(0, 0, 1, 1);
	for(TIntFloat lDrawLocation = -iWidth / 2; lDrawLocation < iWidth.GetIntInBaseInt(); lDrawLocation += iSizeOfEachInterval)
	{

		TPointIntFloat lLowerPoint, lUpperPoint;

		//both the same location
		lLowerPoint.iX = lDrawLocation;
		lUpperPoint.iX = lDrawLocation;
		//differs
		lLowerPoint.iY = TIntFloat::Convert(0);
		lUpperPoint.iY = TIntFloat::Convert(2000);

		CFighterPilotThePacificWar::FighterGame->iGraphicsEngine->DrawLine(lLowerPoint, lUpperPoint);
	}
}

void CGameObjectManager::GrabageCollectFlaggedObjects()
{
	//check GameObjects sorted by type
	for(TInt lIndexOuter = 0; lIndexOuter < iGameObjectsByType->GetCount(); lIndexOuter++)
	{
		CPointerArray<CGameObject>* lGameObjectsInCategory = iGameObjectsByType->Get(lIndexOuter)->GameObjects;

		for(TInt lIndexInner = 0; lIndexInner < lGameObjectsInCategory->GetCount(); lIndexInner++)
		{
			if(lGameObjectsInCategory->Get(lIndexInner)->RecycleObject())
			{
				lGameObjectsInCategory->Remove(lIndexInner);
				lIndexInner--;
			}
		}
	}

	//check MoveableObjects
	for(TInt lIndex = 0; lIndex < iMoveableGameObjects->GetCount(); lIndex++)
	{
		if(iMoveableGameObjects->Get(lIndex)->RecycleObject())
		{
			iMoveableGameObjects->Remove(lIndex);//remove method does not call destructor of object
			lIndex--;//since count has been reduced by one, we have to move the index one back
		}
	}

	//objects in slow recycle
	//remove them as well since object will be undefined after destruction
	for(TInt lIndex = 0; lIndex < iMoveableGameObjectsSlowRecycle->GetCount(); lIndex++)
	{
		if(iMoveableGameObjectsSlowRecycle->Get(lIndex)->RecycleObject())
		{
			iMoveableGameObjectsSlowRecycle->Remove(lIndex);//remove method does not call destructor of object
			lIndex--;//since count has been reduced by one, we have to move the index one back
		}
	}

	//objects that need new interval
	for(TInt lIndex = 0; lIndex < iObjectsNeedInterval->GetCount(); lIndex++)
	{
		if(iObjectsNeedInterval->Get(lIndex)->RecycleObject())
		{
			iObjectsNeedInterval->Remove(lIndex);//remove method does not call destructor of object
			lIndex--;//since count has been reduced by one, we have to move the index one back
		}
	}

	//check GameObjects
	for(TInt lIndex = 0; lIndex < iGameObjects->GetCount(); lIndex++)
	{
		if(iGameObjects->Get(lIndex)->RecycleObject())
		{
			iGameObjects->RemoveAndDestroy(lIndex);//also delete objects itself
			lIndex--;//since count has been reduced by one, we have to move the index one back
		}
	}
}

CPointerArray<CGameObject>*& CGameObjectManager::GetAllGameObjects()
{
	return iGameObjects;
}

CPointerArray<CMoveableGameObject>*& CGameObjectManager::GetAllMoveableGameObjects()
{
	return iMoveableGameObjects;
}

CPointerArray<CGameObject>* CGameObjectManager::GetGameObjectsByType(TGameObjectType aTypes)
{
	CPointerArray<CGameObject>* lWantedObjects =  CPointerArray<CGameObject>::New();

	for(TInt lIndexOuter = 0; lIndexOuter < iGameObjectsByType->GetCount(); lIndexOuter++)
	{
		if(iGameObjectsByType->Get(lIndexOuter)->ObjectType & aTypes)
		{
			//needed by Caller, add all objects, copy entire array
			lWantedObjects->AppendArray(*iGameObjectsByType->Get(lIndexOuter)->GameObjects);
		}
	}

	return lWantedObjects;
}

void CGameObjectManager::RemoveAllGameObjects()
{
	for(TInt lIndex = 0; lIndex < iGameObjects->GetCount(); lIndex++)
	{
		iGameObjects->Get(lIndex)->SetRecycleObject(true);
	}

	GrabageCollectFlaggedObjects();//removes the objects
	ResetAllIntervals(); //since objects are gone, we should reset this as well
}

void CGameObjectManager::ResetAllIntervals()
{
	for(TInt lCurrentInterval = 0; lCurrentInterval < iNumberOfIntervals; lCurrentInterval++)
	{
		iIntervals[lCurrentInterval]->RemoveAllGameObjects();
	}
}

void CGameObjectManager::UpdateIntervals()
{
	//go through intervals and remove objects that are moveable or that or that are set for recycling
	for(TInt lCurrentInterval = 0; lCurrentInterval < iNumberOfIntervals; lCurrentInterval++)
	{
		CInterval* lInterval = iIntervals[lCurrentInterval];
		TInt lNumberOfObjectsInInterval = lInterval->GetNumberOfGameObjects(); // how many objects do we need to check in this interval
		CPointerArray<CGameObject>*& lAllGameObjects = lInterval->GetAllGameObjects();

		for(TInt lCurrentGameObjectIndex = 0; lCurrentGameObjectIndex < lNumberOfObjectsInInterval; lCurrentGameObjectIndex++)
		{
			CGameObject* lGameObject = lAllGameObjects->Get(lCurrentGameObjectIndex);

			if(lGameObject->RecycleObject()) //due to be recycled, remove from interval and not added again
			{
				//remove from interval
				lInterval->RemoveGameObject(lGameObject);

				lCurrentGameObjectIndex--; //size decreased by one
				lNumberOfObjectsInInterval--; //size decreased by one
			}
			else if(lGameObject->GetIsMoveableGameObject())
			{
				//remove from interval
				lInterval->RemoveGameObject(lGameObject);
				//needs to get added again since it is moveable
				TBool& lAlreadyAdded = lGameObject->GetAlreadyAddedToInterval();
				lAlreadyAdded = false; //to set this to avoid that objects get added several times, since they might be located in more than one interval
				iObjectsNeedInterval->Append(lGameObject);

				lCurrentGameObjectIndex--; //size decreased by one
				lNumberOfObjectsInInterval--; //size decreased by one
			}
		}
	}

	//add objects to new intervals
	TIntFloat lCurrentObjectsPosition;
	TIntFloat lDistanceWithinInterval;
	TInt lIntervalLocatedIn;
	TInt lExtraIntervals;
	TInt lNumberOfObjects = iObjectsNeedInterval->GetCount();
	CGameObject* lGameObject;
	for(TInt lIndex = 0; lIndex < lNumberOfObjects; lIndex++)
	{
		lGameObject = iObjectsNeedInterval->Get(lIndex);
		TBool& lAlreadyAdded = lGameObject->GetAlreadyAddedToInterval();

		if(!lAlreadyAdded && !lGameObject->RecycleObject())
		{
			lCurrentObjectsPosition = lGameObject->GetCurrentPosition().iX;
			//adjust position
			lCurrentObjectsPosition += iWidth / 2;// since the map is split into half at the X position 0
			//calculates what interval the object should be added to, depending on X position and adds it to that interval
			lIntervalLocatedIn = (lCurrentObjectsPosition / iSizeOfEachInterval).GetIntInBaseInt();//get rid of floatPoint
			lDistanceWithinInterval = lCurrentObjectsPosition -  iSizeOfEachInterval * lIntervalLocatedIn;//must be at least 0 and less than iSizeOfEachInterval

			if(lGameObject->IsReflected())
			{ //the point is to the right of the Object, we need to check if the Object extends into intervals to the left
				//the idea here is,if the Width is bigger than DistanceWithinInterval then it must be also in the interval to the left, and afterwards if the width is also bigger than futher Intervals
				lExtraIntervals = (((iSizeOfEachInterval + lGameObject->GetWidth()) - lDistanceWithinInterval) / iSizeOfEachInterval).GetIntInBaseInt();
				for(TInt lExtraIntervalLocatedIn = 0; lExtraIntervalLocatedIn < lExtraIntervals + 1; lExtraIntervalLocatedIn++)
				{//will add it at least to the interval it is located in and intervals to the right if the width of the object extends into the next intervals
					iIntervals[lIntervalLocatedIn - lExtraIntervalLocatedIn]->AddGameObject(lGameObject);
				}
			}else
			{ // since the locationPoint is located at the bottom left of the Object, we need to check if it extends to the right into other intervals as well
				// since every X further in the interval moves as closer to the next Interval to the right, we need to add it to the Width of the Object
				lExtraIntervals = ((lDistanceWithinInterval + lGameObject->GetWidth()) / iSizeOfEachInterval).GetIntInBaseInt();
				for(TInt lExtraIntervalLocatedIn = 0; lExtraIntervalLocatedIn < lExtraIntervals + 1; lExtraIntervalLocatedIn++)
				{//will add it at least to the interval it is located in and intervals to the right if the width of the object extends into the next intervals
					iIntervals[lIntervalLocatedIn + lExtraIntervalLocatedIn]->AddGameObject(lGameObject);
				}
			}

			lAlreadyAdded = true;//this avoids that objects added twice, since if is in another interval, this flag will stop adding it again
		}
	}

	//all objects have been added
	iObjectsNeedInterval->Clear();
}

void CGameObjectManager::CheckGameObjectsForCollisions()
{
	//go through all Intervals
	for(TInt lCurrentInterval = 0; lCurrentInterval < iNumberOfIntervals; lCurrentInterval++)
	{
		//in this specific interval take the Object from the top and check if it collides with any objects below
		TInt lNumberOfObjectsInInterval = iIntervals[lCurrentInterval]->GetNumberOfGameObjects();

		if(lNumberOfObjectsInInterval > 1) // otherwise no collision possible
		{
			CPointerArray<CGameObject>*& lGameObjectsInInterval = iIntervals[lCurrentInterval]->GetAllGameObjects();
			//HACK!!!
			//we know that the floor and other non-moveable objects will always be first in the array
			//however we want that they check last since it could cause problems if they go first
			//like a bomb that intsects the floor and object, might just explode on the floor, or a plane landing on a surface and crashing into the ground
			for(TInt lIndex = lNumberOfObjectsInInterval - 1; lIndex >= 0; lIndex--)
				lGameObjectsInInterval->Get(lIndex)->PossibleCollision(iIntervals[lCurrentInterval]);//objects need to make sure not to collide with themselves
		}
	}
}

CPointerArray<CGameObject>* CGameObjectManager::ReturnObjectsInRange(TIntFloat aPosition, TIntFloat aRange, TGameObjectType aGameObjectType)
{
	TInt lIntervalLocatedIn;
	TIntFloat lDistanceWithinInterval;
	CPointerArray<CGameObject>* lResult = CPointerArray<CGameObject>::New() ;
	TInt lIntervalsSpansToTheLeft;
	TInt lIntervalsSpansToTheRight;

	//adjust position to have no negative positions
	if(aPosition < 0)
		aPosition = iWidth / 2 + aPosition;
	else
		aPosition += iWidth / 2;

	lIntervalLocatedIn = (aPosition / iSizeOfEachInterval).GetIntInBaseInt();//get rid of floatPoint
	lDistanceWithinInterval = aPosition - iSizeOfEachInterval * lIntervalLocatedIn;//must be at least 0 and less than iSizeOfEachInterval
	lIntervalsSpansToTheLeft = ((iSizeOfEachInterval - lDistanceWithinInterval + aRange) / iSizeOfEachInterval).GetIntInBaseInt();//each X further into interval moves us further away from the left intervals
	lIntervalsSpansToTheRight = ((lDistanceWithinInterval + aRange) / iSizeOfEachInterval).GetIntInBaseInt();//each x further into interval moves us closer the right intervals

	//add all Objects in the Current Interval
	CPointerArray<CGameObject>* lGameObjectsInInterval = iIntervals[lIntervalLocatedIn]->GetGameObjectsByType(aGameObjectType);
	lResult->AppendArray(*lGameObjectsInInterval);

	//clean up
	delete lGameObjectsInInterval;

	//add all objects from intervals that the range spans into, for LEFT
	for(TInt lIntervalToTheLeft = 1; lIntervalToTheLeft < lIntervalsSpansToTheLeft + 1; lIntervalToTheLeft++)
	{
		TInt lExtraInterval = lIntervalLocatedIn - lIntervalToTheLeft;
		if(lExtraInterval > -1)//don't want to go outside of array
		{
			CPointerArray<CGameObject>* lGameObjectsInLeft = iIntervals[lExtraInterval]->GetGameObjectsByType(aGameObjectType);
			lResult->AppendArray(*lGameObjectsInLeft);

			//clean up
			delete lGameObjectsInLeft;
		}
	}

	//do the same for the right, for RIGHT
	for(TInt lIntervalToTheRight = 1; lIntervalToTheRight < lIntervalsSpansToTheRight + 1; lIntervalToTheRight++)
	{
		TInt lExtraInterval = lIntervalLocatedIn + lIntervalToTheRight;
		if(lExtraInterval < iNumberOfIntervals)//don't want to go outside of array
		{
			CPointerArray<CGameObject>* lGameObjectsInRight = iIntervals[lExtraInterval]->GetGameObjectsByType(aGameObjectType);
			lResult->AppendArray(*lGameObjectsInRight);

			//clean up
			delete lGameObjectsInRight;
		}
	}

	//------------ remove duplicate entries -----------//
	lResult->SortMerge(CGameObjectManager::SortPointers);

	if(lResult->GetCount() > 1) //go through sorted list and remove duplicate pointers
	{
		CGameObject* lTarget = lResult->Get(0);

		for(TInt lIndex = 1; lIndex < lResult->GetCount(); lIndex++)
		{
			CGameObject* lInspectObject = lResult->Get(lIndex);

			if(lInspectObject == lTarget) //check if duplicate pointer, since sorted by Pointer value duplicate should be adjacent to each other
			{
				//remove duplicate
				lResult->Remove(lIndex);
				lIndex--; //reduce count to avoid skip
			}
			else
			{
				//not duplicate, since sorted there can be no more duplicates in this array, check for next object
				lTarget = lInspectObject;
			}
		}
	}

	return lResult;//should contain all objects that are in reach of the range relative to the position,which will also include the object itself
}

void CGameObjectManager::CheckIfGameObjectsAreLeavingTheMap()
{
	//check how many Intervals are contained within the Safety Margin on the left of the map
	TInt lNumberOfIntervalsInSafetyMargin = (GMAP_SAFETY_MARGIN / (iSizeOfEachInterval.GetIntInBaseInt())) + 1;//usually will not be of common factor, so we add one to round up

	//check in the first and last interval for aircafts and destroy them, that is they left the map. Currently used to remove heavy bombers after their attack run
	for(TInt lCurrentObject = 0; lCurrentObject < iIntervals[0]->GetNumberOfGameObjects(); lCurrentObject++) // left most interval
	{
		CGameObject* lObject = iIntervals[0]->GetAllGameObjects()->Get(lCurrentObject);
		if(lObject->GetGameObjectType() != EObjectTypesFloor)
			lObject->SetRecycleObject(true);
	}
	for(TInt lCurrentObject = 0; lCurrentObject < iIntervals[iNumberOfIntervals - 1]->GetNumberOfGameObjects(); lCurrentObject++)// right most interval
	{
		CGameObject* lObject = iIntervals[iNumberOfIntervals - 1]->GetAllGameObjects()->Get(lCurrentObject);
		if(lObject->GetGameObjectType() != EObjectTypesFloor)
			lObject->SetRecycleObject(true);
	}
	for(TInt lCurrentMoveableGameObjectIndex = 0; lCurrentMoveableGameObjectIndex < iMoveableGameObjects->GetCount(); lCurrentMoveableGameObjectIndex++)// check if object is too high or too low
	{
		CMoveableGameObject* lMoveableGameObject = iMoveableGameObjects->Get(lCurrentMoveableGameObjectIndex);
		TPoint lCurrentPosition = lMoveableGameObject->GetCurrentPositionNormilized();

		//perform check if moveable object is sinking too low
		if(lCurrentPosition.iY < MIN_Y_VALUE_BEFORE_GETTING_RECYCLED)
			if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetCurrentAircaft() != lMoveableGameObject && CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetPlayerBase() != lMoveableGameObject) //otherwise game will crash
				lMoveableGameObject->SetRecycleObject(true);
			else
				//else this object is related to the player and should stop sinking
				static_cast<CAircraft*> (lMoveableGameObject)->SetIsSinking(false);//stop it from sinking any further

		//perform check if plane is flying too high
		if((lMoveableGameObject->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane)))
		{
			CAircraft* lCurrentAircraft = static_cast<CAircraft*> (lMoveableGameObject);
			if(lCurrentAircraft->IsUpSideDown())
				lCurrentPosition.iY -= lCurrentAircraft->GetHeight().GetIntInBaseInt();//adjust height because lower point is now above

			if(lCurrentPosition.iY > GMAX_HEIGHT_IN_GAME_LEVEL_FOR_PLANE && !lCurrentAircraft->IsTurning())
			{
				if(lCurrentAircraft->GetIsAIControlled())
				{
					if(lCurrentAircraft->IsReflected())
					{
						lCurrentAircraft->TurnLeftWithoutAnimationAndCheck();
					}else
					{
						lCurrentAircraft->TurnRightWithoutAnimationAndCheck();
					}
				}else
				{
					//check angle what side the plane to turn to
					TInt lPlaneCurrentAngel = *lCurrentAircraft->GetFlyingAngel();
					if(lPlaneCurrentAngel > 90 && lPlaneCurrentAngel <= 270)
					{
						//turn to 180 degrees since it is closer
						while(*lCurrentAircraft->GetFlyingAngel() != 180)
							lCurrentAircraft->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
					}else
					{
						//turn to 0 degrees since it is closer
						while(*lCurrentAircraft->GetFlyingAngel() != 0)
							lCurrentAircraft->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
					}

					TIntFloat lNeededChange = lCurrentAircraft->GetCurrentPosition().iY - TIntFloat::Convert(GMAX_HEIGHT_IN_GAME_LEVEL_FOR_PLANE);
					if(lCurrentAircraft->IsUpSideDown())
						lNeededChange -= lCurrentAircraft->GetHeight();//adjust height because lower point is now above

					lCurrentAircraft->ChangeYPosition(-lNeededChange);
				}
			}
		}
	}

	//check how many Intervals are contained within the Safety Margin on the left of the map
	for(TInt lCurrentInterval = 1; lCurrentInterval < lNumberOfIntervalsInSafetyMargin + 1; lCurrentInterval++)
		for(TInt lCurrentObject = 0; lCurrentObject < iIntervals[lCurrentInterval]->GetNumberOfGameObjects(); lCurrentObject++)
		{
			CGameObject* lObject = iIntervals[lCurrentInterval]->GetAllGameObjects()->Get(lCurrentObject);
			//check if of type plane, in that case we want to turn the object around
			if(lObject->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane))
			{
				CAircraft* lAircraft = static_cast<CAircraft*> (lObject); //destroyed bombers don't slide off the map
				if((!lAircraft->IsAlive() || lAircraft->GetAircraftType() != EAircraftTypeHeavyBomber) && !lAircraft->IsTurning() && (*lAircraft->GetFlyingAngel() > 90 && *lAircraft->GetFlyingAngel() < 270))//plane points to left of the map
				{
					//need to turn aircarft, since a plane can be reflect but have made 180 degree turn
					if(lAircraft->IsReflected())
						lAircraft->TurnLeftWithoutAnimationAndCheck();
					else
						lAircraft->TurnRightWithoutAnimationAndCheck();
				}else if(*lAircraft->GetFlyingAngel() > 90 && *lAircraft->GetFlyingAngel() < 270) //is moving to the left of the map
					lAircraft->SetIsLeavingMap(true); //only applies for heavy bombers
			}else if(lObject->GetGameObjectType() & (EObjectTypesProjectile | EObjectTypesProjectileExplosive))
			{
				CProjectile* lProjectile = static_cast<CProjectile*> (lObject);
				lProjectile->Destruct();
			}else if(lObject->GetGameObjectType() & (EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesShip))//any other type moveable object
			{
				if(!lObject->IsReflected())
					static_cast<CMoveableGameObject*> (lObject)->SetIsLeavingMap(true); //some Event might want to check is any object leaves the map or not
			}
		}
	//check how many Intervals are contained within the Safety Margin on the right of the map
	for(TInt lCurrentInterval = iNumberOfIntervals - 2; lCurrentInterval > iNumberOfIntervals - lNumberOfIntervalsInSafetyMargin - 2; lCurrentInterval--)
		for(TInt lCurrentObject = 0; lCurrentObject < iIntervals[lCurrentInterval]->GetNumberOfGameObjects(); lCurrentObject++)
		{
			CGameObject* lObject = iIntervals[lCurrentInterval]->GetAllGameObjects()->Get(lCurrentObject);
			//check if of type plane, in that case we want to turn the object around
			if(lObject->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane))
			{
				CAircraft* lAircraft = static_cast<CAircraft*> (lObject);
				if((!lAircraft->IsAlive() || lAircraft->GetAircraftType() != EAircraftTypeHeavyBomber) && !lAircraft->IsTurning() && (*lAircraft->GetFlyingAngel() < 90 || *lAircraft->GetFlyingAngel() > 270))//plane points to right of the map
				{
					//need to turn aircarft, since a plane can be reflect but have made 180 degree turn
					if(lAircraft->IsReflected())
						lAircraft->TurnLeftWithoutAnimationAndCheck();
					else
						lAircraft->TurnRightWithoutAnimationAndCheck();
				}else if(*lAircraft->GetFlyingAngel() < 90 || *lAircraft->GetFlyingAngel() > 270) //is moving to the right of the map
					lAircraft->SetIsLeavingMap(true); //only applies for heavy bombers
			}else if(lObject->GetGameObjectType() & (EObjectTypesProjectile | EObjectTypesProjectileExplosive))
			{
				CProjectile* lProjectile = static_cast<CProjectile*> (lObject);
				lProjectile->Destruct();
			}else if(lObject->GetGameObjectType() & (EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesShip))//any other type moveable object
			{
				if(lObject->IsReflected())
					static_cast<CMoveableGameObject*> (lObject)->SetIsLeavingMap(true);
			}
		}
}

TInt CGameObjectManager::SortPointers(const CGameObject* aObject1, const CGameObject* aObject2)
{
	return aObject1 < aObject2; //uses actual pointer value
}

void CGameObjectManager::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteIntFloat(iWidth);
	aOutputStream.WriteIntFloat(iSizeOfEachInterval);
	aOutputStream.WriteInt32(iNumberOfIntervals);
}

void CGameObjectManager::LoadFromDisk(CFileReadStream &aReadStream)
{
	iWidth = aReadStream.ReadIntFloat();
	iSizeOfEachInterval = aReadStream.ReadIntFloat();
	iNumberOfIntervals = aReadStream.ReadInt32();
}
