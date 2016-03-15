/*
 ============================================================================
 Name		: GeneralSmokeCreator.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGeneralSmokeCreator implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/Decor/CSmoke.h"
#include "includes/game/backend/gameObjects/Decor/CGeneralSmokeCreator.h"


CGeneralSmokeCreator::CGeneralSmokeCreator(CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset, TBool aBehindOwner, TColour aSmokeColour, TInt aSmokeStartsAfterPercHealthLeft,
																					 TSize aSmokeSize, TInt aFramesUntilSmokeClears, TInt aSmokeAngleOffset, TIntFloat aSmokeSpeed, TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY)
:CWeapon(1, WeaponIdentifierGeneralSmokeCreator,false,false, &aOwner->GetCurrentPosition(), aAngelOfOwner, aXOffset, aYOffset, &aOwner->IsReflected(), NULL, 0)
{
	//this constructor is used if smoke does not change
	iIsColourChanges = false;
	iColour = aSmokeColour;
	iStartsAfterPercentageLeft = aSmokeStartsAfterPercHealthLeft;

	iOwner = aOwner;
	iIsBehindOwner = aBehindOwner;
	iSize = TSizeIntFloat(TIntFloat::Convert(aSmokeSize.iWidth), TIntFloat::Convert(aSmokeSize.iHeight));
	iFramesForSmoke = aFramesUntilSmokeClears;
	iSmokeAngleOffset = aSmokeAngleOffset;
	iSmokeSpeed = aSmokeSpeed;
	iDisplacementFactorSquareX = aSquareDisplacementFactorX;
	iDisplacementFactorSquareY = aSquareDisplacementFactorY;
	iPercentageLeft = -1;
}

CGeneralSmokeCreator::CGeneralSmokeCreator(CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset, TBool aBehindOwner,TSize aSmokeSize, TInt aFramesUntilSmokeClears,
																					 TInt aSmokeAngleOffset, TIntFloat aSmokeSpeed, TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY)
:CWeapon(1, WeaponIdentifierGeneralSmokeCreator, false, false, &aOwner->GetCurrentPosition(), aAngelOfOwner, aXOffset, aYOffset, &aOwner->IsReflected(), NULL, 0)
{
	//this constructor is used if smoke colour changes depending on damage on owning object
	iIsColourChanges = true;
	iColour = 0;
	iStartsAfterPercentageLeft = 90; //starts after 90 percent health left

	iOwner = aOwner;
	iIsBehindOwner = aBehindOwner;
	iSize = TSizeIntFloat(TIntFloat::Convert(aSmokeSize.iWidth), TIntFloat::Convert(aSmokeSize.iHeight));
	iFramesForSmoke = aFramesUntilSmokeClears;
	iSmokeAngleOffset = aSmokeAngleOffset;
	iSmokeSpeed = aSmokeSpeed;
	iDisplacementFactorSquareX = aSquareDisplacementFactorX;
	iDisplacementFactorSquareY = aSquareDisplacementFactorY;
	iPercentageLeft = -1;
}

CGeneralSmokeCreator::~CGeneralSmokeCreator()
	{

	}


CGeneralSmokeCreator* CGeneralSmokeCreator::New(
		CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset,
		TBool aBehindOwner, TColour aSmokeColour, TInt aSmokeStartsAfterPercHealthLeft, TSize aSmokeSize,
		TInt aFramesUntilSmokeClears, TInt aSmokeAngleOffset, TIntFloat aSmokeSpeed, TIntFloat aSquareDisplacementFactorX,
		TIntFloat aSquareDisplacementFactorY)
{
	CGeneralSmokeCreator* self = new CGeneralSmokeCreator(aOwner, aAngelOfOwner, aXOffset, aYOffset, aBehindOwner, aSmokeColour, aSmokeStartsAfterPercHealthLeft, aSmokeSize, aFramesUntilSmokeClears, aSmokeAngleOffset, aSmokeSpeed, aSquareDisplacementFactorX, aSquareDisplacementFactorY);
	return self;
}

CGeneralSmokeCreator* CGeneralSmokeCreator::New(
		CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset,
		TBool aBehindOwner, TSize aSmokeSize, TInt aFramesUntilSmokeClears, TInt aSmokeAngleOffset, TIntFloat aSmokeSpeed,
		TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY)
{
	CGeneralSmokeCreator* self = new CGeneralSmokeCreator(aOwner, aAngelOfOwner, aXOffset, aYOffset, aBehindOwner, aSmokeSize, aFramesUntilSmokeClears, aSmokeAngleOffset, aSmokeSpeed, aSquareDisplacementFactorX, aSquareDisplacementFactorY);
	return self;
}


//-------------- functions ----------------------//
void CGeneralSmokeCreator::Update()
{
	 //if object health is below 90% start creating smoke
	  TInt lPercentageOfHealthLeft = ((iOwner->GetHealth()) * 100) / iOwner->GetMaxHealth();

	  if(lPercentageOfHealthLeft <= iStartsAfterPercentageLeft)
	  {
	  	//to modify start size a bit
	  	TIntFloat lRandomInt  = CFighterPilotThePacificWar::FighterGame->iRandomGenerator->GetRandomNumberTIntFloat(-5, 6);

	  	TIntFloat lSmokeScaleFactor = TIntFloat::Convert(1) + lRandomInt / 20;//scale the size of the cloud
			TIntFloat lWidthOfSmokeScaled = iSize.iWidth * lSmokeScaleFactor;
			TIntFloat lHeightOfSmokeScaled = iSize.iHeight * lSmokeScaleFactor;

			if(iIsColourChanges && iPercentageLeft != lPercentageOfHealthLeft)
			{
				//to avoid repetive recalculation
				iPercentageLeft = lPercentageOfHealthLeft;

				//need to update the colour of the smoke, since health changed
				TUint lColourComponent = (lPercentageOfHealthLeft * 255) / 100; //don't want to go completly black
				TUint lColourComponentAdjusted = lColourComponent + 20; //don't wamt to go completly black
				iColour = 0;
				iColour += lColourComponentAdjusted << 24; //Red
				iColour += lColourComponentAdjusted << 16; //Green
				iColour += lColourComponentAdjusted << 8; //Blue
				iColour += 255 - lColourComponent; //Alpha, inverse of health
			}

			//find out how start location changes due to size changes
			TIntFloat lPositionChangeX = (lWidthOfSmokeScaled - iSize.iWidth) / 2;
			TIntFloat lPositionChangeY = (lHeightOfSmokeScaled - iSize.iHeight) / 2;

			TInt lSmokeZValue;
			if(iIsBehindOwner)
				lSmokeZValue = iOwner->iZCor - 1;
			else
				lSmokeZValue = SMOKE_DEFAULT_Z_VALUE; //should be infront of everything

			//special case of if object gets slow recycled
			TUint8 lAlphaOfColour = iColour & (0x000000FF);
			if(iOwner->GetAlpha() < lAlphaOfColour)
			{
				//remove original alpha and substitute with texture objects alpha from owner
				iColour = (iColour & (0xFFFFFF00)) + iOwner->GetAlpha();
			}

			//need to add offset to objects angle to get the correct angle of the smoke
			TInt lSmokeAngle = *iAngel;
			if(*iWeaponReflectedOverYAxis)
				lSmokeAngle -= iSmokeAngleOffset;
			else
				lSmokeAngle += iSmokeAngleOffset;

			CSmoke* lSmoke = CSmoke::New(GetRelativeWeaponPosition(lPositionChangeX.GetIntInBaseInt(), -lPositionChangeY.GetIntInBaseInt()), iSmokeSpeed, lSmokeAngle, *iWeaponReflectedOverYAxis, iColour, TSizeIntFloat(lWidthOfSmokeScaled, lHeightOfSmokeScaled), iFramesForSmoke, lSmokeZValue,
																	 iDisplacementFactorSquareX, iDisplacementFactorSquareY);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lSmoke);
	  }
}

void CGeneralSmokeCreator::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  WeaponObjectSaveContentToDisk(aOutputStream);
  //nothing needs to be saved, since it will be initialized when Owning object is loaded
}
void CGeneralSmokeCreator::LoadFromDisk(CFileReadStream &aReadStream)
{
  WeaponObjectLoadContentFromDisk(aReadStream);
}
