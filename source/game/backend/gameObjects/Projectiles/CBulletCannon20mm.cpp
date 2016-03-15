/*
 * CBulletCannon20mm.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/Projectiles/CBulletCannon20mm.h"
#include "includes/game/backend/gameObjects/Decor/CGeneralSmokeCreator.h"
#include "includes/game/backend/gameObjects/Explosions/CWaterSplash.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CPlayer.h"

SBoundingCollisionPoint* CBulletCannon20mm::CollisionPoints;

CBulletCannon20mm::CBulletCannon20mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TBool aHarmGroundUnits) :
	CProjectile(NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(BULLETCANNON20MM_SPEED), BULLETCANNON20MM_RANGE, aAngel)
{
	iHarmGroundUnits = aHarmGroundUnits;
	iBulletDamageType = EDamageTypeBullet;
}

CBulletCannon20mm::CBulletCannon20mm(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TIntFloat aSpeed, TBool aHarmGroundUnits) :
	CProjectile(NULL, NULL, aLocation, aAttributes, aSpeed + TIntFloat::Convert(BULLETCANNON20MM_SPEED), BULLETCANNON20MM_RANGE, aAngel)
{
	iHarmGroundUnits = aHarmGroundUnits;
	iBulletDamageType = EDamageTypeBullet;
}

CBulletCannon20mm::~CBulletCannon20mm()
{
}

CBulletCannon20mm*CBulletCannon20mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TIntFloat aSpeed, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier20mmBulletCannon;
	CBulletCannon20mm* self = new CBulletCannon20mm(aLocation, lAttributes, aAngel, aSpeed, aHarmGroundUnits);
	self->ConstructL();
	return self;
}

CBulletCannon20mm* CBulletCannon20mm::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifier20mmBulletCannon;
	CBulletCannon20mm* self = new CBulletCannon20mm(aLocation, lAttributes, aAngel, aHarmGroundUnits);
	self->ConstructL();
	return self;
}

void CBulletCannon20mm::ConstructL()
{
	if(iConflictSide == EConflictSideAmerican)
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBulletCannon20mmBulletAmerican), BULLETCANNON20MM_WIDTH * CPlayer::ZoomFactor, BULLETCANNON20MM_HEIGHT, iZCor, 0, 1, 0, 1);
	else
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBulletCannon20mmBulletJapanese), BULLETCANNON20MM_WIDTH * CPlayer::ZoomFactor, BULLETCANNON20MM_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CBulletCannon20mm::CollisionPoints);
	//sound when bullet this target
	iSoundObject = CSoundObject::New(ESoundFileId_Hit_Bullet_Small, iHitBox);
	//for smoke trial
	TInt lWidthOfBullet = BULLETCANNON20MM_WIDTH * CPlayer::ZoomFactor + iSpeed.GetIntInBaseInt(); //in order to fill the gaps between frames
	TInt lHeightOfBullet = BULLETCANNON20MM_HEIGHT;

	iSmokeTrial = CGeneralSmokeCreator::New(this, &iAngle, -(BULLETCANNON20MM_WIDTH * CPlayer::ZoomFactor) / 2, 0, true, KColourGray, 100, TSize(lWidthOfBullet, lHeightOfBullet), BULLETCANNON20MM_FRAMES_FOR_SMOKE_TRIAL,
																					0, TIntFloat::Convert(0), TIntFloat::Convert(0), TIntFloat::Convert(0)); //NOT IMPORTANT SINCE SMOKE DOES NOT MOVE
}

//---------------- functions -------------------------

void CBulletCannon20mm::LoadBulletCannon20mmTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBulletCannon20mmBulletAmerican);
	CGame::Game->iImageStore->LoadImage(EImageIdBulletCannon20mmBulletJapanese);
}

void CBulletCannon20mm::UnloadBulletCannon20mmTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBulletCannon20mmBulletAmerican);
	CGame::Game->iImageStore->UnloadImage(EImageIdBulletCannon20mmBulletJapanese);
}

void CBulletCannon20mm::CreateBulletCannon20mmCollisionPointsL()
{
	CBulletCannon20mm::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CBulletCannon20mm::CollisionPoints[0].XOffset = -2;
	CBulletCannon20mm::CollisionPoints[0].YOffset = BULLETCANNON20MM_HEIGHT + 1;
	CBulletCannon20mm::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CBulletCannon20mm::CollisionPoints[1].XOffset = 5;
	CBulletCannon20mm::CollisionPoints[1].YOffset = -1;
	CBulletCannon20mm::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CBulletCannon20mm::CollisionPoints[2].XOffset = BULLETCANNON20MM_WIDTH + 4;
	CBulletCannon20mm::CollisionPoints[2].YOffset = 0;
	CBulletCannon20mm::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CBulletCannon20mm::CollisionPoints[3].XOffset = BULLETCANNON20MM_WIDTH - 3;
	CBulletCannon20mm::CollisionPoints[3].YOffset = BULLETCANNON20MM_HEIGHT;
	CBulletCannon20mm::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CBulletCannon20mm::CollisionPoints[4].XOffset = 0;
	CBulletCannon20mm::CollisionPoints[4].YOffset = BULLETCANNON20MM_HEIGHT;
	CBulletCannon20mm::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CBulletCannon20mm::RemoveBulletCannon20mmCollisionPoints()
{
	if(CBulletCannon20mm::CollisionPoints)
	{
		delete[] CBulletCannon20mm::CollisionPoints;
		CBulletCannon20mm::CollisionPoints = NULL;
	}
}

void CBulletCannon20mm::InitilizeBulletCannon20mm()
{
	CBulletCannon20mm::LoadBulletCannon20mmTextures();
	CBulletCannon20mm::CreateBulletCannon20mmCollisionPointsL();
}

void CBulletCannon20mm::RemoveBulletCannon20mm()
{
	CBulletCannon20mm::UnloadBulletCannon20mmTextures();
	CBulletCannon20mm::RemoveBulletCannon20mmCollisionPoints();
}

void CBulletCannon20mm::AssignDamageType(TDamageType aDamageType)
{
	iBulletDamageType = aDamageType;
}

void CBulletCannon20mm::Update()
{
	CProjectile::Update();
	iSmokeTrial->Update();
}

void CBulletCannon20mm::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane | EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesShip | EObjectTypesFloor | EObjectTypesBuildingAA);

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
							lDamage.Damage = BULLETCANNON20MM_DAMAGE;
							lDamage.ArmorPenetration = BULLETCANNON20MM_AMORPENETRATION;
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
							lDamage.Damage = BULLETCANNON20MM_DAMAGE;
							lDamage.ArmorPenetration = BULLETCANNON20MM_AMORPENETRATION;
							lCurrentGameObject->TakeDamage(lDamage, iBulletDamageType);
							Destruct();//bullet is used up
							break;
						}
					}else if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesArmouredGroundUnit | EObjectTypesShip | EObjectTypesBuildingAA))
					{
						TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
						if(lCollisionPoint)		//collide
						{
							//find explosion location
							TPointIntFloat lExplosionLocation;
							lExplosionLocation.iX = lCollisionPoint->iX - TIntFloat::Convert(EXPLOSION_IN_AIR_100KG_WIDTH) / 2;
							lExplosionLocation.iY = lCollisionPoint->iY - TIntFloat::Convert(EXPLOSION_IN_AIR_100KG_HEIGHT) / 2;
							CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100KgInAir::New(lExplosionLocation));
							Destruct();//bullet is used up
							break;
						}
					}
					else if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesFloor))
					{
						TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
						if(lCollisionPoint) //collide
						{
							//check if explosion happens on water
							if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierWater)
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100KgInWater::New(*lCollisionPoint));
							}
							else
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100Kg::New(*lCollisionPoint));
							}

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

void CBulletCannon20mm::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iHarmGroundUnits);
}
void CBulletCannon20mm::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileObjectLoadContentFromDisk(aReadStream);
	iHarmGroundUnits = aReadStream.ReadInt32();
}

