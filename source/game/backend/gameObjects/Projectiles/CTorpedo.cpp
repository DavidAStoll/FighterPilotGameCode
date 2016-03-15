/*
 ============================================================================
 Name		: Torpedo.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CTorpedo implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion500Kg.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInWater.h"
#include "includes/game/backend/gameObjects/Projectiles/CTorpedo.h"
#include "includes/game/backend/gameObjects/Projectiles/CUnderWaterTorpedo.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CTorpedo::CollisionPoints;

CTorpedo::CTorpedo(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel) :
	CProjectileBomb(aLocation, aAttributes, aSpeed, aAngel)
{
	iHeightTravelled = TIntFloat::Convert(0);
}

CTorpedo::~CTorpedo()
{
}

CTorpedo* CTorpedo::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierTorpedo;
	CTorpedo* self = new CTorpedo(aLocation, lAttributes, aSpeed, aAngel);
	self->ConstructL();
	return self;
}

void CTorpedo::ConstructL()
{
	//create Hitbox and Texture Object
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBombTorpedo), TORPEDO_WIDTH, TORPEDO_HEIGHT, iZCor, 0, 0.90625, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CTorpedo::CollisionPoints);
	iSoundObject = CSoundObject::New(ESoundFileId_Torepedo_Splash, iHitBox);
}

//------------------ functions -------------------------//

void CTorpedo::InitilizeTorpedo()
{
	CTorpedo::CreateTorpedoCollisionPointsL();
	CTorpedo::LoadTorpedoTextures();
}

void CTorpedo::RemoveTorpedo()
{
	CTorpedo::RemoveTorpedoCollisionPoints();
	CTorpedo::UnloadTorpedoTextures();
}

void CTorpedo::LoadTorpedoTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBombTorpedo);
}

void CTorpedo::UnloadTorpedoTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBombTorpedo);
}

void CTorpedo::CreateTorpedoCollisionPointsL()
{
	CTorpedo::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CTorpedo::CollisionPoints[0].XOffset = -2;
	CTorpedo::CollisionPoints[0].YOffset = TORPEDO_HEIGHT + 3;
	CTorpedo::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CTorpedo::CollisionPoints[1].XOffset = 8;
	CTorpedo::CollisionPoints[1].YOffset = -3;
	CTorpedo::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CTorpedo::CollisionPoints[2].XOffset = TORPEDO_WIDTH + 5;
	CTorpedo::CollisionPoints[2].YOffset = 0;
	CTorpedo::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CTorpedo::CollisionPoints[3].XOffset = TORPEDO_WIDTH - 3;
	CTorpedo::CollisionPoints[3].YOffset = TORPEDO_HEIGHT;
	CTorpedo::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CTorpedo::CollisionPoints[4].XOffset = 0;
	CTorpedo::CollisionPoints[4].YOffset = TORPEDO_HEIGHT + 3;
	CTorpedo::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CTorpedo::RemoveTorpedoCollisionPoints()
{
	if(CTorpedo::CollisionPoints)
	{
		delete[] CTorpedo::CollisionPoints;
		CTorpedo::CollisionPoints = NULL;
	}
}

void CTorpedo::Update()
{
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
		iHeightTravelled += CMath::Abs(iPixelsPerMoveY);
	}
}

void CTorpedo::PossibleCollision(CInterval*& aInterval)
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

						//hit Water, now check if conditions are right to become a Under Water torpedo or just to explode
						if( (iAngle == 180 || iAngle == 0) && CMath::Abs(iSpeed) < TIntFloat::Convert(UNDER_WATER_TORPED_MAX_SPEED_OTHERWISE_EXPLODE) && iHeightTravelled < TIntFloat::Convert(UNDER_WATER_TORPED_MAX_DROP_HEIGHT_OTHERWISE_EXPLODE))
						{
							//need to adjust explosion since the explosion should be centered relative to the collision point
							lCollisionPoint->iX -= TIntFloat::Convert(UNDER_WATER_TORPEDO_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
							CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CUnderWaterTorpedo::New(*lCollisionPoint, iConflictSide, iAngle, iObjectReflected));
							iSoundObject->PlayCheckPointWithoutChannelUpdate(iHitBox,ESoundFileId_Torepedo_Splash);
						}else //conditions are not right, just explode
						{
							//need to adjust explosion since the explosion should be centered relative to the collision point
							lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_1000KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
							CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInWater::New(*lCollisionPoint));
						}
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

