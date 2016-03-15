/*
 ============================================================================
 Name		: Bullet13mm.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBullet13mm implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Projectiles/CBullet13mm.h"
#include "includes/game/backend/gameObjects/Explosions/CWaterSplash.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CPlayer.h"

SBoundingCollisionPoint* CBullet13mm::CollisionPoints;

CBullet13mm::CBullet13mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TBool aHarmGroundUnits) :
	CProjectile(NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(BULLET13MM_SPEED), BULLET13MM_RANGE, aAngel)
{
	iHarmGroundUnits = aHarmGroundUnits;
	iBulletDamageType = EDamageTypeBullet;
}

CBullet13mm::CBullet13mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed, TBool aHarmGroundUnits) :
	CProjectile(NULL, NULL, aLocation, aAttributes, aSpeed + TIntFloat::Convert(BULLET13MM_SPEED), BULLET13MM_RANGE, aAngel)
{
	iHarmGroundUnits = aHarmGroundUnits;
	iBulletDamageType = EDamageTypeBullet;
}

CBullet13mm::~CBullet13mm()
{
}

CBullet13mm*CBullet13mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier13mmBullet;
	CBullet13mm* self = new CBullet13mm(aLocation, lAttributes, aAngel, aSpeed, aHarmGroundUnits);
	self->ConstructL();
	return self;
}

CBullet13mm* CBullet13mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier13mmBullet;
	CBullet13mm* self = new CBullet13mm(aLocation, lAttributes, aAngel, aHarmGroundUnits);
	self->ConstructL();
	return self;
}

void CBullet13mm::ConstructL()
{
	if(iConflictSide == EConflictSideAmerican)
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet13mmBulletAmerican), BULLET13MM_WIDTH * CPlayer::ZoomFactor, BULLET13MM_HEIGHT, iZCor, 0, 1, 0, 1);
	else
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBullet13mmBulletJapanese), BULLET13MM_WIDTH * CPlayer::ZoomFactor, BULLET13MM_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBullet13mm::CollisionPoints);
	iSoundObject = CSoundObject::New(ESoundFileId_Hit_Bullet_Small, iHitBox);
}

//---------------- functions -------------------------

void CBullet13mm::LoadBullet13mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet13mmBulletAmerican);
	CGame::Game->iImageStore->LoadImage(EImageIdBullet13mmBulletJapanese);
}

void CBullet13mm::UnloadBullet13mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet13mmBulletAmerican);
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet13mmBulletJapanese);
}

void CBullet13mm::CreateBullet13mmCollisionPointsL()
{
	CBullet13mm::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBullet13mm::CollisionPoints[0].XOffset = -2;
	CBullet13mm::CollisionPoints[0].YOffset = BULLET13MM_HEIGHT + 1;
	CBullet13mm::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBullet13mm::CollisionPoints[1].XOffset = 5;
	CBullet13mm::CollisionPoints[1].YOffset = -1;
	CBullet13mm::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBullet13mm::CollisionPoints[2].XOffset = BULLET13MM_WIDTH + 4;
	CBullet13mm::CollisionPoints[2].YOffset = 0;
	CBullet13mm::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBullet13mm::CollisionPoints[3].XOffset = BULLET13MM_WIDTH - 3;
	CBullet13mm::CollisionPoints[3].YOffset = BULLET13MM_HEIGHT;
	CBullet13mm::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBullet13mm::CollisionPoints[4].XOffset = 0;
	CBullet13mm::CollisionPoints[4].YOffset = BULLET13MM_HEIGHT;
	CBullet13mm::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBullet13mm::RemoveBullet13mmCollisionPoints()
{
	if(CBullet13mm::CollisionPoints)
	{
		delete[] CBullet13mm::CollisionPoints;
		CBullet13mm::CollisionPoints = NULL;
	}
}

void CBullet13mm::InitilizeBullet13mm()
{
	CBullet13mm::LoadBullet13mmTextures();
	CBullet13mm::CreateBullet13mmCollisionPointsL();
}

void CBullet13mm::RemoveBullet13mm()
{
	CBullet13mm::UnloadBullet13mmTextures();
	CBullet13mm::RemoveBullet13mmCollisionPoints();
}

void CBullet13mm::AssignDamageType(TDamageType aDamageType)
{
	iBulletDamageType = aDamageType;
}

void CBullet13mm::PossibleCollision(CInterval*& aInterval)
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
					if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesPlane))
					{
						//collide
						if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
						{
							SDamage lDamage;
							lDamage.Damage = BULLET13MM_DAMAGE;
							lDamage.ArmorPenetration = BULLET13MM_AMORPENETRATION;
							lCurrentGameObject->TakeDamage(lDamage, iBulletDamageType);
							Destruct();//bullet is used up
							break;
						}
					}else if(iHarmGroundUnits && lCurrentGameObject->GetGameObjectType() == EObjectTypesGroundUnit)
					{
						//collide
						if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
						{
							SDamage lDamage;
							lDamage.Damage = BULLET13MM_DAMAGE;
							lDamage.ArmorPenetration = BULLET13MM_AMORPENETRATION;
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
							lDamage.Damage = BULLET13MM_DAMAGE;
							lDamage.ArmorPenetration = BULLET13MM_AMORPENETRATION;
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

void CBullet13mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iHarmGroundUnits);
}
void CBullet13mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileObjectLoadContentFromDisk(aReadStream);
	iHarmGroundUnits = aReadStream.ReadInt32();
}
