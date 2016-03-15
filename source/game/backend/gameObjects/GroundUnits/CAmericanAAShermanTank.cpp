/*
 ============================================================================
 Name		: AmericanAAShermanTank.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanAAShermanTank implementation
 ============================================================================
 */


#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

SBoundingCollisionPoint* CAmericanAAShermanTank::CollisionPoints;

CAmericanAAShermanTank::CAmericanAAShermanTank(TPoint aLocation, TIntFloat aSpeed, SGameObjectAttributes& aAttributes) :
	CGroundUnit(AMERICAN_AA_SHERMAN_TANK_Z_COORDINATE, NULL, NULL, aLocation, aAttributes, TIntFloat::Convert(AMERICAN_AA_SHERMAN_TANK_MAX_SPEED), AMERICAN_AA_SHERMAN_TANK_MAX_WAIT_FRAMES_BEFORE_FIRING, AMERICAN_AA_SHERMAN_TANK_FRAMES_NEEDED_FOR_FIRING_ANIMATION)
{
	iSpeed = aSpeed;
	iAngle = 180; //by default is looking left
	iPixelsPerMoveX = iPixelsPerMoveX = iSpeed * CMath::GraphicsCosTable(iAngle);
}

CAmericanAAShermanTank::~CAmericanAAShermanTank()
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

CAmericanAAShermanTank* CAmericanAAShermanTank::New(TInt aXLocation, TIntFloat aSpeed, TBool aReflected)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = AMERICAN_AA_SHERMAN_TANK_HEALTH;
	lAttributes.ConflictSide = EConflictSideAmerican;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesArmouredGroundUnit;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = AMERICAN_AA_SHERMAN_TANK_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierAmericanAAShermanTank;

	CAmericanAAShermanTank* self = new CAmericanAAShermanTank(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), aSpeed, lAttributes);
	self->ConstructL();
	return self;
}

void CAmericanAAShermanTank::ConstructL()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankDriving), AMERICAN_AA_SHERMAN_TANK_WIDTH, AMERICAN_AA_SHERMAN_TANK_HEIGHT, iZCor, 0, 1, 0, 1);

	//need to create bounding Hitbox
	iHitBox = CHitBox::New(&iCoordinates, AMERICAN_AA_SHERMAN_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, AMERICAN_AA_SHERMAN_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS, &iPixelsPerMoveX, &iPixelsPerMoveY, &iObjectReflected, &iAngle, CAmericanAAShermanTank::CollisionPoints);
	SAnimationRequest lDefaultRequest;//not really used
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);
	iAAGun = CTankAAGun::New(&iCoordinates, &iAngle,179,AMERICAN_SHERMAN_TANK_AA_GUN_X_LOCATION, AMERICAN_SHERMAN_TANK_AA_GUN_Y_LOCATION, &iObjectReflected, &iConflictSide, &iSpeed, 0, 179, this);
	//smoke creator if object got destroyed
	iSmokeCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_AA_SHERMAN_TANK_SMOKE_CREATOR_X, AMERICAN_AA_SHERMAN_TANK_SMOKE_CREATOR_Y,  true, GROUND_UNIT_SMOKE_DEFAULT_COLOUR, GROUND_UNIT_SMOKE_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_SMOKE_DEFAULT_SIZE, GROUND_UNIT_SMOKE_DEFAULT_FRAMES);
	//another smoke creator if tank is moving to create dust
	iDustCreator = CGeneralSmokeCreator::New(this, &iAngle, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_X, AMERICAN_AA_SHERMAN_TANK_DUST_CREATOR_Y,  false, GROUND_UNIT_DUST_DEFAULT_COLOUR, GROUND_UNIT_DUST_STARTS_AFTER_HEALTH_LEFT, GROUND_UNIT_DUST_DEFAULT_SIZE, GROUND_UNIT_DUST_DEFAULT_FRAMES, GROUND_UNIT_DUST_DEFAULT_ANGLE_OFFSET, GROUND_UNIT_DUST_DEFAULT_SPEED, GROUND_UNIT_DUST_DEFAULT_X_EXPO,GROUND_UNIT_DUST_DEFAULT_Y_EXPO);
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

void CAmericanAAShermanTank::TakeDamage(const SDamage& aDamage, const TDamageType aType)
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

void CAmericanAAShermanTank::Update()
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

void CAmericanAAShermanTank::Draw()
{
	if(iAlive) // no need to draw when object has been destroyed
		iAAGun->Draw();
	
	iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CAmericanAAShermanTank::UpdateCurrentState(TGroundUnitStates aNewState)
{
	if(aNewState == EGroundUnitStateStanding)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_AA_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateMoving)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 2;
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankDriving);
		lDefaultRequest.TotalImages = 4;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_AA_SHERMAN_TANK_WIDTH;
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
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankDriving);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 512;
		lDefaultRequest.Width = AMERICAN_AA_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}else if(aNewState == EGroundUnitStateDieing)
	{
		SAnimationRequest lDefaultRequest;
		lDefaultRequest.FramesUntilSwitch = 100;//since we use only one image, only call method once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdGroundUnitAAShermanTankDestroyed);
		lDefaultRequest.TotalImages = 1;
		lDefaultRequest.TotalLength = 128;
		lDefaultRequest.Width = AMERICAN_AA_SHERMAN_TANK_WIDTH;
		lDefaultRequest.LowerT = 0;
		lDefaultRequest.UpperT = 0.890625;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDefaultRequest));
		iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
		iSoundObject->StopSoundChannel();
	}
}


