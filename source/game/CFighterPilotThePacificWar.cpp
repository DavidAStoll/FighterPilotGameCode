/*
 * CFighterPilotThePacificWar.cpp
 *
 *  Created on: May 8, 2012
 *      Author: dstoll
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/views/CCompanyImageView.h"
#include "includes/game/common/CUserStats.h"

CFighterPilotThePacificWar* CFighterPilotThePacificWar::FighterGame = NULL;

CFighterPilotThePacificWar::CFighterPilotThePacificWar()
:CGame()
{
	CFighterPilotThePacificWar::FighterGame = this;
}

CFighterPilotThePacificWar::~CFighterPilotThePacificWar()
{
	// TODO Auto-generated destructor stub
}

void CFighterPilotThePacificWar::CallBeforeExitGame()
{
	CUserStats::GameFinishedTime();

	//sends stats to server if possible
	iUserStatsTrackerDaemon->SetConnectionTimeout(3); //otherwise shutdown takes too long if server is down
	iUserStatsTrackerDaemon->SendUserStatsIfPossible();
}

void CFighterPilotThePacificWar::CallBeforeMainViewIsCreated()
{
	iSaveGameObject = CSaveGameObject::New();
	iGameVariables = CGameVariables::New();
	iGameData = CGameData::New();

	//load Game Progress
	iSaveGameObject->LoadFromDisk();
	//load Settings from Disk
	iGameVariables->LoadSettingsFromDisk();

	//change environment depending on user settings
	iLocalizationEngine->SetLanguage(iGameVariables->iLanguage);
	iUserStatsTrackerDaemon->SetAgreedToUploadStats(iGameVariables->iAgreementAccepted);

}

//create main view, the first view that will start displaying stuff
void CFighterPilotThePacificWar::CreateMainView()
{
	iView = CCompanyImageView::New();
}

//creates the graphics engine
void CFighterPilotThePacificWar::InitializeGraphicsEngine()
{
	iGraphicsEngine = CGraphicsEngine::CreateGraphicsEngineToWindow(EGraphicsEngineDisplayDepth32, EGraphicsEngineTexture4Bytes8BitEach, 1024, 660);
}

//Loads all the texture for the frontend
void CFighterPilotThePacificWar::LoadTextuesForFrontend()
{

}

//Loads all the sound effects
void CFighterPilotThePacificWar::LoadSoundEffects()
{

}

void CFighterPilotThePacificWar::SavePersistentData()
{
	iSaveGameObject->SaveToDisk();
	iGameVariables->SaveSettingsToDisk();
}

 void CFighterPilotThePacificWar::SetupLocalization()
 {
	 iLocalizationEngine = CLocalizationEngine::New("Localization/");
	 iLocalizationEngine->SetLanguage(ELanguageEnglish);

	 //load all localization files
	 iLocalizationEngine->AddUnlocalizedFile("Common");
	 iLocalizationEngine->AddLocalizationFile("Frontend");
 }

//initialize the music player and SFX stream
void CFighterPilotThePacificWar::SetupMusicAndSfxPlayer()
{
	iAudioEngine = CGameAudioEngine::New();
}

