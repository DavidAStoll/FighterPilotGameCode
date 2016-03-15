/*
 * CLocalizationEngine.cpp
 *
 *  Created on: Jul 18, 2012
 *      Author: dstoll
 */
#include "includes/GlobalDefines.h"
#include "includes/core/utility/CLocalizationEngine.h"
#include "includes/core/utility/CAssetDirectory.h"

#define EXTENSION_FOR_ENGLISH ".english"
#define EXTENSION_FOR_GERMAN ".german"
#define EXTENSION_FOR_FRENCH ".french"
#define EXTENSION_FOR_CHINSIM ".chinsim" //chinese simplified
#define EXTENSION_FOR_CHINTRA ".chintra" //chinese traditional
#define EXTENSION_FOR_UNLOCALIZED ".unlocalized"

CLocalizationEngine::CLocalizationEngine(const char* aPathToLocalizationDirectoy)
{
	iPathToLocalizationDirectoy = new CString(aPathToLocalizationDirectoy);
	iState = EStateReady;
	iLanguage = ELanguageEnglish;
	iFileIndex = -1; //since we need substract 1 to get to the real index
	iCategoryIndex = -1; //since we need substract 1 to get to the real index
	iStringIndex = -1; //since we need substract 1 to get to the real index
	iByteLocationInFile = 0;
}

CLocalizationEngine::~CLocalizationEngine()
{
	if(iPathToLocalizationDirectoy != NULL)
	{
		delete iPathToLocalizationDirectoy;
		iPathToLocalizationDirectoy = NULL;
	}

	if(iFileEntries != NULL)
	{
		//destroy indexes and category pointer arrays, and file name
		for(TInt lFileIndex = 0; lFileIndex < iFileEntries->GetCount(); lFileIndex++)
		{
			DeleteIndexFromFile(iFileEntries->Get(lFileIndex)->FileName);

			delete iFileEntries->Get(lFileIndex)->iCategoryEntries;
			delete iFileEntries->Get(lFileIndex)->FileName;
		}

		iFileEntries->ClearAndDestroy();
		delete iFileEntries;
	}
}

CLocalizationEngine* CLocalizationEngine::New(const char* aPathToLocalizationDirectoy)
{
	CLocalizationEngine* lSelf = new CLocalizationEngine(aPathToLocalizationDirectoy);
	lSelf->Construct();
	return lSelf;
}

void CLocalizationEngine::Construct()
{
	//create array to hold indexes
	iFileEntries = CPointerArray<SFileEntry>::New();
}

//------------------------ functions ----------------------------//

void CLocalizationEngine::AddLocalizationFile(const char* aFileName)
{
	//add new File
	SFileEntry lNewFileEntry;
	lNewFileEntry.FileName = new CString(aFileName);
	lNewFileEntry.IsLocalizedFile = true;
	lNewFileEntry.iCategoryEntries = CPointerArray<SCategoryEntry>::New();
	iFileEntries->Append(new SFileEntry(lNewFileEntry));

	BuildIndexForLanguageFile(lNewFileEntry.FileName);
}

void CLocalizationEngine::AddUnlocalizedFile(const char* aFileName)
{
	//add new File
	SFileEntry lNewFileEntry;
	lNewFileEntry.FileName = new CString(aFileName);
	lNewFileEntry.IsLocalizedFile = false;
	lNewFileEntry.iCategoryEntries = CPointerArray<SCategoryEntry>::New();
	iFileEntries->Append(new SFileEntry(lNewFileEntry));

	BuildIndexForLanguageFile(lNewFileEntry.FileName);
}

TLanguage CLocalizationEngine::GetLanguage()
{
	return iLanguage;
}

