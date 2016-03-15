/*
 ============================================================================
 Name		: USSVictory.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CUSSVictory declaration
 ============================================================================
 */

#ifndef USSVICTORY_H
#define USSVICTORY_H

#include "includes/game/backend/basicClasses/CShipCarrier.h"

class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CUSSVictory
 * 
 */
class CUSSVictory: public CShipCarrier
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CUSSVictory();

	/**
	 * Two-phased constructor.
	 */
	static CUSSVictory* LoadUSSVictory();
	static CUSSVictory* New(TInt aXLocation, TBool aReflected);
	static void InitilizeUSSVictory();
	static void RemoveUSSVictory();
	virtual void Update();
	virtual CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CUSSVictory(TInt aXLocation, SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadUSSVictoryTextures();
	static void UnloadUSSVictoryTextures();
	static void CreateUSSVictoryCollisionPointsL();
	static void RemoveUSSVictoryCollisionPoints();

	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CGeneralSmokeCreator* iSmokeCreatorTop;

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
};

#endif // USSVICTORY_H
