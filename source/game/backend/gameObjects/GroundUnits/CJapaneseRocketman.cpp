/*
 ============================================================================
 Name		: JapaneseRocketman.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseRocketman implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

SBoundingCollisionPoint* CJapaneseRocketman::CollisionPoints;

CJapaneseRocketman::CJapaneseRocketman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(JAPANESE_ROCKETMAN_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(JAPANESE_ROCKETMAN_MAX_SPEED), GRENADELAUNCHER_FRAMES_BETWEEN_FIRE, JAPANESE_ROCKETMAN_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iGrendaeLauncher = NULL;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CJapaneseRocketman::~CJapaneseRocketman()
{
	if(iGrendaeLauncher)
	{
		delete iGrendaeLauncher;
		iGrendaeLauncher = NULL;
	}
}

CJapaneseRocketman* CJapaneseRocketman::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_ROCKETMAN_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_ROCKETMAN_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseRocketman;

	CJapaneseRocketman* self = new CJapaneseRocketman(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseRocketman::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanStanding), JAPANESE_ROCKETMAN_WIDTH, JAPANESE_ROCKETMAN_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_GROUND_UNIT_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseRocketman::CollisionPoints);

	//create GunWeapon
	iGrendaeLauncher = CGrenadeLauncher::New(false, &iCoordinates, &iAngle, JAPANESE_ROCKETMAN_GRENADE_LAUNCHER_X_LOCATION, JAPANESE_ROCKETMAN_GRENADE_LAUNCHER_Y_LOCATION, &iObjectReflected, &iConflictSide);

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

void CJapaneseRocketman::TakeDamage(const SDamage& aDamage, const TDamageType aType)
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

void CJapaneseRocketman::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = JAPANESE_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanWalking);
		lDefaultRequest.TotalImages = 3;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = JAPANESE_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateFiring)
	{
		//the firing will actually be done in the update method, important is that the default animation is standing
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = JAPANESE_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateDieing)
	{
		//this one we first play the falling down animation, and then go back to the default animation, which is the soldier laying down on the floor
		SAnimationRequest lPlayAnimation;
		lPlayAnimation.FramesUntilSwitch = 6;
		lPlayAnimation.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanFalling);
		lPlayAnimation.TotalImages = 1;
		lPlayAnimation.TotalLength = 32;
		lPlayAnimation.Width = JAPANESE_ROCKETMAN_WIDTH;
		lPlayAnimation.LowerT = 0;
		lPlayAnimation.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lPlayAnimation));
		
		iSoundObject->PlayHitBox(ESoundFileId_Male_Scream);
		
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanDeadOnFloor);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = JAPANESE_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}
}

void CJapaneseRocketman::UpdateWeapon()
{
	iGrendaeLauncher->Update();
}

void CJapaneseRocketman::Shoot()
{
	if(iGrendaeLauncher->GetIsReadyToShoot())
	{
		//start firing animation
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseRocketmanShooting);
		lDefaultRequest.TotalImages = JAPANESE_ROCKETMAN_FRAMES_UNTIL_SWITCH_SHOOTING_ANIMATION;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = JAPANESE_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));//only plays it once and then returns to default animation

		//create bullet
		iGrendaeLauncher->Shoot();
	}
}

void CJapaneseRocketman::InitilizeJapaneseRocketman()
{
	CJapaneseRocketman::LoadJapaneseRocketmanTextures();
	CJapaneseRocketman::CreateJapaneseRocketmanCollisionPointsL();
}

void CJapaneseRocketman::RemoveJapaneseRocketman()
{
	CJapaneseRocketman::UnloadJapaneseRocketmanTextures();
	CJapaneseRocketman::RemoveJapaneseRocketmanCollisionPoints();
}

void CJapaneseRocketman::CreateJapaneseRocketmanCollisionPointsL()
{
	CJapaneseRocketman::CollisionPoints = new SBoundingCollisionPoint[BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseRocketman::CollisionPoints[0].XOffset = -2;
	CJapaneseRocketman::CollisionPoints[0].YOffset = JAPANESE_ROCKETMAN_HEIGHT + 3;
	CJapaneseRocketman::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CJapaneseRocketman::CollisionPoints[1].XOffset = 8;
	CJapaneseRocketman::CollisionPoints[1].YOffset = 6;
	CJapaneseRocketman::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseRocketman::CollisionPoints[2].XOffset = JAPANESE_ROCKETMAN_WIDTH + 5;
	CJapaneseRocketman::CollisionPoints[2].YOffset = 0;
	CJapaneseRocketman::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseRocketman::CollisionPoints[3].XOffset = JAPANESE_ROCKETMAN_WIDTH - 3;
	CJapaneseRocketman::CollisionPoints[3].YOffset = JAPANESE_ROCKETMAN_HEIGHT;
	CJapaneseRocketman::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseRocketman::CollisionPoints[4].XOffset = 0;
	CJapaneseRocketman::CollisionPoints[4].YOffset = JAPANESE_ROCKETMAN_HEIGHT + 3;
	CJapaneseRocketman::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CJapaneseRocketman::RemoveJapaneseRocketmanCollisionPoints()
{
	if(CJapaneseRocketman::CollisionPoints)
	{
		delete[] CJapaneseRocketman::CollisionPoints;
		CJapaneseRocketman::CollisionPoints = NULL;
	}
}

void CJapaneseRocketman::LoadJapaneseRocketmanTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRocketmanStanding);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRocketmanWalking);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRocketmanShooting);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRocketmanFalling);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseRocketmanDeadOnFloor);
}

void CJapaneseRocketman::UnloadJapaneseRocketmanTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRocketmanStanding);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRocketmanWalking);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRocketmanShooting);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRocketmanFalling);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseRocketmanDeadOnFloor);
}

void CJapaneseRocketman::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iGrendaeLauncher->SaveOnDisk(aOutputStream);
}

void CJapaneseRocketman::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	TWeaponIdentifier lWeaponId = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iGrendaeLauncher->LoadFromDisk(aReadStream);
}
