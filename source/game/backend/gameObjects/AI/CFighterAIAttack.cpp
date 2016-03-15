/*
 * FighterAIAttack.cpp
 *
 *  Created on: 2011/06/15
 *      Author: Stephen
 */

#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CWeaponBomb.h"
#include "includes/game/backend/basicClasses/CShip.h"
#include "includes/game/CFighterPilotThePacificWar.h"

TBool CFighterAI::AdjustToSafteyHeight()
{
	//ai will stop attacking the plane when it is either flying too fast or under the safety line
	TBool lBelowSafetyHeight = false;
	TBool lTooFast = false;

	if((iFighter->GetCurrentPosition().iY).GetIntInBaseInt() <= FIGHTER_AI_SAFTEY_HEIGHT)
	{
		lBelowSafetyHeight = true;
	}else if((iFighter->GetCurrentPosition().iY).GetIntInBaseInt() > FIGHTER_AI_SAFTEY_HEIGHT)
	{
		lBelowSafetyHeight = false;
	}

	TInt lAngle = *iFighter->GetFlyingAngel();
	if((lAngle >= 225 && lAngle <= 315) && (iFighter->GetCurrentAbsoluteSpeed()).GetIntInBaseInt() >= FIGHTER_AI_DANGER_FLYING_SPEED)
	{
		if((iFighter->GetCurrentPosition().iY).GetIntInBaseInt() <= FIGHTER_AI_VERTICAL_DIVE_SAFTEY_HEIGHT)
		{
			lTooFast = true;
		}else if((iFighter->GetCurrentPosition().iY ).GetIntInBaseInt() > FIGHTER_AI_VERTICAL_DIVE_SAFTEY_HEIGHT || (*iFighter->GetFlyingAngel() >= 15 && *iFighter->GetFlyingAngel() <= 165 && (iFighter->GetCurrentAbsoluteSpeed()).GetIntInBaseInt() <= FIGHTER_AI_DANGER_FLYING_SPEED))
		{
			lTooFast = false;
		}
	}

	if(lTooFast || lBelowSafetyHeight)
	{
		iAttackPlaneStatus = EAttackPlaneStatus_adjust_saftey_height;
		FlyAtAngle(90, FIGHTER_AI_MANEUVER_SMOOTHNESS);
		return false;
	}else
	{
		iAttackPlaneStatus = EAttackPlaneStatus_attack_player;
		return true;
	}
}

