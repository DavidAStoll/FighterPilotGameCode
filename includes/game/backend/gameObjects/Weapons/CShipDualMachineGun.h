/*
 ============================================================================
 Name		: ShipDualMachineGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipDualMachineGun declaration
 ============================================================================
 */

#ifndef SHIPDUALMACHINEGUN_H
#define SHIPDUALMACHINEGUN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"

// CLASS DECLARATION

/**
 *  CShipDualMachineGun
 * 
 */
class CShipDualMachineGun : public CMobileAAGun
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CShipDualMachineGun();

	static CShipDualMachineGun* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip);
	static CShipDualMachineGun* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip, TBool aUseCamoTexture);
	virtual void Shoot(TInt aRange);
	virtual void Draw();// some weapons might want to draw them self
	static void InitilizeShipDualMachineGun();
	static void RemoveShipDualMachineGun();
	static void InitilizeCamoShipDualMachineGun();
	static void RemoveCamoShipDualMachineGun();
	void Rotate(TInt aChange, TPointIntFloat aLocation);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:
	
	CMachineGun13mm* iLeftMachineGun;
	CMachineGun13mm* iRightMachineGun;
	CTextureObject* iTurretTextureObject;
	TInt iXOffsetForTurret;
	TInt iYOffsetForTurret;
	TInt iTurrentBaseOffsetAngle;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CShipDualMachineGun(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank);
	void ConstructL(TBool aUseCamoTexture);
	virtual void UpdateAdditionalWeapons();
	static void LoadShipDualMachineGunTextures();
	static void UnloadShipDualMachineGunTextures();
	static void LoadCamoShipDualMachineGunTextures();
	static void UnloadCamoShipDualMachineGunTextures();

	};

#endif // SHIPDUALMACHINEGUN_H
