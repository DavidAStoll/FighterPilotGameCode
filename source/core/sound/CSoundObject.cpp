/*
 * CSoundObject.cpp
 *
 *  Created on: May 28, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CSoundObject.h"
#include "includes/core/CGame.h"

CSoundObject::CSoundObject()
{
	iChannel = -1;
	iHitBox = NULL;
	iPlayingRepeatSound = false;
}

CSoundObject::~CSoundObject()
{
	if (iHitBox != NULL && iChannel != -1)
	{
		//if iChannel is still in use that means we need to convert it to a point and let the sound continue
		if (CGame::Game->iAudioEngine->IsSoundChannelInUse(iChannel))
		{
			//check if hitbox matches in the hitbox array, if they match then this sound object is playing in this channel
			if (iHitBox == CGame::Game->iAudioEngine->iObjectHitBoxes[iChannel])
			{
				TPointIntFloat lHitBoxMiddlePoint = CMath::GetCenterOfRect(iHitBox->GetFastIntersectionRect());
				CGame::Game->iAudioEngine->iObjectCoordinates[iChannel] = lHitBoxMiddlePoint;
				CGame::Game->iAudioEngine->iObjectHitBoxes[iChannel] = NULL; //prevent accessing deleted object
			}
		}
	}
	//stop the playing back of repeat sounds, since this is our last change to do so, once the object gets deleted we don't have a chance to stop it
	StopSoundChannelIfPlayingRepeatSound();
}

CSoundObject*
CSoundObject::New(TSoundFileId aSoundIndex, CHitBox *aHitbox)
{
	CSoundObject* self = new CSoundObject();
	self->Construct(aSoundIndex, aHitbox);
	return self;
}

CSoundObject*
CSoundObject::New(TSoundFileId aSoundIndex, TPointIntFloat aPoint)
{
	CSoundObject* self = new CSoundObject();
	self->Construct(aSoundIndex, aPoint);
	return self;
}

void CSoundObject::Construct(TSoundFileId aSoundIndex, CHitBox *aHitbox)
{
	iSoundFile = aSoundIndex;
	iHitBox = aHitbox;
}

void CSoundObject::Construct(TSoundFileId aSoundIndex, TPointIntFloat aPoint)
{
	iSoundFile = aSoundIndex;
	iCheckPoint = aPoint;
	iHitBox = NULL;
}

void CSoundObject::AssignHitBox(CHitBox* aHitBox)
{
	iHitBox = aHitBox;
}

void CSoundObject::ChangeSoundType(TSoundFileId aSoundFile)
{

	iSoundFile = aSoundFile;

}

void CSoundObject::PlayHitBox()
{
	//convert the soundchannel to use a point instead of a hitbox
	if(iChannel != -1)//make sure we played in a iChannel before
	{
		//check if our hitbox is still used in the old iChannel, don't want that we have to channels play using the SoundObject and Hitbox
		if(iHitBox != NULL && iHitBox == CGame::Game->iAudioEngine->iObjectHitBoxes[iChannel])
		{
			TPointIntFloat lHitBoxMiddlePoint = CMath::GetCenterOfRect(iHitBox->GetFastIntersectionRect());
			CGame::Game->iAudioEngine->iObjectCoordinates[iChannel] = lHitBoxMiddlePoint;
			CGame::Game->iAudioEngine->iObjectHitBoxes[iChannel] = NULL; //prevent accessing deleted object
		}
	}
	StopSoundChannelIfPlayingRepeatSound();
	iChannel = CGame::Game->iAudioEngine->PlayCheckBox(iSoundFile, iHitBox);
}

void CSoundObject::PlayCheckPoint(TPointIntFloat aPoint)
{

	StopSoundChannelIfPlayingRepeatSound();
	iChannel = CGame::Game->iAudioEngine->PlayCheckPoint(iSoundFile, aPoint);

}

//plays a clip without keeping track of the channel that it played on. It will always play on a point and will not be able to stop the sound played afterwards
void CSoundObject::PlayCheckPointWithoutChannelUpdate(TPointIntFloat aPoint, TSoundFileId aSoundFile)
{

	TSoundFileId lOldSoundFile = iSoundFile;
	iSoundFile = aSoundFile;
	CGame::Game->iAudioEngine->PlayCheckPoint(iSoundFile, aPoint); //don't care what channel has been returned
	iSoundFile = lOldSoundFile;

}

//plays a clip without keeping track of the channel that it played on. It will always play on a point and will not be able to stop the sound played afterwards
void CSoundObject::PlayCheckPointWithoutChannelUpdate(CHitBox* aHitBox, TSoundFileId aSoundFile)
{

	TPointIntFloat lSoundLocation = CMath::GetCenterOfRect(aHitBox->GetFastIntersectionRect());
	PlayCheckPointWithoutChannelUpdate(lSoundLocation,aSoundFile);

}

//used for game objects that uses a hit box, plays the particular SoundFile only once without changing the internal SoundFile
void CSoundObject::PlayHitBox(TSoundFileId aSoundFile)
{
	TSoundFileId lOldSoundFile = iSoundFile;
	iSoundFile = aSoundFile;
	PlayHitBox();
	iSoundFile = lOldSoundFile;
}

//used for game objects that uses a point, plays the particular SoundFile only once without changing the internal SoundFile
void CSoundObject::PlayCheckPoint(TPointIntFloat aPoint, TSoundFileId aSoundFile)
{

	TSoundFileId lOldSoundFile = iSoundFile;
	iSoundFile = aSoundFile;
	PlayCheckPoint(aPoint);
	iSoundFile = lOldSoundFile;
}

void CSoundObject::PlayRepeatHitBox()
{
	StopSoundChannelIfPlayingRepeatSound();
	iPlayingRepeatSound = true;
	iChannel = CGame::Game->iAudioEngine->PlayRepeatCheckBox(iSoundFile, iHitBox);
}

void CSoundObject::PlayRepeatCheckPoint()
{
	StopSoundChannelIfPlayingRepeatSound();
	iPlayingRepeatSound = true;
	iChannel = CGame::Game->iAudioEngine->PlayRepeatCheckPoint(iSoundFile, iCheckPoint);
}

void CSoundObject::StopSoundChannelIfPlayingRepeatSound()
{
	if(iPlayingRepeatSound)
		StopSoundChannel();
}

void CSoundObject::StopSoundChannel()
{
	if(iChannel != -1)
	{
		CGame::Game->iAudioEngine->StopSound(iChannel);
		iPlayingRepeatSound = false;// since don't play any sounds right now
		iChannel = -1;
	}
}

void CSoundObject::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iSoundFile);
	aOutputStream.WriteInt32(iPlayingRepeatSound);
	aOutputStream.WriteIntFloat(iCheckPoint.iX);
	aOutputStream.WriteIntFloat(iCheckPoint.iY);
}
void CSoundObject::LoadFromDisk(CFileReadStream &aReadStream)
{
	//since sometimes the soundobject load function is called after it has been created, it might have already played it sound
	//we need to stop playing this sound and play it again, at the new location
	//in addition, we might have started to play a repeat sound, thus we need to check that as well

	TSoundFileId lTempSoundFile = static_cast<TSoundFileId>(aReadStream.ReadInt32());
	TBool lTempRepeatSound = aReadStream.ReadInt32();
	iCheckPoint.iX = aReadStream.ReadIntFloat();
	iCheckPoint.iY = aReadStream.ReadIntFloat();

	if (lTempRepeatSound)
	{
		//was playing a repeat sound when the object got saved, keep iPlayingRepeatSound to false, since there is no channel to stop
		iSoundFile = lTempSoundFile;
		if (iHitBox)
			PlayRepeatHitBox();
		else
			PlayRepeatCheckPoint();
	}
	else if (iChannel != -1)
	{
		if (iPlayingRepeatSound)
			StopSoundChannel(); //stop playing the repeat sound
		else
		{
			StopSoundChannel(); //stop the sound we played
			if (iHitBox)
				PlayHitBox();
			else
				PlayCheckPoint(iCheckPoint);
		}
	}
}
