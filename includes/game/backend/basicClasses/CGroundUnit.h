/*
 ============================================================================
 Name		: GroundUnit.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGroundUnit declaration
 ============================================================================
 */

#ifndef GROUNDUNIT_H
#define GROUNDUNIT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/game/backend/gameObjects/Decor/CGeneralSmokeCreator.h"

enum TGroundUnitStates
{
	EGroundUnitStateMoving, EGroundUnitStateStanding, EGroundUnitStateFiring, EGroundUnitStateDieing
};

// CLASS DECLARATION

/**
 *  CGroundUnit
 * 
 */
class CGroundUnit: public CMoveableGameObject
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGroundUnit();
	virtual void Shoot();//stub function,causes the the unit to fire
	virtual void Update();// in overrides GameObjects method, in general should be sufficient for most game objects
	virtual void Move();//overrides MoveableGameObject's method

	TIntFloat GetMaxSpeed();
	void SwitchToMovingState();
	void SwitchToStandingState();
	void SwitchToFiringState();
	void SetCurrentSpeed(TIntFloat aSpeed);
	void GroundUnitObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void GroundUnitObjectLoadContentFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	virtual void UpdateCurrentState(TGroundUnitStates aState);
	virtual void UpdateSpecific(); //should contain all the code that is specific for the specific ground unit
	CGroundUnit(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aMaxSpeed, TInt aMaxWaitFramesUntilShoots, TInt aFramesNeededForFiringAnimation);
	TIntFloat iGroundUnitMaxSpeed;
	TInt iMaxWaitFramesUntilNextShoot; //this is an offset that is used to let the unit wait to make it next shoot if it is in the firing state
	TInt iFramesNeededForFiringAnimation;
	TInt iCurrentWaitFrameForNextShoot;
	TBool iJustShoot;
	TGroundUnitStates iCurrentState;
	CGeneralSmokeCreator* iSmokeCreator;

};

#endif // GROUNDUNIT_H
