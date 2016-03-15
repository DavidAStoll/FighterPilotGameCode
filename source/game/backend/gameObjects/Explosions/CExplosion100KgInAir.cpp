/*
 ============================================================================
 Name		: Explosion100KgInAir.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosion100KgInAir implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion100KgInAir.h"

CExplosion100KgInAir::CExplosion100KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion100KgInAir::CExplosion100KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion100KgInAir::~CExplosion100KgInAir()
{
}

CExplosion100KgInAir* CExplosion100KgInAir::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion100kgInAir;
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_100KG_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_100KG_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_100KG_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion100KgInAir* self = new CExplosion100KgInAir(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}
CExplosion100KgInAir* CExplosion100KgInAir::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion100kgInAir;
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_100KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_100KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_100KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion100KgInAir* self = new CExplosion100KgInAir(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion100KgInAir::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_IN_AIR_100KG_WIDTH, EXPLOSION_IN_AIR_100KG_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion100KgShellInAir), EXPLOSION_IN_AIR_100KG_WIDTH, EXPLOSION_IN_AIR_100KG_HEIGHT, iZCor, 0, 1, 0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_IN_AIR_100KG_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion100KgShellInAir);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = EXPLOSION_IN_AIR_100KG_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new  SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Small3, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion100KgInAir::LoadExplosion100KgInAirTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion100KgShellInAir);
}

void CExplosion100KgInAir::UnloadExplosion100KgInAirTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion100KgShellInAir);
}

void CExplosion100KgInAir::InitilizeExplosion100KgInAir()
{
	CExplosion100KgInAir::LoadExplosion100KgInAirTexturesL();
}

void CExplosion100KgInAir::RemoveExplosion100KgInAir()
{
	CExplosion100KgInAir::UnloadExplosion100KgInAirTextures();
}
