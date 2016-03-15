/*
 ============================================================================
 Name		: JapaneseRifleman.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copyright HQ Interactive Inc.
 Description : CJapaneseRifleman implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

SBoundingCollisionPoint* CJapaneseRifleman::CollisionPoints;

CJapaneseRifleman::CJapaneseRifleman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(JAPANESE_RIFLEMAN_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(JAPANESE_RIFLEMAN_MAX_SPEED), RIFLE_FRAMES_BETWEEN_FIRE, JAPANESE_RIFLEMAN_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iRifle = NULL;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CJapaneseRifleman::~CJapaneseRifleman()
{
	if(iRifle)
	{
		delete iRifle;
		iRifle = NULL;
	}
}

CJapaneseRifleman* CJapaneseRifleman::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_RIFLEMAN_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_RIFLEMAN_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseRifleman;

	CJapaneseRifleman* self = new CJapaneseRifleman(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseRifleman::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanStanding), JAPANESE_RIFLEMAN_WIDTH, JAPANESE_RIFLEMAN_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_GROUND_UNIT_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseRifleman::CollisionPoints);

	//create GunWeapon
	iRifle = CRifle::New(false, &iCoordinates, &iAngle, JAPANESE_RIFLEMAN_8MM_RIFLE_X_LOCATION, JAPANESE_RIFLEMAN_8MM_RIFLE_Y_LOCATION, &iObjectReflected, &iConflictSide);

	//Animation Player
	SAnimationRequest lDefaultRequest;//not really used
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	UpdateCurrentState(EGroundUnitStateStanding);//resets the animation player

	if(iObjectReflected)
	{
		iAngle = 0;
		iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
	iSoundObject = CSoundObject::New(ESoundFileId_Male_Painful, GetHitBox());
}

//------------------------- functions --------------------------//

void CJapaneseRifleman::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			iHealth = 0;
			Die(aType);
		}else if(iHealth > 0)
		{
			iSoundObject->PlayHitBox();
		}
	}
	//does not penetrate armor, no damage is dealt
}

void CJapaneseRifleman::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = JAPANESE_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanWalking);
		lDefaultRequest.TotalImages = 3;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = JAPANESE_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateFiring)
	{
		//the firing will actually be done in the update method, important is that the default animation is standing
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = JAPANESE_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateDieing)
	{
		//this one we first play the falling down animation, and then go back to the default animation, which is the soldier laying down on the floor
		SAnimationRequest lPlayAnimation;
		lPlayAnimation.FramesUntilSwitch = 6;
		lPlayAnimation.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanFalling);
		lPlayAnimation.TotalImages = 1;
		lPlayAnimation.TotalLength = 32;
		lPlayAnimation.Width = JAPANESE_RIFLEMAN_WIDTH;
		lPlayAnimation.LowerT = 0;
		lPlayAnimation.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lPlayAnimation));
		
		iSoundObject->PlayHitBox(ESoundFileId_Male_Scream);
		
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanDeadOnFloor);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = JAPANESE_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}
}

void CJapaneseRifleman::UpdateWeapon()
{
	iRifle->Update();
}

void CJapaneseRifleman::Shoot()
{
	if(iRifle->GetIsReadyToShoot())
	{
		//start firing animation
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 2;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRiflemanShooting);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = JAPANESE_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));//only plays it once and then returns to default animation

		//create bullet
		iRifle->Shoot();
	}
}

void CJapaneseRifleman::InitilizeJapaneseRifleman()
{
	CJapaneseRifleman::LoadJapaneseRiflemanTextures();
	CJapaneseRifleman::CreateJapaneseRiflemanCollisionPointsL();
}

void CJapaneseRifleman::RemoveJapaneseRifleman()
{
	CJapaneseRifleman::UnloadJapaneseRiflemanTextures();
	CJapaneseRifleman::RemoveJapaneseRiflemanCollisionPoints();
}

void CJapaneseRifleman::CreateJapaneseRiflemanCollisionPointsL()
{
	CJapaneseRifleman::CollisionPoints = new SBoundingCollisionPoint[BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseRifleman::CollisionPoints[0].XOffset = -2;
	CJapaneseRifleman::CollisionPoints[0].YOffset = JAPANESE_RIFLEMAN_HEIGHT + 3;
	CJapaneseRifleman::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CJapaneseRifleman::CollisionPoints[1].XOffset = 8;
	CJapaneseRifleman::CollisionPoints[1].YOffset = 6;
	CJapaneseRifleman::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseRifleman::CollisionPoints[2].XOffset = JAPANESE_RIFLEMAN_WIDTH + 5;
	CJapaneseRifleman::CollisionPoints[2].YOffset = 0;
	CJapaneseRifleman::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseRifleman::CollisionPoints[3].XOffset = JAPANESE_RIFLEMAN_WIDTH - 3;
	CJapaneseRifleman::CollisionPoints[3].YOffset = JAPANESE_RIFLEMAN_HEIGHT;
	CJapaneseRifleman::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseRifleman::CollisionPoints[4].XOffset = 0;
	CJapaneseRifleman::CollisionPoints[4].YOffset = JAPANESE_RIFLEMAN_HEIGHT + 3;
	CJapaneseRifleman::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CJapaneseRifleman::RemoveJapaneseRiflemanCollisionPoints()
{
	if(CJapaneseRifleman::CollisionPoints)
	{
		delete[] CJapaneseRifleman::CollisionPoints;
		CJapaneseRifleman::CollisionPoints = NULL;
	}
}

void CJapaneseRifleman::LoadJapaneseRiflemanTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRiflemanStanding);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRiflemanWalking);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRiflemanShooting);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRiflemanFalling);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRiflemanDeadOnFloor);
}

void CJapaneseRifleman::UnloadJapaneseRiflemanTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRiflemanStanding);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRiflemanWalking);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRiflemanShooting);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRiflemanFalling);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRiflemanDeadOnFloor);
}

void CJapaneseRifleman::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iRifle->SaveOnDisk(aOutputStream);
}

void CJapaneseRifleman::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	TWeaponIdentifier lWeaponId = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iRifle->LoadFromDisk(aReadStream);
}
