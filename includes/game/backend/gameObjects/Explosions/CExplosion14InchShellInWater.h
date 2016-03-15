/*
 ============================================================================
 Name		: Explosion14InchShellInWater.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosion14InchShellInWater declaration
 ============================================================================
 */

#ifndef EXPLOSION14INCHSHELLINWATER_H
#define EXPLOSION14INCHSHELLINWATER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CBasicExplosion.h"

// CLASS DECLARATION

/**
 *  CExplosion14InchShellInWater
 * 
 */
class CExplosion14InchShellInWater : public CBasicExplosion
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion14InchShellInWater();
	static CExplosion14InchShellInWater* New(const TPoint& aLocation);
	static CExplosion14InchShellInWater* New(const TPointIntFloat& aLocation);
	static void InitilizeExplosion14InchShellInWater();
	static void RemoveExplosion14InchShellInWater();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CExplosion14InchShellInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion14InchShellInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion14InchShellInWaterTexturesL();
	static void UnloadExplosion14InchShellInWaterTextures();
	void ConstructL();

	};

#endif // EXPLOSION14INCHSHELLINWATER_H
