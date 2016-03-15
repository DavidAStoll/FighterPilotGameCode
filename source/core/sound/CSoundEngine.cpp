/*
 * CSoundEngine.cpp
 *
 *  Created on: May 17, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CSoundEngine.h"
#include "includes/core/sound/CSoundFilePreferredVolume.h"
#include "includes/core/sound/CSoundFileNameMapping.h"
#include "includes/core/sound/CWavLoader.h"
#include "includes/core/CGame.h"

CSoundEngine::CSoundEngine(SSoundEngineArgList& aArgs)
{
	iFreeSoundChannelIndex = 0;
	iNumberOfChannels = aArgs.NumberOfChannels;
	iFileFrequency = aArgs.FileFrequency;
}

CSoundEngine::~CSoundEngine()
{
	if(iSoundPlayer)
	{
		delete iSoundPlayer;
		iSoundPlayer = NULL;
	}
	//need to delete Audio Track data
	for(TInt lIndex = 0; lIndex < ESoundFileId_LAST_INDEX; lIndex++)
	{
		delete iGameSoundTracks[lIndex].SoundTrack.iData;
		iGameSoundTracks[lIndex].SoundTrack.iData = NULL;

		CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForSoundEffects(-CGame::Game->iMemoryUsageTracker->iKBytesUsedInRAMForSoundEffects * 1000);
	}
}

CSoundEngine* CSoundEngine::New(SSoundEngineArgList& aArgs)
{
	CSoundEngine* lSoundEngine = new CSoundEngine(aArgs);
	lSoundEngine->Construct(aArgs);
	return lSoundEngine;
}

void CSoundEngine::Construct(SSoundEngineArgList& aArgs)
{
	iSoundEffectPath = new CString(aArgs.SoundEffectPath);
	//initilize Audio tracks
	CSoundFileNameMapping* lNameMapping = new CSoundFileNameMapping();
	CSoundFilePreferredVolume* lPreferredVolume = new CSoundFilePreferredVolume();
	CWavLoader* lWavLoader = new CWavLoader();

	for(TInt lIndex = 0; lIndex < ESoundFileId_LAST_INDEX; lIndex++)
	{
		//load audio clip into memory
		CString* lSoundWithPath = (new CString(iSoundEffectPath->GetData()))->Append(lNameMapping->iSoundMappingArray[lIndex]);
		iGameSoundTracks[lIndex].SoundTrack = lWavLoader->Load16BitSkipHeader(lSoundWithPath->GetData());
		//get preferred audio level for sound
		iGameSoundTracks[lIndex].PreferredVolume = lPreferredVolume->iPreferredVolume[lIndex];

		//since it is a new CString, we need to delete it as well
		delete lSoundWithPath;
	}

	//audio tracks have been loaded
	//clean up
	delete lNameMapping;
	delete lPreferredVolume;
	delete lWavLoader;

	//create the Audio Player
	iSoundPlayer =  CSoundPlayer::New(aArgs.NumberOfChannels, aArgs.SampleRate, aArgs.FileChannelType, aArgs.AudioMixtureCallsPerSec);
}

TInt CSoundEngine::GetPreferredSoundTrackVolume(TSoundFileId aSoundFileId)
{
	return iGameSoundTracks[aSoundFileId].PreferredVolume;
}

TInt CSoundEngine::Play(TSoundFileId aSoundFileId)
{
	return Play(aSoundFileId, iGameSoundTracks[aSoundFileId].PreferredVolume);
}

TInt CSoundEngine::Play(TSoundFileId aSoundFileId, TInt aPreferredVolume)
{
	if(FindFreeSoundEffectChannel())
	{
		iSoundPlayer->Play(iGameSoundTracks[aSoundFileId].SoundTrack, iFreeSoundChannelIndex, iFileFrequency, aPreferredVolume);
		return iFreeSoundChannelIndex;
	}
	else
	{
		return -1; // signals that no available for playback
	}
}

TInt CSoundEngine::PlayRepeat(TSoundFileId aSoundFileId)
{
	return PlayRepeat(aSoundFileId, iGameSoundTracks[aSoundFileId].PreferredVolume);
}

TInt CSoundEngine::PlayRepeat(TSoundFileId aSoundFileId, TInt aPreferredVolume)
{
	if(FindFreeSoundEffectChannel())
	{
		//create temp track to avoid to modify the original
		TSoundTrack lTempSoundTrack = iGameSoundTracks[aSoundFileId].SoundTrack;
		lTempSoundTrack.iRepStart = 0;
		lTempSoundTrack.iRepEnd = iGameSoundTracks[aSoundFileId].SoundTrack.iLength;

		iSoundPlayer->Play(lTempSoundTrack, iFreeSoundChannelIndex, iFileFrequency, aPreferredVolume);
		return iFreeSoundChannelIndex;
	}
	else
	{
		return -1; // signals that no available for playback
	}
}

void CSoundEngine::StopChannel(TInt aChannelIndex)
{
	iSoundPlayer->Stop(aChannelIndex);
}

TBool CSoundEngine::FindFreeSoundEffectChannel()
{
	TInt lNumberOfChannelsChecked = 0; //avoid deadlock
	while(iSoundPlayer->IsAudioChannelIsInUse(iFreeSoundChannelIndex))
	{
		iFreeSoundChannelIndex++;
		if(iFreeSoundChannelIndex >= iNumberOfChannels)
		{
			iFreeSoundChannelIndex = 0; //start at the beginning again
		}

		lNumberOfChannelsChecked++;
		if(lNumberOfChannelsChecked >= iNumberOfChannels) //checked all available channels
		{
			return false;
		}
	}

	return true; //found a good channel
}



