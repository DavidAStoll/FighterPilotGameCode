/*
 ============================================================================
 Name		: BigTree.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBigTree declaration
 ============================================================================
 */

#ifndef BIGTREE_H
#define BIGTREE_H

// INCLUDES

#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

/**
 *  CBigTree
 * 
 */

enum TBigTree 
{
  EBigTreeNormal, EBigTreeReflected
};

class CBigTree : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBigTree();

	/**
	 * Two-phased constructor.
	 */
	static CBigTree* New(TPoint aLocation,TBigTree aOrientation);
	static CBigTree* New(TInt aXLocation,TBigTree aType);

	virtual void PossibleCollision(CInterval*& aInterval);
	static void InitilizeBigTree();      
	static void RemoveBigTree();

private:

        /**
         * Constructor for performing 1st stage construction
         */
	CBigTree(TPoint aLocation,SGameObjectAttributes& aAttributes);

        virtual void Die(TDamageType aType); // does nothing since land can't die

        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL(TBigTree aOrientation);
        
        static void LoadBigTreeTextures();
        static void UnloadBigTreeTextures();
	};

#endif // BIGTREE_H
