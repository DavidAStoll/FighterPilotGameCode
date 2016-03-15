/*
 ============================================================================
 Name		: Map.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMap implementation
 ============================================================================
 */
#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
#include "includes/game/backend/gameObjects/Maps/GameMaps.h"
#include "includes/game/backend/CTouchControls.h"
#include "includes/game/CFighterPilotThePacificWar.h"
//views
#include "includes/game/views/CGameSettingsView.h"
#include "includes/game/views/CChangeSongView.h"
#include "includes/game/views/CGamePauseView.h"

CMap::CMap(TGameMap aGameMapId, TUint aSize, TInt aNumberOfIntervals, TInt aExpectedMissionTimeInMins)
{
	//these will be set in the initlization phase
	iGameObjectManager = NULL;
	iPlayer = NULL;
	iExpectedMissionTimeInMins = aExpectedMissionTimeInMins;
	iGameMapId = aGameMapId;
	iSize = aSize;
	iNumberOfIntervals = aNumberOfIntervals;
	iCurrentFrame = 0;
	iMapAlreadyInitilized = false;
	iTexturesAlreadyLoaded = false;
	iIsTutorialMap = false; //by default, false. If true, it will load some extra texture just needed for the tutorial menu
}

CMap::~CMap()
{
	if(iOldToNewPointerMapper != NULL)
	{
		iOldToNewPointerMapper->ClearAndDestroy();
		delete iOldToNewPointerMapper;
		iOldToNewPointerMapper = NULL;
	}
	if(iPointersThatNeedToBeSet != NULL)
	{
		iPointersThatNeedToBeSet->ClearAndDestroy();
		delete iPointersThatNeedToBeSet;
		iPointersThatNeedToBeSet = NULL;
	}
	if(iEvents != NULL)
	{
		iEvents->ClearAndDestroy();
		delete iEvents;
		iEvents = NULL;
	}
	if(iGameObjectManager)
	{
		delete iGameObjectManager;
		iGameObjectManager = NULL;
	}
	if(iPlayer)
	{
		delete iPlayer;
		iPlayer = NULL;
	}
	if(iClockTimerObjects != NULL)
	{
		iClockTimerObjects->ClearAndDestroy();
		delete iClockTimerObjects;
		iClockTimerObjects = NULL;
	}
}

void CMap::Construct()
{
	iGameObjectManager = CGameObjectManager::New(iSize,iNumberOfIntervals, iPlayer);
	iEvents = CPointerArray<CEvent>::New();
	iClockTimerObjects = CPointerArray<CClockTimer>::New();
	iOldToNewPointerMapper = CPointerArray<SNewToOldPointerMapping>::New(); //keeps track of the mapping, what new value the pointer has
	iPointersThatNeedToBeSet = CPointerArray<SSetPointerToNewValue>::New();
	CGameObjectDefaultValues::LoadDefaultValues();
	InitializeGameObjectSettings();
	//Some Objects use this reference to add Objects to the map, those we need to set it here
	CFighterPilotThePacificWar::FighterGame->iGameData->iGameMap = this;
	InitializeMap();
	PostMapInit();
}

//--------------functions -------------------

CGameObjectManager* CMap::GetObjectManager()
{
	return iGameObjectManager;
}

CPointerArray<CEvent>* CMap::GetEvents()
{
	return iEvents;
}

void CMap::Update()
{
	//update the current time in the map
	iCurrentFrame++;
	//go through all events to see if any of them are ready to be triggered
	for(TInt lCurrentEvent = 0; lCurrentEvent < iEvents->GetCount(); lCurrentEvent++)
	{
		CEvent* lEvent = iEvents->Get(lCurrentEvent);
		if(lEvent->CheckIfEventConditionIsMet())
		{
			lEvent->TriggerEvent();
			iEvents->RemoveAndDestroy(lCurrentEvent);
			lCurrentEvent--;//since we just removed an items we have to move back the index
		}
	}
}

void CMap::AddClockTimerObject(CClockTimer * aObject)
{
	iClockTimerObjects->Append(aObject);
}

void CMap::DeleteMap()
{
	ClearMap();
	RemoveGameObjectSettings();
	delete this;
}

void CMap::DrawClockTimers()
{
	for(TInt lCurrentClockObject = 0; lCurrentClockObject < iClockTimerObjects->GetCount(); lCurrentClockObject++)
		iClockTimerObjects->Get(lCurrentClockObject)->Draw();
}

CPlayer* CMap::GetPlayer()
{
	return iPlayer;
}

TInt CMap::GetCurrentFrame()
{
	return iCurrentFrame;
}

TInt CMap::GetLeftSideSpawningAreaXCoordinate(TInt aObjectWidth)
{
	TInt lSizeOfEachInterval = iSize / iNumberOfIntervals;
	TInt lSafetyMargin = GMAP_SAFETY_MARGIN;
	if(lSafetyMargin < lSizeOfEachInterval)
		lSafetyMargin = lSizeOfEachInterval;

	TInt lXOffsetFromEdge = lSafetyMargin + aObjectWidth + MAP_EXTRA_EDGE_SAFETY_OFFSET; //must be out side removal interval

	return -((iSize / 2) - lXOffsetFromEdge); // since it is at the negative end of the X axis
}

