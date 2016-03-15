/*
 * FighterAIShipBomb.cpp
 *
 *  Created on: 2011/07/04
 *      Author: Stephen
 */

#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/game/backend/basicClasses/CShip.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CWeaponBomb.h"
#include "includes/game/CFighterPilotThePacificWar.h"

void CFighterAI::AvoidShipCollision()
{
	TInt lFighterAngle = *iFighter->GetFlyingAngel();
	TInt lFighterLocation = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	TInt lShipHeight = iShip->GetHeight().GetIntInBaseInt();
	TInt lShipLocation = iShip->GetCurrentPosition().iX.GetIntInBaseInt();
	TInt lShipWidth = iShip->GetWidth().GetIntInBaseInt();
	TInt lTargetHeight = iTarget->GetHeight().GetIntInBaseInt();

	//check if problem ship is leaving map
	if(iShip && (static_cast<CMoveableGameObject*>(iShip)->GetIsLeavingMap()))
	{
		iShip = NULL;
		iShipAvoidCollision = EShipAvoidCollision_adjust_to_targeted_ship_height;
	}
	
	switch(iShipAvoidCollision)
	{
		//fly up at 45 degrees to avoid hitting iShip directly
		case EShipAvoidCollision_tilt_up:
		{
			if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() != FIGHTER_AI_MAX_ENGINE_SPEED)
			{
				iFighter->IncreaseSpeed();
			}
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle == 135)
				{
					iShipAvoidCollision = EShipAvoidCollision_fly_to_height;
				}
				FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
			}
			else if(iFighter->IsReflected())
			{
				if(lFighterAngle == 45)
				{
					iShipAvoidCollision = EShipAvoidCollision_fly_to_height;
				}
				FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
			}
			break;
		}
		//climb to a height that is the same as the height of iShip plus the torpedo height
		case EShipAvoidCollision_fly_to_height:
		{
			if(lFighterHeight >= FIGHTER_AI_TORPEDO_DROP_HEIGHT + lShipHeight)
			{
				iShipAvoidCollision = EShipAvoidCollision_fly_horizontally;
			}
			break;
		}
		//fly horizontally so that the ai will not fly up infinitely
		case EShipAvoidCollision_fly_horizontally:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 180)
				{
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				}
				else 
				{
					iShipAvoidCollision = EShipAvoidCollision_fly_to_end_of_ship;
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
					iShipAvoidCollision = EShipAvoidCollision_fly_to_end_of_ship;
				}
			}
			break;
		}
		//fly horizontally until the ai is at the end of iShip
		case EShipAvoidCollision_fly_to_end_of_ship:
		{
			if(!iFighter->IsReflected())
			{
				if(!iTarget->IsReflected())
				{
					if(lFighterLocation <= lShipLocation)
					{
						iShipAvoidCollision = EShipAvoidCollision_adjust_to_targeted_ship_height;
					}
				}
				else if(iTarget->IsReflected())
				{
					if(lFighterLocation <= lShipLocation - lShipWidth)
					{
						iShipAvoidCollision = EShipAvoidCollision_adjust_to_targeted_ship_height;
					}
				}
			}
			else if(iFighter->IsReflected())
			{
				if(iTarget->IsReflected())
				{
					if(lFighterLocation >= lShipLocation)
					{
						iShipAvoidCollision = EShipAvoidCollision_adjust_to_targeted_ship_height;
					}
				}
				else if(!iTarget->IsReflected())
				{
					if(lFighterLocation >= lShipLocation + lShipWidth)
					{
						iShipAvoidCollision = EShipAvoidCollision_adjust_to_targeted_ship_height;
					}
				}
			}
			break;
		}
		//fly down so that it is flying down to the height of iTarget
		case EShipAvoidCollision_adjust_to_targeted_ship_height:
		{
			if(iFighter->IsReflected())
			{
				if(lFighterHeight > lTargetHeight)
				{
					if(lFighterAngle != 345)
					{
						FlyAtAngle(345, FIGHTER_AI_TILT_DELAY);	
					}
					else
					{
						iShipAvoidCollision = EShipAvoidCollision_check_height;
					}
				}
				else if(lFighterHeight < lTargetHeight)
				{
					if(lFighterAngle != 15)
					{
						FlyAtAngle(15, FIGHTER_AI_TILT_DELAY);
					}
					else
					{
						iShipAvoidCollision = EShipAvoidCollision_check_height;
					}
				}
			}
			else if(!iFighter->IsReflected())
			{
				if(lFighterHeight > lTargetHeight)
				{
					if(lFighterAngle != 195)
					{
						FlyAtAngle(195, FIGHTER_AI_TILT_DELAY);
					}
					else
					{
						iShipAvoidCollision = EShipAvoidCollision_check_height;
					}
				}
				else if(lFighterHeight < lTargetHeight)
				{
					if(lFighterAngle != 165)
					{
						FlyAtAngle(165, FIGHTER_AI_TILT_DELAY);
					}
					else
					{
						iShipAvoidCollision = EShipAvoidCollision_check_height;
					}
				}
			}
			break;
		}
		//check the height so that the ai will know when to start flying horizontally
		case EShipAvoidCollision_check_height:
		{
			if(lFighterAngle >= 15 && lFighterAngle <= 165)
			{
				if(lFighterHeight >= 2 * FIGHTER_AI_TORPEDO_DROP_HEIGHT + lTargetHeight)
				{
					iShipAvoidCollision = EShipAvoidCollision_fly_horizontally2;
				}
			}
			else if(lFighterAngle >= 195 && lFighterAngle <= 345)
			{
				if(lFighterHeight <= 2 * FIGHTER_AI_TORPEDO_DROP_HEIGHT + lTargetHeight)
				{
					iShipAvoidCollision = EShipAvoidCollision_fly_horizontally2;
				}
			}
			break;
		}
		//fly horizontally and jump to the case where the ai avoids iTarget and bombs on the other side
		case EShipAvoidCollision_fly_horizontally2:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 180)
				{
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				}
				else
				{
					iShipAvoidCollision = EShipAvoidCollision_tilt_up;
					iCombatStatus = iPreviousCombatStatus;
					iShipBombBehindTarget = EShipBombBehindTarget_slow_down_to_torpedo_speed;
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
					iShipAvoidCollision = EShipAvoidCollision_tilt_up;
					iCombatStatus = iPreviousCombatStatus;
					iShipBombBehindTarget = EShipBombBehindTarget_slow_down_to_torpedo_speed;
				}
			}
			break;
		}
	}
}

