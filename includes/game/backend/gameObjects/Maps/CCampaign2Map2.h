/*
 ============================================================================
 Name		: Campaign2Map2.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map2 declaration
 ============================================================================
 */

#ifndef CAMPAIGN2MAP2_H
#define CAMPAIGN2MAP2_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign2Map2
 * 
 */
class CCampaign2Map2 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign2Map2* New();

private:

	virtual ~CCampaign2Map2();
	CCampaign2Map2();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();
	};

#endif // CAMPAIGN2MAP2_H
