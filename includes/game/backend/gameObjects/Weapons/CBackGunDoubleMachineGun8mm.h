/*
 * CBackGunDoubleDoubleMachineGun8mm.h
 *
 *  Created on: Jun 20, 2013
 *      Author: dstoll
 */

#ifndef CBackGunDoubleDOUBLEMACHINEGUN8MM_H_
#define CBackGunDoubleDOUBLEMACHINEGUN8MM_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

class CMachineGun8mm;

// CLASS DECLARATION

/**
 *  Use only some of the method of WeaponBomb, since it has already a lot of functionality we can just use it
 *
 */
class CBackGunDoubleMachineGun8mm: public CMobileAAGun
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBackGunDoubleMachineGun8mm();

	/**
	 * Two-phased constructor.
	 */
	static CBackGunDoubleMachineGun8mm* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft);
	//special call just by the small planes, used because it looks better if the gun is aligned with the tail of the plane by default
	static CBackGunDoubleMachineGun8mm* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle);

	static void InitilizeBackGunDoubleMachineGun8mm();
	static void RemoveBackGunDoubleMachineGun8mm();
	virtual void Shoot();
	virtual void Shoot(TInt aRange);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBackGunDoubleMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft);
	CBackGunDoubleMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle);

	virtual void UpdateAdditionalWeapons();
	static void LoadBackGunDoubleMachineGun8mmTextures();
	static void UnloadBackGunDoubleMachineGun8mmTextures();
	void ConstructL();

	CMachineGun8mm* iMachineGun1;
	CMachineGun8mm* iMachineGun2;
};

#endif /* CBackGunDoubleDOUBLEMACHINEGUN8MM_H_ */