void CFighterAI::AvoidOtherShipHitBox()
{
	//go through all objects that exists on the map and find one that is a ship
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesShip);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TRectIntFloat lShipArea;

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lFighterConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject != iTarget)
		{
			lShipArea = lCurrentGameObject->GetHitBox()->GetFastIntersectionRect();
			//make the hit box larger so that the ai will have enough time to tilt away from the other ship
			lShipArea.iTl.iX -= iFighter->GetWidth();
			lShipArea.iTl.iY += iFighter->GetHeight();
			lShipArea.iBr.iX += iFighter->GetWidth();
								
			//avoid crashing into the ship of the target when performing torpedo drops
			if(iFighter->GetHitBox()->GetFastIntersectionRect().Intersects(lShipArea))
			{
				iShip = lCurrentGameObject;
				iPreviousCombatStatus = ECombat_ship_bomb;
				iCombatStatus = ECombat_avoid_ship;

				//clean up
				delete lGameObjects;
				return;
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CFighterAI::AdjustShipBombHeight()
{
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	TInt lFighterAngle = *iFighter->GetFlyingAngel();
	TInt lFighterPosition = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
	TInt lTargetPosition = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
	if(lFighterPosition > lTargetPosition)
	{
		iFighter->TurnLeft();
	}
	else if(lFighterPosition < lTargetPosition)
	{
		iFighter->TurnRight();
	}
	switch(iShipBombHeightAdjust)
	{
		case EShipBombHeightAdjust_fly_to_torpedo_height:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 210)
				{
					FlyAtAngle(210, FIGHTER_AI_TILT_DELAY);
				}
				else 
				{
					if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT + 10)
					{
						iShipBombHeightAdjust = EShipBombHeightAdjust_fly_horizontal;
					}
				}
			}
			else if(iFighter->IsReflected())
			{
				if(lFighterAngle != 330)
				{
					FlyAtAngle(330, FIGHTER_AI_TILT_DELAY);
				}
				else 
				{
					if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT + 10)
					{
						iShipBombHeightAdjust = EShipBombHeightAdjust_fly_horizontal;
					}
				}
			}
			break;
		}
		case EShipBombHeightAdjust_fly_horizontal:
		{
			if(!iFighter->IsReflected())
			{
				if(lFighterAngle != 180)
				{
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				}
				else
				{
					if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iShipBombHeightAdjust = EShipBombHeightAdjust_tilt_down;
					}
					else if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iShipBombStatus = EShipBombStatus_initiate_bombing;
						iShipBombHeightAdjust = EShipBombHeightAdjust_fly_to_torpedo_height;
						iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
					}
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
					if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iShipBombHeightAdjust = EShipBombHeightAdjust_tilt_down;
					}
					else if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iShipBombStatus = EShipBombStatus_initiate_bombing;
						iShipBombHeightAdjust = EShipBombHeightAdjust_fly_to_torpedo_height;
						iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
					}
				}
			}
			break;
		}
		case EShipBombHeightAdjust_tilt_up:
		{
			iFighter->IncreaseFlyingAngle();
			if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iShipBombStatus = EShipBombStatus_initiate_bombing;
				iShipBombHeightAdjust = EShipBombHeightAdjust_fly_to_torpedo_height;
				iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
			}
			else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iShipBombHeightAdjust = EShipBombHeightAdjust_tilt_down;
			}
			break;
		}
		case EShipBombHeightAdjust_tilt_down:
		{
			iFighter->DecreaseFlyingAngle();
			if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iFighter->IncreaseFlyingAngle();
				iShipBombStatus = EShipBombStatus_initiate_bombing;
				iShipBombHeightAdjust = EShipBombHeightAdjust_fly_to_torpedo_height;
				iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
			}
			else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iShipBombHeightAdjust = EShipBombHeightAdjust_tilt_up;
			}
			break;
		}
	}
}

