/*
 ============================================================================
 Name		: Bomb100kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBomb100kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion100Kg.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion100KgInWater.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb100kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CBomb100kg::CollisionPoints;

CBomb100kg::CBomb100kg(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel) :
	CProjectileBomb(aLocation, aAttributes, aSpeed, aAngel)
{
}

CBomb100kg::~CBomb100kg()
{
}

CBomb100kg* CBomb100kg::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier100kgBomb;
	CBomb100kg* self = new CBomb100kg(aLocation, lAttributes, aSpeed, aAngel);
	self->ConstructL();
	return self;
}

void CBomb100kg::ConstructL()
{
	//create Hitbox and Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBomb100KgBomb), BOMB_100KG_WIDTH, BOMB_100KG_HEIGHT, iZCor, 0, 0.59375, 0, 0.75);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBomb100kg::CollisionPoints);
}

//------------------ functions -------------------------//

void CBomb100kg::InitilizeBomb100kg()
{
	CBomb100kg::CreateBomb100kgCollisionPointsL();
	CBomb100kg::LoadBomb100kgTextures();
}

void CBomb100kg::RemoveBomb100kg()
{
	CBomb100kg::RemoveBomb100kgCollisionPoints();
	CBomb100kg::UnloadBomb100kgTextures();
}

void CBomb100kg::LoadBomb100kgTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBomb100KgBomb);
}

void CBomb100kg::UnloadBomb100kgTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBomb100KgBomb);
}

void CBomb100kg::CreateBomb100kgCollisionPointsL()
{
	CBomb100kg::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBomb100kg::CollisionPoints[0].XOffset = -2;
	CBomb100kg::CollisionPoints[0].YOffset = BOMB_100KG_HEIGHT + 3;
	CBomb100kg::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBomb100kg::CollisionPoints[1].XOffset = 8;
	CBomb100kg::CollisionPoints[1].YOffset = -3;
	CBomb100kg::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBomb100kg::CollisionPoints[2].XOffset = BOMB_100KG_WIDTH + 5;
	CBomb100kg::CollisionPoints[2].YOffset = 0;
	CBomb100kg::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBomb100kg::CollisionPoints[3].XOffset = BOMB_100KG_WIDTH - 3;
	CBomb100kg::CollisionPoints[3].YOffset = BOMB_100KG_HEIGHT;
	CBomb100kg::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBomb100kg::CollisionPoints[4].XOffset = 0;
	CBomb100kg::CollisionPoints[4].YOffset = BOMB_100KG_HEIGHT + 3;
	CBomb100kg::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBomb100kg::RemoveBomb100kgCollisionPoints()
{
	if(CBomb100kg::CollisionPoints)
	{
		delete[] CBomb100kg::CollisionPoints;
		CBomb100kg::CollisionPoints = NULL;
	}
}

void CBomb100kg::PossibleCollision(CInterval*& aInterval)
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
						lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
						CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100KgInWater::New(*lCollisionPoint));
					}else
					{
						//need to adjust explosion since the explosion should be centered relative to the collision point
						lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
						//create explosion on Ground
						CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100Kg::New(*lCollisionPoint));
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