CString* CLocalizationEngine::GetString(const char* aId)
{
	CString* lLocalizedString = new CString(aId); //if String can't be found, use the indentifier for the String
	CString* lStringId = new CString(aId);
	CString* lFileName;
	CString* lCategoryName;
	CString* lStringName;
	TInt lStartIndex = 0;
	TInt lDotIndex = 0;

	//Find FileName
	lDotIndex = lStringId->FindString(lStartIndex,"."); //find location of seperator
	if(lDotIndex == -1)
	{
		lLocalizedString->Replace("ID FILENAME ERROR!");

		//clean up
		delete lStringId;

		return lLocalizedString;
	}
	else
	{
		lFileName = lStringId->GetSubString(lStartIndex,lDotIndex);
		lStartIndex = lDotIndex + 1; //skip dot
	}

	//Find CategoryName
	lDotIndex = lStringId->FindString(lStartIndex,"."); //find location of seperator
	if(lDotIndex == -1)
	{
		lLocalizedString->Replace("ID CATEGORYNAME ERROR!");

		//clean up
		delete lStringId;
		delete lFileName;

		return lLocalizedString;
	}
	else
	{
		lCategoryName = lStringId->GetSubString(lStartIndex,lDotIndex - lStartIndex);
		lStartIndex = lDotIndex + 1; //skip dot
	}

	//get StringName
	lStringName = lStringId->GetSubString(lStartIndex, lStringId->GetLength() - lStartIndex); //cut the last part of the String that remains
	if(lStringName == NULL)
	{
		lLocalizedString->Replace("ID STRINGNAME ERROR!");

		//clean up
		delete lStringId;
		delete lFileName;
		delete lCategoryName;

		return lLocalizedString;
	}

	//call method to handle the actual lookup
	GetStringFromFile(lFileName, lCategoryName, lStringName, lLocalizedString);

	//clean up
	delete lStringId;
	delete lFileName;
	delete lCategoryName;
	delete lStringName;

	return lLocalizedString;
}

CString* CLocalizationEngine::GetString(const CString* aId)
{
	return GetString(aId->GetData());
}

void CLocalizationEngine::SetLanguage(TLanguage aLanguage)
{
	iLanguage = aLanguage;

	//create Index on the new Language files
	for(TInt lFileIndex = 0; lFileIndex < iFileEntries->GetCount(); lFileIndex++)
	{
		DeleteIndexFromFile(iFileEntries->Get(lFileIndex)->FileName);

		BuildIndexForLanguageFile(iFileEntries->Get(lFileIndex)->FileName);
	}
}

void CLocalizationEngine::BuildIndexForLanguageFile(CString* aFileName)
{
	//find FileIndex and reset other attributes
	iFileIndex = -1;
	for(TInt lFileIndex = 0; lFileIndex < iFileEntries->GetCount(); lFileIndex++)
	{
		if(iFileEntries->Get(lFileIndex)->FileName->EqualTo(aFileName))
		{
			iFileIndex = lFileIndex;
			break;
		}
	}
	//reset state to be ready for reading data
	iState = EStateReady;
	iCategoryIndex = -1;
	iStringIndex = -1;
	iByteLocationInFile = 0;

	CString* lFullPath = GetFullPathForFileName(aFileName->GetData());
	TInt lFileContentSize = CAssetDirectory::GetAssetFileSizeInBytes(lFullPath->GetData());
	TUint8* lFileContent = CAssetDirectory::ReadAssetFile(lFullPath->GetData());
	//need to expend the memory block by 1 byte to append the NULL character
	lFileContent = (TUint8*) realloc(lFileContent, sizeof(TUint8) * (lFileContentSize + 1));
	lFileContent[lFileContentSize] = '\0';
	//convert the file content to a CString
	CString* lFileContentAsString = new CString((char*) lFileContent);
	//clean up
	delete[] lFileContent; //made a copy of it
	delete lFullPath;

	//go through the file and build up an index to the String Values
	for (TInt lIndex = 0; lIndex < lFileContentAsString->GetLength(); lIndex++)
	{
		CString* lCharacter = lFileContentAsString->GetSubString(lIndex, 1);

		switch (iState)
		{
			case EStateReady:
			{
				HandleReadyState(lCharacter);
				break;
			}
			case EStateLookingForEndOfCategoryName:
			{
				HandleEndOfCategoryNameState(lCharacter);
				break;
			}
			case EStateLookingForEndOfStringId:
			{
				HandleEndOfStringId(lCharacter);
				break;
			}
			case EStateLookingForBeginningOfStringValue:
			{
				HandleBeginningOfStringValue(lCharacter);
				break;
			}
			case EStateLookingForEndOfStringValue:
			{
				HandleEndOfStringValue(lCharacter);
				break;
			}
			case EStateComment:
			{
				HandleCommentState(lCharacter);
				break;
			}
		}

		iByteLocationInFile += lCharacter->GetSizeOfStringInBytes(); //need increment the byte Location
		delete lCharacter; //not needed anymore
	}

	//not needed anymore
	delete lFileContentAsString;
}

