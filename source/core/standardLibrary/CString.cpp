/*
 * CString.cpp
 *
 *  Created on: May 4, 2012
 *      Author: dstoll
 */
#include <string.h>
#include <stdlib.h>

#include "includes/core/standardLibrary/StandardLibrary.h"

CString::CString()
{
	iData = NULL;
	iCumulativeBytesForUTF8Characters = NULL;
	iLength = 0;
}

CString::CString(const char* aString)
{
	iData = NULL;
	iCumulativeBytesForUTF8Characters = NULL;
	iLength = 0;

	Append(aString);
}

CString::~CString()
{
	if(iData)
	{
		delete [] iData;
		iData = NULL;
	}
	if(iCumulativeBytesForUTF8Characters)
	{
		delete [] iCumulativeBytesForUTF8Characters;
		iCumulativeBytesForUTF8Characters = NULL;
	}
}

//----------------- functions -------------------------//

CString* CString::Append(const CString* aString)
{
	Append(aString->GetData());
	return this;
}

CString* CString::Append(const char* aString)
{
	TInt lBytesOfAppendString = 0;
	if(aString != NULL)
		lBytesOfAppendString += strlen(aString);
	else
		return this; //nothing to append
	TInt lBytesOfOldString = 0;
	if(iData != NULL)
		lBytesOfOldString += strlen(iData);

	//allocate enough space to hold the new string
	char* lNewData = new char[lBytesOfAppendString + lBytesOfOldString + 1]; //need 1 byte for the NULL terminating character

	//copy strings over
	if(iData != NULL)
		strcpy(lNewData, iData);
	//append the new String, since we append we need to add a offset
	strncpy(lNewData + lBytesOfOldString, aString, lBytesOfAppendString + 1);//1 byte for NULL terminating character

	//reset pointers and delete old String
	if(iData != NULL)
		delete[] iData;
	iData = lNewData;

	//update Length and the cumulative array
	UpdateLengthForAppend(aString);
	return this;
}

CString* CString::AppendUtf8Value(TUint32 aValue)
{
	char* lData = NULL;

	//find out how many bytes we have too allocate
	if((aValue >> 24) > 0) //4 bytes
	{
		lData = new char[5];
		lData [0] = (TUint8) (aValue >> 24);
		lData [1] = (TUint8) (aValue >> 16);
		lData [2] = (TUint8) (aValue >> 8);
		lData [3] = (TUint8) (aValue >> 0);
		lData [4] = '\0';
	}
	else if((aValue >> 16) > 0) //3 bytes
	{
		lData = new char[4];
		lData [0] = (TUint8) (aValue >> 16);
		lData [1] = (TUint8) (aValue >> 8);
		lData [2] = (TUint8) (aValue >> 0);
		lData [3] = '\0';
	}
	else if((aValue >> 8) > 0) //2 bytes
	{
		lData = new char[3];
		lData [0] = (TUint8) (aValue >> 8);
		lData [1] = (TUint8) (aValue >> 0);
		lData [2] = '\0';
	}
	else //1 bytes
	{
		lData = new char[2];
		lData [0] = (TUint8) (aValue >> 0);
		lData [1] = '\0';
	}

	Append(lData);
	delete[] lData;

	return this;
}

