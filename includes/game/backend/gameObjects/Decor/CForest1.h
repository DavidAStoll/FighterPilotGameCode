/*
 ============================================================================
 Name		: Forest1.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest1 declaration
 ============================================================================
 */

#ifndef FOREST1_H
#define FOREST1_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

/**
 *  CForest1
 * 
 */
class CSmallTree;
class CTree1;
class CBigTree;

class CForest1 : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CForest1();

	/**
	 * Two-phased constructor.
	 */
	static CForest1* LoadForest1(); //doesn't create any trees, since they will be create it by the load function
	static CForest1* New(TInt aXLocation);

	virtual void Draw();
  virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
  virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CForest1(TPoint aLocation,SGameObjectAttributes& aAttributes);
	void ConstructWithoutCreatingOtherObjectsL();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
		
	CSmallTree* iSmallTree1;
	CSmallTree* iSmallTree2;
	CSmallTree* iSmallTree3;
	CSmallTree* iSmallTree4;
	CSmallTree* iSmallTree5;
	CBigTree* iBigTree1;
	CBigTree* iBigTree2;
	CBigTree* iBigTree3;
	CTree1* iTree11;
	CTree1* iTree12;
	CTree1* iTree13;

	};

#endif // FOREST1_H
