/*
 * CInterval.cpp
 *
 *  Created on: May 18, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/CInterval.h"
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/game/backend/CGameObjectManager.h"

//---------------------- Intervals Class starts here ---------------------------//

CInterval::CInterval()
{
	iGameObjects = NULL;
	iGameObjectsByType = NULL;
}

CInterval::~CInterval()
{
	if(iGameObjects)
	{
		delete iGameObjects;
		iGameObjects = NULL;
	}
	if(iGameObjectsByType)
	{
		//since structer contains array, we need remove array first
		for(TInt lIndex = 0; lIndex < iGameObjectsByType->GetCount(); lIndex++)
		{
			delete  iGameObjectsByType->Get(lIndex)->GameObjects;
		}

		delete iGameObjectsByType;
		iGameObjectsByType = NULL;
	}
}

CInterval* CInterval::New()
{
	CInterval* lInterval = new CInterval();
	lInterval->Construct();
	return lInterval;
}

void CInterval::Construct()
{
	iGameObjects = CPointerArray<CGameObject>::New();
	iGameObjectsByType = CPointerArray<SGameObjectTypeArray>::New();
}

void CInterval::AddGameObject(CGameObject* aGameObject)
{
	//very simple
	iGameObjects->Append(aGameObject);

	//more complicated :)
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



CPointerArray<CGameObject>*& CInterval::GetAllGameObjects()
{
	return iGameObjects;
}

CPointerArray<CGameObject>* CInterval::GetGameObjectsByType(TGameObjectType aTypes)
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

TUint CInterval::GetNumberOfGameObjects()
{
	return iGameObjects->GetCount();
}

void CInterval::RemoveAllGameObjects()
{
	iGameObjects->Clear();

	for(TInt lIndex = 0; lIndex < iGameObjectsByType->GetCount(); lIndex++)
	{
		iGameObjectsByType->Get(lIndex)->GameObjects->Clear();
	}
}

void CInterval::RemoveGameObject(CGameObject* aGameObject)
{
	//remove from general Object array
	for(TInt lIndex = 0; lIndex < iGameObjects->GetCount(); lIndex++)
	{
		if(aGameObject == iGameObjects->Get(lIndex))
		{
			iGameObjects->Remove(lIndex);
			break; //no need to look further
		}
	}

	//also remove from category one
	for(TInt lIndexOutter = 0; lIndexOutter < iGameObjectsByType->GetCount(); lIndexOutter++)
	{
		if(aGameObject->GetGameObjectType() == iGameObjectsByType->Get(lIndexOutter)->ObjectType)
		{
			//found category where object is located
			CPointerArray<CGameObject>* lGameObjects = iGameObjectsByType->Get(lIndexOutter)->GameObjects;

			for(TInt lIndexInner = 0; lIndexInner < lGameObjects->GetCount(); lIndexInner++)
			{
				if(lGameObjects->Get(lIndexInner) == aGameObject)
				{
					lGameObjects->Remove(lIndexInner);
					return; //found the target
				}
			}
		}
	}
}