void CFighterAI::AvoidBigPlaneHitBox()
{
	//go through all objects that exists on the map and find one that is a big plane
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesBigPlane);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TRectIntFloat lBigPlaneArea;
	TInt lPlaneAngle = *iFighter->GetFlyingAngel();

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lFighterConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject != iTarget)
		{
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
			TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();
			lBigPlaneArea = lCurrentGameObject->GetHitBox()->GetFastIntersectionRect();
			//make the hit box larger so that the ai will have enough time to tilt away from the big plane
			if((lPlaneAngle >= 225 && lPlaneAngle <= 315) || (lPlaneAngle >= 45 && lPlaneAngle <= 135))
			{
				lBigPlaneArea.iTl.iX -= lCurrentGameObject->GetWidth() * 2;
				lBigPlaneArea.iTl.iY += lCurrentGameObject->GetHeight() * 2;
				lBigPlaneArea.iBr.iX += lCurrentGameObject->GetWidth() * 2;
				lBigPlaneArea.iBr.iY -= lCurrentGameObject->GetHeight() * 2;
			}else
			{
				lBigPlaneArea.iTl.iX -= lCurrentGameObject->GetWidth();
				lBigPlaneArea.iTl.iY += lCurrentGameObject->GetHeight() * 2;
				lBigPlaneArea.iBr.iX += lCurrentGameObject->GetWidth();
				lBigPlaneArea.iBr.iY -= lCurrentGameObject->GetHeight();
			}
			//avoid crashing into the other big planes when shooting down itarget
			if(lBigPlaneArea.Intersects(iFighter->GetHitBox()->GetFastIntersectionRect()))
			{
				iBigPlane = lCurrentGameObject;
				iPreviousCombatStatus = ECombat_attack;
				iCombatStatus = ECombat_avoid_plane;
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CFighterAI::AvoidTargetCollision()
{
	switch(iAvoidCollisionStatus)
	{
		//ai will tilt according to its position and angle away from the possible colliding plane
		case EAvoidCollisionStatus_check_angle:
		{
			TInt lFighterAngle = *iFighter->GetFlyingAngel();
			TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
			TInt lTargetHeight = iTarget->GetCurrentPosition().iY.GetIntInBaseInt();
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle >= 0 && lFighterAngle < 90)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}
				}else if(lFighterAngle >= 90 && lFighterAngle < 180)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}
				}else if(lFighterAngle >= 180 && lFighterAngle < 270)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}
				}else if(lFighterAngle >= 270 && lFighterAngle < 360)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}
				}
			}else if(iFighter->IsReflected())
			{
				if(lFighterAngle >= 0 && lFighterAngle < 90)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}
				}else if(lFighterAngle >= 90 && lFighterAngle < 180)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}
				}else if(lFighterAngle >= 180 && lFighterAngle < 270)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}
				}else if(lFighterAngle >= 270 && lFighterAngle < 360)
				{
					if(lFighterHeight >= lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_up;
					}else if(lFighterHeight < lTargetHeight)
					{
						iAvoidCollisionStatus = EAvoidCollisionStatus_tilt_down;
					}
				}
			}
			break;
		}
			//ai will keep tilting up until it is outside of the target's hit box
		case EAvoidCollisionStatus_tilt_up:
		{
			iFighter->IncreaseFlyingAngle();
			TRectIntFloat lFighterRectangle = iFighter->GetHitBox()->GetFastIntersectionRect();
			TRectIntFloat lBigPlaneRectangle = iBigPlane->GetHitBox()->GetFastIntersectionRect();
			if(!lFighterRectangle.Intersects(lBigPlaneRectangle))
			{
				iCombatStatus = iPreviousCombatStatus;
				iAvoidCollisionStatus = EAvoidCollisionStatus_check_angle;
			}
			break;
		}
			//ai will keep tilting down until it is outside of the target's hit box
		case EAvoidCollisionStatus_tilt_down:
		{
			iFighter->DecreaseFlyingAngle();
			TRectIntFloat lFighterRectangle = iFighter->GetHitBox()->GetFastIntersectionRect();
			TRectIntFloat lBigPlaneRectangle = iBigPlane->GetHitBox()->GetFastIntersectionRect();
			if(!lFighterRectangle.Intersects(lBigPlaneRectangle))
			{
				iCombatStatus = iPreviousCombatStatus;
				iAvoidCollisionStatus = EAvoidCollisionStatus_check_angle;
			}
			break;
		}
	}
}

