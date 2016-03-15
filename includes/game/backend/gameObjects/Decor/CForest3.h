/*
 ============================================================================
 Name		: Forest3.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest3 declaration
 ============================================================================
 */

#ifndef FOREST3_H
#define FOREST3_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

class CSmallTree;
class CTree1;
class CBigTree;

/**
 *  CForest3
 * 
 */
class CForest3: public CGameObject
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CForest3();

	/**
	 * Two-phased constructor.
	 */
	static CForest3* LoadForest3(); //doesn't create any trees, since they will be create it by the load function
	static CForest3* New(TInt aXLocation);

	virtual void Draw();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CForest3(TPoint aLocation, SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();

	CSmallTree* iSmallTree1;
	CSmallTree* iSmallTree2;
	CBigTree* iBigTree1;
	CBigTree* iBigTree2;
};

#endif // FOREST3_H
