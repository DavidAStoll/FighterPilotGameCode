/*
 * CWavLoader.h
 *
 *  Created on: May 16, 2012
 *      Author: dstoll
 */

#ifndef CWAVLOADER_H_
#define CWAVLOADER_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/sound/CSoundPlayer.h"

#define WAVLOADER_WAV_FILE_HEADER_SIZE_IN_BYTES 44

class CWavLoader
{
public:

	CWavLoader();
	virtual ~CWavLoader();

	TSoundTrack Load16Bit(const char* aFileName);
	TSoundTrack Load16BitSkipHeader(const char* aFileName);
	TSoundTrack Load8Bit(const char* aFileName);
	TSoundTrack Load8BitSkipHeader(const char* aFileName);
};

#endif /* CWAVLOADER_H_ */
