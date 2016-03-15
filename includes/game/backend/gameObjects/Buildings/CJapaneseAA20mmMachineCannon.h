/*
 ============================================================================
 Name		: JapaneseAA20mmMachineCannon.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseAA20mmMachineCannon declaration
 ============================================================================
 */

#ifndef JAPANESEAA20MMMACHINECANNON_H
#define JAPANESEAA20MMMACHINECANNON_H

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/game/backend/basicClasses/CAAArterially.h"

class CMachineGun13mm;

// CLASS DECLARATION

/**
 *  CJapaneseAA20mmMachineCannon
 * 
 */
class CJapaneseAA20mmMachineCannon: public CAAArterially
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseAA20mmMachineCannon();

	/**
	 * Two-phased constructor.
	 */
	static CJapaneseAA20mmMachineCannon* New(TPoint aLocation, TBool aCollideWithPlanes);
	static CJapaneseAA20mmMachineCannon* New(TPoint aLocation);
	static CJapaneseAA20mmMachineCannon* New(TInt aXLocation, TConflictSide aSide);
	static CJapaneseAA20mmMachineCannon* New(TInt aXLocation);

	static void InitilizeJapaneseAA20mmMachineCannon();
	static void RemoveJapaneseAA20mmMachineCannon();

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
	CJapaneseAA20mmMachineCannon(TPoint aLocation, SGameObjectAttributes& aAttributes);//by default building will collide with planes
	CJapaneseAA20mmMachineCannon(TPoint aLocation, SGameObjectAttributes& aAttributes, TBool aCollideWithPlanes);
	//should be called if canon has been rotated sinc this method will reposition the canon back to its fix point
	virtual void RepositionGun();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	CTextureObject* iBase;
	CMachineGun13mm* i13mmMachineGun;
	TBool iCollideWithPlanes;

	static void LoadJapaneseAA20mmMachineCannonTextures();
	static void UnloadJapaneseAA20mmMachineCannonTextures();
	void ConstructL();
};

#endif // JAPANESEAA20MMMACHINECANNON_H
