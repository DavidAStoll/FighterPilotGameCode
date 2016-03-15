/*
 ============================================================================
 Name		: ControlsTutorialZoomInAndZoomOut.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialZoomInAndZoomOut declaration
 ============================================================================
 */

#ifndef CONTROLSTUTORIALZOOMINANDZOOMOUT_H
#define CONTROLSTUTORIALZOOMINANDZOOMOUT_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/TutorialEventIncludes.h"
// CLASS DECLARATION

/**
 *  CControlsTutorialZoomInAndZoomOut
 * 
 */
class CControlsTutorialZoomInAndZoomOut : public CBasicTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CControlsTutorialZoomInAndZoomOut();
	CControlsTutorialZoomInAndZoomOut(TTutorialMethodCallIds aMethodCallIfSuccess);
	virtual TBool TutorialEventLogic();//the implementing child class should have its logic in here
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:
	
	TBool iZoomedOutCompletely;
	TBool iZoomedInCompletely;



	};

#endif // CONTROLSTUTORIALZOOMINANDZOOMOUT_H
