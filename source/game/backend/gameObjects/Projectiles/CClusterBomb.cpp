/*
 ============================================================================
 Name		: ClusterBomb.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CClusterBomb implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Projectiles/CClusterBomb.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInAir.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion500KgInAir.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion500KgInWater.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion500Kg.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb100kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CClusterBomb::CollisionPoints;

CClusterBomb::CClusterBomb(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel, TInt aTimerFrames) :
	CProjectileBomb(aLocation, aAttributes, aSpeed, aAngel)
{
	iCurrentFrameCounterUntilExplosion = 0;
	iFramesUntilExplodes = aTimerFrames;
}

CClusterBomb::~CClusterBomb()
{
}

CClusterBomb* CClusterBomb::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aAngel, TInt aTimerFrames, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierClusterBomb;
	CClusterBomb* self = new CClusterBomb(aLocation, lAttributes, aSpeed, aAngel, aTimerFrames);
	self->ConstructL();
	return self;
}

void CClusterBomb::ConstructL()
{
	//create Hitbox and Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBombClusterBomb), CLUSTER_BOMB_WIDTH, CLUSTER_BOMB_HEIGHT, iZCor, 0, 0.625, 0, 0.625);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CClusterBomb::CollisionPoints);
}

//------------------ functions -------------------------//

void CClusterBomb::Update()
{
	//play bomb drop sound only if the equipped bomb is moving down and it hasn't been played once
	TInt lBombYSpeed = GetPixelsPerMoveY()->GetIntInBaseInt();
	//check if the timer goes off and the bomb explodes
	iCurrentFrameCounterUntilExplosion++;
	if(iCurrentFrameCounterUntilExplosion == iFramesUntilExplodes)
	{

		//create an explosion to simulate that the bomb exploded and releases bombs
		TPointIntFloat lExplosionLocation;
		//needs to be centered
		lExplosionLocation.iX = iCoordinates.iX - TIntFloat::Convert(EXPLOSION_IN_AIR_1000KG_WIDTH) / 2;
		lExplosionLocation.iY = iCoordinates.iY - TIntFloat::Convert(EXPLOSION_IN_AIR_1000KG_HEIGHT) / 2;
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInAir::New(lExplosionLocation));

		//Bomb explodes, create a lot of small mini bombs
		for(TInt lCurrentBombIndex = 0; lCurrentBombIndex < CLUSTER_BOMB_MINI_BOMBS_PER_BOMB; lCurrentBombIndex++)
		{
			TIntFloat lRangeXExtraOffset = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(-150, 150);
			TIntFloat lRangeYExtraOffset = CGame::Game->iRandomGenerator->GetRandomNumberTIntFloat(-50, 50);
			TInt lAngleExtraOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(-45, 45);

			TPointIntFloat lBombPosition;
			lBombPosition = iCoordinates;
			lBombPosition.iX += lRangeXExtraOffset;
			lBombPosition.iY += lRangeYExtraOffset;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb100kg::New(lBombPosition, iSpeed, iAngle + lAngleExtraOffset, iObjectReflected));
		}

		//remove bomb from gameObjects, it has served its purpose :)
		Destruct();
		return;
	}

	iCurrentFrame++;
	if(iCurrentFrame == GRAVITATIONAL_FACTOR_FRAMES_UNTIL_SPEED_UPDATE)
	{
		iPixelsPerMoveY -= TIntFloat::Convert(GRAVITATIONAL_FACTOR_SPEED_INCREASE_VALUE);//increase speed over time, simulate gravitional attraction
		iCurrentFrame = 0;
	}

	if(iRange < 0)
	{
		Destruct();
	}else
	{
		iRange -= iSpeed.GetIntInBaseInt();
		iCoordinates.iX += iPixelsPerMoveX;
		iTextureObject->ChangeXCoordinate(iPixelsPerMoveX);
		iCoordinates.iY += iPixelsPerMoveY;
		iTextureObject->ChangeYCoordinate(iPixelsPerMoveY);
	}
}

void CClusterBomb::InitilizeClusterBomb()
{
	CClusterBomb::CreateClusterBombCollisionPointsL();
	CClusterBomb::LoadClusterBombTextures();
}

void CClusterBomb::RemoveClusterBomb()
{
	CClusterBomb::RemoveClusterBombCollisionPoints();
	CClusterBomb::UnloadClusterBombTextures();
}

void CClusterBomb::LoadClusterBombTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBombClusterBomb);
}

void CClusterBomb::UnloadClusterBombTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBombClusterBomb);
}

void CClusterBomb::CreateClusterBombCollisionPointsL()
{
	CClusterBomb::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CClusterBomb::CollisionPoints[0].XOffset = -2;
	CClusterBomb::CollisionPoints[0].YOffset = CLUSTER_BOMB_HEIGHT + 3;
	CClusterBomb::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CClusterBomb::CollisionPoints[1].XOffset = 8;
	CClusterBomb::CollisionPoints[1].YOffset = -3;
	CClusterBomb::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CClusterBomb::CollisionPoints[2].XOffset = CLUSTER_BOMB_WIDTH + 5;
	CClusterBomb::CollisionPoints[2].YOffset = 0;
	CClusterBomb::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CClusterBomb::CollisionPoints[3].XOffset = CLUSTER_BOMB_WIDTH - 3;
	CClusterBomb::CollisionPoints[3].YOffset = CLUSTER_BOMB_HEIGHT;
	CClusterBomb::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CClusterBomb::CollisionPoints[4].XOffset = 0;
	CClusterBomb::CollisionPoints[4].YOffset = CLUSTER_BOMB_HEIGHT + 3;
	CClusterBomb::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CClusterBomb::RemoveClusterBombCollisionPoints()
{
	if(CClusterBomb::CollisionPoints)
	{
		delete[] CClusterBomb::CollisionPoints;
		CClusterBomb::CollisionPoints = NULL;
	}
}

void CClusterBomb::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesShip | EObjectTypesFloor);

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

void CClusterBomb::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileBombObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrameCounterUntilExplosion);
	aOutputStream.WriteInt32(iFramesUntilExplodes);
}
void CClusterBomb::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileBombObjectLoadContentFromDisk(aReadStream);
	iCurrentFrameCounterUntilExplosion = aReadStream.ReadInt32();
	iFramesUntilExplodes = aReadStream.ReadInt32();
}
