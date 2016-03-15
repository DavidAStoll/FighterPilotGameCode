/*
 ============================================================================
 Name		: WaterSplash.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWaterSplash declaration
 ============================================================================
 */

#ifndef WATERSPLASH_H
#define WATERSPLASH_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CWaterSplash
 * 
 */
class CWaterSplash: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWaterSplash();

	/**
	 * Two-phased constructor.
	 */
	static CWaterSplash* New(const TPoint& aLocation);
	static CWaterSplash* New(const TPointIntFloat& aLocation);
	static void InitilizeWaterSplash();
	static void RemoveWaterSplash();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWaterSplash(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CWaterSplash(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadWaterSplashTexturesL();
	static void UnloadWaterSplashTextures();
	void ConstructL();
};

#endif // WATERSPLASH_H
