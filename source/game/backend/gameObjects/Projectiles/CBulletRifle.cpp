/*
 ============================================================================
 Name		: BulletRifle.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBulletRifle implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Projectiles/CBulletRifle.h"
#include "includes/game/backend/gameObjects/Explosions/CWaterSplash.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CBulletRifle::CollisionPoints;

CBulletRifle::CBulletRifle(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel) :
	CGravitationalProjectile(aLocation, aAttributes, TIntFloat::Convert(BULLET_RIFLE_SPEED), aAngel)
{
}

CBulletRifle::CBulletRifle(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed) :
	CGravitationalProjectile(aLocation, aAttributes, aSpeed + TIntFloat::Convert(BULLET_RIFLE_SPEED), aAngel)
{
}

CBulletRifle::~CBulletRifle()
{
}

CBulletRifle* CBulletRifle::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierRifleBullet;
	CBulletRifle* self = new CBulletRifle(aLocation, lAttributes, aAngel, aSpeed);
	self->ConstructL();
	return self;
}

CBulletRifle* CBulletRifle::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierRifleBullet;
	CBulletRifle* self = new CBulletRifle(aLocation, lAttributes, aAngel);
	self->ConstructL();
	return self;
}

void CBulletRifle::ConstructL()
{
	if(iConflictSide == EConflictSideAmerican)
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBulletBulletRifleAmerican), BULLET_RIFLE_WIDTH, BULLET_RIFLE_HEIGHT, iZCor, 0, 1, 0, 1);
	else
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBulletBulletRifleJapanese), BULLET_RIFLE_WIDTH, BULLET_RIFLE_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();
	UpdateGravitionalStatus();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBulletRifle::CollisionPoints);
	iSoundObject = CSoundObject::New(ESoundFileId_Hit_Bullet_Small, iHitBox);
}

//---------------- functions -------------------------

void CBulletRifle::LoadBulletRifleTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBulletBulletRifleAmerican);
	CGame::Game->iImageStore->LoadImage(EImageIdBulletBulletRifleJapanese);
}

void CBulletRifle::UnloadBulletRifleTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBulletBulletRifleAmerican);
	CGame::Game->iImageStore->UnloadImage(EImageIdBulletBulletRifleJapanese);
}

void CBulletRifle::CreateBulletRifleCollisionPointsL()
{
	CBulletRifle::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBulletRifle::CollisionPoints[0].XOffset = -2;
	CBulletRifle::CollisionPoints[0].YOffset = BULLET_RIFLE_HEIGHT + 1;
	CBulletRifle::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBulletRifle::CollisionPoints[1].XOffset = 5;
	CBulletRifle::CollisionPoints[1].YOffset = -1;
	CBulletRifle::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBulletRifle::CollisionPoints[2].XOffset = BULLET_RIFLE_WIDTH + 4;
	CBulletRifle::CollisionPoints[2].YOffset = 0;
	CBulletRifle::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBulletRifle::CollisionPoints[3].XOffset = BULLET_RIFLE_WIDTH - 3;
	CBulletRifle::CollisionPoints[3].YOffset = BULLET_RIFLE_HEIGHT;
	CBulletRifle::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBulletRifle::CollisionPoints[4].XOffset = 0;
	CBulletRifle::CollisionPoints[4].YOffset = BULLET_RIFLE_HEIGHT;
	CBulletRifle::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBulletRifle::RemoveBulletRifleCollisionPoints()
{
	if(CBulletRifle::CollisionPoints)
	{
		delete[] CBulletRifle::CollisionPoints;
		CBulletRifle::CollisionPoints = NULL;
	}
}

void CBulletRifle::InitilizeBulletRifle()
{
	CBulletRifle::LoadBulletRifleTextures();
	CBulletRifle::CreateBulletRifleCollisionPointsL();
}

void CBulletRifle::RemoveBulletRifle()
{
	CBulletRifle::UnloadBulletRifleTextures();
	CBulletRifle::RemoveBulletRifleCollisionPoints();
}

void CBulletRifle::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane | EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesShip | EObjectTypesFloor);

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
					if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesGroundUnit | EObjectTypesPlane))
					{
						//collide
						if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
						{
							SDamage lDamage;
							lDamage.Damage = BULLET_RIFLE_DAMAGE;
							lDamage.ArmorPenetration = BULLET_RIFLE_AMORPENETRATION;
							lCurrentGameObject->TakeDamage(lDamage, EDamageTypeBullet);
							Destruct();//bullet is used up
							break;
						}
					}else if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesArmouredGroundUnit | EObjectTypesShip))
					{
						//collide
						if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
						{
							iSoundObject->PlayHitBox(ESoundFileId_Hit_Bullet_Small);
							SDamage lDamage;
							lDamage.Damage = BULLET_RIFLE_DAMAGE;
							lDamage.ArmorPenetration = BULLET_RIFLE_AMORPENETRATION;
							lCurrentGameObject->TakeDamage(lDamage, EDamageTypeBullet);
							Destruct();//bullet is used up
							break;
						}
					}else if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesFloor))
					{

						TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
						//collide
						if(lCollisionPoint)
						{
							iSoundObject->PlayHitBox(ESoundFileId_Bullet_Hit_Tank);
							//check if explosion happens on water
							if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierWater)
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(WATER_SPLASH_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CWaterSplash::New(*lCollisionPoint));
							}else
							{
								//don't do anyhting for now
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
