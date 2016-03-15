/*
 ============================================================================
 Name		: Campaign4Map4.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map4 declaration
 ============================================================================
 */

#ifndef CAMPAIGN4MAP4_H
#define CAMPAIGN4MAP4_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign4Map4
 * 
 */
class CCampaign4Map4 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign4Map4* New();

private:

	virtual ~CCampaign4Map4();
	CCampaign4Map4();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN4MAP4_H
