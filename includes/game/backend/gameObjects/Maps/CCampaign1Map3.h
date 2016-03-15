/*
 ============================================================================
 Name		: Campaign1Map3.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCampaign1Map3 declaration
 ============================================================================
 */

#ifndef CAMPAIGN1MAP3_H
#define CAMPAIGN1MAP3_H

#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign1Map3
 * 
 */
class CCampaign1Map3 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign1Map3* New();

private:

	virtual ~CCampaign1Map3();
	CCampaign1Map3();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN1MAP3_H
