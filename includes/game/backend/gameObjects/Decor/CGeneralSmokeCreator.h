/*
 * CGeneralSmokeCreator.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CGENERALSMOKECREATOR_H_
#define CGENERALSMOKECREATOR_H_

#include "includes/game/backend/basicClasses/CWeapon.h"
// CLASS DECLARATION

/**
 *  CGeneralSmokeCreator
 *
 */
class CGeneralSmokeCreator : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGeneralSmokeCreator();

	/**
	 * Two-phased constructor.
	 */
	//if the smoke has only one colour, independent of damage,
	static CGeneralSmokeCreator* New(CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset, TBool aBehindOwner, TColour aSmokeColour, TInt aSmokeStartsAfterPercHealthLeft,
																	 TSize aSmokeSize, TInt aFramesUntilSmokeClears, TInt aSmokeAngleOffset = DEFAULT_SMOKE_DEFAULT_ANGLE_OFFSET, TIntFloat aSmokeSpeed = TIntFloat::Convert(DEFAULT_SMOKE_SPEED),
																	 TIntFloat aSquareDisplacementFactorX = TIntFloat::Convert(DEFAULT_SMOKE_DISPLACEMENT_X), TIntFloat aSquareDisplacementFactorY = TIntFloat::Convert(DEFAULT_SMOKE_DISPLACEMENT_Y));
	//smoke changes colour and value depending on health of object, should be default for most moveable objects
	static CGeneralSmokeCreator* New(CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset, TBool aBehindOwner,TSize aSmokeSize, TInt aFramesUntilSmokeClears,
																	 TInt aSmokeAngleOffset = DEFAULT_SMOKE_DEFAULT_ANGLE_OFFSET, TIntFloat aSmokeSpeed = TIntFloat::Convert(DEFAULT_SMOKE_SPEED),
																	 TIntFloat aSquareDisplacementFactorX = TIntFloat::Convert(DEFAULT_SMOKE_DISPLACEMENT_X), TIntFloat aSquareDisplacementFactorY = TIntFloat::Convert(DEFAULT_SMOKE_DISPLACEMENT_Y));
	virtual void Update();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	//Constructors
	CGeneralSmokeCreator(CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset, TBool aBehindOwner, TColour aSmokeColour, TInt aSmokeStartsAfterPercHealthLeft,
			 TSize aSmokeSize, TInt aFramesUntilSmokeClears, TInt aSmokeAngleOffset, TIntFloat aSmokeSpeed, TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY);

	CGeneralSmokeCreator(CGameObject* aOwner, TInt* aAngelOfOwner, TInt aXOffset, TInt aYOffset, TBool aBehindOwner,TSize aSmokeSize, TInt aFramesUntilSmokeClears,
			 TInt aSmokeAngleOffset, TIntFloat aSmokeSpeed, TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY);

	CGameObject* iOwner;
	TColour iColour;
	TInt iStartsAfterPercentageLeft;
	TInt iFramesForSmoke;
	TInt iSmokeAngleOffset;
	TInt iPercentageLeft;
	TBool iIsBehindOwner;
	TBool iIsColourChanges;
	TIntFloat iSmokeSpeed;
	TIntFloat iDisplacementFactorSquareX;
	TIntFloat iDisplacementFactorSquareY;
	TSizeIntFloat iSize;

	};
#endif /* CGENERALSMOKECREATOR_H_ */
