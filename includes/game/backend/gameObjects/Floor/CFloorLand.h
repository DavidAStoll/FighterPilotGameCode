/*
 ============================================================================
 Name		: FloorLand.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFloorLand declaration
 ============================================================================
 */

#ifndef FLOORLAND_H
#define FLOORLAND_H

#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

/**
 *  CFloorLand
 * 
 */
class CFloorLand : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CFloorLand();

	/**
	 * Two-phased constructor.
	 */
	static CFloorLand* New(TPoint aLocation,TInt aWidth);
	static CFloorLand* New(TInt aXLocation,TInt aWidth);
	
	virtual void PossibleCollision(CInterval*& aInterval);
	static void InitilizeFloorLand();      
	static void RemoveFloorLand();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CFloorLand(TPoint aLocation,SGameObjectAttributes& aAttributes);
  virtual void Die(TDamageType aType); // does nothing since land can't die

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(TInt aWidth);
	
	static void LoadFloorLandTextures();
	static void UnloadFloorLandTextures();

	};

#endif // FLOORLAND_H
