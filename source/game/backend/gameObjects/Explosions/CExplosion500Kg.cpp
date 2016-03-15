/*
 ============================================================================
 Name		: Explosion500Kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion500Kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion500Kg.h"

CExplosion500Kg::CExplosion500Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
}

CExplosion500Kg::CExplosion500Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
}

CExplosion500Kg::CExplosion500Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
	iDamage = aDamage;
	iArmorPenetration = aArmorPenetration;
}

CExplosion500Kg::CExplosion500Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes, TInt aDamage, TInt aArmorPenetration) :
	CBasicExplosion(aLocation, aAttributes, aExplosionAttributes)
{
	iDamage = aDamage;
	iArmorPenetration = aArmorPenetration;
}

CExplosion500Kg::~CExplosion500Kg()
{
}

CExplosion500Kg* CExplosion500Kg::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;

	CExplosion500Kg* self = new CExplosion500Kg(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion500Kg* CExplosion500Kg::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;

	CExplosion500Kg* self = new CExplosion500Kg(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion500Kg* CExplosion500Kg::New(const TPoint& aLocation, TInt aDamage, TInt aArmorPenetration)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;

	CExplosion500Kg* self = new CExplosion500Kg(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion500Kg* CExplosion500Kg::New(const TPointIntFloat& aLocation, TInt aDamage, TInt aArmorPenetration)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion500kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_500KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_500KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_500KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage = BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;

	CExplosion500Kg* self = new CExplosion500Kg(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion500Kg::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_500KG_WIDTH, EXPLOSION_500KG_HEIGHT));
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion500KgShell), EXPLOSION_500KG_WIDTH, EXPLOSION_500KG_HEIGHT, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_500KG_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion500KgShell);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = 50;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Medium2, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion500Kg::LoadExplosion500KgTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion500KgShell);
}

void CExplosion500Kg::UnloadExplosion500KgTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion500KgShell);
}

void CExplosion500Kg::InitilizeExplosion500Kg()
{
	CExplosion500Kg::LoadExplosion500KgTexturesL();
}

void CExplosion500Kg::RemoveExplosion500Kg()
{
	CExplosion500Kg::UnloadExplosion500KgTextures();
}
