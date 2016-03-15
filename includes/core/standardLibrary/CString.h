/*
 * CString.h
 *
 *  Created on: May 4, 2012
 *      Author: dstoll
 */

// A Basic String class that should have all the functions that you would expect from a String object

#ifndef CSTRING_H_
#define CSTRING_H_

#include "includes/core/standardLibrary/BasicTypes.h"

class CString
{
public:

	CString();
	CString(const char* aString);
	virtual ~CString();

	CString* Append(const CString* aString);
	CString* Append(const char* aString);
	//converts the the integer value to a utf-8 character and appends it to the string
	CString* AppendUtf8Value(TUint32 aValue);
	CString* AppendNum(TInt64 aNumber);
	//deletes all content of the String
	void Clear();
	//returns 0 if both Strings are equal, a value greater than 0 if owning object comes Lexicographically later then the Argument and less than 0 if the opposite is true
	TInt CompareTo(const CString* aString) const;
	//returns true if the bytes in both Strings are the same
	TBool EqualTo(const char* aString);
	//return true if both Strings are exactly the same
	TBool EqualTo(const CString* aString) const;
	//returns the Utf-8 character code, index is used to select which character to choose in the string
	TInt64 GetCharacterCodeAsUtf8(TInt lIndex) const;
	//returns the Utf-32 character code, index is used to select which character to choose in the string
	TInt64 GetCharacterCodeAsUtf32(TInt lIndex) const;
	//get the raw character array
	const char* GetData() const;
	//returns number of Characters in the String, independent of bytes
	TInt GetLength() const;
	//returns the Size of the Strings Data in Number of Bytes
	TUint GetSizeOfStringInBytes();
	//converts the String to a TInt64 integer, if the String is not a decimal number the result is not defined but most often it will just return 0
	TInt64 GetStringAsInt();
	//returns a SubString that starts at BeginIndex and copies over as many characters as Length specifies
	CString* GetSubString(TUint aBeginIndex, TInt aLength) const;
	//Finds the Index of the String if it is contained in the String(occurence of first character), returns -1 if it can't find it
	//skips all indexes in the String that are before BeginIndex
	TInt FindString(TUint aBeginIndex, const char* aString) const;
	//replaces the Strings content with the new String
	void Replace(const char* aString);
	void Replace(const CString* aString);
	//also deletes the arugment after the call
	void ReplaceAndDelete(CString* aString);

private:

	void UpdateLengthForAppend(const char* aString); //updates the Length fields with new string
	char* iData;
	TInt* iCumulativeBytesForUTF8Characters; //keeps track how many bytes a particular character uses
	TInt iLength; //length of the String, independent on how many bytes the string uses
};

#endif /* CSTRING_H_ */
