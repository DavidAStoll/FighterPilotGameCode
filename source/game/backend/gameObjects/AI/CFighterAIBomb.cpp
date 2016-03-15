/*
 * FighterAIBomb.cpp
 *
 *  Created on: 2011/06/16
 *      Author: Stephen
 */

#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CWeaponBomb.h"
#include "includes/game/CFighterPilotThePacificWar.h"

void CFighterAI::AccurateBombCheckHeight()
{
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	//if the ai flies lower than 300 it will climb back up to 400
	if(lFighterHeight <= FIGHTER_AI_BOMBING_HEIGHT - 100)
	{
		iPreviousCombatStatus = ECombat_accurate_bomb;
		iCombatStatus = ECombat_avoid_flying_too_low;
	}
}

void CFighterAI::AccurateBombLogic()
{
	CheckHealthStatus();
	AccurateBombCheckHeight();
	//retreat when the ai runs out of bombs
	if(iFighter->Fire2HasAmmunitionLeft() == false)
	{
		iPlaneStatus = EPlaneStatus_retreat;
	}
	if(iTarget)
	{
		if(iTarget->IsAlive() && iTarget->GetConflictSide() != iFighter->GetConflictSide())
		{
			CalculateNeededDistanceForDrop();
			CalculateManeuverDistanceTravel(FIGHTER_AI_IDLE_MANEUVER_ANGLE);
			TInt lTargetLocation = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
			TInt lFighterLocation = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
			TInt lTargetWidth = iTarget->GetWidth().GetIntInBaseInt();
			TInt lFighterWidth = iFighter->GetWidth().GetIntInBaseInt();
			TInt lHalfBombLength = static_cast<CWeaponBomb*> (iFighter->GetSecondaryWeapon())->GetXMiddlePointOfWeapon().GetIntInBaseInt();

			switch(iAccurateBombStatus)
			{
				case EAccurateBombStatus_Face_Target:
				{
					//face plane towards target
					if(lFighterLocation >= lTargetLocation + lTargetWidth + iNeededDistanceForDrop + lFighterWidth)
					{
						iFighter->TurnLeft();
						iAccurateBombStatus = EAccurateBombStatus_Initiate_Bombing;
					}
					if(lFighterLocation < lTargetLocation - lTargetWidth - iNeededDistanceForDrop - lFighterWidth)
					{
						iFighter->TurnRight();
						iAccurateBombStatus = EAccurateBombStatus_Initiate_Bombing;
					}
					//go back to bombing state if it is within range
					else if(lFighterLocation < lTargetLocation + lTargetWidth + iNeededDistanceForDrop + lFighterWidth &&
							lFighterLocation > lTargetLocation - lTargetWidth - iNeededDistanceForDrop - lFighterWidth)
					{
						iAccurateBombStatus = EAccurateBombStatus_Initiate_Bombing;
					}
					break;
				}
				case EAccurateBombStatus_Initiate_Bombing:
				{
					TBool lBombLandsOnTarget = false;
					TBool lTooEarlyToBomb = false;
					TBool lPassedTarget = false;
					if(!iFighter->IsReflected())
					{
						if(!iTarget->IsReflected())
						{
							if(lFighterLocation - iNeededDistanceForDrop <= lTargetLocation + lTargetWidth + lHalfBombLength &&
									lFighterLocation - iNeededDistanceForDrop >= lTargetLocation - lHalfBombLength)
							{
								lBombLandsOnTarget = true;
							}
							if(lFighterLocation - iNeededDistanceForDrop < lTargetLocation - lHalfBombLength)
							{
								lPassedTarget = true;
							}
							if(lFighterLocation - iNeededDistanceForDrop > lTargetLocation + lTargetWidth + lHalfBombLength + 2 * iManeuverDistanceTravelled)
							{
								lTooEarlyToBomb = true;
							}
						}
						else if(iTarget->IsReflected())
						{
							if(lFighterLocation - iNeededDistanceForDrop <= lTargetLocation + lHalfBombLength &&
									lFighterLocation - iNeededDistanceForDrop >= lTargetLocation - lTargetWidth - lHalfBombLength)
							{
								lBombLandsOnTarget = true;
							}
							if(lFighterLocation - iNeededDistanceForDrop < lTargetLocation - lTargetWidth - lHalfBombLength)
							{
								lPassedTarget = true;
							}
							if(lFighterLocation - iNeededDistanceForDrop > lTargetLocation + lHalfBombLength + 2 * iManeuverDistanceTravelled)
							{
								lTooEarlyToBomb = true;
							}
						}
					}
					else if(iFighter->IsReflected())
					{
						if(iTarget->IsReflected())
						{
							if(lFighterLocation + iNeededDistanceForDrop <= lTargetLocation + lHalfBombLength &&
									lFighterLocation + iNeededDistanceForDrop >= lTargetLocation - lTargetWidth - lHalfBombLength)
							{
								lBombLandsOnTarget = true;
							}
							if(lFighterLocation + iNeededDistanceForDrop > lTargetLocation + lHalfBombLength)
							{
								lPassedTarget = true;
							}
							if(lFighterLocation + iNeededDistanceForDrop < lTargetLocation - lTargetWidth - lHalfBombLength - 2 * iManeuverDistanceTravelled)
							{
								lTooEarlyToBomb = true;
							}
						}
						else if(!iTarget->IsReflected())
						{
							if(lFighterLocation + iNeededDistanceForDrop <= lTargetLocation + lTargetWidth + lHalfBombLength &&
									lFighterLocation + iNeededDistanceForDrop >= lTargetLocation - lHalfBombLength)
							{
								lBombLandsOnTarget = true;
							}
							if(lFighterLocation + iNeededDistanceForDrop > lTargetLocation + lTargetWidth + lHalfBombLength)
							{
								lPassedTarget = true;
							}
							if(lFighterLocation + iNeededDistanceForDrop < lTargetLocation - lHalfBombLength - 2 * iManeuverDistanceTravelled)
							{
								lTooEarlyToBomb = true;
							}
						}
					}
					if(lBombLandsOnTarget && !lPassedTarget)
					{
						if(iFighter->Fire2HasAmmunitionLeft() == true)
						{
							iFighter->Fire2();
							iAccurateBombStatus = EAccurateBombStatus_Face_Target;
						}
					}
					else if(lPassedTarget)
					{
						iAccurateBombStatus = EAccurateBombStatus_Fly_To_Bombing_Distance;
					}
					else if(lTooEarlyToBomb)
					{
						iPreviousCombatStatus = ECombat_accurate_bomb;
						iManeuverAngle = FIGHTER_AI_BOMBING_MANEUVER_ANGLE;
						iCombatStatus = ECombat_maneuver;
					}
					break;
				}
				case EAccurateBombStatus_Fly_To_Bombing_Distance:
				{
					if(lFighterLocation >= lTargetLocation + lTargetWidth + iNeededDistanceForDrop)
					{
						iAccurateBombStatus = EAccurateBombStatus_Face_Target;
					}
					if(lFighterLocation < lTargetLocation - lTargetWidth - iNeededDistanceForDrop)
					{
						iAccurateBombStatus = EAccurateBombStatus_Face_Target;
					}
					iPreviousCombatStatus = ECombat_accurate_bomb;
					iManeuverAngle = FIGHTER_AI_BOMBING_MANEUVER_ANGLE;
					iCombatStatus = ECombat_maneuver;
					break;
				}
			}
		}
		else
		{
			iTarget = NULL;
		}
	}
	else if(!iTarget)
	{
		//find a suitable target
		//go through all objects that exists on the map and find one that is closest
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesBuilding);
		TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
		TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
		CGameObject* lCurrentBestTarget = NULL;
		CGameObject* lCurrentPossibleTarget = NULL;

		for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

			//look for different target based on unused equipped bomb
			if(iFighter->GetAmmunitionLeftOfSecondaryWeapon() > 0)
			{
				switch(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier())
				{
					case EWeaponIdentifier1000kgBomb:
					{
						if(lFighterConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive())
						{
							//check if the object is not too close to drop bombs on it properly
							lCurrentPossibleTarget = lCurrentGameObject;
							TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
							TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();
							TInt lDistanceBetweenTargetLocation = CMath::Abs(lBomberLocation.iX - lTargetLocation.iX).GetIntInBaseInt();
							iTarget = lCurrentGameObject;
							CalculateNeededDistanceForDrop();

							if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
							{
								lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
								lCurrentBestTarget = lCurrentGameObject;
							}
						}
						break;
					}
					default:
						break;
				}
			}
		}
		//clean up
		delete lGameObjects;

		TBool lTargetIsCloseEnough = lCurrentShortestPossibleDistance < FIGHTER_AI_MAX_DISTANCE_TO_ATTACK_ANOTHER_TARGET;
		//check if we found a valid target
		if(lCurrentBestTarget && lTargetIsCloseEnough)
		{
			iTarget = lCurrentBestTarget;
			CalculateNeededDistanceForDrop();
		}else
		{
			if(!lCurrentPossibleTarget || !lTargetIsCloseEnough)//no targets left in its own weapon category
			{
				iTarget = NULL;
				iPlaneStatus = EPlaneStatus_retreat;
			}else
			{
				iTarget = lCurrentPossibleTarget;
			}
		}
		iAccurateBombStatus = EAccurateBombStatus_Face_Target;
	}
}

