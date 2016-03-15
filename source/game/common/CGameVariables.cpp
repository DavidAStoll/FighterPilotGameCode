/*
 * CGameVariables.cpp
 *
 *  Created on: Jul 26, 2012
 *      Author: dstoll
 */

#include "includes/game/common/CGameVariables.h"
#include "includes/core/utility/CFileStream.h"

CGameVariables::CGameVariables()
{
	// TODO Auto-generated constructor stub

}

CGameVariables::~CGameVariables()
{
	// TODO Auto-generated destructor stub
}

CGameVariables* CGameVariables::New()
{
	CGameVariables* lSelf = new CGameVariables();
	lSelf->Construct();
	return lSelf;
}

void CGameVariables::Construct()
{
	iDifficulty = EGameDifficultyEasy;
	iMusicPercentage = GDEFAULT_MUSIC_VOLUME;
	iSoundEffectsPercentage = GDEFAULT_SOUND_EFFECT_VOLUME;
	iUseIconButton = false;
	iAgreementAccepted = false;
	iFirstTimeStartingUp = true;
	iLanguage = ELanguageEnglish;
	iStringPlayerName = new CString("Player");
	iLastSaveGame = EGameMapNone;
	iDisplayBackgroundImageInPauseMenu = true;
	iFirstTimeGettingHighscore = true;


	//for Frontend Music
	iFrontendSong = new CString(SONG_MENU_LOOP);
	iIsUserSong = false;
}

//--------------------------- functions ----------------------------//


void CGameVariables::LoadSettingsFromDisk()
{
	CNativeFile* lSaveFile = new CNativeFile(GAME_VARIABLES_SETTINGS_FILE);
	CFileReadStream* lReadStream = lSaveFile->OpenForReading();

	if(lReadStream != NULL)
	{
		if(lReadStream->ReadInt32() == GAME_VARIABLES_SETTINGS_VERSION)
		{
			iDifficulty = static_cast<TGameDifficulty>(lReadStream->ReadInt32());
			iMusicPercentage = lReadStream->ReadInt32();
			iSoundEffectsPercentage = lReadStream->ReadInt32();
			iUseIconButton = lReadStream->ReadInt32();
			iAgreementAccepted = lReadStream->ReadInt32();
			iLanguage = static_cast<TLanguage>(lReadStream->ReadInt32());
			iLastSaveGame = static_cast<TGameMap>(lReadStream->ReadInt32());
			iDisplayBackgroundImageInPauseMenu = lReadStream->ReadInt32();
			iFirstTimeGettingHighscore = lReadStream->ReadInt32();
			lReadStream->ReadStringEncrypted(iStringPlayerName, "Player");
			iFirstTimeStartingUp = false; //must mean that this is not the first time the user played the game
		}

		lReadStream->CloseReadStream();
		delete lReadStream;
	}

	delete lSaveFile;
}

void CGameVariables::SaveSettingsToDisk()
{
	CNativeFile* lSaveFile = new CNativeFile(GAME_VARIABLES_SETTINGS_FILE);
	CFileWriteStream* lWriteStream = lSaveFile->OpenForWriting(EFileWriteStreamModeReplace);

	//write version
	lWriteStream->WriteInt32(GAME_VARIABLES_SETTINGS_VERSION);
	//write Settings
	lWriteStream->WriteInt32(iDifficulty);
	lWriteStream->WriteInt32(iMusicPercentage);
	lWriteStream->WriteInt32(iSoundEffectsPercentage);
	lWriteStream->WriteInt32(iUseIconButton);
	lWriteStream->WriteInt32(iAgreementAccepted);
	lWriteStream->WriteInt32(iLanguage);
	lWriteStream->WriteInt32(iLastSaveGame);
	lWriteStream->WriteInt32(iDisplayBackgroundImageInPauseMenu);
	lWriteStream->WriteInt32(iFirstTimeGettingHighscore);
	lWriteStream->WriteStringEncrypted(iStringPlayerName);

	lWriteStream->CloseWriteStream();
	delete lWriteStream;
	delete lSaveFile;
}

