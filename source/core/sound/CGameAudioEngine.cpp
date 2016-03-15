/*
 * CGameAudioEngine.cpp
 *
 *  Created on: May 28, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CGameAudioEngine.h"
#include "includes/core/utility/CFileStream.h"
#include "includes/core/utility/CSystem.h"

CGameAudioEngine::CGameAudioEngine()
{
	iSilentProfileIsActivated = false;
	iSoundEngine = NULL;
	iMusicPlayer = NULL;
	iObjectHitBoxes = NULL;
	iOriginalChannelVolume = NULL;
	iObjectCoordinates = NULL;
}

CGameAudioEngine::~CGameAudioEngine()
{
	if(iSoundEngine != NULL)
	{
	  iSoundEngine->iSoundPlayer->DisableSound();
		delete iSoundEngine;
		iSoundEngine = NULL;
	}
	if(iMusicPlayer != NULL)
	{
		iMusicPlayer->SetMute(true);
		delete iMusicPlayer;
		iMusicPlayer = NULL;
	}
	if(iObjectHitBoxes != NULL)
	{
		delete[] iObjectHitBoxes;
		iObjectHitBoxes = NULL;
	}
	if(iOriginalChannelVolume != NULL)
	{
		delete[] iOriginalChannelVolume;
		iOriginalChannelVolume = NULL;
	}
	if(iObjectCoordinates != NULL)
	{
		delete[] iObjectCoordinates;
		iObjectCoordinates = NULL;
	}
}

CGameAudioEngine* CGameAudioEngine::New()
{
	CGameAudioEngine* lSelf = new CGameAudioEngine();
	lSelf->Construct();
	return lSelf;
}

void* MusicThread(void* aArg)
{
	void** lActualArg = (void**) aArg;

	//create the Music Player
	CString* lGameSongPath = CDirectory::GetGameMusicDirectory();
	CString* lUserSongPath = CDirectory::GetUserMusicDirectory();

	void** lMusicObjectPointer = (void**) lActualArg [0];
	*lMusicObjectPointer = CMusicPlayer::New(lGameSongPath->GetData(), lUserSongPath->GetData());
	delete lGameSongPath;
	delete lUserSongPath;

	//unlock the Main Thread
	CNativeSemaphore* lMusicSem = (CNativeSemaphore*) lActualArg[1];
	lMusicSem->Increment();

	//wait on it indefinetly
	CNativeSemaphore* lStopDeadSem = new CNativeSemaphore(0);
	lStopDeadSem->Decrement();
	return NULL;
}

void* SoundThread(void* aArg)
{
	void** lActualArg = (void**) aArg;

	//create the SoundEngine
	CString* lSoundPath = CDirectory::GetSoundDirectory();
	SSoundEngineArgList lSoundArg = {
																	GAMEAUDIOENGINE_NUMBER_OF_SOUND_EFFECTS_CHANNELS,
																	lSoundPath->GetData(),
																	GAMEAUDIOENGINE_SOUND_EFFECTS_SAMPLE_RATE,
																	GAMEAUDIOENGINE_SOUND_EFFECTS_CHANNEL_TYPE,
																	GAMEAUDIOENGINE_SOUND_EFFECTS_FILE_FREQUENCY,
																	GAMEAUDIOENGINE_SOUND_MIXER_UPDATES_PER_SEC
																	};

	void** lSoundObjectPointer = (void**) lActualArg [0];
	*lSoundObjectPointer = CSoundEngine::New(lSoundArg);
	delete lSoundPath;

	//unlock the Main Thread
	CNativeSemaphore* lSoundSem = (CNativeSemaphore*) lActualArg[1];
	lSoundSem->Increment();

	//wait on it indefinetly
	CNativeSemaphore* lStopDeadSem = new CNativeSemaphore(0);
	lStopDeadSem->Decrement();
	return NULL;
}

void CGameAudioEngine::Construct()
{
	void* lSoundEngineArgPointers [2];
	CNativeSemaphore* lSoundEngineCreatedSem = new CNativeSemaphore(0);
	lSoundEngineArgPointers [0] = (void**) &iSoundEngine;
	lSoundEngineArgPointers [1] = lSoundEngineCreatedSem;

	void* lMusicPlayerArgPointers [2];
	CNativeSemaphore* lMusicPlayerCreatedSem = new CNativeSemaphore(0);
	lMusicPlayerArgPointers [0] = (void**) &iMusicPlayer; //method will reset this pointer to the right location
	lMusicPlayerArgPointers [1] = lMusicPlayerCreatedSem; //this Semaphore will be used to signal if the tread has been created

	//create a new thread for the playback of the SoundEngine
	CNativeThread::CreateNativeThread(SoundThread, lSoundEngineArgPointers);
	//create a new thread for playback of the Music Player
	CNativeThread::CreateNativeThread(MusicThread, lMusicPlayerArgPointers);

	//wait for the Sound and Music Thread to complete ini
	lSoundEngineCreatedSem->Decrement();
	lMusicPlayerCreatedSem->Decrement();
	delete lSoundEngineCreatedSem; //not needed anymore
	delete lMusicPlayerCreatedSem; //not neeeded anymore

	//allocate Memory
	TInt lNumberOfSoundChannels = iSoundEngine->iSoundPlayer->GetNumberOfChannels();
	iObjectHitBoxes = new CHitBox* [lNumberOfSoundChannels];
	iOriginalChannelVolume = new TInt [lNumberOfSoundChannels];
	iObjectCoordinates = new TPointIntFloat[lNumberOfSoundChannels];

	//intilize data
	for(TInt lIndex = 0; lIndex < lNumberOfSoundChannels; lIndex++)
	{
		iObjectHitBoxes[lIndex] = NULL;
		iOriginalChannelVolume[lIndex] = 0;
		TPointIntFloat lDefaultLocation;  lDefaultLocation.iX = TIntFloat::Convert(0); lDefaultLocation.iY = TIntFloat::Convert(0);
		iObjectCoordinates[lIndex] = lDefaultLocation;
	}

	//check if we have mute everything because the player uses a Silent Profile
	CheckPlayerProfileIfAudioHasToBeMuted();
}

//--------------------------------- functions --------------------------------------//

void CGameAudioEngine::CheckPlayerProfileIfAudioHasToBeMuted()
{
	TBool lProfileIsSilent = CSystem::GetIsSilentModeActivated();
	if(lProfileIsSilent == iSilentProfileIsActivated)
	{
		return; //nothing has changed and nothing needs to be done
	}
	else
	{
		iSilentProfileIsActivated = lProfileIsSilent;
		if(iSilentProfileIsActivated)
		{
			//profile is silent, turn audio off
			TurnAudioOff();
		}
		else
		{
			//profile is not silent anymore, turn audio on again
			TurnAudioOn();
		}
	}
}

CMusicPlayer& CGameAudioEngine::GetMusicPlayer()
{
	return *iMusicPlayer;
}

CSoundEngine& CGameAudioEngine::GetSoundEngine()
{
	return *iSoundEngine;
}

TBool CGameAudioEngine::IsSoundChannelInUse(TInt aSoundChannelIndex)
{
	return iSoundEngine->iSoundPlayer->IsAudioChannelIsInUse(aSoundChannelIndex);
}

TInt CGameAudioEngine::Play(TSoundFileId lSoundIndex)
{
	return iSoundEngine->Play(lSoundIndex);
}

//method that plays sound with a specific volume
TInt CGameAudioEngine::Play(TSoundFileId lSoundIndex, TInt lPreferredVolume)
{
	return iSoundEngine->Play(lSoundIndex, lPreferredVolume);
}

//method that checks if the soudn is within player view using hit box
//if it is in play view play otherwise set the volume to 0
TInt CGameAudioEngine::PlayCheckBox(TSoundFileId aSoundFile,CHitBox *aHitBox)
{
	TInt lPreferredSoundVolume = iSoundEngine->GetPreferredSoundTrackVolume(aSoundFile);
	TInt lChannelIndex = iSoundEngine->Play(aSoundFile,GetRelativeSoundAtLocation(aHitBox,lPreferredSoundVolume));

	if(lChannelIndex != -1) //we have an open Channel
	{
		iObjectHitBoxes[lChannelIndex] = aHitBox;//keep track of where the object is moving
		iOriginalChannelVolume[lChannelIndex] = lPreferredSoundVolume;
	}
	return lChannelIndex;
}

//method that checks if the sound is within player view using point checking
TInt CGameAudioEngine::PlayCheckPoint(TSoundFileId aSoundFile,const TPointIntFloat& aCoordinate)
{
	TInt lChannelIndex = -1;
	TInt lPreferredSoundVolume = iSoundEngine->GetPreferredSoundTrackVolume(aSoundFile);
	lChannelIndex = iSoundEngine->Play(aSoundFile,GetRelativeSoundAtLocation(aCoordinate,lPreferredSoundVolume));

	if(lChannelIndex != -1)//we have an open Channel
	{
		iObjectHitBoxes[lChannelIndex] = NULL;
		iObjectCoordinates[lChannelIndex] = aCoordinate;//keep track of where the object is moving
		iOriginalChannelVolume[lChannelIndex] = lPreferredSoundVolume;
	}
	return lChannelIndex;
}

TInt CGameAudioEngine::PlayCheckPoint(TSoundFileId aSoundFile,CHitBox *aHitBox)
{
	TPointIntFloat lHitBoxMiddlePoint = CMath::GetCenterOfRect(aHitBox->GetFastIntersectionRect());
	return PlayCheckPoint(aSoundFile, lHitBoxMiddlePoint);
}

TInt CGameAudioEngine::PlayRepeatCheckBox(TSoundFileId aSoundFile,CHitBox *aHitBox)
{
	TInt lPreferredSoundVolume = iSoundEngine->GetPreferredSoundTrackVolume(aSoundFile);
	TInt lChannelIndex = iSoundEngine->PlayRepeat(aSoundFile,GetRelativeSoundAtLocation(aHitBox,lPreferredSoundVolume));

	if(lChannelIndex != -1)
	{
		iObjectHitBoxes[lChannelIndex] = aHitBox;//keep track of where the object is moving
		iOriginalChannelVolume[lChannelIndex] = lPreferredSoundVolume;
	}
	return lChannelIndex;
}

TInt CGameAudioEngine::PlayRepeatCheckPoint(TSoundFileId aSoundFile,const TPointIntFloat& aCoordinate)
{
	TInt lPreferredSoundVolume = iSoundEngine->GetPreferredSoundTrackVolume(aSoundFile);
	TInt lChannelIndex = iSoundEngine->PlayRepeat(aSoundFile,GetRelativeSoundAtLocation(aCoordinate,lPreferredSoundVolume));

	if(lChannelIndex != -1)
	{
		iObjectHitBoxes[lChannelIndex] = NULL;
		iObjectCoordinates[lChannelIndex] = aCoordinate;//keep track of where the object is moving
		iOriginalChannelVolume[lChannelIndex] = lPreferredSoundVolume;
	}
	return lChannelIndex;
}

TInt CGameAudioEngine::PlayRepeat(TSoundFileId aSoundFile)
{
	return iSoundEngine->PlayRepeat(aSoundFile);
}

TInt CGameAudioEngine::PlayRepeat(TSoundFileId aSoundFile, TInt lPreferredVolume)
{
	return iSoundEngine->PlayRepeat(aSoundFile, lPreferredVolume);
}

void CGameAudioEngine::StopSound(TInt aChannelIndex)
{
	iSoundEngine->StopChannel(aChannelIndex);
	iObjectHitBoxes[aChannelIndex] = NULL; //since this HitBox is not in use anymore
}

void CGameAudioEngine::StopAllSoundChannels()
{
	TInt lNumberOfSoundChannels = iSoundEngine->iSoundPlayer->GetNumberOfChannels();
	for(TInt lCurrentIndex = 0; lCurrentIndex < lNumberOfSoundChannels; lCurrentIndex++)
	{
		StopSound(lCurrentIndex);
	}
}

TInt CGameAudioEngine::GetRelativeSoundAtLocation(CHitBox* aHitBox,TInt aNormalSound)
{
		return 0; //player has no game object right now
}

TInt CGameAudioEngine::GetRelativeSoundAtLocation(TPointIntFloat aPoint,TInt aNormalSound)
{
		return 0; //player has no game object right now, volume returned will be 0
}

//checks if the sound is still in player view
//if it is not in view stop it from playing
void CGameAudioEngine::UpdateAllSoundChannelVolumeRelativeToPlayerLoaction(const TRectIntFloat& aPlayerRect)
{
	TPointIntFloat lPlayerViewMiddlePoint = CMath::GetCenterOfRect(aPlayerRect);

	for(TInt lIndex = 0; lIndex < iSoundEngine->iSoundPlayer->GetNumberOfChannels(); lIndex++)
	{
		if(iSoundEngine->iSoundPlayer->IsAudioChannelIsInUse(lIndex))
		{
			if(iObjectHitBoxes[lIndex] != NULL)
			{
				TPointIntFloat lHitBoxMiddlePoint =  CMath::GetCenterOfRect(iObjectHitBoxes[lIndex]->GetFastIntersectionRect());
				TIntFloat lDistanceToPlayerView = TIntFloat::Convert(CMath::DistanceBetweenPoints(lPlayerViewMiddlePoint, lHitBoxMiddlePoint));
				lDistanceToPlayerView = lDistanceToPlayerView / GAMEAUDIOENGINE_DECREASE_VOLUME_OVER_DISTANCE_CONSTANT;

				if(lDistanceToPlayerView < 1)
					iSoundEngine->iSoundPlayer->SetVolumeForChannel(iOriginalChannelVolume[lIndex], lIndex);
				else
					iSoundEngine->iSoundPlayer->SetVolumeForChannel((TIntFloat::Convert(iOriginalChannelVolume[lIndex]) / (lDistanceToPlayerView * lDistanceToPlayerView)).GetIntInBaseInt(), lIndex);
			}
			else//checkPoint case
			{
				TIntFloat lDistanceToPlayerView = TIntFloat::Convert(CMath::DistanceBetweenPoints(lPlayerViewMiddlePoint, iObjectCoordinates[lIndex]));
				lDistanceToPlayerView = lDistanceToPlayerView / GAMEAUDIOENGINE_DECREASE_VOLUME_OVER_DISTANCE_CONSTANT;

				if(lDistanceToPlayerView < 1)
					iSoundEngine->iSoundPlayer->SetVolumeForChannel(iOriginalChannelVolume[lIndex], lIndex);
				else
					iSoundEngine->iSoundPlayer->SetVolumeForChannel((TIntFloat::Convert(iOriginalChannelVolume[lIndex]) / (lDistanceToPlayerView * lDistanceToPlayerView)).GetIntInBaseInt(), lIndex);
			}
		}
	}
}

//turns all Sound effects off
void CGameAudioEngine::TurnAudioOff()
{
	iSoundEngine->iSoundPlayer->DisableSound();
	iMusicPlayer->SetMute(true);
}

//turns all SoundEffctsOn
void CGameAudioEngine::TurnAudioOn()
{
	if(!iSilentProfileIsActivated)//only possible to enable sounds if profile is not silent
	{
		iSoundEngine->iSoundPlayer->EnableSound();
		iMusicPlayer->SetMute(false);
	}
}
