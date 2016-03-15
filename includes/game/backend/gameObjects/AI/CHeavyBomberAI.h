/*
 * CHeavyBomberAI.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CHEAVYBOMBERAI_H_
#define CHEAVYBOMBERAI_H_

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

// CLASS DECLARATION
class CAircraft;
class CGameObject;

/**
 *  CHeavyBomberAI
 *  This AI will control the bombing on the heavy bomber. It can have an assigned target that it will attack or will choose an alive target by itself.
 *  If the orignal assigned targets gets destroyed it will choose itself another one.
 *
 */

struct SHeavyBombingMission // can be used to specify a specific target to the bomber, what to attack
{
	CGameObject* Target;
};

class CHeavyBomberAI
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHeavyBomberAI();
	CHeavyBomberAI(CAircraft* aBomber, TInt aFirstBombLocationOffset, SHeavyBombingMission aMission);
	CHeavyBomberAI(CAircraft* aBomber, TInt aFirstBombLocationOffset);

	void Update();
	void AssignBombingMission(SHeavyBombingMission aMission);
	void SetTargetPriority(TUint aTargetTypes);
	void SaveOnDisk(CFileWriteStream& aOutputStream);
	void LoadFromDisk(CFileReadStream& aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	void CalculateNeededDistanceForDrop(CGameObject* aTarget);

	CAircraft* iBomber;
	SHeavyBombingMission iBombingMission;
	TBool iHasBombingMissionAssigned;
	TBool iHasPriotizedTargetAssigned;//if this is true, it will not look for closer targets unless the target has been destroyed
	TBool iStartBombing;
	TInt iMiddlePositionOfBombsOffset;
	TInt iNeededDistanceForDrop;
	TInt iExtraWidthOffset;
	TInt iCurrentFrameForClosestTargetCheck;
	TUint iTargetPriority;

};

#endif /* CHEAVYBOMBERAI_H_ */