//returns the X coordinate for right most X point, where planes will be turned around
TInt CMap::GetLeftXOutermostCoordinate()
{
	TInt lSizeOfEachInterval = iSize / iNumberOfIntervals;
	TInt lNumberOfIntervalsInSafetyMargin = (GMAP_SAFETY_MARGIN / (lSizeOfEachInterval)) + 1;//has to be at least one

	TInt lOutMostXPoint = (iSize / 2) - (lNumberOfIntervalsInSafetyMargin + 1) * lSizeOfEachInterval;//need to add 1 because the last interval is always skipped
	return -lOutMostXPoint;
}

TInt CMap::GetRightXOutermostCoordinate()
{
	return -GetLeftXOutermostCoordinate();
}

void 	CMap::EndMissionPreCall()
{
	CPointerArray<CGameObject>* lGameObjects = iGameObjectManager->GetGameObjectsByType(EObjectTypesBuilding);

	//right now just update the stats for the buildings
	for(TInt lIndex = 0; lIndex < lGameObjects->GetCount(); lIndex++)
	{
		CGameObject* lBuilding = lGameObjects->Get(lIndex);
		//means it has to be a building
		static_cast<CBuilding*>(lBuilding)->UpdateStatsIfBuildingWasSuccessfullyCaptured();
	}

	//clean up
	delete lGameObjects;
}

TInt CMap::GetRightSideSpawningAreaXCoordinate(TInt aObjectWidth)
{
	TInt lSizeOfEachInterval = iSize / iNumberOfIntervals;
	TInt lSafetyMargin = GMAP_SAFETY_MARGIN;
	if(lSafetyMargin < lSizeOfEachInterval)
		lSafetyMargin = lSizeOfEachInterval;

	TInt lXOffsetFromEdge = lSafetyMargin + aObjectWidth + MAP_EXTRA_EDGE_SAFETY_OFFSET; //must be out side removal interval

	return ((iSize / 2) - lXOffsetFromEdge); // at the positive end of the X axis
}

void CMap::SaveGame(const char* aSaveGameFileName)
{
	CNativeFile* lNativeFile = new CNativeFile(aSaveGameFileName);
	CFileWriteStream* lWriteStream = lNativeFile->OpenForWriting(EFileWriteStreamModeReplace);

	//save mapId
	lWriteStream->WriteInt32(iGameMapId);
	//save difficulty of the game
	lWriteStream->WriteInt32(CFighterPilotThePacificWar::FighterGame->FighterGame->iGameVariables->iDifficulty);

	//save all game objects that exists to file
	CPointerArray<CGameObject>*& lGameObjects = iGameObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lGameObjects->GetCount(); lIndex++)
	{
		lGameObjects->Get(lIndex)->SaveOnDisk(*lWriteStream);
	}
	//signal that we wrote down the last game object
	lWriteStream->WriteInt32(EGameObjectIdentifierStopReading);//signals that all gameObject have been written to disk

	//save all events
	for(TInt lCurrentEvent = 0; lCurrentEvent < iEvents->GetCount(); lCurrentEvent++)
		iEvents->Get(lCurrentEvent)->SaveOnDisk(*lWriteStream);
	//signal that we wrote down the last event
	lWriteStream->WriteInt32(EEventIdentifierStopReading);

	//save map data, like what time the map has
	SaveMapData(*lWriteStream);

	//save all non-gameObjects
	iPlayer->SaveOnDisk(*lWriteStream);

	lWriteStream->CloseWriteStream();
	delete lWriteStream;
	delete lNativeFile;
}

void CMap::LoadGame(const char* aSaveGameFileName)
{
	CNativeFile* lNativeFile = new CNativeFile(aSaveGameFileName);
	CFileReadStream* lReadStream = lNativeFile->OpenForReading();

	if(lReadStream != NULL)
	{
		iGameObjectManager->RemoveAllGameObjects();
		iEvents->ClearAndDestroy();//removes all the events
		iClockTimerObjects->ClearAndDestroy();//destroy all timers, they will be created later in the game
		iPlayer->ResetForLoad();
		CGame::Game->iAudioEngine->StopAllSoundChannels();//to avoid that sounds are played from the previous game that just has been ended

		//---IMPORTANT: HAS TO BE AT FIRST POSITION OTHERWISE CHECK MAP ID will not work
		iGameMapId = static_cast<TGameMap> (lReadStream->ReadInt32());
		//---IMPORTANT: HAS TO BE AT SECOND POSITION OTHERWISE CHECK DIFFICULTY WILL NOT WORK
		//load difficulty for which this game was saved as
		CFighterPilotThePacificWar::FighterGame->FighterGame->iGameVariables->iDifficulty = static_cast<TGameDifficulty> (lReadStream->ReadInt32());
		//reset difficulty for game objects
		CGameObjectDefaultValues::LoadDefaultValues();

		LoadGameObjects(*lReadStream);//loads all gameObjects that have been saved and adds them to the gameObject array
		LoadEvents(*lReadStream); //load all events
		LoadMapData(*lReadStream);

		iPlayer->LoadFromDisk(*lReadStream); // reset the player object, to the properties it had during the safe

		CMap::SetPointersToNewValues(); // fixes broken pointers
		CMap::RemoveMappingAndPointerSettingInstructions();//remove all mappings since they are not needed anymore

		//functions to sync the game to the new data
		iPlayer->PostLoadSetupFunction();
		PostMapInit();

		lReadStream->CloseReadStream();
		delete lReadStream;
		delete lNativeFile;
	}

	//set game to paused
	CFighterPilotThePacificWar::FighterGame->iGameData->SetPaused(true);
}

