/*
 ============================================================================
 Name		: ShipDual75mmGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipDual75mmGun declaration
 ============================================================================
 */

#ifndef SHIPDUAL75MMGUN_H
#define SHIPDUAL75MMGUN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"

// CLASS DECLARATION

/**
 *  CShipDual75mmGun
 * 
 */
class CShipDual75mmGun : public CMobileAAGun
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CShipDual75mmGun();
	static CShipDual75mmGun* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip);
	static CShipDual75mmGun* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip, TBool aUseCamoTexture);
	virtual void Shoot(TInt aRange);
	virtual void Draw();// some weapons might want to draw them self
	static void InitilizeShipDual75mmGun();
	static void RemoveShipDual75mmGun();
	static void InitilizeCamoShipDual75mmGun();
	static void RemoveCamoShipDual75mmGun();
	void Rotate(TInt aChange, TPointIntFloat aLocation);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:
	
	CGunAA75mm* iLeftGun;
	CGunAA75mm* iMiddleGun;
	CGunAA75mm* iRightGun;
	CTextureObject* iTurretTextureObject;
	TInt iXOffsetForTurret;
	TInt iYOffsetForTurret;
	TInt iTurrentBaseOffsetAngle;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CShipDual75mmGun(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank);
	void ConstructL(TBool aUseCamoTexture);
	virtual void UpdateAdditionalWeapons();
	static void LoadShipDual75mmGunTextures();
	static void UnloadShipDual75mmGunTextures();
	static void LoadCamoShipDual75mmGunTextures();
	static void UnloadCamoShipDual75mmGunTextures();

	};

#endif // SHIPDUAL75MMGUN_H
