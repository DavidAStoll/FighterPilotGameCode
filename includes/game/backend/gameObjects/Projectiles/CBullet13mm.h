/*
 ============================================================================
 Name		: Bullet13mm.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBullet13mm declaration
 ============================================================================
 */

#ifndef BULLET13mm_H
#define BULLET13mm_H

#include "includes/game/backend/basicClasses/CProjectile.h"

// CLASS DECLARATION

/**
 *  CBullet13mm
 * 
 */
class CBullet13mm: public CProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBullet13mm();

	static CBullet13mm* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TBool aHarmGroundUnits);
	static CBullet13mm* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed, TBool aHarmGroundUnits);

	static void InitilizeBullet13mm();
	static void RemoveBullet13mm();
	void AssignDamageType(TDamageType aDamageType);
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBullet13mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngle, TBool aHarmGroundUnitsl);
	CBullet13mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngle, TIntFloat aSpeed, TBool aHarmGroundUnits);

	static void LoadBullet13mmTextures();
	static void UnloadBullet13mmTextures();
	static void CreateBullet13mmCollisionPointsL();
	static void RemoveBullet13mmCollisionPoints();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	TBool iHarmGroundUnits;
	TDamageType iBulletDamageType;
	static SBoundingCollisionPoint* CollisionPoints;
};

#endif // BULLET13mm_H
