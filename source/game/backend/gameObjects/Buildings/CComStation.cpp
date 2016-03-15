/*
 ============================================================================
 Name		: ComStation.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CComStation implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CComStation::CComStation(TPoint aLocation, SGameObjectAttributes& aAttributes, TInt aSecOffsetFromCallingBomber, TInt aHeightOffset) :
	CBuilding(BUILDINGS_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
{
	iCurrentFrame = 0;
	iExtraFrameOffset = aSecOffsetFromCallingBomber * GFRAMES_PER_SECOND_BACKEND;
	iExtraHeightOffset = aHeightOffset;
}

CComStation::~CComStation()
{
	//flagpole is part of gameObjects array
}

CComStation* CComStation::CreateLoadComStation()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = COMSTATION_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = COMSTATION_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierComStation;

	CComStation* self = new CComStation(TPoint(0, 0), lAttributes, 0, 0);
	self->ConstructWithoutObjectCreationL();
	return self;
}

CComStation* CComStation::New(TPoint aLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = COMSTATION_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = COMSTATION_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierComStation;

	CComStation* self = new CComStation(aLocation, lAttributes, 0, 0);
	self->ConstructL();
	return self;
}

CComStation* CComStation::New(TInt aXLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = COMSTATION_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = COMSTATION_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierComStation;

	CComStation* self = new CComStation(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes, 0, 0);
	self->ConstructL();
	return self;
}
CComStation* CComStation::New(TInt aXLocation, TConflictSide aConflictSide, TInt aSecOffsetFromCallingBomber, TInt aHeightOffset)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = COMSTATION_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = COMSTATION_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierComStation;

	CComStation* self = new CComStation(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes, aSecOffsetFromCallingBomber, aHeightOffset);
	self->ConstructL();
	return self;
}
CComStation* CComStation::New(TPoint aLocation, TConflictSide aConflictSide, TInt aSecOffsetFromCallingBomber, TInt aHeightOffset)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = COMSTATION_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = COMSTATION_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierComStation;

	CComStation* self = new CComStation(aLocation, lAttributes, aSecOffsetFromCallingBomber, aHeightOffset);
	self->ConstructL();
	return self;
}

void CComStation::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingComStation), COMSTATION_WIDTH, COMSTATION_HEIGHT, iZCor, 0, 1, 0, 0.625);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(COMSTATION_HITBOX_WIDTH, COMSTATION_HITBOX_HEIGHT)); //however, will not be used,since clouds will
	//create the Flagpole that shows what side the HQ is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += COMSTATION_FLAG_POLE_OFFSET_X;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);
	//sound object
	iSoundObject = CSoundObject::New(ESoundFileId_Radio_Signals, GetHitBox());
	iSoundObject->PlayRepeatHitBox();
}

void CComStation::ConstructWithoutObjectCreationL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingComStation), COMSTATION_WIDTH, COMSTATION_HEIGHT, iZCor, 0, 1, 0, 0.625);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(COMSTATION_HITBOX_WIDTH, COMSTATION_HITBOX_HEIGHT)); //however, will not be used,since clouds will
	//sound object
	iSoundObject = CSoundObject::New(ESoundFileId_Radio_Signals, GetHitBox());
	iSoundObject->PlayRepeatHitBox();
}

//------------------ functions ---------------------------//


void CComStation::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			iSoundObject->StopSoundChannel();
			//set image to destroyed
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingComStationBroken2));
			iHealth = 0;
			Die(aType);
		}else if(iHealth <= COMSTATION_HEALTH_DAMAGED)
		{
			//changes image to damaged image
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingComStationBroken1));
		}
	}
}

void CComStation::InitilizeComStation()
{
	CComStation::LoadComStationTextures();
}

void CComStation::RemoveComStation()
{
	CComStation::UnloadComStationTextures();
}

void CComStation::Update()
{
	if(!iAlive)
		return;

	iCurrentFrame++;
	if(iCurrentFrame == (COMSTATION_FRAMES_UNTIL_NEW_BOMBER_GETS_CALLED + iExtraFrameOffset))
	{
		//create a new bomber
		iCurrentFrame = iExtraFrameOffset; // otherwise spawns will not be in sync, and it will always longer between spawns
		if(iConflictSide == EConflictSideJapanese)
		{
			TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(BETTY_WIDTH);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBetty::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocation, COMSTATION_BOMBER_SPAWNING_HEIGHT + iExtraHeightOffset)));
		}else
		{
			TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(MITCHELL_WIDTH);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CMitchell::New(false, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocation, COMSTATION_BOMBER_SPAWNING_HEIGHT + iExtraHeightOffset)));
		}
	}
}

void CComStation::PossibleCollision(CInterval*& aInterval)
{
	//go through all objects and check if care to collide with them
	if(iAlive)
	{
		CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane);

		for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

			//only collide with alive objects
			if(lCurrentGameObject->IsAlive())//don't compare to anything if already dead
			{
				//collide
				if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
				{ //make sure plane is dead
					SDamage lDamage;
					lDamage.Damage = KILL_ANY_AIRPLANE_DAMAGE;
					lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;//should kill any object
					lCurrentGameObject->TakeDamage(lDamage, EDamageTypeTankPlaneCollision);
				}
			}
		}

		//clean up
		delete lGameObjects;
	}

	//check if this object got captured
	CGameObject* lCaptureObject = CheckIfObjectGotCaptured(aInterval);
	if(lCaptureObject)
	{
		iFlagPole->ChangeFlagConflictSide(lCaptureObject->GetConflictSide());
		iConflictSide = lCaptureObject->GetConflictSide();
	}
}

void CComStation::LoadComStationTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingComStation);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingComStationBroken1);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingComStationBroken2);
}

void CComStation::UnloadComStationTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingComStation);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingComStationBroken1);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingComStationBroken2);
}

void CComStation::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BuildingSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iExtraFrameOffset);
	aOutputStream.WriteInt32(iExtraHeightOffset);
	TUint32 lFlagPolePointerMemoryAddress = (TUint32)iFlagPole;
	aOutputStream.WriteUint32(lFlagPolePointerMemoryAddress);
}

void CComStation::LoadFromDisk(CFileReadStream &aReadStream)
{
	BuildingLoadContentFromDisk(aReadStream);
	iCurrentFrame = aReadStream.ReadInt32();
	iExtraFrameOffset = aReadStream.ReadInt32();
	iExtraHeightOffset = aReadStream.ReadInt32();
	TUint32 lFlagPolePointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lFlagPolePointerMemoryAddress, (TUint32)(&iFlagPole));

}

