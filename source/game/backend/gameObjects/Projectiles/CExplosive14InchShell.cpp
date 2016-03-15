/*
 ============================================================================
 Name		: Explosive14InchShell.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive14InchShell implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CExplosive14InchShell::CollisionPoints;

CExplosive14InchShell::CExplosive14InchShell(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed) :
CGravitationalProjectile(aLocation, aAttributes, aSpeed, aAngel)
{
	// No implementation required
}

CExplosive14InchShell::~CExplosive14InchShell()
{
}

CExplosive14InchShell* CExplosive14InchShell::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosive14InchShell;
	CExplosive14InchShell* self = new CExplosive14InchShell(aLocation, lAttributes, aAngel, aSpeed);
	self->ConstructL();
	return self;
}

void CExplosive14InchShell::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet14InchExplosiveShell), EXPLOSIVE_14_INCH_SHELL_WIDTH, EXPLOSIVE_14_INCH_SHELL_HEIGHT, iZCor, 0, 1, 0, 1);

	AdjustCoordinatesAndTexture();
  UpdateGravitionalStatus();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CExplosive14InchShell::CollisionPoints);
}


//---------------- functions -------------------------

void CExplosive14InchShell::LoadExplosive14InchShellTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet14InchExplosiveShell);
}


void CExplosive14InchShell::UnloadExplosive14InchShellTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet14InchExplosiveShell);
}

void CExplosive14InchShell::CreateExplosive14InchShellCollisionPointsL()
{
	CExplosive14InchShell::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CExplosive14InchShell::CollisionPoints[0].XOffset = -2;
	CExplosive14InchShell::CollisionPoints[0].YOffset = EXPLOSIVE_14_INCH_SHELL_HEIGHT + 1;
	CExplosive14InchShell::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CExplosive14InchShell::CollisionPoints[1].XOffset = 5;
	CExplosive14InchShell::CollisionPoints[1].YOffset = -1;
	CExplosive14InchShell::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CExplosive14InchShell::CollisionPoints[2].XOffset = EXPLOSIVE_14_INCH_SHELL_WIDTH + 4;
	CExplosive14InchShell::CollisionPoints[2].YOffset = 0;
	CExplosive14InchShell::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CExplosive14InchShell::CollisionPoints[3].XOffset = EXPLOSIVE_14_INCH_SHELL_WIDTH - 3;
	CExplosive14InchShell::CollisionPoints[3].YOffset = EXPLOSIVE_14_INCH_SHELL_HEIGHT;
	CExplosive14InchShell::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CExplosive14InchShell::CollisionPoints[4].XOffset = 0;
	CExplosive14InchShell::CollisionPoints[4].YOffset = EXPLOSIVE_14_INCH_SHELL_HEIGHT;
	CExplosive14InchShell::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CExplosive14InchShell::RemoveExplosive14InchShellCollisionPoints()
{
	if(CExplosive14InchShell::CollisionPoints)
	{
		delete[] CExplosive14InchShell::CollisionPoints;
		CExplosive14InchShell::CollisionPoints = NULL;
	}
}

void CExplosive14InchShell::InitilizeExplosive14InchShell()
{
	CExplosive14InchShell::LoadExplosive14InchShellTextures();
	CExplosive14InchShell::CreateExplosive14InchShellCollisionPointsL();
}

void CExplosive14InchShell::RemoveExplosive14InchShell()
{
	CExplosive14InchShell::UnloadExplosive14InchShellTextures();
	CExplosive14InchShell::RemoveExplosive14InchShellCollisionPoints();
}

void CExplosive14InchShell::PossibleCollision(CInterval*& aInterval)
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
				//now check if it is not member of the same team and not decor or another projectile
				if((lCurrentGameObject->GetConflictSide() != iConflictSide))
				{
					TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
					//collide
					if(lCollisionPoint)
					{
						//check if explosion happens on water
						if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierWater)
						{
							//need to adjust explosion since the explosion should be centered relative to the collision point
							lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_14INCH_SHELL_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
							CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion14InchShellInWater::New(*lCollisionPoint));
						}else
						{
							//need to adjust explosion since the explosion should be centered relative to the collision point
							lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_14INCH_SHELL_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
							//create explosion on Ground
							CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion14InchShell::New(*lCollisionPoint));
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

	//clean up
	delete lGameObjects;
}
