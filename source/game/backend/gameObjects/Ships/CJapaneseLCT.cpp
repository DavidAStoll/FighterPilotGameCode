/*
 ============================================================================
 Name		: JapaneseLCT.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseLCT implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"
;
SBoundingCollisionPoint* CJapaneseLCT::CollisionPoints;

CJapaneseLCT::CJapaneseLCT(TInt aXLocation, SGameObjectAttributes& aAttributes, TJapaneseLCTType aType, SGroundUnitOrder& aOrder) :
	CShip(JAPANESE_LCT_Z, aXLocation, JAPANESE_LCT_KEEL_DEPTH,TIntFloat::Convert(JAPANESE_LCT_DEFAULT_SPEED), aAttributes)
{
	iSpeed = TIntFloat::Convert(JAPANESE_LCT_DEFAULT_SPEED);
	iLCTType = aType;
	iHasTroopsLoaded = true;
	iOrderForUnloadedTroops = aOrder;
}

CJapaneseLCT::~CJapaneseLCT()
{
	if(iSmokeCreator)
	{
		delete iSmokeCreator;
		iSmokeCreator = NULL;
	}
}

CJapaneseLCT* CJapaneseLCT::New(TInt aXLocation, TBool aReflected, TJapaneseLCTType aType, SGroundUnitOrder& aOrder)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::JapaneseLCTHealth;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_LCT_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseLCT;

	CJapaneseLCT* self = new CJapaneseLCT(aXLocation, lAttributes, aType, aOrder);
	self->ConstructL();
	return self;
}


void CJapaneseLCT::ConstructL()
{
	TImageId lLCTTexture;
	switch(iLCTType)
	{
		case EJapaneseLCTTypeEmpty:
		{
			lLCTTexture = EImageIdShipJapaneseLCTEmpty;
			break;
		}
		case EJapaneseLCTTypeTankAndTroops:
		{
			lLCTTexture = EImageIdShipJapaneseLCTLoadedWithSoldiersAndTanks;
			break;
		}
		case EJapaneseLCTTypeTanksOnly:
		{
			lLCTTexture = EImageIdShipJapaneseLCTLoadedWithTanks;
			break;
		}
		case EJapaneseLCTTypeTroopsOnly:
		{
			lLCTTexture = EImageIdShipJapaneseLCTLoadedWithSoldiers;
			break;
		}
	}

	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(lLCTTexture), JAPANESE_LCT_WIDTH, JAPANESE_LCT_HEIGHT, iZCor, 0, 1, 0, 1);
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_LCT_SMOKE_CREATOR_X_OFFSET, JAPANESE_LCT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, JAPANESE_LCT_DUAL_MACHINE_GUN_X_LOCATION, JAPANESE_LCT_DUAL_MACHINE_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 200, this);
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_LCT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_LCT_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseLCT::CollisionPoints);
	BasicShipPostConstructCall();
}

//------------------- functions ------------------------------//

void CJapaneseLCT::Update()
{
	ShipBasicUpdate();
	iSmokeCreator->Update();
	iMachineGun->Update();
}

void CJapaneseLCT::Draw()
{
	iMachineGun->Draw();	
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CJapaneseLCT::PossibleCollision(CInterval*& aInterval)
{
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane | EObjectTypesFloor);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

		//only collide with objects of type plane that are alive
		if(lCurrentGameObject->IsAlive() && ((lCurrentGameObject->GetGameObjectType() & EObjectTypesPlane) && static_cast<CAircraft*>(lCurrentGameObject)->IsFlying()) && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
		{
			SDamage lDamage;
			lDamage.Damage = KILL_ANY_AIRPLANE_DAMAGE;
			lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;//should kill any object
			lCurrentGameObject->TakeDamage(lDamage, EDamageTypeShipPlaneCollision);
		}
		//check if we collide with land. Tells us to unload the troop
		else if(iHasTroopsLoaded && lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierLand)
		{
			//collide, need special intersection method that returns the collision point to create the explosion
			TPointIntFloat* lCollisionPoint = iHitBox->IntersectionWithCollisionPointL(lCurrentGameObject->GetHitBox());
			if(lCollisionPoint)
			{
				UnloadTroopsAtLocation(*lCollisionPoint);
				delete lCollisionPoint;
				break;//only one explosion possible per bomb :)
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CJapaneseLCT::UnloadTroopsAtLocation(TPointIntFloat& aLocation)
{
	if(iHasTroopsLoaded)
	{
		CGroundUnitOverlordAI* lJapaneseOverlord = NULL;
		TInt lXLocation = aLocation.iX.GetIntInBaseInt();
		if(iObjectReflected)
			lXLocation += JAPANESE_LCT_X_UNLOAD_SAFETY_MARGIN;
		else
			lXLocation -= JAPANESE_LCT_X_UNLOAD_SAFETY_MARGIN;
		
		switch(iLCTType)
		{
			case EJapaneseLCTTypeTankAndTroops:
			{
				TInt lReturnNumber = CGame::Game->iRandomGenerator->GetRandomNumberTInt(1,101);
				if(lReturnNumber < 50 )
					lJapaneseOverlord = CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(lXLocation,iObjectReflected);
				else
					lJapaneseOverlord = CJapaneseSquad::CreateJapaneseSoldierAATankSquad(lXLocation,iObjectReflected);
				break;
			}
			case EJapaneseLCTTypeTanksOnly:
			{
				lJapaneseOverlord = CJapaneseSquad::CreateJapaneseTankSquad(lXLocation,iObjectReflected);
				break;
			}
			case EJapaneseLCTTypeTroopsOnly:
			{
				lJapaneseOverlord = CJapaneseSquad::CreateJapaneseSoldierSquad(lXLocation,iObjectReflected);
				break;
			}
			default:
				break;
		}
		
		iHasTroopsLoaded = false;
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdShipJapaneseLCTEmpty));
		if(lJapaneseOverlord)
		{
			lJapaneseOverlord->GiveOrder(iOrderForUnloadedTroops);
		}
		if(iObjectReflected)
			TurnLeft();
		else
			TurnRight();
	}
}

TBool CJapaneseLCT::GetIsLoaded()
{
	return iHasTroopsLoaded;
}

void CJapaneseLCT::InitilizeJapaneseLCT()
{
	CJapaneseLCT::LoadJapaneseLCTTextures();
	CJapaneseLCT::CreateJapaneseLCTCollisionPointsL();
}

void CJapaneseLCT::RemoveJapaneseLCT()
{
	CJapaneseLCT::UnloadJapaneseLCTTextures();
	CJapaneseLCT::RemoveJapaneseLCTCollisionPoints();
}

void CJapaneseLCT::LoadJapaneseLCTTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseLCTEmpty);
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseLCTLoadedWithSoldiers);
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseLCTLoadedWithSoldiersAndTanks);
	CGame::Game->iImageStore->LoadImage(EImageIdShipJapaneseLCTLoadedWithTanks);
}

void CJapaneseLCT::UnloadJapaneseLCTTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseLCTEmpty);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseLCTLoadedWithSoldiers);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseLCTLoadedWithSoldiersAndTanks);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipJapaneseLCTLoadedWithTanks);
}

void CJapaneseLCT::CreateJapaneseLCTCollisionPointsL()
{
	CJapaneseLCT::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_LCT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseLCT::CollisionPoints[0].XOffset = 30;
	CJapaneseLCT::CollisionPoints[0].YOffset = 0;
	CJapaneseLCT::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CJapaneseLCT::CollisionPoints[1].XOffset = 0;
	CJapaneseLCT::CollisionPoints[1].YOffset = 48;
	CJapaneseLCT::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseLCT::CollisionPoints[2].XOffset = 160;
	CJapaneseLCT::CollisionPoints[2].YOffset = 44;
	CJapaneseLCT::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseLCT::CollisionPoints[3].XOffset = 180;
	CJapaneseLCT::CollisionPoints[3].YOffset = 86;
	CJapaneseLCT::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseLCT::CollisionPoints[4].XOffset = 220;
	CJapaneseLCT::CollisionPoints[4].YOffset = 86;
	CJapaneseLCT::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseLCT::CollisionPoints[5].XOffset = 250;
	CJapaneseLCT::CollisionPoints[5].YOffset = 42;
	CJapaneseLCT::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CJapaneseLCT::CollisionPoints[6].XOffset = 215;
	CJapaneseLCT::CollisionPoints[6].YOffset = 0;
	CJapaneseLCT::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CJapaneseLCT::CollisionPoints[7].XOffset = 30;
	CJapaneseLCT::CollisionPoints[7].YOffset = 6;
	CJapaneseLCT::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
}

void CJapaneseLCT::RemoveJapaneseLCTCollisionPoints()
{
	if(CJapaneseLCT::CollisionPoints)
	{
		delete[] CJapaneseLCT::CollisionPoints;
		CJapaneseLCT::CollisionPoints = NULL;
	}
}

void CJapaneseLCT::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ShipSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iLCTType);
	aOutputStream.WriteInt32(iHasTroopsLoaded);
	aOutputStream.WriteInt32(iOrderForUnloadedTroops.ShouldProtectArea);
	aOutputStream.WriteInt32(iOrderForUnloadedTroops.DefaultXPosition);
	aOutputStream.WriteIntFloat(iOrderForUnloadedTroops.ProtectionArea.LeftXPosition);
	aOutputStream.WriteIntFloat(iOrderForUnloadedTroops.ProtectionArea.RightXPosition);
	aOutputStream.WriteInt32(iOrderForUnloadedTroops.MoveToLocation);
	aOutputStream.WriteInt32(iOrderForUnloadedTroops.XLocationDestination);
	iSmokeCreator->SaveOnDisk(aOutputStream);
	iMachineGun->SaveOnDisk(aOutputStream);
	
}

void CJapaneseLCT::LoadFromDisk(CFileReadStream &aReadStream)
{
	ShipLoadContentFromDisk(aReadStream);
	iLCTType = static_cast<TJapaneseLCTType> (aReadStream.ReadInt32());
	iHasTroopsLoaded = aReadStream.ReadInt32();
	iOrderForUnloadedTroops.ShouldProtectArea = aReadStream.ReadInt32();
	iOrderForUnloadedTroops.DefaultXPosition = aReadStream.ReadInt32();
	iOrderForUnloadedTroops.ProtectionArea.LeftXPosition = aReadStream.ReadIntFloat();
	iOrderForUnloadedTroops.ProtectionArea.RightXPosition = aReadStream.ReadIntFloat();
	iOrderForUnloadedTroops.MoveToLocation = aReadStream.ReadInt32();
	iOrderForUnloadedTroops.XLocationDestination = aReadStream.ReadInt32();
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iSmokeCreator->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iMachineGun->LoadFromDisk(aReadStream);
}

