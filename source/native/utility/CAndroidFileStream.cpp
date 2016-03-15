/*
 * CAndroidFileStream.cpp
 *
 *  Created on: May 5, 2012
 *      Author: dstoll
 */

#include "includes/native/utility/CAndroidFileStream.h"
#include "includes/native/CAndroidFighterPilotThePacificWar.h"

//needed for the JNI calls
jobject CAndroidReadStream::JavaReadStreamObject;
jobject CAndroidWriteStream::JavaWriteStreamObject;
jobject CAndroidDirectory::JavaDirectoryObject;

//----------------------------- DIRECTORY ------------------------------------//

void CAndroidDirectory::InitJavaDirectoryObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidDirectory", &CAndroidDirectory::JavaDirectoryObject);
}

CString* GetPublicDirectory()
{
	CString* lPublicPath = new CString();

	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lDirectoryClass = lJniEnv->GetObjectClass(CAndroidDirectory::JavaDirectoryObject);
	//method ID to Write String
	jmethodID lGetPublicDirectoryMethod = lJniEnv->GetStaticMethodID(lDirectoryClass, "GetPublicDirectory", "()Ljava/lang/String;");

	//Write String to file
	jstring lReadJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lDirectoryClass, lGetPublicDirectoryMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lReadJavaString, NULL);
	lPublicPath->Replace(lNativeString); //fill Strings data array

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lReadJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lDirectoryClass);
	lJniEnv->DeleteLocalRef(lReadJavaString);

	return lPublicPath;
}

CString* GetStorageDirectory()
{
	CString* lStoragePath = new CString();

	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lDirectoryClass = lJniEnv->GetObjectClass(CAndroidDirectory::JavaDirectoryObject);
	//method ID to Write String
	jmethodID lGetStorageDirectoryMethod = lJniEnv->GetStaticMethodID(lDirectoryClass, "GetStorageDirectory", "()Ljava/lang/String;");

	//Write String to file
	jstring lReadJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lDirectoryClass, lGetStorageDirectoryMethod);
	const char* lNativeString = lJniEnv->GetStringUTFChars(lReadJavaString, NULL);
	lStoragePath->Replace(lNativeString); //fill Strings data array

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lReadJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lDirectoryClass);
	lJniEnv->DeleteLocalRef(lReadJavaString);

	return lStoragePath;
}

void CDirectory::CreateDirectory(const char* aDirectory)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lDirectoryClass = lJniEnv->GetObjectClass(CAndroidDirectory::JavaDirectoryObject);
	//method ID to Write String
	jmethodID lCreateDirectoryMethod = lJniEnv->GetStaticMethodID(lDirectoryClass, "CreateDirectory", "(Ljava/lang/String;)V");

	//Write String to file
	jstring lJavaString = lJniEnv->NewStringUTF(aDirectory);
	lJniEnv->CallStaticVoidMethod(lDirectoryClass, lCreateDirectoryMethod, lJavaString);
	//garbage collection
	lJniEnv->DeleteLocalRef(lDirectoryClass);
	lJniEnv->DeleteLocalRef(lJavaString);
}

CString* CDirectory::GetGameMusicDirectory()
{
	return GetStorageDirectory()->Append("Music/");
}

