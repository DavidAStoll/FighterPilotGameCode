/*
 ============================================================================
 Name		: JapaneseCarrierShokaku.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseCarrierShokaku declaration
 ============================================================================
 */

#ifndef JAPANESECARRIERSHOKAKU_H
#define JAPANESECARRIERSHOKAKU_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShipCarrier.h"
class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CJapaneseCarrierShokaku
 * 
 */
class CJapaneseCarrierShokaku : public CShipCarrier
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseCarrierShokaku();
	static CJapaneseCarrierShokaku* LoadJapaneseCarrierShokaku();
	static CJapaneseCarrierShokaku* New(TInt aXLocation, TBool aReflected);

	static void InitilizeJapaneseCarrierShokaku();
	static void RemoveJapaneseCarrierShokaku();
	virtual void Update();
	virtual CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

protected:

	CJapaneseCarrierShokaku(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseCarrierShokakuTextures();
	static void UnloadJapaneseCarrierShokakuTextures();
	static void CreateJapaneseCarrierShokakuCollisionPointsL();
	static void RemoveJapaneseCarrierShokakuCollisionPoints();

	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CGeneralSmokeCreator* iSmokeCreatorTop;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // JAPANESECARRIERSHOKAKU_H
