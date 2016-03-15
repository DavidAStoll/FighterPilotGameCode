/*
 * CBullet8mm.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Projectiles/CBullet8mm.h"
#include "includes/game/backend/gameObjects/Explosions/CWaterSplash.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CPlayer.h"

SBoundingCollisionPoint* CBullet8mm::CollisionPoints;

CBullet8mm::CBullet8mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel) :
	CProjectile(NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(BULLET8MM_SPEED), BULLET8MM_RANGE, aAngel)
{
	iBulletDamageType = EDamageTypeBullet;
}

CBullet8mm::CBullet8mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed) :
	CProjectile(NULL, NULL, aLocation, aAttributes, aSpeed + TIntFloat::Convert(BULLET8MM_SPEED), BULLET8MM_RANGE, aAngel)
{
	iBulletDamageType = EDamageTypeBullet;
}

CBullet8mm::~CBullet8mm()
{
}

CBullet8mm*CBullet8mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier8mmBullet;
	CBullet8mm* self = new CBullet8mm(aLocation, lAttributes, aAngel, aSpeed);
	self->ConstructL();
	return self;
}

CBullet8mm* CBullet8mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier8mmBullet;
	CBullet8mm* self = new CBullet8mm(aLocation, lAttributes, aAngel);
	self->ConstructL();
	return self;
}

void CBullet8mm::ConstructL()
{
	if(iConflictSide == EConflictSideAmerican)
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet8mmBulletAmerican), BULLET8MM_WIDTH * CPlayer::ZoomFactor, BULLET8MM_HEIGHT, iZCor, 0, 1, 0, 1);
	else
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet8mmBulletJapanese), BULLET8MM_WIDTH * CPlayer::ZoomFactor, BULLET8MM_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBullet8mm::CollisionPoints);
	iSoundObject = CSoundObject::New(ESoundFileId_Hit_Bullet_Small, iHitBox);
}

//---------------- functions -------------------------

void CBullet8mm::LoadBullet8mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet8mmBulletAmerican);
	CGame::Game->iImageStore->LoadImage(EImageIdBullet8mmBulletJapanese);
}

void CBullet8mm::UnloadBullet8mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet8mmBulletAmerican);
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet8mmBulletJapanese);
}

void CBullet8mm::CreateBullet8mmCollisionPointsL()
{
	CBullet8mm::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBullet8mm::CollisionPoints[0].XOffset = -2;
	CBullet8mm::CollisionPoints[0].YOffset = BULLET8MM_HEIGHT + 1;
	CBullet8mm::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBullet8mm::CollisionPoints[1].XOffset = 5;
	CBullet8mm::CollisionPoints[1].YOffset = -1;
	CBullet8mm::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBullet8mm::CollisionPoints[2].XOffset = BULLET8MM_WIDTH + 4;
	CBullet8mm::CollisionPoints[2].YOffset = 0;
	CBullet8mm::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBullet8mm::CollisionPoints[3].XOffset = BULLET8MM_WIDTH - 3;
	CBullet8mm::CollisionPoints[3].YOffset = BULLET8MM_HEIGHT;
	CBullet8mm::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBullet8mm::CollisionPoints[4].XOffset = 0;
	CBullet8mm::CollisionPoints[4].YOffset = BULLET8MM_HEIGHT;
	CBullet8mm::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBullet8mm::RemoveBullet8mmCollisionPoints()
{
	if(CBullet8mm::CollisionPoints)
	{
		delete[] CBullet8mm::CollisionPoints;
		CBullet8mm::CollisionPoints = NULL;
	}
}

void CBullet8mm::InitilizeBullet8mm()
{
	CBullet8mm::LoadBullet8mmTextures();
	CBullet8mm::CreateBullet8mmCollisionPointsL();
}

void CBullet8mm::RemoveBullet8mm()
{
	CBullet8mm::UnloadBullet8mmTextures();
	CBullet8mm::RemoveBullet8mmCollisionPoints();
}

void CBullet8mm::AssignDamageType(TDamageType aDamageType)
{
	iBulletDamageType = aDamageType;
}

void CBullet8mm::PossibleCollision(CInterval*& aInterval)
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
					if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesPlane | EObjectTypesGroundUnit))
					{
						//collide
						if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
						{
							SDamage lDamage;
							lDamage.Damage = BULLET8MM_DAMAGE;
							lDamage.ArmorPenetration = BULLET8MM_AMORPENETRATION;
							lCurrentGameObject->TakeDamage(lDamage, iBulletDamageType);
							Destruct();//bullet is used up
							break;
						}
					}else if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesArmouredGroundUnit | EObjectTypesShip))
					{
						//collide
						if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
						{
							CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Bullet_Hit_Tank, GetHitBox());
							lSoundObject->PlayHitBox();
							delete lSoundObject;
							SDamage lDamage;
							lDamage.Damage = BULLET8MM_DAMAGE;
							lDamage.ArmorPenetration = BULLET8MM_AMORPENETRATION;
							lCurrentGameObject->TakeDamage(lDamage, iBulletDamageType);
							Destruct();//bullet is used up
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
								lCollisionPoint->iX -= TIntFloat::Convert(WATER_SPLASH_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CWaterSplash::New(*lCollisionPoint));
							}
							else if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierLand)
							{
								CSoundObject* lSoundObject = CSoundObject::New(ESoundFileId_Bullet_Hit_Ground, GetHitBox());
								lSoundObject->PlayHitBox();
								delete lSoundObject;
							}
							else
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

