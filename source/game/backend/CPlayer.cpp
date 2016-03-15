/*
 ============================================================================
 Name		: Player.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CPlayer implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CPlayer.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/game/backend/basicClasses/CMap.h"

GLfloat CPlayer::ZoomFactor = 1;

CPlayer::CPlayer()
{
	iPoints = 0;
	iLives = CGameObjectDefaultValues::PlayerLives;
	iAircraft = NULL;
	iBackgroundImage = NULL;
	iHud = NULL;
	iIsInTopView = false;
	iIsInGroundViewMode = true;
	iIsFacingRight = true;
	iIsInRightView = false;
	iIsInLeftView = false;
	iIsUsingArmentHud = false;
	iGameSaved = false;
	iPlayerWaitedEnoughTimeToRespawn = false;
	iSpawnPlayer = false;
	iDeleteStatsTracker = true;
	iMissionStatus = EMissionStatusUndecided;
	CPlayer::ZoomFactor = 1.5;
	iPlayerLocation.iX = TIntFloat::Convert(0);
	iPlayerLocation.iY = TIntFloat::Convert(0);
	iFramesSincePlayerAircraftDied = 0;
	iTimeInFrames = 0;
}

CPlayer::~CPlayer()
{
	if(iHud)
	{
		delete iHud;
		iHud = NULL;
	}
	if(iBackgroundImage)
	{
		delete iBackgroundImage;
		iBackgroundImage = NULL;
	}
	if(iSpeedoMeter)
	{
		delete iSpeedoMeter;
		iSpeedoMeter = NULL;
	}
	if(iHeavyBomberLocator)
	{
		delete iHeavyBomberLocator;
		iHeavyBomberLocator = NULL;
	}
	if(iSmallPlaneLocator)
	{
		delete iSmallPlaneLocator;
		iSmallPlaneLocator = NULL;
	}
	if(iBombingSystem)
	{
		delete iBombingSystem;
		iBombingSystem = NULL;
	}
	if(iHeightoMeter)
	{
		delete iHeightoMeter;
		iHeightoMeter = NULL;
	}
	if(iTouchScreenToRespawnReminder)
	{
		delete iTouchScreenToRespawnReminder;
		iTouchScreenToRespawnReminder = NULL;
	}
	if(iDeleteStatsTracker) //the stats player object will not be delete if we go to the MissionSuccessful screen, since it needs it
	{
		if(iStatsTracker)
		{
			delete iStatsTracker;
			iStatsTracker = NULL;
		}
	}
	delete iStringMissionWon;
	delete iStringMissionLost;
}

CPlayer* CPlayer::New()
{
	CPlayer* self = new  CPlayer();
	self->ConstructL();
	return self;
}

void CPlayer::ConstructL()
{
	iStatsTracker = CStatsTracker::New();//violates two phase construction but who cares
	iHud = CHud::New();
	iSpeedoMeter = CSpeedoMeter::New(NULL, TIntFloat::Convert(10));//will be changed once an airplane is assigned
	iHeightoMeter = CHeightoMeter::New(iAircraft);
	iSmallPlaneLocator = CSmallPlaneLocator::New(&iPlayerLocation, EConflictSideAmerican);
	iHeavyBomberLocator = CHeavyBomberLocator::New(&iPlayerLocation, EConflictSideAmerican);
	iBackgroundImage = CBackgroundImage::New();
	iBombingSystem = CBombingGuidingSystem::New(iAircraft, 0, 0);
	iTouchScreenToRespawnReminder = CTouchScreenToRespawnReminder::New(this);

	iStringMissionWon = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameExtra.MissionStatusSuccess");
	iStringMissionLost = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameExtra.MissionStatusFailed");

	//find proper Font
	CPointerArray<CString>* lStringArray = CPointerArray<CString>::New();
	lStringArray->Append(iStringMissionWon);
	lStringArray->Append(iStringMissionLost);
	iFontForMissionStatus = CGame::Game->iGraphicsFont->CreateFontThatFitsHorizontalStrings(lStringArray, 1000, 50);
	//clean up
	delete lStringArray;
}

//------------------------------- funcitons ---------------------------------------

void CPlayer::SetDeleteStatsTracker(TBool aValue)
{
	iDeleteStatsTracker = aValue;
}

void CPlayer::SetGameSaved(TBool aValue)
{
	iGameSaved = aValue;
}

TBool CPlayer::GetGameSaved()
{
	return iGameSaved;
}

void CPlayer::AssignAircarft(CAircraft* aAircraft)
{
	iAircraft = aAircraft;
	//update how fast the player will move
	iPlayerLocation.iX = iAircraft->GetCurrentPosition().iX;
	iPlayerLocation.iY = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight / 2;
	iPixelsPerMoveX = iAircraft->GetPixelsPerMoveX();
	iPixelsPerMoveY = iAircraft->GetPixelsPerMoveY();
	iAngel = iAircraft->GetFlyingAngel();
	iSpeedoMeter->UpdateObjectProperties(&iAircraft->GetCurrentEngineAccelerationRef(), iAircraft->GetNormalMaxEngineAcceleration());
	iHeightoMeter->AssignAircraft(iAircraft);
	iBombingSystem->AssignAircraft(iAircraft);
	UpdateBackgroundImagePositionWithoutSpeed();
	ChangeZoomMode(0); //need to adjust Zoom Value
}

TBool CPlayer::PlayerCanRespawn()
{
	if(iLives == 0)
		return false;
	else if(!iPlayerWaitedEnoughTimeToRespawn)
		return false;


	if(!iPlayerBase->IsAlive() || iPlayerBase->GetConflictSide() != iAircraft->GetConflictSide()) //has been captured
	{
		//player base is dead, is it possible to find another one?
		//look for another potential player base that we could start from
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesPlaneSpawnLocation);
		TBool lFoundAnotherBase = false;

		for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

			if(lCurrentGameObject->IsAlive() && //has to be alive
				lCurrentGameObject->GetConflictSide() == iAircraft->GetConflictSide()) //needs to be on the same side
			{
				lFoundAnotherBase = true;
				break;
			}
		}
		delete lGameObjects;

		if(!lFoundAnotherBase)
			return false;
	}
	//check all valid conditions, should be able to respawn
	return true;
}

TBool CPlayer::SpawnNewPlayerDelayed()
{

	if(iSpawnPlayer)
	{
		return false; //already spawning a new player
	}
	else if(iPlayerBase->IsAlive() &&  ( !iAircraft || iPlayerBase->GetConflictSide() == iAircraft->GetConflictSide()))
	{
		iSpawnPlayer = true;
		return true;
	}
	else
	{
		//look for another potential player base that we could start from
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesPlaneSpawnLocation);

		for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

			if(lCurrentGameObject->IsAlive() && //has to be alive
				lCurrentGameObject->GetConflictSide() == iAircraft->GetConflictSide()) //needs to be on the same side
			{
				AssignPlayerBase(lCurrentGameObject);
				delete lGameObjects;
				iSpawnPlayer = true;
				return true;
			}
		}
		delete lGameObjects;
	}
	return false; //didn't find a new base
}

void CPlayer::SpawnNewPlayerImmediately()
{
	SpawnNewPlayerDelayed();
	CheckIfNewPlayerHasToBeSpawned();
}

void CPlayer::AssignPlayerBase(CGameObject* aBase)
{
	iPlayerBase = aBase;
}

CGameObject* CPlayer::GetPlayerBase()
{
	return iPlayerBase;
}

CAircraft* CPlayer::GetCurrentAircaft()
{
	return iAircraft;
}

// either sets it to zoom in or to zoom out
void CPlayer::ChangeZoomMode(GLfloat aZoomeFactor)
{
	CPlayer::ZoomFactor += aZoomeFactor;

	//needs a special case, because when we are zoomed out and then zoom in it might mess stuff up because we then should not be inGroundView mode anymore
	if(iIsInGroundViewMode)
	{
		TIntFloat lMinYPosition = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight * CPlayer::ZoomFactor;
		TIntFloat lMaxYPosition = TIntFloat::Convert(GMAX_HEIGHT_IN_GAME_LEVEL_FOR_PLANE) - (lMinYPosition / 2);
		lMaxYPosition += iAircraft->GetHeight();
		TIntFloat lPlaneYPosition = iAircraft->GetCurrentPosition().iY;

		if(lMinYPosition < lPlaneYPosition)
		{
			iIsInGroundViewMode = false;//moved outside, focus on player
			iPlayerLocation.iY = lPlaneYPosition + iAircraft->GetHeight(); //just adjust Y
		}else
		{
			iIsInGroundViewMode = true;
			iPlayerLocation.iY = lMinYPosition / 2;
		}
	}

	iIsInTopView = false;
	iIsInLeftView = false;
	iIsInRightView = false;
	iBackgroundImage->UpdateForZoomFactor();
}

void CPlayer::CenterViewOnPlayerPosition()
{
	CGame::Game->iGraphicsEngine->CenterViewToLocation(iPlayerLocation, CPlayer::ZoomFactor, CPlayer::ZoomFactor);
	//update the player view rect
	TIntFloat lWidthOfScreen = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth * CPlayer::ZoomFactor;
	TIntFloat lHeightOfScreen =  CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight * CPlayer::ZoomFactor;
	//top left corner of rect
	iPlayerViewRect.iTl.iX = iPlayerLocation.iX - lWidthOfScreen / 2;
	iPlayerViewRect.iTl.iY = iPlayerLocation.iY + lHeightOfScreen / 2;
	//bottom right corner of rect
	iPlayerViewRect.iBr.iX = iPlayerLocation.iX + lWidthOfScreen / 2;
	iPlayerViewRect.iBr.iY = iPlayerLocation.iY - lHeightOfScreen / 2;
}

void CPlayer::AdjustViewToTheLeft()
{
	iIsFacingRight = false;
	TInt lCurrentAngel = *iAngel;
	while(*iAngel != 0)
	{
		iAircraft->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
	}
	iPlayerLocation.iX = iAircraft->GetCurrentPosition().iX; //now should have proper X coordinates
	while(*iAngel != lCurrentAngel)
	{
		iAircraft->DecreaseFlyingAngleWithoutSpeedCheckSmallAngle();
	}
	//now airplane should be back at its former position
}

void CPlayer::AdjustViewToTheRight()
{
	iIsFacingRight = true;
	TInt lCurrentAngel = *iAngel;
	while(*iAngel != 180)
	{
		iAircraft->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
	}
	iPlayerLocation.iX = iAircraft->GetCurrentPosition().iX; //now should have proper X coordinates
	while(*iAngel != lCurrentAngel)
	{
		iAircraft->DecreaseFlyingAngleWithoutSpeedCheckSmallAngle();
	}
	//now airplane should be back at its former position
}

TMissionStatus CPlayer::GetMissionStatus()
{
	return iMissionStatus;
}

void CPlayer::SetMissionStatus(TMissionStatus aNewStatus)
{
	iMissionStatus = aNewStatus;
}

void CPlayer::Move()
{
	if(iAircraft)
	{
		//check if we camera can move along the Y direction
		TIntFloat lMinYPosition = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight * CPlayer::ZoomFactor;
		TIntFloat lMaxYPosition = TIntFloat::Convert(GMAX_HEIGHT_IN_GAME_LEVEL_FOR_PLANE) - (lMinYPosition / 2);
		lMaxYPosition += iAircraft->GetHeight();
		TIntFloat lPlaneYPosition = iAircraft->GetCurrentPosition().iY;

		if(iIsInGroundViewMode)//in this mode Y does not change to make it easier to see ground units
		{
			//check if we moved outside the border of GroundViewMode, which is usually the height of the screen
			if(lMinYPosition < lPlaneYPosition)
			{
				iIsInGroundViewMode = false;//moved outside, focus on player
				iPlayerLocation.iY = lMinYPosition;
			}
		}
		else
		{
			//here adjust just Y
			if(lMinYPosition > lPlaneYPosition)
			{
				//need to switch to GroundView again
				iIsInGroundViewMode = true;
				iPlayerLocation.iY = lMinYPosition / 2;
			}else if(lPlaneYPosition > lMaxYPosition)// is in top view
			{
				iPlayerLocation.iY = lMaxYPosition;
				iIsInTopView = true;
			}else //not in top view anymore
			{
				if(iIsInTopView && lPlaneYPosition < lMaxYPosition)
				{
					iIsInTopView = false;
					if(iPlayerLocation.iY == lMaxYPosition) //means that we move from topView into not TopView, it is also possible that this is not the case
						iPlayerLocation.iY = lMaxYPosition + iAircraft->GetHeight(); //just adjust Y
				}else
				{
					iPlayerLocation.iY += *iPixelsPerMoveY; //just adjust Y
				}
			}
		}

		//check if camera can move along the X direction
		TInt lLeftMostXPoint = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftXOutermostCoordinate() + (890 / 2) * CPlayer::ZoomFactor; // add Screen width, which is 1024 - button width(67) twice
		TInt lRightMostXPoint = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightXOutermostCoordinate() - (890  / 2) * CPlayer::ZoomFactor; // substract Screen width, which is 1024 - button width(67) twice
		TInt lPlaneXPosition = iAircraft->GetCurrentPosition().iX.GetIntInBaseInt();

		if(iIsInLeftView)
		{
			if(lPlaneXPosition >= lLeftMostXPoint)
			{
				iIsInLeftView = false;
			}
		}else if(iIsInRightView)
		{
			if(lPlaneXPosition <= lRightMostXPoint)
			{
				iIsInRightView = false;
			}
		}else //not in the left or right view, check if we are now in one of those cases. If not, just update X
		{
			if(lPlaneXPosition < lLeftMostXPoint)
			{
				iIsInLeftView = true;
				iPlayerLocation.iX = TIntFloat::Convert(lLeftMostXPoint);
			}else if(lPlaneXPosition > lRightMostXPoint)
			{
				iIsInRightView = true;
				iPlayerLocation.iX = TIntFloat::Convert(lRightMostXPoint);
				;
			}else
			{
				iPlayerLocation.iX += *iPixelsPerMoveX;
			}
		}

		//adjust view Direction if not in the left or right view and is not turning
		if(!iIsInLeftView && !iIsInRightView && !iAircraft->IsTurning())
		{
			if(iIsFacingRight)
			{
				if(*iAngel < 90 || *iAngel > 270)//should be facing left
				{
					AdjustViewToTheLeft();
				}
			}else
			{
				if(*iAngel > 90 && *iAngel < 270)//should be facing right
				{
					AdjustViewToTheRight();
				}
			}
		}
	}
}

TPointIntFloat& CPlayer::GetPosition()
{
	return iPlayerLocation;
}

CStatsTracker* CPlayer::GetStatsTracker()
{
	return iStatsTracker;
}

void CPlayer::Update()
{
	//don't update if game is paused
	if(iMissionStatus == EMissionStatusUndecided)
	{
		iTimeInFrames++;
	}

	//----------------- Check if the Mission Failed because Player has no more lives left -----------------//
	if(iLives == 0 && !iAircraft->IsAlive() && iMissionStatus == EMissionStatusUndecided)
	{
		iMissionStatus = EMissionStatusFailed;
	}
	if(iAircraft && !iAircraft->IsAlive() && !iPlayerWaitedEnoughTimeToRespawn) //not alive
	{
		iFramesSincePlayerAircraftDied++;
		if(iFramesSincePlayerAircraftDied > GPLAYER_WAIT_FRAMES_UNTIL_RESPAWN_POSSIBLE)
		{
			iPlayerWaitedEnoughTimeToRespawn = true;
			iFramesSincePlayerAircraftDied = 0;
		}
	}

	//check if the player needs a new aircraft
	CheckIfNewPlayerHasToBeSpawned();

	//look for any bombers and point where they are
	iHeavyBomberLocator->Update();
	//look for small planes
	iSmallPlaneLocator->Update();

	//---------------- Checks if we are currently in the menu to rearm the aircraft ----------------------//
	//check if need to open the ArmentHud
	if(!iAircraft->IsFlying() && iAircraft->GetCurrentEngineAccelerationRef() == TIntFloat::Convert(0) && iAircraft->IsAlive())
	{
		//armentHud should be displayed
		if(!iIsUsingArmentHud)
		{
			//first time use, therefore intilize and set flag to true
			iHud->InitilizeArmentHud();
			iIsUsingArmentHud = true;
		}
	}else
		iIsUsingArmentHud = false;
}

void CPlayer::DrawHud()
{
	iHud->Draw(this);
}

void CPlayer::DrawOtherHudElements()
{
	//draw SpeedoMeter
	iSpeedoMeter->Draw();

	//draw HeavyBomberLocator
	iHeavyBomberLocator->Draw();

	//draw smallPlanelocator
	iSmallPlaneLocator->Draw();

	//draw gameHints if they are enabeled and game is not paused
	if(!CFighterPilotThePacificWar::FighterGame->iGameData->iGamePaused)
		iTouchScreenToRespawnReminder->Draw();

	//bombing system
	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty != EGameDifficultyHard && !iAircraft->IsTurning())
		iBombingSystem->Draw(); //draw also does the calculations as well for the bombing system

	if(iIsUsingArmentHud && !CFighterPilotThePacificWar::FighterGame->iGameData->iGamePaused)//don't draw the weapon selection screen if game is paused since it might cut of some of the text in the main menu
		iHud->DrawArmentHud();

	//draw height of plane
	iHeightoMeter->Draw();

	//draw Mission Status if Mission is lost or Won
	if(iMissionStatus != EMissionStatusUndecided)
	{
		CGame::Game->iGraphicsFont->SetFont(iFontForMissionStatus);
		CGame::Game->iGraphicsFont->SetColour(KColourRed);
		CString* lStringMissionStatus = NULL;

		if(iMissionStatus == EMissionStatusSuccess)
		{
			lStringMissionStatus = iStringMissionWon;
		}
		else
		{
			lStringMissionStatus = iStringMissionLost;
		}

		//draw centered
		TInt lWidthOfScreen = 1024;
		TInt lXOffset = (lWidthOfScreen - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForMissionStatus, lStringMissionStatus)) / 2;
		CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(TPoint(lXOffset, 500), lStringMissionStatus);
	}
}

TInt CPlayer::GetLivesLeft()
{
	return iLives;
}

//just returns the current PlayerRect
const TRectIntFloat& CPlayer::GetPlayerViewRect()
{
	return iPlayerViewRect;
}

TInt CPlayer::GetPoints()
{
	return iPoints;
}

void CPlayer::ChangePoints(TInt aChange)
{
	iPoints += aChange;
}

void CPlayer::ChangePlayerXLocation(TIntFloat aChange)
{
	iPlayerLocation.iX += aChange;
}

void CPlayer::ChangeLives(TInt aChange)
{
	iLives += aChange;
}

TBool CPlayer::UsesArmentHud()
{
	return iIsUsingArmentHud;
}

CHud* CPlayer::GetHud()
{
	return iHud;
}
void CPlayer::UpdateBackgroundImagePositionWithoutSpeed()
{
	TPointIntFloat lPosition;
	TIntFloat lWidthOfScreen = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
	TIntFloat lHeightOfScreen = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight;
	lPosition.iX = iPlayerLocation.iX - ((lWidthOfScreen / 2));//since player location will be at the center of the screen
	lPosition.iY = iPlayerLocation.iY - (lHeightOfScreen / 2);//since the background is always at the bottom of the Screen's left corner
	iBackgroundImage->UpdatePosition(lPosition, iIsInGroundViewMode, 0);
}

void CPlayer::UpdateBackgroundImagePosition()
{
	TPointIntFloat lPosition;
	TIntFloat lWidthOfScreen = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth;
	TIntFloat lHeightOfScreen = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight;
	lPosition.iX = iPlayerLocation.iX - ((lWidthOfScreen / 2));//since player location will be at the center of the screen
	lPosition.iY = iPlayerLocation.iY - (lHeightOfScreen / 2);//since the background is always at the bottom of the Screen's left corner
	TInt lCurrentSpeed = (*iAircraft->GetPixelsPerMoveX()).GetIntInBaseInt();

	//if we the view is currently locked in one of the sideviews, don't move background due to airplane speed
	if(iIsInLeftView || iIsInRightView)
		iBackgroundImage->UpdatePosition(lPosition, iIsInGroundViewMode, 0);
	else
		iBackgroundImage->UpdatePosition(lPosition, iIsInGroundViewMode, lCurrentSpeed);
}

void CPlayer::DrawBackgroundImage()
{
	iBackgroundImage->Draw();
}
void CPlayer::ResetForLoad()
{
	//has to be reset since some GameObject might use this Aircraft pointer during construction
	iAircraft = NULL;
}

void CPlayer::UpdateStatsTrackerTimeValues()
{
	TInt lTimeInSeconds = iTimeInFrames / GFRAMES_PER_SECOND_BACKEND;

	iStatsTracker->iGameTimeHours = lTimeInSeconds / (3600);
	iStatsTracker->iGameTimeMins = (lTimeInSeconds - (iStatsTracker->iGameTimeHours * 3600)) / 60;
	iStatsTracker->iGameTimeSecs = lTimeInSeconds % 60;
}

void CPlayer::CheckIfNewPlayerHasToBeSpawned()
{
	if(iSpawnPlayer)
	{
		//find out what weapon the new plane will start with
		TWeaponIdentifier lCurrentPlayerWeapon = EWeaponIdentifier1000kgBomb;//this is the default weapon selected
		if(iAircraft && iAircraft->GetSecondaryWeapon())
			lCurrentPlayerWeapon = iAircraft->GetSecondaryWeapon()->GetWeaponIdentifier();//this is weapon the player used when he last died

		CAircraft* lNewPlane = iPlayerBase->CreateNewAircraft();
		lNewPlane->EquipWithNewSecondaryWeapon(lCurrentPlayerWeapon);
		AssignAircarft(lNewPlane);
		iPlayerWaitedEnoughTimeToRespawn = false;//set back the counter to respawn
		iSpawnPlayer = false;
	}
}

void CPlayer::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteUint32((TUint32)iAircraft);
	aOutputStream.WriteUint32((TUint32)iPlayerBase);
	aOutputStream.WriteInt32(iPoints);
	aOutputStream.WriteInt32(iLives);
	aOutputStream.WriteInt32(iFramesSincePlayerAircraftDied);
	aOutputStream.WriteIntFloat(iPlayerLocation.iX);
	aOutputStream.WriteIntFloat(iPlayerLocation.iY);
	aOutputStream.WriteIntFloat(iPlayerViewRect.iTl.iX);
	aOutputStream.WriteIntFloat(iPlayerViewRect.iTl.iY);
	aOutputStream.WriteIntFloat(iPlayerViewRect.iBr.iX);
	aOutputStream.WriteIntFloat(iPlayerViewRect.iBr.iY);
	aOutputStream.WriteIntFloat(TIntFloat::Convert(CPlayer::ZoomFactor));
	aOutputStream.WriteInt32(iPlayerWaitedEnoughTimeToRespawn);
	aOutputStream.WriteInt32(iIsInGroundViewMode);
	aOutputStream.WriteInt32(iIsInTopView);
	aOutputStream.WriteInt32(iIsInLeftView);
	aOutputStream.WriteInt32(iIsInRightView);
	aOutputStream.WriteInt32(iIsFacingRight);
	aOutputStream.WriteInt32(iMissionStatus);
	aOutputStream.WriteInt32(iIsUsingArmentHud);
	aOutputStream.WriteInt32(iGameSaved);
	aOutputStream.WriteInt32(iSpawnPlayer);
	aOutputStream.WriteInt32(iTimeInFrames);;
	iHud->SaveToDisk(aOutputStream);
	iHeavyBomberLocator->SaveOnDisk(aOutputStream);
	iSmallPlaneLocator->SaveOnDisk(aOutputStream);
	iStatsTracker->SaveOnDisk(aOutputStream);
}

void CPlayer::LoadFromDisk(CFileReadStream &aReadStream)
{
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iAircraft));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iPlayerBase));
	iPoints = aReadStream.ReadInt32();
	iLives = aReadStream.ReadInt32();
	iFramesSincePlayerAircraftDied = aReadStream.ReadInt32();
	iPlayerLocation.iX = aReadStream.ReadIntFloat();
	iPlayerLocation.iY = aReadStream.ReadIntFloat();
	iPlayerViewRect.iTl.iX = aReadStream.ReadIntFloat();
	iPlayerViewRect.iTl.iY = aReadStream.ReadIntFloat();
	iPlayerViewRect.iBr.iX = aReadStream.ReadIntFloat();
	iPlayerViewRect.iBr.iY = aReadStream.ReadIntFloat();
	CPlayer::ZoomFactor = aReadStream.ReadIntFloat().GetAsDouble();
	iPlayerWaitedEnoughTimeToRespawn = aReadStream.ReadInt32();
	iIsInGroundViewMode = aReadStream.ReadInt32();
	iIsInTopView = aReadStream.ReadInt32();
	iIsInLeftView = aReadStream.ReadInt32();
	iIsInRightView = aReadStream.ReadInt32();
	iIsFacingRight = aReadStream.ReadInt32();
	iMissionStatus = static_cast<TMissionStatus> (aReadStream.ReadInt32());
	iIsUsingArmentHud = aReadStream.ReadInt32();
	iGameSaved = aReadStream.ReadInt32();
	iSpawnPlayer = aReadStream.ReadInt32();
	iTimeInFrames = aReadStream.ReadInt32();
	iHud->LoadFromDisk(aReadStream);
	iHeavyBomberLocator->LoadFromDisk(aReadStream);
	iSmallPlaneLocator->LoadFromDisk(aReadStream);
	iStatsTracker->LoadFromDisk(aReadStream);
}

void CPlayer::PostLoadSetupFunction()
{
	iPixelsPerMoveX = iAircraft->GetPixelsPerMoveX();
	iPixelsPerMoveY = iAircraft->GetPixelsPerMoveY();
	iAngel = iAircraft->GetFlyingAngel();
	iSpeedoMeter->UpdateObjectProperties(&iAircraft->GetCurrentEngineAccelerationRef(), iAircraft->GetNormalMaxEngineAcceleration());
	iBombingSystem->AssignAircraft(iAircraft);
	iHeightoMeter->AssignAircraft(iAircraft);
	ChangeZoomMode(0);//reset to the zoom at which the game was saved
	UpdateBackgroundImagePositionWithoutSpeed();
}