void CFighterAI::AvoidTargetHitBox()
{
	//go through all objects that exists on the map and find one that is a big plane
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesBigPlane);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TRectIntFloat lBigPlaneArea;
	TInt lPlaneAngle = *iFighter->GetFlyingAngel();

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lFighterConflictSide != lCurrentGameObject->GetConflictSide())
		{
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
			TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();
			iBigPlane = lCurrentGameObject;
			lBigPlaneArea = iBigPlane->GetHitBox()->GetFastIntersectionRect();
			//make the hit box larger so that the ai will have enough time to tilt away from the big plane
			if((lPlaneAngle >= 225 && lPlaneAngle <= 315) || (lPlaneAngle >= 45 && lPlaneAngle <= 135))
			{
				lBigPlaneArea.iTl.iX -= iBigPlane->GetWidth() * 2;
				lBigPlaneArea.iTl.iY += iBigPlane->GetHeight() * 2;
				lBigPlaneArea.iBr.iX += iBigPlane->GetWidth() * 2;
				lBigPlaneArea.iBr.iY -= iBigPlane->GetHeight() * 2;
			}else
			{
				lBigPlaneArea.iTl.iX -= iBigPlane->GetWidth();
				lBigPlaneArea.iTl.iY += iBigPlane->GetHeight() * 2;
				lBigPlaneArea.iBr.iX += iBigPlane->GetWidth();
				lBigPlaneArea.iBr.iY -= iBigPlane->GetHeight();
			}

			//avoid crashing into the big plane when shooting them down
			if(iFighter->GetHitBox()->GetFastIntersectionRect().Intersects(lBigPlaneArea))
			{
				iPreviousCombatStatus = ECombat_attack;
				iCombatStatus = ECombat_avoid_plane;
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CFighterAI::ContinueAfterDogFight()
{
	//look for target based on equipped weapon
	if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifier1000kgBomb)
	{
		TargetSpecificTarget(EObjectTypesBuilding);
	}else if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifier100kgBomb)
	{
		TargetSpecificTarget((EObjectTypesArmouredGroundUnit | EObjectTypesGroundUnit));
	}else if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifierTorpedo)
	{
		TargetSpecificTarget(EObjectTypesShip);
	}
	else
	{
		TargetSpecificTarget(EObjectTypesPlane | EObjectTypesBigPlane);
	}

	if(!iTarget)
	{
		SetFighterAIToRetreatToBase();
		return;
	}

	//continue attacking using equipped weapon
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	TInt lFighterAngle = *iFighter->GetFlyingAngel();
	switch(iContinueFromDogFightStatus)
	{
		case EContinueFromDogFightStatus_face_target:
		{
			if(!iFighter->IsReflected())
			{
				if(iFighter->GetCurrentPosition().iX > iTarget->GetCurrentPosition().iX)
				{
					iFighter->TurnLeft();
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_check_height;
				}else if(iFighter->GetCurrentPosition().iX < iTarget->GetCurrentPosition().iX)
				{
					iFighter->TurnRight();
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_check_height;
				}
			}else if(iFighter->IsReflected())
			{
				if(iFighter->GetCurrentPosition().iX > iTarget->GetCurrentPosition().iX)
				{
					iFighter->TurnLeft();
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_check_height;
				}else if(iFighter->GetCurrentPosition().iX < iTarget->GetCurrentPosition().iX)
				{
					iFighter->TurnRight();
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_check_height;
				}
			}
			break;
		}
		case EContinueFromDogFightStatus_check_height:
		{
			TIntFloat lAwayFromTargetDistance = CMath::Abs(iFighter->GetCurrentPosition().iX - iTarget->GetCurrentPosition().iX);
			if(lAwayFromTargetDistance > FIGHTER_AI_BOMBING_CLOSE_DISTANCE)
			{
				if(lFighterHeight > FIGHTER_AI_BOMBING_DIVE_HEIGHT)
				{
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_climb_down;
				}else if(lFighterHeight < FIGHTER_AI_BOMBING_DIVE_HEIGHT)
				{
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_climb_up;
				}
			}else if(lAwayFromTargetDistance < FIGHTER_AI_BOMBING_CLOSE_DISTANCE)
			{
				if(lFighterHeight > FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT)
				{
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_short_climb_down;
				}else if(lFighterHeight < FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT)
				{
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_short_climb_up;
				}
			}
			break;
		}
		case EContinueFromDogFightStatus_climb_up:
		{
			if(lFighterHeight < FIGHTER_AI_BOMBING_DIVE_HEIGHT)
			{
				if(!iFighter->IsReflected())
				{
					FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
				}else if(iFighter->IsReflected())
				{
					FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
				}
			}else if(lFighterHeight >= FIGHTER_AI_BOMBING_DIVE_HEIGHT)
			{
				iContinueFromDogFightStatus = EContinueFromDogFightStatus_fly_horizontally;
			}
			break;
		}
		case EContinueFromDogFightStatus_climb_down:
		{
			if(lFighterHeight > FIGHTER_AI_BOMBING_DIVE_HEIGHT)
			{
				if(!iFighter->IsReflected())
				{
					FlyAtAngle(225, FIGHTER_AI_TILT_DELAY);
				}else if(iFighter->IsReflected())
				{
					FlyAtAngle(315, FIGHTER_AI_TILT_DELAY);
				}
			}else if(lFighterHeight <= FIGHTER_AI_BOMBING_DIVE_HEIGHT)
			{
				iContinueFromDogFightStatus = EContinueFromDogFightStatus_fly_horizontally;
			}
			break;
		}
		case EContinueFromDogFightStatus_short_climb_up:
		{
			if(lFighterHeight < FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT)
			{
				if(!iFighter->IsReflected())
				{
					FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
				}else if(iFighter->IsReflected())
				{
					FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
				}
			}else if(lFighterHeight >= FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT)
			{
				iContinueFromDogFightStatus = EContinueFromDogFightStatus_fly_horizontally;
			}
			break;
		}
		case EContinueFromDogFightStatus_short_climb_down:
		{
			if(lFighterHeight > FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT)
			{
				if(!iFighter->IsReflected())
				{
					FlyAtAngle(225, FIGHTER_AI_TILT_DELAY);
				}else if(iFighter->IsReflected())
				{
					FlyAtAngle(315, FIGHTER_AI_TILT_DELAY);
				}
			}else if(lFighterHeight <= FIGHTER_AI_BOMBING_CLOSE_DIVE_HEIGHT)
			{
				iContinueFromDogFightStatus = EContinueFromDogFightStatus_fly_horizontally;
			}
			break;
		}
		case EContinueFromDogFightStatus_fly_horizontally:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 180)
				{
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				}else if(lFighterAngle == 180)
				{
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_resume_attack;
				}
			}else if(iFighter->IsReflected())
			{
				if(lFighterAngle != 0)
				{
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				}else if(lFighterAngle == 0)
				{
					iContinueFromDogFightStatus = EContinueFromDogFightStatus_resume_attack;
				}
			}
			break;
		}
		case EContinueFromDogFightStatus_resume_attack:
		{
			if(iTarget->GetGameObjectType() & (EObjectTypesBuilding | EObjectTypesArmouredGroundUnit | EObjectTypesGroundUnit))
			{
				iCombatStatus = ECombat_dive_to_target;
				iContinueFromDogFightStatus = EContinueFromDogFightStatus_face_target;
			}else if(iTarget->GetGameObjectType() & EObjectTypesShip)
			{
				iCombatStatus = ECombat_dive_to_sea_level;
				iContinueFromDogFightStatus = EContinueFromDogFightStatus_face_target;
			}
			break;
		}
	}
}

