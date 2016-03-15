/*
 * IPC.h
 *
 *  Created on: May 19, 2012
 *      Author: dstoll
 */

#ifndef IPC_H_
#define IPC_H_

class CNativeThread
{
public:
	//needs to be implemented on the native Side, takes a method as an argument
	static void CreateNativeThread(void* (*lThreadMethod) (void*), void* aArg);
};

//this class needs to be implemented on the native side
class CNativeMutex
{
public:

	CNativeMutex();
	~CNativeMutex();

	void Lock(); // looks the given mutex, if already locked it will wait until it is unlocked
	void Unlock(); // unlocks the give mutex

private:
	void* iNativeMutexInstance; //pointer to the native mutex that might be needed for the implementation

};

class CNativeSemaphore
{
public:
	CNativeSemaphore(TInt aInitialValue);
	~CNativeSemaphore();

	void Increment();
	void Decrement();
	TInt GetValue();

private:
	void* iNativeSemaphoreInstance;
};

#endif /* IPC_H_ */
