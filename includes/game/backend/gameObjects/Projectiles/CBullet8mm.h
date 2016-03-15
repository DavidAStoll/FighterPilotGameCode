/*
 * CBullet8mm.h
 *
 *  Created on: Jun 18, 2013
 *      Author: dstoll
 */

#ifndef CBULLET8MM_H_
#define CBULLET8MM_H_

#include "includes/game/backend/basicClasses/CProjectile.h"

// CLASS DECLARATION

/**
 *  CBullet8mm
 *
 */
class CBullet8mm: public CProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBullet8mm();

	static CBullet8mm* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected);
	static CBullet8mm* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed);

	static void InitilizeBullet8mm();
	static void RemoveBullet8mm();
	void AssignDamageType(TDamageType aDamageType);
	virtual void PossibleCollision(CInterval*& aInterval);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBullet8mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngle);
	CBullet8mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngle, TIntFloat aSpeed);

	static void LoadBullet8mmTextures();
	static void UnloadBullet8mmTextures();
	static void CreateBullet8mmCollisionPointsL();
	static void RemoveBullet8mmCollisionPoints();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	TDamageType iBulletDamageType;
	static SBoundingCollisionPoint* CollisionPoints;
};

#endif /* CBULLET8MM_H_ */
