/*
 * CFighterAI.h
 *
 *  Created on: Aug 27, 2012
 *      Author: dstoll
 */

#ifndef CFIGHTERAI_H_
#define CFIGHTERAI_H_

#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/basicClasses/CGameObject.h"

class CShip;
class CAircraft;

#define FIGHTER_AI_ALL_TARGET_TYPES (GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP)
#define FIGHTER_AI_ALL_TARGET_BUT_SHIP (GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_TROOPS)
#define FIGHTER_AI_PREFERRED_TARGET_TYPES (GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP)

//AI global define
#define FIGHTER_AI_MAX_ENGINE_SPEED 12
#define FIGHTER_AI_TILT_DELAY 2

//AI landing define
#define FIGHTER_AI_RETREAT_HEIGHT 1200
#define FIGHTER_AI_FRAMES_UNTIL_NEXT_UPDATE 1
#define FIGHTER_AI_PREFERRED_ENGINE_ACCELERATION_WHEN_FLYING_HOME (12)
#define FIGHTER_AI_ANGLE_WHEN_DIVING_TOWARDS_LANDING_SURFACE 45
#define FIGHTER_AI_SAFETY_SLOW_DOWN_DISTANCE_FOR_LANDING 500
#define FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_LANDING (140)
#define FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING ((BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING+1))
#define FIGHTER_AI_SLOW_SPEED_ON_LANDING_SURFACE (1)
#define FIGHTER_AI_WAIT_FRAMES 23

//AI bombing define
#define FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET 40
#define FIGHTER_AI_BOMBING_DIVE_HEIGHT 1400
#define FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT 600
#define FIGHTER_AI_BOMBING_HEIGHT 400
#define FIGHTER_AI_BOMBING_DISTANCE 1800
#define FIGHTER_AI_BOMBING_CLOSE_DISTANCE 4000
#define FIGHTER_AI_SHIP_BOMB_SPEED (FIGHTER_AI_MAX_ENGINE_SPEED)
#define FIGHTER_AI_SHIP_BOMB_HEIGHT 190
#define FIGHTER_AI_SHIP_BOMB_LOW_HEIGHT 80
#define FIGHTER_AI_SHIP_BOMB_SHIP_MAX_AWAY_DISTANCE_FOR_SECOND_ATTACK 2500
#define FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT 500
#define FIGHTER_AI_TORPEDO_DROP_HEIGHT 40
#define FIGHTER_AI_SAFE_TORPEDO_DROP_DISTANCE 200
#define FIGHTER_AI_TORPEDO_ATTACK_DISTANCE 500
#define FIGHTER_AI_CHECK_IF_SHIP_IS_BLOCKED_ON_SIDE_EXTRA_DEPTH 100

//AI maneuver define
#define FIGHTER_AI_MAX_DISTANCE_TO_ATTACK_ANOTHER_TARGET (640*2)
#define FIGHTER_AI_IDLE_MANEUVER_ANGLE EManeuverAngle45
#define FIGHTER_AI_BOMBING_MANEUVER_ANGLE EManeuverAngle45
#define FIGHTER_AI_MANEUVER_SMOOTHNESS 2 //how many frames it will wait before doing another tilt action
#define FIGHTER_AI_DIVE_DOWN_MANEUVER_DELAY 3 //how many update cycles it takes for the double dive down of the maneuver

//AI fighting player define
#define FIGHTER_AI_CHASE_DOWN_TILT_DELAY 2
#define FIGHTER_AI_ATTACK_DISTANCE 4000
#define FIGHTER_AI_SAFTEY_HEIGHT 350
#define FIGHTER_AI_VERTICAL_DIVE_SAFTEY_HEIGHT 700
#define FIGHTER_AI_TAILGATE_DISTANCE (3 * DAUNTLESS_WIDTH)
#define FIGHTER_AI_DANGER_FLYING_SPEED 16

//AI avoid big plane define
#define FIGHTER_AI_BIG_PLANE_CRASHED_HEIGHT 100

//controls the ai whether to attack or retreat
enum planeStatus
{
	EPlaneStatus_combat,
	EPlaneStatus_retreat
};

