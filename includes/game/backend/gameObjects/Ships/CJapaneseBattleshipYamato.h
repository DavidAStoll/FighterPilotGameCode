/*
 ============================================================================
 Name		: JapaneseBattleshipYamato.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseBattleshipYamato declaration
 ============================================================================
 */

#ifndef JAPANESEBATTLESHIPYAMATO_H
#define JAPANESEBATTLESHIPYAMATO_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"

class CGun7Inch;
class CGun14Inch;
class CShipDual75mmGun;
class CShipDualMachineGun;
class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CJapaneseBattleshipYamato
 * 
 */
class CJapaneseBattleshipYamato : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseBattleshipYamato();
	static CJapaneseBattleshipYamato* New(TInt aXLocation, TBool aReflected);
	
	static CJapaneseBattleshipYamato* LoadJapaneseBattleshipYamato();
	static void InitilizeJapaneseBattleshipYamato();
	static void RemoveJapaneseBattleshipYamato();
	virtual void Update();
	virtual void Draw();
  virtual void UpdateGunPriorities(); //stub
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	CJapaneseBattleshipYamato(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
	void ConstructWithoutCreatingOtherObjectsL();
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseBattleshipYamatoTextures();
	static void UnloadJapaneseBattleshipYamatoTextures();
	static void CreateJapaneseBattleshipYamatoCollisionPointsL();
	static void RemoveJapaneseBattleshipYamatoCollisionPoints();

	CGun14Inch* iFront14InchGun;
	CGun14Inch* iFrontTop14InchGun;
	CGun7Inch* i7InchGun;
	CShipDualMachineGun* iFrontMachineGun1;
	CShipDualMachineGun* iFrontMachineGun2;
	CShipDualMachineGun* iFrontMachineGun3;
	CShipDualMachineGun* iBack1MachineGun;
	CShipDualMachineGun* iBack2MachineGun;
	CShipDualMachineGun* iBack3MachineGun;
	CShipDual75mmGun* iFront75mmGun;
	CShipDual75mmGun* iBack75mmGun;
	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorTopMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	CFlagPole* iFlagPole;

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // JAPANESEBATTLESHIPYAMATO_H