void CFighterAI::CheckBeforeDiveToSeaLevel()
{
	switch(iShipDiveMethod)
	{
		case EShipDiveMethod_check_location:
		{
			TInt lTargetLocation = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
			TInt lTargetWidth = iTarget->GetWidth().GetIntInBaseInt();
			TInt lFighterLocation = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
			TInt lFighterWidth = iFighter->GetWidth().GetIntInBaseInt();
			TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
			TBool lShipBlockedOnLeft = GetIfShipIsBlockedOnLeft(static_cast<CShip*> (iTarget));
			TBool lShipBlockedOnRight = GetIfShipIsBlockedOnRight(static_cast<CShip*> (iTarget));
			
			
			if(!iTarget->IsReflected())
			{
				if(!iFighter->IsReflected())
				{
					//torpedo right side of ship
					if(!lShipBlockedOnRight)
					{
						if(lFighterLocation >= lTargetLocation + lTargetWidth + FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= lFighterLocation - (lTargetLocation + lTargetWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > lFighterLocation - (lTargetLocation + lTargetWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
					//torpedo left side of ship
					if(!lShipBlockedOnLeft)
					{
						if(lFighterLocation + lFighterWidth <= lTargetLocation - FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= lTargetLocation - (lFighterLocation + lFighterWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > lTargetLocation - (lFighterLocation + lFighterWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
				}
				else if(iFighter->IsReflected())
				{
					//torpedo left side of ship
					if(!lShipBlockedOnLeft)
					{
						if(lFighterLocation <= lTargetLocation - FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= lTargetLocation - lFighterLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > lTargetLocation - lFighterLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
					//torpedo right side of ship
					else if(!lShipBlockedOnRight)
					{
						if(lFighterLocation - lFighterWidth >= lTargetLocation + lTargetWidth + FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= (lFighterLocation - lFighterWidth) - (lTargetLocation + lTargetWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > (lFighterLocation - lFighterWidth) - (lTargetLocation + lTargetWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}

				}
			}
			else if(iTarget->IsReflected())
			{
				if(!iFighter->IsReflected())
				{
					//torpedo right side of ship
					if(!lShipBlockedOnRight)
					{
						if(lFighterLocation >= lTargetLocation + FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= lFighterLocation - lTargetLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > lFighterLocation - lTargetLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
					//torpedo left side of ship
					else if(!lShipBlockedOnLeft)
					{
						if(lFighterLocation + lFighterWidth <= lTargetLocation - lTargetWidth - FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= (lTargetLocation - lTargetWidth) - (lFighterLocation + lFighterWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > (lTargetLocation - lTargetWidth) - (lFighterLocation + lFighterWidth))
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
				}
				else if(iFighter->IsReflected())
				{
					//torpedo left side of ship
					if(!lShipBlockedOnLeft)
					{
						if(lFighterLocation <= lTargetLocation - lTargetWidth - FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= (lTargetLocation - lTargetWidth) - lFighterLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > (lTargetLocation - lTargetWidth) - lFighterLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
					//torpedo right side of ship
					else if(!lShipBlockedOnRight)
					{
						if(lFighterLocation - lFighterWidth >= lTargetLocation + FIGHTER_AI_SHIP_BOMB_DIVE_WITH_TURN_HEIGHT)
						{
							//check if ai is 2 times the height difference away from target
							if(lFighterHeight * 2 <= (lFighterLocation - lFighterWidth) - lTargetLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method1;
							}
							//ai has to do a half dive and then turn and continue the dive since its too close to the target
							else if(lFighterHeight * 2 > (lFighterLocation - lFighterWidth) - lTargetLocation)
							{
								iShipDiveMethod = EShipDiveMethod_dive_method2;
							}
						}
						else
						{
							iPreviousCombatStatus = ECombat_dive_to_sea_level;
							iManeuverAngle = FIGHTER_AI_IDLE_MANEUVER_ANGLE;
							iCombatStatus = ECombat_maneuver;
						}
					}
				}
			}
			break;
		}
		case EShipDiveMethod_dive_method1:
		{
			DiveToSeaLevel();
			break;
		}
		case EShipDiveMethod_dive_method2:
		{
			DiveToSeaLevelWithTurnInBetween();
			break;
		}
	}
}

TBool CFighterAI::CheckIfSaveToReleaseTorpedo()
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesShip);
	
	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lCurrentGameObject->IsAlive())
		{
			TInt lShipDistance = iFighter->DistanceBetweenGameObjects(lCurrentGameObject).GetIntInBaseInt();
			if(lShipDistance < FIGHTER_AI_SAFE_TORPEDO_DROP_DISTANCE)
			{
				//clean up
				delete lGameObjects;

				return false;
			}
		}
	}

	//clean up
	delete lGameObjects;

	return true;
}

TBool CFighterAI::GetIfShipIsBlockedOnLeft(CShip* aTargetShip)
{
	//check if a ship is infront of it or behind it
	TRectIntFloat lShipLeftRect = aTargetShip->GetHitBox()->GetFastIntersectionRect();
	//extend Rect by attack Distance
	lShipLeftRect.iTl.iX -= TIntFloat::Convert(FIGHTER_AI_TORPEDO_ATTACK_DISTANCE * 2);
	lShipLeftRect.iBr.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT); //only collide with object above water
	
	//now check if this Rect intersects with any other ship
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(GAMEOBJECT_TYPE_GROUP_SHIP);

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lCurrentGameObject != aTargetShip)
		{
			TRectIntFloat lCurrentShipRect = lCurrentGameObject->GetHitBox()->GetFastIntersectionRect();
			if(lCurrentShipRect.Intersects(lShipLeftRect))
			{
				//clean up
				delete lGameObjects;

				return true;
			}
		}
	}

	//clean up
	delete lGameObjects;

	return false;
}

TBool CFighterAI::GetIfShipIsBlockedOnRight(CShip* aTargetShip)
{
	//check if a ship is infront of it or behind it
	TRectIntFloat lShipRightRect = aTargetShip->GetHitBox()->GetFastIntersectionRect();
	//extend Rect by attack Distance
	lShipRightRect.iBr.iX += TIntFloat::Convert(FIGHTER_AI_TORPEDO_ATTACK_DISTANCE * 2);
	lShipRightRect.iBr.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT); //only collide with object above water
	
	//now check if this Rect intersects with any other ship
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(GAMEOBJECT_TYPE_GROUP_SHIP);

	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if(lCurrentGameObject != aTargetShip)
		{
			TRectIntFloat lCurrentShipRect = lCurrentGameObject->GetHitBox()->GetFastIntersectionRect();
			if(lCurrentShipRect.Intersects(lShipRightRect))
			{
				//clean up
				delete lGameObjects;

				return true;
			}
		}
	}

	//clean up
	delete lGameObjects;

	return false;
}

TBool CFighterAI::CheckIfFlyingOverSea()
{
	//go through all objects that exists on the map and find one that is a ship
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesFloor);
	TConflictSide lFighterConflictSide = iFighter->GetConflictSide();
	TInt lFighterPosition = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
	TInt lTargetPosition = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
	TInt lGameSeaPosition;
	TInt lGameSeaWidth;
	
	for(TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

		if((lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierWater))
		{
			lGameSeaPosition = lCurrentGameObject->GetCurrentPosition().iX.GetIntInBaseInt();
			lGameSeaWidth = lCurrentGameObject->GetWidth().GetIntInBaseInt();
			//avoid crashing into the ship of the target when performing torpedo drops
			if(lTargetPosition >= lGameSeaPosition && lTargetPosition <= lGameSeaPosition + lGameSeaWidth)
			{
				if(lFighterPosition >= lGameSeaPosition && lFighterPosition <= lGameSeaPosition + lGameSeaWidth)
				{
					//clean up
					delete lGameObjects;

					return true;
				}
			}
		}
	}

	//clean up
	delete lGameObjects;

	return false;
}

void CFighterAI::DiveToSeaLevel()
{
	CheckHealthStatus();
	AvoidOtherShipHitBox();
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	TInt lDistanceFromTarget = iFighter->DistanceBetweenGameObjects(iTarget).GetIntInBaseInt();
	switch(iFlyTowardsShip)
	{
		case EFlyTowardsShip_Check_Dive_Down:
		{
			if(iTarget)
			{
				iManeuverSmoothness = 0;
				CalculateManeuverDistanceTravel(FIGHTER_AI_IDLE_MANEUVER_ANGLE);
				//check what X distance we need to be away to start the dive
				TIntFloat lHeightDifference = iFighter->GetCurrentPosition().iY - FIGHTER_AI_TORPEDO_DROP_HEIGHT;
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
					iFlyTowardsShip = EFlyTowardsShip_Dive_Down;
				}
				else
				{
					iPreviousCombatStatus = ECombat_dive_to_sea_level;
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
		case EFlyTowardsShip_Dive_Down:
		{
			//slow down to bombing speed
			if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
			{
				iFighter->DecreaseSpeed();
				iFighter->DecreaseSpeed();
			}
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
			//check if we are already at the torpedo releasing height
			if(iShipBombLowHeight == 1)
			{
				if(FIGHTER_AI_SHIP_BOMB_LOW_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > lFighterHeight)
				{
					iFlyTowardsShip = EFlyTowardsShip_Fly_Horizontally;
				}
			}
			else if(iShipBombLowHeight == 0)
			{
				if(FIGHTER_AI_SHIP_BOMB_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > lFighterHeight)
				{
					iFlyTowardsShip = EFlyTowardsShip_Fly_Horizontally;
				}
			}
			break;
		}
		case EFlyTowardsShip_Fly_Horizontally:
		{
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 0)
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iCombatStatus = ECombat_ship_bomb;
						iFlyTowardsShip = EFlyTowardsShip_Check_Dive_Down;
					}
					else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iFlyTowardsShip = EFlyTowardsShip_decrease_flying_height;
					}
				}
			}
			else if(!iFighter->IsReflected())
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 180)
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iCombatStatus = ECombat_ship_bomb;
						iFlyTowardsShip = EFlyTowardsShip_Check_Dive_Down;
					}
					else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iFlyTowardsShip = EFlyTowardsShip_decrease_flying_height;
					}
				}
			}
			break;
		}
		case EFlyTowardsShip_decrease_flying_height:
		{
			if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iCombatStatus = ECombat_ship_bomb;
				iFlyTowardsShip = EFlyTowardsShip_Check_Dive_Down;
				break;
			}
			else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iFighter->DecreaseFlyingAngle();
				iFlyTowardsShip = EFlyTowardsShip_increase_flying_height;
			}
			break;
		}
		case EFlyTowardsShip_increase_flying_height:
		{
			iFighter->IncreaseFlyingAngle();
			if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iCombatStatus = ECombat_ship_bomb;
				iFlyTowardsShip = EFlyTowardsShip_Check_Dive_Down;
				break;
			}
			else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iFlyTowardsShip = EFlyTowardsShip_decrease_flying_height;
			}
			break;
		}
	}
}

