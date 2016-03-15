/*
 * CMusicPlayer.cpp
 *
 *  Created on: May 22, 2012
 *      Author: dstoll
 */


#include <assert.h>
#include "includes/core/sound/CMusicPlayer.h"
#include "includes/core/sound/COpenSLEngine.h"
#include "includes/core/utility/CFileStream.h"
#include "includes/GlobalDefines.h"

#if PLATFORM_ANDROID //need for workaround on Android
#include "includes/native/utility/CAndroidFileStream.h"
#include "includes/native/CAndroidFighterPilotThePacificWar.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

//special android function
SLDataLocator_AndroidFD AndroidGameMusicSourceCreator(const char* aAssetName);
#endif

CMusicPlayer::CMusicPlayer()
{
	iIsStarted = false;
	iIsMuted = false;
	iIsPlayingUserSong = false;
	iVolume = 0;
	iAudioPlayerObject = NULL;
	iSongName = NULL;
	iGameSongPath = NULL;
	iUserSongPath = NULL;
}

CMusicPlayer::~CMusicPlayer()
{
	Stop();
	if(iSongName != NULL)
	{
		delete iSongName;
		iSongName = NULL;
	}
	if(iGameSongPath != NULL)
	{
		delete iGameSongPath;
		iGameSongPath = NULL;
	}
	if(iUserSongPath != NULL)
	{
		delete iUserSongPath;
		iUserSongPath = NULL;
	}
	//clean up other OpenSL objects as well
	if (iOutputMixerObject != NULL)
	{
		(*iOutputMixerObject)->Destroy(iOutputMixerObject);
		iOutputMixerObject = NULL;
	}
}

CMusicPlayer* CMusicPlayer::New(const char* aGameSongPath, const char* aUserSongPath)
{
	CMusicPlayer* lMusicPlayer = new CMusicPlayer();
	lMusicPlayer->Construct(aGameSongPath, aUserSongPath);
	return lMusicPlayer;
}

void CMusicPlayer::Construct(const char* aGameSongPath, const char* aUserSongPath)
{
	iSongName = new CString("");
	iGameSongPath = new CString(aGameSongPath);
	iUserSongPath = new CString(aUserSongPath);

	//create User Song Directory
	CDirectory::CreateDirectory(aUserSongPath);

	//all these OpenSL objects will be reused, thus we need to create them only once
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
}

//----------------------------- functions ----------------------//

TBool CMusicPlayer::GetIsPlayingUserSong()
{
	return iIsPlayingUserSong;
}

const CString* CMusicPlayer::GetNameOfSong()
{
	return iSongName;
}

const CString* CMusicPlayer::GetGameSongPath()
{
	return iGameSongPath;
}

const CString* CMusicPlayer::GetUserSongPath()
{
	return iUserSongPath;
}

TUint CMusicPlayer::GetVolume()
{
	return iVolume;
}

void CMusicPlayer::Pause()
{
	if(iIsStarted)
	{
		(*iAudioPlayerInterface)->SetPlayState(iAudioPlayerInterface, SL_PLAYSTATE_PAUSED);
	}
}

