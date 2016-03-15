/*
 ============================================================================
 Name		: Explosive14InchShell.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive14InchShell declaration
 ============================================================================
 */

#ifndef EXPLOSIVE14INCHSHELL_H
#define EXPLOSIVE14INCHSHELL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGravitationalProjectile.h"

// CLASS DECLARATION

/**
 *  CExplosive14InchShell
 * 
 */
class CExplosive14InchShell : public CGravitationalProjectile
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosive14InchShell();
	static CExplosive14InchShell* New(TPointIntFloat aLocation,TConflictSide aConflictSide, TInt aAngel,TBool aReflected,TIntFloat aSpeed);

	static void InitilizeExplosive14InchShell();
	static void RemoveExplosive14InchShell();
	virtual void PossibleCollision(CInterval*& aInterval);

private:
	CExplosive14InchShell(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed);
	void ConstructL();
	static void LoadExplosive14InchShellTextures();
	static void UnloadExplosive14InchShellTextures();
	static void CreateExplosive14InchShellCollisionPointsL();
	static void RemoveExplosive14InchShellCollisionPoints();
	
	static SBoundingCollisionPoint* CollisionPoints;

	};

#endif // EXPLOSIVE14INCHSHELL_H
