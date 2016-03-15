/*
 ============================================================================
 Name		: SmallTree.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CSmallTree declaration
 ============================================================================
 */

#ifndef SMALLTREE_H
#define SMALLTREE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

enum TSmallTree 
{
  ESmallTreeNormal, ESmallTreeReflected
};


/**
 *  CSmallTree
 * 
 */
class CSmallTree : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSmallTree();

	/**
	 * Two-phased constructor.
	 */
	static CSmallTree* New(TPoint aLocation,TSmallTree aOrientation);
	static CSmallTree* New(TInt aXLocation,TSmallTree aType);
	
	virtual void PossibleCollision(CInterval*& aInterval);
	static void InitilizeSmallTree();      
	static void RemoveSmallTree();

private:
	CSmallTree(TPoint aLocation,SGameObjectAttributes& aAttributes);

	virtual void Die(TDamageType aType); // does nothing since tree can't die
	void ConstructL(TSmallTree aOrientation);

	static void LoadSmallTreeTextures();
	static void UnloadSmallTreeTextures();
        
	};

#endif // SMALLTREE_H
