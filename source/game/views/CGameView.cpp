/*
 * CGameView.cpp
 *
 *  Created on: Aug 22, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CGameView.h"
#include "includes/game/views/CGamePauseView.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"

CGameView::CGameView(CGameData& aGameData)
:iGameData(aGameData)
{
	iPlayerShouldRespawn = false;
}

CGameView::~CGameView()
{
	if(iTouchControls != NULL)
	{
		delete iTouchControls;
		iTouchControls = NULL;
	}
}

CGameView* CGameView::New(CGameData& aGameData)
{
	CGameView* lSelf = new CGameView(aGameData);
	lSelf->Construct();
	return lSelf;
}

void CGameView::Construct()
{
	iTouchControls = CTouchControls::New(CFighterPilotThePacificWar::FighterGame->iGameVariables->iUseIconButton);
}

//checks if any new User Input is available
void CGameView::CheckForUserInput()
{
	if(CheckIfUserActionIsTriggered(iTouchControls->GetFramesSincePushed(EGameControlTypePauseMenu)))
	{
		//go to Pause Menu
		iGameData.SetPaused(true);
		return;

	}
	else if(CheckIfUserActionIsTriggered(iTouchControls->GetFramesSincePushed(EGameControlTypeZoomIn)))
	{
		if(CPlayer::ZoomFactor > 1)
			iGameData.GetMap()->GetPlayer()->ChangeZoomMode(-0.5);
	}
	else if(CheckIfUserActionIsTriggered(iTouchControls->GetFramesSincePushed(EGameControlTypeZoomOut)))
	{
		if(CPlayer::ZoomFactor < 2)
			iGameData.GetMap()->GetPlayer()->ChangeZoomMode(0.5);
	}
	//plane can't be controlled in this case, but maybe spawn a new plane if it already crashed
	else if(iPlayerShouldRespawn)
	{
		if(iGameData.GetMap()->GetPlayer()->GetLivesLeft() > 0)
		{
			if(iGameData.GetMap()->GetPlayer()->SpawnNewPlayerDelayed())
			{
				iPlayerShouldRespawn = false;
				iGameData.GetMap()->GetPlayer()->ChangeLives(-1);
			}
		}
	}
	//player can only control the plane if it has still some health left
	else if(iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->IsAlive()
			&& !iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->IsTurning())
	{
		if(iTouchControls->GetFramesSincePushed(EGameControlTypeLeft) > 0)
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->TurnLeft();
		}
		if(iTouchControls->GetFramesSincePushed(EGameControlTypeRight)  > 0)
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->TurnRight();
		}
		if(iTouchControls->GetFramesSincePushed(EGameControlTypeDown)  > 0)
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->DecreaseFlyingAngle();
		}
		if(iTouchControls->GetFramesSincePushed(EGameControlTypeUp) > 0)
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->IncreaseFlyingAngle();
		}
		if(CheckIfUserActionIsTriggered(iTouchControls->GetFramesSincePushed(EGameControlTypeIncreaseSpeed)))
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->IncreaseSpeed();
		}
		if(CheckIfUserActionIsTriggered(iTouchControls->GetFramesSincePushed(EGameControlTypeDecreaseSpeed)))
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->DecreaseSpeed();
		}
		if(iTouchControls->GetFramesSincePushed(EGameControlTypeFirePrimary)  > 0)
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->Fire1();
		}
		if(iTouchControls->GetFramesSincePushed(EGameControlTypeFireSecondary)  > 0)
		{
			iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()->Fire2();
		}
	}

	//update Frames counter for Controls Pushed
	iTouchControls->Update();
}

TBool CGameView::CheckIfUserActionIsTriggered(TInt aFrameSinceControlTouched)
{
	//check if it the button has just been touched
	if(aFrameSinceControlTouched == 0)
	{
		return true;
	}
	//check if enough frames have passed to create the first repeat input event
	else if(aFrameSinceControlTouched == GFRAMES_UNTIL_FIRST_REPEAT_INPUT_HAPPENS)
	{
		return true;
	}
	//check if we already passed the first repeat event
	else if(aFrameSinceControlTouched > GFRAMES_UNTIL_FIRST_REPEAT_INPUT_HAPPENS)
	{
		if(aFrameSinceControlTouched % GFRAMES_UNTIL_REPEAT_INPUT_HAPPENS == 0)
		{
			return true;
		}
	}

	return false;
}

void CGameView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	if(aTouchEvent.FingerId < 10)
	{
		//check if the user is currently in rearm Mode
		if(iGameData.GetMap()->GetPlayer()->UsesArmentHud())
		{
			if(iGameData.GetMap()->GetPlayer()->GetHud()->HandleTouchEvent(aTouchEvent, iGameData.GetMap()->GetPlayer()->GetCurrentAircaft()))
			{
				//Arment Hud has been selected, don't let other controls handle this event
				return;
			}
		}

		iTouchControls->HandleTouchEvent(aTouchEvent);

		//player can't click any of the buttons, to avoid that the action will be carried on to his next life
		if(iTouchControls->GetFramesSincePushed(EGameControlTypePauseMenu) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeZoomIn) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeZoomOut) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeIncreaseSpeed) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeDecreaseSpeed) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeFirePrimary) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeFireSecondary) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeLeft) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeRight) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeUp) == -1
		&& iTouchControls->GetFramesSincePushed(EGameControlTypeDown) == -1)
		{
			if(iGameData.GetMap()->GetPlayer()->PlayerCanRespawn())
			{
				if(aTouchEvent.Type == ETouchEventHandlerTouchTypeDown) //must be a new touch down event
				{
					iPlayerShouldRespawn = true;
				}
			}
		}
	}
}

void CGameView::ChildRunLogicCode()
{
	//check if TouchControls are active
	CheckForUserInput();

	if(!iGameData.iGamePaused)
	{
		CMap* lMap = iGameData.iGameMap;
		CGameObjectManager* lObjectManager = lMap->GetObjectManager();

		//update the map, the current time counter and the associated events
		lMap->Update();
		//do garbage collection and interval update
		//inspect all dead moveable objects if their time to live timer expired
		lObjectManager->CheckDeadMoveableGameObjectsForExpiring();
		//update GameObjectManager's Intervals
		lObjectManager->UpdateIntervals();
		//inspect all objects and recycle the ones that are flagged for recycle
		lObjectManager->GrabageCollectFlaggedObjects();

		//update all Objects
		//get AllGameObjects to iterate over, take a snap shot before update, since new objects might be added, but they should be updated next frame
		CPointerArray<CGameObject>* lGameObjects = CPointerArray<CGameObject>::New(*lObjectManager->GetAllGameObjects());
		CPointerArray<CMoveableGameObject>* lMoveableObjects = CPointerArray<CMoveableGameObject>::New(*lObjectManager->GetAllMoveableGameObjects());
		TInt lNumberOfGameObjects = lGameObjects->GetCount();
		TInt lNumberOfMoveableObjects = lMoveableObjects->GetCount();

		//move all moveableObjects
		for(TInt lIndex = 0; lIndex < lNumberOfMoveableObjects; lIndex++)
		{
			lMoveableObjects->Get(lIndex)->Move();
		}

		//update all game objects
		for(TInt lIndex = 0; lIndex < lNumberOfGameObjects; lIndex++)
		{
			lGameObjects->Get(lIndex)->Update();
		}

		//not needed anymore
		delete lGameObjects;
		delete lMoveableObjects;

		//Update the Player Object
		CPlayer* lPlayer = lMap->GetPlayer();
		//move Player
		lPlayer->Move();
		//update Hud
		lPlayer->Update();
		//update backgroundImage
		lPlayer->UpdateBackgroundImagePosition();


		//do collision checking and leave map
		//check if any object collides
		lObjectManager->CheckGameObjectsForCollisions();
		//check if any objects try to leave the map
		lObjectManager->CheckIfGameObjectsAreLeavingTheMap();

		//------ Update SOUND CHANNELS ----------------//
		CGame::Game->iAudioEngine->UpdateAllSoundChannelVolumeRelativeToPlayerLoaction(lPlayer->GetCurrentAircaft()->GetHitBox()->GetFastIntersectionRect());
	}
	else
	{
		if(!iIsDoingTransition) //otherwise transition doesn't work if we make a transition to the view while it is paused
		{
			//since game is paused we need to go to PauseView otherwise we are stuck
			CTransitionHorizontalFlip::New(&CGame::Game->iView, this, CGamePauseView::New(iGameData), 1);
		}
	}
}

void CGameView::ChildViewDrawMethod()
{
	CPlayer* lPlayer = iGameData.iGameMap->GetPlayer();

	//need to save state of OpenGL Projection Matrix
	GLfloat lProjectionMatrix[16];
	glMatrixMode(GL_PROJECTION);
	glGetFloatv(GL_PROJECTION_MATRIX, lProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);

	//------------ Before HUD elements ----------------//
	FixViewOnCenter();
	//draw Hud
	lPlayer->DrawHud();
	//drawBackground
	lPlayer->DrawBackgroundImage();
	//Draw Touch Controls that can be overlayed with game Objects
	iTouchControls->DrawButtonsOutsideHud();
	UnfixView();


	//------------ GAME OBJECTS ----------------//
	//move OpenGL view to center of Player Position
	lPlayer->CenterViewOnPlayerPosition();

	//get Draw Game objects that are most likely inside the view
	CPointerArray<CGameObject>* lDrawObjects = iGameData.iGameMap->GetAllDrawObjectsInRange(lPlayer->GetPosition().iX,
																						(CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth * CPlayer::ZoomFactor) / 2);

	//remove Draw Object that are outside
	const TRectIntFloat& lViewRect = lPlayer->GetPlayerViewRect();
	//check if draw object is inside the players view. Otherwise remove it
	for(TInt lIndex = 0; lIndex < lDrawObjects->GetCount(); lIndex++)
	{
		CHitBox* lObjectsHitBox = lDrawObjects->Get(lIndex)->GetHitBox();

		if(lObjectsHitBox != NULL && !lViewRect.Intersects(lObjectsHitBox->GetFastIntersectionRect()))
		{
			//not inside the view, remove object
			lDrawObjects->Remove(lIndex);
			lIndex--; //reduce index since the array shrinked, otherwise we skip the next element
		}
	}
	//sort by Z order
	lDrawObjects->SortMerge(CGameObject::DetermineObjectDrawOrder);//sort everything after Z value

	//draw Objects
	for(TInt lIndex = 0; lIndex < lDrawObjects->GetCount(); lIndex++)
	{
		lDrawObjects->Get(lIndex)->Draw();
	}
	delete lDrawObjects; //does not delete the objects, just the array that points to them

	//------------ Other HUD elements ---------------//
	FixViewOnCenter();
	//draw Other Hud element
	lPlayer->DrawOtherHudElements();
	//draw Clock timers if any
	iGameData.iGameMap->DrawClockTimers();
	//draw Controls
	iTouchControls->Draw();
	UnfixView();

	//need reset Projection state
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}

void CGameView::ContextGained()
{
	//we can only gain the context if it has been lost previously
	//Pause Music And Sound, since we going to Pause anyway
	CGame::Game->iAudioEngine->GetMusicPlayer().Pause();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Pause();
}

void CGameView::ContextLost()
{
	//pause Music And Sound
	CGame::Game->iAudioEngine->GetMusicPlayer().Pause();
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Pause();

	//switch to PauseView
	iGameData.SetPaused(true);
}

void CGameView::FixViewOnCenter()
{
	//since we change the state, we need to render textures in old state first
	CFighterPilotThePacificWar::FighterGame->iGraphicsEngine->FlushBatchedTexturesToScreen();

	//change projection so that we face the center of the player view
	glMatrixMode(GL_PROJECTION);
	glGetFloatv(GL_PROJECTION_MATRIX, iProjectionMatrix);
	glLoadIdentity();
	glOrthox(0, TIntFloat::Convert(1024).GetIntInBaseIntFloat(), //left X, right X
					 0, TIntFloat::Convert(660).GetIntInBaseIntFloat(), //left Y, right Y
					 TIntFloat::Convert(32000).GetIntInBaseIntFloat(),//near Z, far Z
					 TIntFloat::Convert(-32000).GetIntInBaseIntFloat()
					);
}

void CGameView::UnfixView()
{
	//since we change the state, we need to render textures in old state first
	CFighterPilotThePacificWar::FighterGame->iGraphicsEngine->FlushBatchedTexturesToScreen();

	//set back to the default mode
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(iProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}

