/*
 ============================================================================
 Name		: TankAAGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTankAAGun declaration
 ============================================================================
 */

#ifndef TANKAAGUN_H
#define TANKAAGUN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"

// CLASS DECLARATION
class CGun20mm;

/**
 *  CTankAAGun
 * 
 */
class CTankAAGun : public CMobileAAGun
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTankAAGun();

	/**
	 * Two-phased constructor.
	 */
	static CTankAAGun* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank);

	virtual void Shoot(TInt aRange);
	static void InitilizeTankAAGun();
	static void RemoveTankAAGun();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:
	
	CMachineGun13mm* iLeftMachineGun;
	CMachineGun13mm* iRightMachineGun;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CTankAAGun(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank);
	void ConstructL();
	virtual void UpdateAdditionalWeapons();
	static void LoadTankAAGunTextures();
	static void UnloadTankAAGunTextures();

	};

#endif // TANKAAGUN_H
