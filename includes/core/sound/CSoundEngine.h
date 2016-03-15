/*
 * CSoundEngine.h
 *
 *  Created on: May 17, 2012
 *      Author: dstoll
 */

#ifndef CSOUNDENGINE_H_
#define CSOUNDENGINE_H_

#include "includes/core/sound/CSoundFileNameMapping.h"
#include "includes/core/sound/CSoundPlayer.h"

struct SSoundEngineArgList
{
	TInt NumberOfChannels;
	const char* SoundEffectPath;
	TSoundSampleRate SampleRate;
	TSoundFileChannelType FileChannelType;
	TUint FileFrequency;
	TUint AudioMixtureCallsPerSec; //more calls more quality, but uses more CPU
};

struct SGameAudioTrack
{
	TSoundTrack SoundTrack;
	TInt PreferredVolume;
};

class CSoundEngine
{
public:
	virtual ~CSoundEngine();
	static CSoundEngine* New(SSoundEngineArgList& aArgs);

	//returns the default volume for SoundFile
	TInt GetPreferredSoundTrackVolume(TSoundFileId aSoundFileId);
	//plays the specific sound in the next available channels, returns what channel it is playing in, return -1 if no channel is available
	TInt Play(TSoundFileId aSoundFileId);
	TInt Play(TSoundFileId aSoundFileId, TInt aPreferredVolume);
	//plays the specific sound in the next available channels, loops the playback until StopChannel is called on the channel
	TInt PlayRepeat(TSoundFileId aSoundFileId);
	TInt PlayRepeat(TSoundFileId aSoundFileId, TInt aPreferredVolume);
	//stops any playback of the channel, if not used it has no effect
	void StopChannel(TInt aChannelIndex);
	CSoundPlayer* iSoundPlayer;

protected:
	CSoundEngine(SSoundEngineArgList& aArgs);
	void Construct(SSoundEngineArgList& aArgs);
	TBool FindFreeSoundEffectChannel(); //increments iFreeSoundChannelIndex until it detects an used Channel

	TInt iNumberOfChannels;
	TInt iFreeSoundChannelIndex;
	TUint iFileFrequency;
	CString* iSoundEffectPath;
	SGameAudioTrack iGameSoundTracks [ESoundFileId_LAST_INDEX];
};

#endif /* CSOUNDENGINE_H_ */
