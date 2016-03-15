/*
 ============================================================================
 Name		: Airport.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CAirport implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CAirport::CAirport(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CBuilding(BUILDINGS_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
{
	iTargetPriority = (GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP);
	iTarget = NULL;
	iAircraft = NULL;
	iSpawnPlanes = true;
	iFramesLeftUntilSpawnPlane = 0;
}

CAirport::~CAirport()
{
	//flagpole is part of gameObjects array
}

CAirport* CAirport::CreateLoadAirport()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AIRPORT_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = AIRPORT_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAirport;

	CAirport* self = new CAirport(TPoint(0, 0), lAttributes);
	self->ConstructWithoutObjectCreationL();
	return self;
}

CAirport* CAirport::New(TPoint aLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AIRPORT_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = AIRPORT_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAirport;

	CAirport* self = new CAirport(aLocation, lAttributes);
	self->ConstructL();
	return self;
}


CAirport* CAirport::New(TInt aXLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AIRPORT_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesPlaneSpawnLocation;
	lAttributes.Reflected = false;
	lAttributes.Armor = AIRPORT_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAirport;

	CAirport* self = new CAirport(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

void CAirport::ConstructWithoutObjectCreationL()
{
	//values are going to be reset during loading anyway
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportMainBuilding), AIRPORT_WIDTH, AIRPORT_HEIGHT, iZCor, 0, 1, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(AIRPORT_WIDTH, AIRPORT_HEIGHT)); //however, will not be used
}

void CAirport::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportMainBuilding), AIRPORT_WIDTH, AIRPORT_HEIGHT, iZCor, 0, 1, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(AIRPORT_WIDTH, AIRPORT_HEIGHT)); //however, will not be used,since clouds will

	//create LandingSurface
	TPoint lLandingSurfacePosition = GetCurrentPositionNormilized();
	lLandingSurfacePosition.iY += AIRPORT_LANDING_SURFACE_Y_OFFSET;
	lLandingSurfacePosition.iX += AIRPORT_LANDING_SURFACE_X_OFFSET;

	CTextureObject* aTextureOBject = CTextureObject::New(lLandingSurfacePosition, &CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportRunway), AIRPORT_LANDING_SURFACE_WIDTH, AIRPORT_LANDING_SURFACE_HEIGHT, AIRPORT_LANDING_SURFACE_Z_VALUE, 0, 1, 0, 1);
	iLandingSurface = CLandingSurface::New(lLandingSurfacePosition, aTextureOBject, AIRPORT_LANDING_SURFACE_Z_VALUE, iConflictSide, TIntFloat::Convert(AIRPORT_LANDING_SURFACE_WIDTH), TIntFloat::Convert(AIRPORT_LANDING_SURFACE_HEIGHT));
	//add surface to currentMap
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLandingSurface);

	TPoint lHangarPosition = GetCurrentPositionNormilized();
	lHangarPosition.iX += AIRPORT_HANGAR_1_OFFSET_X;
	iHangar1 = CHangar::New(lHangarPosition);
	iHangar1->AssignConflictSide(iConflictSide);
	//add hangar to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iHangar1);

	lHangarPosition = GetCurrentPositionNormilized();
	lHangarPosition.iX += AIRPORT_HANGAR_2_OFFSET_X;
	iHangar2 = CHangar::New(lHangarPosition);
	iHangar2->AssignConflictSide(iConflictSide);
	//add hangar to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iHangar2);

	//create the Flagpole that shows what side the airport is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += AIRPORT_FLAG_POLE_OFFSET_X;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	//create some weapons for the airport, NOTE: need to disable that Guns collide with Planes otherwise it is not possible to land
	TPoint lGunPosition = GetCurrentPositionNormilized();
	lGunPosition.iX += AIRPORT_TYPE_88_AA_GUN_1_X_POSITION_OFFSET;
	CGameObject* lCurrentGun = CJapaneseAA75mmType88::New(lGunPosition, false);
	lCurrentGun->AssignConflictSide(iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(lCurrentGun);

	lGunPosition = GetCurrentPositionNormilized();
	lGunPosition.iX += AIRPORT_LIGHT_MACHINE_GUN_1_X_POSITION_OFFSET;
	lCurrentGun = CJapaneseAA20mmMachineCannon::New(lGunPosition, false);
	lCurrentGun->AssignConflictSide(iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(lCurrentGun);

	lGunPosition = GetCurrentPositionNormilized();
	lGunPosition.iX += AIRPORT_LIGHT_MACHINE_GUN_2_X_POSITION_OFFSET;
	lCurrentGun = CJapaneseAA20mmMachineCannon::New(lGunPosition, false);
	lCurrentGun->AssignConflictSide(iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(lCurrentGun);

	lGunPosition = GetCurrentPositionNormilized();
	lGunPosition.iX += AIRPORT_LIGHT_MACHINE_GUN_3_X_POSITION_OFFSET;
	lCurrentGun = CJapaneseAA20mmMachineCannon::New(lGunPosition, false);
	lCurrentGun->AssignConflictSide(iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(lCurrentGun);

	lGunPosition = GetCurrentPositionNormilized();
	lGunPosition.iX += AIRPORT_LIGHT_MACHINE_GUN_4_X_POSITION_OFFSET;
	lCurrentGun = CJapaneseAA20mmMachineCannon::New(lGunPosition, false);
	lCurrentGun->AssignConflictSide(iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(lCurrentGun);

	lGunPosition = GetCurrentPositionNormilized();
	lGunPosition.iX += AIRPORT_TYPE_88_AA_GUN_2_X_POSITION_OFFSET;
	lCurrentGun = CJapaneseAA75mmType88::New(lGunPosition, false);
	lCurrentGun->AssignConflictSide(iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(lCurrentGun);
}

//------------------ functions ---------------------------//


void CAirport::Update()
{
	if(iAlive && iSpawnPlanes)
	{
		if(iTarget && !iTarget->IsAlive())//make sure target has not been destroyed yet
			iTarget = NULL;
		
		//check if a plane died
		if(iAircraft && !iAircraft->IsAlive())
			iAircraft = NULL;
		
		if(iFramesLeftUntilSpawnPlane == 0)
		{
			//check if we can spawn a new plane
			if(iHangar1->IsPlaneStillLeft())
			{
				iHangar1->RemovePlane();
				iFramesLeftUntilSpawnPlane = AIRPORT_DEFAULT_SECS_UNTIL_SPAWN_AIRCRAFT;
			}
			else if(iHangar2->IsPlaneStillLeft())
			{
				iHangar2->RemovePlane();
				iFramesLeftUntilSpawnPlane = AIRPORT_DEFAULT_SECS_UNTIL_SPAWN_AIRCRAFT;
			}
			else
			{
				//no planes left, now it takes a lot longer to spawn another plane
				iFramesLeftUntilSpawnPlane = AIRPORT_DEFAULT_SECS_UNTIL_SPAWN_AIRCRAFT * CARRIER_DEFAULT_EXTRA_SPAWN_TIME_MULTIPLIER_IF_NO_PLANES_LEFT;
			}
				
			if(iHangar1->IsAlive() || iHangar2->IsAlive())
			{
				iAircraft = CreateNewAircraft();
				if(!iAircraft->GetFighterAI())
					iAircraft->SetAIControlled();
				iAircraft->GetFighterAI()->SetTargetAttackType(iTargetPriority);
				iAircraft->GetFighterAI()->SetTargetToAttack(iTarget);//doesn't matter if NULL
			}
		}
		else if(!iAircraft) //at least one slot is unused
		{
			iFramesLeftUntilSpawnPlane--;
		}
	}
}

void CAirport::SetTargetPriority(TUint aTargetPriority)
{
	iTargetPriority = aTargetPriority;
}

void CAirport::SetTarget(CGameObject* aTarget)
{
	iTarget = aTarget;
}

CAircraft* CAirport::CreateNewAircraft()
{
	CAircraft* lAircraft;
	if(iConflictSide == EConflictSideAmerican)
		lAircraft = CDauntless::New(false, TIntFloat::Convert(0), false, TPoint(0, 0));//position does matter since we reposition it anyway
	else
		lAircraft = CVal::New(false, TIntFloat::Convert(0), false, TPoint(0, 0));//position does matter since we reposition it anyway
	TPoint lCurrentPosition = GetCurrentPositionNormilized();
	lCurrentPosition.iX += AIRPORT_AIRPLANE_POSITION_X_OFFSET; //need to adjust it to be on the landing surface to the landing surface
	lAircraft->SetPosition(lCurrentPosition);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager()->AddMoveableGameObject(lAircraft);
	return lAircraft;//all done
}

void CAirport::SetSpawnPlanes(TBool aValue) //enable or disable the spawning of planes for this airport
{
	iSpawnPlanes = aValue;
}

void CAirport::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			//set image to destroyed
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportMainBuildingBroken2));
			iHealth = 0;
			Die(aType);
		}else if(iHealth <= AIRPORT_HEALTH_DAMAGED)
		{
			//changes image to damaged image
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportMainBuildingBroken1));
		}
	}
}

void CAirport::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once
	
	iLandingSurface->SetLandingSurfaceDestroyed();
	BuildingBasicDieRoutine();
	GameObjectMoveIntoBackground();
}

void CAirport::PossibleCollision(CInterval*& aInterval)
{
	CGameObject* lNewConflictSideGameObject = CheckIfObjectGotCaptured(aInterval);

	//check if the GameObject changes sides
	if(lNewConflictSideGameObject)
	{
		iConflictSide = lNewConflictSideGameObject->GetConflictSide();
		iHangar1->AssignConflictSide(lNewConflictSideGameObject->GetConflictSide());
		iHangar1->SetBuildingGotCaptured(iGotCaptured);
		iHangar2->AssignConflictSide(lNewConflictSideGameObject->GetConflictSide());
		iHangar2->SetBuildingGotCaptured(iGotCaptured);
		iLandingSurface->AssignConflictSide(lNewConflictSideGameObject->GetConflictSide());
		iFlagPole->ChangeFlagConflictSide(lNewConflictSideGameObject->GetConflictSide());
	}
}

void CAirport::InitilizeAirport()
{
	CAirport::LoadAirportTextures();
}

void CAirport::RemoveAirport()
{
	CAirport::UnloadAirportTextures();
}

void CAirport::LoadAirportTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportMainBuilding);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportMainBuildingBroken1);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportMainBuildingBroken2);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportRunway);
}

void CAirport::UnloadAirportTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportMainBuilding);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportMainBuildingBroken1);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportMainBuildingBroken2);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportRunway);
}

void CAirport::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BuildingSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iSpawnPlanes);
	aOutputStream.WriteInt32(iFramesLeftUntilSpawnPlane);
	aOutputStream.WriteInt32(iTargetPriority);
	//save pointers
	aOutputStream.WriteUint32((TUint32)iLandingSurface);
	aOutputStream.WriteUint32((TUint32)iHangar1);
	aOutputStream.WriteUint32((TUint32)iHangar2);
	aOutputStream.WriteUint32((TUint32)iFlagPole);
	aOutputStream.WriteUint32((TUint32)iAircraft);
	aOutputStream.WriteUint32((TUint32)iTarget);
}

void CAirport::LoadFromDisk(CFileReadStream &aReadStream)
{
	BuildingLoadContentFromDisk(aReadStream);
	iSpawnPlanes = aReadStream.ReadInt32();
	iFramesLeftUntilSpawnPlane = aReadStream.ReadInt32();
	iTargetPriority = aReadStream.ReadInt32();
	//reset pointers
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iLandingSurface));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iHangar1));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iHangar2));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iFlagPole));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iAircraft));
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(aReadStream.ReadUint32(), (TUint32)(&iTarget));

}