void CMap::RestartMap()
{
	ClearMap();
	//Initilize map again
	InitializeMap();
	iPlayer->ChangeZoomMode(0);
	PostMapInit();
}

TBool CMap::CheckIfSaveGameExists(const char* aSaveGameFileName)
{
	CNativeFile* lNativeFile = new CNativeFile(aSaveGameFileName);
	CFileReadStream* lReadStream = lNativeFile->OpenForReading();
	TBool lFileFound = false;

	if(lReadStream != NULL)
	{
		lFileFound = true;
		lReadStream->CloseReadStream();
		delete lReadStream;
	}

	delete lNativeFile;
	return lFileFound;
}

//return current Map Id
TGameMap CMap::GetMapId()
{
	return iGameMapId;
}

TBool CMap::GetIsTutorialMap()
{
	return iIsTutorialMap;
}

TInt CMap::GetExpectedNumberOfMinutesForMap()
{
	return iExpectedMissionTimeInMins;
}

TGameDifficulty CMap::GetGameDifficulty(const char* aSaveGameFileName)
{
	CNativeFile* lNativeFile = new CNativeFile(aSaveGameFileName);
	CFileReadStream* lReadStream = lNativeFile->OpenForReading();
	TGameDifficulty lDifficultyOfSaveGameFile = EGameDifficultyEasy;

	if(lReadStream != NULL)
	{
		TGameMap lMapId = static_cast<TGameMap>(lReadStream->ReadInt32()); //just the map Id, not needed in this case
		lDifficultyOfSaveGameFile = static_cast<TGameDifficulty>(lReadStream->ReadInt32());

		lReadStream->CloseReadStream();
		delete lReadStream;
	}

	delete lNativeFile;
	return lDifficultyOfSaveGameFile;
}

TGameMap CMap::GetMapIdFromSaveGameFile(const char* aSaveGameFileName)
{
	CNativeFile* lNativeFile = new CNativeFile(aSaveGameFileName);
	CFileReadStream* lReadStream = lNativeFile->OpenForReading();
	TGameMap lMapId = EGameMapCampaign1Mission1;

	if(lReadStream != NULL)
	{
		lMapId = static_cast<TGameMap>(lReadStream->ReadInt32()); //just the map Id, not needed in this case

		lReadStream->CloseReadStream();
		delete lReadStream;
	}

	delete lNativeFile;
	return lMapId;
}

void CMap::ClearMap()
{
	iGameObjectManager->RemoveAllGameObjects();
	iEvents->ClearAndDestroy();//removes all the events
	iClockTimerObjects->ClearAndDestroy();//destroy all timers, they will be created later in the game

	if(iPlayer != NULL)
	{
		delete iPlayer;
		iPlayer = NULL;
	}
	iCurrentFrame = 0;
	iMapAlreadyInitilized = false;
}

void CMap::InitializeGameObjectSettings()
{
	//Load data that is needed for any map
	CTouchControls::InitilizeTouchControls();
	CHeavyBomberLocator::InitilizeHeavyBomberLocator();
	CSmallPlaneLocator::InitilizeSmallPlaneLocator();
	CBackgroundImage::InitilizeBackgroundImage();
	CSpeedoMeter::InitilizeSpeedoMeter();
	CHud::InitilizeHud();
	//for Views
	CGameSettingsView::LoadImagesNeededForView();
	CChangeSongView::LoadImagesNeededForView();
	CGamePauseView::LoadImagesNeededForView();
	//load Map Specific Data
	InitializeGameObjectSettingsForMap();
}

void CMap::PostMapInit()
{
	//functions to sync the game to the new data
	GetPlayer()->PostLoadSetupFunction();
	GetObjectManager()->ResetAllIntervals();// all intervals have to be destroyed since their permament objects are gone
	GetObjectManager()->UpdateIntervals();//objects might be in different intervals now !!! CRASH HERE !!!

	//Need to do these do these calls to make sure that during the transition to this view all objects are displayed properly
	//if plane starts on landing surface this check is required
	GetObjectManager()->CheckGameObjectsForCollisions();

	//need to update gameObjects to make sure that they display the correct image
	CPointerArray<CGameObject>*& lGameObjects = iGameObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lGameObjects->GetCount(); lIndex++)
	{
		lGameObjects->Get(lIndex)->Update();
	}

	//update Player stats
	GetPlayer()->Update();
}

void CMap::RemoveGameObjectSettings()
{
	iTexturesAlreadyLoaded = false;
	//Load data that is needed for any map
	CTouchControls::RemoveTouchControls();
	CHeavyBomberLocator::RemoveHeavyBomberLocator();
	CSmallPlaneLocator::RemoveSmallPlaneLocator();
	CBackgroundImage::RemoveBackgroundImage();
	CSpeedoMeter::RemoveSpeedoMeter();
	CHud::RemoveHud();
	//for Views
	CGameSettingsView::UnloadImagesNeededForView();
	CChangeSongView::UnloadImagesNeededForView();
	CGamePauseView::UnloadImagesNeededForView();
	//load Map Specific Data
	RemoveGameObjectSettingsForMap();
}

