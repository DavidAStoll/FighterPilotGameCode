/*
 ============================================================================
 Name		: DauntlessBackCannon.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBackGunMachineGun8mm declaration
 ============================================================================
 */

#ifndef DAUNTLESSBACKCANNON_H
#define DAUNTLESSBACKCANNON_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

class CMachineGun8mm;

// CLASS DECLARATION

/**
 *  Use only some of the method of WeaponBomb, since it has already a lot of functionality we can just use it
 * 
 */
class CBackGunMachineGun8mm: public CMobileAAGun
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBackGunMachineGun8mm();

	/**
	 * Two-phased constructor.
	 */
	static CBackGunMachineGun8mm* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft);
	//special call just by the small planes, used because it looks better if the gun is aligned with the tail of the plane by default
	static CBackGunMachineGun8mm* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle);

	static void InitilizeBackGunMachineGun8mm();
	static void RemoveBackGunMachineGun8mm();
	virtual void Shoot();
	virtual void Shoot(TInt aRange);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBackGunMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft);
	CBackGunMachineGun8mm(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CAircraft* aAircraft, TInt aDefaultOffsetAngle);
	
	virtual void UpdateAdditionalWeapons();
	static void LoadBackGunMachineGun8mmTextures();
	static void UnloadBackGunMachineGun8mmTextures();
	void ConstructL();
	
	CMachineGun8mm* iMachineGun;

};

#endif // DAUNTLESSBACKCANNON_H
