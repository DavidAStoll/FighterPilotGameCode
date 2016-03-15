/*
 ============================================================================
 Name		: AmericanRifleman.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanRifleman implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

SBoundingCollisionPoint* CAmericanRifleman::CollisionPoints;

CAmericanRifleman::CAmericanRifleman(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(AMERICAN_RIFLEMAN_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(AMERICAN_RIFLEMAN_MAX_SPEED), RIFLE_FRAMES_BETWEEN_FIRE, AMERICAN_RIFLEMAN_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iRifle = NULL;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX =iSpeed * CMath::GraphicsCosTable(iAngle);
}

CAmericanRifleman::~CAmericanRifleman()
{
	if(iRifle)
	{
		delete iRifle;
		iRifle = NULL;
	}
}

CAmericanRifleman* CAmericanRifleman::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AMERICAN_RIFLEMAN_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_RIFLEMAN_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanRifleman;

	CAmericanRifleman* self = new CAmericanRifleman(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanRifleman::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanWalking), AMERICAN_RIFLEMAN_WIDTH, AMERICAN_RIFLEMAN_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, BASIC_GROUND_UNIT_NUMBER_OF_PROJECTED_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanRifleman::CollisionPoints);
	//create GunWeapon
	iRifle = CRifle::New(false, &iCoordinates, &iAngle, AMERICAN_RIFLEMAN_8MM_RIFLE_X_LOCATION, AMERICAN_RIFLEMAN_8MM_RIFLE_Y_LOCATION, &iObjectReflected, &iConflictSide);

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

void CAmericanRifleman::TakeDamage(const SDamage& aDamage, const TDamageType aType)
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

void CAmericanRifleman::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = AMERICAN_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanWalking);
		lDefaultRequest.TotalImages = 3;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = AMERICAN_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateFiring)
	{
		//the firing will actually be done in the update method, important is that the default animation is standing
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanStanding);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = AMERICAN_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}else if(aNewState == EGroundUnitStateDieing)
	{
		//this one we first play the falling down animation, and then go back to the default animation, which is the soldier laying down on the floor
		SAnimationRequest lPlayAnimation;
		lPlayAnimation.FramesUntilSwitch = 6;
		lPlayAnimation.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanFalling);
		lPlayAnimation.TotalImages = 1;
		lPlayAnimation.TotalLength = 32;
		lPlayAnimation.Width = AMERICAN_RIFLEMAN_WIDTH;
		lPlayAnimation.LowerT = 0;
		lPlayAnimation.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lPlayAnimation));
		
		iSoundObject->PlayHitBox(ESoundFileId_Male_Scream2);
		
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanDeadOnFloor);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 32;
		lDefaultRequest.Width = AMERICAN_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	}
}

void CAmericanRifleman::UpdateWeapon()
{
	iRifle->Update();
}

void CAmericanRifleman::Shoot()
{
	if(iRifle->GetIsReadyToShoot())
	{
		//start firing animation
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 2;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAmericanRiflemanShooting);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = AMERICAN_RIFLEMAN_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 1;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));//only plays it once and then returns to default animation

		//create bullet
		iRifle->Shoot();
	}
}

void CAmericanRifleman::InitilizeAmericanRifleman()
{
	CAmericanRifleman::LoadAmericanRiflemanTextures();
	CAmericanRifleman::CreateAmericanRiflemanCollisionPointsL();
}

void CAmericanRifleman::RemoveAmericanRifleman()
{
	CAmericanRifleman::UnloadAmericanRiflemanTextures();
	CAmericanRifleman::RemoveAmericanRiflemanCollisionPoints();
}

void CAmericanRifleman::CreateAmericanRiflemanCollisionPointsL()
{
	CAmericanRifleman::CollisionPoints = new SBoundingCollisionPoint[BASIC_GROUND_UNIT_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanRifleman::CollisionPoints[0].XOffset = -2;
	CAmericanRifleman::CollisionPoints[0].YOffset = AMERICAN_RIFLEMAN_HEIGHT + 3;
	CAmericanRifleman::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CAmericanRifleman::CollisionPoints[1].XOffset = 8;
	CAmericanRifleman::CollisionPoints[1].YOffset = 6;
	CAmericanRifleman::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanRifleman::CollisionPoints[2].XOffset = AMERICAN_RIFLEMAN_WIDTH + 5;
	CAmericanRifleman::CollisionPoints[2].YOffset = 0;
	CAmericanRifleman::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanRifleman::CollisionPoints[3].XOffset = AMERICAN_RIFLEMAN_WIDTH - 3;
	CAmericanRifleman::CollisionPoints[3].YOffset = AMERICAN_RIFLEMAN_HEIGHT;
	CAmericanRifleman::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanRifleman::CollisionPoints[4].XOffset = 0;
	CAmericanRifleman::CollisionPoints[4].YOffset = AMERICAN_RIFLEMAN_HEIGHT + 3;
	CAmericanRifleman::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
}

void CAmericanRifleman::RemoveAmericanRiflemanCollisionPoints()
{
	if(CAmericanRifleman::CollisionPoints)
	{
		delete[] CAmericanRifleman::CollisionPoints;
		CAmericanRifleman::CollisionPoints = NULL;
	}
}

void CAmericanRifleman::LoadAmericanRiflemanTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRiflemanStanding);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRiflemanWalking);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRiflemanShooting);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRiflemanFalling);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAmericanRiflemanDeadOnFloor);
}

void CAmericanRifleman::UnloadAmericanRiflemanTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRiflemanStanding);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRiflemanWalking);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRiflemanShooting);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRiflemanFalling);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAmericanRiflemanDeadOnFloor);
}

void CAmericanRifleman::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iRifle->SaveOnDisk(aOutputStream);
}

void CAmericanRifleman::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	TWeaponIdentifier lWeaponId = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iRifle->LoadFromDisk(aReadStream);
}
