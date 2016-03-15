/*
 ============================================================================
 Name		: Aircraft.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CAircraft implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
#include "includes/core/collision/CHitBox.h"

CAircraft::CAircraft(TInt aZValue, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aCurrentAcceleration, TIntFloat aMaxAcceleration, TBool aFlying, TInt aTurnTimeFactor, TInt aAngleChangeTimeFactor) :
	CMoveableGameObject(aZValue, aTextureObject, aAnimationPlayer, aLocation, aAttributes, BASIC_AIRCAFT_DEFAULT_FRAMES_PER_MOVE)
{
	iCurrentEngineAcceleration = aCurrentAcceleration;
	iEngineSpeed = aCurrentAcceleration;
	iTotalSpeed = aCurrentAcceleration; //to planes have already some speed if they have the acceleration
	iMaxEngineAcceleration = aMaxAcceleration / BASIC_AIRCARFT_MAX_ACCELERATION_FACTOR;
	iNormalMaxEngineAcceleration = iMaxEngineAcceleration;
	iCurrentDiveAcceleration =  TIntFloat::Convert(0);
	iDiveSpeed = TIntFloat::Convert(0);
	iExtraAngelAcceleration = TIntFloat::Convert(0);
	iFrameInTurnAnimation = 0;
	iExtraDownSpeed = TIntFloat::Convert(0);
	iAngleChangeExtraSpeed = TIntFloat::Convert(0);
	iAircraftChangeDownAngleExtraSpeed = TIntFloat::Convert(BASIC_AIRCARFT_CHANGE_ANGLE_SPEED_CHANGE_DOWN);//usually is always the same, but for bigger planes like bomber it might be more
	if(iCurrentEngineAcceleration < TIntFloat::Convert(BASIC_AIRCARFT_ACCELERATION_TOO_FAST_TO_LAND))
		iGearUp = false;
	else
		iGearUp = true;
	iIsFlying = aFlying;
	//set to default value for every plane
	if(iObjectReflected)
		iAngle = 0;
	else
		iAngle = 180;
	iIsTurning = false;
	iIsUpsideDown = false;
	iCrashed = false;
	iIsSinking = false;
	iMinimumYPosition = TIntFloat::Convert(0);
	iLandingGearTextureObject = NULL;
	iSmokeCreator1 = NULL;
	iFrontWheel = NULL;
	iBackWheel = NULL;
	iFighterAI = NULL;
	iFramesSinceLastAngleChange = 0;

	//find out fast this plane can turn
	iFramesPerTurnImage =  (aTurnTimeFactor * GFRAMES_PER_SECOND_BACKEND)  / (AIRCRAFT_NUMBER_OF_IMAGES_FOR_TOTAL_TURN * BASIC_AIRCRAFT_TURN_BASE_FACTOR);
	//find out how many frames it need for an angle change
	iFramesNeededPerAngleChange = (aAngleChangeTimeFactor + BASIC_AIRCRAFT_ANGLE_CHANGE_BASE_FACTOR_ADJUSTMENT) / BASIC_AIRCRAFT_ANGLE_CHANGE_BASE_FACTOR;
	if(iFramesNeededPerAngleChange < 0)
		iFramesNeededPerAngleChange = 1; //has to be at least 1 frame

	//updates the PixelMove Values
	AdjustFlyingSpeedDueToFlyingAngel();
}

CAircraft::~CAircraft()
{
	if(iFrontWheel)
	{
		delete iFrontWheel;
		iFrontWheel = NULL;
	}
	if(iBackWheel)
	{
		delete iBackWheel;
		iBackWheel = NULL;
	}
	if(iLandingGearTextureObject)
	{
		delete iLandingGearTextureObject;
		iLandingGearTextureObject = NULL;
	}
	if(iSmokeCreator1)
	{
		delete iSmokeCreator1;
		iSmokeCreator1 = NULL;
	}
	if(iFighterAI)
	{
		delete iFighterAI;
		iFighterAI = NULL;
	}
}

void CAircraft::ConstructL()
{
}

//------------------functions-----------------------

void CAircraft::MoveToLocation(TPoint aNewLocation)
{
	//do nothing, disableded for planes for now
	aNewLocation.iX = 0; //remove warning
}

void CAircraft::Move()
{
	if(iIsSinking)
	{
		ChangeYPosition(-(TIntFloat::Convert(BASIC_AIRCRAFT_SINKING_FACTOR)));//simulate sinking
	}

	//update X and Y location
	iCoordinates.iX += iPixelsPerMoveX;
	iTextureObject->ChangeXCoordinate(iPixelsPerMoveX);
	if(iLandingGearTextureObject)
		iLandingGearTextureObject->ChangeXCoordinate(iPixelsPerMoveX);

	if(!iCrashed)
	{
		iCoordinates.iY += iPixelsPerMoveY;
		iTextureObject->ChangeYCoordinate(iPixelsPerMoveY);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->ChangeYCoordinate(iPixelsPerMoveY);
	}
}

void CAircraft::Update()
{
	if(iIsTurning && !iCrashed)
	{
		TInt lFramesForStartTurn = AIRCRAFT_NUMBER_OF_IMAGES_FOR_START_TURN * iFramesPerTurnImage;
		TInt lFramesForMiddleTurn = AIRCRAFT_NUMBER_OF_IMAGES_PER_MIDDLE_PART * iFramesPerTurnImage;
		TInt lFramesForFinishTurn = AIRCRAFT_NUMBER_OF_IMAGES_FOR_FINISH_TURN * iFramesPerTurnImage;

		//plane now needs to follow a sequence of predefined steps during the turn
		if(iFrameInTurnAnimation < lFramesForStartTurn)
		{
			//keeps moving to the same direction
			iAnimationPlayer->Update();
			//iSmokeCreator1->Update();
			UpdateSpeed();
		}else if(iFrameInTurnAnimation >= lFramesForStartTurn && iFrameInTurnAnimation < lFramesForMiddleTurn + lFramesForStartTurn)
		{
			//should only move a little, is in the middle of the turn
			if(iObjectReflected)
				iPixelsPerMoveX = TIntFloat::Convert(3);
			else
				iPixelsPerMoveX = TIntFloat::Convert(-3);
			iPixelsPerMoveY = TIntFloat::Convert(-1);
			iAnimationPlayer->Update();
			//iSmokeCreator1->Update();
		}else if(iFrameInTurnAnimation >= lFramesForMiddleTurn + lFramesForStartTurn && iFrameInTurnAnimation < lFramesForMiddleTurn + lFramesForStartTurn + lFramesForFinishTurn)
		{
			//keep moving into opposite direction
			iAnimationPlayer->Update();
			//iSmokeCreator1->Update();
			UpdateSpeed();
		}

		// these are special cases that change how the plane is flying
		if(iFrameInTurnAnimation == 0)
		{
			//beginning of turn, should turn a little bit down
			if(iIsUpsideDown)
			{
				if(iObjectReflected)
					iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
				else
					iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
			}
			else
			{
				if(iObjectReflected)
					iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
				else
					iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
			}
			AdjustFlyingSpeedDueToFlyingAngel();
		}else if(iFrameInTurnAnimation == lFramesForMiddleTurn + lFramesForStartTurn)
		{
			//correction needed for the hitbox to be located at the right location
			iObjectReflected = !iObjectReflected;
			iTextureObject->ReflectOverYAxis();
			iCoordinates = iTextureObject->ReturnCurrentFixPoint();
			iTextureObject->ReflectOverYAxis();//undue since imgaes should not be reflected
			//now moving to the opposite direction
			if(iIsUpsideDown)
			{
				if(!iObjectReflected)
					iAngle += (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
				else
					iAngle -= (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
			}
			else
			{
				if(!iObjectReflected)
					iAngle -= (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
				else
					iAngle += (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
			}
			AdjustFlyingSpeedDueToFlyingAngel();
		}else if(iFrameInTurnAnimation == lFramesForMiddleTurn + lFramesForStartTurn + lFramesForFinishTurn)
		{
			if(iIsUpsideDown)
			{
				if(!iObjectReflected)
					iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
				else
					iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
			}
			else
			{
				if(!iObjectReflected)
					iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
				else
					iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
			}

			AdjustFlyingSpeedDueToFlyingAngel();
			iTextureObject->ReflectOverYAxis();
			if(iLandingGearTextureObject)
				iLandingGearTextureObject->ReflectOverYAxis();
			iIsTurning = false;
			AfterTurnSettings();
			UpdateAircraft();
		}
		iFrameInTurnAnimation++;
	}else
	{
		UpdateAircraft();
		iFramesSinceLastAngleChange++;
	}
}

void CAircraft::AngleCorrectionIfCrashedDuringTurn()
{
	if(iIsTurning)
	{
		TInt lFramesForStartTurn = AIRCRAFT_NUMBER_OF_IMAGES_FOR_START_TURN * iFramesPerTurnImage;
		TInt lFramesForMiddleTurn = AIRCRAFT_NUMBER_OF_IMAGES_PER_MIDDLE_PART * iFramesPerTurnImage;
		TInt lFramesForFinishTurn = AIRCRAFT_NUMBER_OF_IMAGES_FOR_FINISH_TURN * iFramesPerTurnImage;

		//need to return the angle back to its original position so that it is in sync with the texture object
		if(iFrameInTurnAnimation < lFramesForStartTurn + lFramesForMiddleTurn)
		{
			if(iFrameInTurnAnimation != 0) //this means that the turn code at ran once at least
			{
				if(iIsUpsideDown)
				{
					if(iObjectReflected)
						iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
					else
						iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
				}
				else
				{
					if(iObjectReflected)
						iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
					else
						iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
				}
			}
		}
		else
		{
			//complete the turn, are in the middle of the turn, still need to the reflection
			if(iFrameInTurnAnimation == lFramesForStartTurn + lFramesForMiddleTurn)
			{
				//correction needed for the hitbox to be located at the right location
				iObjectReflected = !iObjectReflected;
				iTextureObject->ReflectOverYAxis();
				iCoordinates = iTextureObject->ReturnCurrentFixPoint();
				iTextureObject->ReflectOverYAxis();//undue since imgaes should not be reflected

				if(iIsUpsideDown)
				{
					if(!iObjectReflected)
					{
						iAngle += (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
					}else
					{
						iAngle -= (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
					}
				}
				else
				{
					if(!iObjectReflected)
					{
						iAngle -= (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
					}else
					{
						iAngle += (180 - (BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE * 2));
					}
				}
			}
			//just need to correct the angle and the last corrections
			if(iFrameInTurnAnimation <= lFramesForMiddleTurn + lFramesForStartTurn + lFramesForFinishTurn)
			{
				if(iIsUpsideDown)
				{
					if(!iObjectReflected)
					{
						iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
					}else
					{
						iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
					}
				}
				else
				{
					if(!iObjectReflected)
					{
						iAngle -= BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
					}else
					{
						iAngle += BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE;
					}
				}
			}
			iTextureObject->ReflectOverYAxis();
			if(iLandingGearTextureObject)
				iLandingGearTextureObject->ReflectOverYAxis();
		}
		iAnimationPlayer->CancelCurrentAnimation();//stop turning animation
		AdjustFlyingSpeedDueToFlyingAngel();
		iIsTurning = false;
		AfterTurnSettings();
	}
}

void CAircraft::UpdateAircraft()
{
	//stub
}

void CAircraft::BeforeTurnSettings()
{
	//stub
}

void CAircraft::AfterTurnSettings()
{
	//stub
}

//stub
void CAircraft::Fire1()
{
}
//stub
void CAircraft::Fire2()
{
}
//stub
TBool CAircraft::Fire1HasAmmunitionLeft()
{
	return true;
}
//stub
TBool CAircraft::Fire2HasAmmunitionLeft()
{
	return true;
}
//must increase Angel in 15 degree units to increase hight if min speed is 4
void CAircraft::IncreaseSpeed()
{
	//increase the plane speed if not yet maximum
	iCurrentEngineAcceleration += TIntFloat::Convert(BASIC_AIRCRAFT_CHANGE_ACCElERATION_STEP_SIZE);
	if(iCurrentEngineAcceleration > iMaxEngineAcceleration)//can't be faster than the plane capable to handle
		iCurrentEngineAcceleration = iMaxEngineAcceleration;
}

void CAircraft::DecreaseSpeed()
{
	//decrease the plane speed if not yet minimum
	iCurrentEngineAcceleration -= TIntFloat::Convert(BASIC_AIRCRAFT_CHANGE_ACCElERATION_STEP_SIZE);
	if(iCurrentEngineAcceleration < 0)//only when not flying we can be slower
		iCurrentEngineAcceleration = TIntFloat::Convert(0);
}

void CAircraft::IncreaseFlyingAngleSmallAngle()
{
	IncreaseFlyingAngle(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE_SMALL);
}
void CAircraft::DecreaseFlyingAngleSmallAngle()
{
	DecreaseFlyingAngle(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE_SMALL);
}
void CAircraft::DecreaseFlyingAngleWithoutSpeedCheckSmallAngle()
{
	DecreaseFlyingAngleWithoutSpeedCheck(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE_SMALL);
}
void CAircraft::IncreaseFlyingAngleWithoutSpeedCheckSmallAngle()
{
	IncreaseFlyingAngleWithoutSpeedCheck(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE_SMALL);
}

void CAircraft::IncreaseFlyingAngle()
{
	IncreaseFlyingAngle(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE);
}
void CAircraft::DecreaseFlyingAngle()
{
	DecreaseFlyingAngle(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE);
}
void CAircraft::DecreaseFlyingAngleWithoutSpeedCheck()
{
	DecreaseFlyingAngleWithoutSpeedCheck(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE);
}
void CAircraft::IncreaseFlyingAngleWithoutSpeedCheck()
{
	IncreaseFlyingAngleWithoutSpeedCheck(BASIC_AIRCRAFT_FLYING_ANGEL_CHANGE_STEP_SIZE);
}

void CAircraft::IncreaseFlyingAngle(TInt aChange)
{
	if(iIsTurning || iFramesSinceLastAngleChange <= iFramesNeededPerAngleChange) //can't change during turning
		return;

	TIntFloat lAngleChangeExtraSpeed;
	TBool lPointingMoreUp = false;
	TInt lOldDiffBetweenTopAngel;
	TInt lNewDiffBetweenTopAngel;
	//need to check if we can fly at this angel
	if(iObjectReflected) // kind of messed up during 0 and 270 degress
		if((270 - aChange) < iAngle && iAngle < 360)
			lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle - aChange));
		else
			lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle + aChange));
	else if((270 < iAngle && iAngle < 360))
		lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle + aChange));
	else
		lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle - aChange));
	lOldDiffBetweenTopAngel = CMath::Abs(90 - iAngle);

	if(lNewDiffBetweenTopAngel > lOldDiffBetweenTopAngel)
	{
		lPointingMoreUp = false;
		lAngleChangeExtraSpeed = iAircraftChangeDownAngleExtraSpeed;
	}else
	{
		lPointingMoreUp = true;
		lAngleChangeExtraSpeed = TIntFloat::Convert(BASIC_AIRCARFT_CHANGE_ANGEL_SPEED_CHANGE_UP);//points more up
	}

	TIntFloat lFallingSpeed = CMath::Abs(lAngleChangeExtraSpeed + iTotalSpeed) - (TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING)); //plane is too slow
	if(lPointingMoreUp && lFallingSpeed < TIntFloat::Convert(0) && iAngle != 270 && !iCrashed)
	{
		return; //plane is trying to point up but is too slow
	}
	iAngleChangeExtraSpeed += lAngleChangeExtraSpeed;

	if(iObjectReflected) //need to be aware what direction we are facing
	{
		//increase the flying angel at a predertimed rate
		iAngle += aChange;
		iTextureObject->RotateVertexes(aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(aChange);
	}else
	{
		//increase the flying angel at a predertimed rate
		iAngle -= aChange;
		iTextureObject->RotateVertexes(-aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(-aChange);
	}

	CMath::AdjustAngleToUnitCircle(iAngle);

	AdjustFlyingSpeedDueToFlyingAngel();
	iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	iFramesSinceLastAngleChange = 0; //reset so that plane needs to wait until it can change again
}

void CAircraft::DecreaseFlyingAngleWithoutSpeedCheck(TInt aChange)
{
	if(iIsTurning) //can't change during turning
		return;

	if(iObjectReflected)
	{
		//increase the flying angel at a predertimed rate
		iAngle -= aChange;
		iTextureObject->RotateVertexes(-aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(-aChange);
	}else
	{
		//increase the flying angel at a predertimed rate
		iAngle += aChange;
		iTextureObject->RotateVertexes(aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(aChange);
	}
	//check if negative, if so, add 360
	CMath::AdjustAngleToUnitCircle(iAngle);

	AdjustFlyingSpeedDueToFlyingAngel();
	iCoordinates = iTextureObject->ReturnCurrentFixPoint();
}

void CAircraft::IncreaseFlyingAngleWithoutSpeedCheck(TInt aChange)
{
	if(iIsTurning) //can't change during turning
		return;

	if(iObjectReflected) //need to be aware what direction we are facing
	{
		//increase the flying angel at a predertimed rate
		iAngle += aChange;
		iTextureObject->RotateVertexes(aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(aChange);
	}else
	{
		//increase the flying angel at a predertimed rate
		iAngle -= aChange;
		iTextureObject->RotateVertexes(-aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(-aChange);
	}
	//check if the flying angel is more than 360 or equal, substract 360 since it remains the same angel in a unit circle
	CMath::AdjustAngleToUnitCircle(iAngle);

	AdjustFlyingSpeedDueToFlyingAngel();
	iCoordinates = iTextureObject->ReturnCurrentFixPoint();
}

void CAircraft::DecreaseFlyingAngle(TInt aChange)
{
	if(iIsTurning || iFramesSinceLastAngleChange <= iFramesNeededPerAngleChange) //can't change during turning
		return;

	if(!iIsFlying) 	//can't go down if we are not flying
		return;

	//need to check if we can fly at this angel
	TIntFloat lAngleChangeExtraSpeed;
	TBool lPointingMoreUp = false;
	TInt lNewDiffBetweenTopAngel;
	TInt lOldDiffBetweenTopAngel;
	//need to check if we can fly at this angel
	if(iObjectReflected)
		if(270 < iAngle && iAngle < 360) // kind of messed up during 0 and 270 degress
			lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle + aChange));
		else
			lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle - aChange));
	else if((270 <= iAngle && iAngle < 360))
		lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle - aChange));
	else
		lNewDiffBetweenTopAngel = CMath::Abs(90 - (iAngle + aChange));
	lOldDiffBetweenTopAngel = CMath::Abs(90 - iAngle);

	if(lNewDiffBetweenTopAngel > lOldDiffBetweenTopAngel)
	{
		lPointingMoreUp = false;
		lAngleChangeExtraSpeed = iAircraftChangeDownAngleExtraSpeed;
	}else
	{
		lPointingMoreUp = true;
		lAngleChangeExtraSpeed = TIntFloat::Convert(BASIC_AIRCARFT_CHANGE_ANGEL_SPEED_CHANGE_UP);//points more up
	}

	TIntFloat lFallingSpeed =CMath::Abs(lAngleChangeExtraSpeed + iTotalSpeed) - (TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING)); //plane is too slow
	if(lPointingMoreUp && lFallingSpeed < 0 && iAngle != 270 && !iCrashed && lNewDiffBetweenTopAngel > lOldDiffBetweenTopAngel)
	{
		return;//plane is trying to point up but is too slow
	}
	iAngleChangeExtraSpeed += lAngleChangeExtraSpeed;

	if(iObjectReflected)
	{
		//increase the flying angel at a predertimed rate
		iAngle -= aChange;
		iTextureObject->RotateVertexes(-aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(-aChange);
	}else
	{
		//increase the flying angel at a predertimed rate
		iAngle += aChange;
		iTextureObject->RotateVertexes(aChange);
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->RotateVertexes(aChange);
	}
	//check if negative, if so, add 360
	CMath::AdjustAngleToUnitCircle(iAngle);

	AdjustFlyingSpeedDueToFlyingAngel();
	iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	iFramesSinceLastAngleChange = 0; //reset so that plane needs to wait until it can change again
}

void CAircraft::AdjustFlyingSpeedDueToFlyingAngel()
{
	CMath::AdjustAngleToUnitCircle(iAngle);
	//change speed in calculate the length of the X and Y axis of the standard circle
	iXAxisLength = CMath::GraphicsCosTable(iAngle);
	iYAxisLength = CMath::GraphicsSinTable(iAngle);

	if(iObjectReflected)
	{
		if(iAngle >= 0 && iAngle < 90)
		{
			iIsUpsideDown = false;
		}else if(iAngle >= 90 && iAngle <= 180)
		{
			iIsUpsideDown = true;
		}else if(iAngle > 180 && iAngle < 270)
		{
			iIsUpsideDown = true;
		}else if(iAngle >= 270 && iAngle <= 360)
		{
			iIsUpsideDown = false;
		}
	}else
	{
		if(iAngle >= 0 && iAngle <= 90)
		{
			iIsUpsideDown = true;
		}else if(iAngle > 90 && iAngle <= 180)
		{
			iIsUpsideDown = false;
		}else if(iAngle > 180 && iAngle <= 270)
		{
			iIsUpsideDown = false;
		}else if(iAngle > 270 && iAngle <= 360)
		{
			iIsUpsideDown = true;
		}
	}

	//update how many pixels the plane will move
	AdjustFlyingSpeedDueToChanges();
}

void CAircraft::AdjustFlyingSpeedDueToChanges()
{
	//adjust X axis speed
	iPixelsPerMoveX = iTotalSpeed * iXAxisLength;
	//adjust Y axis speed
	iPixelsPerMoveY = iTotalSpeed * iYAxisLength;
}

void CAircraft::TurnLeftWithoutAnimationAndAngleChange()
{
	//can't turn left or right if flying straight down or up
	if(iAngle == 270 || iAngle == 90)
		return;

	//need to add anitation somehow,  do later
	if(iObjectReflected)
	{
		iObjectReflected = false;
		iTextureObject->ReflectOverYAxis();
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->ReflectOverYAxis();
		AdjustFlyingSpeedDueToFlyingAngel();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

void CAircraft::TurnRightWithoutAnimationAndAngleChange()
{
	//can't turn left or right if flying straight down or up
	if(iAngle == 270 || iAngle == 90)
		return;

	if(!iObjectReflected)
	{
		iObjectReflected = true;
		iTextureObject->ReflectOverYAxis();
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->ReflectOverYAxis();
		AdjustFlyingSpeedDueToFlyingAngel();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

//maybe just use 180 degree flip of standard bitmap
void CAircraft::TurnLeft()
{
	//can't turn left or right if flying straight down or up
	if(iAngle == 270 || iAngle == 90 || iIsTurning)
		return;

	//if we are on a landing surface do the turn immediatly and don't show an anitmation
	if(iIsFlying)
		TurnLeftWithoutCheck();
	else
		TurnLeftWithoutAnimationAndCheck();
}

//maybe just use 180 degree flip of standard bitmap
void CAircraft::TurnRight()
{
	//can't turn left or right if flying straight down or up
	if(iAngle == 270 || iAngle == 90 || iIsTurning)
		return;

	//if we are on a landing surface do the turn immediatly and don't show an anitmation
	if(iIsFlying)
		TurnRightWithoutCheck();
	else
		TurnRightWithoutAnimationAndCheck();
}

void CAircraft::TurnRightWithoutCheck()
{
	if(!iObjectReflected)
	{
		iFrameInTurnAnimation = 0;
		PlayTurnAnimation();
		BeforeTurnSettings();
		iIsTurning = true;
	}
}

void CAircraft::TurnLeftWithoutCheck()
{
	//need to add anitation somehow,  do later
	if(iObjectReflected)
	{
		iFrameInTurnAnimation = 0;
		PlayTurnAnimation();
		BeforeTurnSettings();
		iIsTurning = true;
	}
}

void CAircraft::TurnRightWithoutAnimationAndCheck()
{
	if(!iObjectReflected)
	{
		iObjectReflected = true;
		iTextureObject->ReflectOverYAxis();
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->ReflectOverYAxis();
		iAngle -= 180;
		AdjustFlyingSpeedDueToFlyingAngel();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

void CAircraft::TurnLeftWithoutAnimationAndCheck()
{
	if(iObjectReflected)
	{
		iObjectReflected = false;
		iTextureObject->ReflectOverYAxis();
		if(iLandingGearTextureObject)
			iLandingGearTextureObject->ReflectOverYAxis();
		iAngle += 180;
		AdjustFlyingSpeedDueToFlyingAngel();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

void CAircraft::PlayTurnAnimation()
{
	//stub function
}

TBool CAircraft::GetIsAIControlled()
{
	if(iFighterAI)
		return true;
	else
		return false;
}

TInt* CAircraft::GetFlyingAngel()
{
	return &iAngle;
}

void CAircraft::UpdateSpeed()
{
	//update the engine speed component
	if(iEngineSpeed < iCurrentEngineAcceleration)
	{
		//haven't reach maxiumun speed yet
		if(iCurrentEngineAcceleration - iEngineSpeed < TIntFloat::Convert(BASIC_AIRCRAFT_ENGINE_SPEED_UPDATE_FACTOR))
			iEngineSpeed = iCurrentEngineAcceleration;
		else
			iEngineSpeed += TIntFloat::Convert(BASIC_AIRCRAFT_ENGINE_SPEED_UPDATE_FACTOR);
	}else
	{
		//need to slow down
		if(iEngineSpeed - iCurrentEngineAcceleration < TIntFloat::Convert(BASIC_AIRCRAFT_ENGINE_SPEED_UPDATE_FACTOR))
			iEngineSpeed = iCurrentEngineAcceleration;
		else
			iEngineSpeed -= TIntFloat::Convert(BASIC_AIRCRAFT_ENGINE_SPEED_UPDATE_FACTOR);
	}
	//update the dive component
	iCurrentDiveAcceleration = iExtraAngelAcceleration + iExtraDownSpeed;
	if(iDiveSpeed < iCurrentDiveAcceleration)
	{
		if(iCurrentDiveAcceleration - iDiveSpeed < TIntFloat::Convert(BASIC_AIRCRAFT_DIVE_SPEED_UPDATE_FACTOR))
			iDiveSpeed = iCurrentDiveAcceleration;
		else
			iDiveSpeed += TIntFloat::Convert(BASIC_AIRCRAFT_DIVE_SPEED_UPDATE_FACTOR);
	}else
	{
		//need to slow down
		if(iDiveSpeed - iCurrentDiveAcceleration < TIntFloat::Convert(BASIC_AIRCRAFT_DIVE_SPEED_UPDATE_FACTOR))
			iDiveSpeed = iCurrentDiveAcceleration;
		else
			iDiveSpeed -= TIntFloat::Convert(BASIC_AIRCRAFT_DIVE_SPEED_UPDATE_FACTOR);
	}
	//sum all speeds
	iTotalSpeed = iDiveSpeed + iEngineSpeed + iAngleChangeExtraSpeed;
	//make sure speed is not negative
	if(iTotalSpeed < 0)
		iTotalSpeed = TIntFloat::Convert(0);

	//decay extra angle speed if that is the case
	if(CMath::Abs(iAngleChangeExtraSpeed) > 0)
	{
		iAngleChangeExtraSpeed = (iAngleChangeExtraSpeed * 90) / 100;
	}

	//check if the plane is flying too slow and needs to be turned down
	TIntFloat lFallingSpeed = CMath::Abs(iTotalSpeed) - (TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING)); //plane is too slow
	if(lFallingSpeed < 0 && iAngle != 270 && !iCrashed && iIsFlying && !iIsTurning)// the plane will not move up due to the up factor
	{
		if(iObjectReflected)
		{
			if(iAngle < 90 || iAngle > 270)
			{
				DecreaseFlyingAngleSmallAngle();//points more towards the ground
			}else
			{
				IncreaseFlyingAngleSmallAngle();//points more towards the ground
			}
		}else
		{
			if(iAngle > 90 && iAngle < 270)
			{
				DecreaseFlyingAngleSmallAngle();
			}else
			{
				IncreaseFlyingAngleSmallAngle();
			}
		}
	}

	if(iAngle < 360 && iAngle > 180 && !iCrashed)//plane is pointing down
	{
		iExtraAngelAcceleration = TIntFloat::Convert(BASIC_AIRCAFT_ACCELERATION_CHANGE_DUE_TO_ANGEL) * (iYAxisLength * iYAxisLength);//get some extra acceleration
		iExtraDownSpeed += (TIntFloat::Convert(BASIC_AIRCAFT_EXTRA_DOWN_SPEED) * CMath::Abs(iYAxisLength));//Multi(iYAxisLength, iYAxisLength));//get some extra acceleration
	}else
	{
		iExtraDownSpeed = TIntFloat::Convert(0);
		iExtraAngelAcceleration = TIntFloat::Convert(BASIC_AIRCAFT_ACCELERATION_CHANGE_DUE_TO_ANGEL) * (iYAxisLength * iYAxisLength) * TIntFloat::Convert(-1);//decrease acceleration
	}

	//speed of plane has most likely changed, adjust the X and Y components
	AdjustFlyingSpeedDueToChanges();

	//adjust speed if we are not flying, only if we the plane is not dead otherwise gear might come out when plane is shot down
	if(iAlive)
	{
		if(!iIsFlying && iTotalSpeed > TIntFloat::Convert(BASIC_AIRCRAFT_MAXIMUM_SPEED_ON_LANDING_SURFACE))
			iTotalSpeed = TIntFloat::Convert(BASIC_AIRCRAFT_MAXIMUM_SPEED_ON_LANDING_SURFACE);

		//also check if the plane has slowed down enough to let down the landing Gear
		if(iTotalSpeed < TIntFloat::Convert(BASIC_AIRCARFT_ACCELERATION_TOO_FAST_TO_LAND))
		{
			iGearUp = false;
		}else
		{
			iIsFlying = true; //has to be true
			iGearUp = true;
		}
	}
}

TBool CAircraft::AirplaneCrashed()
{
	return iCrashed;
}

//will be overriden by child if something should be done if hit by another fighter
void CAircraft::RunAIAvoidanceLogicCode()
{
	if(iFighterAI && !iIsTurning)
	{
		iFighterAI->GotHitByEnemyPlane();
	}
}

void CAircraft::PlayCrashSound(const TDamageType aType)
{
	switch(aType)
	{
		case EDamageTypePlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		case EDamageTypeAAGunPlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision_Cannon, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		case EDamageTypeBuildingPlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision_Building, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		case EDamageTypeShipPlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		case EDamageTypeTankPlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		case EDamageTypeTreePlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision_Tree, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		case EDamageTypeSoldierPlaneCollision:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision_Soldier, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			break;
		}
		default:
			break;
	}
}

void CAircraft::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	if(iAlive) //avoid playing sound again if aircraft is already dead
		PlayCrashSound(aType);

	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth < 0)
			iHealth = 0;

		if(iHealth < 80)
		{
			iMaxEngineAcceleration = (iNormalMaxEngineAcceleration * (iHealth + 20)) / 100; // too avoid that engine gets broken too fast

			if(iCurrentEngineAcceleration > iMaxEngineAcceleration)//reduce acceleration since engine has been damaged
				iCurrentEngineAcceleration = iMaxEngineAcceleration;
		}
		if(iHealth == 0)
		{
			Die(aType);
			if(iFrontWheel)
				iFrontWheel->PlaneDied(); //remove from landing surface if has died
			if(iBackWheel)
				iBackWheel->PlaneDied();
			AngleCorrectionIfCrashedDuringTurn();//too make sure that plane either finishes the started turn or goes back to the state before it made the turn
			iMaxEngineAcceleration = TIntFloat::Convert(0);
			iIsFlying = true;

			if(iCurrentEngineAcceleration > iMaxEngineAcceleration)//reduce acceleration since engine has been damaged
				iCurrentEngineAcceleration = iMaxEngineAcceleration;
			if(iEngineSpeed > (TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING)))//to avoid that a plane takes too long to drop down
				iEngineSpeed = (TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING));
		}
	}
	switch(aType)
	{
		case EDamageTypePlaneCrash:
		{
			iCrashed = true;
			iCoordinates.iY -= iTextureObject->GetHeight();
			//create new simple hitbox, to allow that the plane can sink
			delete iHitBox;
			iHitBox = NULL;
			iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize((iTextureObject->GetWidth().GetIntInBaseInt()), (iTextureObject->GetHeight().GetIntInBaseInt())));
			if(iSoundObject)
				iSoundObject->AssignHitBox(iHitBox);//need to update hitbox
			//class crash method of plane that might change the texture and do other things
			PlaneCrash();
			break;
		}
		case EDamageTypePlaneBullet:
		case EDamageTypeBullet:
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Hit_Bullet_Small, GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
			if(aType == EDamageTypePlaneBullet)
				RunAIAvoidanceLogicCode();
			break;
		}
		default:
			break;
	}
}

void CAircraft::SetPlaneToYPosition(TIntFloat lYPosition)
{
	TIntFloat lRequiredChangeInY = lYPosition - iCoordinates.iY;
	iCoordinates.iY += lRequiredChangeInY;
	lRequiredChangeInY = lYPosition - iTextureObject->ReturnCurrentFixPoint().iY;
	iTextureObject->ChangeYCoordinate(lRequiredChangeInY);
	if(iLandingGearTextureObject)
	{
		lRequiredChangeInY = lYPosition - iLandingGearTextureObject->ReturnCurrentFixPoint().iY;
		iLandingGearTextureObject->ChangeYCoordinate(lRequiredChangeInY);
	}
}

CFighterAI* CAircraft::GetFighterAI()
{
	return iFighterAI;
}

void CAircraft::SetAIControlled()
{
	if(!iFighterAI)
		iFighterAI = CFighterAI::New(this); //make sure no double initialization
}

TIntFloat CAircraft::GetTextureHeight()
{
	return iTextureObject->GetHeight();
}

//doesn't change with angel or direction
TIntFloat CAircraft::GetCurrentAbsoluteSpeed()
{
	return iTotalSpeed;
}

void CAircraft::SetCurrentAbsoluteSpeed(TIntFloat aSpeed)
{
	iTotalSpeed = aSpeed;
}

void CAircraft::SetIsSinking(TBool aValue)
{
	iIsSinking = aValue;
}

void CAircraft::ChangeYPosition(TIntFloat aChange)
{
	iCoordinates.iY += aChange;
	iTextureObject->ChangeYCoordinate(aChange);
	if(iLandingGearTextureObject)
		iLandingGearTextureObject->ChangeYCoordinate(aChange);
}

void CAircraft::ChangeXPosition(TIntFloat aChange)
{
	iCoordinates.iX += aChange;
	iTextureObject->ChangeXCoordinate(aChange);
	if(iLandingGearTextureObject)
		iLandingGearTextureObject->ChangeXCoordinate(aChange);
}

TBool CAircraft::IsUpSideDown()
{
	return iIsUpsideDown;
}

TBool CAircraft::IsTurning()
{
	return iIsTurning;
}

//stub
void CAircraft::PlaneCrash()
{
	//might be implemented by child, this function is called when the plane crashes with the floor
}

//stub
TInt CAircraft::GetAmmunitionLeftOfSecondaryWeapon()
{
	return 0;
}

void CAircraft::SetPlaneIsFlying(TBool aIsFlying)
{
	iIsFlying = aIsFlying;
}

CWeapon* CAircraft::GetSecondaryWeapon()
{
	return NULL;
}

CWeapon* CAircraft::GetPrimaryWeapon()
{
	return NULL;
}

void CAircraft::PossibleCollision(CInterval*& aInterval)
{
	if(iAlive && !iGearUp) //otherwise no landing possible
	{
		if(iFrontWheel)
		{
			iFrontWheel->PossibleCollision(aInterval);
		}
		if(iBackWheel)
		{
			iBackWheel->PossibleCollision(aInterval);
		}
	}
	if(!iCrashed) //check if the plane collides with a plane from the other conflict side
	{
		CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesBigPlane | EObjectTypesArmouredGroundUnit | EObjectTypesGroundUnit);

		for(TInt lIndex = 0; lIndex < lGameObjects->GetCount(); lIndex++)
		{
			CGameObject* lGameObject = lGameObjects->Get(lIndex);
			//only collide with alive objects
			TGameObjectType lObjectType = lGameObject->GetGameObjectType();
			//only collide with planes from a different conflict side
			if(lObjectType & EObjectTypesBigPlane)
			{
				if(lGameObject->IsAlive() && lGameObject->GetConflictSide() != iConflictSide && !GetIsAIControlled()) //AI controlled can't handle to avoid big planes, thus a quick fix is that they can't collide :)
				{
					//collide
					if(iHitBox->IntersectionL(lGameObject->GetHitBox()))
					{
						//make sure plane is dead
						SDamage lDamage;
						lDamage.Damage = KILL_ANY_AIRPLANE_DAMAGE;
						lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;//should kill any object

						//both planes die after collision
						lGameObject->TakeDamage(lDamage, EDamageTypePlaneCollision);
						TakeDamage(lDamage, EDamageTypePlaneCollision);
					}
				}
				break;
			}
			//collision with tanks
			else if(lObjectType & EObjectTypesArmouredGroundUnit)
			{
				//collide, Fighter AI can't collide since it usually doesn't unless AI is starting or landing from Airport which would be considered a bug. However, too complex to fix
				if((iIsFlying || iConflictSide != lGameObject->GetConflictSide()) //either the aircraft is flying or when it is landed it can only collide with tanks from the enemy side
						&& !iFighterAI && lGameObject->IsAlive() && iHitBox->IntersectionL(lGameObject->GetHitBox())
					)
				{
					//make sure plane is dead
					SDamage lDamage;
					lDamage.Damage = KILL_ANY_AIRPLANE_DAMAGE;
					lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;

					//maybe add a different collision sound for tank collision
					TakeDamage(lDamage, EDamageTypePlaneCollision);
				}
				break;
			}
			//only collide with soldiers from a different conflict side
			else if(lObjectType & EObjectTypesGroundUnit)
			{
				if(lGameObject->IsAlive())
				{
					//collide
					if(iIsFlying && iHitBox->IntersectionL(lGameObject->GetHitBox()))
					{
						CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Crush_Human, iHitBox);
						lSoundObject->PlayHitBox();
						delete lSoundObject;
						SDamage lDamage;
						lDamage.Damage = JAPANESE_RIFLEMAN_HEALTH;
						lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;
						lGameObject->TakeDamage(lDamage, EDamageTypeSoldierPlaneCollision);
						TakeDamage(lDamage, EDamageTypeSoldierPlaneCollision);
					}
				}
				break;
			}
		}

		//clean up
		delete lGameObjects;
	}else //plane crashed, but we want that the plane still can hurt foot soldiers if it has a bit of speed remaining - Sliding kill
	{
		CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesGroundUnit);

		//special case just for soliders
		for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

			//only collide with alive objects
			if(lCurrentGameObject->IsAlive())
			{
				if(iTotalSpeed > 0 && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
				{
					SDamage lDamage;
					lDamage.Damage = JAPANESE_RIFLEMAN_HEALTH;
					lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;
					//create temporary sound object to play sound
					CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Collision_Soldier, iHitBox);
					lSoundObject->PlayHitBox();
					delete lSoundObject;//need to clean it up
					lCurrentGameObject->TakeDamage(lDamage, EDamageTypeSoldierPlaneCollision);
					TakeDamage(lDamage, EDamageTypeSoldierPlaneCollision);
				}
			}
		}

		//clean up
		delete lGameObjects;
	}
}

TIntFloat CAircraft::GetCurrentEngineAcceleration()
{
	return iCurrentEngineAcceleration;
}

TIntFloat& CAircraft::GetCurrentEngineAccelerationRef()
{
	return iCurrentEngineAcceleration;
}

TIntFloat CAircraft::GetCurrentDiveAcceleration()
{
	return iCurrentDiveAcceleration;
}

TIntFloat& CAircraft::GetCurrentDiveAccelerationRef()
{
	return iCurrentDiveAcceleration;
}

TIntFloat CAircraft::GetTotalAcceleration()
{
	return iCurrentDiveAcceleration + iCurrentEngineAcceleration;
}

TIntFloat CAircraft::GetMaxEngineAccelerationWithCurrentHealth()
{
	return iMaxEngineAcceleration;
}

TIntFloat CAircraft::GetNormalMaxEngineAcceleration()
{
	return iNormalMaxEngineAcceleration;
}

void CAircraft::SetCurrentAcceleration(TIntFloat aValue)
{
	iCurrentEngineAcceleration = aValue;
}

TBool CAircraft::IsFlying()
{
	return iIsFlying;
}

TAircraftType& CAircraft::GetAircraftType()
{
	return iAircraftType;
}

//stub function must be implemented by child to rearm all its weapons
void CAircraft::RearmAircraft()
{

}

//stub function, must be implemented by child to have any effect, if the plane as a secondary weapon that is

void CAircraft::EquipWithNewSecondaryWeapon(TWeaponIdentifier aWeaponIdentifier)
{

}

void CAircraft::StopEngineSound()
{
	if(iHitBox)
	{
		TPointIntFloat lMiddleOfPlane = CMath::GetCenterOfRect(iHitBox->GetFastIntersectionRect());
		iSoundObject->PlayCheckPoint(lMiddleOfPlane, ESoundFileId_Airplane_Engine_End);
	}
}

void CAircraft::AircraftGameObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	MoveableGameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iIsFlying);
	aOutputStream.WriteInt32(iGearUp);
	aOutputStream.WriteInt32(iIsSinking);
	aOutputStream.WriteInt32(iCrashed);
	aOutputStream.WriteIntFloat(iTotalSpeed);
	aOutputStream.WriteIntFloat(iEngineSpeed);
	aOutputStream.WriteIntFloat(iDiveSpeed);
	aOutputStream.WriteIntFloat(iAngleChangeExtraSpeed);
	aOutputStream.WriteIntFloat(iCurrentEngineAcceleration);
	aOutputStream.WriteIntFloat(iCurrentDiveAcceleration);
	aOutputStream.WriteIntFloat(iExtraAngelAcceleration);
	aOutputStream.WriteIntFloat(iExtraDownSpeed);
	aOutputStream.WriteIntFloat(iMaxEngineAcceleration);
	aOutputStream.WriteIntFloat(iNormalMaxEngineAcceleration);
	aOutputStream.WriteInt32(iAngle);
	aOutputStream.WriteInt32(iIsTurning);
	aOutputStream.WriteInt32(iFrameInTurnAnimation);
	aOutputStream.WriteInt32(iFramesPerTurnImage);
	aOutputStream.WriteInt32(iFramesNeededPerAngleChange);
	aOutputStream.WriteInt32(iFramesSinceLastAngleChange);
	aOutputStream.WriteInt32(iIsUpsideDown);
	aOutputStream.WriteIntFloat(iXAxisLength);
	aOutputStream.WriteIntFloat(iYAxisLength);
	aOutputStream.WriteIntFloat(iMinimumYPosition);
	aOutputStream.WriteInt32(iAircraftType);
	if(iSmokeCreator1)
		iSmokeCreator1->SaveOnDisk(aOutputStream);
	if(iLandingGearTextureObject)
		iLandingGearTextureObject->SaveOnDisk(aOutputStream);
	if(iFrontWheel)
		iFrontWheel->SaveOnDisk(aOutputStream);
	if(iBackWheel)
		iBackWheel->SaveOnDisk(aOutputStream);
	if(iFighterAI)
	{
		aOutputStream.WriteInt32(true); // plane is controlled by AI
		iFighterAI->SaveOnDisk(aOutputStream);
	}else
	{
		aOutputStream.WriteInt32(false); // plane is NOT controlled by AI
	}
}
void CAircraft::AircraftGameObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	MoveableGameObjectLoadContentFromDisk(aReadStream);
	iIsFlying = aReadStream.ReadInt32();
	iGearUp = aReadStream.ReadInt32();
	iIsSinking = aReadStream.ReadInt32();
	iCrashed = aReadStream.ReadInt32();
	iTotalSpeed = aReadStream.ReadIntFloat();
	iEngineSpeed = aReadStream.ReadIntFloat();
	iDiveSpeed = aReadStream.ReadIntFloat();
	iAngleChangeExtraSpeed = aReadStream.ReadIntFloat();
	iCurrentEngineAcceleration = aReadStream.ReadIntFloat();
	iCurrentDiveAcceleration = aReadStream.ReadIntFloat();
	iExtraAngelAcceleration = aReadStream.ReadIntFloat();
	iExtraDownSpeed = aReadStream.ReadIntFloat();
	iMaxEngineAcceleration = aReadStream.ReadIntFloat();
	iNormalMaxEngineAcceleration = aReadStream.ReadIntFloat();
	iAngle = aReadStream.ReadInt32();
	iIsTurning = aReadStream.ReadInt32();
	iFrameInTurnAnimation = aReadStream.ReadInt32();
	iFramesPerTurnImage = aReadStream.ReadInt32();
	iFramesNeededPerAngleChange = aReadStream.ReadInt32();
	iFramesSinceLastAngleChange = aReadStream.ReadInt32();
	iIsUpsideDown = aReadStream.ReadInt32();
	iXAxisLength = aReadStream.ReadIntFloat();
	iYAxisLength = aReadStream.ReadIntFloat();
	iMinimumYPosition = aReadStream.ReadIntFloat();
	iAircraftType = static_cast<TAircraftType> (aReadStream.ReadInt32());
	if(iSmokeCreator1)
	{
		TWeaponIdentifier lSmokeCreator;
		lSmokeCreator = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
		iSmokeCreator1->LoadFromDisk(aReadStream);
	}
	if(iLandingGearTextureObject)
		iLandingGearTextureObject->LoadFromDisk(aReadStream);
	if(iFrontWheel)
		iFrontWheel->LoadFromDisk(aReadStream);
	if(iBackWheel)
		iBackWheel->LoadFromDisk(aReadStream);
	TBool lIsControlledByAI = aReadStream.ReadInt32();
	if(lIsControlledByAI)
	{
		SetAIControlled();
		iFighterAI->LoadFromDisk(aReadStream);
	}
}
