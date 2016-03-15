/*
 ============================================================================
 Name		: Hq.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHq declaration
 ============================================================================
 */

#ifndef HQ_H
#define HQ_H

#include "includes/game/backend/basicClasses/CBuilding.h"

class CFlagPole;
class CJapaneseAAConcrete;
class CJapaneseAA75mmType88;
class CJapaneseAA20mmMachineCannon;
// CLASS DECLARATION

/**
 *  CHq
 * 
 */
class CHq: public CBuilding
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHq();

	/**
	 * Two-phased constructor.
	 */
	static CHq* CreateLoadHq();//only used as a temp object, used to create an object for loading puposes
	static CHq* New(TPoint aLocation, TConflictSide aConflictSide);
	static CHq* New(TInt aXLocation, TConflictSide aConflictSide);

	static void InitilizeHq();
	static void RemoveHq();
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType); // method call from outside to inflict damage on the object, might have special death animation for specific damage
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHq(TPoint aLocation, SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void ConstructWithoutObjectCreationL();

	static void LoadHqTextures();
	static void UnloadHqTextures();

	//to keep track of units attached to this building
	CFlagPole* iFlagPole;
	CJapaneseAA20mmMachineCannon* iRightMachineGun1;
	CJapaneseAA20mmMachineCannon* iRightMachineGun2;
	CJapaneseAA20mmMachineCannon* iLeftMachineGun1;
	CJapaneseAA20mmMachineCannon* iLeftMachineGun2;
	CJapaneseAA75mmType88* iRightType88Gun1;
	CJapaneseAA75mmType88* iRightType88Gun2;
	CJapaneseAA75mmType88* iLeftType88Gun1;
	CJapaneseAA75mmType88* iLeftType88Gun2;
	CJapaneseAAConcrete* iRightConcreteGun1;
	CJapaneseAAConcrete* iLeftConcreteGun1;

};

#endif // HQ_H
