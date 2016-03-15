/*
 ============================================================================
 Name		: Bomb100kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBomb100kg declaration
 ============================================================================
 */

#ifndef BOMB100KG_H
#define BOMB100KG_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CProjectileBomb.h"

/**
 *  CBomb100kg
 * 
 */
class CBomb100kg : public CProjectileBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBomb100kg();

	/**
	 * Two-phased constructor.
	 */
	static CBomb100kg* New(TPointIntFloat aLocation,TIntFloat aSpeed, TInt aAngel,TBool aReflected);
	static void InitilizeBomb100kg();
	static void RemoveBomb100kg();
	virtual void PossibleCollision(CInterval*& aInterval);
        
private:

        /**
         * Constructor for performing 1st stage construction
         */
        CBomb100kg(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed,TInt aAngel);
        
        static void LoadBomb100kgTextures();
        static void UnloadBomb100kgTextures();
        static void CreateBomb100kgCollisionPointsL();
        static void RemoveBomb100kgCollisionPoints();
        

        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL();
        
        static SBoundingCollisionPoint* CollisionPoints;
	};

#endif // BOMB100KG_H
