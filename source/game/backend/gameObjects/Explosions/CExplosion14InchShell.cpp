/*
 ============================================================================
 Name		: Explosion14InchShell.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosion14InchShell implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Explosions/CExplosion14InchShell.h"

CExplosion14InchShell::CExplosion14InchShell(const TPoint& aLocation, SGameObjectAttributes& aAttributes,SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion14InchShell::CExplosion14InchShell(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes,SExplosionAttributes& aExplosionAttributes) :
CBasicExplosion(aLocation, aAttributes,aExplosionAttributes)
{
}

CExplosion14InchShell::~CExplosion14InchShell()
{
}

CExplosion14InchShell* CExplosion14InchShell::New(const TPoint& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion14InchShell;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_14INCH_SHELL_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_14INCH_SHELL_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_14INCH_SHELL_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	
	CExplosion14InchShell* self = new CExplosion14InchShell(aLocation, lAttributes,lExplosionAttributes);
	self->ConstructL();
	return self;
}

CExplosion14InchShell* CExplosion14InchShell::New(const TPointIntFloat& aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_DEFAULT_ARMOR;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_DEFAULT_HEALTH;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierExplosion14InchShell;
	//explosion properties
	SExplosionAttributes lExplosionAttributes;
	lExplosionAttributes.Damage = EXPLOSION_14INCH_SHELL_DAMAGE;
	lExplosionAttributes.ArmorPenetration = EXPLOSION_14INCH_SHELL_ARMOR_PENETRATION;
	lExplosionAttributes.FramesForExplosion = EXPLOSION_14INCH_SHELL_TOTAL_TIME_FOR_EXPLOSION;
	lExplosionAttributes.ObjectTypesItCanDamage= BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES;
	lExplosionAttributes.KillOnlyDuringFirstFrame = true;
	
	CExplosion14InchShell* self = new CExplosion14InchShell(aLocation, lAttributes,lExplosionAttributes);
	self->ConstructL();
	return self;
}

void CExplosion14InchShell::ConstructL()
{
	//create HitBox and AnimationPlayer
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(EXPLOSION_14INCH_SHELL_WIDTH, EXPLOSION_14INCH_SHELL_HEIGHT));

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdExplosion14InchShell), EXPLOSION_14INCH_SHELL_WIDTH, EXPLOSION_14INCH_SHELL_HEIGHT, iZCor, 0, 1, 0, 1);
	
	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = EXPLOSION_14INCH_SHELL_TOTAL_TIME_FOR_EXPLOSION / 5;
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdExplosion14InchShell);
	lDefaultRequest.TotalImages = 5;
	lDefaultRequest.TotalLength = 1024;
	lDefaultRequest.Width = 200;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.78125;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));

	iSoundObject = CSoundObject::New(ESoundFileId_Battle_Ship_Explosion, GetHitBox());
	iSoundObject->PlayHitBox();
}

//------------- functions -------------------------

void CExplosion14InchShell::LoadExplosion14InchShellTexturesL()
{
	CGame::Game->iImageStore->LoadImage(EImageIdExplosion14InchShell);
}

void CExplosion14InchShell::UnloadExplosion14InchShellTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdExplosion14InchShell);
}

void CExplosion14InchShell::InitilizeExplosion14InchShell()
{
	CExplosion14InchShell::LoadExplosion14InchShellTexturesL();
}

void CExplosion14InchShell::RemoveExplosion14InchShell()
{
	CExplosion14InchShell::UnloadExplosion14InchShellTextures();
}
