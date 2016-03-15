/*
 ============================================================================
 Name		: ArteriallyAI.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CArteriallyAI implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/AI/CAAArteriallyAI.h"
#include "includes/game/backend/basicClasses/CAAArterially.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CGameObject.h"

CAAArteriallyAI::CAAArteriallyAI(CAAArterially * aArterially, SWaitingCase& aWaitingCase)
{
	iCurrentFrame = 0;
	iBurstFireMode = false;
	iCurrentShotInBurst = 0;
	iShotsPerBurstFire = 0;
	iAAArterially = aArterially;
	iWaitingCase = aWaitingCase;
	iCurrentState = EAAArteriallyAIWaiting;
	iJustShotAtAirplane = false;
}

CAAArteriallyAI::CAAArteriallyAI(CAAArterially * aArterially, SWaitingCase& aWaitingCase, TInt aShotsPerBurstFire, TInt aWaitFramesUntilNextBurstShot)
{
	iCurrentFrame = 0;
	iBurstFireMode = true;
	iCurrentShotInBurst = 0;
	iShotsPerBurstFire = aShotsPerBurstFire;
	iAAArterially = aArterially;
	iWaitingCase = aWaitingCase;
	iWaitFramesUntilNextBurstShot = aWaitFramesUntilNextBurstShot;
	iCurrentState = EAAArteriallyAIWaiting;
	iJustShotAtAirplane = false;
}

CAAArteriallyAI::~CAAArteriallyAI()
{
}

//---------------------- functions -------------------------------//

void CAAArteriallyAI::Update()
{
	//only due something if object is still alive
	if(!iAAArterially->IsAlive())
		return;

	//check what state we are currently in
	switch(iCurrentState)
	{
		case EAAArteriallyAIWaiting:
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
					iCurrentState = EAAArteriallyAIShooting;
				}else
				{
					iCurrentState = EAAArteriallyAIWaiting;
				}
			}
		}
			break;
		case EAAArteriallyAIShooting:
		{
			if(iAAArterially->GetCurrentGunAngel() == iShootingCase.NeededAngel)
			{
				if(iBurstFireMode)//special mode that fires more than once
				{
					if(iCurrentFrame == 0)
					{
						//reached the required angel, therefore shoot the weapon
						iAAArterially->Shoot(iShootingCase.Range);
						iCurrentShotInBurst++;

						if(iCurrentShotInBurst == iShotsPerBurstFire)
						{
							//all required shots have been shot
							iCurrentFrame = 0;
							iCurrentShotInBurst = 0;
							iJustShotAtAirplane = true;
							iCurrentState = EAAArteriallyAIWaiting;
						}else
						{
							iCurrentFrame = iWaitFramesUntilNextBurstShot;
						}
					}else
						iCurrentFrame--;//just waiting until we can make the next burst shot
				}else
				{
					//reached the required angel, therefore shoot the weapon
					iAAArterially->Shoot(iShootingCase.Range);
					//let the canon rest for a while until it can shoot again
					iCurrentFrame = 0;
					iJustShotAtAirplane = true;
					iCurrentState = EAAArteriallyAIWaiting;
				}
			}
			//turn Gun
			else if(CMath::Abs(iShootingCase.NeededAngel - iAAArterially->GetCurrentGunAngel()) < iAAArterially->GetGunRotationSpeed())
			{
				//difference is so small that by the next rotation we will be on the right angel, by might over turn it
				//therefore need to set it to the right angel explicitly
				TInt lChangeAngel = iShootingCase.NeededAngel - iAAArterially->GetCurrentGunAngel();
				iAAArterially->ChangeGunAngel(lChangeAngel);
			}else
			{
				//need to turn gun to the correct Angel position
				//check if we need to turn up or down
				if(iShootingCase.NeededAngel > iAAArterially->GetCurrentGunAngel())
				{
					//need to increase angel
					iAAArterially->ChangeGunAngel(iAAArterially->GetGunRotationSpeed());
				}else
				{
					//need to decrease angel
					iAAArterially->ChangeGunAngel(-iAAArterially->GetGunRotationSpeed());
				}
			}
		}
			break;
	}
}

TBool CAAArteriallyAI::TargetEnemyPlane()
{
	CPointerArray<CGameObject>* lGameObjectsInRange = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->ReturnObjectsInRange(iAAArterially->GetCurrentPosition().iX, TIntFloat::Convert(iAAArterially->GetRange()), EObjectTypesPlane);
	TBool lFoundTarget = false;

	//check all Objects of type Planes that are not on your side
	for(TInt lCurrentIndex = 0; lCurrentIndex < lGameObjectsInRange->GetCount(); lCurrentIndex++)
	{
		CGameObject* lCurrentGameObject = (*lGameObjectsInRange)[lCurrentIndex];
		if(lCurrentGameObject->GetConflictSide() != iAAArterially->GetConflictSide() && lCurrentGameObject->IsAlive())
		{
			//make a closer check calculating in the Y factor
			TInt lDistanceToPlane = CMath::DistanceBetweenPoints(iAAArterially->GetGunCoordinates(), lCurrentGameObject->GetCurrentPosition());
			if(lDistanceToPlane < iAAArterially->GetRange() && lDistanceToPlane > iAAArterially->GetMinRange())
			{
				//this plane qualifies to be targeted, congrats :)
				CAircraft* lCurrentPlane = static_cast<CAircraft*> (lCurrentGameObject);
				TPointIntFloat lPlanePosition = lCurrentPlane->GetCurrentPosition();
				//find angel that we need to shoot at to hit the plane at the current Location
				TInt lAngel = CMath::AngleFromOriginBetweenPoints(iAAArterially->GetGunCoordinates(), lPlanePosition);
				//check if angel is reachable for the gun
				if(lAngel > iAAArterially->GetGunMinAngel() && lAngel < 180 - iAAArterially->GetGunMinAngel())
				{
					//approximate time to reach aircraft
					TInt lFramesNeededToTurnGun = CMath::Abs(lAngel - iAAArterially->GetCurrentGunAngel()) / iAAArterially->GetGunRotationSpeed();
					TInt lFramesNeededForProjectileToReachTarget = lDistanceToPlane / iAAArterially->GetProjectileSpeed();
					TInt lTotalFramesNeededToReachPlane = lFramesNeededForProjectileToReachTarget + lFramesNeededToTurnGun;
					//need to find new location of aircraft with current speed
					lPlanePosition.iX += (*lCurrentPlane->GetPixelsPerMoveX()) * lTotalFramesNeededToReachPlane;
					lPlanePosition.iY += (*lCurrentPlane->GetPixelsPerMoveY()) * lTotalFramesNeededToReachPlane;

					//now have the most likely position of the Aircraft, now we just to find the angel we need to make with this position
					ShootingCase lShootingOrder;
					lShootingOrder.NeededAngel = CMath::AngleFromOriginBetweenPoints(iAAArterially->GetGunCoordinates(), lPlanePosition);
					lShootingOrder.Range = CMath::DistanceBetweenPoints(iAAArterially->GetGunCoordinates(), lPlanePosition);
					//check if angel and range can be shot at
					if(lShootingOrder.NeededAngel > iAAArterially->GetGunMinAngel() && lShootingOrder.NeededAngel < 180 - iAAArterially->GetGunMinAngel() && lShootingOrder.Range < iAAArterially->GetRange() && lShootingOrder.Range > iAAArterially->GetMinRange())
					{

						TBool lIsGoodToShoot = true;
						if(iAAArterially->GetProjectileExplosionWidth() > 0) // causes an explosion, need to perform check that we don't hit friendly planes
						{
							//need to perform check if projectile might hit an friendly aircraft         
							CAircraft* lCurrentFriendlyPlane = NULL;
							CGameObject* lCurrentFriendlyGameObject = NULL;
							// where the actual explosion will be centered, needs to be shifted down and to the side depending on the explosion width and height
							TPointIntFloat lExplosionLocation;
							lExplosionLocation.iX = lPlanePosition.iX - TIntFloat::Convert(iAAArterially->GetProjectileExplosionWidth()) / 2;
							lExplosionLocation.iY = lPlanePosition.iY - TIntFloat::Convert(iAAArterially->GetProjectileExplosionWidth()) / 2;
							TBool lObjectReflected = false; //need to create a hit box
							CHitBox* lExplosionHitBox = CHitBox::New(&lExplosionLocation, &lObjectReflected, TSize(iAAArterially->GetProjectileExplosionWidth(), iAAArterially->GetProjectileExplosionWidth()));
							for(TInt lCurrentFriendlyIndex = 0; lCurrentFriendlyIndex < lGameObjectsInRange->GetCount(); lCurrentFriendlyIndex++)
							{
								lCurrentFriendlyGameObject = (*lGameObjectsInRange)[lCurrentFriendlyIndex];
								if(lCurrentFriendlyGameObject->IsAlive() && lCurrentFriendlyGameObject->GetConflictSide() == iAAArterially->GetConflictSide())
								{//the plane is on our side, and is still alive. Perform check if our explosion might harm it if shoot to the target location
									lCurrentFriendlyPlane = static_cast<CAircraft*> (lCurrentFriendlyGameObject);
									TPointIntFloat lFriendlyOldPosition = lCurrentFriendlyPlane->GetCurrentPosition();
									TPointIntFloat lFriendlyNewPosition = lFriendlyOldPosition;
									lFriendlyNewPosition.iX += (*lCurrentFriendlyPlane->GetPixelsPerMoveX()) * lTotalFramesNeededToReachPlane;
									lFriendlyNewPosition.iY += (*lCurrentFriendlyPlane->GetPixelsPerMoveY()) * lTotalFramesNeededToReachPlane;
									lCurrentFriendlyPlane->SetPositionWithoutTextureObjectUpdate(lFriendlyNewPosition);

									//have now the plane at location where it should be if it keeps moving until the projectile hits its target location
									if(lExplosionHitBox->IntersectionL(lCurrentFriendlyPlane->GetHitBox()))
									{
										//explosion would hit our friendly plane, don't shoot at this plane
										lIsGoodToShoot = false;
										lCurrentFriendlyPlane->SetPositionWithoutTextureObjectUpdate(lFriendlyOldPosition);
										break;
									}
									//move plane back to hits original position, since it didn't really move there
									lCurrentFriendlyPlane->SetPositionWithoutTextureObjectUpdate(lFriendlyOldPosition);
								}
							}
							delete lExplosionHitBox; //all done, clean up heap memory                 
						}
						//would this target location hit any friendlies?
						if(lIsGoodToShoot)
						{
							iShootingCase = lShootingOrder;
							lFoundTarget = true;
							break;//done, found a plane and it will be shot at next
						}
					}
				}
			}
		}
	}
	//clean up
	lGameObjectsInRange->Clear();
	delete lGameObjectsInRange;//needs to be delete as well since it resides on the heap

	return lFoundTarget;
}

void CAAArteriallyAI::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iWaitingCase.FrameDelay);
	aOutputStream.WriteInt32(iWaitingCase.FrameDelayAfterShot);
	aOutputStream.WriteInt32(iShootingCase.NeededAngel);
	aOutputStream.WriteInt32(iShootingCase.Range);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iShotsPerBurstFire);
	aOutputStream.WriteInt32(iWaitFramesUntilNextBurstShot);
	aOutputStream.WriteInt32(iBurstFireMode);
	aOutputStream.WriteInt32(iJustShotAtAirplane);
	aOutputStream.WriteInt32(iCurrentShotInBurst);
	aOutputStream.WriteInt32(iCurrentState);
}

void CAAArteriallyAI::LoadFromDisk(CFileReadStream &aReadStream)
{
	iWaitingCase.FrameDelay = aReadStream.ReadInt32();
	iWaitingCase.FrameDelayAfterShot = aReadStream.ReadInt32();
	iShootingCase.NeededAngel = aReadStream.ReadInt32();
	iShootingCase.Range = aReadStream.ReadInt32();
	iCurrentFrame = aReadStream.ReadInt32();
	iShotsPerBurstFire = aReadStream.ReadInt32();
	iWaitFramesUntilNextBurstShot = aReadStream.ReadInt32();
	iBurstFireMode = aReadStream.ReadInt32();
	iJustShotAtAirplane = aReadStream.ReadInt32();
	iCurrentShotInBurst = aReadStream.ReadInt32();
	iCurrentState = static_cast<TAAArteriallyAICurrentState> (aReadStream.ReadInt32());
}

