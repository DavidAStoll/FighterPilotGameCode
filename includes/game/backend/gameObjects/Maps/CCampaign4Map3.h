/*
 ============================================================================
 Name		: Campaign4Map3.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign4Map3 declaration
 ============================================================================
 */

#ifndef CAMPAIGN4MAP3_H
#define CAMPAIGN4MAP3_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign4Map3
 * 
 */
class CCampaign4Map3 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign4Map3* New();

private:

	virtual ~CCampaign4Map3();
	CCampaign4Map3();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();
	};

#endif // CAMPAIGN4MAP3_H
