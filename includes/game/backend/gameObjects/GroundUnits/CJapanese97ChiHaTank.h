/*
 ============================================================================
 Name		: Japanese97ChiHaTank.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapanese97ChiHaTank declaration
 ============================================================================
 */

#ifndef JAPANESE97CHIHATANK_H
#define JAPANESE97CHIHATANK_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"

class CTankGun80mm;

// CLASS DECLARATION
/**
 *  CJapanese97ChiHaTank
 * 
 */
class CJapanese97ChiHaTank : public CGroundUnit
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapanese97ChiHaTank();

	/**
	 * Two-phased constructor.
	 */
	static CJapanese97ChiHaTank* New(TInt aXLocation, TIntFloat aSpeed, TBool aRefleced);
	
	virtual void Shoot();//stub function,causes the the unit to fire
	static void InitilizeJapanese97ChiHaTank();
	static void RemoveJapanese97ChiHaTank();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CJapanese97ChiHaTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();
	
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateSpecific(); //should contain all the code to update all weapons that are attached to this unit
	static void LoadJapanese97ChiHaTankTextures();
	static void UnloadJapanese97ChiHaTankTextures();
	static void CreateJapanese97ChiHaTankCollisionPointsL();
	static void RemoveJapanese97ChiHaTankCollisionPoints();
	
	CTankGun80mm* iTankGun;
	CGeneralSmokeCreator* iDustCreator;
	static SBoundingCollisionPoint* CollisionPoints;

	};

#endif // JAPANESE97CHIHATANK_H
