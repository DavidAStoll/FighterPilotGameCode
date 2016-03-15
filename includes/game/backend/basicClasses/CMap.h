/*
 * CMap.h
 *
 *  Created on: Aug 25, 2012
 *      Author: dstoll
 */

#ifndef CMAP_H_
#define CMAP_H_

#include "includes/game/backend/basicClasses/CEvent.h"
#include "includes/game/backend/CGameObjectManager.h"
#include "includes/game/backend/CPlayer.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/backend/CClockTimer.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CGraphicsEngine.h"

struct SNewToOldPointerMapping
{
	TUint32 OldPointer;
	TUint32 NewPointer;
};
struct SSetPointerToNewValue
{
	TUint32 OldPointer;
	TUint32 SetPointer;
};

// CLASS DECLARATION

/**
 *  CMap
 *
 *  This class will be inheritaed by all maps, it contains all functionallity that a map needs, but certain methods will need to be overrided,
 *  since each map will have a different initlization method, as well as size and gameobject and events
 */
class CMap
{
public:

	//returns the Object Manager, might be used by the engine to update the member data
	CGameObjectManager* GetObjectManager();
	//returns the event array, that holds all current events in the map
	CPointerArray<CEvent>* GetEvents();
	//add clock timer object
	void AddClockTimerObject(CClockTimer * aObject);
	//deletes the map, destroys all content of the map and unloads all textures needed for the map
	void DeleteMap();
	//draws clocks in order
	void DrawClockTimers();
	//runs the update code of the map, basically will just Check if any events will occur and run their code
	void Update();
	//just returns the current player
	CPlayer* GetPlayer();
	//returns how many Frames in the game have already passed
	TInt GetCurrentFrame();
	//returns how many Seconds in the Game have already passed
	TInt GetCurrentGameTimeInSeconds();
	//just a wrapper function around the GameObjects Add MoveabelObject
	void AddMoveableObject(CMoveableGameObject* aObject);
	//just a wrapper function around the GameObjectManger's AddGameObject
	void AddGameObject(CGameObject* aObject);
	//just a wrapper function to add an Event to the iEvents array
	void AddEventObject(CEvent* aEvent);
	//add an Always DrawObject, has the specific feature that this object will always be drawn no matter where the current view is located
	CPointerArray<CGameObject>* GetAllDrawObjectsInRange(TIntFloat aPosition, TIntFloat aRange);
	//retunrs the X coordinate where new objects should appear that want to enter the map from the left
	TInt GetLeftSideSpawningAreaXCoordinate(TInt aObjectWidth);
	//returns the X coordiante where new object should apper that want to enter the map from the right
	TInt GetRightSideSpawningAreaXCoordinate(TInt aObjectWidth);
	//returns the X coordinate for left most X point, where planes will be turned around
	TInt GetLeftXOutermostCoordinate();
	//returns the X coordinate for right most X point, where planes will be turned around
	TInt GetRightXOutermostCoordinate();
	//This method should be called if the mission is to be ended, it will be used to clean up certain states before ending the mission can be successfully be done
	void EndMissionPreCall();
	//saves the entire state of the game on disk
	//Loads all the gameObject from a saveGame file and adds them to the gameObject array
	void LoadGameObjects(CFileReadStream& aReadStream);
	//load all events from the saveGame file
	void LoadEvents(CFileReadStream& aReadStream);
	//deletes all objects and the map and the player and loads everything up like at the beginning of the map
	void RestartMap();
	//save the current game to file
	void SaveGame(const char* aSaveGameFileName);
	//loads the entire game from disk, from the current level
	void LoadGame(const char* aSaveGameFileName);
	//save the map properties that we need to reset
	void SaveMapData(CFileWriteStream& aWriteStream);
	//Loads the map data and resets the current one
	void LoadMapData(CFileReadStream& aReadStream);
	//return how many minutes a player should play to beat this mission
	TInt GetExpectedNumberOfMinutesForMap();
	//return current Map Id
	TGameMap GetMapId();
	//returns true if this map is used in a tutorial
	TBool GetIsTutorialMap();
	//add TextureIdentifiers to the global array
	void AddPointerMapping(TUint32 aOldPointer, TUint32 aNewPointer);
	//finds the new pointer value
	TUint32 FindNewPointerValue(TUint32 aOldPointer);
	//finds out the OpenGL Identifer
	void AddNewPointerSettingInstruction(TUint32 aOldPointer, TUint32 aSetPointer);
	//deletes the entire array
	void RemoveMappingAndPointerSettingInstructions();
	//fixes the pointers
	void SetPointersToNewValues();
	//saves a dynamic array to disk
	static void SaveDynamicArray(CPointerArray<void>* aPointerArray, CFileWriteStream& aOutputStream);
	//loads a dynamic array from disk
	static void LoadDynamicArray(CPointerArray<void>* aPointerArray, CFileReadStream& aReadStream);
	//check if saveGameFile exists
	static TBool CheckIfSaveGameExists(const char* aFileName);
	//return difficulty of game
	static TGameDifficulty GetGameDifficulty(const char* aFileName);
	//return mapID of save game file
	static TGameMap GetMapIdFromSaveGameFile(const char* aFileName);

protected:

	//making it protected, since we don't want that map gets deleted through the normal delete keyword
	virtual ~CMap();
	//removes all events and objects from the map and resets it back to is beginng state
	void ClearMap();
	//contains method calls that need to be right after the Map object has been created
	void Construct();
	//this is the generic Map method that will call the map specific one and some methods that always need to be called
	void InitializeGameObjectSettings();
	//does the actual loading of textures and other settings for GameObjects, is map specific
	virtual void InitializeGameObjectSettingsForMap() = 0;
	//runs the code that creates all relevant map data
	virtual void InitializeMap() = 0;
	//contains the code that needs to be called to Update the Map after Initialization
	void PostMapInit();
	//this is the generic Map method that will call the map specific one and some methods that always need to be called
	void RemoveGameObjectSettings();
	//does the actual unloading of textures, other settings for GameObjects, is map specific
	virtual void RemoveGameObjectSettingsForMap() = 0;

	TUint iSize;//the total length of the map, should be a even number, since it will be split in half and less than 2^16 due the TIntFloat contraints
	TInt iNumberOfIntervals;// how many intervals the map will be subdivied
	TInt iCurrentFrame;
	TInt iExpectedMissionTimeInMins;
	TBool iMapAlreadyInitilized;
	TBool iTexturesAlreadyLoaded;
	TBool iIsTutorialMap;
	TGameMap iGameMapId;
	CPointerArray<CEvent>* iEvents;//containts all the events that will happen
	CPointerArray<CClockTimer>* iClockTimerObjects;
	CGameObjectManager* iGameObjectManager;
	CPlayer* iPlayer;//the player in the map

	CPointerArray<SNewToOldPointerMapping>* iOldToNewPointerMapper; //keeps track of the mapping, what new value the pointer has
	CPointerArray<SSetPointerToNewValue>* iPointersThatNeedToBeSet; // keeps track of pointers that need to be set to the new pointer value

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMap(TGameMap aGameMapId, TUint aSize, TInt aNumberOfIntervals, TInt aExpectedMissionTimeInMins);
};

#endif /* CMAP_H_ */
