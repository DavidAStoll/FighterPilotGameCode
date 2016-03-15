/*
 ============================================================================
 Name		: Explosion1000KgInWater.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion1000KgInWater declaration
 ============================================================================
 */

#ifndef EXPLOSION1000KGINWATER_H
#define EXPLOSION1000KGINWATER_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CExplosion1000KgInWater
 * 
 */
class CExplosion1000KgInWater: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion1000KgInWater();

	/**
	 * Two-phased constructor.
	 */
	static CExplosion1000KgInWater* New(const TPoint& aLocation);
	static CExplosion1000KgInWater* New(const TPointIntFloat& aLocation);

	static void InitilizeExplosion1000KgInWater();
	static void RemoveExplosion1000KgInWater();
protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CExplosion1000KgInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion1000KgInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion1000KgInWaterTexturesL();
	static void UnloadExplosion1000KgInWaterTextures();
	void ConstructL();

};

#endif // EXPLOSION1000KGINWATER_H
