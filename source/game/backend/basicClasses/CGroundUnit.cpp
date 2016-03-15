/*
 ============================================================================
 Name		: GroundUnit.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGroundUnit implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CGroundUnit.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/core/utility/CRandomGenerator.h"

CGroundUnit::CGroundUnit(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aMaxSpeed, TInt aMaxWaitFramesUntilShoots, TInt aFramesNeededForFiringAnimation) :
	CMoveableGameObject(aZCor, aTextureObject, aAnimationPlayer, aLocation, aAttributes, BASIC_GROUND_UNIT_DEFAULT_FRAMES_PER_MOVE_HEALTH)
{
	iSpeed = TIntFloat::Convert(0);
	iGroundUnitMaxSpeed = aMaxSpeed;
	iCurrentState = EGroundUnitStateStanding;
	iMaxWaitFramesUntilNextShoot = aMaxWaitFramesUntilShoots;
	iFramesNeededForFiringAnimation = aFramesNeededForFiringAnimation;
	iCurrentWaitFrameForNextShoot = CGame::Game->iRandomGenerator->GetRandomNumberTInt(1, iMaxWaitFramesUntilNextShoot);
	iJustShoot = false;
	iSmokeCreator = NULL;
}

CGroundUnit::~CGroundUnit()
{
	if(iSmokeCreator != NULL)
	{
		delete iSmokeCreator;
		iSmokeCreator = NULL;
	}
}

//--------------- functions -----------------//

void CGroundUnit::Update()
{
	//need to update state of weapon if any attached
	UpdateSpecific();

	if(iCurrentState == EGroundUnitStateFiring)
	{
		iCurrentFrame++;
		if(iJustShoot)// wait a certain time to make sure the shooting animation is done for this ground unit
		{
			if(iCurrentFrame > iFramesNeededForFiringAnimation)
			{
				iJustShoot = false;
				iCurrentFrame = 0;
			}
		}else if(iCurrentFrame > iCurrentWaitFrameForNextShoot) //to always make sure it will shoot at some point
		{
			iCurrentFrame = 0;
			Shoot();
			//find out when the next shoot should take place
			iCurrentWaitFrameForNextShoot = CGame::Game->iRandomGenerator->GetRandomNumberTInt(iMaxWaitFramesUntilNextShoot, iMaxWaitFramesUntilNextShoot * 2);
		}
	}

	if(iSmokeCreator != NULL)
	{
		iSmokeCreator->Update();
	}

	iAnimationPlayer->Update();
}

void CGroundUnit::Move()
{
	if(iCurrentState == EGroundUnitStateMoving)
	{
		iCoordinates.iX += iPixelsPerMoveX;
		iTextureObject->ChangeXCoordinate(iPixelsPerMoveX);
	}
}

void CGroundUnit::Die(TDamageType aDamageType)
{
	if(!iAlive)
		return;
	
	MoveableObjectBasicDieRoutine();
	if(iGameObjectType == EObjectTypesArmouredGroundUnit)
		GameObjectMoveIntoBackground();//since this objects are quite large and might obscure objects standing infront of them

	//change states
	iCurrentState = EGroundUnitStateDieing;
	UpdateCurrentState(EGroundUnitStateDieing);
}

void CGroundUnit::Shoot() //just a stub function
{
	//do nothing
}

TIntFloat CGroundUnit::GetMaxSpeed()
{
	return iGroundUnitMaxSpeed;
}

void CGroundUnit::SetCurrentSpeed(TIntFloat aSpeed)
{
	iSpeed = aSpeed;
	iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

void CGroundUnit::SwitchToFiringState()
{
	if(iAlive && iCurrentState != EGroundUnitStateFiring) //only if we are alive and haven't changed into this state yet
	{
		iCurrentState = EGroundUnitStateFiring;
		UpdateCurrentState(EGroundUnitStateFiring);//update animation player
	}
}

void CGroundUnit::SwitchToStandingState()
{
	if(iAlive && iCurrentState != EGroundUnitStateStanding) //only if we are alive and haven't changed into this state yet
	{
		iCurrentState = EGroundUnitStateStanding;
		UpdateCurrentState(EGroundUnitStateStanding);//update animation player
	}
}

void CGroundUnit::SwitchToMovingState()
{
	if(iAlive && iCurrentState != EGroundUnitStateMoving) //only if we are alive and haven't changed into this state yet
	{
		iCurrentState = EGroundUnitStateMoving;
		UpdateCurrentState(EGroundUnitStateMoving);//update animation player
	}
}

void CGroundUnit::UpdateCurrentState(TGroundUnitStates aState)
{
	//stub function, has to be implemented by child
}

void CGroundUnit::UpdateSpecific()
{
	//stub function, has to be implemented by child
}

void CGroundUnit::GroundUnitObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	MoveableGameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteIntFloat(iGroundUnitMaxSpeed);
	aOutputStream.WriteInt32(iMaxWaitFramesUntilNextShoot);
	aOutputStream.WriteInt32(iFramesNeededForFiringAnimation);
	aOutputStream.WriteInt32(iCurrentWaitFrameForNextShoot);
	aOutputStream.WriteInt32(iJustShoot);
	aOutputStream.WriteInt32(iCurrentState);
}
void CGroundUnit::GroundUnitObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	MoveableGameObjectLoadContentFromDisk(aReadStream);
	iGroundUnitMaxSpeed = aReadStream.ReadIntFloat();
	iMaxWaitFramesUntilNextShoot = aReadStream.ReadInt32();
	iFramesNeededForFiringAnimation = aReadStream.ReadInt32();
	iCurrentWaitFrameForNextShoot = aReadStream.ReadInt32();
	iJustShoot = aReadStream.ReadInt32();
	iCurrentState = static_cast<TGroundUnitStates> (aReadStream.ReadInt32());
}

void CGroundUnit::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
}
void CGroundUnit::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
}