void CFighterAI::AvoidFlyingTooLow()
{
	TInt lFighterAngle = *iFighter->GetFlyingAngel();
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	if((iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt()) != FIGHTER_AI_MAX_ENGINE_SPEED)
	{
		iFighter->IncreaseSpeed();
	}
	switch(iFlyToBombingHeightStatus)
	{
		case EFlyToBombingHeightStatus_tilt_up:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 135)
				{
					FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
				}
				else
				{
					iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_check_height;
				}
			}
			else if(iFighter->IsReflected())
			{
				if(lFighterAngle != 45)
				{
					FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
				}
				else
				{
					iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_check_height;
				}
			}
			break;
		}
		case EFlyToBombingHeightStatus_check_height:
		{
			if(lFighterHeight >= FIGHTER_AI_BOMBING_HEIGHT)
			{
				iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_fly_horizontally;
			}
			break;
		}
		case EFlyToBombingHeightStatus_fly_horizontally:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 180)
				{
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				}
				else
				{
					iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_tilt_up;
					iCombatStatus = iPreviousCombatStatus;
				}
			}
			else if(iFighter->IsReflected())
			{
				if(lFighterAngle != 0)
				{
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				}
				else
				{
					iFlyToBombingHeightStatus = EFlyToBombingHeightStatus_tilt_up;
					iCombatStatus = iPreviousCombatStatus;
				}
			}
			break;
		}
	}
}

