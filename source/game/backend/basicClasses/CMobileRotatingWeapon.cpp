/*
 ============================================================================
 Name		: MobileRotatingWeapon.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CMobileRotatingWeapon implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CMobileRotatingWeapon.h"

CMobileRotatingWeapon::CMobileRotatingWeapon(TPointIntFloat* aLocation, TWeaponIdentifier aWeaponId, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aFramesBetweenFire, TInt aMinAngle, TInt aMaxAngle,
		CMoveableGameObject* aMoveableObject, TInt aGunRotationSpeed, TInt aProjectileSpeed, TInt aMinRange, TInt aMaxRange) :
	CWeaponBomb(1/*not used*/, aWeaponId, aLocation, aAngel, NULL, aXOffset, aYOffset, aReflected, aConflictSide, aFramesBetweenFire, false, aSpeed)
{
	iDefaultOffsetAngleFromPlanePosition = aNormalPositionAngel;
	iRealAngle = 0;
	iOffSetAngleFromNormalPosition = CMath::Abs(aMaxAngle - aMinAngle) / 2; //weapon is by default centered in the middle of its range
	iMinAngle = aMinAngle;
	iMaxAngle = aMaxAngle;
	iMinPositionAngle = 0;
	iMoveableObject = aMoveableObject;
	iGunRotationSpeed = aGunRotationSpeed;
	iProjectileSpeed = aProjectileSpeed;
	iMinRange = aMinRange;
	iMaxRange = aMaxRange;
	//is always false, but variable must be a pointer
	iIsUpSideDownDummy = false;
	iIsUpSideDown = &iIsUpSideDownDummy;
}

CMobileRotatingWeapon::~CMobileRotatingWeapon()
{
}

void CMobileRotatingWeapon::Draw()
{
	if(!iDoNotDraw && !iIsDisabled && iTextureObject)
	{
		//UpdateMobileRotatingWeapon();
		iTextureObject->DrawArrays();
	}
}

//just makes sure that that the ReadtoShoot flag is reset to true after a certain timeInterval
void CMobileRotatingWeapon::Update()
{
	CWeaponBomb::Update();

	//find out what the new real angle is
	TInt lDifferenceBetweenAngels;
	if(*iWeaponReflectedOverYAxis)
		lDifferenceBetweenAngels = *iAngel + iDefaultOffsetAngleFromPlanePosition;
	else
		lDifferenceBetweenAngels = *iAngel - iDefaultOffsetAngleFromPlanePosition;
	CMath::AdjustAngleToUnitCircle(lDifferenceBetweenAngels);
	iMinPositionAngle = lDifferenceBetweenAngels;

	//now need to consider what internal angel the gun has and adjust the current angel appropiate it to it
	if(*iWeaponReflectedOverYAxis)
		iRealAngle = lDifferenceBetweenAngels - iOffSetAngleFromNormalPosition;//since we are not basically reflecting over the Y axis
	else
		iRealAngle = lDifferenceBetweenAngels + iOffSetAngleFromNormalPosition;
	CMath::AdjustAngleToUnitCircle(iRealAngle);
	
	//angle has been updated, now update the texture object
	if(iTextureObject)
	{
		TInt lRotationAngle = iRealAngle;//need to add 90 degrees because texture is 90 degress of the normal angle (180) for a texture
		TPointIntFloat lMiddlePoint = GetRelativeWeaponPosition();

		//resets the rotation of the texture object and rotates it into the proper location
		iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(lRotationAngle, false,lMiddlePoint);
		
		//find what X and Y difference is needed to be centered on the middle point
		TIntFloat lRequiredXDifferenceFromMiddlePoint = (iTextureObject->ReturnTopRightCoordinate().iX - iTextureObject->ReturnLowerRightCoordinate().iX) / 2;//half of the length should the difference be from the middle point
		TIntFloat lActualXDiffereneFromMiddlePoint = lMiddlePoint.iX - iTextureObject->ReturnLowerRightCoordinate().iX;
		TIntFloat lRequiredYDifferenceFromMiddlePoint = (iTextureObject->ReturnTopRightCoordinate().iY - iTextureObject->ReturnLowerRightCoordinate().iY) / 2;//half of the length should the difference be from the middle point
		TIntFloat lActualYDiffereneFromMiddlePoint = lMiddlePoint.iY - iTextureObject->ReturnLowerRightCoordinate().iY;

		//move the object to the centre point, by adjusting the X and Y values
		TPointIntFloat lAdjustmentsRequired;
		lAdjustmentsRequired.iX = lActualXDiffereneFromMiddlePoint - lRequiredXDifferenceFromMiddlePoint;
		lAdjustmentsRequired.iY = lActualYDiffereneFromMiddlePoint - lRequiredYDifferenceFromMiddlePoint;
		iTextureObject->ChangeXCoordinate(lAdjustmentsRequired.iX);
		iTextureObject->ChangeYCoordinate(lAdjustmentsRequired.iY);

		//update location of weapon
		iWeaponCoordinate = iTextureObject->ReturnLowerRightCoordinate();
	}
}

