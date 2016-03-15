/*
 ============================================================================
 Name		: BackCannonAI.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBackCannonAI implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/AI/CBackCannonAI.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CBackCannonAI::CBackCannonAI(CMobileAAGun * aMobileAAGun, SBackCannonAIWaitingCase& aWaitingCase)
{
	iCurrentFrame = 0;
	iBurstFireMode = false;
	iGunReflected = false;
	iCurrentShotInBurst = 0;
	iShotsPerBurstFire = 0;
	iMobileAAGun = aMobileAAGun;
	iWaitingCase = aWaitingCase;
	iCurrentState = EBackCannonAIWaiting;
	iJustShotAtAirplane = false;
}

CBackCannonAI::CBackCannonAI(CMobileAAGun * aMobileAAGun, SBackCannonAIWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot)
{
	iCurrentFrame = 0;
	iBurstFireMode = true;
	iCurrentShotInBurst = 0;
	iShotsPerBurstFire = aShotsPerBurstFire;
	iMobileAAGun = aMobileAAGun;
	iWaitingCase = aWaitingCase;
	iWaitFramesUntilNextBurstShot = aWaitFramesUntilNextBurstShot;
	iCurrentState = EBackCannonAIWaiting;
	iJustShotAtAirplane = false;
}

CBackCannonAI::~CBackCannonAI()
{
}

// ---------------------- functions ----------------------------------//


void CBackCannonAI::Update()
{
	//only due something if object is still alive
	if(iMobileAAGun->IsDisabled())
		return;

	//check what state we are currently in
	switch(iCurrentState)
	{
		case EBackCannonAIWaiting:
		{
			iCurrentFrame++;
			if((iJustShotAtAirplane && iCurrentFrame == iWaitingCase.FrameDelayAfterShot) || (!iJustShotAtAirplane && iCurrentFrame == iWaitingCase.FrameDelay))
			{
				iCurrentFrame = 0;//reset timer
				iJustShotAtAirplane = false;
				//look for enemies and maybe go into shooting mode, otherwise return to Waiting State
				if(TargetEnemyPlane())
				{
					//plane found for targeting
					iCurrentState = EBackCannonAIShooting;
					iMobileAAGun->Shoot(iShootingCase.Range);
				}else
				{
					iCurrentState = EBackCannonAIWaiting;
				}
			}
		}
			break;
		case EBackCannonAIShooting:
		{
			//first need to check if we are still in the same reflection mode otherwise the gun might get messed up, since the angle it wanted to go to has become totally different
			if(iGunReflected != iMobileAAGun->GetGunIsReflected())
			{
				//the reflection changed from the time the calculation was done, abort this targetting
				//reset everyting
				iCurrentFrame = 0;
				iCurrentShotInBurst = 0;
				iJustShotAtAirplane = true;
				iCurrentState = EBackCannonAIWaiting;
				return;
			}

			if(iShootingCase.NeededAngleChange == 0) // committed all the change that we needed
			{
				if(iBurstFireMode)//special mode that fires more than once
				{
					if(iCurrentFrame == 0)
					{
						//reached the required angel, therefore shoot the weapon
						iMobileAAGun->Shoot(iShootingCase.Range);
						iCurrentShotInBurst++;

						if(iCurrentShotInBurst == iShotsPerBurstFire)
						{
							//all required shots have been shot
							iCurrentFrame = 0;
							iCurrentShotInBurst = 0;
							iJustShotAtAirplane = true;
							iCurrentState = EBackCannonAIWaiting;
						}else
						{
							iCurrentFrame = iWaitFramesUntilNextBurstShot;
						}
					}else
						iCurrentFrame--;//just waiting until we can make the next burst shot
				}else
				{
					//reached the required angel, therefore shoot the weapon
					iMobileAAGun->Shoot(iShootingCase.Range);
					//let the canon rest for a while until it can shoot again
					iCurrentFrame = 0;
					iJustShotAtAirplane = true;
					iCurrentState = EBackCannonAIWaiting;
				}
			}
			//turn Gun
			else if(CMath::Abs(iShootingCase.NeededAngleChange) < iMobileAAGun->GetGunRotationSpeed())
			{
				//difference is so small that by the next rotation we will be on the right angel, by might over turn it
				//therefore need to set it to the right angel explicitly
				iMobileAAGun->ChangeGunAngle(iShootingCase.NeededAngleChange);
				iShootingCase.NeededAngleChange = 0;
			}else
			{
				//need to turn gun to the correct Angel position
				//check if we need to turn up or down
				if(iShootingCase.NeededAngleChange > 0) //positive
				{
					TInt lRotation = iMobileAAGun->GetGunRotationSpeed();
					iShootingCase.NeededAngleChange -= lRotation;
					iMobileAAGun->ChangeGunAngle(lRotation);
				}else //negative
				{
					TInt lRotation = iMobileAAGun->GetGunRotationSpeed();
					iShootingCase.NeededAngleChange += lRotation;
					iMobileAAGun->ChangeGunAngle(-lRotation);
				}
			}
		}
			break;
	}
}

TInt CBackCannonAI::FindAngleChangeNeededForTargetAngle(TInt aTargetAngle)
{
	TInt lMaxAngle = iMobileAAGun->GetMaxAngle();
	CMath::AdjustAngleToUnitCircle(lMaxAngle);
	TInt lMinAngle = iMobileAAGun->GetMinAngle();
	CMath::AdjustAngleToUnitCircle(lMinAngle);
	TInt lCurrentAngle = iMobileAAGun->GetCurrentGunAngle();
	TInt lAngleChangeNeeded = 0;
	TInt lMaxDegreesWeCanMoveClockwise = 0;
	if(lMinAngle > lCurrentAngle) //this means that the MaxAngle is over the 360 degree turn and those needs to be calculated a bit different
	{
		/*													|
		 *  				180 -------------------------- 0 - Current Angle is Above the 0 - 180 line, while Max angle is below, causing this special case
		 *													|
		 */
		lMaxDegreesWeCanMoveClockwise += lCurrentAngle;
		lMaxDegreesWeCanMoveClockwise += 360 - lMinAngle;
	}else
	{
		//simple case
		lMaxDegreesWeCanMoveClockwise = lCurrentAngle - lMinAngle;
	}
	TInt lMaxDegreesWeCanMoveCounterClockwise = 0;
	if(lCurrentAngle > lMaxAngle) //this means that current angle is below the 360 degree turn, so we can't do a simple substraction find the real distance between these two points
	{
		lMaxDegreesWeCanMoveCounterClockwise += 360 - lCurrentAngle;
		lMaxDegreesWeCanMoveCounterClockwise += lMaxAngle;
	}else
	{
		lMaxDegreesWeCanMoveCounterClockwise = lMaxAngle - lCurrentAngle;
	}
	//now we know how many degrees we can turn clockwise and counterclockwise, since only one way of turning will bring us needed location
	//all that is left to do is to find out which way of turning has enough degrees to reach the target point
	if(lCurrentAngle >= aTargetAngle) //means we can always turn to it clockwise withouth causing the special case
	{
		lAngleChangeNeeded = lCurrentAngle - aTargetAngle; //this would be the change if we turn clockwise
		if(lAngleChangeNeeded > lMaxDegreesWeCanMoveClockwise) // however is it possible to turn this gun this way?
		{
			//not possible, thus the only option left is to turn counter-clockwise, which has to be possible
			lAngleChangeNeeded = 360 - lCurrentAngle;
			lAngleChangeNeeded += aTargetAngle;
			//is a positive angle change since we increase the angle of the gun
		}else
		{
			//is a negative angle change, since we decrease the angle of the gun
			lAngleChangeNeeded *= -1;
		}
	}else if(lCurrentAngle < aTargetAngle) //means we turn to it counter-clockwise without causing the special case
	{
		lAngleChangeNeeded = aTargetAngle - lCurrentAngle;//this would be the change if we turn counter-clockwise
		if(lAngleChangeNeeded > lMaxDegreesWeCanMoveCounterClockwise) //is it possible?
		{
			//not possible, thus the only option left is to turn clockwise, which has to be possible
			lAngleChangeNeeded = lCurrentAngle;
			lAngleChangeNeeded += 360 - aTargetAngle;
			//is a negative angle change since we decrease the angle of the gun
			lAngleChangeNeeded *= -1;
		}
		//is a positive angle change since we increase the angle of the gun
	}
	return lAngleChangeNeeded;
}

