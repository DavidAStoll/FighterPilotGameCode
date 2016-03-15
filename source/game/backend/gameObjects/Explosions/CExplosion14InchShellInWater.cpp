/*
 ============================================================================
 Name		: Explosion14InchShellInWater.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosion14InchShellInWater implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Explosions/CExplosion14InchShellInWater.h"

CExplosion14InchShellInWater::CExplosion14InchShellInWater(const TPoint& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion14InchShellInWater::CExplosion14InchShellInWater(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes, SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion14InchShellInWater::~CExplosion14InchShellInWater()
{
}

CExplosion14InchShellInWater* CExplosion14InchShellInWater::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion14InchShellInWater;	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_14INCH_SHELL_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_14INCH_SHELL_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_14INCH_SHELL_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion14InchShellInWater* self = new CExplosion14InchShellInWater(aLocation, lAttributes,lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion14InchShellInWater* CExplosion14InchShellInWater::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion14InchShellInWater;
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_14INCH_SHELL_IN_WATER_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_14INCH_SHELL_IN_WATER_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_14INCH_SHELL_IN_WATER_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = false;
	
	CExplosion14InchShellInWater* self = new CExplosion14InchShellInWater(aLocation, lAttributes, lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion14InchShellInWater::ConstructL()
{

	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_14INCH_SHELL_IN_WATER_WIDTH, EXPLOSION_14INCH_SHELL_IN_WATER_WIDTH));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion14InchShellInWater), EXPLOSION_14INCH_SHELL_IN_WATER_WIDTH, EXPLOSION_14INCH_SHELL_IN_WATER_WIDTH, iZCor, 0, 1, 0, 1);
	iTextureObject->SetNewTValue(0, 0.58594);

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_14INCH_SHELL_IN_WATER_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion14InchShellInWater);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = EXPLOSION_14INCH_SHELL_IN_WATER_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.58594;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Explosion_Large_Water, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion14InchShellInWater::LoadExplosion14InchShellInWaterTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion14InchShellInWater);
}

void CExplosion14InchShellInWater::UnloadExplosion14InchShellInWaterTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion14InchShellInWater);
}

void CExplosion14InchShellInWater::InitilizeExplosion14InchShellInWater()
{
	CExplosion14InchShellInWater::LoadExplosion14InchShellInWaterTexturesL();
}

void CExplosion14InchShellInWater::RemoveExplosion14InchShellInWater()
{
	CExplosion14InchShellInWater::UnloadExplosion14InchShellInWaterTextures();
}

