/*
 * CCompanyImageView.h
 *
 *  Created on: Sep 12, 2012
 *      Author: dstoll
 */

#ifndef CCOMPANYIMAGEVIEW_H_
#define CCOMPANYIMAGEVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"

class CCompanyImageView : public CBasicView
{

public:
	virtual ~CCompanyImageView();
	static CCompanyImageView* New();

protected:
	CCompanyImageView();
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	TBool iHasBeenDrawnOnce;
	TBool iInitDone;
	CTextureObject* iBackgroundTexture;
};

#endif /* CCOMPANYIMAGEVIEW_H_ */
