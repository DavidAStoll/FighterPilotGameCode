/*
 ============================================================================
 Name		: UnderWaterTorpedo.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CUnderWaterTorpedo declaration
 ============================================================================
 */

#ifndef UNDERWATERTORPEDO_H
#define UNDERWATERTORPEDO_H

#include "includes/game/backend/basicClasses/CProjectile.h"

// CLASS DECLARATION

/**
 *  CUnderWaterTorpedo
 * 
 */
class CUnderWaterTorpedo: public CProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CUnderWaterTorpedo();

	/**
	 * Two-phased constructor.
	 */
	static CUnderWaterTorpedo* New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected);
	static void InitilizeUnderWaterTorpedo();
	static void RemoveUnderWaterTorpedo();
	virtual void Destruct(); //called when range is less than 0,might cause an explosion or just remove the object
	virtual void DestructWithoutExplosion(); //called when range is less than 0,might cause an explosion or just remove the object
	virtual void Update();
	virtual void PossibleCollision(CInterval*& aInterval);

protected:

	CUnderWaterTorpedo(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel);

	static void LoadUnderWaterTorpedoTextures();
	static void UnloadUnderWaterTorpedoTextures();
	static void CreateUnderWaterTorpedoCollisionPointsL();
	static void RemoveUnderWaterTorpedoCollisionPoints();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	static SBoundingCollisionPoint* CollisionPoints;

};

#endif // UNDERWATERTORPEDO_H