void CLocalizationEngine::DeleteIndexFromFile(CString* aFileName)
{
	//delete old indexes
	for(TInt lFileIndex = 0; lFileIndex < iFileEntries->GetCount(); lFileIndex++)
	{
		if(iFileEntries->Get(lFileIndex)->FileName->EqualTo(aFileName)) //found correct file
		{
			CPointerArray<SCategoryEntry>* lCategoryEntries = iFileEntries->Get(lFileIndex)->iCategoryEntries;

			for(TInt lCategoryIndex = 0; lCategoryIndex < lCategoryEntries->GetCount(); lCategoryIndex++)
			{
				SCategoryEntry* lCategory = lCategoryEntries->Get(lCategoryIndex);

				for(TInt lStringIndex = 0; lStringIndex < lCategory->StringEntries->GetCount(); lStringIndex++)
				{
					delete lCategory->StringEntries->Get(lStringIndex)->StringId;
					lCategory->StringEntries->Get(lStringIndex)->StringId = NULL;
				}

				//remove string entries in category
				lCategory->StringEntries->ClearAndDestroy();
				delete lCategory->StringEntries;
				//remove category name
				delete lCategory->CategoryName;
			}

			//reset all categories
			lCategoryEntries->ClearAndDestroy();
		}
	}
}

CString*  CLocalizationEngine::GetFileExtensionForLanguage(TLanguage aLanguage)
{
	switch(aLanguage)
	{
		case ELanguageEnglish:
		{
			return new CString(EXTENSION_FOR_ENGLISH);
		}
		case ELanguageGerman:
		{
			return new CString(EXTENSION_FOR_GERMAN);
		}
		case ELanguageFrench:
		{
			return new CString(EXTENSION_FOR_FRENCH);
		}
		case ELanguageChineseSimplified:
		{
			return new CString(EXTENSION_FOR_CHINSIM);
		}
		case ELanguageChineseTraditional:
		{
			return new CString(EXTENSION_FOR_CHINTRA);
		}
	}
}

CString* CLocalizationEngine::GetFullPathForFileName(const char* aFileName)
{
	CString* lFullPath = NULL;
	CString* lLanguageExtension = NULL;
	TBool lIsLocalizedFile = false;

	//need to check if it is a localized file
	for(TInt lFileIndex = 0; lFileIndex < iFileEntries->GetCount(); lFileIndex++)
	{
		if(iFileEntries->Get(lFileIndex)->FileName->EqualTo(aFileName))
		{
			lIsLocalizedFile = iFileEntries->Get(lFileIndex)->IsLocalizedFile;
			break;
		}
	}

	if(lIsLocalizedFile)
	{
		lLanguageExtension = GetFileExtensionForLanguage(iLanguage);
	}
	else
	{
		lLanguageExtension = new CString(EXTENSION_FOR_UNLOCALIZED);
	}

	lFullPath = new CString(iPathToLocalizationDirectoy->GetData());
	lFullPath->Append(aFileName);
	lFullPath->Append(lLanguageExtension);
	lFullPath->Append(".mp3"); //to avoid compression of the file, we needed to add .mp3 on Android

	delete lLanguageExtension;

	return lFullPath;
}

void CLocalizationEngine::GetStringFromFile(CString* aFileName, CString* aCategoryName, CString* aStringName, CString* aToHoldLocalizedString)
{
	for(TInt lFileIndex = 0; lFileIndex < iFileEntries->GetCount(); lFileIndex++)
	{
		if(iFileEntries->Get(lFileIndex)->FileName->EqualTo(aFileName))
		{
			CPointerArray<SCategoryEntry>* lCategory = iFileEntries->Get(lFileIndex)->iCategoryEntries;

			for(TInt lCategoryIndex = 0; lCategoryIndex < lCategory->GetCount(); lCategoryIndex++)
			{
				if(lCategory->Get(lCategoryIndex)->CategoryName->EqualTo(aCategoryName))
				{
					//found the correct category
					CPointerArray<SStringEntry>* lStringEntries = lCategory->Get(lCategoryIndex)->StringEntries;

					for(TInt lStringIndex = 0; lStringIndex < lStringEntries->GetCount(); lStringIndex++)
					{
						if(lStringEntries->Get(lStringIndex)->StringId->EqualTo(aStringName))
						{
							//need to convert the Filename to work on platform and with the correct language
							CString* lCompleteFileName = GetFullPathForFileName(aFileName->GetData());

							//found the String that we need
							TUint lLengthInBytes = lStringEntries->Get(lStringIndex)->LengthOfStringInBytes;
							TUint lOffSetInBytes = lStringEntries->Get(lStringIndex)->ByteOffset;
							TUint8* lRawData = CAssetDirectory::ReadBytesFromAssetFile(lCompleteFileName->GetData(), lLengthInBytes, lOffSetInBytes);
							TUint8* lStringData = new TUint8[lLengthInBytes + 1]; //plus 1 for NULL terminating character

							//need to append the NULL terminating character to this String
							memcpy(lStringData, lRawData, lLengthInBytes);
							lStringData [lLengthInBytes] = '\0';

							//create the final String
							aToHoldLocalizedString->Replace((char*) lStringData);
							delete[] lRawData;
							delete[] lStringData;
							delete lCompleteFileName;

							return; //found what we wanted
						}
					}
				}
			}
		}
	}
}

