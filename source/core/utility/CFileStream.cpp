/*
 * CFileStrean.cpp
 *
 *  Created on: May 4, 2012
 *      Author: dstoll
 */

#define ENIGMA_NUMBER_OF_DISKS 4

#include "includes/core/utility/CFileStream.h"
#include "includes/core/cryptography/CCryptographyEngima.h"

//------------------------------- READ STREAM ----------------------------------//

CFileReadStream::CFileReadStream()
{

}

CFileReadStream::~CFileReadStream()
{

}

TIntFloat CFileReadStream::ReadIntFloat()
{
	TIntFloat lIntFloat;
	lIntFloat.AssignIntAlreadyInBaseIntFloat(ReadInt32());
	return lIntFloat;
}

TIntFloat64 CFileReadStream::ReadIntFloat64()
{
	TIntFloat64 lIntFloat64;
	lIntFloat64.AssignIntAlreadyInBaseIntFloat64(ReadInt64());
	return lIntFloat64;
}

void CFileReadStream::ReadString(CString*& aString)
{
	ReadStringNative(aString);
}

TInt CFileReadStream::ReadInt32Encrypted(TInt aValueIfChecksumFailed)
{
	TInt lDecodedValue = 0;
	CCryptographyEngima* lEngima = CCryptographyEngima::New(ENIGMA_NUMBER_OF_DISKS);
	CPointerArray<TInt>* lEncodedValues = CPointerArray<TInt>::New();
	CPointerArray<TInt>* lDecodedValues = NULL;

	lEncodedValues->Append(new TInt(ReadInt32())); //Offset
	lEncodedValues->Append(new TInt(ReadInt32())); //actual value
	lEncodedValues->Append(new TInt(ReadInt32())); //checksum

	//decode
	lDecodedValues = lEngima->DecodeData(*lEncodedValues);

	//make sure nobody tempered with the actual value
	if( (*lDecodedValues->Get(0)) == (*lDecodedValues->Get(1)) )
	{
		//same as checksum, must be good
		lDecodedValue = *lDecodedValues->Get(0);
	}
	else
	{
		//value might have been changed since checksum doesn't match anymore
		lDecodedValue = aValueIfChecksumFailed;
	}

	//clean up
	lEncodedValues->ClearAndDestroy();
	lDecodedValues->ClearAndDestroy();
	delete lEngima;
	delete lEncodedValues;
	delete lDecodedValues;

	return lDecodedValue;
}

TInt64 CFileReadStream::ReadInt64Encrypted()
{
	return ReadInt64();
}

void CFileReadStream::ReadStringEncrypted(CString*& aString, const char* aStringIfChecksumFailed)
{
	TInt lChecksum = 0;
	CCryptographyEngima* lEngima = CCryptographyEngima::New(ENIGMA_NUMBER_OF_DISKS);
	CPointerArray<TInt>* lEncodedValues = CPointerArray<TInt>::New();
	CPointerArray<TInt>* lDecodedValues = NULL;
	if(aString == NULL)
	{
		aString = new CString("");
	}
	aString->Replace(""); //clear String just to be save

	//need to find out how long the encoded String is
	TInt lNumberOfValuesStoredForString = ReadInt32();

	if(lNumberOfValuesStoredForString <= 0)
	{
		//means String has either not been saved or has zero length
		return;
	}

	//read encoded values from disk
	for(TInt lIndex = 0; lIndex < lNumberOfValuesStoredForString; lIndex++)
	{
		TInt lEncodedValue = ReadInt32();
		lEncodedValues->Append(new TInt(lEncodedValue));
	}

	//decode
	lDecodedValues = lEngima->DecodeData(*lEncodedValues);

	//recreate String sum up the checksum
	for(TInt lIndex = 0; lIndex < lDecodedValues->GetCount() - 1; lIndex++) // -1 because we don't want to include the checksum
	{
		TInt lUtf8ValueOfCharacter = *lDecodedValues->Get(lIndex);
		aString->AppendUtf8Value(lUtf8ValueOfCharacter);
		lChecksum += lUtf8ValueOfCharacter;
	}

	//check if checksum is valid
	if(lChecksum != *lDecodedValues->Get(lDecodedValues->GetCount() - 1))
	{
		//not equal
		aString->Replace(aStringIfChecksumFailed);
	}

	//clean up
	lEncodedValues->ClearAndDestroy();
	lDecodedValues->ClearAndDestroy();
	delete lEngima;
	delete lEncodedValues;
	delete lDecodedValues;
}

