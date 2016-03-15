/*
 ============================================================================
 Name		: Explosion1000KgInWater.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion1000KgInWater implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInWater.h"

CExplosion1000KgInWater::CExplosion1000KgInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion1000KgInWater::CExplosion1000KgInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion1000KgInWater::~CExplosion1000KgInWater()
{
}

CExplosion1000KgInWater* CExplosion1000KgInWater::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion1000kgInWater;	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_1000KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_1000KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_1000KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion1000KgInWater* self = new CExplosion1000KgInWater(aLocation, lAttributes,lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion1000KgInWater* CExplosion1000KgInWater::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion1000kgInWater;
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_1000KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_1000KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_1000KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion1000KgInWater* self = new CExplosion1000KgInWater(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion1000KgInWater::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_1000KG_IN_WATER_WIDTH, EXPLOSION_1000KG_IN_WATER_WIDTH));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion1000KgShellInWater), EXPLOSION_1000KG_IN_WATER_WIDTH, EXPLOSION_1000KG_IN_WATER_WIDTH, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.58594);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_1000KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion1000KgShellInWater);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = EXPLOSION_1000KG_IN_WATER_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.58594;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Large_Water, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion1000KgInWater::LoadExplosion1000KgInWaterTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion1000KgShellInWater);
}

void CExplosion1000KgInWater::UnloadExplosion1000KgInWaterTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion1000KgShellInWater);
}

void CExplosion1000KgInWater::InitilizeExplosion1000KgInWater()
{
	CExplosion1000KgInWater::LoadExplosion1000KgInWaterTexturesL();
}

void CExplosion1000KgInWater::RemoveExplosion1000KgInWater()
{
	CExplosion1000KgInWater::UnloadExplosion1000KgInWaterTextures();
}

