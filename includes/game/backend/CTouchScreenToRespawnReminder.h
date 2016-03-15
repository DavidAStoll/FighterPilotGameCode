/*
 * CTouchScreenToRespawnReminder.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CTOUCHSCREENTORESPAWNREMINDER_H_
#define CTOUCHSCREENTORESPAWNREMINDER_H_

#include "includes/core/graphic/CGraphicsFont.h"

#define TOUCHSCREEN_TO_RESPAWN_REMINDER_Y_OFFSET (400)
#define TOUCHSCREEN_TO_RESPAWN_REMINDER_FRAMES_INTERVAL_BETWEEN_DRAW_AND_NOT_DRAW 10
#define TOUCHSCREEN_TO_RESPAWN_REMINDER_FONT_SIZE 30

class CPlayer;

class CTouchScreenToRespawnReminder
	{
public:

	~CTouchScreenToRespawnReminder();
	static CTouchScreenToRespawnReminder* New(CPlayer* aPlayer);
	void Draw();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CTouchScreenToRespawnReminder(CPlayer* aPlayer);

	CPlayer* iPlayer;
	SFont iFont;
	CString* iStringRespawnMessage;
	TBool iIsInDrawMode;
	TInt iCurrentFrame;

	};

#endif /* CTOUCHSCREENTORESPAWNREMINDER_H_ */
