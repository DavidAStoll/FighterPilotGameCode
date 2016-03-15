/*
 * CSoundFilePreferredVolume.h
 *
 *  Created on: May 17, 2012
 *      Author: dstoll
 */

#ifndef CSOUNDFILEPREFERREDVOLUME_H_
#define CSOUNDFILEPREFERREDVOLUME_H_

// sound values are 0 to 100, and must be multiplied by this constant in order to be the in the correct range
#define PREFERRED_SOUND_VOLUME_MUTILPLY 25

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/sound/CSoundFileNameMapping.h"

class CSoundFilePreferredVolume
{
public:
	CSoundFilePreferredVolume();
	virtual ~CSoundFilePreferredVolume();

	TInt iPreferredVolume [ESoundFileId_LAST_INDEX];
};

#endif /* CSOUNDFILEPREFERREDVOLUME_H_ */
