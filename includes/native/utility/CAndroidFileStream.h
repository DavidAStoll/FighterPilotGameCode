/*
 * CAndroidFileStream.h
 *
 *  Created on: May 5, 2012
 *      Author: dstoll
 */

#ifndef CANDROIDFILESTREAM_H_
#define CANDROIDFILESTREAM_H_

#include "includes/core/utility/CFileStream.h"
#include <jni.h>

class CAndroidDirectory : public CDirectory
{
public:
	//needed for JNI
	static void InitJavaDirectoryObject(JNIEnv* aJniEnv);
	static jobject JavaDirectoryObject;
};

class CAndroidReadStream : public CFileReadStream
{

public:

	//needed for the JNI calls
	static void InitJavaReadStreamObject(JNIEnv* aJniEnv);
	static jobject JavaReadStreamObject;

	virtual ~CAndroidReadStream();
	static CAndroidReadStream* OpenReadStream(CString* aFileName);
	//after this call the object should be allowed to be deleted
	virtual void CloseReadStream();
	virtual TInt ReadInt32();
	virtual TUint ReadUint32();
	virtual TInt64 ReadInt64();
	virtual TUint64 ReadUint64();

protected:

	//IMPORTANT: If reading fails, should delete String, if String is Not NULL and then set it to NULL to signal that something went wrong
	//if Pointer is set to NULL, a new String Object will be created
	virtual void ReadStringNative(CString*& aString);
	CAndroidReadStream(jobject aReadFile);
	jobject iReadFile;
};

class CAndroidWriteStream : public CFileWriteStream
{

public:

	//needed for the JNI calls
	static void InitJavaWriteStreamObject(JNIEnv* aJniEnv);
	static jobject JavaWriteStreamObject;

	virtual ~CAndroidWriteStream();
	//do anything necessary to gain access to this file and be able to start writing to it
	static CAndroidWriteStream* OpenWriteStream(CString* aFileName, TFileWriteStreamMode aFileMode);
	//do anything necessary to close the connection to the file. After this call this object should be allowed to be deleted
	virtual void CloseWriteStream();
	virtual void WriteInt32(const TInt& aValue);
	virtual void WriteUint32(const TUint& aValue);
	virtual void WriteInt64(const TInt64& aValue);
	virtual void WriteUint64(const TUint64& aValue);
	virtual void WriteStringNative(const char* aString);

protected:

	CAndroidWriteStream(jobject aWriterFile);
	jobject iWriterFile;

};

#endif /* CANDROIDFILESTREAM_H_ */
