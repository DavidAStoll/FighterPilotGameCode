/*
 ============================================================================
 Name		: Explosion100KgInAir.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosion100KgInAir declaration
 ============================================================================
 */

#ifndef EXPLOSION100KGINAIR_H
#define EXPLOSION100KGINAIR_H

// CLASS DECLARATION
#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CExplosion100KgInAir
 * 
 */
class CExplosion100KgInAir: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion100KgInAir();

	/**
	 * Two-phased constructor.
	 */
	static CExplosion100KgInAir* New(const TPoint& aLocation);
	static CExplosion100KgInAir* New(const TPointIntFloat& aLocation);
	static void InitilizeExplosion100KgInAir();
	static void RemoveExplosion100KgInAir();

private:

	CExplosion100KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion100KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion100KgInAirTexturesL();
	static void UnloadExplosion100KgInAirTextures();
	void ConstructL();
};

#endif // EXPLOSION100KGINAIR_H
