/*
 * CAircraft.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CAIRCRAFT_H_
#define CAIRCRAFT_H_

#define AIRCRAFT_NUMBER_OF_IMAGES_FOR_START_TURN 3
#define AIRCRAFT_NUMBER_OF_IMAGES_PER_MIDDLE_PART 1
#define AIRCRAFT_NUMBER_OF_IMAGES_FOR_FINISH_TURN 3
#define AIRCRAFT_NUMBER_OF_IMAGES_FOR_TOTAL_TURN  (AIRCRAFT_NUMBER_OF_IMAGES_FOR_START_TURN + AIRCRAFT_NUMBER_OF_IMAGES_PER_MIDDLE_PART + AIRCRAFT_NUMBER_OF_IMAGES_FOR_FINISH_TURN)

// INCLUDES
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/game/backend/basicClasses/CWeapon.h"
#include "includes/game/backend/basicClasses/CPlaneWheels.h"
#include "includes/game/backend/gameObjects/Decor/CAircraftSmokeCreator.h"
#include "includes/game/backend/gameObjects/AI/CHeavyBomberAI.h"

// CLASS DECLARATION

/**
 *  CAircraft
 *
 */
class CFighterAI;

class CAircraft: public CMoveableGameObject
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CAircraft();

	//methods that need to be overriden from MoveableGameObject, since we need other functionality
	virtual void Move();
	virtual void MoveToLocation(TPoint aNewLocation);
	//used to fire the Fire1(Primary) and Fire2(Secondary) weapon of the aircraft
	virtual void Fire1();
	virtual void Fire2();
	//used to check if the Fire1(Primary) and Fire2(Secondary) weapon has still ammo
	virtual TBool Fire1HasAmmunitionLeft();
	virtual TBool Fire2HasAmmunitionLeft();
	//might be cut, since we can't do anything with it for now
	virtual void Update(); //runs the Game Object's logic code to update its member data
	//updates the currentSpeed of the Airplane depening on the Acceleration and DragFactor
	void UpdateSpeed();
	//new methods
	//should be overriden by the child and include what needs to be done in the regular update of the plane
	virtual void UpdateAircraft();
	//this method is called just before the plane goes into a turn, usually turns of several objects attached to the plane
	virtual void BeforeTurnSettings();
	//this method is called after the turn of the plane has been completed, usually turns on several gameobjects to make them visible again
	virtual void AfterTurnSettings();
	void AngleCorrectionIfCrashedDuringTurn();
	virtual void IncreaseSpeed();
	virtual void DecreaseSpeed();
	virtual void IncreaseFlyingAngle();
	virtual void DecreaseFlyingAngle();
	virtual void DecreaseFlyingAngleWithoutSpeedCheck();
	virtual void IncreaseFlyingAngleWithoutSpeedCheck();
	virtual void IncreaseFlyingAngleSmallAngle();
	virtual void DecreaseFlyingAngleSmallAngle();
	virtual void DecreaseFlyingAngleWithoutSpeedCheckSmallAngle();
	virtual void IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
	virtual void IncreaseFlyingAngle(TInt aChange);
	virtual void DecreaseFlyingAngle(TInt aChange);
	virtual void DecreaseFlyingAngleWithoutSpeedCheck(TInt aChange);
	virtual void IncreaseFlyingAngleWithoutSpeedCheck(TInt aChange);
	virtual void TurnRight();
	virtual void TurnLeft();
	virtual void TurnRightWithoutAnimationAndCheck();
	virtual void TurnLeftWithoutAnimationAndCheck();
	virtual void TurnRightWithoutCheck();
	virtual void TurnLeftWithoutCheck();
	virtual void PlayTurnAnimation();
	void TurnLeftWithoutAnimationAndAngleChange();
	void TurnRightWithoutAnimationAndAngleChange();
	TBool GetIsAIControlled();
	virtual void AdjustFlyingSpeedDueToFlyingAngel();//should only be called after changes in flyingAngel
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType); // method call from outside to inflict damage on the object, might have special death animation for specific damage
	//will be overriden by child if something should be done if hit by another fighter
	virtual void RunAIAvoidanceLogicCode();
	TInt* GetFlyingAngel();
	TBool AirplaneCrashed();
	virtual void SetPlaneToYPosition(TIntFloat lYPosition);
	void SetCurrentAbsoluteSpeed(TIntFloat aSpeed);
	void SetIsSinking(TBool aValue);
	CFighterAI* GetFighterAI();
	void SetAIControlled();
	TIntFloat GetTextureHeight();
	TIntFloat GetCurrentAbsoluteSpeed(); //doesn't change with direction or angel
	TIntFloat GetCurrentEngineAcceleration();
	TIntFloat& GetCurrentEngineAccelerationRef();
	TIntFloat GetCurrentDiveAcceleration();
	TIntFloat GetTotalAcceleration();
	TIntFloat& GetCurrentDiveAccelerationRef();
	//returns the max engine accerlation of the plane taking into account with the damage it has taken so far
	TIntFloat GetMaxEngineAccelerationWithCurrentHealth();
	//this is the max engine acceleration of the plane if the plane has 100% health
	TIntFloat GetNormalMaxEngineAcceleration();
	void SetCurrentAcceleration(TIntFloat aValue);
	void ChangeYPosition(TIntFloat aChange);
	void ChangeXPosition(TIntFloat aChange);
	TBool IsUpSideDown();
	TBool IsTurning();
	TBool IsFlying();
	virtual CWeapon* GetPrimaryWeapon();
	virtual CWeapon* GetSecondaryWeapon();
	virtual void EquipWithNewSecondaryWeapon(TWeaponIdentifier aWeaponIdendifier);
	virtual TInt GetAmmunitionLeftOfSecondaryWeapon();
	virtual void RearmAircraft(); //call to this function will rearm all the airplanes weapons
	void SetPlaneIsFlying(TBool aIsFlying);
	virtual void PossibleCollision(CInterval*& aInterval);
	TAircraftType& GetAircraftType();
	void AircraftGameObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void AircraftGameObjectLoadContentFromDisk(CFileReadStream &aReadStream);

