/*
 * CSoundMixer.h
 *
 *  Created on: May 12, 2012
 *      Author: dstoll
 */

#ifndef CSoundMixer_H_
#define CSoundMixer_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/sound/CSoundPlayer.h"

//OpenSL
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_Platform.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#define SOUND_SHIFT_FACTOR 12 // not sure why it has to be 12
#define SAMPLING_RATE_MULTIPLIER 1000 //all sample rates will be multiplied by this constant when setting up the OpenSL Audio source

/*
 * NOTE!!!!
 * You can change the sample rate of your audio when create the Mixer.
 * However, if you want to change the sample size(by default 16bit) you also need to change the code
 *
 */

class CSoundMixer
{
public:

	virtual ~CSoundMixer();
	static CSoundMixer* New(CSoundChannels& aClientAudioChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aChannelType, TInt aMixtureCallsPerSec);

	//playback still continues but without sound
	void DisableSound();
	//playback now with Sound
	void EnableSound();
	//Pauses the playback but does not delete resources, player should resume playback from the location it stopped, does nothing if it has not been started
	void PauseMixer();
	//Resumes playback from the location it has been paused, does nothing if it has not been started
	void ResumeMixer();
	//do everything necessary to start the mixer and audio playback
	void StartMixer();
	//stop the mixer gracefully
	void StopMixer();

protected:

	CSoundMixer(CSoundChannels& aClientAudioChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aChannelType, TInt aMixtureCallsPerSec); //buffer stream outs per second
	void Construct();
	//needs to be a friend to stream out stuff, has to be C style method.
	friend void MixAndStreamOut(CSoundMixer* aAudioMixer);

	TInt iVolume;

	TInt* iSoundTrackPosition;
	TInt* iSoundTrackEndPosition;
	TInt* iSoundTrackRepStart;
	TInt* iSoundTrackRepEnd;
	TInt* iSoundTrackFrequency;
	TBool* iSoundChannelIsInUse;
	TInt16** iSoundTrackData;
	TInt16* iStreamOutBuffer; //pointer to the buffer that will be streamed out to the audio device
	TInt* iMixBuffer; //all channel data will be mixed into this buffer
	//TInt8* iStreamOutBufferPointer; // points to the stream out buffer
	CSoundChannels& iClientSoundChannels; //pointer to the audio channels of the client
	TSoundSampleRate iSampleRate; //need to set the correct sample rate for audio device and a large enough audio buffers
	TSoundFileChannelType iChannelType;
	TInt iMixtureCallsPerSec; //the more often the mixture is called, the better the sound quality,but uses more CPU
	TInt iSoundBufferSize;
	TInt iSoundBufferSizeInBytes;
	TBool iMixerIsStarted;
	TBool iIsMuted;

	//OpenSL members
	SLEngineItf iEngineInterface; //interface to the engine
	SLObjectItf iOutputMixerObject;
	SLObjectItf iSoundPlayerObject;
	SLPlayItf iSoundPlayerInterface;
	SLBufferQueueItf iBufferQueueInterface;
	SLVolumeItf iVolumeInterface;
};

#endif /* CSoundMixer_H_ */
