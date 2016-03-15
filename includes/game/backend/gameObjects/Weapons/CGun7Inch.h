/*
 ============================================================================
 Name		: Gun7Inch.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGun7Inch declaration
 ============================================================================
 */

#ifndef GUN7INCH_H
#define GUN7INCH_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
class CExplosive7InchGun;
class CShipArteriallyAI;

// CLASS DECLARATION

/**
 *  CGun7Inch
 * 
 */
class CGun7Inch : public CMobileRotatingWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGun7Inch();
	static CGun7Inch* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip, TInt aNumberOfMuzzles, TBool aUseCamoTextures = false);
	virtual void ShootWithSpeed(TIntFloat aSpeed); //some weapons might need to dictate at what speed the projectile has to fly
	virtual void Draw();// some weapons might want to draw them self
	virtual void Update();
	virtual void ChangeGunAngle(TInt aAngleChange);
	static void InitilizeGun7Inch();
	static void RemoveGun7Inch();
	static void InitilizeCamoGun7Inch();
	static void RemoveCamoGun7Inch();
	void Rotate(TInt aChange, TPointIntFloat aLocation);
	CShipArteriallyAI* GetShipArteriallyAI();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:
	
	CShipArteriallyAI* iGunAI;
	CExplosive7InchGun* iGun;
	CTextureObject* iTurretTextureObject;
	TInt iXOffsetForTurret;
	TInt iYOffsetForTurret;
	TInt iTurrentBaseOffsetAngle; //keeps track of how many degrees the turrent is different from the Ship angle
	TBool iUseCamoTexture;

	/**
	 * Constructor for performing 1st stage construction
	 */
	CGun7Inch(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank,TBool aUseCamoTextures);
	void ConstructL(TInt aNumberOfMuzzles);
	virtual void UpdateAdditionalWeapons();
	static void LoadGun7InchTextures();
	static void UnloadGun7InchTextures();
	static void LoadCamoGun7InchTextures();
	static void UnloadCamoGun7InchTextures();
	};

#endif // GUN7INCH_H
