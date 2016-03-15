/*
 ============================================================================
 Name		: FloorLand.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFloorLand implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Floor/CFloorLand.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"

CFloorLand::CFloorLand(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(FLOOR_DEFAULT_Z_COORDINATE, NULL, NULL, aLocation, aAttributes)
{
	// No implementation required
}

CFloorLand::~CFloorLand()
{
}

CFloorLand* CFloorLand::New(TPoint aLocation, TInt aWidth)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesFloor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierLand;

	CFloorLand* self = new CFloorLand(aLocation, lAttributes);
	self->ConstructL(aWidth);
	return self;
}

CFloorLand* CFloorLand::New(TInt aXLocation, TInt aWidth)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesFloor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierLand;

	CFloorLand* self = new CFloorLand(TPoint(aXLocation, 0), lAttributes);
	self->ConstructL(aWidth);
	return self;
}
void CFloorLand::ConstructL(TInt aWidth)
{
	//since the texture will be repeated over and over again until the entire distance has been covered given by width
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundSandDirtFloor), aWidth, FLOOR_TEXTURE_HEIGHT, iZCor, 0, aWidth / FLOOR_TEXTURE_WIDTH, 0, 1);
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(aWidth, FLOOR_TEXTURE_HEIGHT));
}

//----------------- functions ----------------


void CFloorLand::Die(TDamageType aType) // sets the status of the Object to dead but also starts the dead animation loop
{
	//do nothing since land can't die
}

void CFloorLand::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);
		CAircraft* lPlane = (CAircraft*) lCurrentGameObject;

		if(!lPlane->AirplaneCrashed() && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
		{
			lPlane->AngleCorrectionIfCrashedDuringTurn();
			//airplane has crashed rotate to 180 or 0 degrees
			TInt lPlaneCurrentAngel = *lPlane->GetFlyingAngel();
			if(lPlaneCurrentAngel > 90 && lPlaneCurrentAngel <= 270)
			{
				//turn to 180 degrees since it is closer
				while(*lPlane->GetFlyingAngel() != 180)
					lPlane->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
			}else
			{
				//turn to 0 degrees since it is closer
				while(*lPlane->GetFlyingAngel() != 0)
					lPlane->IncreaseFlyingAngleWithoutSpeedCheckSmallAngle();
			}
			//plane how now proper rotation
			//need to apply damage here, to get the right crash image
			//resets the iCoordinates to the HitBox Layed out probably
			SDamage lDamage;
			lDamage.Damage = KILL_ANY_AIRPLANE_DAMAGE;
			lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;//should kill any object
			lPlane->TakeDamage(lDamage, EDamageTypePlaneCrash);

			//need to extra stuff if plane is upside down
			if(lPlane->IsUpSideDown())
			{
				if(*lPlane->GetFlyingAngel() == 180) //faces left
				{
					lPlane->TurnLeftWithoutAnimationAndAngleChange();//since we are upside down the controls are also the other way around
				}else
				{ //angle has to be 0, those faces right
					lPlane->TurnRightWithoutAnimationAndAngleChange();
				}
			}

			//set proper YPosition, readjusts the Y location to be just above the surface area
			lPlane->SetPlaneToYPosition(TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT));
		}
	}

	//clean up
	delete lGameObjects;
}

void CFloorLand::LoadFloorLandTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundSandDirtFloor);
}

void CFloorLand::UnloadFloorLandTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundSandDirtFloor);
}

void CFloorLand::InitilizeFloorLand()
{
	CFloorLand::LoadFloorLandTextures();
}

void CFloorLand::RemoveFloorLand()
{
	CFloorLand::UnloadFloorLandTextures();
}
