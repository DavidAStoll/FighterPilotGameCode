/*
 ============================================================================
 Name		: GeneralSmokeCreator.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGeneralSmokeCreator implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/Decor/CAircraftSmokeCreator.h"
#include "includes/game/backend/gameObjects/Decor/CSmoke.h"


CAircraftSmokeCreator::CAircraftSmokeCreator(TBool aBehindObject, CAircraft* aOwner, TInt aXOffset, TInt aYOffset, TInt aFramesUntilSmokeDisappears)
:CWeapon(1,WeaponIdentifierAirplaneSmokeCreator,false,false, &aOwner->GetCurrentPosition(), &aOwner->GetAngle(),aXOffset, aYOffset, &aOwner->IsReflected(), NULL, 0)
	{
		iBehindObject = aBehindObject;
		iFramesUntilSmokeDisappears = aFramesUntilSmokeDisappears;
		iOwner = aOwner;

		//is always the same colour
		iCrashedColour = 0;
		iCrashedColour += 10 << 24; //Red
		iCrashedColour += 10 << 16; //Green
		iCrashedColour += 10 << 8; //Blue

		iHealthOfPlaneLeft = -1; //health is never negative, thus this will be updated during the first loop
	}

CAircraftSmokeCreator::~CAircraftSmokeCreator()
	{
	}


CAircraftSmokeCreator* CAircraftSmokeCreator::New(TBool aBehindObject, CAircraft* aOwner, TInt aXOffset, TInt aYOffset, TInt aFramesUntilSmokeDisappears)
	{
		CAircraftSmokeCreator* self = new CAircraftSmokeCreator(aBehindObject, aOwner, aXOffset, aYOffset, aFramesUntilSmokeDisappears);
		return self;
	}

//-------------- functions ----------------------//
void CAircraftSmokeCreator::Update()
{
  //if object health is below 90% start creating smoke
  TInt lPercentageOfHealthLeft = ((iOwner->GetHealth()) * 100) / iOwner->GetMaxHealth();

  if(lPercentageOfHealthLeft < 90)
    {
			TIntFloat lRandomInt  = CFighterPilotThePacificWar::FighterGame->iRandomGenerator->GetRandomNumberTIntFloat(-5, 6);

			if(iOwner->AirplaneCrashed())
  		{
				TIntFloat lSmokeScaleFactor = TIntFloat::Convert(1) + lRandomInt / 10;//scale the size of the cloud
				TIntFloat lWidthOfSmoke = TIntFloat::Convert(AIRCRAFT_SMOKE_CREATOR_WHEN_CRASHED_WIDTH);

				//need to update since if crashed Z value changes as well
				TInt lSmokeZValue;
				if(iBehindObject)
					lSmokeZValue = iOwner->iZCor - 1;
				else
					lSmokeZValue = iOwner->iZCor + 1;

				//adjust since object might be slow recycled
				TColour lColour = iCrashedColour;
				lColour += iOwner->GetAlpha();

				TIntFloat lPositionChange = ((lWidthOfSmoke * lSmokeScaleFactor) - lWidthOfSmoke) / 2;
				lWidthOfSmoke *= lSmokeScaleFactor;

				CSmoke* lSmoke = CSmoke::New(GetRelativeWeaponPosition(lPositionChange.GetIntInBaseInt(), -lPositionChange.GetIntInBaseInt()), TIntFloat::Convert(AIRCRAFT_SMOKE_CREATOR_WHEN_CRASHED_SMOKE_SPEED), 90,
																		*iWeaponReflectedOverYAxis, lColour, TSizeIntFloat(lWidthOfSmoke, lWidthOfSmoke), AIRCRAFT_SOMKE_CREATOR_WHEN_CRASHED_FRAMES_FOR_SMOKE, lSmokeZValue,
																		TIntFloat::Convert(AIRCRAFT_SOMKE_CREATOR_WHEN_CRASHED_SMOKE_DISPLACEMENT_X), TIntFloat::Convert(AIRCRAFT_SOMKE_CREATOR_WHEN_CRASHED_SMOKE_DISPLACEMENT_Y));
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lSmoke);
  		}
  		else
  		{
  			TIntFloat lSmokeScaleFactor = TIntFloat::Convert(1) + lRandomInt / 20;//scale the size of the cloud
				TIntFloat lWidthOfSmoke = iOwner->GetCurrentAbsoluteSpeed() * TIntFloat::Convert(2); //100% percent overlap
				TIntFloat lHeightOfSmoke = TIntFloat::Convert(7) * lSmokeScaleFactor;

				if(iHealthOfPlaneLeft != lPercentageOfHealthLeft)
				{
					iHealthOfPlaneLeft = lPercentageOfHealthLeft;

					//need to update the colour of the smoke, since health changed
					TUint lColourComponent = ((lPercentageOfHealthLeft) * 255) / 100; //don't want to go completly black
					TUint lColourComponentAdjusted = lColourComponent + 20; //don't wamt to go completly black
					iSmokeColour = 0;
					iSmokeColour += lColourComponentAdjusted << 24; //Red
					iSmokeColour += lColourComponentAdjusted << 16; //Green
					iSmokeColour += lColourComponentAdjusted << 8; //Blue
					iSmokeColour += 255 - lColourComponent; //Alpha, inverse of health
				}

				//find out how start location changes due to size changes
				TIntFloat lPositionChangeX = ((lWidthOfSmoke * lSmokeScaleFactor) - lWidthOfSmoke) / 2;
				TIntFloat lPositionChangeY = ((lHeightOfSmoke * lSmokeScaleFactor) - lHeightOfSmoke) / 2;

				//scale to proper size
				lWidthOfSmoke *= lSmokeScaleFactor;
				lHeightOfSmoke *= lSmokeScaleFactor;

				TInt lSmokeZValue;
				if(iBehindObject)
					lSmokeZValue = iOwner->iZCor - 1;
				else
					lSmokeZValue = SMOKE_DEFAULT_Z_VALUE; //should be infront of everything

				CSmoke* lSmoke = CSmoke::New(GetRelativeWeaponPosition(lPositionChangeX.GetIntInBaseInt(), -lPositionChangeY.GetIntInBaseInt()), TIntFloat::Convert(0), *iAngel, *iWeaponReflectedOverYAxis, iSmokeColour, TSizeIntFloat(lWidthOfSmoke, lHeightOfSmoke), iFramesUntilSmokeDisappears, lSmokeZValue);
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lSmoke);
  		}
    }
}

void CAircraftSmokeCreator::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  WeaponObjectSaveContentToDisk(aOutputStream);
  //nothing else needs to be saved since it will be reset when aircraft is loaded
}
void CAircraftSmokeCreator::LoadFromDisk(CFileReadStream &aReadStream)
{
  WeaponObjectLoadContentFromDisk(aReadStream);
}
