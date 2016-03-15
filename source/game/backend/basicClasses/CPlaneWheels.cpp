/*
 * CPlaneWheels.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CPlaneWheels.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/core/sound/CSoundObject.h"
#include "includes/core/collision/CHitBox.h"

CPlaneWheels::CPlaneWheels(CHitBox* aHitBox, CAircraft* aAircraft)
{
	iHitBox = aHitBox;
	iAircraft = aAircraft;
	iLandingSurfaceLandedOn = NULL;
}

CPlaneWheels::~CPlaneWheels()
{
	if(iHitBox != NULL)
	{
		delete iHitBox;
		iHitBox = NULL;
	}
}

CPlaneWheels* CPlaneWheels::New(CHitBox* aHitBox, CAircraft* aAircraft)
{
	CPlaneWheels* self = new CPlaneWheels(aHitBox, aAircraft);
	self->ConstructL();
	return self;
}

void CPlaneWheels::ConstructL()
{

}

//------------------ functions ---------------------------//


void CPlaneWheels::InitiateLanding(CLandingSurface* aLandingSurface)
{
	//get Flying angel
	TInt lPlaneCurrentAngel = *iAircraft->GetFlyingAngel();
	//check if the angel is properly, if the angel is too steap, it will not land
	if((((lPlaneCurrentAngel - BASIC_AIRCARFT_LANDING_SURFACE_MAXIMUM_ANGEL_FROM_SURFACE) <= 180 && (lPlaneCurrentAngel) >= 180) || (lPlaneCurrentAngel > (360 - BASIC_AIRCARFT_LANDING_SURFACE_MAXIMUM_ANGEL_FROM_SURFACE)))//must approach at a certain angel otherwise Landing will not work
	)
	{
		//rotate Plane to have proper position
		if(lPlaneCurrentAngel > 90 && lPlaneCurrentAngel <= 270)
		{
			//turn to 180 degrees since it is closer
			while(*iAircraft->GetFlyingAngel() != 180)
				iAircraft->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
		}else
		{
			//turn to 0 degrees since it is closer
			while(*iAircraft->GetFlyingAngel() != 0)
				iAircraft->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
		}
		//maybe reduce speed of plane if too fast
		if(iAircraft->GetCurrentAbsoluteSpeed() > TIntFloat::Convert(BASIC_AIRCRAFT_MAXIMUM_SPEED_ON_LANDING_SURFACE))
			iAircraft->SetCurrentAbsoluteSpeed(TIntFloat::Convert(BASIC_AIRCRAFT_MAXIMUM_SPEED_ON_LANDING_SURFACE));
		//set to proper Height, just that the airplane touches the LandingSurface
		iAircraft->SetPlaneToYPosition(aLandingSurface->GetLandingSurfaceHeightPosition());
		//set the airplane to not flying since we are in contact with a Landing surface now
		iAircraft->SetPlaneIsFlying(false);
		iLandingSurfaceLandedOn = aLandingSurface;
		iLandingSurfaceLandedOn->AddPlaneLandedOnSurface(iAircraft);

		//make sure the skid sound will not play when the plane is already on the surface
		//there is a bug when you tip off at the end of the aircraft carrier and the landing skid sound plays
		if(lPlaneCurrentAngel != 180 && lPlaneCurrentAngel != 0)
		{
			CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Plane_Skid, iAircraft->GetHitBox());
			lSoundObject->PlayHitBox();
			delete lSoundObject;
		}
	}
}

void CPlaneWheels::PlaneDied()
{
	if(iLandingSurfaceLandedOn)
	{
		iLandingSurfaceLandedOn->RemovePlaneLandedOnSurface(iAircraft);
		iLandingSurfaceLandedOn = NULL;
	}
}

void CPlaneWheels::PossibleCollision(CInterval*& aInterval)
{

	//first check if the landingSurface that we are is not alive anymore
	if(iLandingSurfaceLandedOn && !iLandingSurfaceLandedOn->IsAlive())
	{
		iAircraft->SetPlaneIsFlying(true);
		iLandingSurfaceLandedOn->RemovePlaneLandedOnSurface(iAircraft);
		iLandingSurfaceLandedOn = NULL;
	}
	//perform check if we are still in contact with the landing surface
	else if(iLandingSurfaceLandedOn)
	{
		if(!(iHitBox->IntersectionL(iLandingSurfaceLandedOn->GetHitBox()))) // NOT IN CONTACT ANYMORE
		{
			//since we are not in contact anymore with landing surface that we previously intersected, it must be true that we left that surface and are flying
			iAircraft->SetPlaneIsFlying(true);
			iLandingSurfaceLandedOn->RemovePlaneLandedOnSurface(iAircraft);
			iLandingSurfaceLandedOn = NULL;
		}
	}
	//check if we are turning the plane, can't land while turning
	if(iAircraft->IsTurning())
		return;

	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesLandingSurface);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

		//only collide with objects of type LandingSurface, if they are still alive and are of your or neutral ConflictSide
		if(lCurrentGameObject->IsAlive() && (lCurrentGameObject->GetConflictSide() == EConflictSideNeutal || lCurrentGameObject->GetConflictSide() == iAircraft->GetConflictSide()))
		{
			CLandingSurface * lCurrentLandingSurface = static_cast<CLandingSurface*> (lCurrentGameObject);
			//rotate plane to vertical position
			if(!iLandingSurfaceLandedOn && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
			{
				InitiateLanding(lCurrentLandingSurface);
			}
			else if(iLandingSurfaceLandedOn && iAircraft->GetCurrentEngineAcceleration() == 0)
			{
				iAircraft->RearmAircraft();
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CPlaneWheels::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	iHitBox->SaveOnDisk(aOutputStream);
	if(iLandingSurfaceLandedOn)
	{
		aOutputStream.WriteInt32(1);
		aOutputStream.WriteUint32((TUint32)iLandingSurfaceLandedOn);
	}else
		aOutputStream.WriteInt32(0);
}

void CPlaneWheels::LoadFromDisk(CFileReadStream &aReadStream)
{
	iHitBox->LoadFromDisk(aReadStream);
	TInt32 lHasLanded = aReadStream.ReadInt32();
	if(lHasLanded == 1)
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iLandingSurfaceLandedOn));
}
