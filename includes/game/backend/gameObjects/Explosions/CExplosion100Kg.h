/*
 ============================================================================
 Name		: Explosion100Kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion100Kg declaration
 ============================================================================
 */

#ifndef EXPLOSION100KG_H
#define EXPLOSION100KG_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CExplosion100Kg
 * 
 */
class CExplosion100Kg: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion100Kg();

	/**
	 * Two-phased constructor.
	 */
	static CExplosion100Kg* New(const TPoint& aLocation);
	static CExplosion100Kg* New(const TPointIntFloat& aLocation);
	static void InitilizeExplosion100Kg();
	static void RemoveExplosion100Kg();

private:

	CExplosion100Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion100Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion100KgTexturesL();
	static void UnloadExplosion100KgTextures();
	void ConstructL();

};

#endif // EXPLOSION100KG_H