void CFighterAI::EnemyTailGating()
{
	if((iFighter->DistanceBetweenGameObjects(iTarget)).GetIntInBaseInt() <= FIGHTER_AI_TAILGATE_DISTANCE)
	{
		CAircraft* lEnemyPlane = static_cast<CAircraft*> (iTarget);
		TInt lTargetAngle = *lEnemyPlane->GetFlyingAngel();
		TInt lFighterAngle = *iFighter->GetFlyingAngel();
		TInt lTargetXPosition = lEnemyPlane->GetCurrentPosition().iX.GetIntInBaseInt();
		TInt lTargetYPosition = lEnemyPlane->GetCurrentPosition().iY.GetIntInBaseInt();
		TInt lFighterXPosition = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
		TInt lFighterYPosition = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();

		if(lTargetAngle == lFighterAngle)
		{
			if(!iFighter->IsReflected())
			{
				//1 to 89 case
				if(lFighterAngle > 0 && lFighterAngle < 90)
				{
					if(lTargetXPosition < lFighterXPosition)// && lTargetYPosition < lFighterYPosition)
					{
						iFighter->TurnRight();
					}
				}
				//91 to 179 case
				else if(lFighterAngle > 90 && lFighterAngle < 180)
				{
					if(lTargetXPosition > lFighterXPosition)// && lTargetYPosition < lFighterYPosition)
					{
						iFighter->TurnRight();
					}
				}
				//181 to 269 case
				else if(lFighterAngle > 180 && lFighterAngle < 270)
				{
					if(lTargetXPosition > lFighterXPosition)// && lTargetYPosition > lFighterYPosition)
					{
						iFighter->TurnRight();
					}
				}
				//271 to 259 case
				else if(lFighterAngle > 270 && lFighterAngle < 360)
				{
					if(lTargetXPosition < lFighterXPosition)// && lTargetYPosition > lFighterYPosition)
					{
						iFighter->TurnRight();
					}
				}
			}else if(iFighter->IsReflected())
			{
				//1 to 89 case
				if(lFighterAngle > 0 && lFighterAngle < 90)
				{
					if(lTargetXPosition < lFighterXPosition)// && lTargetYPosition < lFighterYPosition)
					{
						iFighter->TurnLeft();
					}
				}
				//91 to 179 case
				else if(lFighterAngle > 90 && lFighterAngle < 180)
				{
					if(lTargetXPosition > lFighterXPosition)// && lTargetYPosition < lFighterYPosition)
					{
						iFighter->TurnLeft();
					}
				}
				//181 to 269 case
				else if(lFighterAngle > 180 && lFighterAngle < 270)
				{
					if(lTargetXPosition > lFighterXPosition)// && lTargetYPosition > lFighterYPosition)
					{
						iFighter->TurnLeft();
					}
				}
				//271 to 259 case
				else if(lFighterAngle > 270 && lFighterAngle < 360)
				{
					if(lTargetXPosition < lFighterXPosition)// && lTargetYPosition > lFighterYPosition)
					{
						iFighter->TurnLeft();
					}
				}
			}
		}
	}
}

