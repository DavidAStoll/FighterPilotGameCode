/*
 * CMusicPlayer.h
 *
 *  Created on: May 22, 2012
 *      Author: dstoll
 */

#ifndef CMUSICPLAYER_H_
#define CMUSICPLAYER_H_

//for OpenSL
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_Platform.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#include "includes/core/standardLibrary/StandardLibrary.h"

class CMusicPlayer
{
public:
	//needs to know in what directory the GameSongs and UserSongs are located
	static CMusicPlayer* New(const char* aGameSongPath, const char* aUserSongPath);
	virtual ~CMusicPlayer();

	//return if the current Song is a UserSong or a GameSong
	TBool GetIsPlayingUserSong();
	//Get the name of the song playing
	const CString* GetNameOfSong();
	//returns the GameSong Path
	const CString* GetGameSongPath();
	//returns the UserSong Path
	const CString* GetUserSongPath();
	//return volume of music player
	TUint GetVolume();
	//Pauses playback, song position is not reset
	void Pause();
	//Plays a song from file that is part of the game
	void PlayGameSong(const char* aGameSongName, TBool aLooped);
	//Plays a song from file that is from the user
	void PlayUserSong(const char* aUserSongName, TBool aLooped);
	//resumes playback
	void Resume();
	//set the volume of the music player, can be set even if no song is playing
	//should be between 0 and 100
	void SetVolume(TUint aVolume);
	//playback continues, but music is muted or not
	void SetMute(TBool aValue);
	//stops playback and resets position, need to call Play to start playback again
	void Stop();

protected:

	CMusicPlayer();
	void Construct(const char* aGameSongPath, const char* aUserSongPath);

	TInt iVolume;
	TBool iIsMuted;
	TBool iIsStarted;
	TBool iIsPlayingUserSong;
	CString* iSongName;
	CString* iGameSongPath;
	CString* iUserSongPath;
	//OpenSL members
	SLEngineItf iEngineInterface; //interface to the engine
	SLObjectItf iOutputMixerObject;
	SLObjectItf iAudioPlayerObject;
	SLPlayItf iAudioPlayerInterface;
	SLSeekItf iSeekInterface;
	SLVolumeItf iVolumeInterface;
};

#endif /* CMUSICPLAYER_H_ */
