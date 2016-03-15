/*
 ============================================================================
 Name		: Explosion100Kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosion100Kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion100Kg.h"

CExplosion100Kg::CExplosion100Kg(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion100Kg::CExplosion100Kg(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion100Kg::~CExplosion100Kg()
{
}

CExplosion100Kg* CExplosion100Kg::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion100kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_100KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_100KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_100KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	
	CExplosion100Kg* self = new CExplosion100Kg(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion100Kg* CExplosion100Kg::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion100kg;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_100KG_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_100KG_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_100KG_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	
	CExplosion100Kg* self = new CExplosion100Kg(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion100Kg::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_100KG_WIDTH, EXPLOSION_100KG_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion100KgShell), EXPLOSION_100KG_WIDTH, EXPLOSION_100KG_HEIGHT, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.78125);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_100KG_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion100KgShell);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = 25;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Small3b, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion100Kg::LoadExplosion100KgTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion100KgShell);
}

void CExplosion100Kg::UnloadExplosion100KgTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion100KgShell);
}

void CExplosion100Kg::InitilizeExplosion100Kg()
{
	CExplosion100Kg::LoadExplosion100KgTexturesL();
}

void CExplosion100Kg::RemoveExplosion100Kg()
{
	CExplosion100Kg::UnloadExplosion100KgTextures();
}
