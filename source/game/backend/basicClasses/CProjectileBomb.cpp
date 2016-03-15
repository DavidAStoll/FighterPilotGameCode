/*
 ============================================================================
 Name		: ProjectileBomb.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CProjectileBomb implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CProjectileBomb.h"

CProjectileBomb::CProjectileBomb(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel) :
	CProjectile(NULL, NULL, aLocation, aAttributes, aSpeed, BOMB_DEFAULT_RANGE, aAngel)
{
	iCurrentFrame = 0;
}

CProjectileBomb::~CProjectileBomb()
{
}

void CProjectileBomb::ConstructL()
{
}

//------------------- functions -------------------------//

void CProjectileBomb::Update()
{
	iCurrentFrame++;
	if(iCurrentFrame == GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE)
	{
		iPixelsPerMoveY -=  TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE);//increase speed over time, simulate gravitional attraction
		iCurrentFrame = 0;
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

void CProjectileBomb::ProjectileBombObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	ProjectileObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrame);
}
void CProjectileBomb::ProjectileBombObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	ProjectileObjectLoadContentFromDisk(aReadStream);
	iCurrentFrame = aReadStream.ReadInt32();
}

void CProjectileBomb::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileBombObjectSaveContentToDisk(aOutputStream);
}
void CProjectileBomb::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileBombObjectLoadContentFromDisk(aReadStream);
}
