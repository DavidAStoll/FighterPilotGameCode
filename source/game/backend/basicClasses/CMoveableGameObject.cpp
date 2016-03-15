/*
 * MoveableGameObject.cpp
 *
 *  Created on: 27-Jan-2010
 *      Author: David Stoll
 */

#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/GlobalDefines.h"

CMoveableGameObject::CMoveableGameObject(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer,
                                         TPoint aLocation,SGameObjectAttributes& aAttributes,
                                         TInt aFramesPerMove)
:CGameObject(aZCor,aTextureObject,aAnimationPlayer,aLocation,aAttributes)
{
  iPixelsPerMoveX= TIntFloat::Convert(0);
  iPixelsPerMoveY= TIntFloat::Convert(0);
  //check what directions we are facing
  if(aAttributes.Reflected)//facing right
  	iAngle=0;
  else
  	iAngle=180;
  iCurrentFrame=0;
  iIsSlowlyRecycled = false;
  iIsLeavingMap=false;
  iIsMoveableObject =true;
  iFramesToLive = GFRAMES_PER_SECOND_BACKEND * GSECONDS_UNTIL_DEAD_MOVEABLE_GAME_OBJECT_GETS_RECYCLED;
  iSpeed = TIntFloat::Convert(0);

}

CMoveableGameObject::CMoveableGameObject(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer,
                                         TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,
                                         TInt aFramesPerMove)
:CGameObject(aZCor,aTextureObject,aAnimationPlayer,aLocation,aAttributes)
{
  iPixelsPerMoveX = TIntFloat::Convert(0);
  iPixelsPerMoveY = TIntFloat::Convert(0);
  //default values
  //check what directions we are facing
  if(aAttributes.Reflected)//facing right
  	iAngle=0;
  else
  	iAngle=180;
  iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
  iCurrentFrame=0;
  iIsSlowlyRecycled = false;
  iIsLeavingMap=false;
  iIsMoveableObject =true;
  iFramesToLive = GFRAMES_PER_SECOND_BACKEND * GSECONDS_UNTIL_DEAD_MOVEABLE_GAME_OBJECT_GETS_RECYCLED;

}

CMoveableGameObject::~CMoveableGameObject()
{
 }
//-----------------------functions----------------------------

void CMoveableGameObject::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once

	MoveableObjectBasicDieRoutine();
	GameObjectMoveIntoBackground();
}

const TInt& CMoveableGameObject::GetAngle()
{
	return iAngle;
}

TInt& CMoveableGameObject::GetFramesToLive()
{
  return iFramesToLive;
}

TBool CMoveableGameObject::GetIsLeavingMap()
{
  return iIsLeavingMap;
}

TBool CMoveableGameObject::GetIsMoving()
{
	if(iPixelsPerMoveX == TIntFloat::Convert(0) && iPixelsPerMoveY == TIntFloat::Convert(0))
	{
		return false;
	}
	else
	{
		return true;
	}
}

TBool& CMoveableGameObject::GetIsSlowlyRecycledRef()
{
	return iIsSlowlyRecycled;
}

TIntFloat * CMoveableGameObject::GetPixelsPerMoveX()
{
  return &iPixelsPerMoveX;
}

TIntFloat * CMoveableGameObject::GetPixelsPerMoveY()
{
  return &iPixelsPerMoveY;
}

void CMoveableGameObject::Move()
{
	iCoordinates.iX += iPixelsPerMoveX;
	if(iTextureObject)
		iTextureObject->ChangeXCoordinate(iPixelsPerMoveX);
}

void CMoveableGameObject::MoveableObjectBasicDieRoutine()
{
	if(!iAlive)
		return; //can only die once

	iPixelsPerMoveX = TIntFloat::Convert(0);
	iPixelsPerMoveY = TIntFloat::Convert(0);
	iSpeed = TIntFloat::Convert(0);
	GameObjectBasicDieRoutine();
}

void CMoveableGameObject::MoveableGameObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
  GameObjectLoadContentFromDisk(aReadStream);
  iPixelsPerMoveX = aReadStream.ReadIntFloat();
  iPixelsPerMoveY = aReadStream.ReadIntFloat();
  iIsSlowlyRecycled = aReadStream.ReadInt32();
  iFramesToLive = aReadStream.ReadInt32();
  iAngle = aReadStream.ReadInt32();
  iSpeed = aReadStream.ReadIntFloat();
  iIsLeavingMap = aReadStream.ReadInt32();
  iCurrentFrame = aReadStream.ReadInt32();
}
void CMoveableGameObject::MoveableGameObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
  GameObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteIntFloat(iPixelsPerMoveX);
  aOutputStream.WriteIntFloat(iPixelsPerMoveY);
  aOutputStream.WriteInt32(iIsSlowlyRecycled);
  aOutputStream.WriteInt32(iFramesToLive);
  aOutputStream.WriteInt32(iAngle);
  aOutputStream.WriteIntFloat(iSpeed);
  aOutputStream.WriteInt32(iIsLeavingMap);
  aOutputStream.WriteInt32(iCurrentFrame);
}

void CMoveableGameObject::SetSpeed(TIntFloat aValue)
{
	iSpeed = aValue;
  iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

// stub function
void CMoveableGameObject::SetIsLeavingMap(TBool aValue)
{
  iIsLeavingMap = aValue;
}

void CMoveableGameObject::SetPixelsPerMoveX(TIntFloat aValue)
{
	iPixelsPerMoveX = aValue;
}

void CMoveableGameObject::SetPixelsPerMoveY(TIntFloat aValue)
{
	iPixelsPerMoveY = aValue;
}

void CMoveableGameObject::TurnLeft()
{
	iAngle = 180;
	iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
	iObjectReflected = false;
	if(iTextureObject && iTextureObject->IsReflectedOverYAxis())
	{
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

void CMoveableGameObject::TurnRight()
{
	iAngle = 0;
	iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
	iObjectReflected = true;
	if(iTextureObject && !iTextureObject->IsReflectedOverYAxis())
	{
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

