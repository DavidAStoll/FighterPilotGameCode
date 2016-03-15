/*
 ============================================================================
 Name		: Explosion14InchShell.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosion14InchShell declaration
 ============================================================================
 */

#ifndef EXPLOSION14INCHSHELL_H
#define EXPLOSION14INCHSHELL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CBasicExplosion.h"

// CLASS DECLARATION

/**
 *  CExplosion14InchShell
 * 
 */
class CExplosion14InchShell : public CBasicExplosion
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion14InchShell();

	static CExplosion14InchShell* New(const TPoint& aLocation);
	static CExplosion14InchShell* New(const TPointIntFloat& aLocation);
	static void InitilizeExplosion14InchShell();
	static void RemoveExplosion14InchShell();

protected:

	CExplosion14InchShell(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion14InchShell(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion14InchShellTexturesL();
	static void UnloadExplosion14InchShellTextures();
	void ConstructL();

	};

#endif // EXPLOSION14INCHSHELL_H