TInt CMap::GetCurrentGameTimeInSeconds()
{
	return (iCurrentFrame / GFRAMES_PER_SECOND_BACKEND);
}

void CMap::AddGameObject(CGameObject* aObject)
{
	iGameObjectManager->AddGameObject(aObject);
}

void CMap::AddEventObject(CEvent* aEvent)
{
	iEvents->Append(aEvent);
}

void CMap::AddMoveableObject(CMoveableGameObject* aObject)
{
	iGameObjectManager->AddMoveableGameObject(aObject);
}

CPointerArray<CGameObject>* CMap::GetAllDrawObjectsInRange(TIntFloat aPosition, TIntFloat aRange)
{
	return iGameObjectManager->ReturnObjectsInRange(aPosition, aRange, GAMEOBJECT_TYPE_GROUP_EVERYTHING);
}

TUint32 CMap::FindNewPointerValue(TUint32 aOldPointer)
{
	for(TInt lCurrentIndex = 0; lCurrentIndex < iOldToNewPointerMapper->GetCount(); lCurrentIndex++)
	{
		if(iOldToNewPointerMapper->Get(lCurrentIndex)->OldPointer == aOldPointer)
			return iOldToNewPointerMapper->Get(lCurrentIndex)->NewPointer;
	}
	return (TUint32)(NULL);//can't be found
}

void CMap::SetPointersToNewValues()
{
	for(TInt lCurrentIndex = 0; lCurrentIndex < iPointersThatNeedToBeSet->GetCount(); lCurrentIndex++)
	{
		void** lPointerThatNeedsToBeSet = (void**)iPointersThatNeedToBeSet->Get(lCurrentIndex)->SetPointer;
		TUint32 lNewPointerValue = CMap::FindNewPointerValue(iPointersThatNeedToBeSet->Get(lCurrentIndex)->OldPointer);
		(*lPointerThatNeedsToBeSet) = (void*)lNewPointerValue;
	}
}

void CMap::SaveDynamicArray(CPointerArray<void>* aPointerArray, CFileWriteStream& aOutputStream)
{
	//stores how many itemns we need to upload next time
	aOutputStream.WriteInt32(aPointerArray->GetCount());

	//write the items onto the disk
	for(TInt lCurrentItem = 0; lCurrentItem < aPointerArray->GetCount(); lCurrentItem++)
	{
		aOutputStream.WriteUint32((TUint32)aPointerArray->Get(lCurrentItem));
	}
}

void CMap::LoadDynamicArray(CPointerArray<void>* aPointerArray, CFileReadStream& aReadStream)
{
	TInt32 lNumberOfObjectsToLoad = aReadStream.ReadInt32();
	//need to make space first otherwise the pointers memory location references might get messed up if the array grows while adding new items
	for(TInt lCurrentIndex = 0; lCurrentIndex < lNumberOfObjectsToLoad; lCurrentIndex++)
		aPointerArray->Append(NULL);

	//restore the broken pointers
	for(TInt lCurrentIndex = 0; lCurrentIndex < aPointerArray->GetCount(); lCurrentIndex++)
	{
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(),(TUint32) &(aPointerArray->GetArrayData()[lCurrentIndex]));
	}
}

void CMap::AddPointerMapping(TUint32 aOldPointer, TUint32 aNewPointer)
{
	SNewToOldPointerMapping lMapper;
	lMapper.OldPointer = aOldPointer;
	lMapper.NewPointer = aNewPointer;
	iOldToNewPointerMapper->Append(new SNewToOldPointerMapping(lMapper));
}

void CMap::AddNewPointerSettingInstruction(TUint32 aOldPointer, TUint32 aSetPointer)
{
	SSetPointerToNewValue lMapper;
	lMapper.OldPointer = aOldPointer;
	lMapper.SetPointer = aSetPointer;

	iPointersThatNeedToBeSet->Append(new SSetPointerToNewValue(lMapper));
}

void CMap::RemoveMappingAndPointerSettingInstructions()
{
	iOldToNewPointerMapper->ClearAndDestroy();
	iPointersThatNeedToBeSet->ClearAndDestroy();
}

void CMap::SaveMapData(CFileWriteStream& aWriteStream)
{
	aWriteStream.WriteInt32(iCurrentFrame);
	aWriteStream.WriteInt32(iMapAlreadyInitilized);
	aWriteStream.WriteInt32(iNumberOfIntervals);
	aWriteStream.WriteInt32(iSize);
	iGameObjectManager->SaveOnDisk(aWriteStream);
}

void CMap::LoadMapData(CFileReadStream& aReadStream)
{
	iCurrentFrame = aReadStream.ReadInt32();
	iMapAlreadyInitilized = aReadStream.ReadInt32();
	iNumberOfIntervals = aReadStream.ReadInt32();
	iSize = aReadStream.ReadInt32();
	iGameObjectManager->LoadFromDisk(aReadStream);
}

