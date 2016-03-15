/*
 ============================================================================
 Name		: AmericanAAShermanTank.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanAAShermanTank declaration
 ============================================================================
 */

#ifndef AMERICANAASHERMANTANK_H
#define AMERICANAASHERMANTANK_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"

class CTankAAGun;

/**
 *  CAmericanAAShermanTank
 * 
 */
class CAmericanAAShermanTank :  public CGroundUnit
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanAAShermanTank();

	/**
	 * Two-phased constructor.
	 */
	static CAmericanAAShermanTank* New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected);
	
	virtual void Update();// in overrides GameObjects method, in general should be sufficient for most game objects
	virtual void Draw();
	static void InitilizeAmericanAAShermanTank();
	static void RemoveAmericanAAShermanTank();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:
	/**
	 * Constructor for performing 1st stage construction
	 */
	CAmericanAAShermanTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	static void LoadAmericanAAShermanTankTextures();
	static void UnloadAmericanAAShermanTankTextures();
	static void CreateAmericanAAShermanTankCollisionPointsL();
	static void RemoveAmericanAAShermanTankCollisionPoints();
	
	CTankAAGun* iAAGun;
	CGeneralSmokeCreator* iDustCreator;
	static SBoundingCollisionPoint* CollisionPoints;

	};

#endif // AMERICANAASHERMANTANK_H
