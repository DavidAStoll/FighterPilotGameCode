/*
 * CSystem.h
 *
 *  Created on: Oct 20, 2012
 *      Author: dstoll
 */

#ifndef CSYSTEM_H_
#define CSYSTEM_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

//contains several Native System depending function calls

class CSystem
{
public:

	//exits the game, after this call the OS will kill the application, might not be immediatly though. It might first pause the application and put it into the background
	static void ExitNative();
	//returns the current clock time in MicroSecs, clock time should be Monotonic
	static TInt64 GetCurrentTimeInMicroSecondsNative();
	//return the current Date, given in this format yyyy:mm::dd
	static CString* GetDateNative();
	//return the IMEI of the PHONE, or anything else that makes it unique
	static CString* GetPhoneSerialNumberNative();
	//return the Name of the Phone model
	static CString* GetPhoneModelNative();
	//return the OS of the Phone, should be less or equal to 5 characters
	static CString* GetPhoneOSVersionNative();
	//return the current system time, given in this format hh:mm::ss
	static CString* GetTimeNative();
	//returns if the phone is connected to a WLan
	static TBool GetIsConnectedToWLanNative();
	//returns if the Phone is in Silent Mode
	static TBool GetIsSilentModeActivated();
	//suspends the calling thread for the given micro seconds
	static void SleepForMicroSecondsNative(TUint64 aSleepTimeInMicroSeconds);
	//Opens A website with the given URL, using the default browser
	static void OpenWebsite(const char* aURL);

};




#endif /* CSYSTEM_H_ */
