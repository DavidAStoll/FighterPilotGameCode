/*
 ============================================================================
 Name		: Hangar.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHangar implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Buildings/CHangar.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CHangar::CHangar(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CBuilding(BUILDINGS_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
{
	iPlanesLeft = HANGAR_DEFAULT_AIRPLANES;
}

CHangar::~CHangar()
{
}

CHangar* CHangar::New(TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = HANGAR_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = HANGAR_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierHangar;

	CHangar* self = new CHangar(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CHangar* CHangar::New(TInt aXLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = HANGAR_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding;
	lAttributes.Reflected = false;
	lAttributes.Armor = HANGAR_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierHangar;

	CHangar* self = new CHangar(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

void CHangar::ConstructL()
{
	//initilize TextureObject
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportHangar), HANGAR_WIDTH, HANGAR_HEIGHT, iZCor, 0, 0.625, 0, 1);

	//create hitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(HANGAR_WIDTH, HANGAR_HEIGHT)); //however, will not be used,since clouds will
}

// ----------------- functions --------------------- //


void CHangar::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			//set image to destroyed
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportHangarBroken2));
			iHealth = 0;
			Die(aType);
		}else if(iHealth <= HANGAR_HEALTH_DAMAGED)
		{
			//changes image to damaged image
			iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBuildingAirportHangarBroken1));
		}
	}
}

void CHangar::RemovePlane()
{
	iPlanesLeft--;
}

TBool CHangar::IsPlaneStillLeft()
{
	if(iPlanesLeft > 0 && iAlive)
		return true;
	else
		return false;
}

TInt CHangar::GetPlanesLeft()
{
	return iPlanesLeft;
}

void CHangar::InitilizeHangar()
{
	CHangar::LoadHangarTextures();
}

void CHangar::RemoveHangar()
{
	CHangar::UnloadHangarTextures();
}

void CHangar::LoadHangarTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportHangar);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportHangarBroken1);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAirportHangarBroken2);
}

void CHangar::UnloadHangarTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportHangar);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportHangarBroken1);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAirportHangarBroken2);
}

void CHangar::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BuildingSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iPlanesLeft);
}

void CHangar::LoadFromDisk(CFileReadStream &aReadStream)
{
	BuildingLoadContentFromDisk(aReadStream);
	iPlanesLeft = aReadStream.ReadInt32();
}