CString* CString::AppendNum(TInt64 aNumber)
{
	if(aNumber < 0) //negative
	{
		Append("-"); //prefix with a negative sign
		aNumber *= -1; //make positive
	}

	char* lNumberArray = new char [21]; // 2^64 has a max value of 1 * 10^19, thus 20 blocks should be enough, plus 1 for \0
	lNumberArray [20] = '\0'; //need to terminate the string
	TInt lCurrentIndex = 19; //we are Starting at the back of the array

	do
	{
		//get the last digit
		switch(aNumber % 10)
		{
			case 9:
			{
				lNumberArray [lCurrentIndex] = '9';
				break;
			}
			case 8:
			{
				lNumberArray [lCurrentIndex] = '8';
				break;
			}
			case 7:
			{
				lNumberArray [lCurrentIndex] = '7';
				break;
			}
			case 6:
			{
				lNumberArray [lCurrentIndex] = '6';
				break;
			}
			case 5:
			{
				lNumberArray [lCurrentIndex] = '5';
				break;
			}
			case 4:
			{
				lNumberArray [lCurrentIndex] = '4';
				break;
			}
			case 3:
			{
				lNumberArray [lCurrentIndex] = '3';
				break;
			}
			case 2:
			{
				lNumberArray [lCurrentIndex] = '2';
				break;
			}
			case 1:
			{
				lNumberArray [lCurrentIndex] = '1';
				break;
			}
			case 0:
			{
				lNumberArray [lCurrentIndex] = '0';
				break;
			}
			default:
			{
				lNumberArray [lCurrentIndex] = '?';//to signal that something went wrong
				break;
			}
		}

		//remove the last digit
		aNumber /= 10;
		lCurrentIndex--;

	}while(aNumber != 0);

	char* lAdjustArrayPointer = lNumberArray;
	lAdjustArrayPointer += lCurrentIndex + 1; //need to add 1 since lCurrentIndex always points to the next free block

	//now have the number in the correct order
	Append(lAdjustArrayPointer);

	//clean up
	delete[] lNumberArray;

	return this;
}

void CString::Clear()
{
	if(iData != NULL)
	{
		delete[] iData;
		iData = NULL;
	}
	if(iCumulativeBytesForUTF8Characters != NULL)
	{
		delete[] iCumulativeBytesForUTF8Characters;
		iCumulativeBytesForUTF8Characters = NULL;
	}
	iLength = 0;
}

TInt CString::CompareTo(const CString* aString) const
{
	size_t lLengthOfObjectString =  strlen(iData);
	//do a comparsion
	TInt lCompareValue = strncmp(iData, aString->iData, lLengthOfObjectString);

	if(lCompareValue == 0)
	{
		size_t lLengthOfArgString = strlen(aString->iData);

		//values are equals, but since strncmp does not compare the entire string if the lengths are different we need
		//to add another check just to make sure
		if(lLengthOfObjectString > lLengthOfArgString)
		{
			return 1; //owning object is a longer String
		}
		else if(lLengthOfObjectString < lLengthOfArgString)
		{
			return -1; // argument object is longer String
		}
		else
		{
			return 0; //both are the same length, thus both strings must be the same
		}
	}
	else
	{
		return lCompareValue; //strings are not equal, thus strncmp does the job just fine
	}
}

TInt64 CString::GetCharacterCodeAsUtf8(TInt lIndex) const
{
	TInt64 lCharacterCode = 0;

	if(lIndex < iLength) //make sure it is inside the string
	{
		//first find out how many bytes this character uses
		TInt lBytesUsedForCharacter = 1;
		if(lIndex != 0)
		{
			lBytesUsedForCharacter = iCumulativeBytesForUTF8Characters[lIndex] - iCumulativeBytesForUTF8Characters[lIndex - 1];
		}
		else
		{
			lBytesUsedForCharacter = iCumulativeBytesForUTF8Characters[lIndex];
		}

		//find out where in the data array the character is stored with its bytes
		TInt lCharacterDataStartIndex = iCumulativeBytesForUTF8Characters[lIndex] - lBytesUsedForCharacter;

		//extract the data from the array
		for(TInt lIndex = lCharacterDataStartIndex; lIndex < lCharacterDataStartIndex + lBytesUsedForCharacter; lIndex++)
		{
			//a string character in UTF-8 can have several bytes like Ab 20 11 (3 Bytes) or just 23 (1 Byte)
			//if the string character has only 1 byte, the value is just the value of its own byte
			//however, it gets more complicated if we have several bytes, since each addional byte is multiplied by 16^(SegmentIndex)

			TInt lBytePowerSegment = (lBytesUsedForCharacter - 1) - (lIndex - lCharacterDataStartIndex); //tells us at what location we are inside bytes of the string
			TInt lPowerValue = 2 * lBytePowerSegment;

			lCharacterCode += (iData[lIndex]) * CMath::Power(16, lPowerValue); //we assume that the front most byte is the most significant
		}
	}

	return lCharacterCode;
}


