/*
 ============================================================================
 Name		: GroundUnitOverloadAI.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGroundUnitOverlordAI declaration
 ============================================================================
 */

#ifndef GROUNDUNITOVERLOADAI_H
#define GROUNDUNITOVERLOADAI_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
// CLASS DECLARATION
class CGroundUnit;

enum TGroundUnitOverlordAIStates
{
	EGroundUnitOverlordAIStateWaiting, EGroundUnitOverlordAIStateWalking, EGroundUnitOverlordAIStateFighting
};

/**
 *  CGroundUnitOverlordAI
 * 
 */

struct SProtectPositionSpan
{
	TIntFloat LeftXPosition;
	TIntFloat RightXPosition;
};

//this order merges the types of different order you can give. So you can store them somwhere and assign them once they are needed
struct SGroundUnitOrder
{
	TBool ShouldProtectArea;
	SProtectPositionSpan ProtectionArea;
	TInt DefaultXPosition;
	TBool MoveToLocation;
	TInt XLocationDestination;
};

class CGroundUnitOverlordAI: public CMoveableGameObject
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGroundUnitOverlordAI();

	static CGroundUnitOverlordAI* New(TConflictSide aConflictSide);
	static CGroundUnitOverlordAI* New(TConflictSide aConflictSide, SProtectPositionSpan aArea, TInt aDefaultXPosition);

	virtual void Update();//run logic code, do some housekeeping work and also check if close to an EnemyOverlord
	void GiveOrder(SGroundUnitOrder& aOrder);
	void AddGroundUnit(CGroundUnit* aGroundUnit);//gives this overload control of a new ground unit
	void MoveGroundUnitsToLocation(TInt aXLocation); //gives orders where the GroundUnits owned by the overlord should move
	void ProtectArea(SProtectPositionSpan& aArea, TInt aDefaultXPosition);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CGroundUnitOverlordAI(SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void UpdatePosition(); //runs code that checks where the Overlord is located on the map
	void TurnGroundUnitsToFacePosition(TIntFloat& aXPosition);
	CGroundUnitOverlordAI* FindSuitableTarget();
	CGroundUnitOverlordAI* FindSuitableTarget(SProtectPositionSpan& aSpaningArea);
	CPointerArray<CGroundUnit>* iGameObjectInControlOf;
	CGroundUnitOverlordAI* iCurrentEnemyOverlord;
	TGroundUnitOverlordAIStates iCurrentState;
	TIntFloat iXTargetLocation;
	TIntFloat iDistanceUntilEngage;
	TBool iHaveWalkingTarget;
	TBool iIsInProtectingPositionMode;
	TInt iDefaultXPosition;
	SProtectPositionSpan iProtectedArea;

};

#endif // GROUNDUNITOVERLOADAI_H
