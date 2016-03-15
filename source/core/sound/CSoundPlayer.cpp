/*
 * CSoundPlayer.cpp
 *
 *  Created on: May 11, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CSoundPlayer.h"
#include "includes/core/sound/CSoundMixer.h"
#include <stdlib.h>

CSoundPlayer::CSoundPlayer(TInt aNumberOfChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aSoundFileChannelType, TInt aMixtureCallsPerSec)
{
	iPaused = false;
	iSoundChannels = new CSoundChannels(aNumberOfChannels);
	iEmptyTrack = TSoundTrack();
	iSampleRate = aSampleRate;
	iAudioFileChannelType = aSoundFileChannelType;
	iMixtureCallsPerSec = aMixtureCallsPerSec;
}

CSoundPlayer::~CSoundPlayer()
{
	if(iSoundChannels != NULL)
	{
		delete iSoundChannels;
		iSoundChannels = NULL;
	}
	if(iSoundMixer != NULL)
	{
		delete iSoundMixer;
		iSoundMixer = NULL;
	}
}

CSoundPlayer* CSoundPlayer::New(TInt aNumberOfChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aSoundFileChannelType, TInt aMixtureCallsPerSec)
{
	CSoundPlayer* lAudioPlayer = new CSoundPlayer(aNumberOfChannels, aSampleRate, aSoundFileChannelType, aMixtureCallsPerSec);
	lAudioPlayer->Construct();
	return lAudioPlayer;
}

void CSoundPlayer::Construct()
{
	iSoundMixer = CSoundMixer::New(*iSoundChannels, iSampleRate, iAudioFileChannelType, iMixtureCallsPerSec);
	//need to make a specific call to start the mixer
	iSoundMixer->StartMixer();
	SetVolume(100);
}

void CSoundPlayer::DisableSound()
{
	iSoundMixer->DisableSound();
}

void CSoundPlayer::EnableSound()
{
	iSoundMixer->EnableSound();
}

TBool CSoundPlayer::IsAudioChannelIsInUse(TInt aChannel)
{
	return iSoundChannels->iChannelIsInUse[aChannel];
}

TInt CSoundPlayer::GetVolume()
{
	TInt lVolume = 0;
	if(iSoundChannels != NULL)
	{
		lVolume = iSoundChannels->iMainVolumePercentage;
	}
	return lVolume;
}

TInt CSoundPlayer::GetNumberOfChannels()
{
	return iSoundChannels->iNumberOfChannels;
}

void CSoundPlayer::Pause()
{
	if(iPaused)
	{
		return; //nothing to do
	}

	iPaused = true;
	//tell the mixer the pause playback
	iSoundMixer->PauseMixer();
}

void CSoundPlayer::Play(const TSoundTrack& aTrack, TInt aChannel, TInt aFrequency, TInt aVolume)
{
	//order is here important, avoids the use of mutexes
	iSoundChannels->iSoundTracks[aChannel] = aTrack;
	iSoundChannels->iFrequency[aChannel] = aFrequency;
	iSoundChannels->iChannelVolume[aChannel] = aVolume;
	iSoundChannels->iChannelIsInUse[aChannel] = true;
	iSoundChannels->iPlayStarted[aChannel] = true;
}

void CSoundPlayer::Resume()
{
	if(!iPaused)
	{
		return; //nothing to do
	}

	iPaused = false;
	//tell the mixer the resume playback
	iSoundMixer->ResumeMixer();
}

void CSoundPlayer::SetVolume(TInt aVolumePercentage)
{
	if(iSoundChannels != NULL)
	{
		iSoundChannels->iMainVolumePercentage = aVolumePercentage;
	}
}

void CSoundPlayer::SetVolumeForChannel(TInt aVolume, TInt aChannelIndex)
{
	if(iSoundChannels != NULL)
	{
		iSoundChannels->iChannelVolume[aChannelIndex] = aVolume;
	}
}

void CSoundPlayer::Stop(TInt aChannel)
{
	//order is here important, avoids the use of mutexes
	iSoundChannels->iChannelIsInUse[aChannel] = false;
	iSoundChannels->iSoundTracks[aChannel] = iEmptyTrack;
	iSoundChannels->iPlayStarted[aChannel] = true;
}


//------------------------------ Audio Channels ---------------------------------//

CSoundChannels::CSoundChannels(TInt aNumberOfChannels)
{
	//allocate memory
	iSoundTracks = new TSoundTrack [aNumberOfChannels];
	iChannelVolume = new TInt [aNumberOfChannels];
	iFrequency = new TInt [aNumberOfChannels];
	iPlayStarted = new TBool [aNumberOfChannels];
	iChannelIsInUse = new TBool [aNumberOfChannels];

	//set to default values for safety reasons
	memset(iChannelVolume, 1, sizeof(TInt) * aNumberOfChannels); //since it works with bytes
	memset(iFrequency, 1, sizeof(TInt) * aNumberOfChannels); //since it works with bytes
	memset(iPlayStarted, false, sizeof(TBool) * aNumberOfChannels); //since it works with bytes
	memset(iChannelIsInUse, false, sizeof(TBool) * aNumberOfChannels); //since it works with bytes

	//set to default values
	iMainVolumePercentage = 0;
	iNumberOfChannels = aNumberOfChannels;


}

CSoundChannels::~CSoundChannels()
{
	delete[] iSoundTracks;
	delete[] iChannelVolume;
	delete[] iFrequency;
	delete[] iPlayStarted;
	delete[] iChannelIsInUse;
}