void CFighterAI::BurstBombCheckHeight()
{
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	//if the ai flies lower than 300 it will climb back up to 400
	if(lFighterHeight <= FIGHTER_AI_BOMBING_HEIGHT - 100)
	{
		iPreviousCombatStatus = ECombat_burst_bomb;
		iCombatStatus = ECombat_avoid_flying_too_low;
	}
}

void CFighterAI::BurstBombLogic()
{
	CheckHealthStatus();
	BurstBombCheckHeight();
	//retreat when the ai runs out of bombs
	if(iFighter->Fire2HasAmmunitionLeft() == false)
	{
		iPlaneStatus = EPlaneStatus_retreat;
	}

	if(iTarget)
	{
		if(iTarget->IsAlive() && iTarget->GetConflictSide() != iFighter->GetConflictSide())
		{
			CalculateNeededDistanceForDrop();
			TInt lTargetLocation = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
			TInt lFighterLocation = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
			TInt lDistanceBetweenTargetLocation = CMath::Abs(lFighterLocation - lTargetLocation);
			TInt lTargetWidth = iTarget->GetWidth().GetIntInBaseInt();
			TInt lFighterWidth = iFighter->GetWidth().GetIntInBaseInt();
			TInt lTargetDistanceTravelled = 0;
			if(iTarget->GetIsMoveableGameObject())
			{
				 CMoveableGameObject* lMoveableTarget = static_cast<CMoveableGameObject*> (iTarget);
				 lTargetDistanceTravelled = CMath::Abs(((*lMoveableTarget->GetPixelsPerMoveX()) * iFramesNeededToHitGround).GetIntInBaseInt());
			}
			
			switch(iBurstBombStatus)
			{
				case EBurstBombStatus_Face_Target:
				{
					//face plane towards target
					if(lFighterLocation >= lTargetLocation + 2 * lTargetWidth + iNeededDistanceForDrop + lFighterWidth)
					{
						iFighter->TurnLeft();
						iBurstBombStatus = EBurstBombStatus_Initiate_Bombing;
					}
					if(lFighterLocation < lTargetLocation - 2 * lTargetWidth - iNeededDistanceForDrop - lFighterWidth)
					{
						iFighter->TurnRight();
						iBurstBombStatus = EBurstBombStatus_Initiate_Bombing;
					}
					//go back to bombing state if it is within range
					else if(lFighterLocation < lTargetLocation + 2 * lTargetWidth + iNeededDistanceForDrop + lFighterWidth &&
							lFighterLocation > lTargetLocation - 2 * lTargetWidth - iNeededDistanceForDrop - lFighterWidth)
					{
						iBurstBombStatus = EBurstBombStatus_Initiate_Bombing;
					}
					break;
				}
				case EBurstBombStatus_Initiate_Bombing:
				{
					TBool lIsWithinNeededDistance = (lDistanceBetweenTargetLocation + iExtraWidthOffset) - iNeededDistanceForDrop	<= CMath::Abs(iManeuverDistanceTravelled * 1.5) + lTargetWidth;
					TBool lPassedTarget = false;
					if(!iFighter->IsReflected())
					{
						if(!iTarget->IsReflected())
						{
							if(lFighterLocation - 2 * (iNeededDistanceForDrop) <= lTargetLocation - iNeededDistanceForDrop - lTargetDistanceTravelled)
							{
								lPassedTarget = true;
							}
						}
						else if(iTarget->IsReflected())
						{
							if(lFighterLocation - 2 * iNeededDistanceForDrop <= lTargetLocation - lTargetWidth - iNeededDistanceForDrop - lTargetDistanceTravelled)
							{
								lPassedTarget = true;
							}
						}
					}
					else if(iFighter->IsReflected())
					{
						if(iTarget->IsReflected())
						{
							if(lFighterLocation + 2 * iNeededDistanceForDrop >= lTargetLocation + iNeededDistanceForDrop + lTargetDistanceTravelled)
							{
								lPassedTarget = true;
							}
						}
						else if(!iTarget->IsReflected())
						{
							if(lFighterLocation + 2 * iNeededDistanceForDrop >= lTargetLocation + lTargetWidth + iNeededDistanceForDrop + lTargetDistanceTravelled)
							{
								lPassedTarget = true;
							}
						}
					}
					if(lIsWithinNeededDistance && !lPassedTarget)
					{
						if(iFighter->Fire2HasAmmunitionLeft() == true)
						{
							iFighter->Fire2();
							iBurstBombStatus = EBurstBombStatus_Face_Target;
						}
					}
					else if(lPassedTarget)
					{
						iBurstBombStatus = EBurstBombStatus_Fly_To_Bombing_Distance;
					}
					else if(!lIsWithinNeededDistance)
					{
						iPreviousCombatStatus = ECombat_burst_bomb;
						iManeuverAngle = FIGHTER_AI_BOMBING_MANEUVER_ANGLE;
						iCombatStatus = ECombat_maneuver;
					}
					break;
				}
				case EBurstBombStatus_Fly_To_Bombing_Distance:
				{
					if(lFighterLocation >= lTargetLocation + 2 * lTargetWidth + iNeededDistanceForDrop)
					{
						iBurstBombStatus = EBurstBombStatus_Face_Target;
					}
					if(lFighterLocation < lTargetLocation - 2 * lTargetWidth - iNeededDistanceForDrop)
					{
						iBurstBombStatus = EBurstBombStatus_Face_Target;
					}
					iPreviousCombatStatus = ECombat_burst_bomb;
					iManeuverAngle = FIGHTER_AI_BOMBING_MANEUVER_ANGLE;
					iCombatStatus = ECombat_maneuver;
					break;
				}
			}
		}
		else
		{
			iTarget = NULL;
		}
	}
	else if(!iTarget)
	{
		//find a suitable target
		//go through all objects that exists on the map and find one that is closest
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit);
		TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
		TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
		CGameObject* lCurrentBestTarget = NULL;
		CGameObject* lCurrentPossibleTarget = NULL;

		for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

			//look for different target based on unused equipped bomb
			if(iFighter->GetAmmunitionLeftOfSecondaryWeapon() > 0)
			{
				switch(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier())
				{
					case EWeaponIdentifier100kgBomb:
					{
						if(lFighterConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive())
						{
							//check if the object is not too close to drop bombs on it properly
							lCurrentPossibleTarget = lCurrentGameObject;
							TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
							TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();
							TInt lDistanceBetweenTargetLocation = CMath::Abs(lBomberLocation.iX - lTargetLocation.iX).GetIntInBaseInt();
							iTarget = lCurrentGameObject;
							CalculateNeededDistanceForDrop();

							if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
							{
								lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
								lCurrentBestTarget = lCurrentGameObject;
							}
						}
						break;
					}
					default:
						break;
				}
			}
		}
		//clean up
		delete lGameObjects;

		TBool lTargetIsCloseEnough = lCurrentShortestPossibleDistance < FIGHTER_AI_MAX_DISTANCE_TO_ATTACK_ANOTHER_TARGET;
		//check if we found a valid target
		if(lCurrentBestTarget && lTargetIsCloseEnough)
		{
			iTarget = lCurrentBestTarget;
			CalculateNeededDistanceForDrop();
		}else
		{
			if(!lCurrentPossibleTarget || !lTargetIsCloseEnough)//no targets left in its own weapon category
			{
				iTarget = NULL;
				iPlaneStatus = EPlaneStatus_retreat;
			}else
			{
				iTarget = lCurrentPossibleTarget;
			}
		}
		iBurstBombStatus = EBurstBombStatus_Face_Target;
	}
}

