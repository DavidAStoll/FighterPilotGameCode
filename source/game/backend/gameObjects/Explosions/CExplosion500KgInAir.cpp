/*
 ============================================================================
 Name		: Explosion500KgInAir.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion500KgInAir implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion500KgInAir.h"

CExplosion500KgInAir::CExplosion500KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
}

CExplosion500KgInAir::CExplosion500KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
}

CExplosion500KgInAir::CExplosion500KgInAir(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
	iDamage = aDamage;
	iArmorPenetration = aArmorPenetration;
}

CExplosion500KgInAir::CExplosion500KgInAir(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
	iDamage = aDamage;
	iArmorPenetration = aArmorPenetration;
}

CExplosion500KgInAir::~CExplosion500KgInAir()
{

}

CExplosion500KgInAir* CExplosion500KgInAir::New(const TPoint& aLocation, TInt aDamage, TInt aArmorPenetration, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	if(aHarmGroundUnits)
		lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	else
		lExplosionAttributes.ObjectTypesItCanDamage = (GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);

	CExplosion500KgInAir* self = new CExplosion500KgInAir(aLocation, lAttributes, lExplosionAttributes, aDamage, aArmorPenetration);
	self->ConstructL();
	return self;
}

CExplosion500KgInAir* CExplosion500KgInAir::New(const TPointIntFloat& aLocation, TInt aDamage, TInt aArmorPenetration, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	if(aHarmGroundUnits)
		lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	else
		lExplosionAttributes.ObjectTypesItCanDamage = (GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);

	CExplosion500KgInAir* self = new CExplosion500KgInAir(aLocation, lAttributes, lExplosionAttributes, aDamage, aArmorPenetration);
	self->ConstructL();
	return self;
}

CExplosion500KgInAir* CExplosion500KgInAir::NewForRocket(const TPointIntFloat& aLocation, TInt aDamage, TInt aArmorPenetration)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;

	CExplosion500KgInAir* self = new CExplosion500KgInAir(aLocation, lAttributes, lExplosionAttributes, aDamage, aArmorPenetration);
	self->ConstructL();
	return self;
}

CExplosion500KgInAir* CExplosion500KgInAir::New(const TPoint& aLocation, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	if(aHarmGroundUnits)
		lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	else
		lExplosionAttributes.ObjectTypesItCanDamage = (GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);

	CExplosion500KgInAir* self = new CExplosion500KgInAir(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion500KgInAir* CExplosion500KgInAir::New(const TPointIntFloat& aLocation, TBool aHarmGroundUnits)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kgInAir;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_IN_AIR_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_IN_AIR_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_IN_AIR_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	if(aHarmGroundUnits)
		lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	else
		lExplosionAttributes.ObjectTypesItCanDamage = (GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_SHIP);

	CExplosion500KgInAir* self = new CExplosion500KgInAir(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion500KgInAir::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_IN_AIR_500KG_WIDTH, EXPLOSION_IN_AIR_500KG_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion500KgShellInAir), EXPLOSION_IN_AIR_500KG_WIDTH, EXPLOSION_IN_AIR_500KG_HEIGHT, iZCor, 0, 1, 0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_IN_AIR_500KG_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion500KgShellInAir);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = 50;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Medium, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion500KgInAir::LoadExplosion500KgInAirTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion500KgShellInAir);
}

void CExplosion500KgInAir::UnloadExplosion500KgInAirTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion500KgShellInAir);
}

void CExplosion500KgInAir::InitilizeExplosion500KgInAir()
{
	CExplosion500KgInAir::LoadExplosion500KgInAirTexturesL();
}

void CExplosion500KgInAir::RemoveExplosion500KgInAir()
{
	CExplosion500KgInAir::UnloadExplosion500KgInAirTextures();
}

