/*
 ============================================================================
 Name		: WeaponBomb.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBomb implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CWeaponBomb.h"

CWeaponBomb::CWeaponBomb(TInt aAmmunition,TWeaponIdentifier aWeaponIdentifier,const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown,TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide,TInt aFramesBetweenFire,TBool aDoNotDraw, const TIntFloat* aSpeed)
:CWeaponAttached(aAmmunition, aWeaponIdentifier, true, aLocation, aAngel,aXOffset,aYOffset,aReflected,aConflictSide,aFramesBetweenFire, aDoNotDraw, aSpeed)
	{

	  iIsUpSideDown=aUpSideDown;
	}

CWeaponBomb::~CWeaponBomb()
	{

	}

//------------------------- functions -------------------------------------//

void CWeaponBomb::Draw()
{
  if(!iDoNotDraw && !iIsDisabled && iTextureObject && iAmmunitionLeft>0)
    {
			iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(*iAngel,*iWeaponReflectedOverYAxis,GetRelativeWeaponPosition());
			iTextureObject->DrawArrays();
    }
}

//stub
void CWeaponBomb::Explode()
{
  //some weapons might want to explode
}

void CWeaponBomb::WeaponBombObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
  WeaponAttachedObjectSaveContentToDisk(aOutputStream);
}
void CWeaponBomb::WeaponBombObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	WeaponAttachedObjectLoadContentFromDisk(aReadStream);
}
