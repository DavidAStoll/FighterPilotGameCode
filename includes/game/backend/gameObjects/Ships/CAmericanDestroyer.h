/*
 ============================================================================
 Name		: AmericanDestroyer.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanDestroyer declaration
 ============================================================================
 */

#ifndef AMERICANDESTROYER_H
#define AMERICANDESTROYER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"

class CGun7Inch;
class CShipDual75mmGun;
class CShipDualMachineGun;
class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CAmericanDestroyer
 * 
 */
class CAmericanDestroyer : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanDestroyer();
	static CAmericanDestroyer* New(TInt aXLocation, TBool aReflected);
	
	static void InitilizeAmericanDestroyer();
	static void RemoveAmericanDestroyer();
	virtual void Update();
	virtual void Draw();
  virtual void UpdateGunPriorities(); //stub
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

protected:

	CAmericanDestroyer(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadAmericanDestroyerTextures();
	static void UnloadAmericanDestroyerTextures();
	static void CreateAmericanDestroyerCollisionPointsL();
	static void RemoveAmericanDestroyerCollisionPoints();

	CGun7Inch* i7InchGun;
	CShipDual75mmGun* i75mmGun;
	CShipDualMachineGun* iFrontMachineGun;
	CShipDualMachineGun* iBackMachineGun1;
	CShipDualMachineGun* iBackMachineGun2;
	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // AMERICANDESTROYER_H
