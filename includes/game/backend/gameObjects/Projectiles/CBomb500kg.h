/*
 ============================================================================
 Name		: Bomb500kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBomb500kg declaration
 ============================================================================
 */

#ifndef BOMB500KG_H
#define BOMB500KG_H

#include "includes/game/backend/basicClasses/CProjectileBomb.h"

// CLASS DECLARATION

/**
 *  CBomb500kg
 * 
 */
class CBomb500kg : public CProjectileBomb
	{
public:
        // Constructors and destructor

        /**
         * Destructor.
         */
        ~CBomb500kg();

        /**
         * Two-phased constructor.
         */
        static CBomb500kg* New(TPointIntFloat aLocation,TIntFloat aSpeed, TInt aAngel,TBool aReflected);
        
        static void InitilizeBomb500kg();      
        static void RemoveBomb500kg();
        virtual void PossibleCollision(CInterval*& aInterval);
        
        static GLuint Texture; // texture might be used by some weapons

protected:

        /**
         * Constructor for performing 1st stage construction
         */
        CBomb500kg(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed,TInt aAngel);
        
        static void LoadBomb500kgTextures();
        static void UnloadBomb500kgTextures();
        static void CreateBomb500kgCollisionPointsL();
        static void RemoveBomb500kgCollisionPoints();
        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL();
        
        static SBoundingCollisionPoint* CollisionPoints;
        };

#endif // BOMB500KG_H
