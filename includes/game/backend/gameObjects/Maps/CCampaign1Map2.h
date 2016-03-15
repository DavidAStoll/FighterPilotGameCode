/*
 ============================================================================
 Name		: CCampaign1Map2.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCampaign1Map2 declaration
 ============================================================================
 */

#ifndef CAMPAIGN1MAP2_H
#define CAMPAIGN1MAP2_H

#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign1Map2
 * 
 */
class CCampaign1Map2 : public CMap
	{
public:
	static CCampaign1Map2* New();

private:

	virtual ~CCampaign1Map2();
	CCampaign1Map2();
	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();
	};

#endif // MAP1_H
