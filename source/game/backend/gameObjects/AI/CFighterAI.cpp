/*
 ============================================================================
 Name		: FighterAI.cpp
 Author	  : stephen cheung
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFighterAI implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CWeaponBomb.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CFighterAI::CFighterAI(CAircraft* aFighter)
{
	iAvoidCollisionTiltDelay = 0;
	iBaseLandingHeight = TIntFloat::Convert(90);
	iChaseDownTiltDelay = 0;
	iDistanceToTarget = 0;
	iCurrentAltitute = 0;
	iDistanceToBase = 0;
	iExtraWidthOffset = 0;
	iNeededDistanceForDrop = 0;
	iFrameCount = 0;
	iFramesNeededToHitGround = 0;
	iLandingSurface = NULL;
	iMiddlePositionOfBombsOffset = 0;
	iManeuverDelay = 0;
	iManeuverSmoothness = 0;
	iSimulateReplinish = 0;
	iManeuverFrames = 0;
	iManeuverDistanceTravelled = 0;
	iReturnToBase = false;
	iShipBombLowHeight = 0;
	iTargetPriority = FIGHTER_AI_ALL_TARGET_TYPES;
	iTarget = NULL;
	iBigPlane = NULL;
	iShip = NULL;
	iLandingSurface = NULL;
	iFighter = aFighter;

	iPositionFromBase = EPositionFromBase_left;
	iCombatStatus = Ecombat_takeOff;
	iLandingStatus = ELandingStatus_Leave_Current_Zone;
	iManeuverCycle = EManeuverCycle_step1_tilt_up;
	iManeuverAngle = EManeuverAngle15;
	iPlaneStatus = EPlaneStatus_combat;
	iPreviousCombatStatus = ECombat_maneuver;
	iPreviousRetreatStatus = ERetreat_maneuver;
	iRetreatStatus = ERetreat_return_to_base;
	iTakeOffStatus = ETakeOffStatus_start_engine;

	iFlyTowardsTarget = EFlyTowardsTarget_Check_Dive_Down;
	iAccurateBombStatus = EAccurateBombStatus_Face_Target;
	iBurstBombStatus = EBurstBombStatus_Face_Target;

	iShipDiveMethod = EShipDiveMethod_check_location;
	iFlyTowardsShip = EFlyTowardsShip_Check_Dive_Down;
	iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_check_dive_down;
	iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_tilt_up;
	iShipAvoidCollision = EShipAvoidCollision_tilt_up;
	iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
	iShipBombHeightAdjust = EShipBombHeightAdjust_fly_to_torpedo_height;
	iShipBombStatus = EShipBombStatus_face_target;
	iAttackPlaneStatus = EAttackPlaneStatus_adjust_saftey_height;
	iContinueFromDogFightStatus = EContinueFromDogFightStatus_face_target;
	iAvoidCollisionStatus = EAvoidCollisionStatus_check_angle;
	iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_check_angle;
	iShotByOtherPlaneStatus = EShotByOtherPlaneStatus_search_closest_plane;
	iBombTargetSwitchingStatus = EBombTargetSwitchingStatus_face_target;
}

CFighterAI* CFighterAI::New(CAircraft* aFighter)
{
	CFighterAI* self = new CFighterAI(aFighter);
	return self;
}

CFighterAI::~CFighterAI()
{
}

void CFighterAI::AdjustPlaneSpeed(TIntFloat aTargetSpeed)
{
	if(iFighter->GetCurrentEngineAccelerationRef() < aTargetSpeed - TIntFloat::Convert(1))
	{
		iFighter->IncreaseSpeed();
	}else if(iFighter->GetCurrentEngineAccelerationRef() > aTargetSpeed)
	{
		iFighter->DecreaseSpeed();
	}
}

void CFighterAI::AvoidGettingHitAgain()
{
	//vertical dive then head back to where it is going
	//360 loop heading up/down
	//random dive down and up
}

void CFighterAI::CheckHealthStatus()
{
	if(iFighter->GetHealth() <= iFighter->GetMaxHealth() * 40 / 100 && iLandingSurface)
	{
		iPlaneStatus = EPlaneStatus_retreat;
		ResetAIBehaviour();
	}else
	{
		iPlaneStatus = EPlaneStatus_combat;
	}
}

void CFighterAI::FindSuitableBaseForLanding()
{
	//first find a suitable, closest, undestroyed landing surface
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesLandingSurface);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
	CGameObject* lCurrentBestTarget = NULL;

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lFighterConflictSide == lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive())
		{
			//find distance to target
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
			TPointIntFloat lFighterLocation = iFighter->GetCurrentPosition();
			TInt lDistanceBetweenTargetLocation = CMath::Abs(lFighterLocation.iX - lTargetLocation.iX).GetIntInBaseInt();

			if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
			{
				lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
				lCurrentBestTarget = lCurrentGameObject;
			}
		}
	}

	//now have the closest landing surface
	iLandingSurface = lCurrentBestTarget;
	//clean up
	delete lGameObjects;

	//in theory might not find a landing surface
	if(!iLandingSurface)
		return; //try to do something if no landing surface exists anymore

	//orient the plane to be in proper position to land on the landingSurface
	TIntFloat lPlanePosition = iFighter->GetCurrentPosition().iX;
	TIntFloat lBasePosition = iLandingSurface->GetCurrentPosition().iX;
	//travel to the base direction
	if(lPlanePosition >= lBasePosition)
	{
		iPositionFromBase = EPositionFromBase_right;
		//iFighter->TurnLeft();
	}else
	{
		iPositionFromBase = EPositionFromBase_left;
	}
}

void CFighterAI::FlipPlane()
{
	//the minimum speed needed to do a 180 gaining altitude route is 9
	//there are problems with this because the speed will decrease when the plane tilts up
	//always do a flip by diving down because you gain speed when you dive down and you reach your destination faster
	//upside down not reflected case
	if(!iFighter->IsReflected() && *iFighter->GetFlyingAngel() == 0)
	{
		iFighter->TurnRight();
		//		iFighter->IncreaseFlyingAngle();
	}else if(iFighter->IsReflected() && *iFighter->GetFlyingAngel() > 180 && *iFighter->GetFlyingAngel() <= 360)
	{
		iFighter->IncreaseFlyingAngle();
	}
	//upside down reflected case
	if(iFighter->IsReflected() && *iFighter->GetFlyingAngel() == 180)
	{
		iFighter->TurnLeft();
		//		iFighter->IncreaseFlyingAngle();
	}else if(!iFighter->IsReflected() && *iFighter->GetFlyingAngel() > 180 && *iFighter->GetFlyingAngel() <= 360)
	{
		iFighter->IncreaseFlyingAngle();
	}
}

//does not matter if ai is upside down or not
void CFighterAI::FlyAtAngle(TInt aAngle, TInt frameDelay)
{
	TInt lCurrentFighterAngle = *iFighter->GetFlyingAngel();
	if(aAngle == lCurrentFighterAngle)
		return;

	TInt lAngleHemisphere;
	if(aAngle + 180 < 360)
	{
		lAngleHemisphere = aAngle + 180;
	}else if(aAngle + 180 >= 360)
	{
		lAngleHemisphere = aAngle - 180;
	}
	if(frameDelay > 0)
	{
		iManeuverSmoothness++;
	}
	//facing left when flying horizontally not upside down
	if(!iFighter->IsReflected())
	{
		if(lAngleHemisphere > aAngle && frameDelay == iManeuverSmoothness)
		{
			if(lCurrentFighterAngle >= aAngle && lCurrentFighterAngle < lAngleHemisphere)
			{
				iFighter->IncreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
			//the annoying case where it has the 0/360 angle
			else
			{
				iFighter->DecreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
		}else if(lAngleHemisphere < aAngle && frameDelay == iManeuverSmoothness)
		{
			if(lCurrentFighterAngle >= lAngleHemisphere && lCurrentFighterAngle < aAngle)
			{
				iFighter->DecreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
			//the annoying case where it has the 0/360 angle
			else
			{
				iFighter->IncreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
		}
	}else if(iFighter->IsReflected())
	{
		if(lAngleHemisphere > aAngle && frameDelay == iManeuverSmoothness)
		{
			if(lCurrentFighterAngle >= aAngle && lCurrentFighterAngle < lAngleHemisphere)
			{
				iFighter->DecreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
			//the annoying case where it has the 0/360 angle
			else
			{
				iFighter->IncreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
		}else if(lAngleHemisphere < aAngle && frameDelay == iManeuverSmoothness)
		{
			if(lCurrentFighterAngle >= lAngleHemisphere && lCurrentFighterAngle < aAngle)
			{
				iFighter->IncreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
			//the annoying case where it has the 0/360 angle
			else
			{
				iFighter->DecreaseFlyingAngle();
				iManeuverSmoothness = 0;
			}
		}
	}
}

TIntFloat CFighterAI::GetXDistanceTravelledWhenChangingHeightDown(TIntFloat aHeightDifference, TInt aDiveAngle)
{
	//calculate extra speed that will be added because the plane is flying down
	TIntFloat lYExtraAngleSpeed = (CMath::GraphicsSinTable(aDiveAngle) * CMath::GraphicsSinTable(aDiveAngle)) * BASIC_AIRCAFT_ACCELERATION_CHANGE_DUE_TO_ANGEL;
	//constant acceleration that will be added
	TIntFloat lYDiveAcceleration = ((CMath::GraphicsSinTable(aDiveAngle) * CMath::GraphicsSinTable(aDiveAngle))) * BASIC_AIRCAFT_EXTRA_DOWN_SPEED;//the acceleration gained every frame when starting diving on the Y vector
	TIntFloat lYInitialVelocity = iFighter->GetTotalAcceleration() * CMath::GraphicsSinTable(aDiveAngle);//the initial acceleration speed of the plane before the dive starts on the Y vector

	//Newton formula
	//Vf^2 = Vi^2 + 2AD
	//Vf = velocity final, Vi = velocity initial, A = total acceleration, D = distance travelled
	//find final velocity, used to find time
	TIntFloat lYInitialVelocitySquared = lYInitialVelocity * lYInitialVelocity;
	TIntFloat lAccTimesDistance = (lYDiveAcceleration * aHeightDifference) * 2;
	TDouble lTemp = (lYInitialVelocitySquared + lAccTimesDistance).GetIntInBaseInt();
	lTemp = CMath::Sqrt(lTemp);
	TIntFloat lYFinalVelocity = TIntFloat::Convert(lTemp);
	lYFinalVelocity = lYFinalVelocity + lYExtraAngleSpeed;

	//Newton formula
	//Vf = Vi + AT
	//Vf = velocity final, Vi = velocity initial, A = total acceleration, T = time
	//find time
	TInt lFramesNeededForDive = ((lYFinalVelocity - lYInitialVelocity) / lYDiveAcceleration).GetIntInBaseInt();
	//need to add extra frames for turning the plane up and down
	TInt lFramesNeedToChangeAngle = (aDiveAngle / BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE) * FIGHTER_AI_FRAMES_UNTIL_NEXT_UPDATE * 2;
	lFramesNeededForDive += lFramesNeedToChangeAngle;

	//Newton forumla
	//Dx = 0.5(Vxi+Vxf)T
	//Dx = distance travelled on X axis, Vxi = inital X Velocity, Vxf = final X velocity, T = time
	TIntFloat lXFinalPlusInitialVelocity = (lYFinalVelocity + lYInitialVelocity) * CMath::GraphicsCosTable(aDiveAngle);
	TIntFloat lXDistanceNeededForDive = (lXFinalPlusInitialVelocity * (lFramesNeededForDive / 2));
	return lXDistanceNeededForDive;
}

void CFighterAI::LoadFromDisk(CFileReadStream &aReadStream)
{
	iAvoidCollisionTiltDelay = aReadStream.ReadInt32();
	iFrameCount = aReadStream.ReadInt32();
	iSimulateReplinish = aReadStream.ReadInt32();
	iReturnToBase = aReadStream.ReadInt32();
	iCurrentAltitute = aReadStream.ReadInt32();
	iDistanceToBase = aReadStream.ReadInt32();
	iDistanceToTarget = aReadStream.ReadInt32();
	iBaseLandingHeight = aReadStream.ReadIntFloat();
	iFramesNeededToHitGround = aReadStream.ReadInt32();
	iNeededDistanceForDrop = aReadStream.ReadInt32();
	iExtraWidthOffset = aReadStream.ReadInt32();
	iMiddlePositionOfBombsOffset = aReadStream.ReadInt32();
	iManeuverDelay = aReadStream.ReadInt32();
	iManeuverSmoothness = aReadStream.ReadInt32();
	iManeuverFrames = aReadStream.ReadInt32();
	iManeuverDistanceTravelled = aReadStream.ReadInt32();
	iChaseDownTiltDelay = aReadStream.ReadInt32();
	iShipBombLowHeight = aReadStream.ReadInt32();
	iTargetPriority = aReadStream.ReadInt32();
	//states
	iPlaneStatus = static_cast<planeStatus> (aReadStream.ReadInt32());
	iCombatStatus = static_cast<combatStatus> (aReadStream.ReadInt32());
	iPreviousCombatStatus = static_cast<combatStatus> (aReadStream.ReadInt32());
	iPositionFromBase = static_cast<positionFromBase> (aReadStream.ReadInt32());
	iLandingStatus = static_cast<TLandingStatus> (aReadStream.ReadInt32());
	iTakeOffStatus = static_cast<takeOffStatus> (aReadStream.ReadInt32());
	iRetreatStatus = static_cast<retreatStatus> (aReadStream.ReadInt32());
	iPreviousRetreatStatus = static_cast<retreatStatus> (aReadStream.ReadInt32());
	iManeuverCycle = static_cast<maneuverCycle> (aReadStream.ReadInt32());
	iManeuverAngle = static_cast<maneuverAngle> (aReadStream.ReadInt32());
	iFlyToBombingHeightStatus = static_cast<flyToBombingHeightStatus> (aReadStream.ReadInt32());
	iFlyTowardsTarget = static_cast<flyTowardsTarget> (aReadStream.ReadInt32());
	iAccurateBombStatus = static_cast<accurateBombStatus> (aReadStream.ReadInt32());
	iBurstBombStatus = static_cast<burstBombStatus> (aReadStream.ReadInt32());
	iFlyTowardsShip = static_cast<flyTowardsShip> (aReadStream.ReadInt32());
	iShipBombStatus = static_cast<shipBombStatus> (aReadStream.ReadInt32());
	iShipBombBehindTarget = static_cast<shipBombBehindTarget> (aReadStream.ReadInt32());
	iShipBombHeightAdjust = static_cast<shipBombHeightAdjust> (aReadStream.ReadInt32());
	iShipAvoidCollision = static_cast<shipAvoidCollision> (aReadStream.ReadInt32());
	iAttackPlaneStatus = static_cast<attackPlaneStatus> (aReadStream.ReadInt32());
	iAvoidCollisionStatus = static_cast<avoidCollisionStatus> (aReadStream.ReadInt32());
	iContinueFromDogFightStatus = static_cast<continueFromDogFightStatus> (aReadStream.ReadInt32());
	iRetreatAvoidOtherPlaneCollisionStatus = static_cast<retreatAvoidOtherPlaneCollisionStatus> (aReadStream.ReadInt32());
	iShotByOtherPlaneStatus = static_cast<shotByOtherPlaneStatus> (aReadStream.ReadInt32());
	iBombTargetSwitchingStatus = static_cast<bombTargetSwitchingStatus> (aReadStream.ReadInt32());
	//Reset Pointer
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iTarget));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iBigPlane));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iShip));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iLandingSurface));
	//no need to save Fighter, since Fighter will create this object and set the pointer automatically
}

void CFighterAI::Maneuver(TInt aLeftUp, TInt aLeftDown, TInt aRightUp, TInt aRightDown, TInt aTiltDelay)
{
	//do maneuvers when the plane is flying horizontally for a long period of time
	if(!iFighter->IsReflected())
	{
		switch(iManeuverCycle)
		{
			case EManeuverCycle_step1_tilt_up:
			{
				FlyAtAngle(aRightUp, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == aRightUp)
					iManeuverCycle = EManeuverCycle_step2_tilt_down;
				break;
			}
			case EManeuverCycle_step2_tilt_down:
			{
				FlyAtAngle(aRightDown, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == aRightDown)
					iManeuverDelay++;
				if(iManeuverDelay == FIGHTER_AI_DIVE_DOWN_MANEUVER_DELAY)
					iManeuverCycle = EManeuverCycle_step3_tilt_up;
				break;
			}
			case EManeuverCycle_step3_tilt_up:
			{
				iManeuverDelay = 0;
				FlyAtAngle(aRightUp, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == aRightUp)
					iManeuverCycle = EManeuverCycle_step4_adjust_to_horizontal;
				break;
			}
			case EManeuverCycle_step4_adjust_to_horizontal:
			{
				FlyAtAngle(180, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == 180)
				{
					iManeuverCycle = EManeuverCycle_step1_tilt_up;
					iCombatStatus = iPreviousCombatStatus;
					iRetreatStatus = ERetreat_return_to_base;
				}
				break;
			}
		}
	}else if(iFighter->IsReflected())
	{
		switch(iManeuverCycle)
		{
			case EManeuverCycle_step1_tilt_up:
			{
				FlyAtAngle(aLeftUp, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == aLeftUp)
					iManeuverCycle = EManeuverCycle_step2_tilt_down;
				break;
			}
			case EManeuverCycle_step2_tilt_down:
			{
				FlyAtAngle(aLeftDown, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == aLeftDown)
					iManeuverDelay++;
				if(iManeuverDelay == FIGHTER_AI_DIVE_DOWN_MANEUVER_DELAY)
					iManeuverCycle = EManeuverCycle_step3_tilt_up;
				break;
			}
			case EManeuverCycle_step3_tilt_up:
			{
				iManeuverDelay = 0;
				FlyAtAngle(aLeftUp, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == aLeftUp)
					iManeuverCycle = EManeuverCycle_step4_adjust_to_horizontal;
				break;
			}
			case EManeuverCycle_step4_adjust_to_horizontal:
			{
				FlyAtAngle(0, aTiltDelay);
				if(*iFighter->GetFlyingAngel() == 0)
				{
					iManeuverCycle = EManeuverCycle_step1_tilt_up;
					iCombatStatus = iPreviousCombatStatus;
					iRetreatStatus = ERetreat_return_to_base;
				}
				break;
			}
		}
	}
}

void CFighterAI::ManeuverAngles()
{
	if(iFighter)
	{
		if((iFighter->GetCurrentEngineAcceleration()).GetIntInBaseInt() != FIGHTER_AI_MAX_ENGINE_SPEED)
		{
			iFighter->IncreaseSpeed();
		}
		switch(iManeuverAngle)
		{
			case EManeuverAngle15:
				Maneuver(15, 345, 165, 195, FIGHTER_AI_MANEUVER_SMOOTHNESS);
				break;
			case EManeuverAngle30:
				Maneuver(30, 330, 150, 210, FIGHTER_AI_MANEUVER_SMOOTHNESS);
				break;
			case EManeuverAngle45:
				Maneuver(45, 315, 135, 225, FIGHTER_AI_MANEUVER_SMOOTHNESS);
				break;
			case EManeuverAngle60:
				Maneuver(60, 300, 120, 240, FIGHTER_AI_MANEUVER_SMOOTHNESS);
				break;
		}
	}
}

void CFighterAI::ResetAIBehaviour()
{
	iAccurateBombStatus = EAccurateBombStatus_Face_Target;
	iAttackPlaneStatus = EAttackPlaneStatus_adjust_saftey_height;
	iBombTargetSwitchingStatus = EBombTargetSwitchingStatus_face_target;
	iBurstBombStatus = EBurstBombStatus_Face_Target;
	iContinueFromDogFightStatus = EContinueFromDogFightStatus_face_target;
	iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_tilt_up;
	iFlyTowardsShip = EFlyTowardsShip_Check_Dive_Down;
	iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_check_dive_down;
	iFlyTowardsTarget = EFlyTowardsTarget_Check_Dive_Down;
	iManeuverCycle = EManeuverCycle_step1_tilt_up;
	iManeuverAngle = EManeuverAngle15;
	iRetreatStatus = ERetreat_return_to_base;
	iShipBombStatus = EShipBombStatus_face_target;
	iShipBombHeightAdjust = EShipBombHeightAdjust_fly_to_torpedo_height;
	iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
	iShipDiveMethod = EShipDiveMethod_check_location;
	iShotByOtherPlaneStatus = EShotByOtherPlaneStatus_search_closest_plane;
	iTakeOffStatus = ETakeOffStatus_start_engine;
}

void CFighterAI::RunLogicCodeForLanding()
{
	if(!iLandingSurface || !iLandingSurface->IsAlive())
	{
		FindSuitableBaseForLanding();
		if(iLandingSurface)
		{
			iLandingStatus = ELandingStatus_Leave_Current_Zone;
		}
		else if(!iLandingSurface)
		{
			if(iFighter->GetSecondaryWeapon()->GetAmmunitionLeft() > 0)
			{
				if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifier1000kgBomb)
				{
					TargetSpecificTarget(GAMEOBJECT_TYPE_GROUP_BUILDINGS);
				}
				else if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifier100kgBomb)
				{
					TargetSpecificTarget(GAMEOBJECT_TYPE_GROUP_TROOPS);
				}
				else if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifierTorpedo)
				{
					TargetSpecificTarget(GAMEOBJECT_TYPE_GROUP_SHIP);
				}
			}
			else if(iFighter->GetSecondaryWeapon()->GetAmmunitionLeft() == 0)
			{
				TargetSpecificTarget(GAMEOBJECT_TYPE_GROUP_PLANES);
			}
			//check if we found a valid target
			if(iTarget)
			{
				CalculateNeededDistanceForDrop();
				iPlaneStatus = EPlaneStatus_combat;
				if(iTarget->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane))
				{
					iCombatStatus = ECombat_attack;
				}
				else if(iTarget->GetGameObjectType() & (GAMEOBJECT_TYPE_GROUP_SHIP))
				{
					iCombatStatus = ECombat_dive_to_sea_level;
				}
				else
				{
					iCombatStatus = ECombat_dive_to_target;
				}
			}
			else if(!iTarget)
			{
				iPlaneStatus = EPlaneStatus_combat;
				iCombatStatus = ECombat_maneuver;
				iPreviousCombatStatus = ECombat_attack;
				ManeuverCheckHeight();
			}
			return;
		}
	}

	switch(iLandingStatus)
	{
		case ELandingStatus_Leave_Current_Zone:
		{
			if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() < FIGHTER_AI_MAX_ENGINE_SPEED)
			{
				iFighter->IncreaseSpeed();
			}
			if(iPositionFromBase == EPositionFromBase_left)
			{
				if(iFighter->GetCurrentPositionNormilized().iY == FIGHTER_AI_RETREAT_HEIGHT)
				{
					if(*iFighter->GetFlyingAngel() == 0 || *iFighter->GetFlyingAngel() == 180)
					{
						iLandingStatus = ELandingStatus_Fly_At_Retreat_Height_Above;
					}
				}else if(iFighter->GetCurrentPositionNormilized().iY < FIGHTER_AI_RETREAT_HEIGHT)
				{
					if(*iFighter->GetFlyingAngel() == 45)
					{
						iLandingStatus = ELandingStatus_Fly_At_Retreat_Height_Above;
					}
					FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
				}else if(iFighter->GetCurrentPositionNormilized().iY > FIGHTER_AI_RETREAT_HEIGHT)
				{
					if(*iFighter->GetFlyingAngel() == 315)
					{
						iLandingStatus = ELandingStatus_Fly_At_Retreat_Height_Below;
					}
					if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() < FIGHTER_AI_MAX_ENGINE_SPEED)
					{
						iFighter->IncreaseSpeed();
					}
					FlyAtAngle(315, FIGHTER_AI_TILT_DELAY);
				}
			}else if(iPositionFromBase == EPositionFromBase_right)
			{
				if(iFighter->GetCurrentPositionNormilized().iY == FIGHTER_AI_RETREAT_HEIGHT)
				{
					if(*iFighter->GetFlyingAngel() == 0 || *iFighter->GetFlyingAngel() == 180)
					{
						iLandingStatus = ELandingStatus_Fly_At_Retreat_Height_Above;
					}
				}else if(iFighter->GetCurrentPositionNormilized().iY < FIGHTER_AI_RETREAT_HEIGHT)
				{
					if(*iFighter->GetFlyingAngel() == 135)
					{
						iLandingStatus = ELandingStatus_Fly_At_Retreat_Height_Above;
					}
					if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() < FIGHTER_AI_MAX_ENGINE_SPEED)
					{
						iFighter->IncreaseSpeed();
					}
					FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
				}else if(iFighter->GetCurrentPositionNormilized().iY > FIGHTER_AI_RETREAT_HEIGHT)
				{
					if(*iFighter->GetFlyingAngel() == 225)
					{
						iLandingStatus = ELandingStatus_Fly_At_Retreat_Height_Below;
					}
					if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() < FIGHTER_AI_MAX_ENGINE_SPEED)
					{
						iFighter->IncreaseSpeed();
					}
					FlyAtAngle(225, FIGHTER_AI_TILT_DELAY);
				}
			}
			break;
		}
		case ELandingStatus_Fly_At_Retreat_Height_Above:
		{
			iCurrentAltitute = iFighter->GetCurrentPositionNormilized().iY;
			if(iCurrentAltitute >= FIGHTER_AI_RETREAT_HEIGHT)
			{
				iLandingStatus = ELandingStatus_Adjust_To_Horizontal_Angle;
			}
			break;
		}
		case ELandingStatus_Fly_At_Retreat_Height_Below:
		{
			iCurrentAltitute = iFighter->GetCurrentPositionNormilized().iY;
			if(iCurrentAltitute <= FIGHTER_AI_RETREAT_HEIGHT)
			{
				iLandingStatus = ELandingStatus_Adjust_To_Horizontal_Angle;
			}
			break;
		}
		case ELandingStatus_Adjust_To_Horizontal_Angle:
		{
			if(iPositionFromBase == EPositionFromBase_left)
			{
				FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
			}else if(iPositionFromBase == EPositionFromBase_right)
			{
				FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
			}
			//switch states once we have the flying angle correct
			if(*iFighter->GetFlyingAngel() == 0 || *iFighter->GetFlyingAngel() == 180)
			{
				iLandingStatus = ELandingStatus_Adjust_Not_Upside_Down;
			}
			break;
		}
		case ELandingStatus_Adjust_Not_Upside_Down:
		{
			FlipPlane();
			if(*iFighter->GetFlyingAngel() == 0 || *iFighter->GetFlyingAngel() == 180)
			{
				iLandingStatus = ELandingStatus_Flying_Towards_Target;
			}
			break;
		}
			//all we need to do is to check if we are with the target range to start diving the plane
		case ELandingStatus_Flying_Towards_Target:
		{
			//otherwise we could speed up the plane to reach our destination, fly to a certain target secure height and peridoically update at what distance we need to start the dive
			AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_PREFERRED_ENGINE_ACCELERATION_WHEN_FLYING_HOME));
			//check what X distance we need to be away to start the dive
			iBaseLandingHeight = iLandingSurface->GetCurrentPosition().iY + iLandingSurface->GetHeight();
			TIntFloat lHeightDifference = iFighter->GetCurrentPosition().iY - iBaseLandingHeight;
			TInt lXDistanceBeforeStartingLanding = GetXDistanceTravelledWhenChangingHeightDown(lHeightDifference, FIGHTER_AI_ANGLE_WHEN_DIVING_TOWARDS_LANDING_SURFACE).GetIntInBaseInt();
			lXDistanceBeforeStartingLanding += (FIGHTER_AI_SAFETY_SLOW_DOWN_DISTANCE_FOR_LANDING * 4);
			//find current distance to base
			iDistanceToBase = iFighter->DistanceBetweenGameObjects(iLandingSurface).GetIntInBaseInt();
			//check if we are close enough to dive down to landing height
			if(lXDistanceBeforeStartingLanding > iDistanceToBase)
			{
				iLandingStatus = ELandingStatus_Flying_Towards_Target_JustBefore_Dive;
			}else
			{
				iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
				iRetreatStatus = ERetreat_maneuver;
			}
			break;
		}
		case ELandingStatus_Flying_Towards_Target_JustBefore_Dive:
		{
			//otherwise we could speed up the plane to reach our destination, fly to a certain target secure height and peridoically update at what distance we need to start the dive
			AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING));
			//check what X distance we need to be away to start the dive
			iBaseLandingHeight = iLandingSurface->GetCurrentPosition().iY + iLandingSurface->GetHeight();
			TIntFloat lHeightDifference = iFighter->GetCurrentPosition().iY - iBaseLandingHeight;
			TInt lXDistanceBeforeStartingLanding = GetXDistanceTravelledWhenChangingHeightDown(lHeightDifference, FIGHTER_AI_ANGLE_WHEN_DIVING_TOWARDS_LANDING_SURFACE).GetIntInBaseInt();
			lXDistanceBeforeStartingLanding += FIGHTER_AI_SAFETY_SLOW_DOWN_DISTANCE_FOR_LANDING;
			//find current distance to base
			iDistanceToBase = iFighter->DistanceBetweenGameObjects(iLandingSurface).GetIntInBaseInt();
			//check if we are close enough to dive down to landing height
			if(lXDistanceBeforeStartingLanding > iDistanceToBase)
			{
				//dive down towards the base instead of away
				if(iFighter->GetCurrentPosition().iX <= iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth() / 2)
				{
					iFighter->TurnRight();
				}else if(iFighter->GetCurrentPosition().iX > iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth() / 2)
				{
					iFighter->TurnLeft();
				}
				iLandingStatus = ELandingStatus_Diving_Down_To_Landing_Height;
			}else
			{
				iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
				iRetreatStatus = ERetreat_maneuver;
			}
			break;
		}
			//need to change the angle to right diving angle, and slow down the plane to a certain speed
		case ELandingStatus_Diving_Down_To_Landing_Height:
		{
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 315)
				{
					FlyAtAngle(315, FIGHTER_AI_TILT_DELAY);
				}
			}else
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 225)
				{
					FlyAtAngle(225, FIGHTER_AI_TILT_DELAY);
				}
			}

			//slow down the plane to our target speed
			AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING));

			//check if we are already at the target height
			if(iBaseLandingHeight + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_LANDING > iFighter->GetCurrentPosition().iY)
			{
				iLandingStatus = ELandingStatus_Diving_Up_To_Become_Horizontal_With_Landing_Surface;
			}
			break;
		}
			//just need to turn the plane up until it is horizontal
		case ELandingStatus_Diving_Up_To_Become_Horizontal_With_Landing_Surface:
		{
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 0)
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				else
					iLandingStatus = ELandingStatus_Slow_Down_Until_Reaches_Edge_Of_Landing_Surface; // are now in proper position to start landing
			}else
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 180)
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				else
					iLandingStatus = ELandingStatus_Slow_Down_Until_Reaches_Edge_Of_Landing_Surface;
			}

			//slow down the plane to our target speed
			AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING));
			break;
		}

			//keeps the plane horizontal and slows down the plane until it reaches the dedicated speed for landing
		case ELandingStatus_Slow_Down_Until_Reaches_Edge_Of_Landing_Surface:
		{
			//hover above the landing surface
			if(iFighter->GetCurrentPosition().iX < iLandingSurface->GetCurrentPosition().iX)
			{
				iFighter->TurnRight();
			}else if(iFighter->GetCurrentPosition().iX > iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth())
			{
				iFighter->TurnLeft();
			}
			//only slow down when the plane is above the landing surface
			if(iFighter->GetCurrentPosition().iX >= iLandingSurface->GetCurrentPosition().iX && iFighter->GetCurrentPosition().iX <= iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth())
			{
				//keep adjusting the speed
				AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING));

				if(iFighter->GetCurrentAbsoluteSpeed() <= TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING))
				{
					//now the plane is slow enough to land, check if we are over the landing surface
					if(iFighter->IsReflected())
					{
						//pointing to the right
						//landing surface diagram < 1 | 2 | 3 | 4 >
						//land only when the plane is at the first 3/4 the landing surface
						if(iFighter->GetCurrentPosition().iX > iLandingSurface->GetCurrentPosition().iX && iFighter->GetCurrentPosition().iX < iLandingSurface->GetCurrentPosition().iX + (iLandingSurface->GetWidth() * 5) / 8)
						{
							//plane is over landing surface, can land now
							iFighter->DecreaseFlyingAngle();
							iLandingStatus = ELandingStatus_Dive_Down_To_Land_On_Landing_Surface;
						}else
						{
							iFighter->TurnLeft();
						}
					}else
					{
						//landing surface diagram < 1 | 2 | 3 | 4 >
						//only land when the plane is at the last 3/4 of the landing surface
						if(iFighter->GetCurrentPosition().iX > iLandingSurface->GetCurrentPosition().iX + ((iLandingSurface->GetWidth() * 3) / 8) && iFighter->GetCurrentPosition().iX < iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth())
						{
							//plane is over landing surface, can land now
							iFighter->DecreaseFlyingAngle();
							iLandingStatus = ELandingStatus_Dive_Down_To_Land_On_Landing_Surface;
						}else
						{
							iFighter->TurnRight();
						}
					}
				}
			}
			break;
		}

			//all we do in this stage is to check if we touched down on the surface yet and then go into the next stage
		case ELandingStatus_Dive_Down_To_Land_On_Landing_Surface:
		{
			if(!iFighter->IsFlying())
			{
				//okay touched down, should slow down now
				iLandingStatus = ELandingStatus_Turn_Plane_To_Middle_Of_Landing_Surface;
			}
			break;
		}
		case ELandingStatus_Turn_Plane_To_Middle_Of_Landing_Surface:
		{
			AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_SLOW_SPEED_ON_LANDING_SURFACE));
			TIntFloat lXLandingSurfaceMidpoint = iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth() / 2;
			TIntFloat lFighterCurrentPosition = iFighter->GetCurrentPosition().iX;

			//move to default location on carrier
			if(lFighterCurrentPosition <= lXLandingSurfaceMidpoint)
			{
				//is to the left, need to face right
				iFighter->TurnRight();
			}else
			{
				//is to the right, need to face left
				iFighter->TurnLeft();
			}
			iLandingStatus = ELandingStatus_Move_To_Middle;
			break;
		}
		case ELandingStatus_Move_To_Middle:
		{
			AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_SLOW_SPEED_ON_LANDING_SURFACE));
			TIntFloat lXLandingSurfaceMidpoint = iLandingSurface->GetCurrentPosition().iX + iLandingSurface->GetWidth() / 2;
			TIntFloat lFighterCurrentPosition = iFighter->GetCurrentPosition().iX;

			if(iFighter->IsReflected())
			{
				//points right
				if(lFighterCurrentPosition >= lXLandingSurfaceMidpoint)
				{
					//is to the left, need to face right
					iLandingStatus = ELandingStatus_Wait;
				}
			}else
			{
				//points left
				if(lFighterCurrentPosition <= lXLandingSurfaceMidpoint)
				{
					//is to the left, need to face right
					iLandingStatus = ELandingStatus_Wait;
				}
			}

			break;
		}
		case ELandingStatus_Wait:
		{
			//just slow down to 0
			AdjustPlaneSpeed(TIntFloat::Convert(0));
			if(iFighter->GetCurrentAbsoluteSpeed().GetIntInBaseInt() == 0)
			{
				iLandingStatus = ELandingStatus_Stopped;
			}
			break;
		}
		case ELandingStatus_Stopped:
		{
			iCombatStatus = Ecombat_takeOff;
			iTakeOffStatus = ETakeOffStatus_start_engine;
			iPlaneStatus = EPlaneStatus_combat;
			break;
		}
	}
}

void CFighterAI::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iAvoidCollisionTiltDelay);
	aOutputStream.WriteInt32(iFrameCount);
	aOutputStream.WriteInt32(iSimulateReplinish);
	aOutputStream.WriteInt32(iReturnToBase);
	aOutputStream.WriteInt32(iCurrentAltitute);
	aOutputStream.WriteInt32(iDistanceToBase);
	aOutputStream.WriteInt32(iDistanceToTarget);
	aOutputStream.WriteIntFloat(iBaseLandingHeight);
	aOutputStream.WriteInt32(iFramesNeededToHitGround);
	aOutputStream.WriteInt32(iNeededDistanceForDrop);
	aOutputStream.WriteInt32(iExtraWidthOffset);
	aOutputStream.WriteInt32(iMiddlePositionOfBombsOffset);
	aOutputStream.WriteInt32(iManeuverDelay);
	aOutputStream.WriteInt32(iManeuverSmoothness);
	aOutputStream.WriteInt32(iManeuverFrames);
	aOutputStream.WriteInt32(iManeuverDistanceTravelled);
	aOutputStream.WriteInt32(iChaseDownTiltDelay);
	aOutputStream.WriteInt32(iShipBombLowHeight);
	aOutputStream.WriteInt32(iTargetPriority);
	//states
	aOutputStream.WriteInt32(iPlaneStatus);
	aOutputStream.WriteInt32(iCombatStatus);
	aOutputStream.WriteInt32(iPreviousCombatStatus);
	aOutputStream.WriteInt32(iPositionFromBase);
	aOutputStream.WriteInt32(iLandingStatus);
	aOutputStream.WriteInt32(iTakeOffStatus);
	aOutputStream.WriteInt32(iRetreatStatus);
	aOutputStream.WriteInt32(iPreviousRetreatStatus);
	aOutputStream.WriteInt32(iManeuverCycle);
	aOutputStream.WriteInt32(iManeuverAngle);
	aOutputStream.WriteInt32(iFlyToBombingHeightStatus);
	aOutputStream.WriteInt32(iFlyTowardsTarget);
	aOutputStream.WriteInt32(iAccurateBombStatus);
	aOutputStream.WriteInt32(iBurstBombStatus);
	aOutputStream.WriteInt32(iFlyTowardsShip);
	aOutputStream.WriteInt32(iShipBombStatus);
	aOutputStream.WriteInt32(iShipBombBehindTarget);
	aOutputStream.WriteInt32(iShipBombHeightAdjust);
	aOutputStream.WriteInt32(iShipAvoidCollision);
	aOutputStream.WriteInt32(iAttackPlaneStatus);
	aOutputStream.WriteInt32(iAvoidCollisionStatus);
	aOutputStream.WriteInt32(iContinueFromDogFightStatus);
	aOutputStream.WriteInt32(iRetreatAvoidOtherPlaneCollisionStatus);
	aOutputStream.WriteInt32(iShotByOtherPlaneStatus);
	aOutputStream.WriteInt32(iBombTargetSwitchingStatus);
	//pointers
	aOutputStream.WriteUint32((TUint32)iTarget);
	aOutputStream.WriteUint32((TUint32)iBigPlane);
	aOutputStream.WriteUint32((TUint32)iShip);
	aOutputStream.WriteUint32((TUint32)iLandingSurface);
	//no need to save Fighter, since Fighter will create this object and set the pointer automatically
}

void CFighterAI::TakeOffFromAirport()
{
	//find target to kill
	if(!iTarget)
	{
		TargetSpecificTarget(iTargetPriority);
	}else if(iTarget)
	{
		//procedure to take off
		//accelerate one step to get the plane moving (or the plane will not face any direction when attempting in the next step)
		//always face the airport side that has more runway by looking at where it is located on the airport
		//increase speed
		//attempt to take off when ever possible
		iReturnToBase = false;
		switch(iTakeOffStatus)
		{
			case ETakeOffStatus_start_engine:
			{
				iSimulateReplinish++;
				iManeuverSmoothness = 0;
				//change weapon equipment base on target locked on
				if(iTarget)
				{
					switch(iTarget->GetGameObjectType())
					{
						case EObjectTypesBuilding:
						{
							iFighter->EquipWithNewSecondaryWeapon(EWeaponIdentifier1000kgBomb);
							break;
						}
						case EObjectTypesPlane:
						case EObjectTypesBigPlane:
						{
							iFighter->EquipWithNewSecondaryWeapon(EWeaponIdentifier1000kgBomb);
							break;
						}
						case EObjectTypesArmouredGroundUnit:
						{
							iFighter->EquipWithNewSecondaryWeapon(EWeaponIdentifier100kgBomb);
							break;
						}
						case EObjectTypesGroundUnit:
						{
							iFighter->EquipWithNewSecondaryWeapon(EWeaponIdentifier100kgBomb);
							break;
						}
						case EObjectTypesLandingSurface:
						{
							iFighter->EquipWithNewSecondaryWeapon(EWeaponIdentifier1000kgBomb);
							break;
						}
						case EObjectTypesShip:
						{
							iFighter->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
							break;
						}
						default:
							break;
					}
				}
				//delay 3 seconds to emulate replinish ammunition
				if(iSimulateReplinish == FIGHTER_AI_WAIT_FRAMES)
				{
					//speed up one step so that we can turn if it is too close to the edge of the base
					if(iFighter->GetCurrentAbsoluteSpeed().GetIntInBaseInt() == 0 && iTarget)
					{
						iFighter->IncreaseSpeed();
						iTakeOffStatus = ETakeOffStatus_face_target;
					}
					iSimulateReplinish = 0;
				}
				break;
			}
			case ETakeOffStatus_face_target:
			{
				if(iTarget)
				{
					if(iFighter->GetCurrentPosition().iX >= iTarget->GetCurrentPosition().iX)
					{
						iFighter->TurnLeft();
					}
					if(iFighter->GetCurrentPosition().iX < iTarget->GetCurrentPosition().iX)
					{
						iFighter->TurnRight();
					}
				}
				iTakeOffStatus = ETakeOffStatus_accelerate_and_tilt_up;
				break;
			}
			case ETakeOffStatus_accelerate_and_tilt_up:
			{
				if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() < FIGHTER_AI_MAX_ENGINE_SPEED)
				{
					iFighter->IncreaseSpeed();
				}
				if(iFighter->IsReflected())
				{
					if(*iFighter->GetFlyingAngel() == 45)
					{
						if(CMath::Abs(iFighter->GetCurrentPosition().iX - iTarget->GetCurrentPosition().iX) <= TIntFloat::Convert(FIGHTER_AI_BOMBING_CLOSE_DISTANCE))
						{
							iTakeOffStatus = ETakeOffStatus_climb_to_low_fighting_altitude;
						}else if(CMath::Abs(iFighter->GetCurrentPosition().iX - iTarget->GetCurrentPosition().iX) > TIntFloat::Convert(FIGHTER_AI_BOMBING_CLOSE_DISTANCE))
						{
							iTakeOffStatus = ETakeOffStatus_climb_to_high_fighting_altitude;
						}
					}
					FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
				}else if(!iFighter->IsReflected())
				{
					if(*iFighter->GetFlyingAngel() == 135)
					{
						if(CMath::Abs(iFighter->GetCurrentPosition().iX - iTarget->GetCurrentPosition().iX) <= TIntFloat::Convert(FIGHTER_AI_BOMBING_CLOSE_DISTANCE))
						{
							iTakeOffStatus = ETakeOffStatus_climb_to_low_fighting_altitude;
						}else if(CMath::Abs(iFighter->GetCurrentPosition().iX - iTarget->GetCurrentPosition().iX) > TIntFloat::Convert(FIGHTER_AI_BOMBING_CLOSE_DISTANCE))
						{
							iTakeOffStatus = ETakeOffStatus_climb_to_high_fighting_altitude;
						}
					}
					FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
				}
				break;
			}
			case ETakeOffStatus_climb_to_low_fighting_altitude:
			{
				if(iFighter->GetCurrentPositionNormilized().iY > FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT - 10 && iFighter->GetCurrentPositionNormilized().iY < FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT + 10)
				{
					iShipBombLowHeight = 1;
					iTakeOffStatus = ETakeOffStatus_adjust_horizontal_angle;
				}
				break;
			}
			case ETakeOffStatus_climb_to_high_fighting_altitude:
			{
				if(iFighter->GetCurrentPositionNormilized().iY > FIGHTER_AI_BOMBING_DIVE_HEIGHT - 10 && iFighter->GetCurrentPositionNormilized().iY < FIGHTER_AI_BOMBING_DIVE_HEIGHT + 10)
				{
					iShipBombLowHeight = 0;
					iTakeOffStatus = ETakeOffStatus_adjust_horizontal_angle;
				}
				break;
			}
			case ETakeOffStatus_adjust_horizontal_angle:
			{
				if(iFighter->IsReflected())
				{
					if(*iFighter->GetFlyingAngel() == 0 && !(iTarget->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane | EObjectTypesShip)))
					{
						iPlaneStatus = EPlaneStatus_combat;
						iCombatStatus = ECombat_dive_to_target;
					}else if(*iFighter->GetFlyingAngel() == 0 && (iTarget->GetGameObjectType() & (EObjectTypesShip)))
					{
						iPlaneStatus = EPlaneStatus_combat;
						iCombatStatus = ECombat_dive_to_sea_level;
					}else if(*iFighter->GetFlyingAngel() == 0 && (iTarget->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane)))
					{
						iPlaneStatus = EPlaneStatus_combat;
						iCombatStatus = ECombat_attack;
					}
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				}else if(!iFighter->IsReflected())
				{
					if(*iFighter->GetFlyingAngel() == 180 && !(iTarget->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane | EObjectTypesShip)))
					{
						iPlaneStatus = EPlaneStatus_combat;
						iCombatStatus = ECombat_dive_to_target;
					}else if(*iFighter->GetFlyingAngel() == 180 && (iTarget->GetGameObjectType() & (EObjectTypesShip)))
					{
						iPlaneStatus = EPlaneStatus_combat;
						iCombatStatus = ECombat_dive_to_sea_level;
					}else if(*iFighter->GetFlyingAngel() == 180 && (iTarget->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesBigPlane)))
					{
						iPlaneStatus = EPlaneStatus_combat;
						iCombatStatus = ECombat_attack;
					}
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				}
				break;
			}
		}
	}
}

void CFighterAI::Update()
{
	if(!iFighter->IsAlive())
		return; ///don't update if dead

	//check if the plane is ai controlled
	if(iFrameCount == FIGHTER_AI_FRAMES_UNTIL_NEXT_UPDATE)
	{
		iFrameCount = 0;
		//plane will do different objectives based on its status
		switch(iPlaneStatus)
		{
			case EPlaneStatus_combat:
			{
				if(iTarget && iTarget->GetIsMoveableGameObject() && static_cast<CMoveableGameObject*> (iTarget)->GetIsLeavingMap())
				{
					iTarget = NULL;
					iPlaneStatus = EPlaneStatus_retreat;
					break;
				}

				switch(iCombatStatus)
				{
					case Ecombat_takeOff:
					{
						TakeOffFromAirport();
						break;
					}
					case ECombat_attack:
					{
						switch(iAttackPlaneStatus)
						{
							case EAttackPlaneStatus_adjust_saftey_height:
							{
								AdjustToSafteyHeight();
								break;
							}
							case EAttackPlaneStatus_attack_player:
							{
								ShortestPathShootDown();
								break;
							}
						}
						break;
					}
					case ECombat_continue_priority_target:
					{
						ContinueAfterDogFight();
						break;
					}
					case ECombat_avoid_flying_too_low:
					{
						AvoidFlyingTooLow();
						break;
					}
					case ECombat_avoid_plane:
					{
						if(iAvoidCollisionTiltDelay == FIGHTER_AI_CHASE_DOWN_TILT_DELAY)
						{
							iAvoidCollisionTiltDelay = 0;
							AvoidTargetCollision();
						}
						iAvoidCollisionTiltDelay++;
						break;
					}
					case ECombat_avoid_ship:
					{
						AvoidShipCollision();
						break;
					}
					case ECombat_accurate_bomb:
					{
						AccurateBombLogic();
						break;
					}
					case ECombat_burst_bomb:
					{
						BurstBombLogic();
						break;
					}
					case ECombat_ship_bomb:
					{
						ShipBombLogic();
						break;
					}
					case ECombat_maneuver:
					{
						ManeuverAngles();
						break;
					}
					case ECombat_dive_to_target:
					{
						DiveToTarget();
						break;
					}
					case ECombat_dive_to_sea_level:
					{
						CheckBeforeDiveToSeaLevel();
						//DiveToSeaLevel();
						break;
					}
					case ECombat_sucicide:
					{
						break;
					}
					case ECombat_revenge:
					{
						break;
					}
				}
				break;
			}
			case EPlaneStatus_retreat:
			{
				switch(iRetreatStatus)
				{
					case ERetreat_return_to_base:
					{
						if(!iReturnToBase)
						{
							iReturnToBase = true;
							ResetAIBehaviour();
							FindSuitableBaseForLanding();
							iLandingStatus = ELandingStatus_Leave_Current_Zone;//fly back to home base
							iManeuverSmoothness = 0;
						}
						if(iReturnToBase && iFighter->GetHealth() > 0)
						{
							RetreatAvoidBigPlaneHitBox();
							RunLogicCodeForLanding();
						}
						break;
					}
					case ERetreat_maneuver:
					{
						ManeuverAngles();
						break;
					}
					case ERetreat_avoid_plane:
					{
						RetreatAvoidOtherPlaneCollision();
						break;
					}
				}
				break;
			}
		}

		//set to null if target is dead, something went wrong, shouldn't point to a dead target, reset to NULL and go back to stage from which we can recover
		if( (iTarget && !iTarget->IsAlive())
				|| (iTarget && iTarget->GetIsMoveableGameObject() && static_cast<CMoveableGameObject*> (iTarget)->GetIsLeavingMap()))
		{
			iTarget = NULL;
			iPlaneStatus = EPlaneStatus_retreat;
			iRetreatStatus = ERetreat_return_to_base;
		}
	}
	iFrameCount++;


}
