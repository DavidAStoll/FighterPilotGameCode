/*
 ============================================================================
 Name		: JapaneseRifleman.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseRifleman declaration
 ============================================================================
 */

#ifndef JAPANESERIFLEMAN_H
#define JAPANESERIFLEMAN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"
#include "includes/game/backend/gameObjects/Weapons/CRifle.h"

// CLASS DECLARATION

/**
 *  CJapaneseRifleman
 * 
 */
class CJapaneseRifleman: public CGroundUnit
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseRifleman();
	static CJapaneseRifleman* New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected);

	virtual void Shoot();//stub function,causes the the unit to fire
	static void InitilizeJapaneseRifleman();
	static void RemoveJapaneseRifleman();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CJapaneseRifleman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateWeapon(); //should contain all the code to update all weapons that are attached to this unit
	static void LoadJapaneseRiflemanTextures();
	static void UnloadJapaneseRiflemanTextures();
	static void CreateJapaneseRiflemanCollisionPointsL();
	static void RemoveJapaneseRiflemanCollisionPoints();

	CRifle* iRifle;
	static SBoundingCollisionPoint* CollisionPoints;
};

#endif // JAPANESERIFLEMAN_H
