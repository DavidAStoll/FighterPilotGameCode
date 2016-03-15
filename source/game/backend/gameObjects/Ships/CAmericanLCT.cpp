/*
 ============================================================================
 Name		: AmericanLCT.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanLCT implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

SBoundingCollisionPoint* CAmericanLCT::CollisionPoints;

CAmericanLCT::CAmericanLCT(TInt aXLocation, SGameObjectAttributes& aAttributes, TAmericanLCTType aType, SGroundUnitOrder& aOrder) :
	CShip(AMERICAN_LCT_Z, aXLocation, AMERICAN_LCT_KEEL_DEPTH,TIntFloat::Convert(AMERICAN_LCT_DEFAULT_SPEED), aAttributes)
{
	iSpeed = TIntFloat::Convert(AMERICAN_LCT_DEFAULT_SPEED);
	iLCTType = aType;
	iHasTroopsLoaded = true;
	iOrderForUnloadedTroops = aOrder;
}

CAmericanLCT::~CAmericanLCT()
{
	if(iSmokeCreator)
	{
		delete iSmokeCreator;
		iSmokeCreator = NULL;
	}
}

CAmericanLCT* CAmericanLCT::New(TInt aXLocation, TBool aReflected, TAmericanLCTType aType, SGroundUnitOrder& aOrder)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = CGameObjectDefaultValues::AmericanLCTHealth;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesShip;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_LCT_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanLCT;

	CAmericanLCT* self = new CAmericanLCT(aXLocation, lAttributes, aType, aOrder);
	self->ConstructL();
	return self;
}

void CAmericanLCT::ConstructL()
{
	TImageId lLCTTexture;
	switch(iLCTType)
	{
		case EAmericanLCTTypeEmpty:
		{
			lLCTTexture = EImageIdShipAmericanLCTEmpty;
			break;
		}
		case EAmericanLCTTypeTankAndTroops:
		{
			lLCTTexture = EImageIdShipAmericanLCTLoadedWithSoldiersAndTanks;
			break;
		}
		case EAmericanLCTTypeTanksOnly:
		{
			lLCTTexture = EImageIdShipAmericanLCTLoadedWithTanks;
			break;
		}
		case EAmericanLCTTypeTroopsOnly:
		{
			lLCTTexture = EImageIdShipAmericanLCTLoadedWithSoldiers;
			break;
		}
	}

	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(lLCTTexture), AMERICAN_LCT_WIDTH, AMERICAN_LCT_HEIGHT, iZCor, 0, 1, 0, 1);
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_LCT_SMOKE_CREATOR_X_OFFSET, AMERICAN_LCT_SMOKE_CREATOR_Y_OFFSET, false, SHIP_SMOKE_DEFAULT_SIZE, SHIP_SMOKE_DEFAULT_FRAMES);
	iMachineGun = CShipDualMachineGun::New(&iCoordinates, &iAngle,190, AMERICAN_LCT_DUAL_MACHINE_GUN_X_LOCATION, AMERICAN_LCT_DUAL_MACHINE_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 200, this);
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_LCT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_LCT_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanLCT::CollisionPoints);
	BasicShipPostConstructCall();
}

//------------------- functions ------------------------------//

void CAmericanLCT::Update()
{
	ShipBasicUpdate();
	iSmokeCreator->Update();
	iMachineGun->Update();
}

void CAmericanLCT::Draw()
{
	iMachineGun->Draw();	
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CAmericanLCT::PossibleCollision(CInterval*& aInterval)
{
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane | EObjectTypesFloor);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

		//only collide with objects of type plane that are alive
		if(lCurrentGameObject->IsAlive() && ((lCurrentGameObject->GetGameObjectType() & (EObjectTypesPlane)) && static_cast<CAircraft*>(lCurrentGameObject)->IsFlying()) && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
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

void CAmericanLCT::UnloadTroopsAtLocation(TPointIntFloat& aLocation)
{
	if(iHasTroopsLoaded)
	{
		CGroundUnitOverlordAI* lAmericanOverlord = NULL;
		TInt lXLocation = aLocation.iX.GetIntInBaseInt();
		if(iObjectReflected)
			lXLocation += AMERICAN_LCT_X_UNLOAD_SAFETY_MARGIN;
		else
			lXLocation -= AMERICAN_LCT_X_UNLOAD_SAFETY_MARGIN;
		
		switch(iLCTType)
		{
			case EAmericanLCTTypeTankAndTroops:
			{
				TInt lReturnNumber = CGame::Game->iRandomGenerator->GetRandomNumberTInt(1,101);
				if(lReturnNumber < 50 )
					lAmericanOverlord = CAmericanSquad::CreateAmericanSoldierAndTankSquad(lXLocation,iObjectReflected);
				else
					lAmericanOverlord = CAmericanSquad::CreateAmericanSoldierAATankSquad(lXLocation,iObjectReflected);
				break;
			}
			case EAmericanLCTTypeTanksOnly:
			{
				lAmericanOverlord = CAmericanSquad::CreateAmericanTankSquad(lXLocation,iObjectReflected);
				break;
			}
			case EAmericanLCTTypeTroopsOnly:
			{
				lAmericanOverlord = CAmericanSquad::CreateAmericanSoldierSquad(lXLocation,iObjectReflected);
				break;
			}
			default:
				break;
		}
		
		iHasTroopsLoaded = false;
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdShipAmericanLCTEmpty));
		if(lAmericanOverlord)
		{
			lAmericanOverlord->GiveOrder(iOrderForUnloadedTroops);
		}
		if(iObjectReflected)
			TurnLeft();
		else
			TurnRight();
	}
}

TBool CAmericanLCT::GetIsLoaded()
{
	return iHasTroopsLoaded;
}

void CAmericanLCT::InitilizeAmericanLCT()
{
	CAmericanLCT::LoadAmericanLCTTextures();
	CAmericanLCT::CreateAmericanLCTCollisionPointsL();
}

void CAmericanLCT::RemoveAmericanLCT()
{
	CAmericanLCT::UnloadAmericanLCTTextures();
	CAmericanLCT::RemoveAmericanLCTCollisionPoints();
}

void CAmericanLCT::LoadAmericanLCTTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanLCTEmpty);
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanLCTLoadedWithSoldiers);
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanLCTLoadedWithSoldiersAndTanks);
	CGame::Game->iImageStore->LoadImage(EImageIdShipAmericanLCTLoadedWithTanks);
}

void CAmericanLCT::UnloadAmericanLCTTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanLCTEmpty);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanLCTLoadedWithSoldiers);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanLCTLoadedWithSoldiersAndTanks);
	CGame::Game->iImageStore->UnloadImage(EImageIdShipAmericanLCTLoadedWithTanks);
}

void CAmericanLCT::CreateAmericanLCTCollisionPointsL()
{
	CAmericanLCT::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_LCT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanLCT::CollisionPoints[0].XOffset = 30;
	CAmericanLCT::CollisionPoints[0].YOffset = 0;
	CAmericanLCT::CollisionPoints[0].iMovementType = EPointMovementTypeNone;//don't really need a projected hitbox
	CAmericanLCT::CollisionPoints[1].XOffset = 0;
	CAmericanLCT::CollisionPoints[1].YOffset = 48;
	CAmericanLCT::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanLCT::CollisionPoints[2].XOffset = 160;
	CAmericanLCT::CollisionPoints[2].YOffset = 44;
	CAmericanLCT::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanLCT::CollisionPoints[3].XOffset = 180;
	CAmericanLCT::CollisionPoints[3].YOffset = 86;
	CAmericanLCT::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanLCT::CollisionPoints[4].XOffset = 220;
	CAmericanLCT::CollisionPoints[4].YOffset = 86;
	CAmericanLCT::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanLCT::CollisionPoints[5].XOffset = 250;
	CAmericanLCT::CollisionPoints[5].YOffset = 42;
	CAmericanLCT::CollisionPoints[5].iMovementType = EPointMovementTypeNone;
	CAmericanLCT::CollisionPoints[6].XOffset = 215;
	CAmericanLCT::CollisionPoints[6].YOffset = 0;
	CAmericanLCT::CollisionPoints[6].iMovementType = EPointMovementTypeNone;
	CAmericanLCT::CollisionPoints[7].XOffset = 30;
	CAmericanLCT::CollisionPoints[7].YOffset = 6;
	CAmericanLCT::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
}

void CAmericanLCT::RemoveAmericanLCTCollisionPoints()
{
	if(CAmericanLCT::CollisionPoints)
	{
		delete[] CAmericanLCT::CollisionPoints;
		CAmericanLCT::CollisionPoints = NULL;
	}
}

void CAmericanLCT::SaveOnDisk(CFileWriteStream &aOutputStream)
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

void CAmericanLCT::LoadFromDisk(CFileReadStream &aReadStream)
{
	ShipLoadContentFromDisk(aReadStream);
	iLCTType = static_cast<TAmericanLCTType> (aReadStream.ReadInt32());
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
