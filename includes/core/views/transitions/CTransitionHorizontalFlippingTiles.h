/*
 * CTransitionHorizontalFlippingTiles.h
 *
 *  Created on: Jul 12, 2012
 *      Author: dstoll
 */

#ifndef CTRANSITIONHORIZONTALFLIPPINGTILES_H_
#define CTRANSITIONHORIZONTALFLIPPINGTILES_H_

#include "includes/core/views/transitions/CBasicTransition.h"

class CTransitionHorizontalFlippingTiles : public CBasicTransition
{
	struct STile //keeps track of an individual Tile
	{
		TFloat Angle;
		TBool IsDoingFinishingRotation;
		CTextureObject* TextureObject;
	};

public:

	virtual ~CTransitionHorizontalFlippingTiles();
	static CTransitionHorizontalFlippingTiles* New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TInt aNumberOfTilesHorizontal, TInt aNumberOfTilesVertical, TInt aAngleDelayBetweenTiles = 10);

protected:
	CTransitionHorizontalFlippingTiles(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TInt aNumberOfTilesHorizontal, TInt aNumberOfTilesVertical, TInt aAngleDelayBetweenTiles = 10);
	//update transition progress and draw it
	virtual void DrawTransition();

	TInt iAngleDelayBetweenTiles;
	TInt iNumberOfTilesHoriontally;
	TInt iNumberOfTilesVertically;
	TFloat iAngleIncrementValue;
	TFloat iTextureSPerTile;
	TFloat iTextureTPerTile;
	TIntFloat iWidthOfTile;
	TIntFloat iHeightOfTile;
	STile* iTiles; //for keep tracking of all the tiles
};

#endif /* CTRANSITIONHORIZONTALFLIPPINGTILES_H_ */
