/*
 ============================================================================
 Name		: MobileAAGun.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CMobileAAGun implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CMobileAAGun.h"

CMobileAAGun::CMobileAAGun(TPointIntFloat* aLocation, TWeaponIdentifier aWeaponId, TInt* aAngel, TInt aNormalPositionAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aFramesBetweenFire, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aMoveableObject,
		TInt aGunRotationSpeed, TInt aProjectileSpeed, TInt aMinRange, TInt aMaxRange) :
	CMobileRotatingWeapon(aLocation, aWeaponId, aAngel, aNormalPositionAngel, aXOffset, aYOffset, aReflected, aConflictSide, aSpeed, aFramesBetweenFire, aMinAngle, aMaxAngle, aMoveableObject, aGunRotationSpeed, aProjectileSpeed, aMinRange, aMaxRange)
{
	iBackCannonAI = NULL;
}

CMobileAAGun::~CMobileAAGun()
{
	if(iBackCannonAI)
	{
		delete iBackCannonAI;
		iBackCannonAI = NULL;
	}
}

// ----------------------- functions ---------------------------------//

void CMobileAAGun::Update()
{
	CMobileRotatingWeapon::Update();

	//run AI for AA Gun
	if(iBackCannonAI && iMoveableObject->IsAlive())
		iBackCannonAI->Update();//don't let AI run if aircraft is dead
}

void CMobileAAGun::SaveMobileAAGunOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileRotatingWeaponOnDisk(aOutputStream);
	if(iBackCannonAI)
	{
		aOutputStream.WriteInt32(true);
		iBackCannonAI->SaveOnDisk(aOutputStream);
	}
	else
		aOutputStream.WriteInt32(false);
}

void CMobileAAGun::LoadMobileAAGunFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileRotatingWeaponFromDisk(aReadStream);
	TBool lHasAIObject = aReadStream.ReadInt32();
	if(lHasAIObject)
		iBackCannonAI->LoadFromDisk(aReadStream);
}

void CMobileAAGun::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	SaveMobileAAGunOnDisk(aOutputStream);
}

void CMobileAAGun::LoadFromDisk(CFileReadStream &aReadStream)
{
	LoadMobileAAGunFromDisk(aReadStream);
}
