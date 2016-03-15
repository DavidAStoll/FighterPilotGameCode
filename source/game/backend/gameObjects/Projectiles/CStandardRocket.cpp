/*
 ============================================================================
 Name		: StandardRocket.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStandardRocket implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CStandardRocket::CollisionPoints;

CStandardRocket::CStandardRocket(TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel, CGameObject* aCreator) :
	CProjectile(NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(STANDARD_ROCKET_SPEED) + aSpeed, STANDARD_ROCKET_RANGE, aAngel)
{
	iCreatorObject = aCreator;
}

CStandardRocket::CStandardRocket(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel, CGameObject* aCreator) :
	CProjectile(NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(STANDARD_ROCKET_SPEED) + aSpeed, STANDARD_ROCKET_RANGE, aAngel)
{
	iCreatorObject = aCreator;
}

CStandardRocket::~CStandardRocket()
{
}

CStandardRocket* CStandardRocket::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aAngel, TBool aReflected, CGameObject* aCreator)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesProjectile;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierStandardRocket;
	CStandardRocket* self = new CStandardRocket(aLocation, lAttributes, aSpeed, aAngel, aCreator);
	self->ConstructL();
	return self;
}

void CStandardRocket::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdRocketStandardRocketFireSequence), STANDARD_ROCKET_WIDTH, STANDARD_ROCKET_HEIGHT, iZCor, 0, 1, 0, 1);
	AdjustCoordinatesAndTexture();

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_PROJECTILE_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CStandardRocket::CollisionPoints);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 2;//since we use only one image, only call method once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdRocketStandardRocketFireSequence);
	lDefaultRequest.TotalImages = 4;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = STANDARD_ROCKET_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//Smoke Creator for Rocket smoke trail
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, 0, 4,  true, KColourDarkGray, 100, TSize(iSpeed.GetIntInBaseInt() + 10,8), 10, 0, TIntFloat::Convert(0), TIntFloat::Convert(0), TIntFloat::Convert(0));

	//play sound
	iSoundObject = CSoundObject::New(ESoundFileId_Rocket_Firing, iHitBox);
	iSoundObject->PlayHitBox();
}

//---------------- functions -------------------------

void CStandardRocket::LoadStandardRocketTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdRocketStandardRocketFireSequence);
}

void CStandardRocket::Update()
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

	if(iAlive)
	{
		iSmokeCreator->Update();
	}
}

void CStandardRocket::UnloadStandardRocketTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdRocketStandardRocketFireSequence);
}

void CStandardRocket::CreateStandardRocketCollisionPointsL()
{
	CStandardRocket::CollisionPoints = new SBoundingCollisionPoint[BASIC_PROJECTILE_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CStandardRocket::CollisionPoints[0].XOffset = -2;
	CStandardRocket::CollisionPoints[0].YOffset = STANDARD_ROCKET_HEIGHT + 3;
	CStandardRocket::CollisionPoints[0].iMovementType = EPointMovementTypeConnectionStartPoint;
	CStandardRocket::CollisionPoints[1].XOffset = 8;
	CStandardRocket::CollisionPoints[1].YOffset = -3;
	CStandardRocket::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CStandardRocket::CollisionPoints[2].XOffset = STANDARD_ROCKET_WIDTH + 5;
	CStandardRocket::CollisionPoints[2].YOffset = 0;
	CStandardRocket::CollisionPoints[2].iMovementType = EPointMovementTypeConnectionEndPoint;
	CStandardRocket::CollisionPoints[3].XOffset = STANDARD_ROCKET_WIDTH - 3;
	CStandardRocket::CollisionPoints[3].YOffset = STANDARD_ROCKET_HEIGHT;
	CStandardRocket::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CStandardRocket::CollisionPoints[4].XOffset = 0;
	CStandardRocket::CollisionPoints[4].YOffset = STANDARD_ROCKET_HEIGHT + 3;
	CStandardRocket::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CStandardRocket::RemoveStandardRocketCollisionPoints()
{
	if(CStandardRocket::CollisionPoints)
	{
		delete[] CStandardRocket::CollisionPoints;
		CStandardRocket::CollisionPoints = NULL;
	}
}

void CStandardRocket::InitilizeStandardRocket()
{
	CStandardRocket::LoadStandardRocketTextures();
	CStandardRocket::CreateStandardRocketCollisionPointsL();
}

void CStandardRocket::RemoveStandardRocket()
{
	CStandardRocket::UnloadStandardRocketTextures();
	CStandardRocket::RemoveStandardRocketCollisionPoints();
}

void CStandardRocket::Destruct()//default behaviour, just set it to dead to remove it
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game

	//create explosion at current Location
	TPointIntFloat lExplosionLocation;
	//needs to be centered
	lExplosionLocation.iX = iCoordinates.iX - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_WIDTH) / 2;
	lExplosionLocation.iY = iCoordinates.iY - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_HEIGHT) / 2;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500KgInAir::NewForRocket(lExplosionLocation, STANDARD_ROCKET_DAMAGAE, STANDARD_ROCKET_ARMORPENETRATION));
}

void CStandardRocket::Destruct(TPointIntFloat* lCollisionPoint)//used if we have a location where it should explode
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game

	//create explosion at current Location
	TPointIntFloat lExplosionLocation;
	//needs to be centered
	lExplosionLocation.iX = lCollisionPoint->iX - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_WIDTH) / 2;
	lExplosionLocation.iY = lCollisionPoint->iY - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_HEIGHT) / 2;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500KgInAir::NewForRocket(lExplosionLocation, STANDARD_ROCKET_DAMAGAE, STANDARD_ROCKET_ARMORPENETRATION));
}

void CStandardRocket::DestructWithoutExplosion()
{
	iAlive = false;
	iDoNotDraw = true;
	iRecycleObject = true;//remove from game
}

void CStandardRocket::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesBuilding | EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesPlane | EObjectTypesShip | EObjectTypesFloor);

	if(iAlive)
	{
		for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

			//only collide with alive objects
			if(lCurrentGameObject->IsAlive() && lCurrentGameObject != this)
			{
				//make sure the rocket doesn't collide with the object that created it
				if(lCurrentGameObject != iCreatorObject)
				{
					if(lCurrentGameObject->GetGameObjectType() & (EObjectTypesBuilding | EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit | EObjectTypesPlane |  EObjectTypesShip))
					{
						//special case, don't collide with Airport or Hangar buildings
						if(lCurrentGameObject->GetGameObjectIdentifier() != EGameObjectIdentifierAirport && lCurrentGameObject->GetGameObjectIdentifier() != EGameObjectIdentifierHangar)
						{
							TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
							//collide
							if(lCollisionPoint)
							{
								//need to create an explosion here
								Destruct(lCollisionPoint);//alread centers the explosion relative to the collision point
								delete lCollisionPoint;
								break;
							}
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
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_IN_WATER_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100KgInWater::New(*lCollisionPoint));
							}else
							{
								//need to adjust explosion since the explosion should be centered relative to the collision point
								lCollisionPoint->iX -= TIntFloat::Convert(EXPLOSION_100KG_WIDTH) / 2;//since the collision point is at the left end of the explosion, we center it by moving the collision point left
								//create explosion on Ground
								CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion100Kg::New(*lCollisionPoint));
							}

							//rocket is used Up
							DestructWithoutExplosion();
							delete lCollisionPoint;
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

void CStandardRocket::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileObjectSaveContentToDisk(aOutputStream);
	TUint32 lCreatorObjectPointerMemoryAddress = (TUint32)iCreatorObject;
	aOutputStream.WriteUint32(lCreatorObjectPointerMemoryAddress);
}
void CStandardRocket::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileObjectLoadContentFromDisk(aReadStream);
	TUint32 lOldCreatorObjectPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lOldCreatorObjectPointerMemoryAddress, (TUint32)(&iCreatorObject));
}
