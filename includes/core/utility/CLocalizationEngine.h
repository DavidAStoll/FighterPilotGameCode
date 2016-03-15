/*
 * CLocalizationEngine.h
 *
 *  Created on: Jul 18, 2012
 *      Author: dstoll
 */

#ifndef CLocalizationEngine_H_
#define CLocalizationEngine_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

enum TLanguage
{
	ELanguageEnglish,
	ELanguageGerman,
	ELanguageFrench,
	ELanguageChineseTraditional,
	ELanguageChineseSimplified
};


class CLocalizationEngine
{

private:

	struct SStringEntry
	{
		CString* StringId; //carries the id Name of the String
		TUint ByteOffset; //how many bytes to the beginning of this String
		TUint LengthOfStringInBytes; //how many bytes this String contains
	};

	struct SCategoryEntry
	{
		CString* CategoryName;
		CPointerArray<SStringEntry>* StringEntries;
	};

	struct SFileEntry
	{
		CString* FileName;
		TBool IsLocalizedFile;
		CPointerArray<SCategoryEntry>* iCategoryEntries;
	};

	enum TState
	{
		EStateReady,
		EStateLookingForEndOfCategoryName,
		EStateLookingForEndOfStringId,
		EStateLookingForBeginningOfStringValue,
		EStateLookingForEndOfStringValue,
		EStateComment
	};

public:
	static CLocalizationEngine* New(const char* aPathToLocalizationDirectoy);//should contain the path to the folder that contains all the localization files
	virtual ~CLocalizationEngine();

	//reads and adds the content of a Localization File(contained in the asset directory) to the data structer
	void AddLocalizationFile(const char* aFileName);
	//reads and adds the content of a Unlocalized File, the same file will always be loaded no matter what language is selected (.unlocalized)
	void AddUnlocalizedFile(const char* aFileName);
	//returns what language is currently used for localization
	TLanguage GetLanguage();
	//returns Localized String that depends on the current language and the String Id, can also be an unlocalized String
	//a Id is built through 3 components, the FileId, CategoryId, and StringId seperated by dots
	//for example a valid argument would look this file.category.StringId
	CString* GetString(const char* aId);
	CString* GetString(const CString* aId);
	//changes the language that Localized Strings will be in for subsequent loads
	//will automatically clear its data structer and create a new index on the new language files versions
	void SetLanguage(TLanguage aLanguage);

protected:
	CLocalizationEngine(const char* aPathToLocalizationDirectoy);
	void Construct();

	//creates an Index for the specified file that resides in the Asset directory
	void BuildIndexForLanguageFile(CString* aFileName);
	// Deletes Index build for this file
	void DeleteIndexFromFile(CString* aFileName);
	//returns a String that contains the file extension for this particular language
	CString* GetFileExtensionForLanguage(TLanguage aLanguage);
	//Get Full Path for Filename, gonna create a special function for it, in case it needs to be changed
	CString* GetFullPathForFileName(const char* aFileName);
	//contains the Code to load a String from the given file under the following category
	void GetStringFromFile(CString* aFileName, CString* aCategoryName, CString* aStringName, CString* aToHoldLocalizedString);
	//contains the code that will run when the system looks for the beginning of the String Value
	void HandleBeginningOfStringValue(CString* aNewCharacter);
	//contains the code that will run when the system is in comment State
	void HandleCommentState(CString* aNewCharacter);
	//contains the code that will run when the system looks for the end of the category name
	void HandleEndOfCategoryNameState(CString* aNewCharacter);
	//contains the code that will run when the system looks for the end of the String Id
	void HandleEndOfStringId(CString* aNewCharacter);
	//contains the code that will run when the system looks for the end of the String Value
	void HandleEndOfStringValue(CString* aNewCharacter);
	//contains the code that will run if the system is in the ready state
	void HandleReadyState(CString* aNewCharacter);


	TState iState;
	TUint iByteLocationInFile; //keeps track at what byte we are currently in the file
	TInt iFileIndex;
	TInt iCategoryIndex; //keeps track of what category we are currently working on
	TInt iStringIndex;	//keeps track of what String we are currently working on
	TLanguage iLanguage; //keeps track what current language we use
	CPointerArray<SFileEntry>* iFileEntries;
	CString* iPathToLocalizationDirectoy;


};

#endif /* CLocalizationEngine_H_ */
