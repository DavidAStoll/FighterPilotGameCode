/*
 ============================================================================
 Name		: Explosion500KgInWater.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion500KgInWater implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion500KgInWater.h"

CExplosion500KgInWater::CExplosion500KgInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
}

CExplosion500KgInWater::CExplosion500KgInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
}

CExplosion500KgInWater::~CExplosion500KgInWater()
{
}

CExplosion500KgInWater* CExplosion500KgInWater::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInWater;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_500KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_500KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_500KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;

	CExplosion500KgInWater* self = new CExplosion500KgInWater(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion500KgInWater* CExplosion500KgInWater::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInWater;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_500KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_500KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_500KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;

	CExplosion500KgInWater* self = new CExplosion500KgInWater(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion500KgInWater::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_500KG_IN_WATER_WIDTH, EXPLOSION_500KG_IN_WATER_WIDTH));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion500KgShellInWater), EXPLOSION_500KG_IN_WATER_WIDTH, EXPLOSION_500KG_IN_WATER_WIDTH, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_500KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion500KgShellInWater);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = EXPLOSION_500KG_IN_WATER_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new  SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Medium_Water, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion500KgInWater::LoadExplosion500KgInWaterTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion500KgShellInWater);
}

void CExplosion500KgInWater::UnloadExplosion500KgInWaterTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion500KgShellInWater);
}

void CExplosion500KgInWater::InitilizeExplosion500KgInWater()
{
	CExplosion500KgInWater::LoadExplosion500KgInWaterTexturesL();
}

void CExplosion500KgInWater::RemoveExplosion500KgInWater()
{
	CExplosion500KgInWater::UnloadExplosion500KgInWaterTextures();
}
