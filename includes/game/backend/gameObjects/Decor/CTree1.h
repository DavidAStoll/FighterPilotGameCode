/*
 ============================================================================
 Name		: Tree1.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CTree1 declaration
 ============================================================================
 */

#ifndef TREE1_H
#define TREE1_H

#include "includes/game/backend/basicClasses/CGameObject.h"
// CLASS DECLARATION

/**
 *  CTree1
 * 
 */

enum TBigThinTree 
{
  EBigThinTreeNormal, EBigThinTreeReflected
};

class CTree1 : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTree1();

	/**
	 * Two-phased constructor.
	 */
	static CTree1* New(TPoint aLocation,TBigThinTree aOrientation);
	static CTree1* New(TInt aXLocation,TBigThinTree aType);

	virtual void PossibleCollision(CInterval*& aInterval);
	static void InitilizeTree1();      
	static void RemoveTree1();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CTree1(TPoint aLocation,SGameObjectAttributes& aAttributes);

	virtual void Die(TDamageType aType); // does nothing since land can't die

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(TBigThinTree aOrientation);

	static void LoadTree1Textures();
	static void UnloadTree1Textures();

	};

#endif // TREE1_H
