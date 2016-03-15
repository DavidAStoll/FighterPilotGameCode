/*
 ============================================================================
 Name		: BulletRifle.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBulletRifle declaration
 ============================================================================
 */

#ifndef CBulletRifle_H
#define CBulletRifle_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGravitationalProjectile.h"

// CLASS DECLARATION

/**
 *  CBulletRifle
 * 
 */
class CBulletRifle: public CGravitationalProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBulletRifle();

	/**
	 * Two-phased constructor.
	 */
	static CBulletRifle* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected);
	static CBulletRifle* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed);

	static void InitilizeBulletRifle();
	static void RemoveBulletRifle();
	virtual void PossibleCollision(CInterval*& aInterval);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBulletRifle(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel);
	CBulletRifle(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed);

	static void LoadBulletRifleTextures();
	static void UnloadBulletRifleTextures();
	static void CreateBulletRifleCollisionPointsL();
	static void RemoveBulletRifleCollisionPoints();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	static SBoundingCollisionPoint* CollisionPoints;
};

#endif // BulletRifle_H
