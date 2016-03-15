/*
 * CBackGunDoubleMachineGun13mm.h
 *
 *  Created on: Jun 20, 2013
 *      Author: dstoll
 */

#ifndef CBACKGUNDOUBLEMACHINEGUN13MM_H_
#define CBACKGUNDOUBLEMACHINEGUN13MM_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

class CMachineGun13mm;

// CLASS DECLARATION

/**
 *  Use only some of the method of WeaponBomb, since it has already a lot of functionality we can just use it
 *
 */
class CBackGunDoubleMachineGun13mm: public CMobileAAGun
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBackGunDoubleMachineGun13mm();

	/**
	 * Two-phased constructor.
	 */
	static CBackGunDoubleMachineGun13mm* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft);
	//special call just by the small planes, used because it looks better if the gun is aligned with the tail of the plane by default
	static CBackGunDoubleMachineGun13mm* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle);

	static void InitilizeBackGunDoubleMachineGun13mm();
	static void RemoveBackGunDoubleMachineGun13mm();
	virtual void Shoot();
	virtual void Shoot(TInt aRange);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBackGunDoubleMachineGun13mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft);
	CBackGunDoubleMachineGun13mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle);

	virtual void UpdateAdditionalWeapons();
	static void LoadBackGunDoubleMachineGun13mmTextures();
	static void UnloadBackGunDoubleMachineGun13mmTextures();
	void ConstructL();

	CMachineGun13mm* iMachineGun1;
	CMachineGun13mm* iMachineGun2;
};

#endif /* CBACKGUNDOUBLEMACHINEGUN13MM_H_ */
