/*
 * CGame.h
 *
 *  Created on: May 8, 2012
 *      Author: dstoll
 */

#ifndef CGAME_H_
#define CGAME_H_

#include "includes/core/standardLibrary/BasicTypes.h"
#include "includes/core/sound/CGameAudioEngine.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/utility/CImageStore.h"
#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/utility/CLocalizationEngine.h"
#include "includes/core/utility/CFramesPerSecondCounter.h"
#include "includes/core/utility/CMemoryUsageTracker.h"
#include "includes/core/utility/CRandomGenerator.h"
#include "includes/core/utility/CUserStatsTrackerDaemon.h"

#include <android/log.h>
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

class CBasicView;

class CGame
{

public:

	//should be called by the platform dependent code before terminating the game
	void ExitGame();
	//initialize game, starts loading textures, sound and everything that the game needs to start running
	void InitializeGame();
	//returns the current Frame of the Game
	TInt GetFrame();
	//Return if the Game loop is suspended or not
	TBool GetIsSuspended();
	//resumes from the current view
	void ResumeGame();
	//will either resume or pause the Game loop
	void SuspendGame();


	//global variables
	static CGame* Game;
	//these data members will be used by other components in the game
	CGameAudioEngine* iAudioEngine;
	CGraphicsEngine* iGraphicsEngine;
	CImageStore* iImageStore;
	CGraphicsFont* iGraphicsFont;
	CLocalizationEngine* iLocalizationEngine;
	CTouchEventHandler* iTouchEventHandler;
	CFramesPerSecondCounter* iFramesPerSecondCounter;
	CMemoryUsageTracker* iMemoryUsageTracker; //keeps track on how much memory we are currently using
	CRandomGenerator* iRandomGenerator;
	CUserStatsTrackerDaemon* iUserStatsTrackerDaemon;
	CBasicView* iView;	//points to the current view

protected:

	CGame();
	virtual ~CGame();

	//this method will be called just before the game exits
	virtual void CallBeforeExitGame() = 0;
	//this method will be called before the MainView method is called
	virtual void CallBeforeMainViewIsCreated() = 0;
	//create main view, the first view that will start displaying stuff
	virtual void CreateMainView() = 0;
	//creates the graphics engine
	virtual void InitializeGraphicsEngine() = 0;
	//Loads all the texture for the frontend
	virtual void LoadTextuesForFrontend() = 0;
	//Loads all the sound effects
	virtual void LoadSoundEffects() = 0;
	//put code here to write down any persistent data for the game
	virtual void SavePersistentData() = 0;
	//initiliaze localization and put load all localization files
	virtual void SetupLocalization() = 0;
	//initialize the music player and SFX stream
	virtual void SetupMusicAndSfxPlayer() = 0;

	TInt iFrame; //make it signed, so that values compared to it can be set negative, to make sure they are not equal by coincidence
	//for some reason need to be static otherwise volatile doesn't work
	static volatile TBool HaveToSuspend; //need this temporary variable, since we can only set the SuspendValue at the end of the Game Loop
	static volatile TBool IsSuspended; //game will stop processing once it is suspended
};

#endif /* CGAME_H_ */
