/*
 ============================================================================
 Name		: ShipCarrier.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipCarrier declaration
 ============================================================================
 */

#ifndef SHIPCARRIER_H
#define SHIPCARRIER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"

// CLASS DECLARATION

/**
 *  CShipCarrier
 * 
 */
class CAircraft;
class CLandingSurface;
class CFlagPole;
class CShipCarrier : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CShipCarrier();
  virtual void Update();
  void SetTarget(CGameObject* aTarget);
  void ShipCarrierBasicUpdate();
  void ShipCarrierSaveContentToDisk(CFileWriteStream &aOutputStream);
  void ShipCarrierLoadContentFromDisk(CFileReadStream &aReadStream);

protected:

	CShipCarrier(TInt aZValue,TInt aXLocation, TInt aKeelDepth, TIntFloat aShipMaxSpeed, TInt aFlagPoleXOffset, TInt aFlagPoleYOffset, SGameObjectAttributes& aAttributes);
	
	TInt iNumberOfAircraftsLeft;
	TInt iFramesLeftUntilSpawnPlane;
	TInt iFlagPoleXOffset;
	TInt iFlagPoleYOffset;
	TBool iDoesNotSpawnPlanes; //by default NO, but the home carrier should not
	CGameObject* iTarget;
	CFlagPole* iFlagPole; //all carriers have a flagpole
	CLandingSurface* iLandingSurface;
	CAircraft* iFirstAircraft;
	CAircraft* iSecondAircraft;
	};

#endif // SHIPCARRIER_H
