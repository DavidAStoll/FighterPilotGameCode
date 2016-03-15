/*
 ============================================================================
 Name		: Campaign4Map1.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map1 declaration
 ============================================================================
 */

#ifndef CAMPAIGN4MAP1_H
#define CAMPAIGN4MAP1_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign4Map1
 * 
 */
class CCampaign4Map1 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign4Map1* New();

private:

	virtual ~CCampaign4Map1();
	CCampaign4Map1();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN4MAP1_H
