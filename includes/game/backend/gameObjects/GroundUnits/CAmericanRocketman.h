/*
 ============================================================================
 Name		: AmericanRocketman.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanRocketman declaration
 ============================================================================
 */

#ifndef AMERICANROCKETMAN_H
#define AMERICANROCKETMAN_H

#include "includes/game/backend/basicClasses/CGroundUnit.h"
class CGrenadeLauncher;

// CLASS DECLARATION

/**
 *  CAmericanRocketman
 * 
 */
class CAmericanRocketman: public CGroundUnit
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanRocketman();

	static CAmericanRocketman* New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected);

	virtual void Shoot();//stub function,causes the the unit to fire
	static void InitilizeAmericanRocketman();
	static void RemoveAmericanRocketman();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAmericanRocketman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();
	
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateWeapon(); //should contain all the code to update all weapons that are attached to this unit
	static void LoadAmericanRocketmanTextures();
	static void UnloadAmericanRocketmanTextures();
	static void CreateAmericanRocketmanCollisionPointsL();
	static void RemoveAmericanRocketmanCollisionPoints();

	CGrenadeLauncher* iGrendaeLauncher;
	static SBoundingCollisionPoint* CollisionPoints;

};

#endif // AMERICANROCKETMAN_H
