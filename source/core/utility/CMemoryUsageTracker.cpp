/*
 * CMemoryUsageTracker.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: dstoll
 */

#include "includes/core/utility/CMemoryUsageTracker.h"

CMemoryUsageTracker::CMemoryUsageTracker()
{
	//RAM
	iKBytesUsedInRAMForTextureCache = 0;
	iKBytesUsedInRAMForSoundEffects = 0;
	iKBytesUsedInRAMForFontFile = 0;
	iTotalKBytesUsedInRAM = 0;

	//GPU
	iKBytesUsedInGPUForTextures = 0;
	iKBytesUsedInGPUForTextRendering = 0;
	iTotalKBytesUsedInGPU = 0;
}

CMemoryUsageTracker::~CMemoryUsageTracker()
{
	// TODO Auto-generated destructor stub
}

CMemoryUsageTracker* CMemoryUsageTracker::New()
{
	CMemoryUsageTracker* lSelf = new CMemoryUsageTracker();
	return lSelf;
}

void CMemoryUsageTracker::UpdateGPUUsedForTextRendering(TInt aChangeInBytes)
{
	iKBytesUsedInGPUForTextRendering += aChangeInBytes / 1000;
	UpdateTotalValues();
}

void CMemoryUsageTracker::UpdateGPUUsedForTextures(TInt aChangeInBytes)
{
	iKBytesUsedInGPUForTextures += aChangeInBytes / 1000;
	UpdateTotalValues();
}

void CMemoryUsageTracker::UpdateRAMUsedForFontFile(TInt aChangeInBytes)
{
	iKBytesUsedInRAMForFontFile += aChangeInBytes / 1000;
	UpdateTotalValues();
}

void CMemoryUsageTracker::UpdateRAMUsedForSoundEffects(TInt aChangeInBytes)
{
	iKBytesUsedInRAMForSoundEffects += aChangeInBytes / 1000;
	UpdateTotalValues();
}

void CMemoryUsageTracker::UpdateRAMUsedForTextureCache(TInt aChangeInBytes)
{
	iKBytesUsedInRAMForTextureCache += aChangeInBytes / 1000;
	UpdateTotalValues();
}


void CMemoryUsageTracker::UpdateTotalValues()
{
	iTotalKBytesUsedInRAM = iKBytesUsedInRAMForTextureCache + iKBytesUsedInRAMForSoundEffects + iKBytesUsedInRAMForFontFile;
	iTotalKBytesUsedInGPU = iKBytesUsedInGPUForTextures + iKBytesUsedInGPUForTextRendering;
}

