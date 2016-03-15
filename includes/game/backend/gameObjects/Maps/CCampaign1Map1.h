/*
 * CCampaign1Map1.h
 *
 *  Created on: Aug 28, 2012
 *      Author: dstoll
 */

#ifndef CCAMPAIGN1MAP1_H_
#define CCAMPAIGN1MAP1_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CMap.h"

// CLASS DECLARATION

/**
 *  CCampaign1Map1
 *
 */
class CCampaign1Map1 : public CMap
	{
public:

	static CCampaign1Map1* New();

private:

	virtual ~CCampaign1Map1();
	CCampaign1Map1();

	virtual void InitializeMap();

	//does the actual loading of textures, can be specific for each map
	virtual void InitializeGameObjectSettingsForMap();
	//does the actual unloading of textures, can be specific for each map
	virtual void RemoveGameObjectSettingsForMap();

	};

#endif /* CCAMPAIGN1MAP1_H_ */