TInt64 CString::GetCharacterCodeAsUtf32(TInt lIndex) const
{
	TInt64 lCharacterCode = 0;

	if(lIndex < iLength) //make sure it is inside the string
	{
		//first find out how many bytes this character uses
		TInt lBytesUsedForCharacter = 1;
		if(lIndex != 0)
		{
			lBytesUsedForCharacter = iCumulativeBytesForUTF8Characters[lIndex] - iCumulativeBytesForUTF8Characters[lIndex - 1];
		}
		else
		{
			lBytesUsedForCharacter = iCumulativeBytesForUTF8Characters[lIndex];
		}

		//find out where in the data array the character is stored with its bytes
		TInt lCharacterDataStartIndex = iCumulativeBytesForUTF8Characters[lIndex] - lBytesUsedForCharacter;

		//since the string is stored in UTf-8 we need to convert its value to UTF-32
		//the following algorithm should do the trick

		//find out what the first byte is, depending on its value, we have more bytes following
		lCharacterCode = iData[lCharacterDataStartIndex];

		if(lCharacterCode >= 0x80)
		{
			TInt lAdditionalBytes = 0;
			char* lSequencePointer = iData + lCharacterDataStartIndex;
			lSequencePointer++;//need to increment it by 1, since we want to skip the front most byte

			if(lCharacterCode < 0xe0)
			{
				lAdditionalBytes = 1;
				lCharacterCode &= 0x1f;
			}
			else if(lCharacterCode < 0xf0)
			{
				lAdditionalBytes = 2;
				lCharacterCode &= 0x0f;
			}
			else
			{
				lAdditionalBytes = 3;
				lCharacterCode &= 0x07;
			}

			//need to add the values of the extra bytes
			for(TInt lIndex = 0; lIndex < lAdditionalBytes; lIndex++)
			{
				lCharacterCode = (lCharacterCode << 6) | (lSequencePointer[0] & 0x3f);
				lSequencePointer++;
			}
		}
	}

	return lCharacterCode;
}

TBool CString::EqualTo(const char* aString)
{
	TBool lAreTheSame;
	CString* lTempString = new CString(aString);
	lAreTheSame = EqualTo(lTempString);
	delete lTempString; //not needed anymore

	return lAreTheSame;
}

TBool CString::EqualTo(const CString* aString) const
{
	size_t lLengthOfObjectString =  strlen(iData);

	//first can do a quick check if they have the same length
	if(lLengthOfObjectString == strlen(aString->iData))
	{
		if(strncmp(iData, aString->iData, lLengthOfObjectString) == 0)
		{
			return true; //string content is exactly the same
		}
	}

	return false;
}