void CFighterAI::GotHitByEnemyPlane()
{
	if(iCombatStatus != ECombat_avoid_plane //can't do anything in this case, since the plane might crash into a big plane
		&& iCombatStatus != Ecombat_takeOff) //can't do anything because startup is very touchy and if it is interrupted the plane will just sit around doing nothing
	{
		TargetSpecificTarget(EObjectTypesPlane);
		iCombatStatus = ECombat_attack;
	}
}

void CFighterAI::ManeuverCheckHeight()
{
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	//if the ai flies lower than 300 it will climb back up to 400
	if(lFighterHeight <= FIGHTER_AI_BOMBING_HEIGHT - 100)
	{
		iPreviousCombatStatus = ECombat_attack;
		iCombatStatus = ECombat_avoid_flying_too_low;
	}
}

void CFighterAI::RetreatAvoidBigPlaneHitBox()
{
	//go through all objects that exists on the map and find one that is a big plane
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesBigPlane);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TRectIntFloat lBigPlaneArea;

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lFighterConflictSide != lCurrentGameObject->GetConflictSide())
		{
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
			TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();
			iBigPlane = lCurrentGameObject;
			lBigPlaneArea = iBigPlane->GetHitBox()->GetFastIntersectionRect();
			lBigPlaneArea.iTl.iX -= iBigPlane->GetWidth();
			lBigPlaneArea.iTl.iY += iBigPlane->GetHeight();
			lBigPlaneArea.iBr.iX += iBigPlane->GetWidth();
			lBigPlaneArea.iBr.iY -= iBigPlane->GetHeight();

			//avoid crashing into the other big planes when shooting down itarget
			if(iFighter->GetHitBox()->GetFastIntersectionRect().Intersects(lBigPlaneArea))
			{
				//only avoid the big plane only if it is above this height
				if(iBigPlane->GetCurrentPosition().iY > FIGHTER_AI_BIG_PLANE_CRASHED_HEIGHT)
				{
					iPreviousRetreatStatus = ERetreat_return_to_base;
					iRetreatStatus = ERetreat_avoid_plane;
				}
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CFighterAI::RetreatAvoidOtherPlaneCollision()
{
	TInt lFighterAngle = *iFighter->GetFlyingAngel();
	switch(iRetreatAvoidOtherPlaneCollisionStatus)
	{
		//ai will tilt according to its position and angle from the possible colliding plane during retreating to base
		case ERetreatAvoidOtherPlaneCollisionStatus_check_angle:
		{
			TInt lFighterAngle = *iFighter->GetFlyingAngel();
			TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
			TInt lBigPlaneHeight = iBigPlane->GetCurrentPosition().iY.GetIntInBaseInt();
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle >= 0 && lFighterAngle < 90)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}
				}else if(lFighterAngle >= 90 && lFighterAngle < 180)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}
				}else if(lFighterAngle >= 180 && lFighterAngle < 270)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}
				}else if(lFighterAngle >= 270 && lFighterAngle < 360)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}
				}
			}else if(iFighter->IsReflected())
			{
				if(lFighterAngle >= 0 && lFighterAngle < 90)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}
				}else if(lFighterAngle >= 90 && lFighterAngle < 180)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}
				}else if(lFighterAngle >= 180 && lFighterAngle < 270)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}
				}else if(lFighterAngle >= 270 && lFighterAngle < 360)
				{
					if(lFighterHeight >= lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_up;
					}else if(lFighterHeight < lBigPlaneHeight)
					{
						iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_tilt_down;
					}
				}
			}
			break;
		}
			//ai will keep tilting up until it is outside of the target's hit box
		case ERetreatAvoidOtherPlaneCollisionStatus_tilt_up:
		{
			iFighter->IncreaseFlyingAngle();
			TRectIntFloat lFighterRectangle = iFighter->GetHitBox()->GetFastIntersectionRect();
			TRectIntFloat lBigPlaneRectangle = iBigPlane->GetHitBox()->GetFastIntersectionRect();
			if(!lFighterRectangle.Intersects(lBigPlaneRectangle))
			{
				iRetreatStatus = iPreviousRetreatStatus;
				iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_check_angle;
			}
			break;
		}
			//ai will keep tilting down until it is outside of the target's hit box
		case ERetreatAvoidOtherPlaneCollisionStatus_tilt_down:
		{
			iFighter->DecreaseFlyingAngle();
			TRectIntFloat lFighterRectangle = iFighter->GetHitBox()->GetFastIntersectionRect();
			TRectIntFloat lBigPlaneRectangle = iBigPlane->GetHitBox()->GetFastIntersectionRect();
			if(!lFighterRectangle.Intersects(lBigPlaneRectangle))
			{
				iRetreatStatus = iPreviousRetreatStatus;
				iRetreatAvoidOtherPlaneCollisionStatus = ERetreatAvoidOtherPlaneCollisionStatus_check_angle;
			}
			break;
		}
	}
}

