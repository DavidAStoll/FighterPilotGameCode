/*
 ============================================================================
 Name		: Hq.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHq implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CHq::CHq(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CBuilding(BUILDINGS_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
{
}

CHq::~CHq()
{
	//flagpole is part of gameObjects array
}

CHq* CHq::CreateLoadHq()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = HQ_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = HQ_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierHq;

	CHq* self = new CHq(TPoint(0, 0), lAttributes);
	self->ConstructWithoutObjectCreationL();
	return self;
}

CHq* CHq::New(TPoint aLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = HQ_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = HQ_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierHq;

	CHq* self = new CHq(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CHq* CHq::New(TInt aXLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = HQ_HEALTH;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = HQ_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierHq;

	CHq* self = new CHq(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

void CHq::ConstructWithoutObjectCreationL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingHQ), HQ_WIDTH, HQ_HEIGHT, iZCor, 0, 0.78125, 0, 1);
	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(HQ_HITBOX_WIDTH, HQ_HITBOX_HEIGHT)); //however, will not be used,since clouds will
}

void CHq::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingHQ), HQ_WIDTH, HQ_HEIGHT, iZCor, 0, 0.78125, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(HQ_HITBOX_WIDTH, HQ_HITBOX_HEIGHT)); //however, will not be used,since clouds will

	//create the Flagpole that shows what side the HQ is on
	TPoint lFlagPolePosition = GetCurrentPositionNormilized();
	lFlagPolePosition.iX += HQ_FLAG_POLE_OFFSET_X;
	iFlagPole = CFlagPole::New(lFlagPolePosition, iConflictSide);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iFlagPole);

	//create Guns attached to this HQ

	//----------add Light Machine Guns-----------------

	TPoint lMachineGunPosition = GetCurrentPositionNormilized();
	lMachineGunPosition.iX += HQ_LIGHT_MACHINE_MACHINE_GUN_RIGHT_1_OFFSET;
	iRightMachineGun1 = CJapaneseAA20mmMachineCannon::New(lMachineGunPosition);
	iRightMachineGun1->AssignConflictSide(iConflictSide);
	//add MachineGun to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iRightMachineGun1);

	lMachineGunPosition = GetCurrentPositionNormilized();
	lMachineGunPosition.iX += HQ_LIGHT_MACHINE_MACHINE_GUN_RIGHT_2_OFFSET;
	iRightMachineGun2 = CJapaneseAA20mmMachineCannon::New(lMachineGunPosition);
	iRightMachineGun2->AssignConflictSide(iConflictSide);
	//add MachineGun to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iRightMachineGun2);

	lMachineGunPosition = GetCurrentPositionNormilized();
	lMachineGunPosition.iX += HQ_LIGHT_MACHINE_MACHINE_GUN_LEFT_1_OFFSET;
	iLeftMachineGun1 = CJapaneseAA20mmMachineCannon::New(lMachineGunPosition);
	iLeftMachineGun1->AssignConflictSide(iConflictSide);
	//add MachineGun to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLeftMachineGun1);

	lMachineGunPosition = GetCurrentPositionNormilized();
	lMachineGunPosition.iX += HQ_LIGHT_MACHINE_MACHINE_GUN_LEFT_2_OFFSET;
	iLeftMachineGun2 = CJapaneseAA20mmMachineCannon::New(lMachineGunPosition);
	iLeftMachineGun2->AssignConflictSide(iConflictSide);
	//add MachineGun to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLeftMachineGun2);

	//-----------------add Type 88 Guns-------------------

	TPoint lType88GunPosition = GetCurrentPositionNormilized();
	lType88GunPosition.iX += HQ_TYPE_88_AA_GUN_RIGHT_1_OFFSET;
	iRightType88Gun1 = CJapaneseAA75mmType88::New(lType88GunPosition);
	iRightType88Gun1->AssignConflictSide(iConflictSide);
	//add Type 88 to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iRightType88Gun1);

	lType88GunPosition = GetCurrentPositionNormilized();
	lType88GunPosition.iX += HQ_TYPE_88_AA_GUN_RIGHT_2_OFFSET;
	iRightType88Gun2 = CJapaneseAA75mmType88::New(lType88GunPosition);
	iRightType88Gun2->AssignConflictSide(iConflictSide);
	//add Type 88 to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iRightType88Gun2);

	lType88GunPosition = GetCurrentPositionNormilized();
	lType88GunPosition.iX += HQ_TYPE_88_AA_GUN_LEFT_1_OFFSET;
	iLeftType88Gun1 = CJapaneseAA75mmType88::New(lType88GunPosition);
	iLeftType88Gun1->AssignConflictSide(iConflictSide);
	//add Type 88 to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLeftType88Gun1);

	lType88GunPosition = GetCurrentPositionNormilized();
	lType88GunPosition.iX += HQ_TYPE_88_AA_GUN_LEFT_2_OFFSET;
	iLeftType88Gun2 = CJapaneseAA75mmType88::New(lType88GunPosition);
	iLeftType88Gun2->AssignConflictSide(iConflictSide);
	//add Type 88 to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLeftType88Gun2);

	//-------------add Concrete Guns to map------------------

	//add Concrete Gun
	TPoint lConceteGunPosition = GetCurrentPositionNormilized();
	lConceteGunPosition.iX += HQ_CONCRETE_GUN_RIGHT_1_OFFSET;
	iRightConcreteGun1 = CJapaneseAAConcrete::New(lConceteGunPosition);
	iRightConcreteGun1->AssignConflictSide(iConflictSide);
	//add Concrete Gun to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iRightConcreteGun1);

	lConceteGunPosition = GetCurrentPositionNormilized();
	lConceteGunPosition.iX += HQ_CONCRETE_GUN_LEFT_1_OFFSET;
	iLeftConcreteGun1 = CJapaneseAAConcrete::New(lConceteGunPosition);
	iLeftConcreteGun1->AssignConflictSide(iConflictSide);
	//add Concrete Gun to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iLeftConcreteGun1);
}

//------------------ functions ---------------------------//


void CHq::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			//set image to destroyed
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingHQBroken2));
			iHealth = 0;
			Die(aType);
		}else if(iHealth <= HQ_HEALTH_DAMAGED)
		{
			//changes image to damaged image
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingHQBroken1));
		}
	}
}

void CHq::InitilizeHq()
{
	CHq::LoadHqTextures();
}

void CHq::RemoveHq()
{
	CHq::UnloadHqTextures();
}

void CHq::PossibleCollision(CInterval*& aInterval)
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
					lCurrentGameObject->TakeDamage(lDamage, EDamageTypeBuildingPlaneCollision);
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
		iConflictSide = lCaptureObject->GetConflictSide();

		iFlagPole->ChangeFlagConflictSide(lCaptureObject->GetConflictSide());
		iRightMachineGun1->AssignConflictSide(lCaptureObject->GetConflictSide());
		iRightMachineGun2->AssignConflictSide(lCaptureObject->GetConflictSide());
		iLeftMachineGun1->AssignConflictSide(lCaptureObject->GetConflictSide());
		iLeftMachineGun2->AssignConflictSide(lCaptureObject->GetConflictSide());

		iRightType88Gun1->AssignConflictSide(lCaptureObject->GetConflictSide());
		iRightType88Gun2->AssignConflictSide(lCaptureObject->GetConflictSide());
		iLeftType88Gun1->AssignConflictSide(lCaptureObject->GetConflictSide());
		iLeftType88Gun2->AssignConflictSide(lCaptureObject->GetConflictSide());

		iRightConcreteGun1->AssignConflictSide(lCaptureObject->GetConflictSide());
		iLeftConcreteGun1->AssignConflictSide(lCaptureObject->GetConflictSide());
	}
}

void CHq::LoadHqTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingHQ);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingHQBroken1);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingHQBroken2);
}

void CHq::UnloadHqTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingHQ);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingHQBroken1);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingHQBroken2);
}

void CHq::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BuildingSaveContentToDisk(aOutputStream);
	TUint32 lFlagPolePointerMemoryAddress = (TUint32)iFlagPole;
	aOutputStream.WriteUint32(lFlagPolePointerMemoryAddress);

	TUint32 lMachineGun = (TUint32)iRightMachineGun1;
	aOutputStream.WriteUint32(lMachineGun);
	lMachineGun = (TUint32)iRightMachineGun2;
	aOutputStream.WriteUint32(lMachineGun);
	lMachineGun = (TUint32)iLeftMachineGun1;
	aOutputStream.WriteUint32(lMachineGun);
	lMachineGun = (TUint32)iLeftMachineGun2;
	aOutputStream.WriteUint32(lMachineGun);

	TUint32 l88Gun = (TUint32)iRightType88Gun1;
	aOutputStream.WriteUint32(l88Gun);
	l88Gun = (TUint32)iRightType88Gun2;
	aOutputStream.WriteUint32(l88Gun);
	l88Gun = (TUint32)iLeftType88Gun1;
	aOutputStream.WriteUint32(l88Gun);
	l88Gun = (TUint32)iLeftType88Gun2;
	aOutputStream.WriteUint32(l88Gun);

	TUint32 lConcreteGun = (TUint32)iRightConcreteGun1;
	aOutputStream.WriteUint32(lConcreteGun);
	lConcreteGun = (TUint32)iLeftConcreteGun1;
	aOutputStream.WriteUint32(lConcreteGun);
}

void CHq::LoadFromDisk(CFileReadStream &aReadStream)
{
	BuildingLoadContentFromDisk(aReadStream);
	TUint32 lFlagPolePointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lFlagPolePointerMemoryAddress, (TUint32)(&iFlagPole));

	TUint32 lMachineGunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lMachineGunPointerMemoryAddress, (TUint32)(&iRightMachineGun1));
	lMachineGunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lMachineGunPointerMemoryAddress, (TUint32)(&iRightMachineGun2));
	lMachineGunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lMachineGunPointerMemoryAddress, (TUint32)(&iLeftMachineGun1));
	lMachineGunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lMachineGunPointerMemoryAddress, (TUint32)(&iLeftMachineGun2));

	TUint32 l88GunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(l88GunPointerMemoryAddress, (TUint32)(&iRightType88Gun1));
	l88GunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(l88GunPointerMemoryAddress, (TUint32)(&iRightType88Gun2));
	l88GunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(l88GunPointerMemoryAddress, (TUint32)(&iLeftType88Gun1));
	l88GunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(l88GunPointerMemoryAddress, (TUint32)(&iLeftType88Gun2));

	TUint32 lConcreteGunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lConcreteGunPointerMemoryAddress, (TUint32)(&iRightConcreteGun1));
	lConcreteGunPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction(lConcreteGunPointerMemoryAddress, (TUint32)(&iLeftConcreteGun1));
}
