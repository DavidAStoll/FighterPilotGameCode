/*
 ============================================================================
 Name		: Explosion500KgInAir.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion500KgInAir declaration
 ============================================================================
 */

#ifndef EXPLOSION500KGINAIR_H
#define EXPLOSION500KGINAIR_H

// CLASS DECLARATION
#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CExplosion500KgInAir
 * 
 */
class CExplosion500KgInAir: public CBasicExplosion
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion500KgInAir();

	/**
	 * Two-phased constructor.
	 */

	//These explosions can have different kinds of damage types depending what object uses them
	static CExplosion500KgInAir* New(const TPoint& aLocation, TInt aDamage, TInt aArmorPenetration, TBool aHarmGroundUnits);
	static CExplosion500KgInAir* New(const TPointIntFloat& aLocation, TInt aDamage, TInt aArmorPenetration, TBool aHarmGroundUnits);
	static CExplosion500KgInAir* NewForRocket(const TPointIntFloat& aLocation, TInt aDamage, TInt aArmorPenetration);
	static CExplosion500KgInAir* New(const TPoint& aLocation, TBool aHarmGroundUnits);
	static CExplosion500KgInAir* New(const TPointIntFloat& aLocation, TBool aHarmGroundUnits);
	static void InitilizeExplosion500KgInAir();
	static void RemoveExplosion500KgInAir();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	//These contructors are used if the default damage and armor penetration is to be used
	CExplosion500KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	CExplosion500KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
	//these ones are if a custom ones needs to be specified
	CExplosion500KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration);
	CExplosion500KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration);

	//functions intilize global data shared by all dauntless objects, such Bounding Collision points and textures
	static void LoadExplosion500KgInAirTexturesL();
	static void UnloadExplosion500KgInAirTextures();
	void ConstructL();

};

#endif // EXPLOSION500KGINAIR_H
