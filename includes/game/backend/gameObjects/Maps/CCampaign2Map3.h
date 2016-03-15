/*
 ============================================================================
 Name		: Campaign2Map3.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map3 declaration
 ============================================================================
 */

#ifndef CAMPAIGN2MAP3_H
#define CAMPAIGN2MAP3_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign2Map3
 * 
 */
class CCampaign2Map3 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign2Map3* New();

private:

	virtual ~CCampaign2Map3();
	CCampaign2Map3();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN2MAP3_H