CMoveableGameObject* CMobileRotatingWeapon::GetMoveableObject()
{
	return iMoveableObject;
}

TBool CMobileRotatingWeapon::GetGunIsReflected()
{
	return *iWeaponReflectedOverYAxis;
}

TInt CMobileRotatingWeapon::GetMoveableObjectAngle()
{
	return *iAngel;
}

TInt CMobileRotatingWeapon::GetCurrentGunAngle()
{
	return iRealAngle;
}

TInt CMobileRotatingWeapon::GetGunRotationSpeed()
{
	return iGunRotationSpeed;
}

void CMobileRotatingWeapon::ChangeGunAngle(TInt aAngleChange)
{
	if(*iWeaponReflectedOverYAxis)
		iOffSetAngleFromNormalPosition -= aAngleChange;
	else
		iOffSetAngleFromNormalPosition += aAngleChange;
}

TInt CMobileRotatingWeapon::GetMaxRange()
{
	return iMaxRange;
}

TInt CMobileRotatingWeapon::GetMinRange()
{
	return iMinRange;
}

TInt CMobileRotatingWeapon::GetProjectileSpeed()
{
	return iProjectileSpeed + ((*iSpeed).GetIntInBaseInt());
}

TInt CMobileRotatingWeapon::GetMinAngle()
{
	TInt lMinAngle;
	if(*iWeaponReflectedOverYAxis)
		lMinAngle = iMinPositionAngle - iMaxAngle;
	else
		lMinAngle = iMinPositionAngle;
	CMath::AdjustAngleToUnitCircle(lMinAngle);
	return lMinAngle;
}

TInt CMobileRotatingWeapon::GetMaxAngle()
{
	TInt lMaxAngle;
	if(*iWeaponReflectedOverYAxis)
	{
		if(iMinPositionAngle < GetMinAngle())
			lMaxAngle = iMinPositionAngle + 360;//handle special case
		else
			lMaxAngle = iMinPositionAngle;//handle special case
	}else
		lMaxAngle = iMinPositionAngle + iMaxAngle;
	return lMaxAngle;
}

const TConflictSide& CMobileRotatingWeapon::GetConflictSide()
{
	return *iConflictSide;
}

void CMobileRotatingWeapon::SaveMobileRotatingWeaponOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponBombObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteIntFloat(iWeaponCoordinate.iX);
	aOutputStream.WriteIntFloat(iWeaponCoordinate.iY);
	aOutputStream.WriteInt32(iGunRotationSpeed);
	aOutputStream.WriteInt32(iMinAngle);
	aOutputStream.WriteInt32(iMaxAngle);
	aOutputStream.WriteInt32(iProjectileSpeed);
	aOutputStream.WriteInt32(iMaxRange);
	aOutputStream.WriteInt32(iMinRange);
	aOutputStream.WriteInt32(iMinPositionAngle);
	aOutputStream.WriteInt32(iDefaultOffsetAngleFromPlanePosition);
	aOutputStream.WriteInt32(iOffSetAngleFromNormalPosition);
	aOutputStream.WriteInt32(iRealAngle);
}

void CMobileRotatingWeapon::LoadMobileRotatingWeaponFromDisk(CFileReadStream &aReadStream)
{
	WeaponBombObjectLoadContentFromDisk(aReadStream);
	iWeaponCoordinate.iX = aReadStream.ReadIntFloat();
	iWeaponCoordinate.iY = aReadStream.ReadIntFloat();
	iGunRotationSpeed = aReadStream.ReadInt32();
	iMinAngle = aReadStream.ReadInt32();
	iMaxAngle = aReadStream.ReadInt32();
	iProjectileSpeed = aReadStream.ReadInt32();
	iMaxRange = aReadStream.ReadInt32();
	iMinRange = aReadStream.ReadInt32();
	iMinPositionAngle = aReadStream.ReadInt32();
	iDefaultOffsetAngleFromPlanePosition = aReadStream.ReadInt32();
	iOffSetAngleFromNormalPosition = aReadStream.ReadInt32();
	iRealAngle = aReadStream.ReadInt32();
}

void CMobileRotatingWeapon::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileRotatingWeaponOnDisk(aOutputStream);
}

void CMobileRotatingWeapon::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileRotatingWeaponFromDisk(aReadStream);
}
