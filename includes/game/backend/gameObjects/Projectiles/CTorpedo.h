/*
 ============================================================================
 Name		: Torpedo.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CTorpedo declaration
 ============================================================================
 */

#ifndef TORPEDO_H
#define TORPEDO_H


// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CProjectileBomb.h"

/**
 *  CTorpedo
 * 
 */
class CTorpedo : public CProjectileBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTorpedo();

	/**
	 * Two-phased constructor.
	 */
	static CTorpedo* New(TPointIntFloat aLocation,TIntFloat aSpeed, TInt aAngel,TBool aReflected);
	
        static void InitilizeTorpedo();      
        static void RemoveTorpedo();
      	virtual void Update();
      	virtual void PossibleCollision(CInterval*& aInterval);
        
        TIntFloat iHeightTravelled;

private:

        /**
         * Constructor for performing 1st stage construction
         */
        CTorpedo(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed,TInt aAngel);
        
        static void LoadTorpedoTextures();
        static void UnloadTorpedoTextures();
        static void CreateTorpedoCollisionPointsL();
        static void RemoveTorpedoCollisionPoints();
        
        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL();
        
        static SBoundingCollisionPoint* CollisionPoints;
	};

#endif // TORPEDO_H