//controls which state the ai is in when it is in combat mode
enum combatStatus
{
	Ecombat_takeOff,
	ECombat_maneuver,
	ECombat_attack,
	ECombat_continue_priority_target,
	ECombat_revenge,
	ECombat_avoid_flying_too_low,
	ECombat_avoid_plane,
	ECombat_avoid_ship,
	ECombat_accurate_bomb,
	ECombat_burst_bomb,
	ECombat_ship_bomb,
	ECombat_dive_to_target,
	ECombat_dive_to_sea_level,
	ECombat_sucicide
};

//controls which state the ai is in when it is in retreat mode
enum retreatStatus
{
	ERetreat_return_to_base,
	ERetreat_maneuver,
	ERetreat_avoid_plane
};

//indicates where the ai is located from the base
enum positionFromBase
{
	EPositionFromBase_left,
	EPositionFromBase_right
};

//responsible for the state in which the ai takes off from a landing surface
enum takeOffStatus
{
	ETakeOffStatus_start_engine,
	ETakeOffStatus_face_target,
	ETakeOffStatus_accelerate_and_tilt_up,
	ETakeOffStatus_climb_to_low_fighting_altitude,
	ETakeOffStatus_climb_to_high_fighting_altitude,
	ETakeOffStatus_adjust_horizontal_angle
};

//responsible for the state in which the ai lands on to a landing surface
enum TLandingStatus
{
	ELandingStatus_Leave_Current_Zone,
	ELandingStatus_Fly_At_Retreat_Height_Above,
	ELandingStatus_Fly_At_Retreat_Height_Below,
	ELandingStatus_Adjust_To_Horizontal_Angle,
	ELandingStatus_Adjust_Not_Upside_Down,
	ELandingStatus_Flying_Towards_Target,
	ELandingStatus_Flying_Towards_Target_JustBefore_Dive,
	ELandingStatus_Diving_Down_To_Landing_Height,
	ELandingStatus_Diving_Up_To_Become_Horizontal_With_Landing_Surface,
	ELandingStatus_Slow_Down_Until_Reaches_Edge_Of_Landing_Surface,
	ELandingStatus_Dive_Down_To_Land_On_Landing_Surface,
	ELandingStatus_Turn_Plane_To_Middle_Of_Landing_Surface,
	ELandingStatus_Move_To_Middle,
	ELandingStatus_Wait,
	ELandingStatus_Stopped
};

/*diagram for sine / cosine curve
    ---             |             ---
   /   \            |            /   \
  /     \           |           /     \
 /       \          |          /       \
          \       / | \       /
           \     /  |  \     /
            \   /   |   \   /
             ---    |    ---
 |--| |------| |--|   |--| |------| |--|
  up    down    up    down    up    down*/
enum maneuverAngle
{
	EManeuverAngle15,
	EManeuverAngle30,
	EManeuverAngle45,
	EManeuverAngle60
};

//general method to use to avoid being hit
enum maneuverCycle
{
	EManeuverCycle_step1_tilt_up,
	EManeuverCycle_step2_tilt_down,
	EManeuverCycle_step3_tilt_up,
	EManeuverCycle_step4_adjust_to_horizontal
};

//used for when the ai is heading for a bombing target
enum flyTowardsTarget
{
	EFlyTowardsTarget_Check_Dive_Down,
	EFlyTowardsTarget_Dive_Down,
	EFlyTowardsTarget_Fly_Horizontally,
};

//used in ship bomb and decides which dive method to use
enum shipDiveMethod
{
	EShipDiveMethod_check_location,
	EShipDiveMethod_dive_method1,
	EShipDiveMethod_dive_method2
};

//used in ship bomb dive down method 1
enum flyTowardsShip
{
	EFlyTowardsShip_Check_Dive_Down,
	EFlyTowardsShip_Dive_Down,
	EFlyTowardsShip_Fly_Horizontally,
	EFlyTowardsShip_decrease_flying_height,
	EFlyTowardsShip_increase_flying_height
};

//used in ship bomb dive down method 2
enum flyTowardsShipMethod2
{
	EFlyTowardsShipMethod2_check_dive_down,
	EFlyTowardsShipMethod2_dive_down,
	EFlyTowardsShipMethod2_fly_horizontally,
	EFlyTowardsShipMethod2_turn_to_target,
	EFlyTowardsShipMethod2_dive_down2,
	EFlyTowardsShipMethod2_fly_horizontally2,
	EFlyTowardsShipMethod2_decrease_flying_height,
	EFlyTowardsShipMethod2_increase_flying_height
};

