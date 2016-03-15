/*
 ============================================================================
 Name		: AmericanLCT.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanLCT declaration
 ============================================================================
 */

#ifndef AMERICANLCT_H
#define AMERICANLCT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"
#include "includes/game/backend/gameObjects/AI/CGroundUnitOverlordAI.h"
// CLASS DECLARATION

class CGeneralSmokeCreator;
class CShipDualMachineGun;

enum TAmericanLCTType
{
	EAmericanLCTTypeEmpty,EAmericanLCTTypeTankAndTroops,EAmericanLCTTypeTroopsOnly,EAmericanLCTTypeTanksOnly
};

/**
 *  CAmericanLCT
 * 
 */
class CAmericanLCT : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAmericanLCT();

	/**
	 * Two-phased constructor.
	 */
	static CAmericanLCT* New(TInt aXLocation, TBool aReflected, TAmericanLCTType aType, SGroundUnitOrder& aOrder);
	
	static void InitilizeAmericanLCT();
	static void RemoveAmericanLCT();
	TBool GetIsLoaded();
	virtual void Update();
	virtual void Draw();
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);
	
private:

	void UnloadTroopsAtLocation(TPointIntFloat& aLocation);
	
	CAmericanLCT(TInt aXLocation, SGameObjectAttributes& aAttributes, TAmericanLCTType aType, SGroundUnitOrder& aOrder);
	void ConstructL();
	static void LoadAmericanLCTTextures();
	static void UnloadAmericanLCTTextures();
	static void CreateAmericanLCTCollisionPointsL();
	static void RemoveAmericanLCTCollisionPoints();
	
	CGeneralSmokeCreator* iSmokeCreator;
	CShipDualMachineGun* iMachineGun;
	TAmericanLCTType iLCTType;
	TBool iHasTroopsLoaded;
	SGroundUnitOrder iOrderForUnloadedTroops;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // AMERICANLCT_H
