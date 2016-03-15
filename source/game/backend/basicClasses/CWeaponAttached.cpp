/*
 * CWeaponAttached.cpp
 *
 *  Created on: May 3, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/basicClasses/CWeaponAttached.h"

CWeaponAttached::CWeaponAttached(TInt aAmmunition,TWeaponIdentifier aWeaponIdentifier, TBool aHasLimitedAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel,TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide,TInt aFramesBetweenFire,TBool aDoNotDraw, const TIntFloat* aSpeed)
:CWeapon(aAmmunition,aWeaponIdentifier, aHasLimitedAmmunition, false, aLocation,aAngel,aXOffset,aYOffset,aReflected,aConflictSide,aFramesBetweenFire)
	{
	  iTextureObject=NULL;
	  iAnimationPlayer=NULL;
	  iDoNotDraw=aDoNotDraw;
	  iSpeed=aSpeed;
	}

CWeaponAttached::~CWeaponAttached()
	{
	  if(iTextureObject)
	    {
	      delete iTextureObject;
	      iTextureObject = NULL;
	    }
	  if(iAnimationPlayer)
	  {
	  	delete iAnimationPlayer;
	  	iAnimationPlayer = NULL;
	  }
	}


//------------------------- functions -------------------------------------//

void CWeaponAttached::Draw()
{
  if(!iDoNotDraw && iTextureObject)
    {
			iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(*iAngel,*iWeaponReflectedOverYAxis,GetRelativeWeaponPosition());
			iTextureObject->DrawArrays();
    }
}

//just makes sure that that the ReadtoShoot flag is reset to true after a certain timeInterval
void CWeaponAttached::Update()
{
	CWeapon::Update();

  if(iAnimationPlayer)
  {
  	iAnimationPlayer->Update();
  }
}

TIntFloat CWeaponAttached::GetXMiddlePointOfWeapon()
{
	TIntFloat lWidthOfWeapon = iTextureObject->GetWidth() / 2;
	TIntFloat lMountingOffset =  CMath::Abs(iCoordinate->iX - GetRelativeWeaponPosition().iX);
	return lWidthOfWeapon + lMountingOffset;
}

void CWeaponAttached::WeaponAttachedObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
  WeaponObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteInt32(iDoNotDraw);
  //Texture Object
  if(iTextureObject)
    {
    aOutputStream.WriteInt32(1);
    iTextureObject->SaveOnDisk(aOutputStream);
    }
  else
    aOutputStream.WriteInt32(0);
  //Animation player
	if(iAnimationPlayer)
	{
		aOutputStream.WriteInt32(true);
		iAnimationPlayer->SaveOnDisk(aOutputStream);
	}
	else
		aOutputStream.WriteInt32(false);
}
void CWeaponAttached::WeaponAttachedObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
  WeaponObjectLoadContentFromDisk(aReadStream);
  iDoNotDraw = aReadStream.ReadInt32();
  //Texture Object
  TInt lHasTextureObject = aReadStream.ReadInt32();
  if(lHasTextureObject == 1)
    iTextureObject->LoadFromDisk(aReadStream);
  //Animation player
	TBool lHasAnimationPlayer = aReadStream.ReadInt32();
	if(lHasAnimationPlayer)
		iAnimationPlayer->LoadFromDisk(aReadStream, iTextureObject);

}

void CWeaponAttached::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponAttachedObjectSaveContentToDisk(aOutputStream);
}

void CWeaponAttached::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponAttachedObjectLoadContentFromDisk(aReadStream);
}

