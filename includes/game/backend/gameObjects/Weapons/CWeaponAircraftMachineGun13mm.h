/*
 * CWeaponAircraftMachineGun13mm.h
 *
 *  Created on: May 3, 2013
 *      Author: dstoll
 */

#ifndef CWeaponAircraftMachineGun13mm_H_
#define CWeaponAircraftMachineGun13mm_H_

#include "includes/game/backend/basicClasses/CWeaponAttachedInvisible.h"

class CMachineGun13mm;

class CWeaponAircraftMachineGun13mm: public CWeaponAttachedInvisible
{
public:

	static CWeaponAircraftMachineGun13mm* New(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed);
	virtual ~CWeaponAircraftMachineGun13mm();

	static void InitilizeWeaponAircraftMachineGun13mm();
	static void RemoveWeaponAircraftMachineGun13mm();
	virtual void Shoot();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);



protected:
	CWeaponAircraftMachineGun13mm(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed);

	void Construct();
	virtual void UpdateAdditionalWeapons();
	static void LoadWeaponAircraftMachineGun13mmTextures();
	static void UnloadWeaponAircraftMachineGun13mmTextures();

	CMachineGun13mm* iGun;
};

#endif /* CWeaponAircraftMachineGun13mm_H_ */
