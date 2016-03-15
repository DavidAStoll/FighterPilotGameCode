/*
 * CBulletCannon20mm.h
 *
 *  Created on: Jun 21, 2013
 *      Author: dstoll
 */

#ifndef CBULLETCANNON20MM_H_
#define CBULLETCANNON20MM_H_

#include "includes/game/backend/basicClasses/CProjectile.h"

class CGeneralSmokeCreator;

class CBulletCannon20mm:  public CProjectile
{
	// Constructors and destructor

public:
	/**
	 * Destructor.
	 */
	~CBulletCannon20mm();

	static CBulletCannon20mm* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TBool aHarmGroundUnits);
	static CBulletCannon20mm* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed, TBool aHarmGroundUnits);

	void AssignDamageType(TDamageType aDamageType);
	virtual void Update(); //runs the Game Object's logic code to update its member data
	static void InitilizeBulletCannon20mm();
	static void RemoveBulletCannon20mm();
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBulletCannon20mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngle, TBool aHarmGroundUnitsl);
	CBulletCannon20mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngle, TIntFloat aSpeed, TBool aHarmGroundUnits);

	static void LoadBulletCannon20mmTextures();
	static void UnloadBulletCannon20mmTextures();
	static void CreateBulletCannon20mmCollisionPointsL();
	static void RemoveBulletCannon20mmCollisionPoints();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	TBool iHarmGroundUnits;
	TDamageType iBulletDamageType;
	CGeneralSmokeCreator* iSmokeTrial;
	static SBoundingCollisionPoint* CollisionPoints;
};

#endif /* CBULLETCANNON20MM_H_ */
