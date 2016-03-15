/*
 * CTutorial1Map.h
 *
 *  Created on: Oct 1, 2012
 *      Author: dstoll
 */

#ifndef CTUTORIAL1MAP_H_
#define CTUTORIAL1MAP_H_

#include "includes/game/backend/basicClasses/CBasicTutorialMap.h"

class CTutorial1Map : public CBasicTutorialMap
{
public:

	static CTutorial1Map* New();
	//use to change the what Tutorial Section we are in
	virtual void TutorialMethodCall(TTutorialMethodCallIds aCallId);

private:
	CTutorial1Map();
	virtual ~CTutorial1Map();
	//not really important in this case, since we always start of with a view by default
	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	//needed for one specific instance
	TTutorialMethodCallIds iPreviousTutorialMethodCall;
};

#endif /* CTUTORIAL1MAP_H_ */
