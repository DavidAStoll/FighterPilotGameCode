/*
 * NativeIPC.cpp
 *
 *  Created on: May 19, 2012
 *      Author: dstoll
 */

#include <pthread.h>
#include <semaphore.h>
#include <jni.h>
#include "includes/native/CAndroidFighterPilotThePacificWar.h"
#include "includes/core/utility/IPC.h"

//this method attaches the new thread to the JavaVM to allow for JNI calls
void* ThreadHandlerMethod(void* lThreadMethodAndArg)
{
	//attach to the VM
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI();
	CAndroidFighterPilotThePacificWar::VM->AttachCurrentThread(&lJniEnv, NULL); //attaches the thread to the VM

	//call actual Thread method
	void** lInt64Pointer = (void**) lThreadMethodAndArg;
	void* lThreadMethod = (void*) lInt64Pointer[0]; //convert Int64 value to memory address
	void* lArg = (void*) lInt64Pointer[1];

	((void* (*)(void*))lThreadMethod)(lArg);

	//detach from the VM
	CAndroidFighterPilotThePacificWar::VM->DetachCurrentThread();

	return NULL;
}

void CNativeThread::CreateNativeThread(void* (*aThreadMethod) (void*), void* aArg)
{
	pthread_t lPid;
	//construct a pointer to an array that holds the references to the memory location when the Method and Arg is located in memory
	void** lMethodAndArg = new void* [2];
	lMethodAndArg[0] =  (void*) aThreadMethod;
	lMethodAndArg[1] =  aArg;

	pthread_create(&lPid, NULL, ThreadHandlerMethod, (void*) lMethodAndArg); //creates a new thread in linux/Androidd
}

CNativeMutex::CNativeMutex()
{
	iNativeMutexInstance = new pthread_mutex_t;
	pthread_mutex_init(static_cast<pthread_mutex_t*> (iNativeMutexInstance), NULL);
}

CNativeMutex::~CNativeMutex()
{
	pthread_mutex_destroy(static_cast<pthread_mutex_t*> (iNativeMutexInstance));
	delete static_cast<pthread_mutex_t*> (iNativeMutexInstance);
}

void CNativeMutex::Lock()
{
	pthread_mutex_lock((pthread_mutex_t*) (iNativeMutexInstance));
}

void CNativeMutex::Unlock()
{
	pthread_mutex_unlock((pthread_mutex_t*) iNativeMutexInstance);
}

CNativeSemaphore::CNativeSemaphore(TInt aInitialValue)
{
	iNativeSemaphoreInstance = new sem_t;
	sem_init(static_cast<sem_t*>(iNativeSemaphoreInstance), 0, aInitialValue);
}

CNativeSemaphore::~CNativeSemaphore()
{
	sem_destroy(static_cast<sem_t*>(iNativeSemaphoreInstance));
	delete static_cast<sem_t*>(iNativeSemaphoreInstance);
}

void CNativeSemaphore::Increment()
{
	sem_post(static_cast<sem_t*>(iNativeSemaphoreInstance));
}

void CNativeSemaphore::Decrement()
{
	sem_wait(static_cast<sem_t*>(iNativeSemaphoreInstance));
}

TInt CNativeSemaphore::GetValue()
{
	TInt lValue;
	sem_getvalue(static_cast<sem_t*>(iNativeSemaphoreInstance), &lValue);
	return lValue;
}




