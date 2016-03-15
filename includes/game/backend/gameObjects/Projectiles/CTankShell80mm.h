/*
 ============================================================================
 Name		: TankShell80mm.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTankShell80mm declaration
 ============================================================================
 */

#ifndef TANKSHELL80MM_H
#define TANKSHELL80MM_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGravitationalProjectile.h"

// CLASS DECLARATION

/**
 *  CTankShell80mm
 * 
 */
class CTankShell80mm : public CGravitationalProjectile
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTankShell80mm();

	static CTankShell80mm* New(TPointIntFloat aLocation,TConflictSide aConflictSide, TInt aAngel,TBool aReflected,TIntFloat aExtraSpeed);

	void DestructAtCollisionPoint(TPointIntFloat* lCollisionPoint); //called when range is less than 0,might cause an explosion or just remove the object
	static void InitilizeTankShell80mm();
	static void RemoveTankShell80mm();
	virtual void PossibleCollision(CInterval*& aInterval);
	
protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CTankShell80mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aExtraSpeed);
	void ConstructL();
	static void LoadTankShell80mmTextures();
	static void UnloadTankShell80mmTextures();
	static void CreateTankShell80mmCollisionPointsL();
	static void RemoveTankShell80mmCollisionPoints();
	
	static SBoundingCollisionPoint* CollisionPoints;

	};

#endif // TANKSHELL80MM_H
