/*
 ============================================================================
 Name		: UnderWaterTorpedo.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CUnderWaterTorpedo implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/Projectiles/CUnderWaterTorpedo.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInWater.h"

SBoundingCollisionPoint* CUnderWaterTorpedo::CollisionPoints;

CUnderWaterTorpedo::CUnderWaterTorpedo(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel) :
	CProjectile(NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(UNDER_WATER_TORPEDO_SPEED), UNDER_WATER_TORPEDO_RANGE, aAngel)
{
	// No implementation required
}

CUnderWaterTorpedo::~CUnderWaterTorpedo()
{
}

CUnderWaterTorpedo* CUnderWaterTorpedo::New(TPointIntFloat aLocation, TConflictSide aConflictSide, TInt aAngel, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectileExplosive;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierUnderWaterTorpedo;
	CUnderWaterTorpedo* self = new CUnderWaterTorpedo(aLocation, lAttributes, aAngel);
	self->ConstructL();
	return self;
}

void CUnderWaterTorpedo::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosionTorpedoGas), UNDER_WATER_TORPEDO_WIDTH, UNDER_WATER_TORPEDO_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CUnderWaterTorpedo::CollisionPoints);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = UNDER_WATER_TORPED_GAS_RELEASE_SEQUENCE_FRAMES_UNTIL_SWITCH;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosionTorpedoGas);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = UNDER_WATER_TORPEDO_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
	
	iSoundObject = CSoundObject::New(ESoundFileId_Torepedo_Travel, GetHitBox());
	iSoundObject->PlayRepeatHitBox();
}

//---------------- functions -------------------------

void CUnderWaterTorpedo::LoadUnderWaterTorpedoTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosionTorpedoGas);
}

void CUnderWaterTorpedo::UnloadUnderWaterTorpedoTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosionTorpedoGas);
}

void CUnderWaterTorpedo::CreateUnderWaterTorpedoCollisionPointsL()
{
	CUnderWaterTorpedo::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CUnderWaterTorpedo::CollisionPoints[0].XOffset = -2;
	CUnderWaterTorpedo::CollisionPoints[0].YOffset = UNDER_WATER_TORPEDO_HEIGHT;
	CUnderWaterTorpedo::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CUnderWaterTorpedo::CollisionPoints[1].XOffset = 8;
	CUnderWaterTorpedo::CollisionPoints[1].YOffset = 0;
	CUnderWaterTorpedo::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CUnderWaterTorpedo::CollisionPoints[2].XOffset = UNDER_WATER_TORPEDO_WIDTH + 5;
	CUnderWaterTorpedo::CollisionPoints[2].YOffset = 0;
	CUnderWaterTorpedo::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CUnderWaterTorpedo::CollisionPoints[3].XOffset = UNDER_WATER_TORPEDO_WIDTH - 3;
	CUnderWaterTorpedo::CollisionPoints[3].YOffset = UNDER_WATER_TORPEDO_HEIGHT;
	CUnderWaterTorpedo::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CUnderWaterTorpedo::CollisionPoints[4].XOffset = 0;
	CUnderWaterTorpedo::CollisionPoints[4].YOffset = UNDER_WATER_TORPEDO_HEIGHT;
	CUnderWaterTorpedo::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CUnderWaterTorpedo::RemoveUnderWaterTorpedoCollisionPoints()
{
	if(CUnderWaterTorpedo::CollisionPoints)
	{
		delete[] CUnderWaterTorpedo::CollisionPoints;
		CUnderWaterTorpedo::CollisionPoints = NULL;
	}
}

void CUnderWaterTorpedo::InitilizeUnderWaterTorpedo()
{
	CUnderWaterTorpedo::LoadUnderWaterTorpedoTextures();
	CUnderWaterTorpedo::CreateUnderWaterTorpedoCollisionPointsL();
}

void CUnderWaterTorpedo::RemoveUnderWaterTorpedo()
{
	CUnderWaterTorpedo::UnloadUnderWaterTorpedoTextures();
	CUnderWaterTorpedo::RemoveUnderWaterTorpedoCollisionPoints();
}

void CUnderWaterTorpedo::Update()
{
	iAnimationPlayer->Update();

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

void CUnderWaterTorpedo::Destruct()//default behaviour, just set it to dead to remove it
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game

	//create explosion at current Location
	TPointIntFloat lExplosionLocation;
	//needs to be centered
	lExplosionLocation.iX = iCoordinates.iX - TIntFloat::Convert(EXPLOSION_1000KG_IN_WATER_WIDTH) / 2;
	lExplosionLocation.iY = iCoordinates.iY;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInWater::New(lExplosionLocation));
}

void CUnderWaterTorpedo::DestructWithoutExplosion()
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game
}

void CUnderWaterTorpedo::PossibleCollision(CInterval*& aInterval)
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
				if((lCurrentGameObject->GetGameObjectType() & (EObjectTypesShip)) || lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierLand) // to avoid that a torpedo is under water on land
				{

					TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
					//collide
					if(lCollisionPoint)
					{
						SDamage lDamage;
						lDamage.Damage = UNDER_WATER_TORPEDO_DAMAGE;
						lDamage.ArmorPenetration = UNDER_WATER_TORPEDO_ARMORPENETRATION;
						lCurrentGameObject->TakeDamage(lDamage, EDamageTypeExplosion);

						//need to adjust explosion since the explosion should be centered relative to the collision point
						lCollisionPoint->iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
						lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_1000KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
						CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInWater::New(*lCollisionPoint));

						//torpedo hit target and is used up now
						DestructWithoutExplosion();
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
