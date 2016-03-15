/*
 * CTransitionHorizontalFlippingTiles.cpp
 *
 *  Created on: Jul 12, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CTransitionHorizontalFlippingTiles::CTransitionHorizontalFlippingTiles(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TInt aNumberOfTilesHorizontal, TInt aNumberOfTilesVertical, TInt aAngleDelayBetweenTiles)
		:CBasicTransition(aViewObserver, aOldView, aNewView, aDurationInSeconds)
{
	iAngleDelayBetweenTiles = aAngleDelayBetweenTiles;
	iNumberOfTilesHoriontally = aNumberOfTilesHorizontal;
	iNumberOfTilesVertically = aNumberOfTilesVertical;

	iWidthOfTile = iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth / TIntFloat::Convert(iNumberOfTilesHoriontally);
	iHeightOfTile = iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight / TIntFloat::Convert(iNumberOfTilesVertically);
	iTextureSPerTile = (TFloat) 1 / iNumberOfTilesHoriontally;
	iTextureTPerTile = (TFloat) 1 / iNumberOfTilesVertically;

	iTiles = NULL;
	iTiles = new STile[aNumberOfTilesHorizontal * aNumberOfTilesVertical];

	//find out delays for each Tile
	for(TInt lVerticalIndex = 0; lVerticalIndex < iNumberOfTilesVertically; lVerticalIndex++)
	{
		for(TInt lHorizontalIndex = 0; lHorizontalIndex < iNumberOfTilesHoriontally; lHorizontalIndex++)
		{
			iTiles[lHorizontalIndex + lVerticalIndex * iNumberOfTilesHoriontally].Angle = -(lHorizontalIndex + lVerticalIndex) * iAngleDelayBetweenTiles;
			iTiles[lHorizontalIndex + lVerticalIndex * iNumberOfTilesHoriontally].IsDoingFinishingRotation = false;
			iTiles[lHorizontalIndex + lVerticalIndex * iNumberOfTilesHoriontally].TextureObject = iOldViewTextureObject;
		}
	}

	TInt lMaxDelayForTiles = iTiles[(iNumberOfTilesVertically * iNumberOfTilesHoriontally) - 1].Angle * -1;
	iAngleIncrementValue = (TFloat) (180 + lMaxDelayForTiles) / iUpdatesUntilComplete; //the entire degrees it has to turn through
}

CTransitionHorizontalFlippingTiles::~CTransitionHorizontalFlippingTiles()
{
	if(iTiles != NULL)
	{
		delete[] iTiles;
		iTiles = NULL;
	}
}

CTransitionHorizontalFlippingTiles* CTransitionHorizontalFlippingTiles::New(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds, TInt aNumberOfTilesHorizontal, TInt aNumberOfTilesVertical, TInt aAngleDelayBetweenTiles)
{
	CTransitionHorizontalFlippingTiles* lSelf = new CTransitionHorizontalFlippingTiles(aViewObserver, aOldView, aNewView, aDurationInSeconds, aNumberOfTilesHorizontal, aNumberOfTilesVertical, aAngleDelayBetweenTiles);
	return lSelf;
}

void CTransitionHorizontalFlippingTiles::DrawTransition()
{
	//move to TopRight corner of screen
	iGraphicsEngine->TranslateMatrix(-(iGraphicsEngine->GetVirtualDimensionsOfScreen().iWidth - iWidthOfTile) / 2,
																	(iGraphicsEngine->GetVirtualDimensionsOfScreen().iHeight - iHeightOfTile) / 2,
																	TIntFloat::Convert(0));

	for(TInt lIndexVertical = iNumberOfTilesVertically; lIndexVertical > 0; lIndexVertical--)  //since the image is UpSide Down in OpenGL Memory
	{
		TInt lIndexVerticalConverted = iNumberOfTilesVertically - lIndexVertical;

		iGraphicsEngine->PushMatrix();//avoid that horizontal movement is saved
		for(TInt lIndexHorizontal = 0; lIndexHorizontal < iNumberOfTilesHoriontally; lIndexHorizontal++)
		{
			STile& lTile = iTiles[lIndexHorizontal + lIndexVerticalConverted * iNumberOfTilesHoriontally];
			CTextureObject* lTextureObject = lTile.TextureObject;
			lTextureObject->SetNewTValue((lIndexVertical - 1) * iTextureTPerTile,  lIndexVertical* iTextureTPerTile);  //since the image is UpSide Down in OpenGL Memory
			lTextureObject->SetNewSValue(lIndexHorizontal * iTextureSPerTile, (lIndexHorizontal + 1) * iTextureSPerTile);

			//draw Tile
			iGraphicsEngine->PushMatrix();
			//need to scale down the texture to fit the correct size
			iGraphicsEngine->ScaleMatrix((TFloat) 1 / iNumberOfTilesHoriontally, (TFloat) 1 / iNumberOfTilesVertically, 1);

			if(lTile.Angle > 0) //only rotate if bigger then 0, this way we avoid rotation for Tiles shouldn't start yet or have already finished
			{
				iGraphicsEngine->RotateMatrix(lTile.Angle, 1, 0, 0);
			}
			lTextureObject->DrawArrays();
			iGraphicsEngine->PopMatrix();

			//update Tile Angle
			if(lTile.IsDoingFinishingRotation)
			{
				lTile.Angle -= iAngleIncrementValue; //just keep decrementing the Angle until it is back to 0
			}
			else
			{
				lTile.Angle += iAngleIncrementValue;
				if(lTile.Angle >= 90) //tile just now rotate back
				{
					lTile.Angle = 90;
					lTile.IsDoingFinishingRotation = true;
					lTile.TextureObject = iNewViewTextureObject;
				}
			}

			iGraphicsEngine->TranslateMatrix(iWidthOfTile, TIntFloat::Convert(0), TIntFloat::Convert(0)); //move to the next column
		}
		iGraphicsEngine->PopMatrix();

		//increment to the next row
		iGraphicsEngine->TranslateMatrix( TIntFloat::Convert(0), -iHeightOfTile,  TIntFloat::Convert(0));
	}
}
