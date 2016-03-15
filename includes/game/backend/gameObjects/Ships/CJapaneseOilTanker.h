/*
 ============================================================================
 Name		: JapaneseOilTanker.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseOilTanker declaration
 ============================================================================
 */

#ifndef JAPANESEOILTANKER_H
#define JAPANESEOILTANKER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"
class CShipDualMachineGun;

// CLASS DECLARATION

/**
 *  CJapaneseOilTanker
 * 
 */
class CJapaneseOilTanker : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseOilTanker();
	static CJapaneseOilTanker* New(TInt aXLocation, TBool aReflected);
	static void InitilizeJapaneseOilTanker();
	static void RemoveJapaneseOilTanker();
	virtual void Update();
	virtual void Draw();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	CJapaneseOilTanker(TInt aXLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();
  virtual void SinkingFunctionCallWhenShipRotates();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	static void LoadJapaneseOilTankerTextures();
	static void UnloadJapaneseOilTankerTextures();
	static void CreateJapaneseOilTankerCollisionPointsL();
	static void RemoveJapaneseOilTankerCollisionPoints();

	CShipDualMachineGun* iFrontMachineGun;
	CShipDualMachineGun* iBackMachineGun;
	CGeneralSmokeCreator* iSmokeCreatorLeft;
	CGeneralSmokeCreator* iSmokeCreatorMiddle;
	CGeneralSmokeCreator* iSmokeCreatorRight;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // JAPANESEOILTANKER_H
