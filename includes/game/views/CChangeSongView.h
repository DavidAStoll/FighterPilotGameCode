/*
 * ChangeSongView.h
 *
 *  Created on: Sep 17, 2012
 *      Author: dstoll
 */

#ifndef CCHANGESONGVIEW_H_
#define CCHANGESONGVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/core/views/components/CRadioButtonGroup.h"
#include "includes/core/views/components/CScrollBar.h"

class CChangeSongView : public CBasicView
{

protected:

	struct SSongEntry
	{
		CString* SongName;
		TBool IsUserSong;
	};

public:
	virtual ~CChangeSongView();
	static CChangeSongView* New(TBool aReturnToGameView);

	//loads all the image through the imageStore that this view needs
	static void LoadImagesNeededForView();
	//unloads all the images from the imageStore that were needed for this view
	static void UnloadImagesNeededForView();

protected:
	CChangeSongView(TBool aReturnToGameView);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//Add a SongTile to the list of SongTiles for display
	void AddSongTile(const char* aSongName, TBool aUserSong);

	CTextureObject* iBackgroundTexture;

	CString* iStringSongPlaying;
	CString* iStringAvailableSongs;
	CString* iStringCancelButton;
	CString* iStringSaveButton;

	SFont iFontForLabels;
	SFont iFontForButtons;
	SFont iFontForSongNames;

	CButton* iCancelButton;
	CButton* iSaveButton;
	CScrollBar* iScrollBar;

	SSongEntry iOriginalSongPlaying;

	CPointerArray<SSongEntry>* iSongTilesNames;
	CRadioButtonGroup* iSongTilesButtons;

	TFloat iSongTilesYOffset;
	TInt iSongTilesTopYLocation;
	TInt iLastSongTileSelected;
	TBool iReturnToGameView;
};

#endif /* CHANGESONGVIEW_H_ */