void CFighterAI::SetTargetAttackType(TUint aTargetPriorities)
{
	iTargetPriority = aTargetPriorities;
}

void CFighterAI::ShortestPathShootDown()
{
	ManeuverCheckHeight();
	AdjustPlaneSpeed(TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED));
	AvoidBigPlaneHitBox();

	if(!iTarget || !(iTarget->IsAlive()))
	{
		iTarget = NULL;

		//if plane has still 80% of health, continue to fight otherwise go home
		if((iFighter->GetHealth() * 100) > (iFighter->GetMaxHealth() * 80))
		{
			TargetSpecificTarget(GAMEOBJECT_TYPE_GROUP_PLANES);

			//check if target is plane
			if(!(iTarget && (iTarget->GetGameObjectType() & (GAMEOBJECT_TYPE_GROUP_PLANES))))
			{
				iTarget = NULL; //can only attack planes from this AI state
			}
		}

		if(!iTarget)
		{
			iPlaneStatus = EPlaneStatus_retreat;
			iRetreatStatus = ERetreat_return_to_base;
		}

		return; //nothing else to do
	}

	if(AdjustToSafteyHeight())
	{
		if(iTarget && (iTarget->GetGameObjectType() & (EObjectTypesBigPlane | EObjectTypesPlane)))
		{
			//calculate distance to find out if we are close enough  to attack, otherwise manouver
			iDistanceToTarget = iFighter->DistanceBetweenGameObjects(iTarget).GetIntInBaseInt();
			if(iDistanceToTarget <= FIGHTER_AI_ATTACK_DISTANCE)
			{
				AvoidTargetHitBox();
				AvoidBigPlaneHitBox();
				if(iChaseDownTiltDelay == FIGHTER_AI_CHASE_DOWN_TILT_DELAY)
				{
					iChaseDownTiltDelay = 0;
					CAircraft* lEnemyPlane = static_cast<CAircraft*> (iTarget);
					//close enough, calculate angle to fly at
					TInt lFlyAtAngle = CMath::AngleFromOriginBetweenPoints(iFighter->GetPrimaryWeapon()->GetRelativeWeaponPosition(), lEnemyPlane->GetPrimaryWeapon()->GetRelativeWeaponPosition());
					FlyAtAngle(lFlyAtAngle, FIGHTER_AI_MANEUVER_SMOOTHNESS);
					iFighter->Fire1();

					//ai will turn when it is being followed by another plane
					EnemyTailGating();
				}
				iChaseDownTiltDelay++;
			}
			//face target and maneuver towards it
			else if(iDistanceToTarget > FIGHTER_AI_ATTACK_DISTANCE)
			{
				TInt lAngle = *iFighter->GetFlyingAngel();
				TInt lFighterPosition = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
				TInt lTargetPosition = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
				if(!iFighter->IsReflected())
				{
					if(lFighterPosition > lTargetPosition && lAngle >= 90 && lAngle < 270)
					{
						iFighter->TurnLeft();
					}else if(lFighterPosition > (lTargetPosition && (lAngle >= 0 && lAngle < 90)) || (lAngle >= 270 && lAngle < 360))
					{
						iFighter->TurnRight();
					}else if(lFighterPosition < lTargetPosition && lAngle >= 90 && lAngle < 270)
					{
						iFighter->TurnRight();
					}else if(lFighterPosition < (lTargetPosition && (lAngle >= 0 && lAngle < 90)) || (lAngle >= 270 && lAngle < 360))
					{
						iFighter->TurnLeft();
					}
				}else if(iFighter->IsReflected())
				{
					if(lFighterPosition > (lTargetPosition && (lAngle >= 0 && lAngle < 90)) || (lAngle >= 270 && lAngle < 360))
					{
						iFighter->TurnLeft();
					}else if(lFighterPosition > lTargetPosition && lAngle >= 90 && lAngle < 270)
					{
						iFighter->TurnRight();
					}else if(lFighterPosition < (lTargetPosition && (lAngle >= 0 && lAngle < 90)) || (lAngle >= 270 && lAngle < 360))
					{
						iFighter->TurnRight();
					}else if(lFighterPosition < lTargetPosition && lAngle >= 90 && lAngle < 270)
					{
						iFighter->TurnLeft();
					}
				}
				iPreviousCombatStatus = ECombat_attack;
				iManeuverAngle = FIGHTER_AI_BOMBING_MANEUVER_ANGLE;
				iCombatStatus = ECombat_maneuver;
			}
		}
		//retreat when there are no targets left in the game
		else if(!iTarget)
		{
			iPlaneStatus = EPlaneStatus_retreat;
			iRetreatStatus = ERetreat_return_to_base;
		}
	}
}

