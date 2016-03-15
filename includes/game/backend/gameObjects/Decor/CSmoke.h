/*
 * CSmoke.h
 *
 *  Created on: May 7, 2013
 *      Author: dstoll
 */

#ifndef CSMOKE_H_
#define CSMOKE_H_

#include "includes/game/backend/basicClasses/CProjectile.h"

class CSmoke : public CProjectile
{

public:
	virtual ~CSmoke();

	static CSmoke* New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aDirectionAngle, TBool aReflected, TColour aColour, TSizeIntFloat aSize,  TInt aFramesUntilDestruct, TInt aZValue);
	//allows the smoke to move along X and Y with increasing square ratio to allow effects such as wind on smoke if desired
	static CSmoke* New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aDirectionAngle, TBool aReflected, TColour aColour, TSizeIntFloat aSize,  TInt aFramesUntilDestruct, TInt aZValue,
										 TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY);
	static void InitilizeSmoke();
	static void RemoveSmoke();
	virtual void Move();
	virtual void Draw();
	virtual void Update();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	CSmoke(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aDirectionAngle, TColour aColour, TSizeIntFloat aSize,
				TInt aFramesUntilDestruct, TInt aZValue, TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY, SGameObjectAttributes& aAttributes);

	static void LoadSmokeTextures();
	static void UnloadSmokeTextures();
	void Construct();

	TInt iFramesUntilDestruct;
	TSizeIntFloat iSmokeSize;
	TIntFloat iIncreaseSizeFactor;
	TIntFloat iExtraDisplacementFactorX;
	TIntFloat iExtraDisplacementFactorY;
	TIntFloat iExtraDisplacementAccumaltorX;
	TIntFloat iExtraDisplacementAccumaltorY;
	TColour iColour;
	TInt iAlphaValue;
};

#endif /* CSMOKE_H_ */
