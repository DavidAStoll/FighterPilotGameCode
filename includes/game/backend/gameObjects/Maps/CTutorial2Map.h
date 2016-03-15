/*
 ============================================================================
 Name		: Tutorial2Map.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTutorial2Map declaration
 ============================================================================
 */

#ifndef TUTORIAL2MAP_H
#define TUTORIAL2MAP_H

// INCLUDES
#include "includes/game/backend/basicClasses/CBasicTutorialMap.h"

class CDauntless;

// CLASS DECLARATION

/**
 *  CTutorial2Map
 * 
 */
class CTutorial2Map : public CBasicTutorialMap
	{
public:

	static CTutorial2Map* New();
	//use to change the what Tutorial Section we are in
	virtual void TutorialMethodCall(TTutorialMethodCallIds aCallId);

private:
	CTutorial2Map();
	virtual ~CTutorial2Map();
	//not really important in this case, since we always start of with a view by default
	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	//needed for user stat
	TTutorialMethodCallIds iPreviousTutorialMethodCall;

	};

#endif // TUTORIAL2MAP_H
