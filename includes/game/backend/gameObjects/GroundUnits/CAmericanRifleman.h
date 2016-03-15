/*
 ============================================================================
 Name		: AmericanRifleman.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanRifleman declaration
 ============================================================================
 */

#ifndef AMERICANRIFLEMAN_H
#define AMERICANRIFLEMAN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"
#include "includes/game/backend/gameObjects/Weapons/CRifle.h"

// CLASS DECLARATION

/**
 *  CAmericanRifleman
 * 
 */
class CAmericanRifleman: public CGroundUnit
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanRifleman();
	static CAmericanRifleman* New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected);

	virtual void Shoot();//stub function,causes the the unit to fire
	static void InitilizeAmericanRifleman();
	static void RemoveAmericanRifleman();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAmericanRifleman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateWeapon(); //should contain all the code to update all weapons that are attached to this unit
	static void LoadAmericanRiflemanTextures();
	static void UnloadAmericanRiflemanTextures();
	static void CreateAmericanRiflemanCollisionPointsL();
	static void RemoveAmericanRiflemanCollisionPoints();

	CRifle* iRifle;
	static SBoundingCollisionPoint* CollisionPoints;
};

#endif // AMERICANRIFLEMAN_H
