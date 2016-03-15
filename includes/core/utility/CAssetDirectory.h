/*
 * CAssetDirectory.h
 *
 *  Created on: Jul 2, 2012
 *      Author: dstoll
 */

#ifndef CASSETDIRECTORY_H_
#define CASSETDIRECTORY_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

//holds function for working in the asset directory of a particular platform

class CAssetDirectory
{

public:

	//these methods needs to be implemented by the native code, since it depends on the platform
	//the aFileName will be a relative path to asset directoy on the platform

	//returns the size of the entire file in bytes
	static TInt GetAssetFileSizeInBytes(const char* aFileName);
	//plays a video using the native platform located inside the asset directory, Volume should be between 0 and 100
	static void PlayVideoInAssetDirectoryNative(const char* aVideoName, TInt aVolume);
	//reads the entire file into memory and returns a pointer to it
	static TUint8* ReadAssetFile(const char* aFileName);
	//reats number of Bytes, in the given file starting at the Offset in Bytes
	static TUint8* ReadBytesFromAssetFile(const char* aFileName, TUint aNumberOfBytes, TUint aOffsetInBytes);
};

#endif /* CASSETDIRECTORY_H_ */
