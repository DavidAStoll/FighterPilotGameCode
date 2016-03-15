/*
 ============================================================================
 Name		: Campaign3Map4.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map4 declaration
 ============================================================================
 */

#ifndef CAMPAIGN3MAP4_H
#define CAMPAIGN3MAP4_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign3Map4
 * 
 */
class CCampaign3Map4 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign3Map4* New();

private:

	virtual ~CCampaign3Map4();
	CCampaign3Map4();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN3MAP4_H
