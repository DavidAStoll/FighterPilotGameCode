/*
 ============================================================================
 Name		: Explosion500Kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion500Kg declaration
 ============================================================================
 */

#ifndef EXPLOSION500KG_H
#define EXPLOSION500KG_H

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

// CLASS DECLARATION

/**
 *  CExplosion500Kg
 * 
 */
class CExplosion500Kg: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion500Kg();
	static CExplosion500Kg* New(const TPoint& aLocation);
	static CExplosion500Kg* New(const TPointIntFloat& aLocation);
	static CExplosion500Kg* New(const TPoint& aLocation, TInt aDamage, TInt aArmorPenetration);
	static CExplosion500Kg* New(const TPointIntFloat& aLocation, TInt aDamage, TInt aArmorPenetration);
	static void InitilizeExplosion500Kg();
	static void RemoveExplosion500Kg();

protected:

	CExplosion500Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion500Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion500Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration);
	CExplosion500Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration);
	static void LoadExplosion500KgTexturesL();
	static void UnloadExplosion500KgTextures();
	void ConstructL();

};

#endif // EXPLOSION500KG_H
