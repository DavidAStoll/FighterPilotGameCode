/*
 ============================================================================
 Name		: ShipArteriallyAI.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CShipArteriallyAI implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/AI/CShipArteriallyAI.h"
#include "includes/game/backend/basicClasses/CMobileAAGun.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CShipArteriallyAI::CShipArteriallyAI(CMobileRotatingWeapon * aArterially, SShipArteriallyAIWaitingCase& aWaitingCase)
{
	iCurrentFrame = 0;
	iBurstFireMode = false;
	iCurrentShotInBurst = 0;
	iShotsPerBurstFire = 0;
	iShipGun = aArterially;
	iWaitingCase = aWaitingCase;
	iCurrentState = EShipArteriallyAIWaiting;
	iJustShot = false;
	iSpeedForProjectile = TIntFloat::Convert(0);
	iTargetPriorities = GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS;
}

CShipArteriallyAI::CShipArteriallyAI(CMobileRotatingWeapon * aArterially, SShipArteriallyAIWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot)
{
	iCurrentFrame = 0;
	iBurstFireMode = true;
	iCurrentShotInBurst = 0;
	iShotsPerBurstFire = aShotsPerBurstFire;
	iShipGun = aArterially;
	iWaitingCase = aWaitingCase;
	iWaitFramesUntilNextBurstShot = aWaitFramesUntilNextBurstShot;
	iCurrentState = EShipArteriallyAIWaiting;
	iJustShot = false;
	iSpeedForProjectile = TIntFloat::Convert(0);
	iTargetPriorities = GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS;
}

CShipArteriallyAI* CShipArteriallyAI::New(CMobileRotatingWeapon * aArterially, SShipArteriallyAIWaitingCase& aWaitingCase)
{
	CShipArteriallyAI* self = new CShipArteriallyAI(aArterially, aWaitingCase);;
	self->ConstructL();
	return self;
}

CShipArteriallyAI* CShipArteriallyAI::New(CMobileRotatingWeapon * aArterially, SShipArteriallyAIWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot)
{
	CShipArteriallyAI* self = new CShipArteriallyAI(aArterially, aWaitingCase, aShotsPerBurstFire, aWaitFramesUntilNextBurstShot);
	self->ConstructL();
	return self;
}

CShipArteriallyAI::~CShipArteriallyAI()
{
	delete[] iRangeForDifferentAngles;
}

void CShipArteriallyAI::ConstructL()
{
	iShipGun->Update();
	CalculateProjectileSpeedForMaxRange();
	SetupRangeForGunAngles();
}

//---------------------- functions -------------------------------//

void CShipArteriallyAI::Update()
{
	//only due something if object is still alive
	if(iShipGun->IsDisabled())
		return;

	//check what state we are currently in
	switch(iCurrentState)
	{
		case EShipArteriallyAIWaiting:
		{
			iCurrentFrame++;
			if((iJustShot && iCurrentFrame == iWaitingCase.FrameDelayAfterShot) || (!iJustShot && iCurrentFrame == iWaitingCase.FrameDelay))
			{
				iCurrentFrame = 0;//reset timer
				iJustShot = false;
				//look for enemies and maybe go into shooting mode, otherwise return to Waiting State
				if(TargetEnemy())
				{
					//plane found for targeting
					iCurrentState = EShipArteriallyAIShooting;
				}else
				{
					iCurrentState = EShipArteriallyAIWaiting;
				}
			}
		}
			break;
		case EShipArteriallyAIShooting:
		{
			//first need to check if we are still in the same reflection mode otherwise the gun might get messed up, since the angle it wanted to go to has become totally different
			if(iGunReflected != iShipGun->GetGunIsReflected())
			{
				//the reflection changed from the time the calculation was done, abort this targetting
				//reset everyting
				iCurrentFrame = 0;
				iCurrentShotInBurst = 0;
				iJustShot = true;
				iCurrentState = EShipArteriallyAIWaiting;
				return;
			}

			if(iShootingCase.NeededAngleChange == 0) // committed all the change that we needed
			{
				if(iBurstFireMode)//special mode that fires more than once
				{
					if(iCurrentFrame == 0)
					{
						//reached the required angel, therefore shoot the weapon
						iShipGun->ShootWithSpeed(iSpeedForProjectile);
						iCurrentShotInBurst++;

						if(iCurrentShotInBurst == iShotsPerBurstFire)
						{
							//all required shots have been shot
							iCurrentFrame = 0;
							iCurrentShotInBurst = 0;
							iJustShot = true;
							iCurrentState = EShipArteriallyAIWaiting;
						}else
						{
							iCurrentFrame = iWaitFramesUntilNextBurstShot;
						}
					}else
						iCurrentFrame--;//just waiting until we can make the next burst shot
				}else
				{
					//reached the required angel, therefore shoot the weapon
					iShipGun->ShootWithSpeed(iSpeedForProjectile);
					//let the canon rest for a while until it can shoot again
					iCurrentFrame = 0;
					iJustShot = true;
					iCurrentState = EShipArteriallyAIWaiting;
				}
			}
			//turn Gun
			else if(CMath::Abs(iShootingCase.NeededAngleChange) < iShipGun->GetGunRotationSpeed())
			{
				//difference is so small that by the next rotation we will be on the right angel, by might over turn it
				//therefore need to set it to the right angel explicitly
				iShipGun->ChangeGunAngle(iShootingCase.NeededAngleChange);
				iShootingCase.NeededAngleChange = 0;
			}else
			{
				//need to turn gun to the correct Angel position
				//check if we need to turn up or down
				if(iShootingCase.NeededAngleChange > 0) //positive
				{
					TInt lRotation = iShipGun->GetGunRotationSpeed();
					iShootingCase.NeededAngleChange -= lRotation;
					iShipGun->ChangeGunAngle(lRotation);
				}else //negative
				{
					TInt lRotation = iShipGun->GetGunRotationSpeed();
					iShootingCase.NeededAngleChange += lRotation;
					iShipGun->ChangeGunAngle(-lRotation);
				}
			}
		}
			break;
	}
}

void CShipArteriallyAI::SetTargetPriorities(TUint aTargetPriorities)
{
	iTargetPriorities = aTargetPriorities;
}

void CShipArteriallyAI::CalculateProjectileSpeedForMaxRange()
{
	//Using equation Speedxy = root[(a*d)/2]
	//knowing that the t = (2*Vy)/a
	//and t = d/Vx
	//if angle is 45 degrees Vx == Vy
	//need to divide by the angle at which the projectile gets shoot, in this case it has to be 45 degrees
	TInt lMaxRange = iShipGun->GetMaxRange();
	TInt lMaxAngle = 45; //in order to the calculation the max angle has to be 45 degrees
	TIntFloat lGravitionalAcceleration = - TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE) / TIntFloat::Convert(GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE);
	TIntFloat lDistanceTimesAcceleration = (lGravitionalAcceleration * lMaxRange) / 2;
	TFloat lVectorSpeed = CMath::Abs(lDistanceTimesAcceleration).GetIntInBaseInt(); //can't be negative otherwise TReal screws up,have to go into Int otherwise Sqrt will not work
	lVectorSpeed = CMath::Sqrt(lVectorSpeed);
	TIntFloat lSourceSpeed = TIntFloat::Convert(lVectorSpeed); //convert back to TIntfloat from TReal in base TInt
	lSourceSpeed = lSourceSpeed / CMath::GraphicsSinTable(lMaxAngle);
	iSpeedForProjectile = lSourceSpeed;
}

void CShipArteriallyAI::SetupRangeForGunAngles()
{
	iGunDegreeRange = CMath::Abs(iShipGun->GetMaxAngle() - iShipGun->GetMinAngle()) + 1;
	iRangeForDifferentAngles = new TInt[iGunDegreeRange];//need to add one

	//find the different distances the gun can shoot to at the different ranges
	for(TInt lCurrentDegree = 0; lCurrentDegree < iGunDegreeRange; lCurrentDegree++)
	{
		TInt lDegree;
		if(iShipGun->GetMoveableObject()->IsReflected()) //Min and Max always go clockwise, with Min Starting
			lDegree = iShipGun->GetMinAngle() + lCurrentDegree;
		else
			lDegree = iShipGun->GetMaxAngle() + lCurrentDegree; //thus, Max is actually Min in this case
		TIntFloat lYVelocity = (CMath::GraphicsSinTable(lDegree) * iSpeedForProjectile);
		TIntFloat lXVelocity = (CMath::GraphicsCosTable(lDegree) * iSpeedForProjectile);
		//Newton formula
		//simplified case: t = 2Vy / a - simpliefied since we ignore initial height
		//find time to hit ground
		TIntFloat lAcceleration = TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE) / TIntFloat::Convert(GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE);
		TInt lFramesToHitGround = ((lYVelocity * 2) / lAcceleration).GetIntInBaseInt();
		//find X distance, which is the range at this angle
		TIntFloat lXDistanceTravelled =  lXVelocity * lFramesToHitGround;
		iRangeForDifferentAngles[lCurrentDegree] = CMath::Abs(lXDistanceTravelled).GetIntInBaseInt(); //make sure it is not negative
	}
}

TInt CShipArteriallyAI::FindBestAngleToHitTarget(TInt aDistanceToTarget)
{
	TInt lCurrentBestDifference = 100000; //something big
	TInt lBestAngle = 0;
	for(TInt lCurrentDegree = 0; lCurrentDegree < iGunDegreeRange; lCurrentDegree++)
	{
		TInt lTempDifference = CMath::Abs(aDistanceToTarget - iRangeForDifferentAngles[lCurrentDegree]);
		if(lTempDifference < lCurrentBestDifference)
		{
			lCurrentBestDifference = lTempDifference;
			lBestAngle = lCurrentDegree;
		}
	}
	//should contain the best Angle to hit the target
	return lBestAngle;
}

TBool CShipArteriallyAI::GunIsFacingCorrectSideToAttackTarget(CGameObject* aTarget)
{
	TPointIntFloat lGunPosition = iShipGun->GetRelativeWeaponPosition();
	TInt lAngleVectorToTarget = CMath::AngleFromOriginBetweenPoints(lGunPosition,aTarget->GetCurrentPosition());
	TInt lShipGunAngle = iShipGun->GetMinAngle();
	TBool lTargetIsToLeft = false;
	TBool lGunIsFacingLeft = false;
	if(270 > lAngleVectorToTarget && lAngleVectorToTarget > 90)
		lTargetIsToLeft = true;
	if(270 > lShipGunAngle && lShipGunAngle > 90)
		lGunIsFacingLeft = true;
	
	if(lGunIsFacingLeft == lTargetIsToLeft)
		return true;
	else
		return false;
}

TBool CShipArteriallyAI::TargetEnemy()
{
	TPointIntFloat lGunPosition = iShipGun->GetRelativeWeaponPosition();
	CPointerArray<CGameObject>* lGameObjectsInRange = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->ReturnObjectsInRange(lGunPosition.iX, TIntFloat::Convert(iShipGun->GetMaxRange()), iTargetPriorities);
	TBool lFoundTarget = false;
	CGameObject* lBestTarget = NULL;
	TInt lBestDistanceToTarget = -1;
			
	//check all Objects of type Planes that are not on your side
	for(TInt lCurrentIndex = 0; lCurrentIndex < lGameObjectsInRange->GetCount(); lCurrentIndex++)
	{
		CGameObject* lCurrentGameObject = (*lGameObjectsInRange)[lCurrentIndex];
		if(lCurrentGameObject->GetConflictSide() != iShipGun->GetConflictSide() && lCurrentGameObject->IsAlive())
		{
			//make a closer check calculating in the Y factor
			TInt lCurrentDistanceToTarget = CMath::DistanceBetweenPoints(lGunPosition, lCurrentGameObject->GetCurrentPosition());
			if(lCurrentDistanceToTarget < iShipGun->GetMaxRange() && lCurrentDistanceToTarget > (iRangeForDifferentAngles[0]) && GunIsFacingCorrectSideToAttackTarget(lCurrentGameObject)) //always the smallest range
			{
				if(lCurrentDistanceToTarget < lBestDistanceToTarget || lBestDistanceToTarget == -1)
				{
					lBestTarget = lCurrentGameObject;
					lBestDistanceToTarget = lCurrentDistanceToTarget;
				}
			}
		}
	}
	if(lBestTarget)
	{
		lFoundTarget = true;
		TInt lNeededAngle;
		if(iShipGun->GetMoveableObject()->IsReflected()) //Min and Max always go clockwise, with Min Starting
			lNeededAngle = iShipGun->GetMinAngle() + FindBestAngleToHitTarget(lBestDistanceToTarget);
		else
			lNeededAngle = iShipGun->GetMaxAngle() - FindBestAngleToHitTarget(lBestDistanceToTarget); // max is actuall Min in this case
		iShootingCase.NeededAngleChange = lNeededAngle - iShipGun->GetCurrentGunAngle();

		//need to do minor adjustments so that it turns the correct way
		if(iShootingCase.NeededAngleChange > 0 && 360 - lNeededAngle + iShipGun->GetCurrentGunAngle() < iShootingCase.NeededAngleChange)
			iShootingCase.NeededAngleChange = (360 - lNeededAngle + iShipGun->GetCurrentGunAngle()) * -1;//going into opposite direction no
		else if(iShootingCase.NeededAngleChange < 0 && iShipGun->GetCurrentGunAngle() - lNeededAngle - 360 > iShootingCase.NeededAngleChange)
			iShootingCase.NeededAngleChange = (iShipGun->GetCurrentGunAngle() - lNeededAngle - 360) * -1;//going into opposite direction no

		//found target no need to look for others
		iGunReflected = iShipGun->GetGunIsReflected();
	}

	//clean up
	delete lGameObjectsInRange;//needs to be delete as well since it resides on the heap
	return lFoundTarget;
}

void CShipArteriallyAI::SaveOnDisk(CFileWriteStream &aOutputStream)
{	
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iShotsPerBurstFire);
	aOutputStream.WriteInt32(iWaitFramesUntilNextBurstShot);
	aOutputStream.WriteInt32(iCurrentShotInBurst);
	aOutputStream.WriteInt32(iGunDegreeRange);
	aOutputStream.WriteInt32(iTargetPriorities);
	aOutputStream.WriteIntFloat(iSpeedForProjectile);
	aOutputStream.WriteInt32(iGunReflected);
	aOutputStream.WriteInt32(iBurstFireMode);
	aOutputStream.WriteInt32(iJustShot);
	aOutputStream.WriteInt32(iCurrentState);
	aOutputStream.WriteInt32(iWaitingCase.FrameDelay); 	aOutputStream.WriteInt32(iWaitingCase.FrameDelayAfterShot);
	aOutputStream.WriteInt32(iShootingCase.NeededAngleChange);
}

void CShipArteriallyAI::LoadFromDisk(CFileReadStream &aReadStream)
{	
	iCurrentFrame = aReadStream.ReadInt32();
	iShotsPerBurstFire = aReadStream.ReadInt32();
	iWaitFramesUntilNextBurstShot = aReadStream.ReadInt32();
	iCurrentShotInBurst = aReadStream.ReadInt32();
	iGunDegreeRange = aReadStream.ReadInt32();
	iTargetPriorities = aReadStream.ReadInt32();
	iSpeedForProjectile = aReadStream.ReadIntFloat();
	iGunReflected = aReadStream.ReadInt32();
	iBurstFireMode = aReadStream.ReadInt32();
	iJustShot = aReadStream.ReadInt32();
	iCurrentState = static_cast<TShipArteriallyAICurrentState>(aReadStream.ReadInt32());
	iWaitingCase.FrameDelay = aReadStream.ReadInt32(); 	iWaitingCase.FrameDelayAfterShot = aReadStream.ReadInt32();
	iShootingCase.NeededAngleChange = aReadStream.ReadInt32();
}

