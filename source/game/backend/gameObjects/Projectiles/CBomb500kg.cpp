/*
 ============================================================================
 Name		: Bomb500kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBomb500kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion500Kg.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion500KgInWater.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb500kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CBomb500kg::CollisionPoints;

CBomb500kg::CBomb500kg(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel) :
	CProjectileBomb(aLocation, aAttributes, aSpeed, aAngel)
{
}

CBomb500kg::~CBomb500kg()
{
}

CBomb500kg* CBomb500kg::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier500kgBomb;
	CBomb500kg* self = new CBomb500kg(aLocation, lAttributes, aSpeed, aAngel);
	self->ConstructL();
	return self;
}

void CBomb500kg::ConstructL()
{
	//create Hitbox and Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBomb500KgBomb), BOMB_500KG_WIDTH, BOMB_500KG_HEIGHT, iZCor, 0, 0.9375, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBomb500kg::CollisionPoints);
}

//------------------ functions -------------------------//

void CBomb500kg::InitilizeBomb500kg()
{
	CBomb500kg::CreateBomb500kgCollisionPointsL();
	CBomb500kg::LoadBomb500kgTextures();
}

void CBomb500kg::RemoveBomb500kg()
{
	CBomb500kg::RemoveBomb500kgCollisionPoints();
	CBomb500kg::UnloadBomb500kgTextures();
}

void CBomb500kg::LoadBomb500kgTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBomb500KgBomb);
}

void CBomb500kg::UnloadBomb500kgTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBomb500KgBomb);
}

void CBomb500kg::CreateBomb500kgCollisionPointsL()
{
	CBomb500kg::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBomb500kg::CollisionPoints[0].XOffset = -2;
	CBomb500kg::CollisionPoints[0].YOffset = BOMB_500KG_HEIGHT + 3;
	CBomb500kg::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBomb500kg::CollisionPoints[1].XOffset = 8;
	CBomb500kg::CollisionPoints[1].YOffset = -3;
	CBomb500kg::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBomb500kg::CollisionPoints[2].XOffset = BOMB_500KG_WIDTH + 5;
	CBomb500kg::CollisionPoints[2].YOffset = 0;
	CBomb500kg::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBomb500kg::CollisionPoints[3].XOffset = BOMB_500KG_WIDTH - 3;
	CBomb500kg::CollisionPoints[3].YOffset = BOMB_500KG_HEIGHT;
	CBomb500kg::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBomb500kg::CollisionPoints[4].XOffset = 0;
	CBomb500kg::CollisionPoints[4].YOffset = BOMB_500KG_HEIGHT + 3;
	CBomb500kg::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBomb500kg::RemoveBomb500kgCollisionPoints()
{
	if(CBomb500kg::CollisionPoints)
	{
		delete[] CBomb500kg::CollisionPoints;
		CBomb500kg::CollisionPoints = NULL;
	}
}

void CBomb500kg::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesFloor | EObjectTypesShip);

	if(iAlive)
	{
		for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

			//only collide with alive objects
			if(lCurrentGameObject->IsAlive() && lCurrentGameObject != this)
			{
				//collide, need special intersection method that returns the collision point to create the explosion
				TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
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
						CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500Kg::New(*lCollisionPoint));
					}

					Destruct();//bullet is used up
					delete lCollisionPoint;
					lCollisionPoint = NULL;
					break;//only one explosion possible per bomb :)
				}
			}
		}
	}

	//clean up
	delete lGameObjects;
}
