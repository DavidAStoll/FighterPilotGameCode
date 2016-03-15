/*
 ============================================================================
 Name		: Explosion1000KgInAir.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion1000KgInAir implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInAir.h"

CExplosion1000KgInAir::CExplosion1000KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion1000KgInAir::CExplosion1000KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion1000KgInAir::~CExplosion1000KgInAir()
{

}

CExplosion1000KgInAir* CExplosion1000KgInAir::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion1000kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_1000KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_1000KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_1000KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion1000KgInAir* self = new CExplosion1000KgInAir(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion1000KgInAir* CExplosion1000KgInAir::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion1000kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_1000KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_1000KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_1000KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion1000KgInAir* self = new CExplosion1000KgInAir(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion1000KgInAir::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_IN_AIR_1000KG_WIDTH, EXPLOSION_IN_AIR_1000KG_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion1000KgShellInAir), EXPLOSION_IN_AIR_1000KG_WIDTH, EXPLOSION_IN_AIR_1000KG_HEIGHT, iZCor, 0, 1, 0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_IN_AIR_1000KG_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion1000KgShellInAir);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = 100;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
	
	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Large, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion1000KgInAir::LoadExplosion1000KgInAirTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion1000KgShellInAir);
}

void CExplosion1000KgInAir::UnloadExplosion1000KgInAirTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion1000KgShellInAir);
}

void CExplosion1000KgInAir::InitilizeExplosion1000KgInAir()
{
	CExplosion1000KgInAir::LoadExplosion1000KgInAirTexturesL();
}

void CExplosion1000KgInAir::RemoveExplosion1000KgInAir()
{
	CExplosion1000KgInAir::UnloadExplosion1000KgInAirTextures();
}