TBool CBackCannonAI::TargetEnemyPlane()
{
	TPointIntFloat lGunPosition = iMobileAAGun->GetRelativeWeaponPosition();
	CPointerArray<CGameObject>* lGameObjectsInRange = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->ReturnObjectsInRange(lGunPosition.iX, TIntFloat::Convert(iMobileAAGun->GetMaxRange()), EObjectTypesPlane);
	TBool lFoundTarget = false;

	//check all Objects of type Planes that are not on your side
	for(TInt lCurrentIndex = 0; lCurrentIndex < lGameObjectsInRange->GetCount(); lCurrentIndex++)
	{
		CGameObject* lCurrentGameObject = (*lGameObjectsInRange)[lCurrentIndex];
		if(lCurrentGameObject->GetConflictSide() != iMobileAAGun->GetConflictSide() && lCurrentGameObject->IsAlive())
		{
			//make a closer check calculating in the Y factor
			TInt lDistanceToPlane = CMath::DistanceBetweenPoints(lGunPosition, lCurrentGameObject->GetCurrentPosition());
			if(lDistanceToPlane < iMobileAAGun->GetMaxRange() && lDistanceToPlane > iMobileAAGun->GetMinRange())
			{
				//this plane qualifies to be targeted, congrats :)
				CAircraft* lCurrentPlane = static_cast<CAircraft*> (lCurrentGameObject);
				// we are going to check each collision point of the plane to see if we can target any of them
				TInt lNumberOfCollisionPoints = lCurrentPlane->GetHitBox()->GetNumberOfPoints();
				TPointIntFloat** lAdvancedCollisionPoints = lCurrentPlane->GetHitBox()->GetAdvancedPointsLC();
				//for loops interates through each collision point and checks if it can be used
				for(TInt lCurrentCollisionPointIndex = 0; lCurrentCollisionPointIndex < lNumberOfCollisionPoints; lCurrentCollisionPointIndex++)
				{
					TPointIntFloat lCurrentCollisionPointFromPlane = *lAdvancedCollisionPoints[lCurrentCollisionPointIndex];
					//find angel that we need to shoot at to hit the plane at the current Location
					TInt lAngel = CMath::AngleFromOriginBetweenPoints(lGunPosition, lCurrentCollisionPointFromPlane);
					//check if angel is reachable for the gun
					if(((lAngel > iMobileAAGun->GetMinAngle() && lAngel < iMobileAAGun->GetMaxAngle()) || ((lAngel + 360) > iMobileAAGun->GetMinAngle() && (lAngel + 360) < iMobileAAGun->GetMaxAngle()) //need to handle special case were mini angle is 270 or bigger, because needed angle could be 45 or 45+360, they mean both the same 
					))
					{
						//approximate time to reach aircraft
						TInt lFramesNeededToTurnGun = CMath::Abs(FindAngleChangeNeededForTargetAngle(lAngel)) / iMobileAAGun->GetGunRotationSpeed();
						TInt lFramesNeededForProjectileToReachTarget = lDistanceToPlane / iMobileAAGun->GetProjectileSpeed();
						TInt lTotalFramesNeededToReachPlane = lFramesNeededForProjectileToReachTarget + lFramesNeededToTurnGun;

						//need to find out how much faster or slower the target plane moves than the Gun 
						lCurrentCollisionPointFromPlane.iX += (*lCurrentPlane->GetPixelsPerMoveX() - *iMobileAAGun->GetMoveableObject()->GetPixelsPerMoveX()) * lTotalFramesNeededToReachPlane;
						lCurrentCollisionPointFromPlane.iY += (*lCurrentPlane->GetPixelsPerMoveY() - *iMobileAAGun->GetMoveableObject()->GetPixelsPerMoveY()) * lTotalFramesNeededToReachPlane;

						//now have the most likely position of the Aircraft, now we just to find the angel we need to make with this position
						TInt lNeededAngle = CMath::AngleFromOriginBetweenPoints(lGunPosition, lCurrentCollisionPointFromPlane);
						TInt lNeededRange = CMath::DistanceBetweenPoints(lGunPosition, lCurrentCollisionPointFromPlane);
						//check if angel and range can be shot at
						if(((lNeededAngle > iMobileAAGun->GetMinAngle() && lNeededAngle < iMobileAAGun->GetMaxAngle()) || ((lNeededAngle + 360) > iMobileAAGun->GetMinAngle() && (lNeededAngle + 360) < iMobileAAGun->GetMaxAngle()) //need to handle special case were mini angle is 270 or bigger, because needed angle could be 45 or 45+360, they mean both the same 
						) && (lNeededRange < iMobileAAGun->GetMaxRange() && lNeededRange > iMobileAAGun->GetMinRange()))
						{
							SBackCannonAIShootingCase lShootingOrder;
							lShootingOrder.NeededAngleChange = FindAngleChangeNeededForTargetAngle(lNeededAngle);
							lShootingOrder.Range = lNeededRange;
							iShootingCase = lShootingOrder;
							lFoundTarget = true;
							iGunReflected = iMobileAAGun->GetGunIsReflected();
							break;//done, found a plane and it will be shot at next
						}
					}
				}//end of for loop that checks if any collision point works

				//need to clean up the collision points
				for(TInt lCurObj = 0; lCurObj < lNumberOfCollisionPoints; lCurObj++)
					delete lAdvancedCollisionPoints[lCurObj];
				delete[] lAdvancedCollisionPoints;
			}
		}
		if(lFoundTarget)
			break;// break out of the second for loop, since we have a valid that we can shoot at
	}
	//clean up
	delete lGameObjectsInRange;//needs to be delete as well since it resides on the heap

	return lFoundTarget;
}

