/*
 ============================================================================
 Name		: PropelledGrenade.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CPropelledGrenade declaration
 ============================================================================
 */

#ifndef PROPELLEDGRENADE_H
#define PROPELLEDGRENADE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGravitationalProjectile.h"

// CLASS DECLARATION

/**
 *  CPropelledGrenade
 * 
 */
class CPropelledGrenade: public CGravitationalProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CPropelledGrenade();

	static CPropelledGrenade* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aExtraSpeed);

	void DestructAtCollisionPoint(TPointIntFloat* lCollisionPoint); //called when range is less than 0,might cause an explosion or just remove the object
	static void InitilizePropelledGrenade();
	static void RemovePropelledGrenade();
	virtual void PossibleCollision(CInterval*& aInterval);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CPropelledGrenade(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aExtraSpeed);

	static void LoadPropelledGrenadeTextures();
	static void UnloadPropelledGrenadeTextures();
	static void CreatePropelledGrenadeCollisionPointsL();
	static void RemovePropelledGrenadeCollisionPoints();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	static SBoundingCollisionPoint* CollisionPoints;

};

#endif // PROPELLEDGRENADE_H
