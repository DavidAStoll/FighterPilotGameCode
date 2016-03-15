/*
 ============================================================================
 Name		: Campaign3Map2.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map2 declaration
 ============================================================================
 */

#ifndef CAMPAIGN3MAP2_H
#define CAMPAIGN3MAP2_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign3Map2
 * 
 */
class CCampaign3Map2 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign3Map2* New();

private:

	virtual ~CCampaign3Map2();
	CCampaign3Map2();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN3MAP2_H
