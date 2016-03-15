/*
 * CTutorial3Map.h
 *
 *  Created on: Oct 4, 2012
 *      Author: dstoll
 */

#ifndef CTUTORIAL3MAP_H_
#define CTUTORIAL3MAP_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CBasicTutorialMap.h"

class CDauntless;

// CLASS DECLARATION

/**
 *  CTutorial2Map
 *
 */
class CTutorial3Map : public CBasicTutorialMap
	{
public:

	static CTutorial3Map* New();
	//use to change the what Tutorial Section we are in
	virtual void TutorialMethodCall(TTutorialMethodCallIds aCallId);

private:
	CTutorial3Map();
	virtual ~CTutorial3Map();
	//not really important in this case, since we always start of with a view by default
	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();
};

#endif /* CTUTORIAL3MAP_H_ */
