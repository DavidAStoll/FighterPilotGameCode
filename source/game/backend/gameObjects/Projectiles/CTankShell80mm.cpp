/*
 ============================================================================
 Name		: TankShell80mm.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTankShell80mm implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CTankShell80mm::CollisionPoints;

CTankShell80mm::CTankShell80mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aExtraSpeed) :
CGravitationalProjectile(aLocation, aAttributes, TIntFloat::Convert(TANKSHELL80MM_SPEED) + aExtraSpeed, aAngel)
{
	// No implementation required
}

CTankShell80mm::~CTankShell80mm()
{
}

CTankShell80mm* CTankShell80mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aExtraSpeed)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierTankShell80mm;
	CTankShell80mm* self = new CTankShell80mm(aLocation, lAttributes, aAngel, aExtraSpeed);
	self->ConstructL();
	return self;
}

void CTankShell80mm::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet80mmTankShell), TANKSHELL80MM_WIDTH, TANKSHELL80MM_HEIGHT, iZCor, 0, 1, 0, 1);

	AdjustCoordinatesAndTexture();
  UpdateGravitionalStatus();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CTankShell80mm::CollisionPoints);
}


//---------------- functions -------------------------

void CTankShell80mm::DestructAtCollisionPoint(TPointIntFloat* lCollisionPoint)//used if we have a location where it should explode
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game

	//create explosion at current Location
	TPointIntFloat lExplosionLocation;
	//needs to be centered
	lExplosionLocation.iX = lCollisionPoint->iX - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_WIDTH) / 2;
	lExplosionLocation.iY = lCollisionPoint->iY - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_HEIGHT) / 2;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500KgInAir::New(lExplosionLocation,TANKSHELL80MM_DAMAGE,TANKSHELL80MM_AMORPENETRATION,true));
}

void CTankShell80mm::LoadTankShell80mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet80mmTankShell);
}


void CTankShell80mm::UnloadTankShell80mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet80mmTankShell);
}

void CTankShell80mm::CreateTankShell80mmCollisionPointsL()
{
	CTankShell80mm::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CTankShell80mm::CollisionPoints[0].XOffset = -2;
	CTankShell80mm::CollisionPoints[0].YOffset = TANKSHELL80MM_HEIGHT + 1;
	CTankShell80mm::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CTankShell80mm::CollisionPoints[1].XOffset = 5;
	CTankShell80mm::CollisionPoints[1].YOffset = -1;
	CTankShell80mm::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CTankShell80mm::CollisionPoints[2].XOffset = TANKSHELL80MM_WIDTH + 4;
	CTankShell80mm::CollisionPoints[2].YOffset = 0;
	CTankShell80mm::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CTankShell80mm::CollisionPoints[3].XOffset = TANKSHELL80MM_WIDTH - 3;
	CTankShell80mm::CollisionPoints[3].YOffset = TANKSHELL80MM_HEIGHT;
	CTankShell80mm::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CTankShell80mm::CollisionPoints[4].XOffset = 0;
	CTankShell80mm::CollisionPoints[4].YOffset = TANKSHELL80MM_HEIGHT;
	CTankShell80mm::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CTankShell80mm::RemoveTankShell80mmCollisionPoints()
{
	if(CTankShell80mm::CollisionPoints)
	{
		delete[] CTankShell80mm::CollisionPoints;
		CTankShell80mm::CollisionPoints = NULL;
	}
}

void CTankShell80mm::InitilizeTankShell80mm()
{
	CTankShell80mm::LoadTankShell80mmTextures();
	CTankShell80mm::CreateTankShell80mmCollisionPointsL();
}

void CTankShell80mm::RemoveTankShell80mm()
{
	CTankShell80mm::UnloadTankShell80mmTextures();
	CTankShell80mm::RemoveTankShell80mmCollisionPoints();
}

void CTankShell80mm::PossibleCollision(CInterval*& aInterval)
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
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_500KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500KgInWater::New(*lCollisionPoint));
							}else
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_500KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								//create explosion on Ground
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500Kg::New(*lCollisionPoint,TANKSHELL80MM_DAMAGE,TANKSHELL80MM_AMORPENETRATION));
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
