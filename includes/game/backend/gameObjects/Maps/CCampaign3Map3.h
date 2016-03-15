/*
 ============================================================================
 Name		: Campaign3Map3.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CCampaign3Map3 declaration
 ============================================================================
 */

#ifndef CAMPAIGN3MAP3_H
#define CAMPAIGN3MAP3_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"
// CLASS DECLARATION

/**
 *  CCampaign3Map3
 * 
 */
class CCampaign3Map3 : public CMap
	{
public:
	// Constructors and destructor

	static CCampaign3Map3* New();

private:

	virtual ~CCampaign3Map3();
	CCampaign3Map3();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif // CAMPAIGN3MAP3_H