void CMap::LoadEvents(CFileReadStream& aReadStream)
{
	while(true)
	{
		TEventIdentifier lNextEventObjectToLoad = static_cast<TEventIdentifier> (aReadStream.ReadInt32());
		if(lNextEventObjectToLoad == EEventIdentifierStopReading)
		{
			//read in the last event, should stop now
			return;
		}

		switch(lNextEventObjectToLoad)
		{
			//Win Events
			case EEventIdentifierWinIfObjectsAreDestroyedEvent:
			{
				CEvent* lEvent = new CWinIfObjectsAreDestroyedEvent();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierWinIfAllJapaneseForcesAreDestroyed:
			{
				CEvent* lEvent = new CWinIfAllJapaneseForcesAreDestroyed(100);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierWinIfAllJapaneseMoveableObjectsAreDestroyed:
			{
				CEvent* lEvent = new CWinIfAllJapaneseMoveableObjectsAreDestroyed(100);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierWinIfObjectsLeaveMapAlive:
			{
				CEvent* lEvent = new CWinIfObjectsLeaveMapAlive();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierWinIfObjectsAreCaptured:
			{
				CEvent* lEvent = new CWinIfObjectsAreCaptured();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}

			case EEventIdentifierWinIfJapaneseDestroyedAfterCertainTime:
			{
				CEvent* lEvent = new CWinIfJapaneseDestroyedAfterACertainTime(10,10);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			//Lose Events
			case EEventIdentifierLoseIfObjectsAreDestroyedEvent:
			{
				CEvent* lEvent = CLoseIfObjectsAreDestroyedEvent::New();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierLoseIfObjectsAreCaptured:
			{
				CEvent* lEvent = new CLoseIfObjectsAreCaptured();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierLoseIfObjectsLeaveMapAlive:
			{
				CEvent* lEvent = new CLoseIfObjectLeavesMap(1);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			//Bonus Events
			case EEventIdentifierGetBonusPointsIfObjectsAreDestroyed:
			{
				CEvent* lEvent = new CGetBonusPointsIfObjectsAreDestroyedEvent();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierGetBonusPointsIfObjectsSurvive:
			{
				CEvent* lEvent = new CGetBonusPointsIfObjectsSurvive();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierGetBonusPointsIfLessTroopsGetKilled:
			{
				CEvent* lEvent = new CGetBonusPointsIfLessTroopsGetKilled();
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			//---------------------- EXTRA ----------------------//
			case EEventIdentifierSpawnMitchellToAttackTargetClockTimer:
			{
				CEvent* lEvent = CCreateMitchellToAttackTargetClockTimer::New(NULL, 60);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnMitchellEveryTimerInterval:
			{
				CEvent* lEvent = new CSpawnMitchellEveryTimeInterval(100,100,100);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnBettyEveryTimerInterval:
			{
				CEvent* lEvent = new CSpawnBettyEveryTimeInterval(100,100,100,false,100);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnAmericanLCTEveryTimerInterval:
			{
				SGroundUnitOrder lDummyOrder;
				CEvent* lEvent = new CSpawnAmericanLCTEveryTimeInterval(EAmericanLCTTypeEmpty, lDummyOrder, 1, 0, 1);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnAmericanSquadEveryTimerInterval:
			{
				SGroundUnitOrder lDummyOrder;
				CEvent* lEvent = new CSpawnAmericanSquadEveryTimeInterval(EAmericanSquadTypeSoldierSquad, lDummyOrder, 1, 0, 1);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnAmericanTroopTransporterIfObjectsAreDestroyed:
			{
				SGroundUnitOrder lDummyOrder;
				CEvent* lEvent = new CSpawnAmericanTroopTransporterIfObjectsDestroyed(0, EAmericanLCTTypeEmpty,lDummyOrder);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnJapaneseSquadEveryTimerInterval:
			{
				SGroundUnitOrder lDummyOrder;
				CEvent* lEvent = new CSpawnJapaneseSquadEveryTimeInterval(EJapaneseSquadTypeSoldierAATankSquad, lDummyOrder, 1, 0, 1);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnJapaneseSquadIfBuildingGetsCaptured:
			{
				SGroundUnitOrder lDummyOrder;
				CEvent* lEvent = new CSpawnJapaneseSquadIfBuildingGetsCaptured(EJapaneseSquadTypeSoldierAATankSquad, lDummyOrder, 1);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnBettyIfBuildingGetsCaptured:
			{
				CEvent* lEvent = new CSpawnBettyIfBuildingGetsCaptured(0,0);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnValIfBuildingGetsCaptured:
			{
				CEvent* lEvent = new CSpawnValIfBuildingGetsCaptured(0,0,EWeaponIdentifier1000kgBomb,GAMEOBJECT_TYPE_GROUP_PLANES);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnValEveryTimerInterval:
			{
				CEvent* lEvent = new CSpawnValEveryTimeInterval(100, 100, 100,GAMEOBJECT_TYPE_GROUP_PLANES);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnDauntlessEveryTimerInterval:
			{
				CEvent* lEvent = new CSpawnDauntlessEveryTimeInterval(100, 100, 100, 100, GAMEOBJECT_TYPE_GROUP_PLANES);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpawnDauntlessIfBuildingGetsCaptured:
			{
				CEvent* lEvent = new CSpawnDauntlessIfBuildingGetsCaptured(0,0,EWeaponIdentifier1000kgBomb,GAMEOBJECT_TYPE_GROUP_PLANES);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierClockTimerEvent:
			{
				CEvent* lEvent = new CClockTimerEvent(100);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}

				//------------------------------ TUTORIAL EVENTS ----------------------------------//
			case EEventIdentifierRestartLastTutorialIfDied:
			{
				CEvent* lEvent = new CRestartLastTutorialScreenIfDiedEvent(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierLoopTutorial:
			{
				CEvent* lEvent = new CControlsTutorialLooping(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierTurnTutorial:
			{
				CEvent* lEvent = new CControlsTutorialTurns(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSpeedUpTutorial:
			{
				CEvent* lEvent = new CControlsTutorialSpeedUp(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierSlowDownTutorial:
			{
				CEvent* lEvent = new CControlsTutorialSlowDown(NULL, ETutorialMethodCallIdTutorialComplete, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierZoomInAndOutTutorial:
			{
				CEvent* lEvent = new CControlsTutorialZoomInAndZoomOut(ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierMovetoEndOfMapAndAvoidAA:
			{
				CEvent* lEvent = new CControlsTutorialMoveToEndOfMap(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierStartFromCarrier:
			{
				CEvent* lEvent = new CControlsTutorialStartFromCarrier(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierLandOnCarrier:
			{
				CEvent* lEvent = new CControlsTutorialLandOnCarrier(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
				//Tutorial 2 events
			case EEventIdentifierFireButtons:
			{
				CEvent* lEvent = new CWeaponsTutorialFireButtons(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierBomb1000Kg:
			{
				CEvent* lEvent = new CWeaponsTutorialBomb1000kg(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierLoseIfFailedToKill:
			{
				CEvent* lEvent = new CWeaponsTutorialLoseIfFailedToKillAnything(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierKillSoldiersUsingMachineGun:
			{
				CEvent* lEvent = new CWeaponsTutorialKillSoldiersUsingMachineGun(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierLoseIfAllAmericanSoldiersDied:
			{
				CEvent* lEvent = new CWeaponsTutorialLoseIfAllAmericanSoldiersDied(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierDiveBombingKill:
			{
				CEvent* lEvent = new CWeaponsTutorialDiveBombingKill(NULL, ETutorialMethodCallIdTutorialComplete, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierAttackBomber:
			{
				CEvent* lEvent = new CWeaponsTutorialAttackBomber(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierAttackFighter:
			{
				CEvent* lEvent = new CWeaponsTutorialAttackFighter(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			case EEventIdentifierAttackShip:
			{
				CEvent* lEvent = new CWeaponsTutorialAttackShip(NULL, ETutorialMethodCallIdTutorialComplete);
				lEvent->LoadFromDisk(aReadStream);
				iEvents->Append(lEvent);
				break;
			}
			default:
			{
				TInt lError = 0;
				lError += 2;//we have a problem!
				break;
			}
		}
	}
}

void CMap::LoadGameObjects(CFileReadStream& aReadStream)
{
	while(true)
	{
		TGameObjectIdentifier lNextGameObjectToLoad = static_cast<TGameObjectIdentifier> (aReadStream.ReadInt32());
		TGameObjectIdentifier lPreviousRead;
		if(lNextGameObjectToLoad == EGameObjectIdentifierStopReading)
		{
			//end of the list of gameObjects that we need to read in
			return;
		}

		switch(lNextGameObjectToLoad)
		{
			//---------------  PLANES ------------------------------- //
			case EGameObjectIdentifierDauntless:
			{
				CAircraft* lNewAircraft = CDauntless::New(true, TIntFloat::Convert(0), true, TPoint(0, 0));
				lNewAircraft->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewAircraft);
			}
				break;
			case EGameObjectIdentifierMitchell:
			{
				CAircraft* lNewAircraft = CMitchell::New(true, TIntFloat::Convert(0), false, TPoint(0, 0));
				lNewAircraft->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewAircraft);
			}
				break;
			case EGameObjectIdentifierVal:
			{
				CVal* lNewAircraft = CVal::New(true, TIntFloat::Convert(0), true, TPoint(0, 0));
				lNewAircraft->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewAircraft);
			}
				break;
			case EGameObjectIdentifierBetty:
			{
				CAircraft* lNewAircraft = CBetty::New(true, TIntFloat::Convert(0), false, TPoint(0, 0));
				lNewAircraft->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewAircraft);
			}
				break;

				//---------------  GROUND UNITS ------------------------------- //

			case EGameObjectIdentifierJapaneseRifleman:
			{
				CGroundUnit* lNewGroundUnit = CJapaneseRifleman::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierJapaneseRocketman:
			{
				CGroundUnit* lNewGroundUnit = CJapaneseRocketman::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierJapanese97ChiHaTank:
			{
				CGroundUnit* lNewGroundUnit = CJapanese97ChiHaTank::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierJapaneseAA97ChiHaTank:
			{
				CGroundUnit* lNewGroundUnit = CJapaneseAA97ChiHaTank::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierAmericanRifleman:
			{
				CGroundUnit* lNewGroundUnit = CAmericanRifleman::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierAmericanRocketman:
			{
				CGroundUnit* lNewGroundUnit = CAmericanRocketman::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierAmericanShermanTank:
			{
				CGroundUnit* lNewGroundUnit = CAmericanShermanTank::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;
			case EGameObjectIdentifierAmericanAAShermanTank:
			{
				CGroundUnit* lNewGroundUnit = CAmericanAAShermanTank::New(0, TIntFloat::Convert(0), false);
				lNewGroundUnit->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lNewGroundUnit);
			}
				break;

				//---------------  BUILDINGS ------------------------------- //

			case EGameObjectIdentifierLandingSurface:
			{
				CGameObject* lGameObject = CLandingSurface::New(TPoint(0, 0), EConflictSideAmerican, TIntFloat::Convert(10), TIntFloat::Convert(10));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierAirport:
			{
				CGameObject* lGameObject = CAirport::CreateLoadAirport();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierComStation:
			{
				CGameObject* lGameObject = CComStation::CreateLoadComStation();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierFlagPole:
			{
				CGameObject* lGameObject = CFlagPole::New(0, EConflictSideAmerican);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierHangar:
			{
				CGameObject* lGameObject = CHangar::New(0);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierHq:
			{
				CGameObject* lGameObject = CHq::CreateLoadHq();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierJapaneseAA20mmMachineCannon:
			{
				CGameObject* lGameObject = CJapaneseAA20mmMachineCannon::New(0);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierJapaneseAA75mmType88:
			{
				CGameObject* lGameObject = CJapaneseAA75mmType88::New(0);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierJapaneseAAConcrete:
			{
				CGameObject* lGameObject = CJapaneseAAConcrete::New(0);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;

				//---------------  Decor ------------------------------- //

			case EGameObjectIdentifierBigTree:
			{
				CGameObject* lGameObject = CBigTree::New(0, EBigTreeNormal);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierSmallTree:
			{
				CGameObject* lGameObject = CSmallTree::New(0, ESmallTreeNormal);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierTree1:
			{
				CGameObject* lGameObject = CTree1::New(0, EBigThinTreeNormal);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierTree2:
			{
				CGameObject* lGameObject = CTree2::New(0, ETree2BowedTree);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierForest1:
			{
				CGameObject* lGameObject = CForest1::LoadForest1();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierForest2:
			{
				CGameObject* lGameObject = CForest2::LoadForest2();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierForest3:
			{
				CGameObject* lGameObject = CForest3::LoadForest3();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierForest4:
			{
				CGameObject* lGameObject = CForest4::LoadForest4();
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;

				//---------------  Explosions ------------------------------- //
			case EGameObjectIdentifierExplosion14InchShell:
			{
				CGameObject* lGameObject = CExplosion14InchShell::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion14InchShellInWater:
			{
				CGameObject* lGameObject = CExplosion14InchShellInWater::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion1000kg:
			{
				CGameObject* lGameObject = CExplosion1000Kg::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion1000kgInAir:
			{
				CGameObject* lGameObject = CExplosion1000KgInAir::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion1000kgInWater:
			{
				CGameObject* lGameObject = CExplosion1000KgInWater::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion500kg:
			{
				CGameObject* lGameObject = CExplosion500Kg::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion500kgInAir:
			{
				CGameObject* lGameObject = CExplosion500KgInAir::New(TPoint(0, 0), true);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion500kgInWater:
			{
				CGameObject* lGameObject = CExplosion500KgInWater::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion100kg:
			{
				CGameObject* lGameObject = CExplosion100Kg::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion100kgInAir:
			{
				CGameObject* lGameObject = CExplosion100KgInAir::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosion100kgInWater:
			{
				CGameObject* lGameObject = CExplosion100KgInWater::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierWaterSplash:
			{
				CGameObject* lGameObject = CWaterSplash::New(TPoint(0, 0));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;

				//---------------  Floor ------------------------------- //

			case EGameObjectIdentifierLand:
			{
				CGameObject* lGameObject = CFloorLand::New(TPoint(0, 0), 1000);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierWater:
			{
				CGameObject* lGameObject = CFloorWater::New(TPoint(0, 0), 1000);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddGameObject(lGameObject);
			}
				break;

				//---------------  PROJECTILES ------------------------------- //
			case EGameObjectIdentifier20mmBulletCannon:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBulletCannon20mm::New(lTempPoint, EConflictSideAmerican, 10, false, false);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifier13mmBullet:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBullet13mm::New(lTempPoint, EConflictSideAmerican, 10, false, false);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifier8mmBullet:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBullet8mm::New(lTempPoint, EConflictSideAmerican, 10, false);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierRifleBullet:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBulletRifle::New(lTempPoint, EConflictSideAmerican, 10, false, TIntFloat::Convert(10));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierPropelledGrenade:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CPropelledGrenade::New(lTempPoint, EConflictSideAmerican, 10, false, TIntFloat::Convert(10));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierSmoke:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CSmoke::New(lTempPoint, TIntFloat::Convert(0), 0, false, 0, TSizeIntFloat(TIntFloat::Convert(1),TIntFloat::Convert(1)), 10, 0);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifier1000kgBomb:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBomb1000kg::New(lTempPoint, TIntFloat::Convert(10), 10, 10);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifier500kgBomb:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBomb500kg::New(lTempPoint, TIntFloat::Convert(10), 10, 10);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifier100kgBomb:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CBomb100kg::New(lTempPoint, TIntFloat::Convert(10), 10, 10);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierClusterBomb:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CClusterBomb::New(lTempPoint, TIntFloat::Convert(10), 10, 10, false);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosive7InchShell:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CExplosive7InchShell::New(lTempPoint, EConflictSideAmerican, 180, false, TIntFloat::Convert(10));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosive14InchShell:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CExplosive14InchShell::New(lTempPoint, EConflictSideAmerican, 180, false, TIntFloat::Convert(10));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierTankShell80mm:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CTankShell80mm::New(lTempPoint, EConflictSideAmerican, 180, false, TIntFloat::Convert(10));
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosive75mmShell:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CExplosive75mmShell::New(lTempPoint, 10, false, 100);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierExplosive88mmShell:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CExplosive88mmShell::New(lTempPoint, 10, false, 100);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierStandardRocket:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CStandardRocket::New(lTempPoint, TIntFloat::Convert(10), 0, false, NULL);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierTorpedo:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CTorpedo::New(lTempPoint, TIntFloat::Convert(10), 10, false);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;
			case EGameObjectIdentifierUnderWaterTorpedo:
			{
				TPointIntFloat lTempPoint;
				lTempPoint.iX =  TIntFloat::Convert(0);
				lTempPoint.iY =  TIntFloat::Convert(0);//will be changed during the load anyway
				CMoveableGameObject* lGameObject = CUnderWaterTorpedo::New(lTempPoint, EConflictSideAmerican, 10, false);
				lGameObject->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
			}
				break;

				//---------------  SHIPS ------------------------------- //

			case EGameObjectIdentifierUSSVictory:
			{
				CMoveableGameObject* lShip = CUSSVictory::LoadUSSVictory();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierAmericanCarrierLexington:
			{
				CMoveableGameObject* lShip = CAmericanCarrierLexington::LoadAmericanCarrierLexington();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierAmericanLCT:
			{
				SGroundUnitOrder lFakeOrder; //don't care, since we are going to load the real one from disk
				lFakeOrder.MoveToLocation = false;
				lFakeOrder.ShouldProtectArea = false;
				CMoveableGameObject* lShip = CAmericanLCT::New(0, false, EAmericanLCTTypeEmpty, lFakeOrder);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierAmericanDestroyer:
			{
				CAmericanDestroyer* lShip = CAmericanDestroyer::New(0, false);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierAmericanBattleshipUSSTennessee:
			{
				CAmericanBattleshipUSSTennessee* lShip = CAmericanBattleshipUSSTennessee::LoadAmericanBattleshipUSSTennessee();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierAmericanTroopTransporter:
			{
				TAmericanLCTType lLCTType; //not important, will be reset during loading
				SGroundUnitOrder lOrder; //not important, will be reset during loading
				CMoveableGameObject* lShip = CAmericanTroopTransporter::New(0,false,0,lLCTType,lOrder);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseLCT:
			{
				SGroundUnitOrder lFakeOrder; //don't care, since we are going to load the real one from disk
				lFakeOrder.MoveToLocation = false;
				lFakeOrder.ShouldProtectArea = false;
				CMoveableGameObject* lShip = CJapaneseLCT::New(0, false, EJapaneseLCTTypeEmpty, lFakeOrder);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseCarrierShoho:
			{
				CMoveableGameObject* lShip = CJapaneseCarrierShoho::LoadJapaneseCarrierShoho();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseCarrierShokaku:
			{
				CMoveableGameObject* lShip = CJapaneseCarrierShokaku::LoadJapaneseCarrierShokaku();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseCarrierTaiho:
			{
				CMoveableGameObject* lShip = CJapaneseCarrierTaiho::LoadJapaneseCarrierTaiho();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseDestroyer:
			{
				CMoveableGameObject* lShip = CJapaneseDestroyer::New(0, false);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseBattleshipYamato:
			{
				CJapaneseBattleshipYamato* lShip = CJapaneseBattleshipYamato::LoadJapaneseBattleshipYamato();
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseTroopTransporter:
			{
				TJapaneseLCTType lJapaneseLCTType; //not important, will be reset during loading
				SGroundUnitOrder lOrder; //not important, will be reset during loading
				CMoveableGameObject* lShip = CJapaneseTroopTransporter::New(0,false,0,lJapaneseLCTType,lOrder);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
			case EGameObjectIdentifierJapaneseOilTanker:
			{
				CMoveableGameObject* lShip = CJapaneseOilTanker::New(0,false);
				lShip->LoadFromDisk(aReadStream);//sets up the data for the object
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lShip);
				break;
			}
				//------------------- AI -------------------------------//
				// Note: Most AI is owned by another gameobject and created and destroyed by it and thus doesn't need to be loaded seperatly.
			case EGameObjectIdentifierGroundUnitOverlordAI:
			{
				CMoveableGameObject* lGameObject = CGroundUnitOverlordAI::New(EConflictSideAmerican);
				lGameObject->LoadFromDisk(aReadStream);
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lGameObject);
				break;
			}
			default:
			{
				TInt lError = 0;
				lError += 2;//we have a problem!
				break;
			}
		}
		lPreviousRead = lNextGameObjectToLoad;

	}
}
