/*
 ============================================================================
 Name		: Arterially.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CArterially implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CAAArterially.h"

CAAArterially::CAAArterially(TPoint aLocation,SGameObjectAttributes& aAttributes,TInt aRange,TInt aMinRange,TInt aMinAngel,TInt aGunRotationSpeed,TInt aProjectileSpeed,TInt aProjectileExplosionWidth)
:CBuilding(AAARTERIALLY_DEFAULT_Z_VALUE,NULL,NULL,aLocation,aAttributes)
	{
	iRange= aRange;
	iMinRange = aMinRange;
	iMinAngel = aMinAngel;
	iGunAngel=90; // since gun is looking straight up in default position
	iGunRotationSpeed=aGunRotationSpeed;
	iProjectileSpeed = aProjectileSpeed;
	iProjectileExplosionWidth = aProjectileExplosionWidth*4;//increase it since calculation is not accurate
	iAI=NULL;
	}

CAAArterially::~CAAArterially()
	{
	  if(iAI)
	    {
	      delete iAI;
	      iAI=NULL;
	    }
	}

//------------------- functions --------------------//


void CAAArterially::RepositionGun()
{
  //stub function
}

void CAAArterially::Shoot(TInt aRange)
{
  //stub function
	aRange = 0; //remove warning
}

TInt CAAArterially::GetProjectileSpeed()
{
  return iProjectileSpeed;
}

void CAAArterially::ChangeGunAngel(TInt aChange)
{
  iTextureObject->RotateVertexes(aChange);
  iGunAngel+=aChange;
  RepositionGun();
}

TInt CAAArterially::GetGunRotationSpeed()
{
 return iGunRotationSpeed; 
}

TInt CAAArterially::GetCurrentGunAngel()
{
  return iGunAngel;
}

TInt CAAArterially::GetRange()
{
  return iRange;
}

TPointIntFloat& CAAArterially::GetGunCoordinates()
{
  return iGunCoordinates;
}

TInt CAAArterially::GetGunMinAngel()
{
  return iMinAngel;
}

TInt CAAArterially::GetMinRange()
{
  return iMinRange;
}

TInt CAAArterially::GetProjectileExplosionWidth()
{
  return iProjectileExplosionWidth;
}

void  CAAArterially::AAArteriallyObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
  
  BuildingSaveContentToDisk(aOutputStream);
  aOutputStream.WriteInt32(iRange);
  aOutputStream.WriteInt32(iMinRange);
  aOutputStream.WriteInt32(iGunRotationSpeed);
  aOutputStream.WriteIntFloat(iGunCoordinates.iX);
  aOutputStream.WriteIntFloat(iGunCoordinates.iY);
  aOutputStream.WriteInt32(iGunAngel);
  aOutputStream.WriteInt32(iMinAngel);
  aOutputStream.WriteInt32(iProjectileSpeed);
  aOutputStream.WriteInt32(iProjectileExplosionWidth);
  iAI->SaveOnDisk(aOutputStream);
}

void CAAArterially::AAArteriallyObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
  BuildingLoadContentFromDisk(aReadStream);
  iRange = aReadStream.ReadInt32();
  iMinRange = aReadStream.ReadInt32();
  iGunRotationSpeed = aReadStream.ReadInt32();
  iGunCoordinates.iX = aReadStream.ReadIntFloat();
  iGunCoordinates.iY = aReadStream.ReadIntFloat();
  iGunAngel = aReadStream.ReadInt32();
  iMinAngel = aReadStream.ReadInt32();
  iProjectileSpeed = aReadStream.ReadInt32();
  iProjectileExplosionWidth = aReadStream.ReadInt32();
  iAI->LoadFromDisk(aReadStream);
}
