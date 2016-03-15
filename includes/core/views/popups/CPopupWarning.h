/*
 * CPopupWarning.h
 *
 *  Created on: Oct 15, 2012
 *      Author: dstoll
 */

#ifndef CPOPUPWARNING_H_
#define CPOPUPWARNING_H_

#include "includes/core/views/popups/CBasicPopup.h"

struct SPopupWarningArgs
{
	CBasicView* OwningView;
	const char* LeftButtonStringId; //pass as NULL if not to be used
	const char* TitleStringId; //pass as NULL if not to be used
	const char* MessageLine1StringId; //pass as NULL if not to be used
	const char* MessageLine2StringId; //pass as NULL if not to be used
	const char* MessageLine3StringId; //pass as NULL if not to be used
	TInt NumberOfSecondsBeforeClosing; //Popup will close by itself if not closed by user
};

class CPopupWarning : public CBasicPopup
{
public:
	static CPopupWarning* New(const SPopupWarningArgs& aArgs);

protected:
	CPopupWarning(const SPopupBasicArgs& aArgs);
	virtual ~CPopupWarning();
	//create Objects here
	void Construct(const char* aMessageLine1StringId, const char* aMessageLine2StringId, const char* aMessageLine3StringId, TInt aSecsBeforeClosing);

	//has to be implemented by Popup child, its additional TouchEvent Code should be added here
	virtual void PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent);
	//has to be implemented by Popup child, its additional Draw Code should be added here
	virtual void PopupDrawViewExtraCode();

	CTextureObject* iWarningSymbol;
	CString* iStringMessageLine1;
	CString* iStringMessageLine2;
	CString* iStringMessageLine3;
	SFont iFontForWarning;
	TInt iFramesRemainingBeforeClosing;
};

#endif /* CPOPUPWARNING_H_ */