void CFighterAI::CalculateManeuverDistanceTravel(maneuverAngle aManeuverAngle)
{
	switch(aManeuverAngle)
	{
		case EManeuverAngle15:
		{
			//6 tilts in the maneuver * number of tilts in the angle + double down delay of the maneuver
			iManeuverFrames = 6 * 1 * FIGHTER_AI_MANEUVER_SMOOTHNESS + 2;
			iManeuverDistanceTravelled = (iFighter->GetCurrentAbsoluteSpeed() * iManeuverFrames).GetIntInBaseInt();
			break;
		}
		case EManeuverAngle30:
		{
			iManeuverFrames = 6 * 2 * FIGHTER_AI_MANEUVER_SMOOTHNESS + 2;
			iManeuverDistanceTravelled = (iFighter->GetCurrentAbsoluteSpeed() * iManeuverFrames).GetIntInBaseInt();
			break;
		}
		case EManeuverAngle45:
		{
			iManeuverFrames = 6 * 3 * FIGHTER_AI_MANEUVER_SMOOTHNESS + 2;
			iManeuverDistanceTravelled = (iFighter->GetCurrentAbsoluteSpeed() * iManeuverFrames).GetIntInBaseInt();
			break;
		}
		case EManeuverAngle60:
		{
			iManeuverFrames = 6 * 4 * FIGHTER_AI_MANEUVER_SMOOTHNESS + 2;
			iManeuverDistanceTravelled = (iFighter->GetCurrentAbsoluteSpeed() * iManeuverFrames).GetIntInBaseInt();
			break;
		}
	}
}

