/*
 ============================================================================
 Name		: Projectile.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CProjectile implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CProjectile.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"

CProjectile::CProjectile(CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aRange, TInt aAngel) :
	CMoveableGameObject(BASIC_PROJECTILE_DEFAULT_Z_COORDINATE, aTextureObject, aAnimationPlayer, aLocation, aAttributes, BASIC_PROJECTILE_DEFAULT_FRAMES_PER_MOVE)
{
	iSpeed = aSpeed;
	iRange = aRange;
	iAngle = aAngel;

	//calculate how fast the object will move via X and Y axis
	//adjust X axis speed
	iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(aAngel);
	//adjust Y axis speed
	iPixelsPerMoveY = iSpeed * CMath::GraphicsSinTable(aAngel);
}

CProjectile::CProjectile(CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aRange, TInt aAngel) :
	CMoveableGameObject(BASIC_PROJECTILE_DEFAULT_Z_COORDINATE, aTextureObject, aAnimationPlayer, aLocation, aAttributes, BASIC_PROJECTILE_DEFAULT_FRAMES_PER_MOVE)
{
	iSpeed = aSpeed;
	iRange = aRange;
	iAngle = aAngel;

	//calculate how fast the object will move via X and Y axis
	//adjust X axis speed
	iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(aAngel);
	//adjust Y axis speed
	iPixelsPerMoveY = iSpeed * CMath::GraphicsSinTable(aAngel);
}

CProjectile::~CProjectile()
{
}

void CProjectile::ConstructL()
{
}
//this method just aligns the texture probably with the current rotation
void CProjectile::AdjustCoordinatesAndTexture()
{
	if(iObjectReflected)
	{
		iTextureObject->ReflectOverYAxis();
		TInt lRotateAngel = iAngle;
		iTextureObject->RotateVertexes(lRotateAngel);
	}else
	{
		TInt lRotateAngel = -180 + iAngle;
		iTextureObject->RotateVertexes(lRotateAngel);
	}

	TPointIntFloat lTextureLowerCorner = iTextureObject->ReturnCurrentFixPoint();
	TPointIntFloat lTextureAdjustmentDueToRotation;
	lTextureAdjustmentDueToRotation.iX = -lTextureLowerCorner.iX + iCoordinates.iX;
	lTextureAdjustmentDueToRotation.iY = -lTextureLowerCorner.iY + iCoordinates.iY;
	iTextureObject->ChangeXCoordinate(lTextureAdjustmentDueToRotation.iX);
	iTextureObject->ChangeYCoordinate(lTextureAdjustmentDueToRotation.iY);
}

//-------------- functions ---------------------


void CProjectile::CProjectile::Move()
{
	//do nothing
}

void CProjectile::Update()
{
	iRange -= iSpeed.GetIntInBaseInt();
	if(iRange < 0)
	{
		Destruct();
	}else
	{
		iCoordinates.iX += iPixelsPerMoveX;
		iCoordinates.iY += iPixelsPerMoveY;
		iTextureObject->ChangeXCoordinate(iPixelsPerMoveX);
		iTextureObject->ChangeYCoordinate(iPixelsPerMoveY);
	}
}

void CProjectile::Destruct()//default behaviour, just set it to dead to remove it
{
	MoveableObjectBasicDieRoutine();
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game
	if(iSoundObject)
	{
		iSoundObject->StopSoundChannel();
	}
}

void CProjectile::SetPixelsPerMoveX(TIntFloat aValue)
{
	iPixelsPerMoveX = aValue;
}

void CProjectile::SetPixelsPerMoveY(TIntFloat aValue)
{
	iPixelsPerMoveY = aValue;
}

void CProjectile::Die(TDamageType aDamageType)
{
	Destruct();
}

void CProjectile::ProjectileObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	MoveableGameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iRange);
}
void CProjectile::ProjectileObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	MoveableGameObjectLoadContentFromDisk(aReadStream);
	iRange = aReadStream.ReadInt32();
}

void CProjectile::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileObjectSaveContentToDisk(aOutputStream);
}
void CProjectile::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileObjectLoadContentFromDisk(aReadStream);
}
