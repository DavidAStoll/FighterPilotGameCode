/*
 ============================================================================
 Name		: Explosion500KgInWater.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion500KgInWater declaration
 ============================================================================
 */

#ifndef EXPLOSION500KGINWATER_H
#define EXPLOSION500KGINWATER_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CExplosion500KgInWater
 * 
 */
class CExplosion500KgInWater: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion500KgInWater();
	static CExplosion500KgInWater* New(const TPoint& aLocation);
	static CExplosion500KgInWater* New(const TPointIntFloat& aLocation);
	static void InitilizeExplosion500KgInWater();
	static void RemoveExplosion500KgInWater();

protected:

	CExplosion500KgInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion500KgInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion500KgInWaterTexturesL();
	static void UnloadExplosion500KgInWaterTextures();
	void ConstructL();

};

#endif // EXPLOSION500KGINWATER_H
