/*
 * CGameView.h
 *
 *  Created on: Aug 22, 2012
 *      Author: dstoll
 */

#ifndef CGAMEVIEW_H_
#define CGAMEVIEW_H_


#include "includes/core/views/CBasicView.h"
#include "includes/game/backend/CTouchControls.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CGameVariables.h"

class CGameView : public CBasicView
{
public:
	~CGameView();
	static CGameView* New(CGameData& aGameData);

protected:
	CGameView(CGameData& aGameData);
	void Construct();
	//checks if any new User Input is available
	void CheckForUserInput();
	//helper function for CheckForUserInput()
	TBool CheckIfUserActionIsTriggered(TInt aFrameSinceControlTouched);
	//will pass them on to the touch Controls
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	//contains all the logic code that runs in the game
	virtual void ChildRunLogicCode();
	//draws all game objects in the view volume and the Hud
	virtual void ChildViewDrawMethod();
	//this method is called when the context is gained, in this case we want pause all Audio and go to PauseView
	virtual void ContextGained();
	//this method is called the context is lost,need to stop audio and switch to the PauseView
	virtual void ContextLost();
	//makes the proper OpenGl calls to center the OpenGL coordinates
	void FixViewOnCenter();
	//unfixes the view back to its original player position
	void UnfixView();

	CTouchControls* iTouchControls;
	CGameData& iGameData;
	TBool iPlayerShouldRespawn;
	GLfloat iProjectionMatrix[16];

};

#endif /* CGAMEVIEW_H_ */
