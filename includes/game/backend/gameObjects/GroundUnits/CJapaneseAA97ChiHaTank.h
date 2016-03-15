/*
 ============================================================================
 Name		: JapaneseAA97ChiHaTank.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseAA97ChiHaTank declaration
 ============================================================================
 */

#ifndef JAPANESEAA97CHIHATANK_H
#define JAPANESEAA97CHIHATANK_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGroundUnit.h"

class CTankAAGun;

/**
 *  CJapaneseAA97ChiHaTank
 * 
 */
class CJapaneseAA97ChiHaTank : public CGroundUnit
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseAA97ChiHaTank();

	/**
	 * Two-phased constructor.
	 */
	static CJapaneseAA97ChiHaTank* New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected);
	
	virtual void Update();// in overrides GameObjects method, in general should be sufficient for most game objects
	virtual void Draw();
	static void InitilizeJapaneseAA97ChiHaTank();
	static void RemoveJapaneseAA97ChiHaTank();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CJapaneseAA97ChiHaTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType);
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	static void LoadJapaneseAA97ChiHaTankTextures();
	static void UnloadJapaneseAA97ChiHaTankTextures();
	static void CreateJapaneseAA97ChiHaTankCollisionPointsL();
	static void RemoveJapaneseAA97ChiHaTankCollisionPoints();
	
	CTankAAGun* iAAGun;
	CGeneralSmokeCreator* iDustCreator;
	static SBoundingCollisionPoint* CollisionPoints;
	
	};


#endif // JAPANESEAA97CHIHATANK_H
