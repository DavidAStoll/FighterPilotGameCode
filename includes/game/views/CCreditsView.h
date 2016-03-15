/*
 * CCreditsView.h
 *
 *  Created on: Sep 20, 2012
 *      Author: dstoll
 */

#ifndef CCREDITSVIEW_H_
#define CCREDITSVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/game/common/CGameVariables.h"

class CCreditsView : public CBasicView
{
private:

	struct SCreditsEntry
	{
		CString* TitleString;
		CString* NameString;
		CString* SectionString;
		TImageId ImageId;
	};

public:

	virtual ~CCreditsView();
	static CCreditsView* New();

protected:
	CCreditsView();
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//functions can be used to create new sections/entries in the Credits
	void AddSectionAsNextEntry(const char* aStringId);
	void AddTitleAsNextEntry(const char* aStringId);
	void AddNameAsNextEntry(const char* aStringId);
	void AddImageAsNextEntry(TImageId aImageId);
	//calculates what the MaxYOffset is with the current Credit Entries
	TInt GetMaxYOffsetForEntries();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iGameObjectTexture;

	TFloat iYOffsetValue;
	TInt iMaxYOffsetValue;
	TBool iWasPlayingAUserSong;

	CString* iStringBackButton;
	CString* iStringSongName;

	SFont iFontForBackButton;
	SFont iFontForTitleEntries;
	SFont iFontForNameEntries;
	SFont iFontForSectionEntries;

	CButton* iBackButton;
	CTouchSurface* iScrollSurface;

	CPointerArray<SCreditsEntry>* iCreditEntries;
};

#endif /* CCREDITSVIEW_H_ */
