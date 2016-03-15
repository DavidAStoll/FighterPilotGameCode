/*
 * CMissionHighscoresView.h
 *
 *  Created on: Oct 19, 2012
 *      Author: dstoll
 */

#ifndef CMISSIONHIGHSCORESVIEW_H_
#define CMISSIONHIGHSCORESVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CScrollBar.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"

class CMissionHighscoresView : public CBasicView
{
public:

	static CMissionHighscoresView* New(SMission* aMissionToDisplay);
	virtual ~CMissionHighscoresView();

protected:
	CMissionHighscoresView(SMission* aMissionToDisplay);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//draw the categories in the highscore table
	void DrawCategories(TPoint& aLocation);
	//draws a single Table entry
	void DrawRow(TPoint& aLocation, const SMissionHighscoreTableEntry& aEntry, TInt aRankNumber);

	CTextureObject* iBackgroundTexture;
	CTextureObject* iDescriptionBackgroundTexture;

	SMission* iMission;

	CString* iStringBack;
	CString* iStringTitle;
	CString* iStringMissionName;
	//category names
	CString* iStringRank;
	CString* iStringName;
	CString* iStringScore;
	CString* iStringTime;
	CString* iStringDate;

	SFont iFontForButton;
	SFont iFontForMissionName;
	SFont iFontForTitle;
	SFont iFontForHighscoresCategories;
	SFont iFontForHighscoresRows;

	TFloat iTableYOffset;

	CButton* iButtonBack;
	CScrollBar* iScrollBar;
	CTouchSurface* iTouchSurface;
};

#endif /* CMISSIONHIGHSCORESVIEW_H_ */