protected:

	//stops the engine sound of the aircraft, should be called when the airplane crashes
	virtual void StopEngineSound();
	//adjust the speed due to a change to the flying angle
	virtual void AdjustFlyingSpeedDueToChanges();
	//might be implemented by child, this function is called when the plane crashes with the floor
	virtual void PlaneCrash();
	//plays a particular crash sound
	void PlayCrashSound(const TDamageType aType);
	//needs to be set via constructer arguments
	TBool iIsFlying;
	TBool iGearUp;
	TBool iCrashed;
	TIntFloat iEngineSpeed;//is only affected by the engine acceleration
	TIntFloat iDiveSpeed;//is only affected by the diving acceleration
	TIntFloat iTotalSpeed; // is the sum of the engine and dive Speed
	//needs to be set via MACROS for planeType
	TIntFloat iAngleChangeExtraSpeed;//if the angle of the airplane changes, this either adds or substracts speed from the plane, effect decays linearly over time
	TIntFloat iCurrentEngineAcceleration;
	TIntFloat iCurrentDiveAcceleration;
	TIntFloat iExtraAngelAcceleration;//can be positive or negative, depending if plane is pointing down or upward
	TIntFloat iExtraDownSpeed;
	TIntFloat iMaxEngineAcceleration;
	TIntFloat iNormalMaxEngineAcceleration;//by default 100, loses life when health dropps below 100
	TIntFloat iAircraftChangeDownAngleExtraSpeed;//usually is always the same, but for bigger planes like bomber it might be more
	//always set to the same value
	TBool iIsTurning;
	TBool iIsUpsideDown;
	TBool iIsSinking;
	TInt iFrameInTurnAnimation;
	TInt iFramesPerTurnImage;
	TUint iFramesNeededPerAngleChange; //tells us how many frames have to pass until angle can be changed again
	TUint iFramesSinceLastAngleChange; //keeps how many frames have passed since the last angle change
	TIntFloat iXAxisLength;
	TIntFloat iYAxisLength;
	TIntFloat iMinimumYPosition;//sets the yPosition that the airplane can't go below
	TAircraftType iAircraftType;//tells an outsider what type of aircraft he is looking at, should be set by implementing class
	CPlaneWheels * iFrontWheel;
	CPlaneWheels * iBackWheel;
	CTextureObject* iLandingGearTextureObject;
	CAircraftSmokeCreator* iSmokeCreator1;
	CFighterAI* iFighterAI;
	/**
	 * Constructor for performing 1st stage construction
	 */
	CAircraft(TInt aZValue, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aCurrentAcceleration, TIntFloat aMaxAcceleration, TBool aFlying, TInt aTurnTimeFactor, TInt aAngleChangeTimeFactor);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
};

#endif /* CAIRCRAFT_H_ */
