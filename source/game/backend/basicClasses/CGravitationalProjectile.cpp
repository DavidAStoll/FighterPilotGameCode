/*
 ============================================================================
 Name		: GravitationalProjectile.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGravitationalProjectile implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CGravitationalProjectile.h"

CGravitationalProjectile::CGravitationalProjectile(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel) :
	CProjectileBomb(aLocation, aAttributes, aSpeed, aAngel)
{
	// No implementation required
}

CGravitationalProjectile::~CGravitationalProjectile()
{
}

//---------------- functions ---------------------- //

void CGravitationalProjectile::Update()
{
	iCurrentFrame++;
	iCurrentFrameInGravitionalStatus++;
	if(iCurrentFrame == GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE)
	{
		iPixelsPerMoveY -=  TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE);//increase speed over time, simulate gravitional attraction
		iCurrentFrame = 0;
	}
	if(iCurrentFrameInGravitionalStatus == iFramesUntilProjecticleGetsShiftedDown)
	{
		iCurrentFrameInGravitionalStatus = 0;
		if(iTextureObject->GetRotationalAngel() != 270)
		{
			if(iObjectReflected) // faces to the right
				iTextureObject->RotateVertexes(-1);//rotate down
			else
				iTextureObject->RotateVertexes(1);//rotate up, which is actually down
		}
	}

	if(iRange < 0)
	{
		Destruct();
	}else
	{
		iRange -= iSpeed.GetIntInBaseInt();
		iCoordinates.iX += iPixelsPerMoveX;
		iTextureObject->ChangeXCoordinate(iPixelsPerMoveX);
		iCoordinates.iY += iPixelsPerMoveY;
		iTextureObject->ChangeYCoordinate(iPixelsPerMoveY);
	}
}

void CGravitationalProjectile::UpdateGravitionalStatus()
{
	iCurrentFrameInGravitionalStatus = 0;
	TIntFloat lGravitionalAcceleration = - TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE) / TIntFloat::Convert(GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE);
	TInt lFramesUntilEqualPoint = CMath::Abs((iPixelsPerMoveY / lGravitionalAcceleration)).GetIntInBaseInt(); // works also if Y is negative, that is it is already pointing down
	TInt lDegreesAboveHorizontal = 0;
	if(iObjectReflected)
	{
		if(iAngle > 90) //means it is the fourth coordinate
		{
			lDegreesAboveHorizontal = CMath::Abs(360 - iAngle);
		}else //is in the 1st coordinate
		{
			lDegreesAboveHorizontal = CMath::Abs(iAngle);
		}
	}else
	{
		lDegreesAboveHorizontal = CMath::Abs(180 - iAngle); // will be between 1 and 90
	}
	if(lDegreesAboveHorizontal == 0)
		lDegreesAboveHorizontal = 1; //avoid null division
	iFramesUntilProjecticleGetsShiftedDown = lFramesUntilEqualPoint / lDegreesAboveHorizontal;
}

void CGravitationalProjectile::GravitationalProjectileSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	ProjectileBombObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrameInGravitionalStatus);
	aOutputStream.WriteInt32(iFramesUntilProjecticleGetsShiftedDown);
}

void CGravitationalProjectile::GravitationalProjectileLoadContentFromDisk(CFileReadStream &aReadStream)
{
	ProjectileBombObjectLoadContentFromDisk(aReadStream);
	iCurrentFrameInGravitionalStatus = aReadStream.ReadInt32();
	iFramesUntilProjecticleGetsShiftedDown = aReadStream.ReadInt32();
}

void CGravitationalProjectile::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GravitationalProjectileSaveContentToDisk(aOutputStream);
}

void CGravitationalProjectile::LoadFromDisk(CFileReadStream &aReadStream)
{
	GravitationalProjectileLoadContentFromDisk(aReadStream);
}