void CString::UpdateLengthForAppend(const char* aString)
{
	TInt lLengthOfNewStringUTF8 = 0;
	const char* aTempString = aString;

	//we need to find the length of the UTF-8 String before we can allocate enough memory for the cumulative array
	while(aTempString[0] != '\0') // \0 stands for the null terminating character
	{
		TInt lValueOfFirstByte = (TInt) aTempString[0];

		//need to inspect the first byte to see how many bytes the UTF-8 character needs
		if((lValueOfFirstByte & 240) == 240) // this is 1111 0000 in binary
		{
			//uses 4 bytes
			aTempString += 4;
		}
		else if((lValueOfFirstByte & 224) == 224) // this is 1110 0000 in binary
		{
			//uses 3 bytes
			aTempString += 3;
		}
		else if((lValueOfFirstByte & 192) == 192) // this is 1100 0000 in binary
		{
			//uses 2 bytes
			aTempString += 2;
		}
		else //has to use 1 byte
		{
			aTempString += 1;
		}

		lLengthOfNewStringUTF8++;
	}

	TInt* lNewCumulativeBytes = new TInt[iLength + lLengthOfNewStringUTF8];
	TInt lCumulativeArrayIndex;

	//copy over the old values first
	for(lCumulativeArrayIndex = 0; lCumulativeArrayIndex < iLength; lCumulativeArrayIndex++)
	{
		lNewCumulativeBytes [lCumulativeArrayIndex] = iCumulativeBytesForUTF8Characters[lCumulativeArrayIndex];
	}

	//now need to add the new data
	aTempString = aString; //reset pointer

	while(aTempString[0] != '\0')
	{
		TInt lValueOfFirstByte = (TInt) aTempString[0];
		TInt lCumulativeValueOfPreviousIndex = 0;
		if(lCumulativeArrayIndex != 0)
			lCumulativeValueOfPreviousIndex = lNewCumulativeBytes[lCumulativeArrayIndex - 1];
		else
			lCumulativeValueOfPreviousIndex = 0;

		//need to inspect the first byte to see how many bytes the UTF-8 character needs
		if((lValueOfFirstByte & 240) == 240) // this is 1111 0000 in binary
		{
			//uses 4 bytes
			aTempString += 4;
			lNewCumulativeBytes[lCumulativeArrayIndex] = lCumulativeValueOfPreviousIndex + 4;
		}
		else if((lValueOfFirstByte & 224) == 224) // this is 1110 0000 in binary
		{
			//uses 3 bytes
			aTempString += 3;
			lNewCumulativeBytes[lCumulativeArrayIndex] = lCumulativeValueOfPreviousIndex + 3;
		}
		else if((lValueOfFirstByte & 192) == 192) // this is 1100 0000 in binary
		{
			//uses 2 bytes
			aTempString += 2;
			lNewCumulativeBytes[lCumulativeArrayIndex] = lCumulativeValueOfPreviousIndex + 2;
		}
		else //has to use 1 byte
		{
			aTempString += 1;
			lNewCumulativeBytes[lCumulativeArrayIndex] = lCumulativeValueOfPreviousIndex + 1;
		}

		lCumulativeArrayIndex++;
	}

	//all done, just some minor stuff left

	//update length
	iLength += lLengthOfNewStringUTF8;

	//update Array
	if(iCumulativeBytesForUTF8Characters != NULL)
		delete[] iCumulativeBytesForUTF8Characters;
	iCumulativeBytesForUTF8Characters = lNewCumulativeBytes;
}

const char* CString::GetData() const
{
	return iData;
}

TInt CString::GetLength() const
{
	return iLength;
}

TUint CString::GetSizeOfStringInBytes()
{
	return iCumulativeBytesForUTF8Characters[iLength - 1];
}

TInt64 CString::GetStringAsInt()
{
	if(iLength <= 0) //just to be save
		return 0;

	TInt64 lExtractedNumber = 0;
	TInt lSignOfNumber = 1;
	TInt lStartIndex = 0; //need to skip sign if it exists
	TInt lDecimalMultiplier = 0;

	//find out sign of Number
	CString* lSignCharacter = GetSubString(0, 1);
	if(lSignCharacter->EqualTo("-"))
	{
		lSignOfNumber = -1;
		lStartIndex = 1;
	}
	delete lSignCharacter; //not needed anymore

	//need to find out the Decimal Multiplier for the first digit and so on
	lDecimalMultiplier = iLength - lStartIndex;
	lDecimalMultiplier -= 1; //need to remove first index, since a DecimalMultiplier of 1, is just 1

	for(TInt lIndex = lStartIndex; lIndex < iLength; lIndex++)
	{
		CString* lNumber = GetSubString(lIndex, 1);
		TInt64 lNumberAsInt = 0;

		if(lNumber->EqualTo("1"))
		{
			lNumberAsInt = 1;
		}
		else if(lNumber->EqualTo("2"))
		{
			lNumberAsInt = 2;
		}
		else if(lNumber->EqualTo("3"))
		{
			lNumberAsInt = 3;
		}
		else if(lNumber->EqualTo("4"))
		{
			lNumberAsInt = 4;
		}
		else if(lNumber->EqualTo("5"))
		{
			lNumberAsInt = 5;
		}
		else if(lNumber->EqualTo("6"))
		{
			lNumberAsInt = 6;
		}
		else if(lNumber->EqualTo("7"))
		{
			lNumberAsInt = 7;
		}
		else if(lNumber->EqualTo("8"))
		{
			lNumberAsInt = 8;
		}
		else if(lNumber->EqualTo("9"))
		{
			lNumberAsInt = 9;
		}
		else //has to be 0
		{
			lNumberAsInt = 0;
		}

		lExtractedNumber += lNumberAsInt * CMath::Power(10, lDecimalMultiplier);
		lDecimalMultiplier--; //should always be between 0....+
		delete lNumber;
	}

	//just need to adjust number for sign
	return lExtractedNumber * lSignOfNumber;
}