//controls which state the ai is in when climbing back to the safe bombing height
enum flyToBombingHeightStatus
{
	EFlyToBombingHeightStatus_tilt_up,
	EFlyToBombingHeightStatus_check_height,
	EFlyToBombingHeightStatus_fly_horizontally
};

//1000kg bombing case
enum accurateBombStatus
{
	EAccurateBombStatus_Face_Target,
	EAccurateBombStatus_Initiate_Bombing,
	EAccurateBombStatus_Fly_To_Bombing_Distance
};

//100kg bombing case
enum burstBombStatus
{
	EBurstBombStatus_Face_Target,
	EBurstBombStatus_Initiate_Bombing,
	EBurstBombStatus_Fly_To_Bombing_Distance
};

enum shipAvoidCollision
{
	EShipAvoidCollision_tilt_up,
	EShipAvoidCollision_fly_to_height,
	EShipAvoidCollision_fly_to_end_of_ship,
	EShipAvoidCollision_fly_horizontally,
	EShipAvoidCollision_adjust_to_targeted_ship_height,
	EShipAvoidCollision_check_height,
	EShipAvoidCollision_fly_horizontally2
};

//controls which case the ai is in during torpedo
enum shipBombStatus
{
	EShipBombStatus_face_target,
	EShipBombStatus_initiate_bombing,
	EShipBombStatus_fly_to_bombing_distance
};

//used to torpedo the other side of the targeted ship
enum shipBombBehindTarget
{
	EShipBombBehindTarget_tilt_up,
	EShipBombBehindTarget_tilt_down,
	EShipBombBehindTarget_fly_to_height,
	EShipBombBehindTarget_slow_down_to_torpedo_speed,
	EShipBombBehindTarget_fly_horizontally,
	EShipBombBehindTarget_fly_away_from_target,
	EShipBombBehindTarget_resume_torpedo
};

//used to fly to the torpedo height
enum shipBombHeightAdjust
{
	EShipBombHeightAdjust_fly_to_torpedo_height,
	EShipBombHeightAdjust_fly_horizontal,
	EShipBombHeightAdjust_tilt_up,
	EShipBombHeightAdjust_tilt_down
};

enum bombTargetSwitchingStatus
{
	EBombTargetSwitchingStatus_face_target,
	EBombTargetSwitchingStatus_tilt_towards_height,
	EBombTargetSwitchingStatus_climb_to_fighting_altitude,
	EBombTargetSwitchingStatus_adjust_horizontal,
	EBombTargetSwitchingStatus_adjust_not_up_side_down,
	EBombTargetSwitchingStatus_adjust_horizontal_angle
};

//used to avoid crashing the floor when performing dog fights
enum attackPlaneStatus
{
	EAttackPlaneStatus_adjust_saftey_height,
	EAttackPlaneStatus_attack_player
};

//used to avoid big planes during attack
enum avoidCollisionStatus
{
	EAvoidCollisionStatus_check_angle,
	EAvoidCollisionStatus_tilt_up,
	EAvoidCollisionStatus_tilt_down
};

//used to avoid big planes during retreat
enum retreatAvoidOtherPlaneCollisionStatus
{
	ERetreatAvoidOtherPlaneCollisionStatus_check_angle,
	ERetreatAvoidOtherPlaneCollisionStatus_tilt_up,
	ERetreatAvoidOtherPlaneCollisionStatus_tilt_down
};

enum shotByOtherPlaneStatus
{
	EShotByOtherPlaneStatus_search_closest_plane,
	EShotByOtherPlaneStatus_attack_plane
};

enum continueFromDogFightStatus
{
	EContinueFromDogFightStatus_face_target,
	EContinueFromDogFightStatus_check_height,
	EContinueFromDogFightStatus_climb_up,
	EContinueFromDogFightStatus_climb_down,
	EContinueFromDogFightStatus_short_climb_up,
	EContinueFromDogFightStatus_short_climb_down,
	EContinueFromDogFightStatus_fly_horizontally,
	EContinueFromDogFightStatus_resume_attack
};

