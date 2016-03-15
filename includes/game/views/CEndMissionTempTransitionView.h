/*
 * CEndMissionTempTransitionView.h
 *
 *  Created on: Dec 21, 2012
 *      Author: dstoll
 */

#ifndef CENDMISSIONTEMPTRANSITIONVIEW_H_
#define CENDMISSIONTEMPTRANSITIONVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/CFighterPilotThePacificWar.h"

class CEndMissionTempTransitionView : public CBasicView
{
public:
	static CEndMissionTempTransitionView* New(TBool aMissionWon, TBool aGameSave, TInt lLivesLeft, TInt lScore, TInt lExpectedMissionTimeInMins, CStatsTracker* lStatsTracker);
	virtual ~CEndMissionTempTransitionView();

protected:
	CEndMissionTempTransitionView(TBool aMissionWon, TBool aGameSave, TInt lLivesLeft, TInt lScore, TInt lExpectedMissionTimeInMins, CStatsTracker* lStatsTracker);
	void Construct();
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//this  method should be overriden by the child view and should contain any code if any that should run before the Draw calls
	virtual void ChildRunLogicCode();

	CTextureObject* iBackgroundTexture;
	TBool iMissionWon;
	TBool iGameSave;
	TInt iLivesLeft;
	TInt iScore;
	TInt iExpectedMissionTimeInMins;
	CStatsTracker* iStatsTracker;
	TInt iFrame;
};

#endif /* CENDMISSIONTEMPTRANSITIONVIEW_H_ */
