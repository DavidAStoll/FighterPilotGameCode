/*
 ============================================================================
 Name		: Forest4.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest4 declaration
 ============================================================================
 */

#ifndef FOREST4_H
#define FOREST4_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

class CSmallTree;
class CTree1;
class CBigTree;

/**
 *  CForest4
 * 
 */
class CForest4 : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CForest4();

	/**
	 * Two-phased constructor.
	 */
	static CForest4* LoadForest4(); //doesn't create any trees, since they will be create it by the load function
	static CForest4* New(TInt aXLocation);
	
	virtual void Draw();
  virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
  virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CForest4(TPoint aLocation,SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
  void ConstructWithoutCreatingOtherObjectsL();
        
	CSmallTree* iSmallTree1;
	CSmallTree* iSmallTree2;
	CSmallTree* iSmallTree3;
	CSmallTree* iSmallTree4;
	CBigTree* iBigTree1;
	CBigTree* iBigTree2;
	CTree1* iBigThinTree1;

	};

#endif // FOREST4_H
