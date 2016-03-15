/*
 ============================================================================
 Name		: Japanese97ChiHaTank.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapanese97ChiHaTank implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CJapanese97ChiHaTank::CollisionPoints;

CJapanese97ChiHaTank::CJapanese97ChiHaTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(JAPANESE_97_CHI_HA_TANK_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(JAPANESE_97_CHI_HA_TANK_MAX_SPEED), TANKGUN88MM_FRAMES_BETWEEN_FIRE, JAPANESE_97_CHI_HA_TANK_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CJapanese97ChiHaTank::~CJapanese97ChiHaTank()
{
	if(iTankGun)
	{
		delete iTankGun;
		iTankGun = NULL;
	}
	if(iDustCreator)
	{
		delete iDustCreator;
		iDustCreator = NULL;
	}
}

CJapanese97ChiHaTank* CJapanese97ChiHaTank::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_97_CHI_HA_TANK_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesArmouredGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_97_CHI_HA_TANK_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapanese97ChiHaTank;

	CJapanese97ChiHaTank* self = new CJapanese97ChiHaTank(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CJapanese97ChiHaTank::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapanese97ChiHaTankDriving), JAPANESE_97_CHI_HA_TANK_WIDTH, JAPANESE_97_CHI_HA_TANK_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_97_CHI_HA_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_97_CHI_HA_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapanese97ChiHaTank::CollisionPoints);

	SAnimationRequest lDefaultRequest;//not really used
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	//create smoke
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_97_CHI_HA_TANK_SMOKE_CREATOR_X, JAPANESE_97_CHI_HA_TANK_SMOKE_CREATOR_Y,  true, GROUND_UNIT_SMOKE_DEFAULT_COLOUR, GROUND_UNIT_SMOKE_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_SMOKE_DEFAULT_SIZE, GROUND_UNIT_SMOKE_DEFAULT_FRAMES);
	//another smoke creator if tank is moving to create dust
	iDustCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_X, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_Y,  false, GROUND_UNIT_DUST_DEFAULT_COLOUR, GROUND_UNIT_DUST_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_DUST_DEFAULT_SIZE, GROUND_UNIT_DUST_DEFAULT_FRAMES, GROUND_UNIT_DUST_DEFAULT_ANGLE_OFFSET, GROUND_UNIT_DUST_DEFAULT_SPEED, GROUND_UNIT_DUST_DEFAULT_X_EXPO,GROUND_UNIT_DUST_DEFAULT_Y_EXPO);
	//create GunWeapon
	iTankGun = CTankGun80mm::New(false, &iCoordinates, &iAngle, JAPANESE_97_CHI_HA_TANK_GUN_X_LOCATION, JAPANESE_97_CHI_HA_TANK_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide);
	
	//create track sound for the tank
	iSoundObject = CSoundObject::New(ESoundFileId_Tank_Tracks, iHitBox);
	
	if(iObjectReflected)
	{
		iAngle = 0;
		iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
	UpdateCurrentState(EGroundUnitStateStanding);//resets the animation player
}

void CJapanese97ChiHaTank::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{	
	//check if it penetrates armor
	//maybe check what kind of damage it is, if bullet play a bullet hit sound
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			iHealth = 0;
			//add explosion when tank dies
			//find mid point of tank and  half the width of the explosion
			TIntFloat lXMidPointOfTank = iTextureObject->ReturnLowerLeftCoordinate().iX + GetWidth() / 2;
			TIntFloat lHalfWidthOf1000KgExplosion = TIntFloat::Convert(EXPLOSION_1000KG_WIDTH / 2);

			//now add them up to find where the explosion needs to be created to make it centered
			TPointIntFloat lExplosionLocation;
			lExplosionLocation.iX = lXMidPointOfTank - lHalfWidthOf1000KgExplosion;//since explosion is not reflected when it is created
			lExplosionLocation.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000Kg::New(lExplosionLocation,BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES_FOR_EXPLODING_TANKS));
			Die(aType);
			iSoundObject->StopSoundChannel();
		}else if(iHealth > 0)
		{
			//play sound
			//iSoundObject->playHitBox();
		}
	}
	//does not penetrate armor, no damage is dealt
}

void CJapanese97ChiHaTank::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapanese97ChiHaTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 2;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapanese97ChiHaTankDriving);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->PlayRepeatHitBox();
	}else if(aNewState == EGroundUnitStateFiring)
	{
		//the firing will actually be done in the shoot method of the object and the animation will be played there as well
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapanese97ChiHaTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateDieing)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapanese97ChiHaTankDestroyed);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = JAPANESE_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}
}

void CJapanese97ChiHaTank::UpdateSpecific()
{
	iTankGun->Update();

	if(iCurrentState == EGroundUnitStateMoving)
	{
		iDustCreator->Update();
	}
}

void CJapanese97ChiHaTank::Shoot()
{
	if(iTankGun->GetIsReadyToShoot())
	{
		//start firing animation
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapanese97ChiHaTankShooting);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));//only plays it once and then returns to default animation

		//shoot weapon
		iTankGun->Shoot();
	}
}

void CJapanese97ChiHaTank::InitilizeJapanese97ChiHaTank()
{
	CJapanese97ChiHaTank::LoadJapanese97ChiHaTankTextures();
	CJapanese97ChiHaTank::CreateJapanese97ChiHaTankCollisionPointsL();
}

void CJapanese97ChiHaTank::RemoveJapanese97ChiHaTank()
{
	CJapanese97ChiHaTank::UnloadJapanese97ChiHaTankTextures();
	CJapanese97ChiHaTank::RemoveJapanese97ChiHaTankCollisionPoints();
}

void CJapanese97ChiHaTank::CreateJapanese97ChiHaTankCollisionPointsL()
{
	CJapanese97ChiHaTank::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_97_CHI_HA_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapanese97ChiHaTank::CollisionPoints[0].XOffset = 20;
	CJapanese97ChiHaTank::CollisionPoints[0].YOffset = 0;
	CJapanese97ChiHaTank::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CJapanese97ChiHaTank::CollisionPoints[1].XOffset = 5;
	CJapanese97ChiHaTank::CollisionPoints[1].YOffset = 22;
	CJapanese97ChiHaTank::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapanese97ChiHaTank::CollisionPoints[2].XOffset = 30;
	CJapanese97ChiHaTank::CollisionPoints[2].YOffset = 29;
	CJapanese97ChiHaTank::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapanese97ChiHaTank::CollisionPoints[3].XOffset = 45;
	CJapanese97ChiHaTank::CollisionPoints[3].YOffset = 52;
	CJapanese97ChiHaTank::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapanese97ChiHaTank::CollisionPoints[4].XOffset = 80;
	CJapanese97ChiHaTank::CollisionPoints[4].YOffset = 47;
	CJapanese97ChiHaTank::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapanese97ChiHaTank::CollisionPoints[5].XOffset = 85;
	CJapanese97ChiHaTank::CollisionPoints[5].YOffset = 32;
	CJapanese97ChiHaTank::CollisionPoints[5].iMovementType = EPointMovementTypeNone;	
	CJapanese97ChiHaTank::CollisionPoints[6].XOffset = 125;
	CJapanese97ChiHaTank::CollisionPoints[6].YOffset = 27;
	CJapanese97ChiHaTank::CollisionPoints[6].iMovementType = EPointMovementTypeNone;	
	CJapanese97ChiHaTank::CollisionPoints[7].XOffset = 115;
	CJapanese97ChiHaTank::CollisionPoints[7].YOffset = 0;
	CJapanese97ChiHaTank::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapanese97ChiHaTank::CollisionPoints[8].XOffset = 20;
	CJapanese97ChiHaTank::CollisionPoints[8].YOffset = 5;
	CJapanese97ChiHaTank::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
}

void CJapanese97ChiHaTank::RemoveJapanese97ChiHaTankCollisionPoints()
{
	if(CJapanese97ChiHaTank::CollisionPoints)
	{
		delete[] CJapanese97ChiHaTank::CollisionPoints;
		CJapanese97ChiHaTank::CollisionPoints = NULL;
	}
}

void CJapanese97ChiHaTank::LoadJapanese97ChiHaTankTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapanese97ChiHaTankDriving);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapanese97ChiHaTankShooting);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapanese97ChiHaTankDestroyed);
}

void CJapanese97ChiHaTank::UnloadJapanese97ChiHaTankTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapanese97ChiHaTankDriving);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapanese97ChiHaTankShooting);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapanese97ChiHaTankDestroyed);
}

void CJapanese97ChiHaTank::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iTankGun->SaveOnDisk(aOutputStream);
	iSmokeCreator->SaveOnDisk(aOutputStream);
}
void CJapanese97ChiHaTank::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iTankGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iSmokeCreator->LoadFromDisk(aReadStream);
}
