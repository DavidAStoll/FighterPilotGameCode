/*
 ============================================================================
 Name		: Explosive7InchShell.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive7InchShell implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CExplosive7InchShell::CollisionPoints;

CExplosive7InchShell::CExplosive7InchShell(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed) :
CGravitationalProjectile(aLocation, aAttributes, aSpeed, aAngel)
{
	// No implementation required
}

CExplosive7InchShell::~CExplosive7InchShell()
{
}

CExplosive7InchShell* CExplosive7InchShell::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosive7InchShell;
	CExplosive7InchShell* self = new CExplosive7InchShell(aLocation, lAttributes, aAngel, aSpeed);
	self->ConstructL();
	return self;
}

void CExplosive7InchShell::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet7InchExplosiveShell), EXPLOSIVE_7_INCH_SHELL_WIDTH, EXPLOSIVE_7_INCH_SHELL_HEIGHT, iZCor, 0, 1, 0, 1);

	AdjustCoordinatesAndTexture();
  UpdateGravitionalStatus();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CExplosive7InchShell::CollisionPoints);
}


//---------------- functions -------------------------

void CExplosive7InchShell::DestructAtCollisionPoint(TPointIntFloat* lCollisionPoint)//used if we have a location where it should explode
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game

	//create explosion at current Location
	TPointIntFloat lExplosionLocation;
	//needs to be centered
	lExplosionLocation.iX = lCollisionPoint->iX - TIntFloat::Convert(EXPLOSION_IN_AIR_1000KG_WIDTH) / 2;
	lExplosionLocation.iY = lCollisionPoint->iY - TIntFloat::Convert(EXPLOSION_IN_AIR_1000KG_HEIGHT) / 2;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInAir::New(lExplosionLocation));
}

void CExplosive7InchShell::LoadExplosive7InchShellTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet7InchExplosiveShell);
}


void CExplosive7InchShell::UnloadExplosive7InchShellTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet7InchExplosiveShell);
}

void CExplosive7InchShell::CreateExplosive7InchShellCollisionPointsL()
{
	CExplosive7InchShell::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CExplosive7InchShell::CollisionPoints[0].XOffset = -2;
	CExplosive7InchShell::CollisionPoints[0].YOffset = EXPLOSIVE_7_INCH_SHELL_HEIGHT + 1;
	CExplosive7InchShell::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CExplosive7InchShell::CollisionPoints[1].XOffset = 5;
	CExplosive7InchShell::CollisionPoints[1].YOffset = -1;
	CExplosive7InchShell::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CExplosive7InchShell::CollisionPoints[2].XOffset = EXPLOSIVE_7_INCH_SHELL_WIDTH + 4;
	CExplosive7InchShell::CollisionPoints[2].YOffset = 0;
	CExplosive7InchShell::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CExplosive7InchShell::CollisionPoints[3].XOffset = EXPLOSIVE_7_INCH_SHELL_WIDTH - 3;
	CExplosive7InchShell::CollisionPoints[3].YOffset = EXPLOSIVE_7_INCH_SHELL_HEIGHT;
	CExplosive7InchShell::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CExplosive7InchShell::CollisionPoints[4].XOffset = 0;
	CExplosive7InchShell::CollisionPoints[4].YOffset = EXPLOSIVE_7_INCH_SHELL_HEIGHT;
	CExplosive7InchShell::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CExplosive7InchShell::RemoveExplosive7InchShellCollisionPoints()
{
	if(CExplosive7InchShell::CollisionPoints)
	{
		delete[] CExplosive7InchShell::CollisionPoints;
		CExplosive7InchShell::CollisionPoints = NULL;
	}
}

void CExplosive7InchShell::InitilizeExplosive7InchShell()
{
	CExplosive7InchShell::LoadExplosive7InchShellTextures();
	CExplosive7InchShell::CreateExplosive7InchShellCollisionPointsL();
}

void CExplosive7InchShell::RemoveExplosive7InchShell()
{
	CExplosive7InchShell::UnloadExplosive7InchShellTextures();
	CExplosive7InchShell::RemoveExplosive7InchShellCollisionPoints();
}

void CExplosive7InchShell::PossibleCollision(CInterval*& aInterval)
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
					if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesShip))
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
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_1000KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInWater::New(*lCollisionPoint));
							}else
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_1000KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								//create explosion on Ground
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000Kg::New(*lCollisionPoint));
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
