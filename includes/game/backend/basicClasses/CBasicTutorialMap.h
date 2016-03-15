/*
 * CBasicTutorialMap.h
 *
 *  Created on: Oct 1, 2012
 *      Author: dstoll
 */

#ifndef CBASICTUTORIALMAP_H_
#define CBASICTUTORIALMAP_H_

#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

enum TTutorialMethodCallIds //these will be passed down to the Method HandleTutorialMethodCall to make a call to a method after tutorial screen has been dismissed
{
	//Tutorial 1
	ETutorialMethodCallIdTutorial1UpNavScreen = 0,ETutorialMethodCallIdTutorial1UpNavPractice, // 0, 1
	ETutorialMethodCallIdTutorial1TurnScreen, ETutorialMethodCallIdTutorial1TurnPractice, // 2, 3
	ETutorialMethodCallIdTutorial1IncreaseSpeedScreen,ETutorialMethodCallIdTutorial1IncreaseSpeedPractice, // 4, 5
	ETutorialMethodCallIdTutorial1SlowDownScreen,ETutorialMethodCallIdTutorial1SlowDownPractice,ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownFailed, // 6, 7, 8
	ETutorialMethodCallIdTutorial1ZoomInAndOutScreenSlowDownSuccess,ETutorialMethodCallIdTutorial1ZoomInAndOutPractice,// 9, 10
	ETutorialMethodCallIdTutorial1AvoidAAScreen, ETutorialMethodCallIdTutorial1AvoidAAPractice, // 11, 12
	ETutorialMethodCallIdTutorial1StartFromCarrierScreen, ETutorialMethodCallIdTutorial1StartFromCarrierPractice, // 13, 14
	ETutorialMethodCallIdTutorial1LandOnCarrierScreen,ETutorialMethodCallIdTutorial1LandOnCarrierPractice, // 15, 16

	//Tutorial 2
	ETutorialMethodCallIdTutorial2UseFireButtonsScreen,ETutorialMethodCallIdTutorial2UseFireButtonsPractice, //17, 18
	ETutorialMethodCallIdTutorial2Use1000KgBombScreen,ETutorialMethodCallIdTutorial2Use1000KgBombPractice, //19, 20
	ETutorialMethodCallIdTutorial2Use500KgBombScreen,ETutorialMethodCallIdTutorial2Use500KgBombPractice, //21, 22
	ETutorialMethodCallIdTutorial2Use100KgBombScreen,ETutorialMethodCallIdTutorial2Use100KgBombPractice, //23, 24
	ETutorialMethodCallIdTutorial2UseClusterBombScreen,ETutorialMethodCallIdTutorial2UseClusterBombPractice, //25, 26
	ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunScreen,ETutorialMethodCallIdTutorial2KillSoldiersUsingMachineGunPractice, //27, 28
	ETutorialMethodCallIdTutorial2ProtectSoldiersScreen,ETutorialMethodCallIdTutorial2ProtectSoldiersPractice, //29, 30
	ETutorialMethodCallIdTutorial2DiveBombingScreen,ETutorialMethodCallIdTutorial2DiveBombingPractice, //31, 32
	ETutorialMethodCallIdTutorial2AttackBomberScreen,ETutorialMethodCallIdTutorial2AttackBomberPractice, //33, 34
	ETutorialMethodCallIdTutorial2AttackFighterScreen,ETutorialMethodCallIdTutorial2AttackFighterPractice, //35, 36
	ETutorialMethodCallIdTutorial2AttackShipScreen,ETutorialMethodCallIdTutorial2AttackShipPractice, //37, 38

	//Tutorial 3
	ETutorialMethodCallIdTutorial3ExplainHudScreen,ETutorialMethodCallIdTutorial3ExplainAAScreen, //39, 40
	ETutorialMethodCallIdTutorial3ExplainAirportScreen,ETutorialMethodCallIdTutorial3ExplainComStationScreen, //41, 42
	ETutorialMethodCallIdTutorial3ExplainMusicPlayer, //43

	//---------- 44 ---------------//
	ETutorialMethodCallIdTutorialComplete // used if the end of the tutorial has been reached, will exit the mission
};

struct STutorialScreenData
{
	TImageId TutorialImage;
	CString* DescriptionStringId;
	TTutorialMethodCallIds MethodCallIdIfContinue; // will be used when screen is dismissed, to know what method to call next
	TTutorialMethodCallIds MethodCallIdIfSkip; // will be used when screen is dismissed, to know what method to call next
};

class CBasicTutorialMap : public CMap
{
public:

	const STutorialScreenData& GetTutorialScreenData();
	virtual void TutorialMethodCall(TTutorialMethodCallIds aCallId);

protected:

	//contains the code to make the transition to the tutorial Screen
	void GoToTutorialView();
	//contains the code to make the transition to the GameView
	void GoToGameView();
	//contains the Code to End the tutorial
	void EndMission();

protected:
	CBasicTutorialMap(TGameMap aGameMapId, TUint aSize, TInt aNumberOfIntervals, TInt aExpectedMissionTimeInMins);
	virtual ~CBasicTutorialMap();

	//contains code that is needed for any Section of the tutorial
	virtual void BasicSetup(CDauntless* aPlayerAircraft, TTutorialMethodCallIds aCallIdIfFailed);

	//not really important in this case, since we always start of with a view by default
	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	STutorialScreenData iTutorialScreenData;
};

#endif /* CBASICTUTORIALMAP_H_ */
