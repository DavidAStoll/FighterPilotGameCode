/*
 * CWeaponAttachedInvisible.cpp
 *
 *  Created on: May 3, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/basicClasses/CWeaponAttachedInvisible.h"

CWeaponAttachedInvisible::CWeaponAttachedInvisible(TInt aAmmunition, TWeaponIdentifier aWeaponIdentifier, TBool aHasLimitedAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TInt aFramesBetweenFire, TBool aDoNotDraw, TIntFloat* aSpeed,
		 TInt aFramesForFireAnimation)
:CWeaponAttached(aAmmunition, aWeaponIdentifier, aHasLimitedAmmunition, aLocation, aAngel, aXOffset, aYOffset, aReflected, aConflictSide, aFramesBetweenFire, aDoNotDraw, aSpeed)
{
	iTotalFramesInFireAnimation = aFramesForFireAnimation;
	iFrameInFireAnimation = 0;
}

CWeaponAttachedInvisible::~CWeaponAttachedInvisible()
{
	// TODO Auto-generated destructor stub
}

void CWeaponAttachedInvisible::Draw()
{

	if(iFrameInFireAnimation != 0 && iFrameInFireAnimation < iTotalFramesInFireAnimation)
		iFrameInFireAnimation++;
	else
		iFrameInFireAnimation = 0;

  if(!iDoNotDraw && iTextureObject && iFrameInFireAnimation > 0)
    {
			iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(*iAngel, *iWeaponReflectedOverYAxis, GetRelativeWeaponPosition());
			iTextureObject->DrawArrays();
    }
}

void CWeaponAttachedInvisible::WeaponAttachedInvisibleObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
  WeaponAttachedObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteInt32(iTotalFramesInFireAnimation); //used by the calling method to find out what weapon it needs to create it, will not be read by weapon
  aOutputStream.WriteInt32(iFrameInFireAnimation);
}
void CWeaponAttachedInvisible::WeaponAttachedInvisibleObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
  WeaponAttachedObjectLoadContentFromDisk(aReadStream);
  iTotalFramesInFireAnimation = aReadStream.ReadInt32();
  iFrameInFireAnimation = aReadStream.ReadInt32();
}

void CWeaponAttachedInvisible::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponAttachedInvisibleObjectSaveContentToDisk(aOutputStream);
}

void CWeaponAttachedInvisible::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponAttachedInvisibleObjectLoadContentFromDisk(aReadStream);
}