void CLocalizationEngine::HandleCommentState(CString* aNewCharacter)
{
	if(aNewCharacter->EqualTo("\n"))
	{
		//found the end of the comment
		iState = EStateReady;
	}
}

void CLocalizationEngine::HandleEndOfCategoryNameState(CString* aNewCharacter)
{
	if(aNewCharacter->EqualTo("]"))
	{
		//found the end of the category
		iState = EStateReady;

		if(iFileEntries->Get(iFileIndex)->iCategoryEntries->Get(iCategoryIndex)->CategoryName->EqualTo("CampaignBriefingAndDebriefing"))
		{
			TInt Ltest0 = 0;
			Ltest0 = 1;
		}
	}
	else
	{
		iFileEntries->Get(iFileIndex)->iCategoryEntries->Get(iCategoryIndex)->CategoryName->Append(aNewCharacter);
	}
}

void CLocalizationEngine::HandleReadyState(CString* aNewCharacter)
{
	//check if we either add a new category or a new String
	if(aNewCharacter->EqualTo("["))
	{
		//going into a new category
		iState = EStateLookingForEndOfCategoryName;

		//allocate a new category
		SCategoryEntry lNewCategory;
		lNewCategory.CategoryName = new CString("");
		lNewCategory.StringEntries = CPointerArray<SStringEntry>::New();
		iFileEntries->Get(iFileIndex)->iCategoryEntries->Append(new SCategoryEntry(lNewCategory));
		//increment counter to use this new category
		iCategoryIndex++;
		iStringIndex = -1; //since we start a new category the StringIndex has to start from the beginning again
	}
	else if(aNewCharacter->EqualTo("#"))
	{
		iState = EStateComment;
	}
	else if(!aNewCharacter->EqualTo(" ") && !aNewCharacter->EqualTo("\n") && !aNewCharacter->EqualTo("\0") && !aNewCharacter->EqualTo("\t"))
	{
		//since it is not equal to any of those, it must be a new String Id
		iState = EStateLookingForEndOfStringId;

		//allocate new StringEntry
		SStringEntry lNewStringEntry;
		lNewStringEntry.ByteOffset = 0;
		lNewStringEntry.LengthOfStringInBytes = 0;
		lNewStringEntry.StringId = new CString("");
		//increment counter to use this new String
		iStringIndex++;

		iFileEntries->Get(iFileIndex)->iCategoryEntries->Get(iCategoryIndex)->StringEntries->Append(new SStringEntry(lNewStringEntry));
		HandleEndOfStringId(aNewCharacter); //need to pass it down as well otherwise the first character gets always cut
	}
}

void CLocalizationEngine::HandleBeginningOfStringValue(CString* aNewCharacter)
{
	if(aNewCharacter->EqualTo("\"")) //skip everything until we find the first "
	{
		//found the beginning of the String Value
		iState = EStateLookingForEndOfStringValue;
		iFileEntries->Get(iFileIndex)->iCategoryEntries->Get(iCategoryIndex)->StringEntries->Get(iStringIndex)->ByteOffset = iByteLocationInFile + aNewCharacter->GetSizeOfStringInBytes(); //need to skip the " sign
	}
}

void CLocalizationEngine::HandleEndOfStringId(CString* aNewCharacter)
{
	if(aNewCharacter->EqualTo(" ") || aNewCharacter->EqualTo("=") || aNewCharacter->EqualTo("\t") || aNewCharacter->EqualTo("\n"))
	{
		//found the end of StringId
		iState = EStateLookingForBeginningOfStringValue;
	}
	else
	{
		iFileEntries->Get(iFileIndex)->iCategoryEntries->Get(iCategoryIndex)->StringEntries->Get(iStringIndex)->StringId->Append(aNewCharacter);
	}
}

void CLocalizationEngine::HandleEndOfStringValue(CString* aNewCharacter)
{
	if(aNewCharacter->EqualTo("\"")) //signals the end of the String
	{
		iState = EStateReady; //go back to Ready State, either find a new String Id or create a new category
	}
	else
	{
		iFileEntries->Get(iFileIndex)->iCategoryEntries->Get(iCategoryIndex)->StringEntries->Get(iStringIndex)->LengthOfStringInBytes += aNewCharacter->GetSizeOfStringInBytes();
	}
}




