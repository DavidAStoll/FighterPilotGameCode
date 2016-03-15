/*
 ============================================================================
 Name		: Explosion100KgInWater.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion100KgInWater implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Explosions/CExplosion100KgInWater.h"

CExplosion100KgInWater::CExplosion100KgInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion100KgInWater::CExplosion100KgInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion100KgInWater::~CExplosion100KgInWater()
{
}

CExplosion100KgInWater* CExplosion100KgInWater::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion100kgInWater;
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_100KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_100KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_100KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion100KgInWater* self = new CExplosion100KgInWater(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion100KgInWater* CExplosion100KgInWater::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion100kgInWater;
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_100KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_100KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_100KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion100KgInWater* self = new CExplosion100KgInWater(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion100KgInWater::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_100KG_IN_WATER_WIDTH, EXPLOSION_100KG_IN_WATER_WIDTH));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion100KgShellInWater), EXPLOSION_100KG_IN_WATER_WIDTH, EXPLOSION_100KG_IN_WATER_WIDTH, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_100KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion100KgShellInWater);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = EXPLOSION_100KG_IN_WATER_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Small3b_Water, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion100KgInWater::LoadExplosion100KgInWaterTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion100KgShellInWater);
}

void CExplosion100KgInWater::UnloadExplosion100KgInWaterTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion100KgShellInWater);
}

void CExplosion100KgInWater::InitilizeExplosion100KgInWater()
{
	CExplosion100KgInWater::LoadExplosion100KgInWaterTexturesL();
}

void CExplosion100KgInWater::RemoveExplosion100KgInWater()
{
	CExplosion100KgInWater::UnloadExplosion100KgInWaterTextures();
}
