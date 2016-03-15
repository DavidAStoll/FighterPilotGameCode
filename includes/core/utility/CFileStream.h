/*
 * CFileStrean.h
 *
 *  Created on: May 4, 2012
 *      Author: dstoll
 */

#include "includes/core/standardLibrary/StandardLibrary.h"

#ifndef CFILESTREAM_H_
#define CFILESTREAM_H_

class CFileReadStream;
class CFileWriteStream;

enum TFileWriteStreamMode
{
	EFileWriteStreamModeReplace = 0,
	EFileWriteStreamModeAppend = 1
};

class CDirectory
{
public:
	//creates a Directory, if it doesn't exists otherwise it does nothing
	static void CreateDirectory(const char* aDirectory);
	//returns the Aboslute Path to the Game Music directory, where all music files are stored
	static CString* GetGameMusicDirectory();
	//returns all Files in the given Directory in alphabetical Ascending order(if possible, A,B,C....Z), must be a valid directory
	//Otherwise it will just return an Empty array
	static CPointerArray<CString>* GetFilesInDirectory(const char* aDirectory);
	//returns the Aboslute Path to the User Music directory, where the user can store his music files
	static CString* GetUserMusicDirectory();
	//returns the Absolute Path to the Sound directory, where all the Sound Effects are Stored
	static CString* GetSoundDirectory();
};

class CNativeFile //this class needs to be implemented on the native side otherwise compiler will not succeed
{
public:
	virtual ~CNativeFile();
	CNativeFile(const char* aFileName);

	//deletes the file, if the file does not exist it does nothing
	void DeleteFile();
	//depending on native it will return an child of ReadStream, returns NULL if no such file exists
	CFileReadStream* OpenForReading();
	//depending on native it will return an child of WriteStream
	CFileWriteStream* OpenForWriting(TFileWriteStreamMode aMode);

protected:
	CString* iFileName;
};

class CFileReadStream
{
public:

	//functions need to be implemented by platform native code
	//after this call the object should be allowed to be deleted
	virtual void CloseReadStream() = 0;
	virtual TInt ReadInt32() = 0;
	virtual TUint ReadUint32() = 0;
	virtual TInt64 ReadInt64() = 0;
	virtual TUint64 ReadUint64() = 0;

protected:

	//IMPORTANT: If reading fails, should delete String, if String is Not NULL and then set it to NULL to signal that something went wrong
	//if Pointer is set to NULL, a new String Object will be created
	virtual void ReadStringNative(CString*& aString) = 0;

public:

	virtual ~CFileReadStream();
	//read IntFloat values
	TIntFloat ReadIntFloat();
	TIntFloat64 ReadIntFloat64();
	//IMPORTANT: If reading fails, should delete String, if String is Not NULL and then set it to NULL to signal that something went wrong
	//if Pointer is set to NULL, a new String Object will be created
	void ReadString(CString*& aString);
	//reads an encrypted TInt32 from disk and decodes it, if the checksum for the value failed the passed value to the method will be returned
	TInt ReadInt32Encrypted(TInt aValueIfChecksumFailed);
	TInt64 ReadInt64Encrypted();
	//reads an encrypted String from disk and decodes it, if the checksum for the String failed, the given fallback value will be assigned instead
	//if no String has been saved or its length is 0, and empty String will be returned new CString("")
	//if Pointer is set to NULL, a new String object will be created
	void ReadStringEncrypted(CString*& aString, const char* aStringIfChecksumFailed);

protected:

	CFileReadStream();
};

class CFileWriteStream
{
public:

	//need to be implemented by the native class
	//do anything necessary to close the connection to the file. After this call this object should be allowed to be deleted
	virtual void CloseWriteStream() = 0;
	virtual void WriteUint32(const TUint& aValue) = 0;
	virtual void WriteInt32(const TInt& aValue) = 0;
	virtual void WriteInt64(const TInt64& aValue) = 0;
	virtual void WriteUint64(const TUint64& aValue) = 0;

protected:

	virtual void WriteStringNative(const char* aString) = 0;

public:

	virtual ~CFileWriteStream();
	//write IntFloat values
	void WriteIntFloat(const TIntFloat& aValue);
	void WriteIntFloat64(const TIntFloat64& aValue);
	//implemented by core, since core will modify the string a bit
	void WriteString(const char* aString);
	void WriteString(CString* aString);
	//encrypt values before writing to disk
	void WriteInt32Encrypted(const TInt& aValue);
	void WriteInt64Encrypted(const TInt64& aValue);
	void WriteStringEncrypted(CString* aString);

protected:

	CFileWriteStream();
};

#endif /* CFILESTREAN_H_ */
