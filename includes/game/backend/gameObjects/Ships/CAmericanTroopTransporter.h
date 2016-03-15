/*
 ============================================================================
 Name		: AmericanTroopTransporter.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanTroopTransporter declaration
 ============================================================================
 */

#ifndef AMERICANTROOPTRANSPORTER_H
#define AMERICANTROOPTRANSPORTER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"
class CShipDualMachineGun;

// CLASS DECLARATION

/**
 *  CAmericanTroopTransporter
 * 
 */
class CAmericanTroopTransporter : public CShip
	{
public:
	// Constructors and destructor

	~CAmericanTroopTransporter();
	static CAmericanTroopTransporter* New(TInt aXLocation, TBool aReflected, TInt aSecOffset, TAmericanLCTType aLCTType, SGroundUnitOrder& aOrder);
	static void InitilizeAmericanTroopTransporter();
	static void RemoveAmericanTroopTransporter();
	virtual void Update();
	virtual void Draw();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	
	CAmericanTroopTransporter(TInt aXLocation, SGameObjectAttributes& aAttributes, TInt aSecOffset, TAmericanLCTType aLCTType, SGroundUnitOrder& aOrder);
	void ConstructL();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadAmericanTroopTransporterTextures();
	static void UnloadAmericanTroopTransporterTextures();
	static void CreateAmericanTroopTransporterCollisionPointsL();
	static void RemoveAmericanTroopTransporterCollisionPoints();

	CShipDualMachineGun* iFrontMachineGun;
	CShipDualMachineGun* iBackMachineGun;
	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
  TInt iCurrentFrame;
  TInt iExtraFrameOffset; //offset from base Secs
  TAmericanLCTType iLCTSpawnType;
  SGroundUnitOrder iOrder;

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
	};

#endif // AMERICANTROOPTRANSPORTER_H
