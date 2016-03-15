/*
 ============================================================================
 Name		: JapaneseTroopTransporter.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseTroopTransporter declaration
 ============================================================================
 */

#ifndef JAPANESETROOPTRANSPORTER_H
#define JAPANESETROOPTRANSPORTER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"
class CShipDualMachineGun;

// CLASS DECLARATION

/**
 *  CJapaneseTroopTransporter
 * 
 */
class CJapaneseTroopTransporter : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseTroopTransporter();
	static CJapaneseTroopTransporter* New(TInt aXLocation, TBool aReflected, TInt aSecOffset, TJapaneseLCTType aLCTType, SGroundUnitOrder& aOrder, TBool aSpawnShips = true);
	static void InitilizeJapaneseTroopTransporter();
	static void RemoveJapaneseTroopTransporter();
	virtual void Update();
	virtual void Draw();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	
	CJapaneseTroopTransporter(TInt aXLocation, SGameObjectAttributes& aAttributes, TInt aSecOffset, TJapaneseLCTType aLCTType, SGroundUnitOrder& aOrder, TBool aSpawnShips);
	void ConstructL();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseTroopTransporterTextures();
	static void UnloadJapaneseTroopTransporterTextures();
	static void CreateJapaneseTroopTransporterCollisionPointsL();
	static void RemoveJapaneseTroopTransporterCollisionPoints();

	CShipDualMachineGun* iFrontMachineGun;
	CShipDualMachineGun* iBackMachineGun;
	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	TBool iCanSpawnShips;
  TInt iCurrentFrame;
  TInt iExtraFrameOffset; //offset from base Secs
  TJapaneseLCTType iLCTSpawnType;
  SGroundUnitOrder iOrder;

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
	};

#endif // JAPANESETROOPTRANSPORTER_H
