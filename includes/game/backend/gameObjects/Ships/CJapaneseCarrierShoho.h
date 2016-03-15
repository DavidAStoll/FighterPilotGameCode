/*
 ============================================================================
 Name		: JapaneseCarrierShoho.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseCarrierShoho declaration
 ============================================================================
 */

#ifndef JAPANESECARRIERSHOHO_H
#define JAPANESECARRIERSHOHO_H

#include "includes/game/backend/basicClasses/CShipCarrier.h"

class CGeneralSmokeCreator;
// CLASS DECLARATION

/**
 *  CJapaneseCarrierShoho
 * 
 */
class CJapaneseCarrierShoho : public CShipCarrier
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseCarrierShoho();
	static CJapaneseCarrierShoho* LoadJapaneseCarrierShoho();
	static CJapaneseCarrierShoho* New(TInt aXLocation, TBool aReflected);

	static void InitilizeJapaneseCarrierShoho();
	static void RemoveJapaneseCarrierShoho();
	virtual void Update();
	virtual CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

protected:

	CJapaneseCarrierShoho(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseCarrierShohoTextures();
	static void UnloadJapaneseCarrierShohoTextures();
	static void CreateJapaneseCarrierShohoCollisionPointsL();
	static void RemoveJapaneseCarrierShohoCollisionPoints();

	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CGeneralSmokeCreator* iSmokeCreatorBottomRight;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // JAPANESECARRIERSHOHO_H
