/*
 ============================================================================
 Name		: AmericanShermanTank.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanShermanTank implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CAmericanShermanTank::CollisionPoints;

CAmericanShermanTank::CAmericanShermanTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(AMERICAN_SHERMAN_TANK_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(AMERICAN_SHERMAN_TANK_MAX_SPEED), TANKGUN88MM_FRAMES_BETWEEN_FIRE, AMERICAN_SHERMAN_TANK_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CAmericanShermanTank::~CAmericanShermanTank()
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

CAmericanShermanTank* CAmericanShermanTank::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AMERICAN_SHERMAN_TANK_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType =  EObjectTypesArmouredGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_SHERMAN_TANK_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanShermanTank;

	CAmericanShermanTank* self = new CAmericanShermanTank(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanShermanTank::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitShermanTankDriving), AMERICAN_SHERMAN_TANK_WIDTH, AMERICAN_SHERMAN_TANK_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_SHERMAN_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_SHERMAN_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanShermanTank::CollisionPoints);

	SAnimationRequest lDefaultRequest;//not really used
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	//smoke creator
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_SHERMAN_TANK_SMOKE_CREATOR_X, AMERICAN_SHERMAN_TANK_SMOKE_CREATOR_Y,  true, GROUND_UNIT_SMOKE_DEFAULT_COLOUR, GROUND_UNIT_SMOKE_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_SMOKE_DEFAULT_SIZE, GROUND_UNIT_SMOKE_DEFAULT_FRAMES);
	//another smoke creator if tank is moving to create dust
	iDustCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_X, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_Y,  false, GROUND_UNIT_DUST_DEFAULT_COLOUR, GROUND_UNIT_DUST_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_DUST_DEFAULT_SIZE, GROUND_UNIT_DUST_DEFAULT_FRAMES, GROUND_UNIT_DUST_DEFAULT_ANGLE_OFFSET, GROUND_UNIT_DUST_DEFAULT_SPEED, GROUND_UNIT_DUST_DEFAULT_X_EXPO,GROUND_UNIT_DUST_DEFAULT_Y_EXPO);
	//create GunWeapon
	iTankGun = CTankGun80mm::New(false, &iCoordinates, &iAngle, AMERICAN_SHERMAN_TANK_GUN_X_LOCATION, AMERICAN_SHERMAN_TANK_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide);
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

void CAmericanShermanTank::TakeDamage(const SDamage& aDamage, const TDamageType aType)
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

void CAmericanShermanTank::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitShermanTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 2;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitShermanTankDriving);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_SHERMAN_TANK_WIDTH;
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
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitShermanTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateDieing)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitShermanTankDestroyed);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = AMERICAN_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}
}

void CAmericanShermanTank::UpdateSpecific()
{
	iTankGun->Update();

	if(iCurrentState == EGroundUnitStateMoving)
	{
		iDustCreator->Update();
	}
}

void CAmericanShermanTank::Shoot()
{
	if(iTankGun->GetIsReadyToShoot())
	{
		//start firing animation
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 4;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitShermanTankShooting);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));//only plays it once and then returns to default animation

		//shoot weapon
		iTankGun->Shoot();
	}
}

void CAmericanShermanTank::InitilizeAmericanShermanTank()
{
	CAmericanShermanTank::LoadAmericanShermanTankTextures();
	CAmericanShermanTank::CreateAmericanShermanTankCollisionPointsL();
}

void CAmericanShermanTank::RemoveAmericanShermanTank()
{
	CAmericanShermanTank::UnloadAmericanShermanTankTextures();
	CAmericanShermanTank::RemoveAmericanShermanTankCollisionPoints();
}

void CAmericanShermanTank::CreateAmericanShermanTankCollisionPointsL()
{
	CAmericanShermanTank::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_SHERMAN_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanShermanTank::CollisionPoints[0].XOffset = 7;
	CAmericanShermanTank::CollisionPoints[0].YOffset = 0;
	CAmericanShermanTank::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[1].XOffset = 25;
	CAmericanShermanTank::CollisionPoints[1].YOffset = 37;
	CAmericanShermanTank::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[2].XOffset = 42;
	CAmericanShermanTank::CollisionPoints[2].YOffset = 40;
	CAmericanShermanTank::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[3].XOffset = 50;
	CAmericanShermanTank::CollisionPoints[3].YOffset = 52;
	CAmericanShermanTank::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[4].XOffset = 87;
	CAmericanShermanTank::CollisionPoints[4].YOffset = 55;
	CAmericanShermanTank::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[5].XOffset = 91;
	CAmericanShermanTank::CollisionPoints[5].YOffset = 41;
	CAmericanShermanTank::CollisionPoints[5].iMovementType = EPointMovementTypeNone;	
	CAmericanShermanTank::CollisionPoints[6].XOffset = 124;
	CAmericanShermanTank::CollisionPoints[6].YOffset = 35;
	CAmericanShermanTank::CollisionPoints[6].iMovementType = EPointMovementTypeNone;	
	CAmericanShermanTank::CollisionPoints[7].XOffset = 126;
	CAmericanShermanTank::CollisionPoints[7].YOffset = 7;
	CAmericanShermanTank::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[8].XOffset = 66;
	CAmericanShermanTank::CollisionPoints[8].YOffset = 5;
	CAmericanShermanTank::CollisionPoints[8].iMovementType = EPointMovementTypeNone;
	CAmericanShermanTank::CollisionPoints[9].XOffset = 7;
	CAmericanShermanTank::CollisionPoints[9].YOffset = 2;
	CAmericanShermanTank::CollisionPoints[9].iMovementType = EPointMovementTypeNone;
}

void CAmericanShermanTank::RemoveAmericanShermanTankCollisionPoints()
{
	if(CAmericanShermanTank::CollisionPoints)
	{
		delete[] CAmericanShermanTank::CollisionPoints;
		CAmericanShermanTank::CollisionPoints = NULL;
	}
}

void CAmericanShermanTank::LoadAmericanShermanTankTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitShermanTankDriving);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitShermanTankDestroyed);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitShermanTankShooting);
}

void CAmericanShermanTank::UnloadAmericanShermanTankTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitShermanTankDriving);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitShermanTankDestroyed);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitShermanTankShooting);
}

void CAmericanShermanTank::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iTankGun->SaveOnDisk(aOutputStream);
	iSmokeCreator->SaveOnDisk(aOutputStream);
}
void CAmericanShermanTank::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iTankGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iSmokeCreator->LoadFromDisk(aReadStream);
}

