/*
 * CMemoryUsageTracker.h
 *
 *  Created on: Jul 30, 2012
 *      Author: dstoll
 */

#ifndef CMEMORYUSAGETRACKER_H_
#define CMEMORYUSAGETRACKER_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

class CMemoryUsageTracker
{
public:

	static CMemoryUsageTracker* New();
	virtual ~CMemoryUsageTracker();

	void UpdateGPUUsedForTextRendering(TInt aChangeInBytes);
	void UpdateGPUUsedForTextures(TInt aChangeInBytes);
	void UpdateRAMUsedForFontFile(TInt aChangeInBytes);
	void UpdateRAMUsedForSoundEffects(TInt aChangeInBytes);
	void UpdateRAMUsedForTextureCache(TInt aChangeInBytes);

	//RAM
	TInt iKBytesUsedInRAMForTextureCache;
	TInt iKBytesUsedInRAMForSoundEffects;
	TInt iKBytesUsedInRAMForFontFile;
	TInt iTotalKBytesUsedInRAM;

	//GPU
	TInt iKBytesUsedInGPUForTextures;
	TInt iKBytesUsedInGPUForTextRendering;
	TInt iTotalKBytesUsedInGPU;

protected:
	CMemoryUsageTracker();

	//should be called after memory data has been modified, it updates the total kilo bytes used in each category
	void UpdateTotalValues();
};

#endif /* CMEMORYUSAGETRACKER_H_ */