void CBackCannonAI::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iWaitingCase.FrameDelay);
	aOutputStream.WriteInt32(iWaitingCase.FrameDelayAfterShot);
	aOutputStream.WriteInt32(iShootingCase.NeededAngleChange);
	aOutputStream.WriteInt32(iShootingCase.Range);
	aOutputStream.WriteInt32(iGunReflected);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iShotsPerBurstFire);
	aOutputStream.WriteInt32(iWaitFramesUntilNextBurstShot);
	aOutputStream.WriteInt32(iBurstFireMode);
	aOutputStream.WriteInt32(iJustShotAtAirplane);
	aOutputStream.WriteInt32(iCurrentShotInBurst);
	aOutputStream.WriteInt32(iCurrentState);
}

void CBackCannonAI::LoadFromDisk(CFileReadStream &aReadStream)
{
	iWaitingCase.FrameDelay = aReadStream.ReadInt32();
	iWaitingCase.FrameDelayAfterShot = aReadStream.ReadInt32();
	iShootingCase.NeededAngleChange = aReadStream.ReadInt32();
	iShootingCase.Range = aReadStream.ReadInt32();
	iGunReflected = aReadStream.ReadInt32();
	iCurrentFrame = aReadStream.ReadInt32();
	iShotsPerBurstFire = aReadStream.ReadInt32();
	iWaitFramesUntilNextBurstShot = aReadStream.ReadInt32();
	iBurstFireMode = aReadStream.ReadInt32();
	iJustShotAtAirplane = aReadStream.ReadInt32();
	iCurrentShotInBurst = aReadStream.ReadInt32();
	iCurrentState = static_cast<TBackCannonAICurrentState> (aReadStream.ReadInt32());
}
