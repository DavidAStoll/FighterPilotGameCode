/*
 * CWavLoader.cpp
 *
 *  Created on: May 16, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CWavLoader.h"
#include "includes/core/utility/CAssetDirectory.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "includes/core/CGame.h"

CWavLoader::CWavLoader()
{
	// TODO Auto-generated constructor stub

}

CWavLoader::~CWavLoader()
{
	// TODO Auto-generated destructor stub
}

TSoundTrack CWavLoader::Load16Bit(const char* aFileName)
{
	TInt16* lFileContent = NULL;
	lFileContent = (TInt16*) CAssetDirectory::ReadAssetFile(aFileName);
	TInt lWavFileSizeInBytes = CAssetDirectory::GetAssetFileSizeInBytes(aFileName);
	TInt lWavFileSizeInShorts = lWavFileSizeInBytes / 2;

	//add memory stats
	CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForSoundEffects(lWavFileSizeInBytes);

	TSoundTrack lNewTrack;
	lNewTrack.iData = lFileContent;
	lNewTrack.iLength = lWavFileSizeInShorts;
	lNewTrack.iRepStart = 0;
	lNewTrack.iRepEnd = 0;

	return lNewTrack;
}

TSoundTrack CWavLoader::Load16BitSkipHeader(const char* aFileName)
{
	TInt16* lFileContent = NULL;
	lFileContent = (TInt16*) CAssetDirectory::ReadAssetFile(aFileName);
	TInt lWavFileSizeInBytes = CAssetDirectory::GetAssetFileSizeInBytes(aFileName);
	TInt lWavFileSizeInShorts = lWavFileSizeInBytes / 2;
	TInt lWavHeaderInShorts = WAVLOADER_WAV_FILE_HEADER_SIZE_IN_BYTES / 2;
	TInt16* lWavStreamWithoutHeader = new TInt16[lWavFileSizeInShorts - lWavHeaderInShorts];

	//add memory stats
	CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForSoundEffects(lWavFileSizeInBytes);

	//copy over values but skip the header
	for(TInt lIndex = lWavHeaderInShorts; lIndex < lWavFileSizeInShorts; lIndex++)
	{
		lWavStreamWithoutHeader[lIndex - lWavHeaderInShorts] = lFileContent[lIndex];
	}

	TSoundTrack lNewTrack;
	lNewTrack.iData = lWavStreamWithoutHeader;
	lNewTrack.iLength = lWavFileSizeInShorts - lWavHeaderInShorts;
	lNewTrack.iRepStart = 0;
	lNewTrack.iRepEnd = 0;

	return lNewTrack;
}

TSoundTrack CWavLoader::Load8Bit(const char* aFileName)
{
	TUint8* lFileContent = NULL;
	lFileContent = CAssetDirectory::ReadAssetFile(aFileName);
	TInt lWavFileSizeInBytes = CAssetDirectory::GetAssetFileSizeInBytes(aFileName);
	TInt16* l16BitBuffer = new TInt16[lWavFileSizeInBytes];

	//add memory stats
	CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForSoundEffects(lWavFileSizeInBytes);

	//transform 8 bit value into a 16 bit one
	for(TInt lIndex = 0; lIndex < lWavFileSizeInBytes; lIndex++)
	{
		TInt lOldValue = lFileContent[lIndex];
		lOldValue -= 128;
		l16BitBuffer[lIndex] = (TInt16) (lOldValue * 256);
	}

	TSoundTrack lNewTrack;
	lNewTrack.iData = l16BitBuffer;
	lNewTrack.iLength = lWavFileSizeInBytes;
	lNewTrack.iRepStart = 0;
	lNewTrack.iRepEnd = 0;

	//clean up 8bit array
	delete[] lFileContent;

	return lNewTrack;
}

TSoundTrack CWavLoader::Load8BitSkipHeader(const char* aFileName)
{
	TUint8* lFileContent = NULL;
	lFileContent = CAssetDirectory::ReadAssetFile(aFileName);
	TInt lWavFileSizeInBytes = CAssetDirectory::GetAssetFileSizeInBytes(aFileName);
	TInt lSizeOfHeader = WAVLOADER_WAV_FILE_HEADER_SIZE_IN_BYTES;
	TInt lWavFileSizeWithoutHeader =  lWavFileSizeInBytes - lSizeOfHeader;
	TInt16* l16BitBuffer = new TInt16[lWavFileSizeWithoutHeader]; //don't contain header

	//add memory stats
	CGame::Game->iMemoryUsageTracker->UpdateRAMUsedForSoundEffects(lWavFileSizeInBytes);

	//transform 8 bit value into a 16 bit one
	for(TInt lIndex = lSizeOfHeader; lIndex < lWavFileSizeInBytes; lIndex++)
	{
		TInt lOldValue = lFileContent[lIndex];
		lOldValue -= 128;
		l16BitBuffer[lIndex - lSizeOfHeader] = (TInt16) (lOldValue * 256);
	}

	TSoundTrack lNewTrack;
	lNewTrack.iData = l16BitBuffer;
	lNewTrack.iLength = lWavFileSizeWithoutHeader;
	lNewTrack.iRepStart = 0;
	lNewTrack.iRepEnd = 0;

	//clean up 8bit array
	delete[] lFileContent;

	return lNewTrack;
}
