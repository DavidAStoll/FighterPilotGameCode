/*
 ============================================================================
 Name		: Campaign2Map4.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign2Map4 declaration
 ============================================================================
 */

#ifndef CAMPAIGN2MAP4_H
#define CAMPAIGN2MAP4_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign2Map4
 * 
 */
class CCampaign2Map4 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign2Map4* New();

private:

	virtual ~CCampaign2Map4();
	CCampaign2Map4();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN2MAP4_H
