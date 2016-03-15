/*
 ============================================================================
 Name		: ShipArteriallyAI.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipArteriallyAI declaration
 ============================================================================
 */

#ifndef SHIPARTERIALLYAI_H
#define SHIPARTERIALLYAI_H

// INCLUDES
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

class CMobileRotatingWeapon;
class CGameObject;

// CLASS DECLARATION

//these parameters will set what the AI will be doing in each specific case
struct SShipArteriallyAIWaitingCase
{
	TInt FrameDelay;
	TInt FrameDelayAfterShot;
};

struct SShipArteriallyAIShootingCase
{
	TInt NeededAngleChange; //how many degrees we need to change the current angle
};

enum TShipArteriallyAICurrentState
{
	EShipArteriallyAIWaiting, EShipArteriallyAIShooting
};

/**
 *  CShipArteriallyAI
 * 
 */
class CShipArteriallyAI
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CShipArteriallyAI();

	/**
	 * Constructor for performing 1st stage construction
	 * 
	 */
	static CShipArteriallyAI* New(CMobileRotatingWeapon* aArterially, SShipArteriallyAIWaitingCase& aWaitingCase);
	static CShipArteriallyAI* New(CMobileRotatingWeapon* aArterially, SShipArteriallyAIWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot);
	void Update();
	void SetTargetPriorities(TUint aTargetPriorities);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	//use this constructor if you don't want to use BurstFire Mode, that is only one shoot is fired for one aiming at a target
	CShipArteriallyAI(CMobileRotatingWeapon* aArterially, SShipArteriallyAIWaitingCase& aWaitingCase);
	//this constructor will shoot several shots at the at the same angel and range (but each shot has usually some spread)
	CShipArteriallyAI(CMobileRotatingWeapon* aArterially, SShipArteriallyAIWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot);
	//this method looks if an enemy is in range and targets it by setting iShootingCase to the proper values to hit the target
	//if no target is close enough it will not target anything and return false
	TBool TargetEnemy();
	void ConstructL();
	TInt FindBestAngleToHitTarget(TInt aDistanceToTarget);
	void CalculateProjectileSpeedForMaxRange();
	void SetupRangeForGunAngles();//populates the range table that keeps track at what angle we shoot what distance, needs to have speed of projectile first set
	TBool GunIsFacingCorrectSideToAttackTarget(CGameObject* aTarget); //performs a check to see if the gun can actually shoot at the target, that is, if the gun faces to the right it can only shoot at stuff to its right
	
	CMobileRotatingWeapon* iShipGun;
	TInt* iRangeForDifferentAngles; // no need to save, will be updated when created
	SShipArteriallyAIWaitingCase iWaitingCase;
	SShipArteriallyAIShootingCase iShootingCase;
	TInt iCurrentFrame;
	TInt iShotsPerBurstFire;//tells us how many times the gun will shoot at a traget until it goes back to rest mode
	TInt iWaitFramesUntilNextBurstShot;
	TInt iCurrentShotInBurst;
	TInt iGunDegreeRange; //keeps track on how many different types of degree option this gun has
	TUint iTargetPriorities;
	TIntFloat iSpeedForProjectile; //make it float for higher precision
	TBool iGunReflected;
	TBool iBurstFireMode;
	TBool iJustShot;
	TShipArteriallyAICurrentState iCurrentState;

	};

#endif // SHIPARTERIALLYAI_H
