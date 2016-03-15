/*
 ============================================================================
 Name		: Explosion1000Kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion1000Kg declaration
 ============================================================================
 */

#ifndef EXPLOSION1000KG_H
#define EXPLOSION1000KG_H

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

// CLASS DECLARATION

/**
 *  CExplosion1000Kg
 * 
 */
class CExplosion1000Kg: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion1000Kg();

	/**
	 * Two-phased constructor.
	 */
	static CExplosion1000Kg* New(const TPoint& aLocation, TUint aObjectsItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES);
	static CExplosion1000Kg* New(const TPointIntFloat& aLocation, TUint aObjectsItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES);
	static void InitilizeExplosion1000Kg();
	static void RemoveExplosion1000Kg();

protected:

	CExplosion1000Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion1000Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	static void LoadExplosion1000KgTexturesL();
	static void UnloadExplosion1000KgTextures();
	void ConstructL();
};

#endif // EXPLOSION1000KG_H
