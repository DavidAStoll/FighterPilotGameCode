/*
 * CSoundObject.h
 *
 *  Created on: May 28, 2012
 *      Author: dstoll
 */

#ifndef CSoundObject_H_
#define CSoundObject_H_

// INCLUDES
#include "includes/core/sound/CGameAudioEngine.h"
#include "includes/core/collision/CHitBox.h"
#include "includes/core/standardLibrary/StandardLibrary.h"

// CLASS DECLARATION

/**
 *  CSoundObject
 *
 */
class CSoundObject
{

public:

	~CSoundObject();
	static CSoundObject* New(TSoundFileId aSoundIndex, CHitBox *aHitbox);
	static CSoundObject* New(TSoundFileId aSoundIndex, TPointIntFloat aPoint);

	//changes the HitBox Pointer to point to the new HitBox
	void AssignHitBox(CHitBox* aHitBox);
	//Changes the internal SoundFile that it is going to play when playing a sound by default
	void ChangeSoundType(TSoundFileId aSoundFile);
	//plays a clip without keeping track of the channel that it played on. It will always play on a point and will not be able to stop the sound played afterwards
	void PlayCheckPointWithoutChannelUpdate(TPointIntFloat aPoint, TSoundFileId aSoundFile);
	//plays a clip without keeping track of the channel that it played on. It will always play on a point and will not be able to stop the sound played afterwards
	void PlayCheckPointWithoutChannelUpdate(CHitBox* aHitBox, TSoundFileId aSoundFile);
	//used for game objects that uses a hit box
	void PlayHitBox();
	//used for game objects that uses a point
	void PlayCheckPoint(TPointIntFloat aPoint);
	//used for game objects that uses a hit box, plays the particular SoundFile only once without changing the internal SoundFile
	void PlayHitBox(TSoundFileId aSoundFile);
	//used for game objects that uses a point, plays the particular SoundFile only once without changing the internal SoundFile
	void PlayCheckPoint(TPointIntFloat aPoint, TSoundFileId aSoundFile);
	//used for game objects that uses a hit box and it is playing a sound repeatly
	void PlayRepeatHitBox();
	//used for game objects that uses a point and it is playing  a sound repeatly
	void PlayRepeatCheckPoint();
	//stops the Sound playing in the last channel that this SoundObject used
	void StopSoundChannel();
	//stops the repeat playback if this object played a repeat sound
	void StopSoundChannelIfPlayingRepeatSound();

	//used by load and save function of the game to store important states of this object
	void SaveOnDisk(CFileWriteStream& aOutputStream);
	void LoadFromDisk(CFileReadStream& aReadStream);

private:

	CSoundObject();
	void Construct(TSoundFileId aSoundIndex, CHitBox *aHitbox);
	void Construct(TSoundFileId aSoundIndex, TPointIntFloat aPoint);

	CHitBox* iHitBox;
	TPointIntFloat iCheckPoint;
	TSoundFileId iSoundFile;
	//keeps track on what channel we played last time a sound, will be -1 if we haven;t played any yet
	TInt iChannel;
	//will be set to true if the last sound we played was  a repeat sound
	TBool iPlayingRepeatSound;
};

#endif /* CSoundObject_H_ */
