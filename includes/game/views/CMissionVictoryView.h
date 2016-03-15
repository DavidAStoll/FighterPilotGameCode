/*
 * CMissionVictoryView.h
 *
 *  Created on: Sep 24, 2012
 *      Author: dstoll
 */

#ifndef CMISSIONVICTORYVIEW_H_
#define CMISSIONVICTORYVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CScrollBar.h"
#include "includes/core/views/components/CTouchSurface.h"

class CMissionVictoryView : public CBasicView
{
public:

	virtual ~CMissionVictoryView();
	static CMissionVictoryView* New(TBool aMissionWon, TBool aGameSaved, TInt aLifesLeft, TInt aScore, TInt aExpectedMissionTimeInMins, CStatsTracker* aStatsTracker);

protected:
	CMissionVictoryView(TBool aMissionWon, TBool aGameSaved, TInt aLifesLeft, TInt aScore, TInt aExpectedMissionTimeInMins, CStatsTracker* aStatsTracker);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//draws the Line of the Stats data is bigger than 0
	void DrawStatIfGreaterThanZero(TPoint& aTopLocation, const SFont& aFont, CString* aString, TInt aNumberOfStat, TInt aMultiplierValue);
	//finds the MaxY value for scrolling down, checks what Stats the StatsTracker has
	TInt FindMaxY(const SFont& aFontForStats);

	CTextureObject* iBackgroundTexture;
	CTextureObject* iDescriptionBackgroundTexture;

	TFloat iStatsYOffset;
	TInt iTotalScore;
	TInt iLivesLeft;
	TInt iExpectedMissionTimeInMins;
	TInt iTimePenaltyBonus;

	TBool iMissionWon;
	TBool iGameSaved;

	CStatsTracker* iStatsTracker;

	SMission* iMission;
	SCampaign* iCampaign;

	CString* iStringRestartMission;
	CString* iStringContinue;
	CString* iStringMissionStatus;
	CString* iStringMissionName;
	CString* iStringUserMissionTime;
	//for Stats
	CString* iStringMultiplierSymbol;
	CString* iStringKills;
	CString* iStringLoses;
	CString* iStringHeavyBomber;
	CString* iStringDiveBomber;
	CString* iStringAircraftCarrier;
	CString* iStringDestroyer;
	CString* iStringBattleshipYamato;
	CString* iStringLctLoaded;
	CString* iStringLctEmpty;
	CString* iStringTroopTransporter;
	CString* iStringOilTanker;
	CString* iStringAirport;
	CString* iStringComStation;
	CString* iStringHangar;
	CString* iStringHQ;
	CString* iStringAA20mmMachineGun;
	CString* iStringAA75mmType88;
	CString* iStringAAConcrete;
	CString* iStringMediumTank;
	CString* iStringAATank;
	CString* iStringRifleman;
	CString* iStringAntiTankman;
	CString* iStringUSSVictory;
	CString* iStringBattleshipUSSTennessee;
	CString* iStringBonus;
	CString* iStringLivesLeft;
	CString* iStringMissionTime;
	CString* iStringSaveGameMultiplier;
	CString* iStringDifficultyMultiplier;
	CString* iStringTotalScore;

	SFont iFontForButtons;
	SFont iFontForMissionStatus;
	SFont iFontForMissionName;
	SFont iFontForStats;

	CButton* iButtonRestart;
	CButton* iButtonContinue;
	CScrollBar* iScrollBar;
	CTouchSurface* iTouchSurface;

};

#endif /* CMISSIONVICTORYVIEW_H_ */
