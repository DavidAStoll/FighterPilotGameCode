/*
 ============================================================================
 Name		: JapaneseCarrierTaiho.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseCarrierTaiho declaration
 ============================================================================
 */

#ifndef JAPANESECARRIERTAIHO_H
#define JAPANESECARRIERTAIHO_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShipCarrier.h"
class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CJapaneseCarrierTaiho
 * 
 */
class CJapaneseCarrierTaiho : public CShipCarrier
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseCarrierTaiho();
	static CJapaneseCarrierTaiho* LoadJapaneseCarrierTaiho();
	static CJapaneseCarrierTaiho* New(TInt aXLocation, TBool aReflected);

	static void InitilizeJapaneseCarrierTaiho();
	static void RemoveJapaneseCarrierTaiho();
	virtual void Update();
	virtual CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

protected:

	CJapaneseCarrierTaiho(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseCarrierTaihoTextures();
	static void UnloadJapaneseCarrierTaihoTextures();
	static void CreateJapaneseCarrierTaihoCollisionPointsL();
	static void RemoveJapaneseCarrierTaihoCollisionPoints();

	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CGeneralSmokeCreator* iSmokeCreatorTop;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // JAPANESECARRIERTAIHO_H
