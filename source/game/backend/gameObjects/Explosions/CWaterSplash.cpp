/*
 ============================================================================
 Name		: WaterSplash.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWaterSplash implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CWaterSplash.h"

CWaterSplash::CWaterSplash(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}


CWaterSplash::CWaterSplash(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}



CWaterSplash::~CWaterSplash()
{
}

CWaterSplash* CWaterSplash::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierWaterSplash;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = WATER_SPLASH_DAMAGE;
	lExplosionAttributes.ArmorPenetration = WATER_SPLASH_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = WATER_SPLASH_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CWaterSplash* self = new CWaterSplash(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

CWaterSplash* CWaterSplash::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierWaterSplash;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = WATER_SPLASH_DAMAGE;
	lExplosionAttributes.ArmorPenetration = WATER_SPLASH_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = WATER_SPLASH_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CWaterSplash* self = new CWaterSplash(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CWaterSplash::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosionWaterSplash), WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 1);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = WATER_SPLASH_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosionWaterSplash);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 64;
	lDefaultRequest.Width = WATER_SPLASH_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 1;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Water_Splash, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CWaterSplash::LoadWaterSplashTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosionWaterSplash);
}

void CWaterSplash::UnloadWaterSplashTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosionWaterSplash);
}

void CWaterSplash::InitilizeWaterSplash()
{
	CWaterSplash::LoadWaterSplashTexturesL();
}

void CWaterSplash::RemoveWaterSplash()
{
	CWaterSplash::UnloadWaterSplashTextures();
}