void CFighterAI::TargetSpecificTarget(TUint aTargetTypes)
{
	//go through all objects that exists on the map and find one that is closest
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(aTargetTypes);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TInt lCurrentShortestPossibleDistance = -1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
	CGameObject* lCurrentBestTarget = NULL;
	iTarget = NULL; //might want to reset in case nothing exists
	TBool lCanExpandSearch = true;

	//need to check if we already included all possible targets in our search
	if(aTargetTypes == FIGHTER_AI_ALL_TARGET_TYPES || aTargetTypes == FIGHTER_AI_ALL_TARGET_BUT_SHIP)
		lCanExpandSearch = false;

	//need to perform check if we have any secondary weapon left, if none we can only attack planes
	if(!iFighter->Fire2HasAmmunitionLeft())
		aTargetTypes = GAMEOBJECT_TYPE_GROUP_PLANES;

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lFighterConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive()
			&& (!lCurrentGameObject->GetIsMoveableGameObject() || !(static_cast<CMoveableGameObject*> (lCurrentGameObject)->GetIsLeavingMap())) //make sure new target is not leaving the map
			)
		{
			//check if the object is not too close to drop bombs on it properly
			TPointIntFloat lTargetLocation = lCurrentGameObject->GetCurrentPosition();
			TPointIntFloat lBomberLocation = iFighter->GetCurrentPosition();
			TInt lDistanceBetweenTargetLocation = CMath::Abs(lBomberLocation.iX - lTargetLocation.iX).GetIntInBaseInt();

			if(lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1)//is the target better than the previous one that we found
			{
				//could be possible target, but if Ship make special case that we can actually attack it
				if(lCurrentGameObject->GetGameObjectType() & GAMEOBJECT_TYPE_GROUP_SHIP)
				{
					if(GetIfShipIsBlockedOnLeft(static_cast<CShip*>(lCurrentGameObject)) && GetIfShipIsBlockedOnRight(static_cast<CShip*>(lCurrentGameObject)))
					{
						iTarget = NULL;
					}
					else if(!GetIfShipIsBlockedOnLeft(static_cast<CShip*>(lCurrentGameObject)) || !GetIfShipIsBlockedOnRight(static_cast<CShip*>(lCurrentGameObject)))
					{
						lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
						lCurrentBestTarget = lCurrentGameObject;
					}
				}
				else if(lCurrentGameObject->GetGameObjectType() != GAMEOBJECT_TYPE_GROUP_SHIP)
				{
					lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
					lCurrentBestTarget = lCurrentGameObject;
				}
			}
		}
	}
	//clean up
	delete lGameObjects;

	//check if we found a valid target
	if(lCurrentBestTarget)
	{
		iTarget = lCurrentBestTarget;
	}
	else if(lCanExpandSearch)
	{
		//now expand search to look for all target types
		if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() != EWeaponIdentifierTorpedo)
		{
			TargetSpecificTarget(FIGHTER_AI_ALL_TARGET_BUT_SHIP);
		}
		else if(iFighter->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponIdentifierTorpedo)
		{
			TargetSpecificTarget(FIGHTER_AI_ALL_TARGET_TYPES);
		}
	}
}
