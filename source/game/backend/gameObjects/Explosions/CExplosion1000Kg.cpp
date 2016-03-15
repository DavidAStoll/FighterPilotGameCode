/*
 ============================================================================
 Name		: Explosion1000Kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion1000Kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion1000Kg.h"

CExplosion1000Kg::CExplosion1000Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes,SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion1000Kg::CExplosion1000Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes,SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion1000Kg::~CExplosion1000Kg()
{
}

CExplosion1000Kg* CExplosion1000Kg::New(const TPoint& aLocation, TUint aObjectsItCanDamage)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion1000kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_1000KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_1000KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_1000KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= aObjectsItCanDamage;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	
	CExplosion1000Kg* self = new CExplosion1000Kg(aLocation, lAttributes,lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion1000Kg* CExplosion1000Kg::New(const TPointIntFloat& aLocation, TUint aObjectsItCanDamage)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion1000kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_1000KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_1000KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_1000KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= aObjectsItCanDamage;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	
	CExplosion1000Kg* self = new CExplosion1000Kg(aLocation, lAttributes,lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion1000Kg::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_1000KG_WIDTH, EXPLOSION_1000KG_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion1000KgShell), EXPLOSION_1000KG_WIDTH, EXPLOSION_1000KG_HEIGHT, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.8);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_1000KG_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion1000KgShell);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = 102;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.7;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Large2, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion1000Kg::LoadExplosion1000KgTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion1000KgShell);
}

void CExplosion1000Kg::UnloadExplosion1000KgTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion1000KgShell);
}

void CExplosion1000Kg::InitilizeExplosion1000Kg()
{
	CExplosion1000Kg::LoadExplosion1000KgTexturesL();
}

void CExplosion1000Kg::RemoveExplosion1000Kg()
{
	CExplosion1000Kg::UnloadExplosion1000KgTextures();
}
