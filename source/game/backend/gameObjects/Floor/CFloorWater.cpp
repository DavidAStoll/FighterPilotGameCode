/*
 ============================================================================
 Name		: FloorWater.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFloorWater implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Floor/CFloorWater.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CFloorWater::CFloorWater(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(FLOOR_DEFAULT_Z_COORDINATE, NULL, NULL, aLocation, aAttributes)
{
	iCurrentTextureBeingDisplayed = EWaterTexture1;
	iCurrentFrame = 0;
}

CFloorWater::~CFloorWater()
{
}

CFloorWater* CFloorWater::New(TPoint aLocation, TInt aWidth)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesFloor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierWater;

	CFloorWater* self = new  CFloorWater(aLocation, lAttributes);
	self->ConstructL(aWidth);
	return self;
}

CFloorWater* CFloorWater::New(TInt aXLocation, TInt aWidth)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesFloor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierWater;

	CFloorWater* self = new  CFloorWater(TPoint(aXLocation, 0), lAttributes);
	self->ConstructL(aWidth);
	return self;
}

void CFloorWater::ConstructL(TInt aWidth)
{
	//since the texture will be repeated over and over again until the entire distance has been covered given by width
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundWater1), aWidth, FLOOR_TEXTURE_HEIGHT, iZCor, 0, aWidth / WATER_TEXTURE_WIDTH, 0, 1);
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(aWidth, FLOOR_TEXTURE_HEIGHT));
}

//----------------- functions ----------------


void CFloorWater::Die(TDamageType aType) // sets the status of the Object to dead but also starts the dead animation loop
{
	//do nothing since land can't die
}

void CFloorWater::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);
		CAircraft* lPlane = (CAircraft*)lCurrentGameObject;

		if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
		{
			lPlane->AngleCorrectionIfCrashedDuringTurn();
			if(lPlane->AirplaneCrashed())
			{
				if(lPlane->GetCurrentAbsoluteSpeed() < TIntFloat::Convert(1))
				{
					lPlane->SetIsSinking(true);
				}
			}else
			{
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
	}

	//clean up
	delete lGameObjects;
}

void CFloorWater::LoadFloorWaterTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundWater1);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundWater2);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundWater3);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundWater4);
}

void CFloorWater::UnloadFloorWaterTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundWater1);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundWater2);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundWater3);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundWater4);
}

void CFloorWater::InitilizeFloorWater()
{
	CFloorWater::LoadFloorWaterTextures();
}

void CFloorWater::RemoveFloorWater()
{
	CFloorWater::UnloadFloorWaterTextures();
}

void CFloorWater::Update()
{
	iTextureObject->ChangeSCoordinate(WATER_S_TEXTURE_MOVEMENT_PER_FRAME); //moves the water to the right, shouldn't cause an overflow since it can run several days before it runs out of space
	iCurrentFrame++;

	if(iCurrentFrame == WATER_FRAMES_UNTIL_IMAGE_SWITCH)
	{
		iCurrentFrame = 0;

		if(iCurrentTextureBeingDisplayed == EWaterTexture1)
		{
			iCurrentTextureBeingDisplayed = EWaterTexture2;
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdGroundWater2));
		}
		else if(iCurrentTextureBeingDisplayed == EWaterTexture2)
		{
			iCurrentTextureBeingDisplayed = EWaterTexture3;
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdGroundWater3));
		}
		else if(iCurrentTextureBeingDisplayed == EWaterTexture3)
		{
			iCurrentTextureBeingDisplayed = EWaterTexture4;
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdGroundWater4));
		}
		else if(iCurrentTextureBeingDisplayed == EWaterTexture4)
		{
			iCurrentTextureBeingDisplayed = EWaterTexture1;
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdGroundWater1));
		}

	}
}

void CFloorWater::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iCurrentTextureBeingDisplayed);
}

void CFloorWater::LoadFromDisk(CFileReadStream &aReadStream)
{
	GameObjectLoadContentFromDisk(aReadStream);
	iCurrentFrame = aReadStream.ReadInt32();
	iCurrentTextureBeingDisplayed = static_cast<TWaterTextures> (aReadStream.ReadInt32());
}

