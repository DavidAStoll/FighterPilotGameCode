/*
 ============================================================================
 Name		: JapaneseRocketman.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseRocketman declaration
 ============================================================================
 */

#ifndef JAPANESEROCKETMAN_H
#define JAPANESEROCKETMAN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"
class CGrenadeLauncher;

// CLASS DECLARATION

/**
 *  CJapaneseRocketman
 * 
 */
class CJapaneseRocketman: public CGroundUnit
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseRocketman();

	static CJapaneseRocketman* New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected);

	virtual void Shoot();//stub function,causes the the unit to fire
	static void InitilizeJapaneseRocketman();
	static void RemoveJapaneseRocketman();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	CJapaneseRocketman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateWeapon(); //should contain all the code to update all weapons that are attached to this unit
	static void LoadJapaneseRocketmanTextures();
	static void UnloadJapaneseRocketmanTextures();
	static void CreateJapaneseRocketmanCollisionPointsL();
	static void RemoveJapaneseRocketmanCollisionPoints();

	CGrenadeLauncher* iGrendaeLauncher;
	static SBoundingCollisionPoint* CollisionPoints;

};

#endif // JAPANESEROCKETMAN_H
