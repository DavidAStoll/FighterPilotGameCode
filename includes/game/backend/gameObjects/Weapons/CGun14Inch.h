/*
 ============================================================================
 Name		: Gun14Inch.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGun14Inch declaration
 ============================================================================
 */

#ifndef GUN14INCH_H
#define GUN14INCH_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
class CExplosive14InchGun;
class CShipArteriallyAI;

// CLASS DECLARATION

/**
 *  CGun14Inch
 * 
 */
class CGun14Inch : public CMobileRotatingWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGun14Inch();
	static CGun14Inch* New(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aShip, TInt aNumberOfMuzzles);
	virtual void ShootWithSpeed(TIntFloat aSpeed); //some weapons might need to dictate at what speed the projectile has to fly
	virtual void Draw();// some weapons might want to draw them self
	virtual void Update();
	virtual void ChangeGunAngle(TInt aAngleChange);
	static void InitilizeGun14Inch();
	static void RemoveGun14Inch();
	void Rotate(TInt aChange, TPointIntFloat aLocation);
	CShipArteriallyAI* GetShipArteriallyAI();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CShipArteriallyAI* iGunAI;
	CExplosive14InchGun* iGun;
	CTextureObject* iTurretTextureObject;
	TInt iXOffsetForTurret;
	TInt iYOffsetForTurret;
	TInt iTurrentBaseOffsetAngle; //keeps track of how many degrees the turrent is different from the Ship angle
	/**
	 * Constructor for performing 1st stage construction
	 */
	CGun14Inch(TPointIntFloat* aLocation, TInt* aAngel, TInt aNormalPositionAngel,TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aMinAngle, TInt aMaxAngle, CMoveableGameObject* aTank);
	void ConstructL(TInt aNumberOfMuzzles);
	virtual void UpdateAdditionalWeapons();
	static void LoadGun14InchTextures();
	static void UnloadGun14InchTextures();
	};

#endif // GUN14INCH_H
