/*
 * CWeaponAircraftCannon20mm.h
 *
 *  Created on: Jun 21, 2013
 *      Author: dstoll
 */

#ifndef CWEAPONAIRCRAFTCANNON20MM_H_
#define CWEAPONAIRCRAFTCANNON20MM_H_

#include "includes/game/backend/basicClasses/CWeaponAttachedInvisible.h"

class CWeaponCannon20mm;

class CWeaponAircraftCannon20mm: public CWeaponAttachedInvisible
{

public:

	static CWeaponAircraftCannon20mm* New(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed);
	virtual ~CWeaponAircraftCannon20mm();

	static void InitilizeWeaponAircraftCannon20mm();
	static void RemoveWeaponAircraftCannon20mm();
	virtual void Shoot();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);



protected:
	CWeaponAircraftCannon20mm(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed);

	void Construct();
	virtual void UpdateAdditionalWeapons();
	static void LoadWeaponAircraftCannon20mmTextures();
	static void UnloadWeaponAircraftCannon20mmTextures();

	CWeaponCannon20mm* iGun;
};

#endif /* CWEAPONAIRCRAFTCANNON20MM_H_ */
