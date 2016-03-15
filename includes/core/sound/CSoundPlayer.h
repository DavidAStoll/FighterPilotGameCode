/*
 * CSoundPlayer.h
 *
 *  Created on: May 11, 2012
 *      Author: dstoll
 */

#ifndef CSoundPlayer_H_
#define CSoundPlayer_H_

#include "includes/core/standardLibrary/BasicTypes.h"

enum TSoundFileChannelType
{
	ESoundFileChannelTypeMono = 1,
	ESoundFileChannelTypeStereo = 2
};

enum TSoundSampleRate
{
	ESoundSampleRate8000Hz  = 8000,
	ESoundSampleRate16000Hz = 16000,
};

class CSoundMixer;

class TSoundTrack
{
public:

	inline TSoundTrack()
	: iData(NULL)
	{}

  inline TSoundTrack( TInt16* aData, TInt aLength )
	: iData( aData )
	, iLength( aLength )
	, iRepStart( 0 )
	, iRepEnd( 0 )
	{}

  inline TSoundTrack( TInt16* aData, TInt aLength, TInt aRepStart, TInt aRepEnd )
	: iData( aData )
	, iLength( aLength )
	, iRepStart( aRepStart )
	, iRepEnd( aRepEnd )
	{}

	TInt16* iData;
	TInt iLength;
	TInt iRepStart;
	TInt iRepEnd;
};

class CSoundChannels
{
public:

	CSoundChannels(TInt aNumberOfChannels);
	~CSoundChannels();

	//public members for efficiency reasons
	TSoundTrack* iSoundTracks;
	TInt* iChannelVolume;
	TInt* iFrequency;
	TBool* iPlayStarted;
	TBool* iChannelIsInUse;
	TInt iMainVolumePercentage;
	TInt iNumberOfChannels;
};

//this class is responsible for talking the to the mixer and updating what sound tracks are being played
class CSoundPlayer
{
public:

	virtual ~CSoundPlayer();
	static CSoundPlayer* New(TInt aNumberOfChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aAudioFileChannelType, TInt aMixtureCallsPerSec);

	//turn the sound off
	void DisableSound();
	//turn sound back on
	void EnableSound();
	//Get Reference to audio channels of audio player
	TBool IsAudioChannelIsInUse(TInt aChannel);
	//Get the current main volume for playback of the audio channels
	TInt GetVolume();
	//return number of channels in this audio player
	TInt GetNumberOfChannels();
	//Pause the playback of all tracks
	void Pause();
	//Play a audio track in a certain Channel
	void Play(const TSoundTrack& aTrack, TInt aChannel, TInt aFrequency, TInt aVolume);
	//Resume the playback of all tracks
	void Resume();
	//sets the playback volume of all Audio channels
	void SetVolume(TInt aVolumePercentage); //0 to 100 are acceptable values
	//Sets the playback volume for the channel
	void SetVolumeForChannel(TInt aVolume, TInt aChannelIndex);
	//Stop playback of a audio track in a certain channel
	void Stop(TInt aChannel);


protected:

	CSoundPlayer(TInt aNumberOfChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aAudioFileChannelType, TInt aMixtureCallsPerSec);//buffer stream outs per second
	void Construct();

	TBool iPaused;
	TInt iMixtureCallsPerSec;
	CSoundChannels* iSoundChannels;
	TSoundTrack iEmptyTrack;
	TSoundSampleRate iSampleRate;
	TSoundFileChannelType iAudioFileChannelType;
	CSoundMixer* iSoundMixer;
};

#endif /* CSoundPlayer_H_ */
