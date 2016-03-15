/*
 ============================================================================
 Name		: Explosion100KgInWater.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion100KgInWater declaration
 ============================================================================
 */

#ifndef EXPLOSION100KGINWATER_H
#define EXPLOSION100KGINWATER_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBasicExplosion.h"

/**
 *  CExplosion100KgInWater
 * 
 */
class CExplosion100KgInWater : public CBasicExplosion
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosion100KgInWater();

        /**
         * Two-phased constructor.
         */
        static CExplosion100KgInWater* New(const TPoint& aLocation);
        static CExplosion100KgInWater* New(const TPointIntFloat& aLocation);
        static void InitilizeExplosion100KgInWater();
        static void RemoveExplosion100KgInWater();
        
protected:

        CExplosion100KgInWater(const TPoint& aLocation,SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
        CExplosion100KgInWater(const TPointIntFloat& aLocation,SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes);
        static void LoadExplosion100KgInWaterTexturesL();
        static void UnloadExplosion100KgInWaterTextures(); 
        void ConstructL();
	};

#endif // EXPLOSION100KGINWATER_H
