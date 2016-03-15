/*
 ============================================================================
 Name		: AmericanRocketman.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanRocketman implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

SBoundingCollisionPoint* CAmericanRocketman::CollisionPoints;

CAmericanRocketman::CAmericanRocketman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(AMERICAN_ROCKETMAN_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(AMERICAN_ROCKETMAN_MAX_SPEED), GRENADELAUNCHER_FRAMES_BETWEEN_FIRE, AMERICAN_ROCKETMAN_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iGrendaeLauncher = NULL;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CAmericanRocketman::~CAmericanRocketman()
{
	if(iGrendaeLauncher)
	{
		delete iGrendaeLauncher;
		iGrendaeLauncher = NULL;
	}
}

CAmericanRocketman* CAmericanRocketman::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AMERICAN_ROCKETMAN_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_ROCKETMAN_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanRocketman;

	CAmericanRocketman* self = new CAmericanRocketman(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanRocketman::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanStanding), AMERICAN_ROCKETMAN_WIDTH, AMERICAN_ROCKETMAN_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_GROUND_UNIT_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanRocketman::CollisionPoints);

	//create GunWeapon
	iGrendaeLauncher = CGrenadeLauncher::New(false, &iCoordinates, &iAngle, AMERICAN_ROCKETMAN_GRENADE_LAUNCHER_X_LOCATION, AMERICAN_ROCKETMAN_GRENADE_LAUNCHER_Y_LOCATION, &iObjectReflected, &iConflictSide);

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
	iSoundObject = CSoundObject::New(ESoundFileId_Male_Painful2, GetHitBox());
}

//------------------------- functions --------------------------//

void CAmericanRocketman::TakeDamage(const SDamage& aDamage, const TDamageType aType)
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

void CAmericanRocketman::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = AMERICAN_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanWalking);
		lDefaultRequest.TotalImages = 3;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = AMERICAN_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateFiring)
	{
		//the firing will actually be done in the update method, important is that the default animation is standing
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = AMERICAN_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateDieing)
	{
		//this one we first play the falling down animation, and then go back to the default animation, which is the soldier laying down on the floor
		SAnimationRequest lPlayAnimation;
		lPlayAnimation.FramesUntilSwitch = 6;
		lPlayAnimation.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanFalling);
		lPlayAnimation.TotalImages = 1;
		lPlayAnimation.TotalLength = 32;
		lPlayAnimation.Width = AMERICAN_ROCKETMAN_WIDTH;
		lPlayAnimation.LowerT = 0;
		lPlayAnimation.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lPlayAnimation));
		
		iSoundObject->PlayHitBox(ESoundFileId_Male_Scream2);
		
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanDeadOnFloor);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = AMERICAN_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}
}

void CAmericanRocketman::UpdateWeapon()
{
	iGrendaeLauncher->Update();
}

void CAmericanRocketman::Shoot()
{
	if(iGrendaeLauncher->GetIsReadyToShoot())
	{
		//start firing animation
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRocketmanShooting);
		lDefaultRequest.TotalImages = AMERICAN_ROCKETMAN_FRAMES_UNTIL_SWITCH_SHOOTING_ANIMATION;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = AMERICAN_ROCKETMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));//only plays it once and then returns to default animation

		//create bullet
		iGrendaeLauncher->Shoot();
	}
}

void CAmericanRocketman::InitilizeAmericanRocketman()
{
	CAmericanRocketman::LoadAmericanRocketmanTextures();
	CAmericanRocketman::CreateAmericanRocketmanCollisionPointsL();
}

void CAmericanRocketman::RemoveAmericanRocketman()
{
	CAmericanRocketman::UnloadAmericanRocketmanTextures();
	CAmericanRocketman::RemoveAmericanRocketmanCollisionPoints();
}

void CAmericanRocketman::CreateAmericanRocketmanCollisionPointsL()
{
	CAmericanRocketman::CollisionPoints = new SBoundingCollisionPoint[BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanRocketman::CollisionPoints[0].XOffset = -2;
	CAmericanRocketman::CollisionPoints[0].YOffset = AMERICAN_ROCKETMAN_HEIGHT + 3;
	CAmericanRocketman::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CAmericanRocketman::CollisionPoints[1].XOffset = 8;
	CAmericanRocketman::CollisionPoints[1].YOffset = 6;
	CAmericanRocketman::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanRocketman::CollisionPoints[2].XOffset = AMERICAN_ROCKETMAN_WIDTH + 5;
	CAmericanRocketman::CollisionPoints[2].YOffset = 0;
	CAmericanRocketman::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanRocketman::CollisionPoints[3].XOffset = AMERICAN_ROCKETMAN_WIDTH - 3;
	CAmericanRocketman::CollisionPoints[3].YOffset = AMERICAN_ROCKETMAN_HEIGHT;
	CAmericanRocketman::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanRocketman::CollisionPoints[4].XOffset = 0;
	CAmericanRocketman::CollisionPoints[4].YOffset = AMERICAN_ROCKETMAN_HEIGHT + 3;
	CAmericanRocketman::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CAmericanRocketman::RemoveAmericanRocketmanCollisionPoints()
{
	if(CAmericanRocketman::CollisionPoints)
	{
		delete[] CAmericanRocketman::CollisionPoints;
		CAmericanRocketman::CollisionPoints = NULL;
	}
}

void CAmericanRocketman::LoadAmericanRocketmanTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRocketmanStanding);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRocketmanWalking);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRocketmanShooting);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRocketmanFalling);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRocketmanDeadOnFloor);
}

void CAmericanRocketman::UnloadAmericanRocketmanTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRocketmanStanding);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRocketmanWalking);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRocketmanShooting);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRocketmanFalling);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRocketmanDeadOnFloor);
}

void CAmericanRocketman::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iGrendaeLauncher->SaveOnDisk(aOutputStream);
}

void CAmericanRocketman::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	TWeaponIdentifier lWeaponId;
	lWeaponId = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iGrendaeLauncher->LoadFromDisk(aReadStream);
}