CPointerArray<CString>* CDirectory::GetFilesInDirectory(const char* aDirectory)
{
	CPointerArray<CString>* lFiles = CPointerArray<CString>::New();
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lDirectoryClass = lJniEnv->GetObjectClass(CAndroidDirectory::JavaDirectoryObject);
	//method ID to Write String
	jmethodID lGetFilesInDirectoryMethod = lJniEnv->GetStaticMethodID(lDirectoryClass, "GetFilesInDirectory", "(Ljava/lang/String;)[Ljava/lang/String;");

	//Write String to file
	jstring lJavaString = lJniEnv->NewStringUTF(aDirectory);
	jobjectArray lJavaStringArray = (jobjectArray) lJniEnv->CallStaticObjectMethod(lDirectoryClass, lGetFilesInDirectoryMethod, lJavaString);

	if(lJavaStringArray != NULL) //might fail on us
	{
		int lNumberOfFileNames = lJniEnv->GetArrayLength(lJavaStringArray);
		for(TInt lIndex = 0; lIndex < lNumberOfFileNames; lIndex++)
		{
			//extract the first jstring from the array
			jstring lJavaFileName = (jstring) lJniEnv->GetObjectArrayElement(lJavaStringArray, lIndex);

			//convert Java String to CString
			const char* lNativeString = lJniEnv->GetStringUTFChars(lJavaFileName, NULL);
			CString* lCFileName = new CString(lNativeString);
			//append file to list
			lFiles->Append(lCFileName);

			//release UTF Chars
			lJniEnv->ReleaseStringUTFChars(lJavaFileName, lNativeString);
		}

		//clean up array
		lJniEnv->DeleteLocalRef(lJavaStringArray);
	}

	//garbage collection
	lJniEnv->DeleteLocalRef(lDirectoryClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lFiles;
}

CString* CDirectory::GetSoundDirectory()
{
	return GetStorageDirectory()->Append("Sound/");
}

CString*  CDirectory::GetUserMusicDirectory()
{
	return GetPublicDirectory()->Append("HQI/FTW/Music/");
}

//----------------------------- NATIVE FILE ------------------------------------//

CNativeFile::CNativeFile(const char* aFileName)
{
	iFileName = NULL;
	iFileName = new CString(aFileName);
}

CNativeFile::~CNativeFile()
{
	if(iFileName != NULL)
	{
		delete iFileName;
		iFileName = NULL;
	}
}

CFileReadStream* CNativeFile::OpenForReading()
{
	CAndroidReadStream* lReadStream = CAndroidReadStream::OpenReadStream(iFileName);
	return lReadStream;
}

CFileWriteStream* CNativeFile::OpenForWriting(TFileWriteStreamMode aMode)
{
	CAndroidWriteStream* lWriteStream = CAndroidWriteStream::OpenWriteStream(iFileName, aMode);
	return lWriteStream;
}

void CNativeFile::DeleteFile()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileWriterClass = lJniEnv->GetObjectClass(CAndroidWriteStream::JavaWriteStreamObject);
	//method ID to close the file
	jmethodID lDeleteObjectMethod = lJniEnv->GetStaticMethodID(lFileWriterClass, "DeleteFile", "(Ljava/lang/String;)V");

	//Delete file
	jstring lJavaString = lJniEnv->NewStringUTF(iFileName->GetData());
	lJniEnv->CallStaticVoidMethod(lFileWriterClass, lDeleteObjectMethod, lJavaString);

	//garbage collection
	lJniEnv->DeleteLocalRef(lJavaString);
	lJniEnv->DeleteLocalRef(lFileWriterClass);
}


//----------------------------- Android READ STREAM ------------------------------------//

void CAndroidReadStream::InitJavaReadStreamObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidFileReader", &CAndroidReadStream::JavaReadStreamObject);
}

CAndroidReadStream::CAndroidReadStream(jobject aReadFile)
{
	iReadFile = aReadFile;
}

CAndroidReadStream::~CAndroidReadStream()
{
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	if(iReadFile != NULL)
	{
		lJniEnv->DeleteLocalRef(iReadFile);
		iReadFile = NULL;
	}
}

CAndroidReadStream* CAndroidReadStream::OpenReadStream(CString* aFileName)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//get IDs
	jclass lFileReaderClass = lJniEnv->GetObjectClass(CAndroidReadStream::JavaReadStreamObject);
	jmethodID lCreateObjectMethod = lJniEnv->GetMethodID(lFileReaderClass, "<init>", "(Ljava/lang/String;)V");
	jmethodID lOpenFileMethod = lJniEnv->GetStaticMethodID(lFileReaderClass, "OpenFile", "(LfighterPilot/RisingSunAlpha/AndroidFileReader;)I");
	jobject lFileReader = NULL;

	//create read object
	jstring lJavaString = lJniEnv->NewStringUTF(aFileName->GetData());
	lFileReader = (jobject) lJniEnv->NewObject(lFileReaderClass, lCreateObjectMethod, lJavaString);

	//open file
	TInt lOpenSuccess = (TInt) lJniEnv->CallStaticIntMethod(lFileReaderClass, lOpenFileMethod, lFileReader);

	//garbage collection
	lJniEnv->DeleteLocalRef(lFileReaderClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	if(lOpenSuccess == 1)
	{
		return new CAndroidReadStream(lFileReader);
	}
	else
	{
		lJniEnv->DeleteLocalRef(lFileReader);
		return NULL;
	}
}

