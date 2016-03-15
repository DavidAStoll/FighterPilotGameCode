/*
 ============================================================================
 Name		: Campaign2Map1.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map1 declaration
 ============================================================================
 */

#ifndef CAMPAIGN2MAP1_H
#define CAMPAIGN2MAP1_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign2Map1
 * 
 */
class CCampaign2Map1 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign2Map1* New();

private:

	virtual ~CCampaign2Map1();
	CCampaign2Map1();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN2MAP1_H
