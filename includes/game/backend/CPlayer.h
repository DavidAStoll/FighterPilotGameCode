/*
 * CPlayer.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CPLAYER_H_
#define CPLAYER_H_

#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/CHud.h"
#include "includes/game/backend/CSpeedoMeter.h"
#include "includes/game/backend/CHeightoMeter.h"
#include "includes/game/backend/CTouchScreenToRespawnReminder.h"
#include "includes/game/backend/CSmallPlaneLocator.h"
#include "includes/game/backend/CHeavyBomberLocator.h"
#include "includes/game/backend/CBombingGuidingSystem.h"
#include "includes/game/backend/basicClasses/CBackgroundImage.h"

class CStatsTracker;
class CGameObject;

// CLASS DECLARATION

/**
 *  CPlayer
 *
 * This class will be used to handle all interactions with the Player and the game
 * As well as keeping track of the score and name
 */

enum TMissionStatus //used to keep track if the game is Lost, Won or not yet decided
{
	EMissionStatusUndecided, EMissionStatusSuccess, EMissionStatusFailed
};

class CPlayer
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CPlayer();

	static CPlayer* New();
	void CenterViewOnPlayerPosition();// resets the orthogonal view to the position of the current aircraft
	void AssignAircarft(CAircraft* aAircraft);//gives the player a new aircraft
	void AssignPlayerBase(CGameObject* aBase);
	CGameObject* GetPlayerBase();
	CAircraft* GetCurrentAircaft();
	void Move();//moves the player positions, which results in moving the camera
	void AdjustViewToTheRight();
	void AdjustViewToTheLeft();
	TPointIntFloat& GetPosition();
	void DrawHud();
	void DrawOtherHudElements();
	void Update(); //runs logic code of player object
	TMissionStatus GetMissionStatus();
	//set Game status
	void SetMissionStatus(TMissionStatus aNewStatus);
	//just returns the current PlayerRect
	const TRectIntFloat& GetPlayerViewRect();
	TInt GetPoints();
	TInt GetLivesLeft();
	void ChangeLives(TInt aChange);
	void ChangePoints(TInt aChange);
	void ChangePlayerXLocation(TIntFloat aChange);
	TBool UsesArmentHud();
	CHud* GetHud();
	void UpdateBackgroundImagePosition();
	void UpdateBackgroundImagePositionWithoutSpeed();
	void DrawBackgroundImage();
	TBool SpawnNewPlayerDelayed();
	void SpawnNewPlayerImmediately();
	TBool PlayerCanRespawn(); //does all the nessary checks to tell an outsider if the player is capable of spawning another plane
	TBool GetIsInZoomOutMode();
	void ChangeZoomMode(TFloat aZoomeFactor);// either sets it to zoom in or to zoom out
	//used during loading, to make sure that the PlayerObject has no dangling pointers and such that could mess things up during loading
	void ResetForLoad();
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aReadStream);
	void SetDeleteStatsTracker(TBool aValue);
	void SetGameSaved(TBool aValue);
	TBool GetGameSaved();
	static TFloat ZoomFactor;
	//should be called when all pointers have been restablished
	void PostLoadSetupFunction();
	//returns the current StatsTracker of the object
	CStatsTracker* GetStatsTracker();
	//these two functions should be called to stop the tracking of time and to resume it
	void UpdateStatsTrackerTimeValues();


protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CPlayer();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void CheckIfNewPlayerHasToBeSpawned();

	TRectIntFloat iPlayerViewRect;
	TInt iPoints; //how many points the user has scored
	TInt iLives; //how many lives are remaining
	TInt iFramesSincePlayerAircraftDied;
	CAircraft* iAircraft;//the plane the user is using
	TIntFloat* iPixelsPerMoveX;// tells us how many pixels the object will move, might be negitive if we move left
	TIntFloat* iPixelsPerMoveY;
	TInt* iFramesPerMove;// tells us after how many Frames an object will move
	TInt* iAngel;
	TPointIntFloat iPlayerLocation;
	TBool iPlayerWaitedEnoughTimeToRespawn;
	TBool iIsInGroundViewMode;
	TBool iIsInTopView;
	TBool iIsInLeftView;
	TBool iIsInRightView;
	TBool iIsFacingRight;
	TBool iDeleteStatsTracker;
	TBool iGameSaved;
	TBool iSpawnPlayer;
	CString* iStringMissionWon;
	CString* iStringMissionLost;
	SFont iFontForMissionStatus;
	TInt iTimeInFrames;

	TMissionStatus iMissionStatus;
	CHud* iHud;
	CBombingGuidingSystem* iBombingSystem;
	CTouchScreenToRespawnReminder* iTouchScreenToRespawnReminder;
	CHeightoMeter* iHeightoMeter;
	CSpeedoMeter* iSpeedoMeter;
	CSmallPlaneLocator* iSmallPlaneLocator;
	CHeavyBomberLocator* iHeavyBomberLocator;
	CBackgroundImage* iBackgroundImage;
	TBool iIsUsingArmentHud;
	CGameObject* iPlayerBase;
	CStatsTracker* iStatsTracker;
};

#endif /* CPLAYER_H_ */
