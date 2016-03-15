/*
 ============================================================================
 Name		: JapaneseAA75mmType88.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseAA75mmType88 declaration
 ============================================================================
 */

#ifndef JAPANESEAA75MMTYPE88_H
#define JAPANESEAA75MMTYPE88_H

// INCLUDES
#include "includes/game/backend/basicClasses/CAAArterially.h"

class CGunAA75mm;

// CLASS DECLARATION

/**
 *  CJapaneseAA75mmType88
 * 
 */

class CJapaneseAA75mmType88: public CAAArterially
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseAA75mmType88();

	/**
	 * Two-phased constructor.
	 */
	static CJapaneseAA75mmType88* New(TPoint aLocation, TConflictSide aSide);
	static CJapaneseAA75mmType88* New(TPoint aLocation);
	static CJapaneseAA75mmType88* New(TInt aXLocation);
	static CJapaneseAA75mmType88* New(TInt aXLocation, TConflictSide aSide);
	static CJapaneseAA75mmType88* New(TPoint aLocation, TBool aCollideWithPlanes);

	static void InitilizeJapaneseAA75mmType88();
	static void RemoveJapaneseAA75mmType88();

	virtual void Update();
	virtual void Draw();
	virtual void Shoot(TInt aRange);
	virtual void DrawAtPositionWithValues(TPointIntFloat& aPosition, TIntFloat lWidth, TIntFloat lHeight);
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CJapaneseAA75mmType88(TPoint aLocation, SGameObjectAttributes& aAttributes);//by default with collide with planes
	CJapaneseAA75mmType88(TPoint aLocation, SGameObjectAttributes& aAttributes, TBool aCollideWithPlanes);
	//should be called if canon has been rotated sinc this method will reposition the canon back to its fix point
	virtual void RepositionGun();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	CTextureObject* iBase;
	CGunAA75mm* iGunAA75mm;
	TBool iCollideWithPlanes;

	static void LoadJapaneseAA75mmType88Textures();
	static void UnloadJapaneseAA75mmType88Textures();

	void ConstructL();
};

#endif // JAPANESEAA75MMTYPE88_H
