/*
 ============================================================================
 Name		: Bomb1000kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBomb1000kg declaration
 ============================================================================
 */

#ifndef BOMB1000KG_H
#define BOMB1000KG_H


#include "includes/game/backend/basicClasses/CProjectileBomb.h"

// CLASS DECLARATION

/**
 *  CBomb1000kg
 * 
 */
class CBomb1000kg : public CProjectileBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBomb1000kg();
	static CBomb1000kg* New(TPointIntFloat aLocation,TIntFloat aSpeed, TInt aAngel,TBool aReflected);
	static void InitilizeBomb1000kg();      
	static void RemoveBomb1000kg();
	virtual void PossibleCollision(CInterval*& aInterval);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBomb1000kg(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed,TInt aAngel);
	
	static void LoadBomb1000kgTextures();
	static void UnloadBomb1000kgTextures();
	static void CreateBomb1000kgCollisionPointsL();
	static void RemoveBomb1000kgCollisionPoints();
	
	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	static SBoundingCollisionPoint* CollisionPoints;
	};

#endif // BOMB1000KG_H