//------------------------------- WRITE STREAM ----------------------------------//

CFileWriteStream::CFileWriteStream()
{

}

CFileWriteStream::~CFileWriteStream()
{

}

void CFileWriteStream::WriteIntFloat(const TIntFloat& aValue)
{
	WriteInt32(aValue.GetIntInBaseIntFloat());
}

void CFileWriteStream::WriteIntFloat64(const TIntFloat64& aValue)
{
	WriteInt64(aValue.GetIntInBaseIntFloat64());
}

void CFileWriteStream::WriteString(const char* aString)
{
	WriteStringNative(aString);
}

void CFileWriteStream::WriteString(CString* aString)
{
	WriteStringNative(aString->GetData());
}

void CFileWriteStream::WriteInt32Encrypted(const TInt& aValue)
{
	TInt lChecksum = aValue;
	CCryptographyEngima* lEngima = CCryptographyEngima::New(ENIGMA_NUMBER_OF_DISKS);
	CPointerArray<TInt>* lIntAndChecksum = CPointerArray<TInt>::New();
	CPointerArray<TInt>* lEncodedValues = NULL;

	lIntAndChecksum->Append(new TInt(aValue));
	lIntAndChecksum->Append(new TInt(lChecksum)); //exactly the same value, but since it is encoded with the engima its code value will be different

	//encode
	lEncodedValues = lEngima->EncodeData(*lIntAndChecksum);

	//write encoded values to disk
	WriteInt32(*lEncodedValues->Get(0)); //Offset
	WriteInt32(*lEncodedValues->Get(1)); //Value
	WriteInt32(*lEncodedValues->Get(2)); //Checksum

	//clean up
	lIntAndChecksum->ClearAndDestroy();
	lEncodedValues->ClearAndDestroy();
	delete lEngima;
	delete lIntAndChecksum;
	delete lEncodedValues;
}

void CFileWriteStream::WriteInt64Encrypted(const TInt64& aValue)
{
	//don't do anything for now
	WriteInt64(aValue);
}

void CFileWriteStream::WriteStringEncrypted(CString* aString)
{
	TInt lChecksum = 0;
	CCryptographyEngima* lEngima = CCryptographyEngima::New(ENIGMA_NUMBER_OF_DISKS);
	CPointerArray<TInt>* lUtf8ValuesOfString = CPointerArray<TInt>::New();
	CPointerArray<TInt>* lEncodedValues = NULL;

	for(TInt lIndex = 0; lIndex < aString->GetLength(); lIndex++)
	{
		TInt64 lUtf8Value = aString->GetCharacterCodeAsUtf8(lIndex);
		lUtf8ValuesOfString->Append(new TInt(lUtf8Value));
		lChecksum += lUtf8Value;
	}
	//append Checksum
	lUtf8ValuesOfString->Append(new TInt(lChecksum));

	//encode values
	lEncodedValues = lEngima->EncodeData(*lUtf8ValuesOfString);

	//write out data values to disk
	WriteInt32(lEncodedValues->GetCount()); //first need to write down how many values we will write, so we know how many reads we need to do
	//write out encoded data
	for(TInt lIndex = 0; lIndex < lEncodedValues->GetCount(); lIndex++)
	{
		WriteInt32(*lEncodedValues->Get(lIndex));
	}

	//all done, clean up
	lUtf8ValuesOfString->ClearAndDestroy();
	lEncodedValues->ClearAndDestroy();
	delete lEngima;
	delete lUtf8ValuesOfString;
	delete lEncodedValues;
}



