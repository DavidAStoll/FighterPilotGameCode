/*
 * CGameObjectManager.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CGAMEOBJECTMANAGER_H_
#define CGAMEOBJECTMANAGER_H_

// INCLUDES


#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/game/backend/CInterval.h"
#include "includes/game/backend/basicClasses/CGameObject.h"

class CMoveableGameObject;
class CGameObject;
class CPlayer;

// CLASS DECLARATION

/**
 *  CGameObjectManager
 *
 *  This class deals with all the game objects in the game, especially what their position is within a map and collision detection
 */
class CGameObjectManager
{

public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGameObjectManager();

	static CGameObjectManager* New(TUint aWidth, TInt aNumberOfIntervals, CPlayer*& aPlayer);
	void AddGameObject(CGameObject* aGameObject);
	void AddMoveableGameObject(CMoveableGameObject* aMoveableGameObject);
	//checks if a dead moveable gameobject is ripe to get recycled
	void CheckDeadMoveableGameObjectsForExpiring();
	//draws all Intervals in the game
	void DrawIntervals();
	//checks if any objects are flagged for recycling
	void GrabageCollectFlaggedObjects();
	//gives direct Access to ALL gameobjects, not a copy but original
	CPointerArray<CGameObject>*& GetAllGameObjects();
	//gives direct Access to ALL MoveableObjects, not a copy but original
	CPointerArray<CMoveableGameObject>*& GetAllMoveableGameObjects();
	//returns an Array that contains all gameObject given with the specific Type(s), array has to be deleted after use by caller
	CPointerArray<CGameObject>* GetGameObjectsByType(TGameObjectType aTypes);
	//this method will very accuretly order all game objects into intervals that subdivided the map depending on their position, therefore making hit detection and searching very fast
	void UpdateIntervals();
	//this method goes through all Objects in its intervals to see if collide with any other objects
	//obviouslt UpdateIntervals must have been be called at least once before commiting a call here
	void CheckGameObjectsForCollisions();
	//this function will return a subset of objects that might a in the range given to a specific position
	//this function will also return the calling object itself in range, therefore it needs to make sure not to detect itself
	//object returned is not owned by anyone, and calling function is responsible for deletion, but just Reset, not the destruction of the Objects pointed to
	CPointerArray<CGameObject>* ReturnObjectsInRange(TIntFloat aPosition, TIntFloat aRange, TGameObjectType aGameObjectType);
	//this method checks First and last interval Turns them so that the objects can't leave the map
	void CheckIfGameObjectsAreLeavingTheMap();
	//destroys all gameobjects added to the object managaer
	void RemoveAllGameObjects();
	void ResetAllIntervals();
	void SaveOnDisk(CFileWriteStream &aReadStream);
	void LoadFromDisk(CFileReadStream &aOutputStream);

	CInterval** iIntervals;// the intervals in wich the game Objects are put
	friend class CInterval;

protected:

	CPlayer*& iPlayer;
	TIntFloat iWidth;//the width of the map
	TIntFloat iSizeOfEachInterval;
	TInt iNumberOfIntervals; //how many intervals the map is split up into
	TInt iFramesUntilNextPossibleRecycle; //wait time until we check if workload is too much to avoid that everything gets recycled because a little spike
	TInt iMaxNumberOfObjectsRecycleLimit; //how many units will be recycled if work overload is detected
	CPointerArray<CGameObject>* iGameObjects;//keeps track of all game objects, will call their update function, recycle them if needed
	CPointerArray<SGameObjectTypeArray>* iGameObjectsByType;
	CPointerArray<CMoveableGameObject>* iMoveableGameObjects;//keeps track of all objects that can move and will call their move function each frame
	CPointerArray<CMoveableGameObject>* iMoveableGameObjectsSlowRecycle;//keeps track of game objects that will be slowly recycled (fading out)
	CPointerArray<CGameObject>* iObjectsNeedInterval; //contains all objects that need to be added to a correct interval, keep track of it to avoid that all objects have to varified all the time
	/**
	 * Constructor for performing 1st stage construction
	 */
	CGameObjectManager(TUint aWidth, TInt aNumberOfIntervals, CPlayer*& aPlayer);
	void ConstructL();
	//sorts Pointers by memory value
	static TInt SortPointers(const CGameObject* aObject1, const CGameObject* aObject2);
};



#endif /* CGAMEOBJECTMANAGER_H_ */
