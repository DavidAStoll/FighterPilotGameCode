/*
 ============================================================================
 Name		: AmericanBattleshipUSSTennessee.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanBattleshipUSSTennessee declaration
 ============================================================================
 */

#ifndef AMERICANBATTLESHIPUSSTENNESSEE_H
#define AMERICANBATTLESHIPUSSTENNESSEE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"

class CGun14Inch;
class CShipDual75mmGun;
class CShipDualMachineGun;
class CGeneralSmokeCreator;
// CLASS DECLARATION

/**
 *  CAmericanBattleshipUSSTennessee
 * 
 */
class CAmericanBattleshipUSSTennessee : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanBattleshipUSSTennessee();

	/**
	 * Two-phased constructor.
	 */
	static CAmericanBattleshipUSSTennessee* New(TInt aXLocation, TBool aReflected);
	
	static CAmericanBattleshipUSSTennessee* LoadAmericanBattleshipUSSTennessee();
	static void InitilizeAmericanBattleshipUSSTennessee();
	static void RemoveAmericanBattleshipUSSTennessee();
	virtual void Update();
	virtual void Draw();
  virtual void UpdateGunPriorities();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAmericanBattleshipUSSTennessee(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadAmericanBattleshipUSSTennesseeTextures();
	static void UnloadAmericanBattleshipUSSTennesseeTextures();
	static void CreateAmericanBattleshipUSSTennesseeCollisionPointsL();
	static void RemoveAmericanBattleshipUSSTennesseeCollisionPoints();

	CGun14Inch* iFront14InchGun;
	CGun14Inch* iFrontTop14InchGun;
	CShipDualMachineGun* iFrontMachineGun1;
	CShipDualMachineGun* iFrontMachineGun2;
	CShipDualMachineGun* iFrontMachineGun3;
	CShipDualMachineGun* iTopMachineGun;
	CShipDualMachineGun* iBack1MachineGun;
	CShipDualMachineGun* iBack2MachineGun;
	CShipDualMachineGun* iBack3MachineGun;
	CShipDual75mmGun* iFront75mmGun;
	CShipDual75mmGun* iTop75mmGun;
	CShipDual75mmGun* iBack75mmGun;
	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CFlagPole* iFlagPole;

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
	};

#endif // AMERICANBATTLESHIPUSSTENNESSEE_H
