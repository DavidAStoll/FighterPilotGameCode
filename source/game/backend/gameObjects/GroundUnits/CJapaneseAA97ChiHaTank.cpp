/*
 ============================================================================
 Name		: JapaneseAA97ChiHaTank.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseAA97ChiHaTank implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CJapaneseAA97ChiHaTank::CollisionPoints;

CJapaneseAA97ChiHaTank::CJapaneseAA97ChiHaTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(JAPANESE_AA_97_CHI_HA_TANK_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(JAPANESE_AA_97_CHI_HA_TANK_MAX_SPEED), JAPANESE_AA_97_CHI_HA_TANK_MAX_WAIT_FRAMES_BEFORE_FIRING, JAPANESE_AA_97_CHI_HA_TANK_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CJapaneseAA97ChiHaTank::~CJapaneseAA97ChiHaTank()
{
	if(iAAGun)
	{
		delete iAAGun;
		iAAGun = NULL;
	}
	if(iDustCreator)
	{
		delete iDustCreator;
		iDustCreator = NULL;
	}
}

CJapaneseAA97ChiHaTank* CJapaneseAA97ChiHaTank::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_97_CHI_HA_TANK_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesArmouredGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = JAPANESE_AA_97_CHI_HA_TANK_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA97ChiHaTank;

	CJapaneseAA97ChiHaTank* self = new CJapaneseAA97ChiHaTank(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseAA97ChiHaTank::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving), JAPANESE_AA_97_CHI_HA_TANK_WIDTH, JAPANESE_AA_97_CHI_HA_TANK_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, JAPANESE_AA_97_CHI_HA_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, JAPANESE_AA_97_CHI_HA_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CJapaneseAA97ChiHaTank::CollisionPoints);

	SAnimationRequest lDefaultRequest;//not really used
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	//create smoke
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, JAPANESE_AA_97_CHI_HA_TANK_SMOKE_CREATOR_X, JAPANESE_AA_97_CHI_HA_TANK_SMOKE_CREATOR_Y,  true, GROUND_UNIT_SMOKE_DEFAULT_COLOUR, GROUND_UNIT_SMOKE_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_SMOKE_DEFAULT_SIZE, GROUND_UNIT_SMOKE_DEFAULT_FRAMES);
	//another smoke creator if tank is moving to create dust
	iDustCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_X, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_Y,  false, GROUND_UNIT_DUST_DEFAULT_COLOUR, GROUND_UNIT_DUST_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_DUST_DEFAULT_SIZE, GROUND_UNIT_DUST_DEFAULT_FRAMES, GROUND_UNIT_DUST_DEFAULT_ANGLE_OFFSET, GROUND_UNIT_DUST_DEFAULT_SPEED, GROUND_UNIT_DUST_DEFAULT_X_EXPO,GROUND_UNIT_DUST_DEFAULT_Y_EXPO);
	
	iAAGun = CTankAAGun::New(&iCoordinates, &iAngle,179,JAPANESE_AA_97_CHI_HA_TANK_GUN_X_LOCATION, JAPANESE_AA_97_CHI_HA_TANK_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 179, this);
	
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

void CJapaneseAA97ChiHaTank::TakeDamage(const SDamage& aDamage, const TDamageType aType)
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
			TIntFloat lXMidPointOfTank = iTextureObject->ReturnLowerLeftCoordinate().iX + GetWidth()/2;
			TIntFloat lHalfWidthOf1000KgExplosion = TIntFloat::Convert(EXPLOSION_1000KG_WIDTH / 2);
			//now add them up to find where the explosion needs to be created to make it centered
			TPointIntFloat lExplosionLocation;
			lExplosionLocation.iX = lXMidPointOfTank - lHalfWidthOf1000KgExplosion;//since explosion is not reflected when it is created
			lExplosionLocation.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000Kg::New(lExplosionLocation, BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES_FOR_EXPLODING_TANKS));
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

void CJapaneseAA97ChiHaTank::Update()
{
	if(iAlive)// no need to update when object has been destroyed
		iAAGun->Update();
	
	if(iSmokeCreator)
		iSmokeCreator->Update();

	iAnimationPlayer->Update();

	if(iCurrentState == EGroundUnitStateMoving)
	{
		iDustCreator->Update();
	}
}

void CJapaneseAA97ChiHaTank::Draw()
{
	if(iAlive) // no need to draw when object has been destroyed
		iAAGun->Draw();
	
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CJapaneseAA97ChiHaTank::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_AA_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 2;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_AA_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->PlayRepeatHitBox();
	}else if(aNewState == EGroundUnitStateFiring)
	{
		//nothing to do
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = JAPANESE_AA_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateDieing)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDestroyed);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = JAPANESE_AA_97_CHI_HA_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}
}


void CJapaneseAA97ChiHaTank::InitilizeJapaneseAA97ChiHaTank()
{
	CJapaneseAA97ChiHaTank::LoadJapaneseAA97ChiHaTankTextures();
	CJapaneseAA97ChiHaTank::CreateJapaneseAA97ChiHaTankCollisionPointsL();
}

void CJapaneseAA97ChiHaTank::RemoveJapaneseAA97ChiHaTank()
{
	CJapaneseAA97ChiHaTank::UnloadJapaneseAA97ChiHaTankTextures();
	CJapaneseAA97ChiHaTank::RemoveJapaneseAA97ChiHaTankCollisionPoints();
}

void CJapaneseAA97ChiHaTank::CreateJapaneseAA97ChiHaTankCollisionPointsL()
{
	CJapaneseAA97ChiHaTank::CollisionPoints = new SBoundingCollisionPoint[JAPANESE_AA_97_CHI_HA_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CJapaneseAA97ChiHaTank::CollisionPoints[0].XOffset = 20;
	CJapaneseAA97ChiHaTank::CollisionPoints[0].YOffset = 0;
	CJapaneseAA97ChiHaTank::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CJapaneseAA97ChiHaTank::CollisionPoints[1].XOffset = 5;
	CJapaneseAA97ChiHaTank::CollisionPoints[1].YOffset = 18;
	CJapaneseAA97ChiHaTank::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CJapaneseAA97ChiHaTank::CollisionPoints[2].XOffset = 35;
	CJapaneseAA97ChiHaTank::CollisionPoints[2].YOffset = 32;
	CJapaneseAA97ChiHaTank::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CJapaneseAA97ChiHaTank::CollisionPoints[3].XOffset = 37;
	CJapaneseAA97ChiHaTank::CollisionPoints[3].YOffset = 47;
	CJapaneseAA97ChiHaTank::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CJapaneseAA97ChiHaTank::CollisionPoints[4].XOffset = 71;
	CJapaneseAA97ChiHaTank::CollisionPoints[4].YOffset = 44;
	CJapaneseAA97ChiHaTank::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CJapaneseAA97ChiHaTank::CollisionPoints[5].XOffset = 75;
	CJapaneseAA97ChiHaTank::CollisionPoints[5].YOffset = 32;
	CJapaneseAA97ChiHaTank::CollisionPoints[5].iMovementType = EPointMovementTypeNone;	
	CJapaneseAA97ChiHaTank::CollisionPoints[6].XOffset = 120;
	CJapaneseAA97ChiHaTank::CollisionPoints[6].YOffset = 27;
	CJapaneseAA97ChiHaTank::CollisionPoints[6].iMovementType = EPointMovementTypeNone;	
	CJapaneseAA97ChiHaTank::CollisionPoints[7].XOffset = 115;
	CJapaneseAA97ChiHaTank::CollisionPoints[7].YOffset = 0;
	CJapaneseAA97ChiHaTank::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CJapaneseAA97ChiHaTank::CollisionPoints[8].XOffset = 22;
	CJapaneseAA97ChiHaTank::CollisionPoints[8].YOffset = 5;
	CJapaneseAA97ChiHaTank::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
}

void CJapaneseAA97ChiHaTank::RemoveJapaneseAA97ChiHaTankCollisionPoints()
{
	if(CJapaneseAA97ChiHaTank::CollisionPoints)
	{
		delete[] CJapaneseAA97ChiHaTank::CollisionPoints;
		CJapaneseAA97ChiHaTank::CollisionPoints = NULL;
	}
}

void CJapaneseAA97ChiHaTank::LoadJapaneseAA97ChiHaTankTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDestroyed);
}

void CJapaneseAA97ChiHaTank::UnloadJapaneseAA97ChiHaTankTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDriving);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitJapaneseAA97ChiHaTankDestroyed);
}

void CJapaneseAA97ChiHaTank::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iAAGun->SaveOnDisk(aOutputStream);
	iSmokeCreator->SaveOnDisk(aOutputStream);
}
void CJapaneseAA97ChiHaTank::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iAAGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iSmokeCreator->LoadFromDisk(aReadStream);
}
