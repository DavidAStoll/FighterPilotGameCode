/*
 * CGameAudioEngine.h
 *
 *  Created on: May 28, 2012
 *      Author: dstoll
 */

#ifndef CGAMEAUDIOENGINE_H_
#define CGAMEAUDIOENGINE_H_

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/sound/CSoundEngine.h"
#include "includes/core/sound/CMusicPlayer.h"
#include "includes/core/collision/CHitBox.h"
#include "includes/core/utility/IPC.h"

#define GAMEAUDIOENGINE_NUMBER_OF_SOUND_EFFECTS_CHANNELS 500
#define GAMEAUDIOENGINE_SOUND_EFFECTS_SAMPLE_RATE ESoundSampleRate16000Hz
#define GAMEAUDIOENGINE_SOUND_EFFECTS_CHANNEL_TYPE ESoundFileChannelTypeMono
#define GAMEAUDIOENGINE_SOUND_EFFECTS_FILE_FREQUENCY 16000
#define GAMEAUDIOENGINE_SOUND_MIXER_UPDATES_PER_SEC 30
#define GAMEAUDIOENGINE_DECREASE_VOLUME_OVER_DISTANCE_CONSTANT 225 //the bigger, the less Sound decreases over distance

class CGameAudioEngine
{

public:

	~CGameAudioEngine();
	static CGameAudioEngine* New();

	//should be called from time to time to check if the current player profile forces us to turn the audio off
	void CheckPlayerProfileIfAudioHasToBeMuted();
	//get the Music player
	CMusicPlayer& GetMusicPlayer();
	//get the SoundEngine
	CSoundEngine& GetSoundEngine();
	//returns true if the SoundChannel is in Use
	TBool IsSoundChannelInUse(TInt aSoundChannelIndex);
	//Plays back the given SoundFile
	TInt Play(TSoundFileId lSoundIndex);
	TInt Play(TSoundFileId lSoundIndex, TInt lPreferredVolume);
	//Plays back the Sound file, using aHitBox to update its volume, it will keep a reference to the original pointer to know if the HitBox has been deleted
	TInt PlayCheckBox(TSoundFileId aSoundFile,CHitBox* aHitBox);
	//Plays the Sound file at a certain Location in the map, good for Sounds that don't move around
	TInt PlayCheckPoint(TSoundFileId aSoundFile,const TPointIntFloat& aCoordinate);
	//this method also plays back at a certain point in space without moving, but it calculates the point in space by using a reference to a HitBox
	TInt PlayCheckPoint(TSoundFileId aSoundFile,CHitBox *aHitBox);
	//plays back a SoundFile but in repeat mode
	TInt PlayRepeat(TSoundFileId TSoundFileId);
	TInt PlayRepeat(TSoundFileId TSoundFileId, TInt lPreferredVolume);
	//uses a HitBox to update the Sound Location and repeats playing the Sound
	TInt PlayRepeatCheckBox(TSoundFileId aSoundFile,CHitBox *aHitBox);
	//uses a Location in the map to playback the sound and keeps repeating the playback
	TInt PlayRepeatCheckPoint(TSoundFileId aSoundFile,const TPointIntFloat& aCoordinate);
	//has a known bug, could mess stuff up if it is used to interrupt a SoundObject repeat playing
	void StopSound(TInt aChannelIndex);
	//stops the playback of all current Sound Effects playing
	void StopAllSoundChannels();
	//turns the Sound and Music off
	void TurnAudioOff();
	//turns the Sound and Music On
	void TurnAudioOn();
	//should be called every frame by the game when to Update each SoundEffects Channel depending on the location to the player
	void UpdateAllSoundChannelVolumeRelativeToPlayerLoaction(const TRectIntFloat& aPlayerRect);

	//make public for now, try to make private later
	//size depends on how many SoundEffects Channels we have
	TPointIntFloat* iObjectCoordinates;
	CHitBox** iObjectHitBoxes;
	TInt* iOriginalChannelVolume;

private:

	CGameAudioEngine();
	void Construct();
	//checks what SoundVolume the Sound would be at the given location relative to the Player's Location
	TInt GetRelativeSoundAtLocation(CHitBox* aHitBox,TInt aNormalSound);
	TInt GetRelativeSoundAtLocation(TPointIntFloat aPoint,TInt aNormalSound);

	//The SounEngine is responsible for playing sound effects
	CSoundEngine* iSoundEngine;
	//The Music Player is used to play Music
	CMusicPlayer* iMusicPlayer;
	TBool iSilentProfileIsActivated;
};

#endif /* CGAMEAUDIOENGINE_H_ */
