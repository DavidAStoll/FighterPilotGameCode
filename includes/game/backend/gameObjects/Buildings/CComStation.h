/*
 ============================================================================
 Name		: ComStation.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CComStation declaration
 ============================================================================
 */

#ifndef COMSTATION_H
#define COMSTATION_H

#include "includes/game/backend/basicClasses/CBuilding.h"

class CFlagPole;
// CLASS DECLARATION

/**
 *  CComStation
 * 
 */
class CComStation: public CBuilding
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CComStation();

	/**
	 * Two-phased constructor.
	 */
	static CComStation* CreateLoadComStation();//only used as a temp object, used to create an object for loading puposes
	static CComStation* New(TPoint aLocation, TConflictSide aConflictSide); // can be used if only one com station exists
	static CComStation* New(TInt aXLocation, TConflictSide aConflictSide);
	static CComStation* New(TPoint aLocation, TConflictSide aConflictSide, TInt aSecOffsetFromCallingBomber, TInt aHeightOffset);//used, if we have more than 2 com stations, so they don't call bombers at the same time, since they would overlap
	static CComStation* New(TInt aXLocation, TConflictSide aConflictSide, TInt aSecOffsetFromCallingBomber, TInt aHeightOffset);//height off set can be used, to create the bomber below its normal height position

	static void InitilizeComStation();
	static void RemoveComStation();
	static void ComStation();
	virtual void Update();
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType); // method call from outside to inflict damage on the object, might have special death animation for specific damage
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CComStation(TPoint aLocation, SGameObjectAttributes& aAttributes, TInt aSecOffsetFromCallingBomber, TInt aHeightOffset);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void ConstructWithoutObjectCreationL();

	static void LoadComStationTextures();
	static void UnloadComStationTextures();

	CFlagPole* iFlagPole;
	TInt iCurrentFrame;
	TInt iExtraFrameOffset;
	TInt iExtraHeightOffset;

};

#endif // COMSTATION_H