void CAndroidReadStream::CloseReadStream()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileReaderClass = lJniEnv->GetObjectClass(CAndroidReadStream::JavaReadStreamObject);
	//method ID to close the file
	jmethodID lCloseObjectMethod = lJniEnv->GetStaticMethodID(lFileReaderClass, "CloseFile", "(LfighterPilot/RisingSunAlpha/AndroidFileReader;)V");

	//Close file
	lJniEnv->CallStaticVoidMethod(lFileReaderClass, lCloseObjectMethod, (jobject) iReadFile);
	//garbage collection
	if(iReadFile != NULL)
	{
		lJniEnv->DeleteLocalRef(iReadFile);
		iReadFile = NULL;
	}
	lJniEnv->DeleteLocalRef(lFileReaderClass);
}

TInt CAndroidReadStream::ReadInt32()
{
	TInt lReturnValue = 0;
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI
	//class id
	jclass lFileReaderClass = lJniEnv->GetObjectClass(CAndroidReadStream::JavaReadStreamObject);
	//method ID to Write String
	jmethodID lReadInt32Method = lJniEnv->GetStaticMethodID(lFileReaderClass, "ReadInt32", "(LfighterPilot/RisingSunAlpha/AndroidFileReader;)I");

	//make the call
	lReturnValue = (TInt) lJniEnv->CallStaticIntMethod(lFileReaderClass, lReadInt32Method, (jobject) iReadFile);
	//garbage
	lJniEnv->DeleteLocalRef(lFileReaderClass);

	return lReturnValue;
}

TUint CAndroidReadStream::ReadUint32()
{
	return ReadInt32();
}

TInt64 CAndroidReadStream::ReadInt64()
{
	TInt64 lReturnValue = 0;
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI(); 	//need reference to the JNI
	//class id
	jclass lFileReaderClass = lJniEnv->GetObjectClass(CAndroidReadStream::JavaReadStreamObject);
	//method ID to Write String
	jmethodID lReadInt64Method = lJniEnv->GetStaticMethodID(lFileReaderClass, "ReadInt64", "(LfighterPilot/RisingSunAlpha/AndroidFileReader;)J");

	//make the call
	lReturnValue = (TInt64) lJniEnv->CallStaticLongMethod(lFileReaderClass, lReadInt64Method, (jobject) iReadFile);
	//garbage
	lJniEnv->DeleteLocalRef(lFileReaderClass);

	return lReturnValue;
}

TUint64 CAndroidReadStream::ReadUint64()
{
	return ReadInt64();
}

void CAndroidReadStream::ReadStringNative(CString*& aString)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileReaderClass = lJniEnv->GetObjectClass(CAndroidReadStream::JavaReadStreamObject);
	//method ID to Write String
	jmethodID lReadStringMethod = lJniEnv->GetStaticMethodID(lFileReaderClass, "ReadString", "(LfighterPilot/RisingSunAlpha/AndroidFileReader;)Ljava/lang/String;");

	//Write String to file
	jstring lReadJavaString = (jstring) lJniEnv->CallStaticObjectMethod(lFileReaderClass, lReadStringMethod, (jobject) iReadFile);
	if(lReadJavaString == NULL)
	{
		//something went wrong
		delete aString;
		aString = NULL;
		return;
	}

	const char* lNativeString = lJniEnv->GetStringUTFChars(lReadJavaString, NULL);
	if(aString != NULL)
		aString->Replace(lNativeString); //fill Strings data array
	else
		aString = new CString(lNativeString);

	//garbage collection
	lJniEnv->ReleaseStringUTFChars(lReadJavaString, lNativeString);
	lJniEnv->DeleteLocalRef(lFileReaderClass);
	lJniEnv->DeleteLocalRef(lReadJavaString);
}


//----------------------------- Android WRITE STREAM ------------------------------------//

void CAndroidWriteStream::InitJavaWriteStreamObject(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidFileWriter", &CAndroidWriteStream::JavaWriteStreamObject);
}

CAndroidWriteStream::CAndroidWriteStream(jobject aWriterFile)
{
	iWriterFile = aWriterFile;
}

CAndroidWriteStream::~CAndroidWriteStream()
{
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI();
	if(iWriterFile != NULL)
	{
		lJniEnv->DeleteLocalRef(iWriterFile);
		iWriterFile = NULL;
	}
}

