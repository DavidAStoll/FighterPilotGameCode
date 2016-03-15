/*
 ============================================================================
 Name		: Bomb1000kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBomb1000kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Projectiles/CBomb1000kg.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInWater.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion1000Kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CBomb1000kg::CollisionPoints;

CBomb1000kg::CBomb1000kg(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel) :
	CProjectileBomb(aLocation, aAttributes, aSpeed, aAngel)
{
	// No implementation required
}

CBomb1000kg::~CBomb1000kg()
{
}

CBomb1000kg* CBomb1000kg::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier1000kgBomb;
	CBomb1000kg* self = new CBomb1000kg(aLocation, lAttributes, aSpeed, aAngel);
	self->ConstructL();
	return self;
}

void CBomb1000kg::ConstructL()
{
	//create Hitbox and Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBomb1000KgBomb), BOMB_1000KG_WIDTH, BOMB_1000KG_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBomb1000kg::CollisionPoints);
}

//------------------ functions -------------------------//

void CBomb1000kg::InitilizeBomb1000kg()
{
	CBomb1000kg::CreateBomb1000kgCollisionPointsL();
	CBomb1000kg::LoadBomb1000kgTextures();
}

void CBomb1000kg::RemoveBomb1000kg()
{
	CBomb1000kg::RemoveBomb1000kgCollisionPoints();
	CBomb1000kg::UnloadBomb1000kgTextures();
}

void CBomb1000kg::LoadBomb1000kgTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBomb1000KgBomb);
}

void CBomb1000kg::UnloadBomb1000kgTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBomb1000KgBomb);
}

void CBomb1000kg::CreateBomb1000kgCollisionPointsL()
{
	CBomb1000kg::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBomb1000kg::CollisionPoints[0].XOffset = -2;
	CBomb1000kg::CollisionPoints[0].YOffset = BOMB_1000KG_HEIGHT + 3;
	CBomb1000kg::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBomb1000kg::CollisionPoints[1].XOffset = 8;
	CBomb1000kg::CollisionPoints[1].YOffset = -3;
	CBomb1000kg::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBomb1000kg::CollisionPoints[2].XOffset = BOMB_1000KG_WIDTH + 5;
	CBomb1000kg::CollisionPoints[2].YOffset = 0;
	CBomb1000kg::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBomb1000kg::CollisionPoints[3].XOffset = BOMB_1000KG_WIDTH - 3;
	CBomb1000kg::CollisionPoints[3].YOffset = BOMB_1000KG_HEIGHT;
	CBomb1000kg::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBomb1000kg::CollisionPoints[4].XOffset = 0;
	CBomb1000kg::CollisionPoints[4].YOffset = BOMB_1000KG_HEIGHT + 3;
	CBomb1000kg::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBomb1000kg::RemoveBomb1000kgCollisionPoints()
{
	if(CBomb1000kg::CollisionPoints)
	{
		delete[] CBomb1000kg::CollisionPoints;
		CBomb1000kg::CollisionPoints = NULL;
	}
}

void CBomb1000kg::PossibleCollision(CInterval*& aInterval)
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
						lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_1000KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
						CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInWater::New(*lCollisionPoint));
					}else
					{
						//need to adjust explosion since the explosion should be centered relative to the collision point
						lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_1000KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
						//create explosion on Ground
						CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000Kg::New(*lCollisionPoint));
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
