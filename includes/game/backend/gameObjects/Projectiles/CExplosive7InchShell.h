/*
 ============================================================================
 Name		: Explosive7InchShell.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive7InchShell declaration
 ============================================================================
 */

#ifndef EXPLOSIVE7INCHSHELL_H
#define EXPLOSIVE7INCHSHELL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGravitationalProjectile.h"

// CLASS DECLARATION

/**
 *  CExplosive7InchShell
 * 
 */
class CExplosive7InchShell : public CGravitationalProjectile
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosive7InchShell();

	/**
	 * Two-phased constructor.
	 */
	static CExplosive7InchShell* New(TPointIntFloat aLocation,TConflictSide aConflictSide, TInt aAngel,TBool aReflected,TIntFloat aSpeed);
	void DestructAtCollisionPoint(TPointIntFloat* lCollisionPoint); //called when range is less than 0,might cause an explosion or just remove the object
	static void InitilizeExplosive7InchShell();
	static void RemoveExplosive7InchShell();
	virtual void PossibleCollision(CInterval*& aInterval);
private:

	CExplosive7InchShell(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed);
	void ConstructL();
	static void LoadExplosive7InchShellTextures();
	static void UnloadExplosive7InchShellTextures();
	static void CreateExplosive7InchShellCollisionPointsL();
	static void RemoveExplosive7InchShellCollisionPoints();
	
	static SBoundingCollisionPoint* CollisionPoints;

	};

#endif // EXPLOSIVE7INCHSHELL_H
