/*
 * CWeaponAircraftMachineGun8mm.h
 *
 *  Created on: Jun 18, 2013
 *      Author: dstoll
 */

#ifndef CWEAPONAIRCRAFTMACHINEGUN8MM_H_
#define CWEAPONAIRCRAFTMACHINEGUN8MM_H_

#include "includes/game/backend/basicClasses/CWeaponAttachedInvisible.h"

class CMachineGun8mm;

class CWeaponAircraftMachineGun8mm: public CWeaponAttachedInvisible
{
public:

	static CWeaponAircraftMachineGun8mm* New(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed);
	virtual ~CWeaponAircraftMachineGun8mm();

	static void InitilizeWeaponAircraftMachineGun8mm();
	static void RemoveWeaponAircraftMachineGun8mm();
	virtual void Shoot();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);



protected:
	CWeaponAircraftMachineGun8mm(TPointIntFloat* aLocation,TInt* aAngel, TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide, TBool aDoNotDraw,TIntFloat* aObjectSpeed);

	void Construct();
	virtual void UpdateAdditionalWeapons();
	static void LoadWeaponAircraftMachineGun8mmTextures();
	static void UnloadWeaponAircraftMachineGun8mmTextures();

	CMachineGun8mm* iGun;
};

#endif /* CWEAPONAIRCRAFTMACHINEGUN8MM_H_ */
