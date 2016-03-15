/*
 * CInterval.h
 *
 *  Created on: May 18, 2013
 *      Author: dstoll
 */

#ifndef CINTERVAL_H_
#define CINTERVAL_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
class CGameObject;

struct SGameObjectTypeArray
{
	TUint ObjectType;
	CPointerArray<CGameObject>* GameObjects;
};

class CInterval
{

public:
	~CInterval();
	static CInterval* New();
	//adds a GameObject to the interval
	void AddGameObject(CGameObject* aGameObject);
	//returns all GameObjects regardless of Category
	CPointerArray<CGameObject>*& GetAllGameObjects();
	//returns only the gameObjects with the given category
	CPointerArray<CGameObject>* GetGameObjectsByType(TUint aTypes);
	//returns the Number of GameObjects in this interval
	TUint GetNumberOfGameObjects();
	//removes all objects in the interval
	void RemoveAllGameObjects();
	//remove GameObject removes the GameObjects from the interval
	void RemoveGameObject(CGameObject* aGameObject);

protected:
	CInterval();
	void Construct();
	CPointerArray<SGameObjectTypeArray>* iGameObjectsByType;
	CPointerArray<CGameObject>*  iGameObjects;
};


#endif /* CINTERVAL_H_ */
