/*
 ============================================================================
 Name		: BombingGuidingSystem.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBombingGuidingSystem implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CBombingGuidingSystem.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

CBombingGuidingSystem::CBombingGuidingSystem(CAircraft* aOwner,TInt aXOffSet,TInt aYOffSet)
	{
	iAircraft = aOwner;
	iXOffset = aXOffSet;
	iYOffset = aYOffSet;
	}

CBombingGuidingSystem::~CBombingGuidingSystem()
	{
	}

CBombingGuidingSystem* CBombingGuidingSystem::New(CAircraft* aOwner,TInt aXOffSet,TInt aYOffSet)
	{
	CBombingGuidingSystem* self = new CBombingGuidingSystem(aOwner,aXOffSet,aYOffSet);
	self->ConstructL();
	return self;
	}

void CBombingGuidingSystem::ConstructL()
	{

	}


//-------------- functions-------------------------//


void CBombingGuidingSystem::Draw()
{
  if(!iAircraft->IsAlive() || iAircraft->GetSecondaryWeapon()->GetAmmunitionLeft()==0 )
    return;//nothing to do

  glColor4f(1,0.1,0.1,1); //R,G,B,A , dark Red


  //need to callculate points and draw them one, by one
  //intilia values
  TIntFloat lAcceleration = -((TIntFloat::Convert(1))*(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE))/GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE;
  TIntFloat lVelocityX = *iAircraft->GetPixelsPerMoveX(); //doesn't change
  TIntFloat lVelocityY = *iAircraft->GetPixelsPerMoveY(); //changes due to gravity
  TIntFloat lDistanceMovedDownCurrentInterval;
  TPointIntFloat lFirstPoint,lSecondPoint;

  TPointIntFloat lScreenCentrePosition = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetPosition();
  TPointIntFloat lWeaponPosition = iAircraft->GetSecondaryWeapon()->GetRelativeWeaponPosition();
  TSizeIntFloat lVirtualDimensions = CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen();

  lFirstPoint = TPointIntFloat((lVirtualDimensions.iWidth) / 2, (lVirtualDimensions.iHeight) / 2); //centre of screen
  //Zoom factor needs to scale down how much we move on the screen, since the screen size increased or decreased
  lFirstPoint.iX +=  (lWeaponPosition.iX - (lScreenCentrePosition.iX)) / TIntFloat::Convert(CPlayer::ZoomFactor);
  lFirstPoint.iY +=	 (lWeaponPosition.iY - (lScreenCentrePosition.iY)) / TIntFloat::Convert(CPlayer::ZoomFactor);

  CPointerArray<TPointIntFloat>* lPoints = CPointerArray<TPointIntFloat>::New();
  lPoints->Append(new TPointIntFloat(lFirstPoint));

  //check if we are dealing with a rocket
  if(iAircraft->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponStandardRocket)
    {
  	TIntFloat lTotalRocketSpeed = iAircraft->GetCurrentAbsoluteSpeed() + TIntFloat::Convert(STANDARD_ROCKET_SPEED);
  	TInt lFramesUntilRocketExplodes = (TIntFloat::Convert(STANDARD_ROCKET_RANGE) / lTotalRocketSpeed).GetIntInBaseInt();
    lSecondPoint.iY = lFirstPoint.iY + (lTotalRocketSpeed * CMath::GraphicsSinTable(*iAircraft->GetFlyingAngel())) * lFramesUntilRocketExplodes;
    lSecondPoint.iX = lFirstPoint.iX + (lTotalRocketSpeed * CMath::GraphicsCosTable(*iAircraft->GetFlyingAngel())) * lFramesUntilRocketExplodes;
    lPoints->Append(new TPointIntFloat(lSecondPoint));
    }
  else
    {
    while(true)
     {
      //find the secondPoint
     lDistanceMovedDownCurrentInterval = (lVelocityY + lAcceleration * 0.5);
     lVelocityY = lVelocityY + lAcceleration;

     //Zoom factor needs to scale down how much we move on the screen, since the screen size increased or decreased
     lSecondPoint.iY = lFirstPoint.iY + (lDistanceMovedDownCurrentInterval / TIntFloat::Convert(CPlayer::ZoomFactor));
     lSecondPoint.iX = lFirstPoint.iX + (lVelocityX / TIntFloat::Convert(CPlayer::ZoomFactor));
     lFirstPoint = lSecondPoint;

     if(lSecondPoint.iY<(TIntFloat::Convert(0)))
       {
       //lSecondPoint.iY = (TIntFloat::Convert(0));
       lPoints->Append(new TPointIntFloat(lSecondPoint));
       break;
       }
     else
       lPoints->Append(new TPointIntFloat(lSecondPoint));
     }
    }

  //draw it
  CGame::Game->iGraphicsEngine->DrawLines(*lPoints);
  lPoints->ClearAndDestroy();
  delete lPoints;
}

void CBombingGuidingSystem::AssignAircraft(CAircraft* aOwner)
{
  iAircraft = aOwner;
}