// CLASS DECLARATION

/**
 *  CFighterAI
 *
 */
class CFighterAI
{
public:
	~CFighterAI();
	static CFighterAI* New(CAircraft* aFighter);
	//runs the code that deals with the case that the plane got hit by enemy plane's machine gun
	void GotHitByEnemyPlane();
	//Loads the previously saved AI data from disk to replace its current AI data
	void LoadFromDisk(CFileReadStream &aReadStream);
	//Saves all AI data to disk
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	//tells the plane to attack this target next
	void SetTargetToAttack(CGameObject* aTarget);
	void SetTargetAttackType(TUint aTargetPriorities);
	void SetFighterAIToRetreatToBase(); //might be useful if plane is created midair, so it either attacks right away or finds a close by landing surface
	void Update();

private:
	CFighterAI(CAircraft* aFighter);
	//controls the ai's exact bombing behaviour
	void AccurateBombLogic();
	//checks if the current bombing height is too low for safe accurate bombing
	void AccurateBombCheckHeight();
	void AdjustShipBombHeight();
	//changes the speed of the plane to target speed step by step
	void AdjustPlaneSpeed(TIntFloat aTargetSpeed);
	//makes the ai avoid crashing to the ground while chasing down a plane
	TBool AdjustToSafteyHeight();
	//if the ai is within the hitbox it will stop what it is doing and avoid the big plane
	void AvoidTargetHitBox();
	//if the ai is within the hitbox it will stop what it is doing and avoid the big plane
	void AvoidBigPlaneHitBox();
	//if the ai is flying too low it will gain back altitude to continue bombing
	void AvoidFlyingTooLow();
	//avoid collision with other ships in the map
	void AvoidOtherShipHitBox();
	//makes the ai avoid crashing into the big plane
	void AvoidTargetCollision();
	void AvoidGettingHitAgain();
	void AvoidShipCollision();
	//controls the ai's burst bombing behaviour
	void BurstBombLogic();
	//calculates the horizontal distance of the maneuver
	void CalculateManeuverDistanceTravel(maneuverAngle aManeuverAngle);
	//calculates the distance for perfect bombing
	void CalculateNeededDistanceForDrop();
	//checks if the current bombing height is too low for safe burst bombing
	void BurstBombCheckHeight();
	//check which dive method to use when performing a torpedo attack
	void CheckBeforeDiveToSeaLevel();
	//changing the plane status based on its health
	void CheckHealthStatus();
	//this function returns true if the plane is located over sea
	TBool CheckIfFlyingOverSea();
	//this function checks if we are too close to an ship to release a torpedo
	TBool CheckIfSaveToReleaseTorpedo();
	//makes the ai attack another target after taking down a plane
	void ContinueAfterDogFight();
	//makes the ai dive at the sea level so that torepedo dropping is possible
	void DiveToSeaLevel();
	//makes the ai dive at the sea level so that torepedo dropping is possible method 2
	void DiveToSeaLevelWithTurnInBetween();
	//makes the ai dive to a bombing height so that it would drop bombs at a certain height
	void DiveToTarget();
	//returns true when the enemy is behind the ai during dog fights
	void EnemyTailGating();
	//search for a non destroyed base for repair or reloading
	void FindSuitableBaseForLanding();
	//flips the plane 180 degrees
	void FlipPlane();
	//adjust the plane to fly at a certain angle
	void FlyAtAngle(TInt aAngle, TInt frameDelay);
	//returns true if the target ship is blocked on the left
	TBool GetIfShipIsBlockedOnLeft(CShip* aTargetShip);
	//returns true if the target ship is blocked on the right
	TBool GetIfShipIsBlockedOnRight(CShip* aTargetShip);
	//This functions will return how much distance the plane will travel in the X direction when changing the height of the plane from a higher Y to a lower Y at a certain angle
	TIntFloat GetXDistanceTravelledWhenChangingHeightDown(TIntFloat aHeightDifference, TInt aDiveAngle);
	//makes the ai avoid being shot down easily
	void Maneuver(TInt aLeftUp, TInt aLeftDown, TInt aRightUp, TInt aRightDown, TInt aTiltDelay);
	//controls what angle the ai should maneuver at
	void ManeuverAngles();
	//turn all the switch cases of the ai to the first case
	void ResetAIBehaviour();
	//used in the retreating state of the ai
	void RetreatAvoidBigPlaneHitBox();
	//used in the retreating state of the ai
	void RetreatAvoidOtherPlaneCollision();
	//this function has all the AI code that deals with the landing of the aircraft
	void RunLogicCodeForLanding();
	//this function avoids the ai from doing maneuvers when it is flying close to the sea surface
	void ManeuverCheckHeight();
	//controls the ai's torepedo bombing
	void ShipBombLogic();
	//controls the ai to shoot the plane down using the shortest path to reach it
	void ShortestPathShootDown();
	//adjust the ai to a combat height when it takes off
	void TakeOffFromAirport();
	//limits the ai to attack a specific target type
	void TargetSpecificTarget(TUint aTargetTypes);

