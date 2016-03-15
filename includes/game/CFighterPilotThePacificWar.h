/*
 * CFighterPilotThePacificWar.h
 *
 *  Created on: May 8, 2012
 *      Author: dstoll
 */

#ifndef CFIGHTERPILOTTHEPACIFICWAR_H_
#define CFIGHTERPILOTTHEPACIFICWAR_H_

#include "includes/core/CGame.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/backend/CGameData.h"

class CGameData;


class CFighterPilotThePacificWar : public CGame
{

public:

	static CFighterPilotThePacificWar* FighterGame;
	CSaveGameObject* iSaveGameObject;
	CGameVariables* iGameVariables;
	CGameData* iGameData;

protected:

	CFighterPilotThePacificWar();
	virtual ~CFighterPilotThePacificWar();

	//this method will be called just before the game exits
	virtual void CallBeforeExitGame();
	//this method will be called before the MainView method is called
	virtual void CallBeforeMainViewIsCreated();
	//create main view, the first view that will start displaying stuff
	virtual void CreateMainView();
	//put here the code to create the Graphics engine of your choice
	virtual void InitializeGraphicsEngine();
	//Loads all the texture for the frontend
	virtual void LoadTextuesForFrontend();
	//Loads all the sound effects
	virtual void LoadSoundEffects();
	//put code here to write down any persistent data for the game
	virtual void SavePersistentData();
	//initiliaze localization and put load all localization files
	virtual void SetupLocalization();
	//initialize the music player and SFX stream
	virtual void SetupMusicAndSfxPlayer();

};

#endif /* CFIGHTERPILOTTHEPACIFICWAR_H_ */