CString* CString::GetSubString(TUint aBeginIndex, TInt aLength) const
{
	//return NULL if requested copy is outside String boundaries
	if(aBeginIndex < 0 || aLength <= 0 || (aBeginIndex + aLength) > iLength)
		return NULL;

	//find out how many bytes the String Needs
	TInt lBytesForSubstring = iCumulativeBytesForUTF8Characters[aBeginIndex + (aLength - 1)];
	TInt lBytesBeforeIndex = 0;
	if(aBeginIndex != 0)
		lBytesBeforeIndex = iCumulativeBytesForUTF8Characters[aBeginIndex - 1]; //get bytes that are before Index
	lBytesForSubstring -= lBytesBeforeIndex;

	//copy over bytes
	char* lSubStringData = new char[lBytesForSubstring + 1]; //need 1 extra Byte for the NULL terminating character
	size_t lSizeOfCharacter = sizeof(char);
	strncpy(lSubStringData, iData + lBytesBeforeIndex, lSizeOfCharacter * lBytesForSubstring);

	//need to add NULL terminate character the String
	lSubStringData[lBytesForSubstring] = '\0';

	//create a String to hold it
	CString* lSubString = new CString(lSubStringData);
	delete[] lSubStringData; //we aleady made a copy of it

	return lSubString;
}

TInt CString::FindString(TUint aBeginIndex, const char* aString) const
{
	TInt lFoundIndex = -1;
	CString* lFindString = new CString(aString);

	//use two loops one for the Main String, and the inside loop to iterate through the Find String
	for(TInt lIndexOuter = aBeginIndex; lIndexOuter < iLength; lIndexOuter++)
	{
		TBool lMatchFound = false;
		TInt lCharactersLeftInMainStringToCompare = iLength - (lIndexOuter);

		//check if the main String has enough character left to actually match the String we are looking for
		if(lCharactersLeftInMainStringToCompare < lFindString->GetLength())
		{
			lFoundIndex = -1;
			break;
		}

		for(TInt lIndexInner = 0; lIndexInner < lFindString->GetLength(); lIndexInner++)
		{
			TInt64 lMainCharacterValue = GetCharacterCodeAsUtf32(lIndexOuter + lIndexInner);
			TInt64 lFindCharacterValue = lFindString->GetCharacterCodeAsUtf32(lIndexInner);

			if(lMainCharacterValue == lFindCharacterValue)
			{
				lMatchFound = true;
			}
			else
			{
				lMatchFound = false;
			}

			if(!lMatchFound)
			{
				//can't be equal
				break;
			}
		}

		if(lMatchFound)
		{
			lFoundIndex = lIndexOuter;
			break;
		}
	}

	//clean up
	delete lFindString;

	return lFoundIndex;
}

void CString::Replace(const char* aString)
{
	Clear();
	Append(aString);
}

void CString::Replace(const CString* aString)
{
	Replace(aString->GetData());
}

void CString::ReplaceAndDelete(CString* aString)
{
	Replace(aString);
	delete aString;
}