	TInt iAvoidCollisionTiltDelay; //used for delaying the ai from tilting away from hitting a big plane
	TInt iFrameCount; //used to limit the updating of the plane tilting
	TInt iSimulateReplinish; //used to stop the plane on the landing surface for some time
	TInt iCurrentAltitute; //used for checking if the ai is at a sutible retreat height
	TInt iDistanceToBase; //used for measuring the distance away from the base
	TInt iDistanceToTarget; //used for measuring the distance away from target
	TIntFloat iBaseLandingHeight;
	TInt iFramesNeededToHitGround;
	TInt iNeededDistanceForDrop;
	TInt iExtraWidthOffset;
	TInt iMiddlePositionOfBombsOffset;
	TInt iManeuverDelay; //used for extending the flying down process in the maneuver process
	TInt iManeuverSmoothness; //used for extending the time before another tilt up or down is performed. 2 or greater for some delay
	TInt iManeuverFrames;
	TInt iManeuverDistanceTravelled; //used for calculating the distance being travelled when doing one maneuver cycle
	TInt iChaseDownTiltDelay; //used for slowing down the ai's machine gun shooting rate
	TInt iShipBombLowHeight; //used for marking if a target is too close to do a full height travel
	TUint iTargetPriority;
	TBool iReturnToBase; //activates the plane return to base function
	planeStatus iPlaneStatus;
	combatStatus iCombatStatus;
	combatStatus iPreviousCombatStatus; //used for switching back to previous state when plane is flying straight in other states
	positionFromBase iPositionFromBase;
	TLandingStatus iLandingStatus;
	takeOffStatus iTakeOffStatus;
	retreatStatus iRetreatStatus;
	retreatStatus iPreviousRetreatStatus; //used for switching back to previous state when plane is intrrupted by a big plane near it
	maneuverCycle iManeuverCycle;
	maneuverAngle iManeuverAngle;
	flyToBombingHeightStatus iFlyToBombingHeightStatus;
	flyTowardsTarget iFlyTowardsTarget;
	accurateBombStatus iAccurateBombStatus;
	burstBombStatus iBurstBombStatus;
	shipDiveMethod iShipDiveMethod;
	flyTowardsShip iFlyTowardsShip;
	flyTowardsShipMethod2 iFlyTowardsShipMethod2;
	shipBombStatus iShipBombStatus;
	shipBombBehindTarget iShipBombBehindTarget;
	shipBombHeightAdjust iShipBombHeightAdjust;
	shipAvoidCollision iShipAvoidCollision;
	attackPlaneStatus iAttackPlaneStatus;
	avoidCollisionStatus iAvoidCollisionStatus;
	continueFromDogFightStatus iContinueFromDogFightStatus;
	retreatAvoidOtherPlaneCollisionStatus iRetreatAvoidOtherPlaneCollisionStatus;
	shotByOtherPlaneStatus iShotByOtherPlaneStatus;
	bombTargetSwitchingStatus iBombTargetSwitchingStatus;
	CGameObject* iTarget; //used for marking the bombing target
	CGameObject* iBigPlane; //used for marking big planes on the map
	CGameObject* iShip; //used for marking other ships that could collide with the ai
	CAircraft* iFighter;
	CGameObject* iLandingSurface;//points to the landing surface that we are trying to land on
};

#endif /* CFIGHTERAI_H_ */
