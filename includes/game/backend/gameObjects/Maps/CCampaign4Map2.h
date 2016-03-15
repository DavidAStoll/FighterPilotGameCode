/*
 ============================================================================
 Name		: Campaign4Map2.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map2 declaration
 ============================================================================
 */

#ifndef CAMPAIGN4MAP2_H
#define CAMPAIGN4MAP2_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"
// CLASS DECLARATION

/**
 *  CCampaign4Map2
 * 
 */
class CCampaign4Map2 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign4Map2* New();

private:

	virtual ~CCampaign4Map2();
	CCampaign4Map2();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN4MAP2_H