void CFighterAI::DiveToSeaLevelWithTurnInBetween()
{
	CheckHealthStatus();
	AvoidOtherShipHitBox();
	TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
	switch(iFlyTowardsShipMethod2)
	{
		case EFlyTowardsShipMethod2_check_dive_down:
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
					//dive down away from the target since it is too close
					if(iFighter->GetCurrentPosition().iX <= iTarget->GetCurrentPosition().iX + iTarget->GetWidth() / 2)
					{
						iFighter->TurnLeft();
					}else if(iFighter->GetCurrentPosition().iX > iTarget->GetCurrentPosition().iX + iTarget->GetWidth() / 2)
					{
						iFighter->TurnRight();
					}
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_dive_down;
				}
				else
				{
					iPreviousCombatStatus = ECombat_dive_to_sea_level;
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
		case EFlyTowardsShipMethod2_dive_down:
		{
			//slow down to bombing speed
			if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
			{
				iFighter->DecreaseSpeed();
				iFighter->DecreaseSpeed();
			}
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 315)
				{
					FlyAtAngle(315, FIGHTER_AI_TILT_DELAY);
				}
			}
			else if(!iFighter->IsReflected())
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 225)
				{
					FlyAtAngle(225, FIGHTER_AI_TILT_DELAY);
				}
			}
			//check if we are already at the bombing height
			if(iShipBombLowHeight == 1)
			{
				if(FIGHTER_AI_BOMBING_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > lFighterHeight)
				{
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_fly_horizontally;
				}
			}
			else if(iShipBombLowHeight == 0)
			{
				if(FIGHTER_AI_BOMBING_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > lFighterHeight)
				{
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_fly_horizontally;
				}
			}
			break;
		}
		case EFlyTowardsShipMethod2_fly_horizontally:
		{
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 0)
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(lFighterHeight <= FIGHTER_AI_BOMBING_HEIGHT)
					{
						iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_turn_to_target;
					}
				}
			}
			else if(!iFighter->IsReflected())
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 180)
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(lFighterHeight <= FIGHTER_AI_BOMBING_HEIGHT)
					{
						iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_turn_to_target;
					}
				}
			}
			break;
		}
		case EFlyTowardsShipMethod2_turn_to_target:
		{
			if(!iFighter->IsReflected())
			{
				if(iFighter->GetCurrentPosition().iX < iTarget->GetCurrentPosition().iX)
				{
					iFighter->TurnRight();
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_dive_down2;
				}
			}
			else if(iFighter->IsReflected())
			{
				if(iFighter->GetCurrentPosition().iX > iTarget->GetCurrentPosition().iX)
				{
					iFighter->TurnLeft();
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_dive_down2;
				}
			}
			break;
		}
		case EFlyTowardsShipMethod2_dive_down2:
		{
			//slow down to bombing speed
			if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
			{
				iFighter->DecreaseSpeed();
				iFighter->DecreaseSpeed();
			}
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
			//check if we are already at the torpedo releasing height
			if(iShipBombLowHeight == 1)
			{
				if(FIGHTER_AI_SHIP_BOMB_LOW_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > lFighterHeight)
				{
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_fly_horizontally2;
				}
			}
			else if(iShipBombLowHeight == 0)
			{
				if(FIGHTER_AI_SHIP_BOMB_HEIGHT + FIGHTER_AI_HEIGHT_SAFETY_MARGIN_WHEN_DIVING_TOWARDS_TARGET > lFighterHeight)
				{
					iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_fly_horizontally2;
				}
			}
			break;
		}
		case EFlyTowardsShipMethod2_fly_horizontally2:
		{
			//adjust the diving angle if needed
			if(iFighter->IsReflected())
			{
				//pointing to the right
				if(*iFighter->GetFlyingAngel() != 0)
					FlyAtAngle(0, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iCombatStatus = ECombat_ship_bomb;
						iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_check_dive_down;
					}
					else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_decrease_flying_height;
					}
				}
			}
			else if(!iFighter->IsReflected())
			{
				//pointing to the left
				if(*iFighter->GetFlyingAngel() != 180)
					FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
				else
				{
					if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iCombatStatus = ECombat_ship_bomb;
						iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_check_dive_down;
					}
					else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
					{
						iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_decrease_flying_height;
					}
				}
			}
			break;
		}
		case EFlyTowardsShipMethod2_decrease_flying_height:
		{
			if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iCombatStatus = ECombat_ship_bomb;
				iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_check_dive_down;
				break;
			}
			else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iFighter->DecreaseFlyingAngle();
				iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_increase_flying_height;
			}
			break;
		}
		case EFlyTowardsShipMethod2_increase_flying_height:
		{
			iFighter->IncreaseFlyingAngle();
			if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iCombatStatus = ECombat_ship_bomb;
				iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_check_dive_down;
				break;
			}
			else if(lFighterHeight > FIGHTER_AI_TORPEDO_DROP_HEIGHT)
			{
				iFlyTowardsShipMethod2 = EFlyTowardsShipMethod2_decrease_flying_height;
			}
			break;
		}
	}
}

