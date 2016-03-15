/*
 ============================================================================
 Name		: PropelledGrenade.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CPropelledGrenade implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CPropelledGrenade::CollisionPoints;

CPropelledGrenade::CPropelledGrenade(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aExtraSpeed) :
CGravitationalProjectile(aLocation, aAttributes, TIntFloat::Convert(PROPELLEDGRENADE_SPEED) + aExtraSpeed, aAngel)
{
}

CPropelledGrenade::~CPropelledGrenade()
{
}

CPropelledGrenade* CPropelledGrenade::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aExtraSpeed)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierPropelledGrenade;
	CPropelledGrenade* self = new CPropelledGrenade(aLocation, lAttributes, aAngel, aExtraSpeed);
	self->ConstructL();
	return self;
}

void CPropelledGrenade::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBulletPropelledGrenade), PROPELLEDGRENADE_WIDTH, PROPELLEDGRENADE_HEIGHT, iZCor, 0, 1, 0, 1);

	AdjustCoordinatesAndTexture();
  UpdateGravitionalStatus();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CPropelledGrenade::CollisionPoints);
}

//---------------- functions -------------------------

void CPropelledGrenade::DestructAtCollisionPoint(TPointIntFloat* lCollisionPoint)//used if we have a location where it should explode
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game

	//create explosion at current Location
	TPointIntFloat lExplosionLocation;
	//needs to be centered
	lExplosionLocation.iX = lCollisionPoint->iX - TIntFloat::Convert(EXPLOSION_IN_AIR_100KG_WIDTH) / 2;
	lExplosionLocation.iY = lCollisionPoint->iY - TIntFloat::Convert(EXPLOSION_IN_AIR_100KG_HEIGHT) / 2;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100KgInAir::New(lExplosionLocation));
}

void CPropelledGrenade::LoadPropelledGrenadeTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBulletPropelledGrenade);
}

void CPropelledGrenade::UnloadPropelledGrenadeTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBulletPropelledGrenade);
}

void CPropelledGrenade::CreatePropelledGrenadeCollisionPointsL()
{
	CPropelledGrenade::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CPropelledGrenade::CollisionPoints[0].XOffset = -2;
	CPropelledGrenade::CollisionPoints[0].YOffset = PROPELLEDGRENADE_HEIGHT + 1;
	CPropelledGrenade::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CPropelledGrenade::CollisionPoints[1].XOffset = 5;
	CPropelledGrenade::CollisionPoints[1].YOffset = -1;
	CPropelledGrenade::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CPropelledGrenade::CollisionPoints[2].XOffset = PROPELLEDGRENADE_WIDTH + 4;
	CPropelledGrenade::CollisionPoints[2].YOffset = 0;
	CPropelledGrenade::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CPropelledGrenade::CollisionPoints[3].XOffset = PROPELLEDGRENADE_WIDTH - 3;
	CPropelledGrenade::CollisionPoints[3].YOffset = PROPELLEDGRENADE_HEIGHT;
	CPropelledGrenade::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CPropelledGrenade::CollisionPoints[4].XOffset = 0;
	CPropelledGrenade::CollisionPoints[4].YOffset = PROPELLEDGRENADE_HEIGHT;
	CPropelledGrenade::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CPropelledGrenade::RemovePropelledGrenadeCollisionPoints()
{
	if(CPropelledGrenade::CollisionPoints)
	{
		delete[] CPropelledGrenade::CollisionPoints;
		CPropelledGrenade::CollisionPoints = NULL;
	}
}

void CPropelledGrenade::InitilizePropelledGrenade()
{
	CPropelledGrenade::LoadPropelledGrenadeTextures();
	CPropelledGrenade::CreatePropelledGrenadeCollisionPointsL();
}

void CPropelledGrenade::RemovePropelledGrenade()
{
	CPropelledGrenade::UnloadPropelledGrenadeTextures();
	CPropelledGrenade::RemovePropelledGrenadeCollisionPoints();
}

void CPropelledGrenade::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesPlane | EObjectTypesShip | EObjectTypesFloor);

	if(iAlive)
	{
		for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

			//only collide with alive objects
			if(lCurrentGameObject->IsAlive() && lCurrentGameObject != this)
			{
				//now check if it is not member of the same team and not decor or another projectile
				if((lCurrentGameObject->GetConflictSide() != iConflictSide))
				{
					if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesPlane | EObjectTypesShip))
					{
						TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
						//collide
						if(lCollisionPoint)
						{
							//need to create an explosion here
							DestructAtCollisionPoint(lCollisionPoint);//alread centers the explosion relative to the collision point
							delete lCollisionPoint;
							break;
						}
					}else if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesFloor))
					{

						TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
						//collide
						if(lCollisionPoint)
						{

							//check if explosion happens on water
							if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierWater)
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100KgInWater::New(*lCollisionPoint));
							}else
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100Kg::New(*lCollisionPoint));
							}

							//rocket is used Up
							Destruct();
							delete lCollisionPoint;
							lCollisionPoint = NULL;
							break;//only one explosion possible per bomb :)
						}
					}
				}
			}
		}
	}

	//clean up
	delete lGameObjects;
}
