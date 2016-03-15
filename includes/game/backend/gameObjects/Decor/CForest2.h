/*
 ============================================================================
 Name		: Forest2.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest2 declaration
 ============================================================================
 */

#ifndef FOREST2_H
#define FOREST2_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

/**
 *  CForest2
 * 
 */

class CSmallTree;
class CTree1;
class CBigTree;

class CForest2 : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CForest2();

	/**
	 * Two-phased constructor.
	 */
	static CForest2* LoadForest2(); //doesn't create any trees, since they will be create it by the load function
	static CForest2* New(TInt aXLocation);

	virtual void Draw();
  virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
  virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CForest2(TPoint aLocation,SGameObjectAttributes& aAttributes);
  void ConstructWithoutCreatingOtherObjectsL();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	CSmallTree* iSmallTree1;
	CBigTree* iBigTree1;
	CBigTree* iBigTree2;

	};

#endif // FOREST2_H