void CFighterAI::CalculateNeededDistanceForDrop()
{
	CGameObject* lTarget = iTarget;
	TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();

	//Newton formula
	//Vf^2 = Vi^2 + 2AD
	//Vf = velocity final, Vi = velocity initial, A = total acceleration, D = distance travelled
	//find final velocity, used to find time

	TIntFloat lAcceleration = TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE) / TIntFloat::Convert(GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE);
	TIntFloat lOldVelocitySquared = (*iFighter->GetPixelsPerMoveY()) *  (*iFighter->GetPixelsPerMoveY());
	TIntFloat lAggTimesDistance = (lAcceleration * lBomberLocation.iY) * 2;
	TInt lSum = (lOldVelocitySquared + lAggTimesDistance).GetIntInBaseInt();
	TFloat lTemp = lSum;
	lTemp = CMath::Sqrt(lTemp);
	TIntFloat lFinalVelocity = TIntFloat::Convert(lTemp);

	//Newton formula
	//Vf = Vi + AT
	//Vf = velocity final, Vi = velocity initial, A = total acceleration, T = time 
	//find time

	TIntFloat lDiffBetweenFinalAndStartingVelocity = lFinalVelocity - (*iFighter->GetPixelsPerMoveY());
	iFramesNeededToHitGround = (lDiffBetweenFinalAndStartingVelocity / lAcceleration).GetIntInBaseInt();
	//at this distance to the target we should drop the bomb
	iNeededDistanceForDrop = CMath::Abs((*iFighter->GetPixelsPerMoveX()) * iFramesNeededToHitGround).GetIntInBaseInt();
	//calculate how much we need we be off the target because of the position of bombs on the plane and to hit traget right in the middle
	//add also where first bomb is mounted on the airplane
	iExtraWidthOffset = 0;
	iExtraWidthOffset -= static_cast<CWeaponBomb*>(iFighter->GetSecondaryWeapon())->GetXMiddlePointOfWeapon().GetIntInBaseInt();
	TIntFloat lTargetMovingDistance = TIntFloat::Convert(0);

	if(iTarget->GetIsMoveableGameObject())
	{
		lTargetMovingDistance = *(static_cast<CMoveableGameObject*> (iTarget)->GetPixelsPerMoveX());
		lTargetMovingDistance = CMath::Abs(lTargetMovingDistance * iFramesNeededToHitGround);
	}
	//need to check if we approach it from a side where the fixpoint is at the end of the object
	TInt lWidthOfTargetObject = lTarget->GetWidth().GetIntInBaseInt();
	if((!lTarget->IsReflected() && !iFighter->IsReflected()) || (lTarget->IsReflected() && iFighter->IsReflected()))
		iExtraWidthOffset -= (lWidthOfTargetObject) / 2 - (lTargetMovingDistance.GetIntInBaseInt());
	else
		iExtraWidthOffset += (lWidthOfTargetObject) / 2 - (lTargetMovingDistance.GetIntInBaseInt());
}

