/*
 * CPopupEnterText.h
 *
 *  Created on: Oct 14, 2012
 *      Author: dstoll
 */

#ifndef CPOPUPENTERTEXT_H_
#define CPOPUPENTERTEXT_H_

#include "includes/core/views/popups/CBasicPopup.h"

class CPopupEnterText : public CBasicPopup
{
public:
	static CPopupEnterText* New(const SPopupBasicArgs& aArgs, CString* aStringData, TInt aMaxNumberOfCharacters);

protected:
	CPopupEnterText(const SPopupBasicArgs& aArgs, CString* aStringData, TInt aMaxNumberOfCharacters);
	virtual ~CPopupEnterText();
	//create Objects here
	void Construct();

	//this  method should be overriden by the child view and should contain any code if any that should run before the Draw calls
	virtual void ChildRunLogicCode();
	//has to be implemented by Popup child, its additional TouchEvent Code should be added here
	virtual void PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent);
	//has to be implemented by Popup child, its additional Draw Code should be added here
	virtual void PopupDrawViewExtraCode();

	CTextureObject* iTextBackground;

	CString* iStringData;

	SFont iFontForText;

	TRect iTextBackgroundHitbox;
	TInt iMaxNumberOfCharacters;
};

#endif /* CPopupEnterText_H_ */