void CAmericanAAShermanTank::InitilizeAmericanAAShermanTank()
{
	CAmericanAAShermanTank::LoadAmericanAAShermanTankTextures();
	CAmericanAAShermanTank::CreateAmericanAAShermanTankCollisionPointsL();
}

void CAmericanAAShermanTank::RemoveAmericanAAShermanTank()
{
	CAmericanAAShermanTank::UnloadAmericanAAShermanTankTextures();
	CAmericanAAShermanTank::RemoveAmericanAAShermanTankCollisionPoints();
}

void CAmericanAAShermanTank::CreateAmericanAAShermanTankCollisionPointsL()
{
	CAmericanAAShermanTank::CollisionPoints = new SBoundingCollisionPoint[AMERICAN_AA_SHERMAN_TANK_NUMBER_OF_ORIGINIAL_HITBOX_POINTS];
	CAmericanAAShermanTank::CollisionPoints[0].XOffset = 10;
	CAmericanAAShermanTank::CollisionPoints[0].YOffset = 0;
	CAmericanAAShermanTank::CollisionPoints[0].iMovementType = EPointMovementTypeNone;
	CAmericanAAShermanTank::CollisionPoints[1].XOffset = 24;
	CAmericanAAShermanTank::CollisionPoints[1].YOffset = 37;
	CAmericanAAShermanTank::CollisionPoints[1].iMovementType = EPointMovementTypeNone;
	CAmericanAAShermanTank::CollisionPoints[2].XOffset = 50;
	CAmericanAAShermanTank::CollisionPoints[2].YOffset = 52;
	CAmericanAAShermanTank::CollisionPoints[2].iMovementType = EPointMovementTypeNone;
	CAmericanAAShermanTank::CollisionPoints[3].XOffset = 87;
	CAmericanAAShermanTank::CollisionPoints[3].YOffset = 49;
	CAmericanAAShermanTank::CollisionPoints[3].iMovementType = EPointMovementTypeNone;
	CAmericanAAShermanTank::CollisionPoints[4].XOffset = 100;
	CAmericanAAShermanTank::CollisionPoints[4].YOffset = 37;
	CAmericanAAShermanTank::CollisionPoints[4].iMovementType = EPointMovementTypeNone;
	CAmericanAAShermanTank::CollisionPoints[5].XOffset = 125;
	CAmericanAAShermanTank::CollisionPoints[5].YOffset = 32;
	CAmericanAAShermanTank::CollisionPoints[5].iMovementType = EPointMovementTypeNone;	
	CAmericanAAShermanTank::CollisionPoints[6].XOffset = 114;
	CAmericanAAShermanTank::CollisionPoints[6].YOffset = 0;
	CAmericanAAShermanTank::CollisionPoints[6].iMovementType = EPointMovementTypeNone;	
	CAmericanAAShermanTank::CollisionPoints[7].XOffset = 10;
	CAmericanAAShermanTank::CollisionPoints[7].YOffset = 7;
	CAmericanAAShermanTank::CollisionPoints[7].iMovementType = EPointMovementTypeNone;
}

void CAmericanAAShermanTank::RemoveAmericanAAShermanTankCollisionPoints()
{
	if(CAmericanAAShermanTank::CollisionPoints)
	{
		delete[] CAmericanAAShermanTank::CollisionPoints;
		CAmericanAAShermanTank::CollisionPoints = NULL;
	}
}

void CAmericanAAShermanTank::LoadAmericanAAShermanTankTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAAShermanTankDriving);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAAShermanTankDestroyed);
	CGame::Game->iImageStore->LoadImage(EImageIdGroundUnitAAShermanTankAAGun);
}

void CAmericanAAShermanTank::UnloadAmericanAAShermanTankTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAAShermanTankDriving);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAAShermanTankDestroyed);
	CGame::Game->iImageStore->UnloadImage(EImageIdGroundUnitAAShermanTankAAGun);
}

void CAmericanAAShermanTank::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GroundUnitObjectSaveContentToDisk(aOutputStream);
	iAAGun->SaveOnDisk(aOutputStream);
	iSmokeCreator->SaveOnDisk(aOutputStream);
}
void CAmericanAAShermanTank::LoadFromDisk(CFileReadStream &aReadStream)
{
	GroundUnitObjectLoadContentFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iAAGun->LoadFromDisk(aReadStream);
	static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not needed
	iSmokeCreator->LoadFromDisk(aReadStream);
}