CAndroidWriteStream* CAndroidWriteStream::OpenWriteStream(CString* aFileName, TFileWriteStreamMode aFileMode)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//get IDs
	jclass lFileWriterClass = lJniEnv->GetObjectClass(CAndroidWriteStream::JavaWriteStreamObject);
	jmethodID lCreateObjectMethod = lJniEnv->GetMethodID(lFileWriterClass, "<init>", "(Ljava/lang/String;I)V");
	jmethodID lOpenFileMethod = lJniEnv->GetStaticMethodID(lFileWriterClass, "OpenFile", "(LfighterPilot/RisingSunAlpha/AndroidFileWriter;)I");

	//create writer object
	jstring lJavaString = lJniEnv->NewStringUTF(aFileName->GetData());
	jobject lFileWriter = (jobject) lJniEnv->NewObject(lFileWriterClass, lCreateObjectMethod, lJavaString, (jint) aFileMode);
	//open file
	TInt lOpenSuccess = (TInt) lJniEnv->CallStaticIntMethod(lFileWriterClass, lOpenFileMethod, lFileWriter);

	//garbage collection
	lJniEnv->DeleteLocalRef(lFileWriterClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	if(lOpenSuccess == 1)
	{
		return new CAndroidWriteStream(lFileWriter);
	}
	else
	{
		lJniEnv->DeleteLocalRef(lFileWriter);
		return NULL;
	}
}

void CAndroidWriteStream::CloseWriteStream()
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileWriterClass = lJniEnv->GetObjectClass(CAndroidWriteStream::JavaWriteStreamObject);
	//method ID to close the file
	jmethodID lCloseObjectMethod = lJniEnv->GetStaticMethodID(lFileWriterClass, "CloseFile", "(LfighterPilot/RisingSunAlpha/AndroidFileWriter;)V");

	//Close file
	lJniEnv->CallStaticVoidMethod(lFileWriterClass, lCloseObjectMethod, (jobject) iWriterFile);
	//garbage collection
	if(iWriterFile != NULL)
	{
		lJniEnv->DeleteLocalRef(iWriterFile);
		iWriterFile = NULL;
	}
	lJniEnv->DeleteLocalRef(lFileWriterClass);
}

void CAndroidWriteStream::WriteInt32(const TInt& aValue)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileWriterClass = lJniEnv->GetObjectClass(CAndroidWriteStream::JavaWriteStreamObject);
	//method ID to Write Int32
	jmethodID lWriteInt32Method = lJniEnv->GetStaticMethodID(lFileWriterClass, "WriteInt32", "(LfighterPilot/RisingSunAlpha/AndroidFileWriter;I)V");

	//Write String to file
	lJniEnv->CallStaticVoidMethod(lFileWriterClass, lWriteInt32Method, (jobject) iWriterFile, (jint) aValue);
	//garbage collection
	lJniEnv->DeleteLocalRef(lFileWriterClass);
}

void CAndroidWriteStream::WriteUint32(const TUint& aValue)
{
	WriteInt32(aValue);
}

void CAndroidWriteStream::WriteInt64(const TInt64& aValue)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileWriterClass = lJniEnv->GetObjectClass(CAndroidWriteStream::JavaWriteStreamObject);
	//method ID to Write Int64
	jmethodID lWriteInt64Method = lJniEnv->GetStaticMethodID(lFileWriterClass, "WriteInt64", "(LfighterPilot/RisingSunAlpha/AndroidFileWriter;J)V");

	//Write String to file
	lJniEnv->CallStaticVoidMethod(lFileWriterClass, lWriteInt64Method, (jobject) iWriterFile, (jlong) aValue);
	//garbage collection
	lJniEnv->DeleteLocalRef(lFileWriterClass);
}

void CAndroidWriteStream::WriteUint64(const TUint64& aValue)
{
	WriteInt64(aValue);
}

void CAndroidWriteStream::WriteStringNative(const char* aString)
{
	//need reference to the JNI
	JNIEnv* lJniEnv = CAndroidFighterPilotThePacificWar::GetJNI();
	//class id
	jclass lFileWriterClass = lJniEnv->GetObjectClass(CAndroidWriteStream::JavaWriteStreamObject);
	//method ID to Write String
	jmethodID lWriteStringMethod = lJniEnv->GetStaticMethodID(lFileWriterClass, "WriteString", "(LfighterPilot/RisingSunAlpha/AndroidFileWriter;Ljava/lang/String;)V");

	//Write String to file
	jstring lJavaString = lJniEnv->NewStringUTF(aString);
	lJniEnv->CallStaticVoidMethod(lFileWriterClass, lWriteStringMethod, (jobject) iWriterFile, lJavaString);
	//garbage collection
	lJniEnv->DeleteLocalRef(lFileWriterClass);
	lJniEnv->DeleteLocalRef(lJavaString);
}


