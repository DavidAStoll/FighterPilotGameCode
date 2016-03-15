/*
 ============================================================================
 Name		: AmericanShermanTank.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanShermanTank declaration
 ============================================================================
 */

#ifndef AMERICANSHERMANTANK_H
#define AMERICANSHERMANTANK_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"

class CTankGun80mm;

// CLASS DECLARATION

/**
 *  CAmericanShermanTank
 * 
 */
class CAmericanShermanTank : public CGroundUnit
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanShermanTank();

	/**
	 * Two-phased constructor.
	 */
	static CAmericanShermanTank* New(TInt aXLocation, TIntFloat aSpeed, TBool aRefleced);
	
	virtual void Shoot();//stub function,causes the the unit to fire
	static void InitilizeAmericanShermanTank();
	static void RemoveAmericanShermanTank();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);


private:
	/**
	 * Constructor for performing 1st stage construction
	 */
	CAmericanShermanTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateSpecific(); //should contain all the code to update all weapons that are attached to this unit
	static void LoadAmericanShermanTankTextures();
	static void UnloadAmericanShermanTankTextures();
	static void CreateAmericanShermanTankCollisionPointsL();
	static void RemoveAmericanShermanTankCollisionPoints();
	
	CTankGun80mm* iTankGun;
	CGeneralSmokeCreator* iDustCreator;
	static SBoundingCollisionPoint* CollisionPoints;
	};

#endif // AMERICANSHERMANTANK_H