void CMusicPlayer::PlayGameSong(const char* aGameSongName, TBool aLooped)
{
	if(iIsStarted)
		Stop();

	//find out the full path the Game Song
	CString* lSongWithPath = (new CString(iGameSongPath->GetData()))->Append(aGameSongName);

  // configure audio source
#if PLATFORM_ANDROID
	//---------------------- ON ANDROID we can't get the path storage directory -----------------//
	SLDataLocator_AndroidFD lURI = AndroidGameMusicSourceCreator(lSongWithPath->GetData());
#else
  SLDataLocator_URI lURI = {SL_DATALOCATOR_URI, (SLchar *) lSongWithPath->GetData()};
#endif
  SLDataFormat_MIME lFormatMIME = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
  SLDataSource lAudioSource = {&lURI, &lFormatMIME};

  // configure audio sink
  SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, iOutputMixerObject};
  SLDataSink lAudioSink = {&loc_outmix, NULL};

  //create the audio player object
	SLresult lResult;
  const SLInterfaceID Ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
  const SLboolean lReq[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
  lResult = (*iEngineInterface)->CreateAudioPlayer(iEngineInterface, &iAudioPlayerObject, &lAudioSource, &lAudioSink, 2, Ids, lReq);
  assert(SL_RESULT_SUCCESS == lResult);

  //Realize the player object, abort if it fails
  lResult = (*iAudioPlayerObject)->Realize(iAudioPlayerObject, SL_BOOLEAN_FALSE);
	 // this will always succeed on Android, but we check result for portability to other platforms
	 if (SL_RESULT_SUCCESS != lResult)
	 {
		 (*iAudioPlayerObject)->Destroy(iAudioPlayerObject);
		 iAudioPlayerObject = NULL;
		 return; //abort
	 }

	 //get interfaces
	 //playback interface
	 lResult = (*iAudioPlayerObject)->GetInterface(iAudioPlayerObject, SL_IID_PLAY, &iAudioPlayerInterface);
   assert(SL_RESULT_SUCCESS == lResult);
   // get the seek interface
   lResult = (*iAudioPlayerObject)->GetInterface(iAudioPlayerObject, SL_IID_SEEK, &iSeekInterface);
   assert(SL_RESULT_SUCCESS == lResult);
   // get the volume interface
   lResult = (*iAudioPlayerObject)->GetInterface(iAudioPlayerObject, SL_IID_VOLUME, &iVolumeInterface);
   assert(SL_RESULT_SUCCESS == lResult);

   //set looping
   if(aLooped)
   {
  	 lResult = (*iSeekInterface)->SetLoop(iSeekInterface, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
  	 assert(SL_RESULT_SUCCESS == lResult);
   }

   //do some house keeping
   delete lSongWithPath; //not needed anymore
   iSongName->Replace(aGameSongName);
   iIsStarted = true;
   iIsPlayingUserSong = false;
   SetMute(iIsMuted);
   SetVolume(iVolume);
   Resume();
}

void CMusicPlayer::PlayUserSong(const char* aGameSongName, TBool aLooped)
{
	if(iIsStarted)
		Stop();

	//find out the full path the Game Song
	CString* lSongWithPath = (new CString(iUserSongPath->GetData()))->Append(aGameSongName);

  // configure audio source
  SLDataLocator_URI lURI = {SL_DATALOCATOR_URI, (SLchar *) lSongWithPath->GetData()};
  SLDataFormat_MIME lFormatMIME = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
  SLDataSource lAudioSource = {&lURI, &lFormatMIME};

  // configure audio sink
  SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, iOutputMixerObject};
  SLDataSink lAudioSink = {&loc_outmix, NULL};

  //create the audio player object
	SLresult lResult;
  const SLInterfaceID Ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
  const SLboolean lReq[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
  lResult = (*iEngineInterface)->CreateAudioPlayer(iEngineInterface, &iAudioPlayerObject, &lAudioSource, &lAudioSink, 2, Ids, lReq);
  assert(SL_RESULT_SUCCESS == lResult);

  //Realize the player object, abort if it fails
  lResult = (*iAudioPlayerObject)->Realize(iAudioPlayerObject, SL_BOOLEAN_FALSE);
	 // this will always succeed on Android, but we check result for portability to other platforms
	 if (SL_RESULT_SUCCESS != lResult)
	 {
		 (*iAudioPlayerObject)->Destroy(iAudioPlayerObject);
		 iAudioPlayerObject = NULL;
		 return; //abort
	 }

	 //get interfaces
	 //playback interface
	 lResult = (*iAudioPlayerObject)->GetInterface(iAudioPlayerObject, SL_IID_PLAY, &iAudioPlayerInterface);
   assert(SL_RESULT_SUCCESS == lResult);
   // get the seek interface
   lResult = (*iAudioPlayerObject)->GetInterface(iAudioPlayerObject, SL_IID_SEEK, &iSeekInterface);
   assert(SL_RESULT_SUCCESS == lResult);
   // get the volume interface
   lResult = (*iAudioPlayerObject)->GetInterface(iAudioPlayerObject, SL_IID_VOLUME, &iVolumeInterface);
   assert(SL_RESULT_SUCCESS == lResult);

   //set looping
   if(aLooped)
   {
  	 lResult = (*iSeekInterface)->SetLoop(iSeekInterface, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
  	 assert(SL_RESULT_SUCCESS == lResult);
   }

   //do some house keeping
   delete lSongWithPath; //not needed anymore
   iSongName->Replace(aGameSongName);
   iIsStarted = true;
   iIsPlayingUserSong = true;
   SetMute(iIsMuted);
   SetVolume(iVolume);
   Resume();
}


void CMusicPlayer::Resume()
{
	if(iIsStarted)
	{
		(*iAudioPlayerInterface)->SetPlayState(iAudioPlayerInterface, SL_PLAYSTATE_PLAYING);
	}
}

void CMusicPlayer::SetVolume(TUint aVolume)
{
	iVolume = aVolume;
	TDouble lSignalPowerRation = (TDouble) ((TInt64) iVolume * iVolume * iVolume * iVolume * iVolume) / ( (TInt64) 100 * 100 * 100 * 100 * 100);

	if(lSignalPowerRation > 0) //log 0 or less is undefined
	{
		lSignalPowerRation = CMath::Log(lSignalPowerRation);
	}
	else
	{
		lSignalPowerRation = -10; //is the same as log 0.000 000 000 001, almost 0 percent of the audio output
	}

	if(iIsStarted && !iIsMuted)
	{
		SLmillibel lVolumeInMillibel = lSignalPowerRation * 1000; //convet to milliDb, 100 for milli, 10 for conversion formula
		(*iVolumeInterface)->SetVolumeLevel(iVolumeInterface, lVolumeInMillibel);
	}
}

void CMusicPlayer::SetMute(TBool aValue)
{
	iIsMuted = aValue;

	if(iIsStarted) //for some reason mutting didn't work
	{
		if(iIsMuted)
		{
			(*iVolumeInterface)->SetVolumeLevel(iVolumeInterface, -10000);
		}
		else
		{
			SetVolume(iVolume);
		}
	}
}

void CMusicPlayer::Stop()
{
	if(iIsStarted)
	{
		TBool lHasBeenMuted = iIsMuted;
		iIsStarted = false;
		SetMute(true); //in case some weird sounds are played when the playback stops
		Pause();

		if (iAudioPlayerObject != NULL)
		{
			(*iAudioPlayerObject)->Destroy(iAudioPlayerObject); //destorys all associated interfaces with it
			iAudioPlayerObject = NULL;
			iAudioPlayerInterface = NULL;
			iSeekInterface = NULL;
			iVolumeInterface = NULL;
		}

		//reset Muted value
		iIsMuted = lHasBeenMuted;
	}
}

#if PLATFORM_ANDROID
SLDataLocator_AndroidFD AndroidGameMusicSourceCreator(const char* aAssetName)
{
	//need to get the Asset Manager
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lDirectoryClass = lJniEnv->GetObjectClass(CAndroidDirectory::JavaDirectoryObject);
	//method ID to Write String
	jmethodID lGetAssetManagerMethod = lJniEnv->GetStaticMethodID(lDirectoryClass, "GetAssetManager", "()Landroid/content/res/AssetManager;");

	//get the Asset Manager from the Java JNI
	jobject lJavaAssetManager = (jobject) lJniEnv->CallStaticObjectMethod(lDirectoryClass,lGetAssetManagerMethod);
	AAssetManager* lAssetManger = AAssetManager_fromJava(lJniEnv, lJavaAssetManager);
	AAsset* lAsset = AAssetManager_open(lAssetManger, aAssetName, AASSET_MODE_UNKNOWN);
	if(lAsset == NULL)
	{
		//does not exists
		SLDataLocator_AndroidFD lUndefinedLocation;
		return lUndefinedLocation;
	}

	//open the Asset as a file descriptor
	off_t lStartOfAsset, lLengthOfAsset;
	int lFileDescriptor = AAsset_openFileDescriptor(lAsset, &lStartOfAsset, &lLengthOfAsset);
	//close the asset
	AAsset_close(lAsset);

	//construct the locator
	SLDataLocator_AndroidFD lMusicLocator = {SL_DATALOCATOR_ANDROIDFD, lFileDescriptor, lStartOfAsset, lLengthOfAsset};

	//clean up
	lJniEnv->DeleteLocalRef(lDirectoryClass);
	lJniEnv->DeleteLocalRef(lJavaAssetManager);

	return lMusicLocator;
}
#endif