void CFighterAI::ShipBombLogic()
{
	CheckHealthStatus();
	AvoidOtherShipHitBox();
	//retreat when the ai runs out of bombs
	if(!iFighter->Fire2HasAmmunitionLeft())
	{
		iPlaneStatus = EPlaneStatus_retreat;
	}
	if(iTarget && iTarget->IsAlive())
	{
		CalculateNeededDistanceForDrop();
		CalculateManeuverDistanceTravel(FIGHTER_AI_IDLE_MANEUVER_ANGLE);
		TInt lTargetLocation = iTarget->GetCurrentPosition().iX.GetIntInBaseInt();
		TInt lBomberLocation = iFighter->GetCurrentPosition().iX.GetIntInBaseInt();
		TInt lTargetWidth = iTarget->GetWidth().GetIntInBaseInt();
		TInt lFighterWidth = iFighter->GetWidth().GetIntInBaseInt();
		TInt lFighterAngle = *iFighter->GetFlyingAngel();
		TInt lFighterHeight = iFighter->GetCurrentPosition().iY.GetIntInBaseInt();
		TInt lTargetHeight = iTarget->GetHeight().GetIntInBaseInt();
		switch(iShipBombStatus)
		{
			case EShipBombStatus_face_target:
			{
				//face plane towards target
				if(lBomberLocation >= lTargetLocation + 2 * lTargetWidth + iNeededDistanceForDrop + lFighterWidth)
				{
					iFighter->TurnLeft();
					iShipBombStatus = EShipBombStatus_initiate_bombing;
				}
				if(lBomberLocation < lTargetLocation - 2 * lTargetWidth - iNeededDistanceForDrop - lFighterWidth)
				{
					iFighter->TurnRight();
					iShipBombStatus = EShipBombStatus_initiate_bombing;
				}
				//go back to bombing state if it is within range
				else if(lBomberLocation < lTargetLocation + 2 * lTargetWidth + iNeededDistanceForDrop + lFighterWidth &&
						lBomberLocation > lTargetLocation - 2 * lTargetWidth - iNeededDistanceForDrop - lFighterWidth)
				{
					iShipBombStatus = EShipBombStatus_initiate_bombing;
				}
				break;
			}
			case EShipBombStatus_initiate_bombing:
			{
				TInt lFramesNeededToHitTarget = UNDER_WATER_TORPEDO_RANGE / UNDER_WATER_TORPEDO_SPEED;
				TInt lTargetTravelDistance = CMath::Abs( (*static_cast<CMoveableGameObject*>(iTarget)->GetPixelsPerMoveX()).GetIntInBaseInt()) * lFramesNeededToHitTarget;
				if(iFighter->IsReflected() == iTarget->IsReflected())
					lTargetTravelDistance = lTargetTravelDistance * -1;
				TBool lIsWithinNeededDistance = (iFighter->DistanceBetweenGameObjects(iTarget).GetIntInBaseInt()) < UNDER_WATER_TORPEDO_RANGE + lTargetTravelDistance;
				TBool lPassedTarget = false;
				if(!iFighter->IsReflected())
				{
					if(!iTarget->IsReflected())
					{
						if(lBomberLocation - FIGHTER_AI_SAFE_TORPEDO_DROP_DISTANCE <= lTargetLocation + lTargetWidth)
						{
							lPassedTarget = true;
						}
					}
					else if(iTarget->IsReflected())
					{
						if(lBomberLocation - FIGHTER_AI_SAFE_TORPEDO_DROP_DISTANCE <= lTargetLocation)
						{
							lPassedTarget = true;
						}
					}
				}
				else if(iFighter->IsReflected())
				{
					if(iTarget->IsReflected())
					{
						if(lBomberLocation + FIGHTER_AI_SAFE_TORPEDO_DROP_DISTANCE >= lTargetLocation - lTargetWidth)
						{
							lPassedTarget = true;
						}
					}
					else if(!iTarget->IsReflected())
					{
						if(lBomberLocation + FIGHTER_AI_SAFE_TORPEDO_DROP_DISTANCE >= lTargetLocation)
						{
							lPassedTarget = true;
						}
					}
				}
				if(lIsWithinNeededDistance && !lPassedTarget)
				{
					if(iFighter->Fire2HasAmmunitionLeft())
					{
						if(iFighter->GetCurrentAbsoluteSpeed().GetIntInBaseInt() <= FIGHTER_AI_SHIP_BOMB_SPEED)
						{
							if(CheckIfSaveToReleaseTorpedo() && CheckIfFlyingOverSea() && (lFighterAngle == 0 || lFighterAngle == 180))
							{
								iFighter->Fire2();
								if(iTarget->GetHealth() <= UNDER_WATER_TORPEDO_DAMAGE)
								{
									TargetSpecificTarget(GAMEOBJECT_TYPE_GROUP_SHIP);
									iShipBombStatus = EShipBombStatus_fly_to_bombing_distance;
									if(iTarget)
									{
										//check if target is too far away
										TInt lDistanceBetweenTargets = CMath::DistanceBetweenPoints(iFighter->GetCurrentPosition(),iTarget->GetCurrentPosition());
										if(lDistanceBetweenTargets > FIGHTER_AI_SHIP_BOMB_SHIP_MAX_AWAY_DISTANCE_FOR_SECOND_ATTACK)
										{
											
											iPlaneStatus = EPlaneStatus_retreat;
										}
									}
								}
								else
								{
									iShipBombStatus = EShipBombStatus_face_target;
								}
							}
						}
					}
				}else if(lPassedTarget)
				{
					iShipBombStatus = EShipBombStatus_fly_to_bombing_distance;
				}
				break;
			}
			case EShipBombStatus_fly_to_bombing_distance:
			{
				if(iFighter->Fire2HasAmmunitionLeft() == true)
				{
					switch(iShipBombBehindTarget)
					{
						case EShipBombBehindTarget_tilt_up:
						{
							if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() != FIGHTER_AI_MAX_ENGINE_SPEED)
							{
								iFighter->IncreaseSpeed();
							}
							if(!iFighter->IsReflected())
							{
								if(lFighterAngle == 135)
								{
									iShipBombBehindTarget = EShipBombBehindTarget_fly_to_height;
								}
								FlyAtAngle(135, FIGHTER_AI_TILT_DELAY);
							}
							else if(iFighter->IsReflected())
							{
								if(lFighterAngle == 45)
								{
									iShipBombBehindTarget = EShipBombBehindTarget_fly_to_height;
								}
								FlyAtAngle(45, FIGHTER_AI_TILT_DELAY);
							}
							break;
						}
						case EShipBombBehindTarget_tilt_down:
						{
							if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() != FIGHTER_AI_MAX_ENGINE_SPEED)
							{
								iFighter->IncreaseSpeed();
							}
							if(!iFighter->IsReflected())
							{
								if(lFighterAngle == 225)
								{
									iShipBombBehindTarget = EShipBombBehindTarget_fly_to_height;
								}
								FlyAtAngle(225, FIGHTER_AI_TILT_DELAY);
							}
							else if(iFighter->IsReflected())
							{
								if(lFighterAngle == 315)
								{
									iShipBombBehindTarget = EShipBombBehindTarget_fly_to_height;
								}
								FlyAtAngle(315, FIGHTER_AI_TILT_DELAY);
							}
							break;
						}
						case EShipBombBehindTarget_fly_to_height:
						{
							if(lFighterAngle >= 15 && lFighterAngle <= 165)
							{
								if(lFighterHeight >= FIGHTER_AI_TORPEDO_DROP_HEIGHT + lTargetHeight)
								{
									iShipBombBehindTarget = EShipBombBehindTarget_fly_horizontally;
								}
							}
							else if(lFighterAngle >= 195 && lFighterAngle <= 345)
							{
								if(lFighterHeight <= FIGHTER_AI_TORPEDO_DROP_HEIGHT + lTargetHeight)
								{
									iShipBombBehindTarget = EShipBombBehindTarget_fly_horizontally;
								}
							}
							break;
						}
						case EShipBombBehindTarget_fly_horizontally:
						{
							if(!iFighter->IsReflected())
							{
								if(lFighterAngle != 180)
								{
									FlyAtAngle(180, FIGHTER_AI_TILT_DELAY);
								}
								else 
								{
									iShipBombBehindTarget = EShipBombBehindTarget_slow_down_to_torpedo_speed;
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
									iShipBombBehindTarget = EShipBombBehindTarget_slow_down_to_torpedo_speed;
								}
							}
							break;
						}
						case EShipBombBehindTarget_slow_down_to_torpedo_speed:
						{
							//check if we are above land, in that case turn other side and fly back up
							if(!CheckIfFlyingOverSea())
							{
								if(iFighter->IsReflected())
								{
									iFighter->TurnLeft();
								}
								else
								{
									iFighter->TurnRight();
								}
								iShipBombBehindTarget = EShipBombBehindTarget_tilt_up;
							}
							if(!iFighter->IsReflected())
							{
								if(!iTarget->IsReflected())
								{
									if(lBomberLocation <= lTargetLocation)
									{
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iFighter->DecreaseSpeed();
											iFighter->DecreaseSpeed();
										}
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() == FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iShipBombBehindTarget = EShipBombBehindTarget_fly_away_from_target;
										}
									}
								}
								else if(iTarget->IsReflected())
								{
									if(lBomberLocation <= lTargetLocation - lTargetWidth)
									{
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iFighter->DecreaseSpeed();
											iFighter->DecreaseSpeed();
										}
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() == FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iShipBombBehindTarget = EShipBombBehindTarget_fly_away_from_target;
										}
									}
								}
							}
							else if(iFighter->IsReflected())
							{
								if(iTarget->IsReflected())
								{
									if(lBomberLocation >= lTargetLocation)
									{
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iFighter->DecreaseSpeed();
											iFighter->DecreaseSpeed();
										}
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() == FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iShipBombBehindTarget = EShipBombBehindTarget_fly_away_from_target;
										}
									}
								}
								else if(!iTarget->IsReflected())
								{
									if(lBomberLocation >= lTargetLocation + lTargetWidth)
									{
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() > FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iFighter->DecreaseSpeed();
											iFighter->DecreaseSpeed();
										}
										if(iFighter->GetCurrentEngineAcceleration().GetIntInBaseInt() == FIGHTER_AI_SHIP_BOMB_SPEED)
										{
											iShipBombBehindTarget = EShipBombBehindTarget_fly_away_from_target;
										}
									}
								}
							}
							break;
						}
						case EShipBombBehindTarget_fly_away_from_target:
						{
							if(iFighter->IsReflected())
							{
								if(iTarget->IsReflected())
								{	
									if(lBomberLocation >= lTargetLocation + (FIGHTER_AI_TORPEDO_ATTACK_DISTANCE * 2))
									{
										iShipBombBehindTarget = EShipBombBehindTarget_resume_torpedo;
									}
								}
								else if(!iTarget->IsReflected())
								{
									if(lBomberLocation >= lTargetLocation + lTargetWidth + (FIGHTER_AI_TORPEDO_ATTACK_DISTANCE * 2))
									{
										iShipBombBehindTarget = EShipBombBehindTarget_resume_torpedo;
									}
								}
							}
							else if(!iFighter->IsReflected())
							{
								if(iTarget->IsReflected())
								{
									if(lBomberLocation <= lTargetLocation - lTargetWidth - (FIGHTER_AI_TORPEDO_ATTACK_DISTANCE * 2))
									{
										iShipBombBehindTarget = EShipBombBehindTarget_resume_torpedo;
									}
								}
								else if(!iTarget->IsReflected())
								{
									if(lBomberLocation <= lTargetLocation - (FIGHTER_AI_TORPEDO_ATTACK_DISTANCE * 2))
									{
										iShipBombBehindTarget = EShipBombBehindTarget_resume_torpedo;
									}
								}
							}
							break;
						}
						case EShipBombBehindTarget_resume_torpedo:
						{
							AdjustShipBombHeight();
							break;
						}
					}
				}
				break;
			}
		}
	}
	else
	{
		iPlaneStatus = EPlaneStatus_retreat;
	}
}
