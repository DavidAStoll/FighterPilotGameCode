/*
 ============================================================================
 Name		: BackCannonAI.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBackCannonAI declaration
 ============================================================================
 */

#ifndef BACKCANNONAI_H
#define BACKCANNONAI_H

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

// CLASS DECLARATION

//these parameters will set what the AI will be doing in each specific case
struct SBackCannonAIWaitingCase
{
	TInt FrameDelay;
	TInt FrameDelayAfterShot;
};

struct SBackCannonAIShootingCase
{
	TInt NeededAngleChange; //how many degrees we need to change the current angle
	TInt Range;
};

enum TBackCannonAICurrentState
{
	EBackCannonAIWaiting, EBackCannonAIShooting
};

class CMobileAAGun;
/**
 *  CBackCannonAI
 * 
 */
class CBackCannonAI
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBackCannonAI();

	//use this constructor if you don't want to use BurstFire Mode, that is only one shoot is fired for one aiming at a target
	CBackCannonAI(CMobileAAGun* aMobileAAGun, SBackCannonAIWaitingCase& aWaitingCase);
	//this constructor will shoot several shots at the at the same angel and range (but each shot has usually some spread)
	CBackCannonAI(CMobileAAGun* aMobileAAGun, SBackCannonAIWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot);

	void Update();
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	
	TInt FindAngleChangeNeededForTargetAngle(TInt aTargetAngle); //does the complex calculation to find out what way the gun needs to be turned to get to the wanted angle

	//this method looks if an enemy is in range and targets it by setting iShootingCase to the proper values to hit the plane
	//if no plane is close enough it will not target anything and return false
	TBool TargetEnemyPlane();

	CMobileAAGun* iMobileAAGun;
	SBackCannonAIWaitingCase iWaitingCase;
	SBackCannonAIShootingCase iShootingCase;
	TBool iGunReflected;
	TInt iCurrentFrame;
	TInt iShotsPerBurstFire;//tells us how many times the gun will shoot at a traget until it goes back to rest mode
	TInt iWaitFramesUntilNextBurstShot;
	TBool iBurstFireMode;
	TBool iJustShotAtAirplane;
	TInt iCurrentShotInBurst;
	TBackCannonAICurrentState iCurrentState;
};

#endif // BACKCANNONAI_H
