/*
 ============================================================================
 Name		: Campaign1Map4.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign1Map4 declaration
 ============================================================================
 */

#ifndef CAMPAIGN1MAP4_H
#define CAMPAIGN1MAP4_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign1Map4
 * 
 */
class CCampaign1Map4 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign1Map4* New();

private:

	virtual ~CCampaign1Map4();
	CCampaign1Map4();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();
	};

#endif // CAMPAIGN1MAP4_H
