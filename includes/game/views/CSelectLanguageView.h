/*
 * CSelectLanguageView.h
 *
 *  Created on: Sep 22, 2012
 *      Author: dstoll
 */

#ifndef CSELECTLANGUAGEVIEW_H_
#define CSELECTLANGUAGEVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/core/views/components/CButton.h"
#include "includes/game/common/CGameVariables.h"

class CSelectLanguageView : public CBasicView
{
	enum TLanguageSelected
	{
		ELanguageSelectedEnglish,
		ELanguageSelectedGerman,
		ELanguageSelectedFrench,
		ELanguageSelectedChineseTraditional,
		ELanguageSelectedChineseSimplified,
		ELanguageSelectedNone,
	};

public:
	virtual ~CSelectLanguageView();
	static CSelectLanguageView* New(TBool aFirstTimeSelectingLanguage);

protected:
	CSelectLanguageView(TBool aFirstTimeSelectingLanguage);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;

	CString* iStringBackButton;
	CString* iStringEnglish;
	CString* iStringGerman;
	CString* iStringFrench;
	CString* iStringChineseSimplified;
	CString* iStringChineseTraditional;

	TRect iRectEnglish;
	TRect iRectGerman;
	TRect iRectFrench;
	TRect iRectChineseSimplified;
	TRect iRectChineseTraditional;

	TBool iFirstTimeSelectingLanguage;

	TLanguageSelected iLanguageSelected;

	SFont iFontForBackButton;
	SFont iFontForLanguages;

	CButton* iBackButton;
};

#endif /* CSELECTLANGUAGEVIEW_H_ */
