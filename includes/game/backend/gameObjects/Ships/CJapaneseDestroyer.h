/*
 ============================================================================
 Name		: JapaneseDestroyer.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseDestroyer declaration
 ============================================================================
 */

#ifndef JAPANESEDESTROYER_H
#define JAPANESEDESTROYER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"

class CGun7Inch;
class CShipDual75mmGun;
class CShipDualMachineGun;
class CGeneralSmokeCreator;

// CLASS DECLARATION

/**
 *  CJapaneseDestroyer
 * 
 */
class CJapaneseDestroyer : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseDestroyer();

	/**
	 * Two-phased constructor.
	 */
	static CJapaneseDestroyer* New(TInt aXLocation, TBool aReflected);
	
	static void InitilizeJapaneseDestroyer();
	static void RemoveJapaneseDestroyer();
	virtual void Update();
	virtual void Draw();
  virtual void UpdateGunPriorities(); //stub
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

protected:

	CJapaneseDestroyer(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseDestroyerTextures();
	static void UnloadJapaneseDestroyerTextures();
	static void CreateJapaneseDestroyerCollisionPointsL();
	static void RemoveJapaneseDestroyerCollisionPoints();

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

#endif // JAPANESEDESTROYER_H
