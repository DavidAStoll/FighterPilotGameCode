/*
 ============================================================================
 Name		: AmericanCarrierLexington.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanCarrierLexington declaration
 ============================================================================
 */

#ifndef AMERICANCARRIERLEXINGTON_H
#define AMERICANCARRIERLEXINGTON_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShipCarrier.h"

class CLandingSurface;
class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CAmericanCarrierLexington
 * 
 */
class CAmericanCarrierLexington : public CShipCarrier
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanCarrierLexington();
	static CAmericanCarrierLexington* LoadAmericanCarrierLexington();
	static CAmericanCarrierLexington* New(TInt aXLocation, TBool aReflected);

	static void InitilizeAmericanCarrierLexington();
	static void RemoveAmericanCarrierLexington();
	virtual void Update();
	virtual CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	CAmericanCarrierLexington(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadAmericanCarrierLexingtonTextures();
	static void UnloadAmericanCarrierLexingtonTextures();
	static void CreateAmericanCarrierLexingtonCollisionPointsL();
	static void RemoveAmericanCarrierLexingtonCollisionPoints();

	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CGeneralSmokeCreator* iSmokeCreatorTop;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // AMERICANCARRIERLEXINGTON_H
