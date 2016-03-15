/*
 * CSoundMixer.cpp
 *
 *  Created on: May 12, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/COpenSLEngine.h"
#include "includes/core/sound/CSoundMixer.h"
#include <stdlib.h>
#include <assert.h>

//forward declaration needed for local functions
void BufferCopied(SLAndroidSimpleBufferQueueItf lBq, void* lContext);

CSoundMixer::CSoundMixer(CSoundChannels& aClientSoundChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aChannelType, TInt aNumberOfTimesMixtureIsCalledPerSec)
		: iClientSoundChannels(aClientSoundChannels)
{
	iIsMuted = false;
	iSampleRate = aSampleRate;
	iMixtureCallsPerSec = aNumberOfTimesMixtureIsCalledPerSec;
	iChannelType = aChannelType;
}

CSoundMixer::~CSoundMixer()
{
	//first stop the mixer buffer
	StopMixer();

	//clean up memory
	delete[] iSoundTrackPosition;
	delete[] iSoundTrackEndPosition;
	delete[] iSoundTrackRepStart;
	delete[] iSoundTrackRepEnd;
	delete[] iSoundTrackFrequency;
	delete[] iSoundChannelIsInUse;
	delete[] iSoundTrackData;
	//delete buffers
	delete[] iStreamOutBuffer;
	delete[] iMixBuffer;
}

CSoundMixer* CSoundMixer::New(CSoundChannels& aClientSoundChannels, TSoundSampleRate aSampleRate, TSoundFileChannelType aChannelType,
		TInt aNumberOfTimesMixtureIsCalledPerSec)
{
	CSoundMixer* lSoundMixer = new CSoundMixer(aClientSoundChannels, aSampleRate, aChannelType, aNumberOfTimesMixtureIsCalledPerSec);
	lSoundMixer->Construct();
	return lSoundMixer;
}

void CSoundMixer::Construct()
{
	iSoundTrackPosition = new TInt[iClientSoundChannels.iNumberOfChannels];
	iSoundTrackEndPosition = new TInt[iClientSoundChannels.iNumberOfChannels];
	iSoundTrackRepStart = new TInt[iClientSoundChannels.iNumberOfChannels];
	iSoundTrackRepEnd = new TInt[iClientSoundChannels.iNumberOfChannels];
	iSoundTrackFrequency = new TInt[iClientSoundChannels.iNumberOfChannels];
	iSoundChannelIsInUse = new TBool[iClientSoundChannels.iNumberOfChannels];
	iSoundTrackData = new TInt16*[iClientSoundChannels.iNumberOfChannels];
	iVolume = iClientSoundChannels.iMainVolumePercentage;
	iMixerIsStarted = false;

	//initiliaze certain arrays to 0
	memset(iSoundChannelIsInUse, false, sizeof(TBool) * iClientSoundChannels.iNumberOfChannels); //since it works with bytes
	memset(iSoundTrackData, NULL, sizeof(TInt16*) * iClientSoundChannels.iNumberOfChannels); //since it works with bytes

	//create buffers
	iSoundBufferSize = static_cast<TInt>(iSampleRate) / iMixtureCallsPerSec;
	iStreamOutBuffer = new TInt16[iSoundBufferSize];
	iMixBuffer = new TInt[iSoundBufferSize];
	iSoundBufferSizeInBytes = iSoundBufferSize * 2; //since we use 16 bits
}

void CSoundMixer::DisableSound()
{
	iIsMuted = true;
	iVolume = 0;
}

void CSoundMixer::EnableSound()
{
	iIsMuted = false;
	iVolume = iClientSoundChannels.iMainVolumePercentage;
}

void CSoundMixer::PauseMixer()
{
	if (iMixerIsStarted)
	{
		(*iSoundPlayerInterface)->SetPlayState(iSoundPlayerInterface, SL_PLAYSTATE_PAUSED);
	}
}

void CSoundMixer::ResumeMixer()
{
	if (iMixerIsStarted)
	{
		(*iSoundPlayerInterface)->SetPlayState(iSoundPlayerInterface, SL_PLAYSTATE_PLAYING);
	}
}

void CSoundMixer::StartMixer()
{
	if (!iMixerIsStarted)
	{
		iMixerIsStarted = true;
		//initiliaze certain arrays to 0
		memset(iSoundChannelIsInUse, false, sizeof(TBool) * iClientSoundChannels.iNumberOfChannels); //since it works with bytes
		memset(iSoundTrackData, NULL, sizeof(TInt16*) * iClientSoundChannels.iNumberOfChannels); //since it works with bytes

		SLresult lResult;
		SLObjectItf lEngineObject = COpenSLEngine::GetOpenSLEngine();
		lResult = (*lEngineObject)->GetInterface(lEngineObject, SL_IID_ENGINE, &iEngineInterface);
		assert(SL_RESULT_SUCCESS == lResult);

		//create output mixer
		const SLInterfaceID lCapabilitiesNeeded[] = {  };
		const SLboolean lIsRequired[] = {  };
		lResult = (*iEngineInterface)->CreateOutputMix(iEngineInterface, &iOutputMixerObject, 0, lCapabilitiesNeeded, lIsRequired);
		assert(SL_RESULT_SUCCESS == lResult);
		lResult = (*iOutputMixerObject)->Realize(iOutputMixerObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == lResult);

		//create source
		SLDataLocator_AndroidSimpleBufferQueue lBufferQueue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, (SLuint32) 2 };
		SLuint32 lSpeakerSettup;
		if(iChannelType == ESoundFileChannelTypeMono)
		{
			lSpeakerSettup = SL_SPEAKER_FRONT_CENTER;
		}
		else
		{
			lSpeakerSettup = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		}

		SLDataFormat_PCM lSoundFormatPCM = { SL_DATAFORMAT_PCM, iChannelType, static_cast<SLuint32>(iSampleRate) * SAMPLING_RATE_MULTIPLIER,
																				 SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16, lSpeakerSettup, SL_BYTEORDER_LITTLEENDIAN };
		SLDataSource lSoundSource = { &lBufferQueue, &lSoundFormatPCM };
		//the sink(destination) will be the outputMixer
		SLDataLocator_OutputMix lDataLocator = { SL_DATALOCATOR_OUTPUTMIX, iOutputMixerObject };
		SLDataSink lSoundSink = { &lDataLocator, NULL }; //format field is ignored because we use an output mix

		//create the Sound player
		const SLInterfaceID lIdsNeeded[] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME };
		const SLboolean lRequired[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
		lResult = (*iEngineInterface)->CreateAudioPlayer(iEngineInterface, &iSoundPlayerObject, &lSoundSource, &lSoundSink, 2,
				lIdsNeeded, lRequired);
		assert(SL_RESULT_SUCCESS == lResult);
		lResult = (*iSoundPlayerObject)->Realize(iSoundPlayerObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == lResult);

		//get playback object and buffer queue, Volume interface for player
		lResult = (*iSoundPlayerObject)->GetInterface(iSoundPlayerObject, SL_IID_PLAY, &iSoundPlayerInterface);
		assert(SL_RESULT_SUCCESS == lResult);
		lResult = (*iSoundPlayerObject)->GetInterface(iSoundPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &iBufferQueueInterface);
		assert(SL_RESULT_SUCCESS == lResult);
		lResult = (*iSoundPlayerObject)->GetInterface(iSoundPlayerObject, SL_IID_VOLUME, &iVolumeInterface);
		assert(SL_RESULT_SUCCESS == lResult);

		//0 means max Volume, we control the volume during the mixing, thus we can just set the max volume here
		SLmillibel lMaxVolume = 0;
		(*iVolumeInterface)->GetMaxVolumeLevel(iVolumeInterface, &lMaxVolume);
		(*iVolumeInterface)->SetVolumeLevel(iVolumeInterface, (SLmillibel) lMaxVolume);

		//setup callback method
		lResult = (*iBufferQueueInterface)->RegisterCallback(iBufferQueueInterface, (slBufferQueueCallback) BufferCopied, this); //pass reference to this object
		assert(SL_RESULT_SUCCESS == lResult);
		//start playback
		lResult = (*iSoundPlayerInterface)->SetPlayState(iSoundPlayerInterface, SL_PLAYSTATE_PLAYING);
		assert(SL_RESULT_SUCCESS == lResult);

		//need to stream out the first buffer
		MixAndStreamOut(this);
	}
}

void CSoundMixer::StopMixer()
{
	//stop the playback of the mixer
	if (iMixerIsStarted)
	{
		iMixerIsStarted = false;

		//need to clean up
		if (iOutputMixerObject != NULL)
		{
			(*iOutputMixerObject)->Destroy(iOutputMixerObject);
			iOutputMixerObject = NULL;
		}
		if (iSoundPlayerObject != NULL)
		{
			(*iSoundPlayerObject)->Destroy(iSoundPlayerObject);
			iSoundPlayerObject = NULL;
			iSoundPlayerInterface = NULL;
			iBufferQueueInterface = NULL;
			iVolumeInterface = NULL;
		}
	}
}

// -------------------- C Style Methods Needed for OpenSL ----------------------//

//called every time when a buffer has been streamed out
void BufferCopied(SLAndroidSimpleBufferQueueItf lBq, void* lContext)
{
	//called when the last Sound buffer has been streamed out
	CSoundMixer* lSoundMixer = static_cast<CSoundMixer*>(lContext);
	MixAndStreamOut(lSoundMixer);
}

//Mixes all sound channels together and streams it into the Sound device
void MixAndStreamOut(CSoundMixer* aSoundMixer)
{
	if (aSoundMixer->iMixerIsStarted)
	{
		//---------------------- check if volume changed and update it ----------------------------//
		if (!aSoundMixer->iIsMuted && aSoundMixer->iVolume != aSoundMixer->iClientSoundChannels.iMainVolumePercentage)
		{
			//volume changed, update it
			aSoundMixer->iVolume = aSoundMixer->iClientSoundChannels.iMainVolumePercentage;
		}

		//--------------------------- Mix Sound from different channels ----------------------//

		//copy over values from Sound channels
		for (TInt lIndex = 0; lIndex < aSoundMixer->iClientSoundChannels.iNumberOfChannels; lIndex++)
		{
			if (aSoundMixer->iClientSoundChannels.iPlayStarted[lIndex] == true)
			{
				//avoid that data is copied again during the next cycle
				aSoundMixer->iClientSoundChannels.iPlayStarted[lIndex] = false;

				//copy values
				aSoundMixer->iSoundTrackData[lIndex] = aSoundMixer->iClientSoundChannels.iSoundTracks[lIndex].iData;
				aSoundMixer->iSoundTrackPosition[lIndex] = 0;
				aSoundMixer->iSoundTrackEndPosition[lIndex] = aSoundMixer->iClientSoundChannels.iSoundTracks[lIndex].iLength
						<< SOUND_SHIFT_FACTOR;
				aSoundMixer->iSoundTrackRepStart[lIndex] = aSoundMixer->iClientSoundChannels.iSoundTracks[lIndex].iRepStart
						<< SOUND_SHIFT_FACTOR;
				aSoundMixer->iSoundTrackRepEnd[lIndex] = aSoundMixer->iClientSoundChannels.iSoundTracks[lIndex].iRepEnd
						<< SOUND_SHIFT_FACTOR;
				aSoundMixer->iSoundTrackFrequency[lIndex] = aSoundMixer->iClientSoundChannels.iFrequency[lIndex];
				aSoundMixer->iSoundChannelIsInUse[lIndex] = aSoundMixer->iClientSoundChannels.iChannelIsInUse[lIndex];
			}
		}

		//clear buffer, needs to be done since channels are mixed by adding their values
		memset(aSoundMixer->iMixBuffer, 0, sizeof(TInt) * aSoundMixer->iSoundBufferSize); //since it works with bytes

		//mix active channels
		for (TInt lChannelIndex = 0; lChannelIndex < aSoundMixer->iClientSoundChannels.iNumberOfChannels; lChannelIndex++)
		{
			if (aSoundMixer->iSoundTrackData[lChannelIndex] != NULL && aSoundMixer->iSoundChannelIsInUse[lChannelIndex] == true)
			{
				TInt* lMixerBufferPointer = aSoundMixer->iMixBuffer;
				TInt* lMixerBufferEnd = lMixerBufferPointer + aSoundMixer->iSoundBufferSize;
				TInt16* lSoundDataPointer = aSoundMixer->iSoundTrackData[lChannelIndex];
				//these values each time this method gets called until the entire track has been played
				TInt lPositionInSoundTrack = aSoundMixer->iSoundTrackPosition[lChannelIndex];
				TInt lEndPositionInSoundTrack = aSoundMixer->iSoundTrackEndPosition[lChannelIndex];
				TInt lRepStartPositionInSoundTrack = aSoundMixer->iSoundTrackRepStart[lChannelIndex];
				TInt lRepEndPositionInSoundTrack = aSoundMixer->iSoundTrackRepEnd[lChannelIndex];
				TInt lPositionAddValue = (aSoundMixer->iSoundTrackFrequency[lChannelIndex] << SOUND_SHIFT_FACTOR)
						/ static_cast<TInt>(aSoundMixer->iSampleRate);
				TInt lChannelVolume = aSoundMixer->iClientSoundChannels.iChannelVolume[lChannelIndex]; //wanna use the current value, that way the client can easily change the value

				while (lMixerBufferPointer < lMixerBufferEnd)
				{
					TInt lSoundValue = (lSoundDataPointer[lPositionInSoundTrack >> SOUND_SHIFT_FACTOR] * lChannelVolume);
					lPositionInSoundTrack += lPositionAddValue;

					if (lPositionInSoundTrack >= lEndPositionInSoundTrack) // track is done playing
					{
						if (lRepEndPositionInSoundTrack == 0) //means, no repition
						{
							aSoundMixer->iSoundTrackData[lChannelIndex] = NULL;
							aSoundMixer->iSoundChannelIsInUse[lChannelIndex] = false;
							aSoundMixer->iClientSoundChannels.iChannelIsInUse[lChannelIndex] = false; //let the client know that this channel can be reused now
							break;
						}
						else
						{
							lPositionInSoundTrack = lRepStartPositionInSoundTrack; //start playing again from RepStart position
							lEndPositionInSoundTrack = lRepEndPositionInSoundTrack; //stop at RepEnd Position
						}
					}
					*lMixerBufferPointer += lSoundValue; //add channel value to the mixer
					lMixerBufferPointer++; //move to the next position in the buffer
				}
				//update position inside the track
				aSoundMixer->iSoundTrackPosition[lChannelIndex] = lPositionInSoundTrack;
				aSoundMixer->iSoundTrackEndPosition[lChannelIndex] = lEndPositionInSoundTrack;
			}
		}
		//done mixing channels

		//Convert the 32-bit buffer into a 16-bit output buffer
		TInt* l32BitBuffer = aSoundMixer->iMixBuffer;
		TInt* l32BitBufferEnd = l32BitBuffer + aSoundMixer->iSoundBufferSize;
		TInt16* l16BitBuffer = aSoundMixer->iStreamOutBuffer;
		while (l32BitBuffer < l32BitBufferEnd)
		{
			//32-bit mixer buffer content is multiplied by main volume percentage
			//shifts are in two phases to prevent overflow and maintain quality -> Multiply by volume here do to a potential overflow
			TInt lSoundValue = ((*l32BitBuffer++ >> 8) * aSoundMixer->iVolume) >> 8; //main volume caps volume of the channels, on 100 (full) the entire channel volume is used and so on..

			//Prevent sound from trashing on overboost volume
			if (lSoundValue < -0x7fff)
				lSoundValue = -0x7fff;
			if (lSoundValue > 0x7fff)
				lSoundValue = 0x7fff;

			//convert 32 bit container into a 16 bit one, lower 16 bits will be lost but they are not important
			*l16BitBuffer++ = (TInt16) lSoundValue;
		}

		//Streamout the Sound buffer
		(*aSoundMixer->iBufferQueueInterface)->Enqueue(aSoundMixer->iBufferQueueInterface, aSoundMixer->iStreamOutBuffer, aSoundMixer->iSoundBufferSizeInBytes);
	}
}

