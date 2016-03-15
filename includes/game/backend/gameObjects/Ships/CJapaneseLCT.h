/*
 ============================================================================
 Name		: JapaneseLCT.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseLCT declaration
 ============================================================================
 */

#ifndef JAPANESELCT_H
#define JAPANESELCT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CShip.h"
#include "includes/game/backend/gameObjects/AI/CGroundUnitOverlordAI.h"

// CLASS DECLARATION

class CGeneralSmokeCreator;
class CShipDualMachineGun;

enum TJapaneseLCTType
{
	EJapaneseLCTTypeEmpty,EJapaneseLCTTypeTankAndTroops,EJapaneseLCTTypeTroopsOnly,EJapaneseLCTTypeTanksOnly
};

/**
 *  CJapaneseLCT
 * 
 */
class CJapaneseLCT : public CShip
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseLCT();

	/**
	 * Two-phased constructor.
	 */
	static CJapaneseLCT* New(TInt aXLocation, TBool aReflected, TJapaneseLCTType aType, SGroundUnitOrder& aOrder);
	
	static void InitilizeJapaneseLCT();
	static void RemoveJapaneseLCT();
	TBool GetIsLoaded();
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void Update();
	virtual void Draw();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aOutputStream);
	
private:

	void UnloadTroopsAtLocation(TPointIntFloat& aLocation);
	
	CJapaneseLCT(TInt aXLocation, SGameObjectAttributes& aAttributes, TJapaneseLCTType aType, SGroundUnitOrder& aOrder);
	void ConstructL();
	static void LoadJapaneseLCTTextures();
	static void UnloadJapaneseLCTTextures();
	static void CreateJapaneseLCTCollisionPointsL();
	static void RemoveJapaneseLCTCollisionPoints();
	
	CGeneralSmokeCreator* iSmokeCreator;
	CShipDualMachineGun* iMachineGun;
	TJapaneseLCTType iLCTType;
	TBool iHasTroopsLoaded;
	SGroundUnitOrder iOrderForUnloadedTroops;
	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision

	};

#endif // JAPANESELCT_H
