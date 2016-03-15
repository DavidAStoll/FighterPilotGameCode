/*
 * CGame.cpp
 *
 *  Created on: May 8, 2012
 *      Author: dstoll
 */

#include "includes/core/CGame.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CTextureObject.h"
#include "includes/core/sound/CWavLoader.h"
#include "includes/core/views/CBasicView.h"
#include "includes/core/utility/CFramesPerSecondCounter.h"
#include "includes/core/utility/CSystem.h"



volatile TBool CGame::HaveToSuspend;
volatile TBool CGame::IsSuspended;

CGame* CGame::Game = NULL;

CGame::CGame()
{
	iFrame = 0;
	CGame::HaveToSuspend = false;
	CGame::IsSuspended = false;
	iGraphicsEngine = NULL;
	iAudioEngine = NULL;
	iImageStore = NULL;
	iView = NULL;
	iUserStatsTrackerDaemon = NULL;
	iMemoryUsageTracker = CMemoryUsageTracker::New(); //might as well create it here

	if(CGame::Game == NULL)
	{
		CGame::Game = this;
	}
}

CGame::~CGame()
{
	if(iUserStatsTrackerDaemon != NULL)
	{
		delete iUserStatsTrackerDaemon;
		iUserStatsTrackerDaemon = NULL;
	}
	if(iGraphicsEngine != NULL)
	{
		delete iGraphicsEngine;
		iGraphicsEngine = NULL;
	}
	if(iAudioEngine != NULL)
	{
		delete iAudioEngine;
		iAudioEngine = NULL;
	}
	if(iImageStore != NULL)
	{
		delete iImageStore;
		iImageStore = NULL;
	}
	if(iLocalizationEngine != NULL)
	{
		delete iLocalizationEngine;
		iLocalizationEngine = NULL;
	}
	if(iView != NULL)
	{
		delete iView;
		iView = NULL;
	}
	if(iMemoryUsageTracker)
	{
		delete iMemoryUsageTracker;
		iMemoryUsageTracker = NULL;
	}
}

void CGame::ExitGame()
{
	CallBeforeExitGame();
	SavePersistentData();
	CSystem::ExitNative();
	//No Need to delete the game object, since the process is going to be destroyed anyway, thus memory will be not be leaked
}

void CGame::InitializeGame()
{
	LoadSoundEffects();

	SetupMusicAndSfxPlayer(); //can play music now

	InitializeGraphicsEngine(); //might have different settings from game to game

	//create Image Store, always the same from game to game
	iImageStore = CImageStore::New();

	//probably should pass a structer to create this object and for the other methods as well!!
	iGraphicsFont = CGraphicsFont::New(*iGraphicsEngine, "HQFonts.ttf.mp3");
	iTouchEventHandler = CTouchEventHandler::New(4);
	iFramesPerSecondCounter = CFramesPerSecondCounter::New(GFRAMES_PER_SECOND_FRONTEND);
	iRandomGenerator = CRandomGenerator::New(iFramesPerSecondCounter);

	LoadTextuesForFrontend();

	SetupLocalization();

	//create user stats tracker, arguments are set through the global defines filr
	iUserStatsTrackerDaemon = CUserStatsTrackerDaemon::New(60 * GSTATS_TRACKER_UPLOAD_SCORES_INTERVAL_IN_MINS);

	CallBeforeMainViewIsCreated();

	CreateMainView(); //after this call, game should be able run
}

TInt CGame::GetFrame()
{
	return iFrame;
}

TBool CGame::GetIsSuspended()
{
	return CGame::IsSuspended;
}

void CGame::ResumeGame()
{
	//set them back to false, since we resumed
	CGame::HaveToSuspend = false;
	CGame::IsSuspended = false;

	//enable Sound again, if not silent profile
	iAudioEngine->CheckPlayerProfileIfAudioHasToBeMuted();
	//let the view know that the context got gained
	iView->ContextGained();

	//do something to start the game
	while (!CGame::IsSuspended)
	{
		if(!CGame::IsSuspended)
		{
			//-------------- Game Logic -------------//
			iFramesPerSecondCounter->CounterFunctionBeforeLogicCode();
			iTouchEventHandler->DeliverTouchEventsToView(iView);
			iView->RunLogicCode();
			iFramesPerSecondCounter->CounterFunctionAfterLogicCode();

			//--------------- Draw ------------------//
			iFramesPerSecondCounter->CounterFunctionBeforeDrawingCode();
			iView->Draw();
			iFramesPerSecondCounter->Draw(iGraphicsFont);
			iGraphicsEngine->CopyDrawingSurfaceToDestinationSurface();
			iFramesPerSecondCounter->CounterFunctionAfterDrawingCode();

			//------------- End of Game Loop-----------//
			iGraphicsEngine->CheckOpenGlCacheForExpiredEntries(); //need to perform this check to see if any textures can be deleted from the GPU
			iFramesPerSecondCounter->SleepRemainingTimeInFrame();
		}

		//all done, update if we have to suspend the game
		CGame::IsSuspended = CGame::HaveToSuspend;
		iFrame++;
	}

	//let the view know that the context has been lost
	iView->ContextLost();
}

void CGame::SuspendGame()
{
	CGame::HaveToSuspend = true;
}

