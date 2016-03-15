/*
 ============================================================================
 Name		: Weapon.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeapon implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CWeapon.h"

CWeapon::CWeapon(TInt aAmmunition, TWeaponIdentifier aWeaponIdentifier, TBool aHasLimitedAmmunition, TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aFramesBetweenFire)
{
	iAmmunitionLeft = aAmmunition;
	iMaxAmmunitionLeft = aAmmunition;
	ihasLimitedAmmunition = aHasLimitedAmmunition;
	iFramesBetweenFire = aFramesBetweenFire;
	iCurrentFrame = 0;
	iReadyToShoot = true;
	iIsDisabled = aDisabled;
	iCoordinate = aLocation;
	iAngel = aAngel;
	iXOffset = aXOffset;
	iYOffset = aYOffset;
	iWeaponReflectedOverYAxis = aReflected;
	iConflictSide = aConflictSide;
	iWeaponIdentifier = aWeaponIdentifier;
	iSoundObject = NULL;
}

CWeapon::~CWeapon()
{
	if(iSoundObject)
	{
		delete iSoundObject;
		iSoundObject = NULL;
	}
}
//---------------------functions-----------------------

TBool CWeapon::HasLimitedAmmunition()
{
	return ihasLimitedAmmunition;
}

TInt CWeapon::GetAmmunitionLeft()
{
	return iAmmunitionLeft;
}

TBool CWeapon::GetIsReadyToShoot()
{
	return iReadyToShoot;
}

TInt CWeapon::GetMaxAmmunitionLeft()
{
	return iMaxAmmunitionLeft;
}

TBool CWeapon::IsDisabled()
{
	return iIsDisabled;
}

void CWeapon::SetDisabled(TBool aValue)
{
	iIsDisabled = aValue;
}

TPointIntFloat CWeapon::GetRelativeWeaponPositionWithAngel(TInt& aAngle)
{
	if(*iWeaponReflectedOverYAxis)
	{
		TPointIntFloat lCoordinate = *iCoordinate;
		lCoordinate.iX += (CMath::GraphicsCosTable(aAngle) * iXOffset) - (CMath::GraphicsSinTable(aAngle) * iYOffset);
		lCoordinate.iY += (CMath::GraphicsCosTable(aAngle) * iYOffset) + (CMath::GraphicsSinTable(aAngle) * iXOffset);
		return lCoordinate;
	}else
	{
		TPointIntFloat lCoordinate = *iCoordinate;
		lCoordinate.iX += (CMath::GraphicsCosTable(aAngle) * iXOffset) + (CMath::GraphicsSinTable(aAngle) * iYOffset);
		lCoordinate.iY += (-CMath::GraphicsCosTable(aAngle) * iYOffset) + (CMath::GraphicsSinTable(aAngle) * iXOffset);
		return lCoordinate;
	}
}

TPointIntFloat CWeapon::GetRelativeWeaponPosition(TInt aExtraXOffset, TInt aExtraYOffset)
{
	if(*iWeaponReflectedOverYAxis)
	{
		TPointIntFloat lCoordinate = *iCoordinate;
		lCoordinate.iX += (CMath::GraphicsCosTable(*iAngel) * (iXOffset + aExtraXOffset)) - (CMath::GraphicsSinTable(*iAngel) * (iYOffset + aExtraYOffset));
		lCoordinate.iY += (CMath::GraphicsCosTable(*iAngel) * (iYOffset + aExtraYOffset)) + (CMath::GraphicsSinTable(*iAngel) * (iXOffset + aExtraXOffset));
		return lCoordinate;
	}else
	{
		TPointIntFloat lCoordinate = *iCoordinate;
		lCoordinate.iX += (CMath::GraphicsCosTable(*iAngel) * (iXOffset + aExtraXOffset)) + (CMath::GraphicsSinTable(*iAngel) * (iYOffset + aExtraYOffset));
		lCoordinate.iY += (-CMath::GraphicsCosTable(*iAngel) * (iYOffset + aExtraYOffset)) + (CMath::GraphicsSinTable(*iAngel) * (iXOffset + aExtraXOffset));
		return lCoordinate;
	}
}

TPointIntFloat CWeapon::GetRelativeWeaponPositionWithoutReflection()
{
	TPointIntFloat lCoordinate = *iCoordinate;
	lCoordinate.iX += (CMath::GraphicsCosTable(*iAngel) * iXOffset) + (CMath::GraphicsSinTable(*iAngel) * iYOffset);
	lCoordinate.iY += (-CMath::GraphicsCosTable(*iAngel) * iYOffset) + (CMath::GraphicsSinTable(*iAngel) * iXOffset);
	return lCoordinate;
}

TPointIntFloat CWeapon::GetRelativeWeaponPosition()
{
	if(*iWeaponReflectedOverYAxis)
	{
		TPointIntFloat lCoordinate = *iCoordinate;
		lCoordinate.iX += (CMath::GraphicsCosTable(*iAngel) * iXOffset) - (CMath::GraphicsSinTable(*iAngel) * iYOffset);
		lCoordinate.iY += (CMath::GraphicsCosTable(*iAngel) * iYOffset) + (CMath::GraphicsSinTable(*iAngel) * iXOffset);
		return lCoordinate;
	}else
	{
		TPointIntFloat lCoordinate = *iCoordinate;
		lCoordinate.iX += (CMath::GraphicsCosTable(*iAngel) * iXOffset) + (CMath::GraphicsSinTable(*iAngel) * iYOffset);
		lCoordinate.iY += (-CMath::GraphicsCosTable(*iAngel) * iYOffset) + (CMath::GraphicsSinTable(*iAngel) * iXOffset);
		return lCoordinate;
	}
}

void CWeapon::ResetAmmunitionToMaxValue()
{
	iAmmunitionLeft = iMaxAmmunitionLeft;
}

void CWeapon::SetAmmunitionToValue(TInt aAmount)
{
	iAmmunitionLeft = aAmount;
}

//stub function
void CWeapon::Shoot()
{
	// do Nothing, since we don't know what kind of attack we will be using
}

//stub function
void CWeapon::Shoot(TInt aRange)
{
	//Do nothing, child needs to implemented functionality
}

//stub function
void CWeapon::Shoot(TInt aRange, TInt aAngleOffset)
{

}

void CWeapon::ShootWithSpeed(TIntFloat aSpeed)
{

}

//stub function
void CWeapon::Draw()
{

}

void CWeapon::Update()
{
  iCurrentFrame++;
  if(iReadyToShoot || iCurrentFrame == iFramesBetweenFire)
  {
  	iReadyToShoot=true;
    iCurrentFrame=0;
  }

  UpdateAdditionalWeapons();
}

TWeaponIdentifier CWeapon::GetWeaponIdentifier()
{
	return iWeaponIdentifier;
}

//stub function
void CWeapon::UpdateAdditionalWeapons()
{
	//nothing
}

void CWeapon::WeaponObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iWeaponIdentifier); //used by the calling method to find out what weapon it needs to create it, will not be read by weapon
	aOutputStream.WriteInt32(iAmmunitionLeft);
	aOutputStream.WriteInt32(iMaxAmmunitionLeft);
	aOutputStream.WriteInt32(ihasLimitedAmmunition);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iFramesBetweenFire);
	aOutputStream.WriteInt32(iReadyToShoot);
	aOutputStream.WriteInt32(iIsDisabled);
	aOutputStream.WriteInt32(iXOffset);
	aOutputStream.WriteInt32(iYOffset);
	if(iSoundObject)
	{
		aOutputStream.WriteInt32(1);
		iSoundObject->SaveOnDisk(aOutputStream);
	}else
		aOutputStream.WriteInt32(0);
}

void CWeapon::WeaponObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	iAmmunitionLeft = aReadStream.ReadInt32();
	iMaxAmmunitionLeft = aReadStream.ReadInt32();
	ihasLimitedAmmunition = aReadStream.ReadInt32();
	iCurrentFrame = aReadStream.ReadInt32();
	iFramesBetweenFire = aReadStream.ReadInt32();
	iReadyToShoot = aReadStream.ReadInt32();
	iIsDisabled = aReadStream.ReadInt32();
	iXOffset = aReadStream.ReadInt32();
	iYOffset = aReadStream.ReadInt32();
	TInt lSoundObjectDefined = aReadStream.ReadInt32();
	if(lSoundObjectDefined == 1)
		iSoundObject->LoadFromDisk(aReadStream);
}

void CWeapon::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponObjectSaveContentToDisk(aOutputStream);
}
void CWeapon::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponObjectLoadContentFromDisk(aReadStream);
}
