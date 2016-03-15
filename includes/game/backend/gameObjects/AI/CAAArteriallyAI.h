/*
 ============================================================================
 Name		: AAArteriallyAI.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CAAArteriallyAI declaration
 ============================================================================
 */

#ifndef AAARTERIALLYAI_H
#define AAARTERIALLYAI_H

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

// CLASS DECLARATION

/**
 *  CArteriallyAI
 * 
 */

//these parameters will set what the AI will be doing in each specific case
struct SWaitingCase
{
	TInt FrameDelay;
	TInt FrameDelayAfterShot;
};

struct ShootingCase
{
	TInt NeededAngel;
	TInt Range;
};

//forward declaration
class CAAArterially;

enum TAAArteriallyAICurrentState
{
	EAAArteriallyAIWaiting, EAAArteriallyAIShooting
};

class CAAArteriallyAI
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAAArteriallyAI();

	/**
	 * Constructor for performing 1st stage construction
	 */
	//use this constructor if you don't want to use BurstFire Mode, that is only one shoot is fired for one aiming at a target
	CAAArteriallyAI(CAAArterially * aArterially, SWaitingCase& aWaitingCase);
	//this constructor will shoot several shots at the at the same angel and range (but each shot has usually some spread)
	CAAArteriallyAI(CAAArterially * aArterially, SWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot);

	void Update();
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	//this method looks if an enemy is in range and targets it by setting iShootingCase to the proper values to hit the plane
	//if no plane is close enough it will not target anything and return false
	TBool TargetEnemyPlane();

	CAAArterially * iAAArterially;
	SWaitingCase iWaitingCase;
	ShootingCase iShootingCase;
	TInt iCurrentFrame;
	TInt iShotsPerBurstFire;//tells us how many times the gun will shoot at a traget until it goes back to rest mode
	TInt iWaitFramesUntilNextBurstShot;
	TBool iBurstFireMode;
	TBool iJustShotAtAirplane;
	TInt iCurrentShotInBurst;
	TAAArteriallyAICurrentState iCurrentState;
};

#endif // AAARTERIALLYAI_H
