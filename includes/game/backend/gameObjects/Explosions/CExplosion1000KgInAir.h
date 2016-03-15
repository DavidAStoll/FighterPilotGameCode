/*
 ============================================================================
 Name		: Explosion1000KgInAir.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion1000KgInAir declaration
 ============================================================================
 */

#ifndef EXPLOSION1000KGINAIR_H
#define EXPLOSION1000KGINAIR_H

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

// CLASS DECLARATION


/**
 *  CExplosion1000KgInAir
 * 
 */
class CExplosion1000KgInAir: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion1000KgInAir();
	static CExplosion1000KgInAir* New(const TPoint& aLocation);
	static CExplosion1000KgInAir* New(const TPointIntFloat& aLocation);
	static void InitilizeExplosion1000KgInAir();
	static void RemoveExplosion1000KgInAir();

protected:


	/**
	 * Constructor for performing 1st stage construction
	 */
	CExplosion1000KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion1000KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion1000KgInAirTexturesL();
	static void UnloadExplosion1000KgInAirTextures();
	void ConstructL();
};

#endif // EXPLOSION1000KGINAIR_H
