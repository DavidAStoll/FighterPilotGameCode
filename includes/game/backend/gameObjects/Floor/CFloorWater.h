/*
 ============================================================================
 Name		: FloorWater.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFloorWater declaration
 ============================================================================
 */

#ifndef FLOORWATER_H
#define FLOORWATER_H

#include "includes/game/backend/basicClasses/CGameObject.h"

enum TWaterTextures
{
	EWaterTexture1 = 0, EWaterTexture2, EWaterTexture3, EWaterTexture4
};

class CFloorWater: public CGameObject
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CFloorWater();

	/**
	 * Two-phased constructor.
	 */
	static CFloorWater* New(TPoint aLocation, TInt aWidth);
	static CFloorWater* New(TInt aXLocation, TInt aWidth);
	virtual void PossibleCollision(CInterval*& aInterval);
	static void InitilizeFloorWater();
	static void RemoveFloorWater();
	virtual void Update();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CFloorWater(TPoint aLocation, SGameObjectAttributes& aAttributes);
	virtual void Die(TDamageType aType); // does nothing since land can't die

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(TInt aWidth);

	static void LoadFloorWaterTextures();
	static void UnloadFloorWaterTextures();

	TWaterTextures iCurrentTextureBeingDisplayed;
	TInt iCurrentFrame;

};

#endif // FLOORWATER_H
