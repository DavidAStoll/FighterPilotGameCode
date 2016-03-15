/*
 * CEndMissionTempTransitionView.cpp
 *
 *  Created on: Dec 21, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CEndMissionTempTransitionView.h"
#include "includes/game/views/CMissionVictoryView.h"
#include "includes/core/views/transitions/CTransitionCubeRotation.h"

CEndMissionTempTransitionView::CEndMissionTempTransitionView(TBool aMissionWon, TBool aGameSave, TInt aLivesLeft, TInt aScore, TInt aExpectedMissionTimeInMins, CStatsTracker* aStatsTracker)
{
	iMissionWon = aMissionWon;
	iGameSave = aGameSave;
	iLivesLeft = aLivesLeft;
	iScore = aScore;
	iExpectedMissionTimeInMins = aExpectedMissionTimeInMins;
	iStatsTracker = aStatsTracker;
	iFrame = 0;
}

CEndMissionTempTransitionView::~CEndMissionTempTransitionView()
{
	// TODO Auto-generated destructor stub
}

CEndMissionTempTransitionView* CEndMissionTempTransitionView::New(TBool aMissionWon, TBool aGameSave, TInt lLivesLeft, TInt lScore, TInt lExpectedMissionTimeInMins, CStatsTracker* lStatsTracker)
{
	CEndMissionTempTransitionView* lSelf = new CEndMissionTempTransitionView(aMissionWon, aGameSave, lLivesLeft, lScore, lExpectedMissionTimeInMins, lStatsTracker);
	lSelf->Construct();
	return lSelf;
}

void CEndMissionTempTransitionView::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendTileScreen), 1024, 660, 100, 0, 1, 0, 1);
}

void CEndMissionTempTransitionView::ChildConsumeTouchEvent(const STouchEventLocationConverted& iTouchEvent)
{

}

void CEndMissionTempTransitionView::ChildViewDrawMethod()
{
	iBackgroundTexture->DrawArrays();
}

void CEndMissionTempTransitionView::ChildRunLogicCode()
{
	iFrame++;

	if(iFrame > CGame::Game->iFramesPerSecondCounter->GetFramesPerSecond() / 2) //half a second
	{
		// did it for 1 sec, continue to the Victory Screen
		CTransitionCubeRotation::New(&CGame::Game->iView, this, CMissionVictoryView::New(iMissionWon, iGameSave, iLivesLeft, iScore, iExpectedMissionTimeInMins, iStatsTracker), 1);
	}
}