void CFighterAI::DiveToTarget()
{
	CheckHealthStatus();
	switch(iFlyTowardsTarget)
	{
		case EFlyTowardsTarget_Check_Dive_Down:
		{
			if(iTarget)
			{
				iManeuverSmoothness = 0;
				CalculateManeuverDistanceTravel(FIGHTER_AI_IDLE_MANEUVER_ANGLE);
				//check what X distance we need to be away to start the dive
				TIntFloat lHeightDifference = iFighter->GetCurrentPosition().iY - FIGHTER_AI_BOMBING_DIVE_HEIGHT;
				TInt lXDistanceBeforeStartingBombing = GetXDistanceTravelledWhenChangingHeightDown(lHeightDifference, FIGHTER_AI_ANGLE_WHEN_DIVING_TOWARDS_LANDING_SURFACE).GetIntInBaseInt();
				lXDistanceBeforeStartingBombing += FIGHTER_AI_BOMBING_DISTANCE + iManeuverDistanceTravelled;
				//find current distance to base
				iDistanceToTarget = iFighter->DistanceBetweenGameObjects(iTarget).GetIntInBaseInt();
				//check if we are close enough to dive down to landing height
				if(lXDistanceBeforeStartingBombing > iDistanceToTarget)
				{
					//dive down towards the base instead of away
					if(iFighter->GetCurrentPosition().iX <= iTarget->GetCurrentPosition().iX + iTarget->GetWidth() / 2)
					{
						iFighter->TurnRight();
					}else if(iFighter->GetCurrentPosition().iX > iTarget->GetCurrentPosition().iX + iTarget->GetWidth() / 2)
					{
						iFighter->TurnLeft();
					}
					iFlyTowardsTarget = EFlyTowardsTarget_Dive_Down;
				}
				else
				{
					iPreviousCombatStatus = ECombat_dive_to_target;
					iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
					iCombatStatus = ECombat_maneuver;
				}
			}
			//every object on the map is dead excluding planes
			else if(!iTarget)
			{
				iPreviousCombatStatus = ECombat_dive_to_target;
				iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
				iCombatStatus = ECombat_maneuver;
			}
			break;
		}
		case EFlyTowardsTarget_Dive_Down:
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

			//check if we are already at the target height
			if(FIGHTER_AI_BOMBING_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > iFighter->GetCurrentPosition().iY.GetIntInBaseInt())
			{
				iFlyTowardsTarget = EFlyTowardsTarget_Fly_Horizontally;
			}
			break;
		}
		case EFlyTowardsTarget_Fly_Horizontally:
		{
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 0)
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(iTarget->GetGameObjectType() == EObjectTypesGroundUnit || iTarget->GetGameObjectType() == EObjectTypesArmouredGroundUnit)
					{
						iCombatStatus = ECombat_burst_bomb;
					}
					else if(iTarget->GetGameObjectType() != EObjectTypesGroundUnit && iTarget->GetGameObjectType() != EObjectTypesArmouredGroundUnit)
					{
						iCombatStatus = ECombat_accurate_bomb;
					}
					iFlyTowardsTarget = EFlyTowardsTarget_Check_Dive_Down;
				}
			}else
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 180)
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(iTarget->GetGameObjectType() == EObjectTypesGroundUnit || iTarget->GetGameObjectType() == EObjectTypesArmouredGroundUnit)
					{
						iCombatStatus = ECombat_burst_bomb;
					}
					else if(iTarget->GetGameObjectType() != EObjectTypesGroundUnit && iTarget->GetGameObjectType() != EObjectTypesArmouredGroundUnit)
					{
						iCombatStatus = ECombat_accurate_bomb;
					}
					iFlyTowardsTarget = EFlyTowardsTarget_Check_Dive_Down;
				}
			}
			break;
		}
	}
}

void CFighterAI::SetFighterAIToRetreatToBase()
{
	iPlaneStatus = EPlaneStatus_retreat;
	iRetreatStatus = ERetreat_return_to_base;
}

void CFighterAI::SetTargetToAttack(CGameObject* aTarget)
{
	iTarget = aTarget;
}
