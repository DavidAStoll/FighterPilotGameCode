/*
 ============================================================================
 Name		: JapaneseAA20mmMachineCannon.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseAA20mmMachineCannon implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Buildings/CJapaneseAA20mmMachineCannon.h"
#include "includes/game/backend/gameObjects/Weapons/CMachineGun13mm.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CJapaneseAA20mmMachineCannon::CJapaneseAA20mmMachineCannon(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAAArterially(aLocation, aAttributes, JAPANESE_AA_20MM_MACHINE_CANNON_MAX_RANGE, JAPANESE_AA_20MM_MACHINE_CANNON_MIN_RANGE, JAPANESE_AA_20MM_MACHINE_CANNON_MIN_ANGEL, JAPANESE_AA_20MM_MACHINE_CANNON_ROTATION_SPEED, BULLET13MM_SPEED, EXPLOSOPN_NO_EXPLOSION_WIDTH)
{
	iCollideWithPlanes = true;
}

CJapaneseAA20mmMachineCannon::CJapaneseAA20mmMachineCannon(TPoint aLocation, SGameObjectAttributes& aAttributes, TBool aCollideWithPlanes) :
	CAAArterially(aLocation, aAttributes, JAPANESE_AA_20MM_MACHINE_CANNON_MAX_RANGE, JAPANESE_AA_20MM_MACHINE_CANNON_MIN_RANGE, JAPANESE_AA_20MM_MACHINE_CANNON_MIN_ANGEL, JAPANESE_AA_20MM_MACHINE_CANNON_ROTATION_SPEED, BULLET13MM_SPEED, EXPLOSOPN_NO_EXPLOSION_WIDTH)
{
	iCollideWithPlanes = aCollideWithPlanes;
}

CJapaneseAA20mmMachineCannon::~CJapaneseAA20mmMachineCannon()
{
	if(iBase)
	{
		delete iBase;
		iBase = NULL;
	}
	if(i13mmMachineGun)
	{
		delete i13mmMachineGun;
		i13mmMachineGun = NULL;
	}
}

CJapaneseAA20mmMachineCannon* CJapaneseAA20mmMachineCannon::New(TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_20MM_MACHINE_CANNON_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_20MM_MACHINE_CANNON_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA20mmMachineCannon;

	CJapaneseAA20mmMachineCannon* self = new CJapaneseAA20mmMachineCannon(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAA20mmMachineCannon* CJapaneseAA20mmMachineCannon::New(TInt aXLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_20MM_MACHINE_CANNON_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_20MM_MACHINE_CANNON_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA20mmMachineCannon;

	CJapaneseAA20mmMachineCannon* self = new CJapaneseAA20mmMachineCannon(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAA20mmMachineCannon* CJapaneseAA20mmMachineCannon::New(TInt aXLocation, TConflictSide aSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_20MM_MACHINE_CANNON_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_20MM_MACHINE_CANNON_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA20mmMachineCannon;

	CJapaneseAA20mmMachineCannon* self = new CJapaneseAA20mmMachineCannon(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAA20mmMachineCannon* CJapaneseAA20mmMachineCannon::New(TPoint aLocation, TBool aCollideWithPlanes)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_20MM_MACHINE_CANNON_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_20MM_MACHINE_CANNON_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA20mmMachineCannon;

	CJapaneseAA20mmMachineCannon* self = new CJapaneseAA20mmMachineCannon(aLocation, lAttributes, aCollideWithPlanes);
	self->ConstructL();
	return self;
}

void CJapaneseAA20mmMachineCannon::ConstructL()
{
	//create Texture Object
	iBase = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA20mmMachineCannon), JAPANESE_AA_20MM_MACHINE_CANNON_WIDTH, JAPANESE_AA_20MM_MACHINE_CANNON_HEIGHT, iZCor, 0, JAPANESE_AA_20MM_MACHINE_CANNON_BASE_UPPER_S_VALUE, 0, 1);
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA20mmMachineCannonGun), JAPANESE_AA_20MM_MACHINE_CANNON_GUN_WIDTH, JAPANESE_AA_20MM_MACHINE_CANNON_GUN_HEIGHT, iZCor, 0, 1/*Not important*/, 0, JAPANESE_AA_20MM_MACHINE_CANNON_UPPER_T_VALUE);

	iTextureObject->ChangeYCoordinate(TIntFloat::Convert(JAPANESE_AA_20MM_MACHINE_CANNON_RELATIVE_CANON_HEIGHT_TO_BASE));

	iGunCoordinates = iTextureObject->ReturnCurrentFixPoint();

	//create HitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(JAPANESE_AA_20MM_MACHINE_CANNON_WIDTH, JAPANESE_AA_20MM_MACHINE_CANNON_HEIGHT));

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100020;//since we use only one image, only call method once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA20mmMachineCannonGun);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 128;
	lDefaultRequest.Width = JAPANESE_AA_20MM_MACHINE_CANNON_GUN_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = JAPANESE_AA_20MM_MACHINE_CANNON_UPPER_T_VALUE;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//create GunWeapon
	i13mmMachineGun = CMachineGun13mm::New(&iGunCoordinates, &iObjectReflected, &iConflictSide,JAPANESE_AA_20MM_MACHINE_CANNON_GUN_WEAPON_Y_LOCATION, JAPANESE_AA_20MM_MACHINE_CANNON_GUN_WEAPON_X_LOCATION, false, iGunAngel, NULL,ESoundFileId_GroundAA_MachineGun13mm);

	//create AI object
	SWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = JAPANESE_AA_20MM_MACHINE_CANNON_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE;
	lWaitingCase.FrameDelayAfterShot = JAPANESE_AA_20MM_MACHINE_CANNON_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE_AFTER_SHOT;
	iAI = new CAAArteriallyAI(this, lWaitingCase, 4, MACHINEGUN_13MM_FRAMES_BETWEEN_FIRE);

	//used for burning sound when building is destroyed
	iSoundObject = CSoundObject::New(ESoundFileId_Building_Fire_Middle, iHitBox);
}

//--------------- functions -------------------------//


void CJapaneseAA20mmMachineCannon::LoadJapaneseAA20mmMachineCannonTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAA20mmMachineCannon);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAA20mmMachineCannonBroken);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAA20mmMachineCannonGun);
}

void CJapaneseAA20mmMachineCannon::UnloadJapaneseAA20mmMachineCannonTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAA20mmMachineCannon);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAA20mmMachineCannonBroken);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAA20mmMachineCannonGun);
}

void CJapaneseAA20mmMachineCannon::InitilizeJapaneseAA20mmMachineCannon()
{
	CJapaneseAA20mmMachineCannon::LoadJapaneseAA20mmMachineCannonTextures();
}

void CJapaneseAA20mmMachineCannon::RemoveJapaneseAA20mmMachineCannon()
{
	CJapaneseAA20mmMachineCannon::UnloadJapaneseAA20mmMachineCannonTextures();
}

void CJapaneseAA20mmMachineCannon::Draw()
{
	if(!iDoNotDraw)
	{
		if(iAlive)
		{
			//draw Canon first
			iTextureObject->DrawArrays();//uses the currently assianged texture
			//draw Base second
			iBase->DrawArrays();
		}else
		{
			iTextureObject->DrawArrays();
		}
	}
}

void CJapaneseAA20mmMachineCannon::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once
	
	//change texture animation Player to use the destruction image
	SAnimationRequest lDestructionRequest;
	lDestructionRequest.FramesUntilSwitch = 5;
	lDestructionRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA20mmMachineCannonBroken);
	lDestructionRequest.TotalImages = 4;
	lDestructionRequest.TotalLength = 128;
	lDestructionRequest.Width = 32;
	lDestructionRequest.LowerT = 0;
	lDestructionRequest.UpperT = 1;
	iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	iTextureObject->SetHeight(TIntFloat::Convert(JAPANESE_AA_20MM_MACHINE_CANNON_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(JAPANESE_AA_20MM_MACHINE_CANNON_WIDTH));
	iAnimationPlayer->AssignDefaultAnimation(lDestructionRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDestructionRequest));

	BuildingBasicDieRoutine();
	GameObjectMoveIntoBackground();
	iSoundObject->PlayRepeatHitBox();
}

void CJapaneseAA20mmMachineCannon::Update()
{
	i13mmMachineGun->Update();
	iAnimationPlayer->Update();
	iAI->Update();//handles shooting and of the gun
}

void CJapaneseAA20mmMachineCannon::Shoot(TInt aRange)
{
	if(i13mmMachineGun->GetIsReadyToShoot())
	{
		i13mmMachineGun->Shoot();

		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_20MM_MACHINE_CANNON_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA20mmMachineCannonGun);
		lFiringRequest.TotalImages = 4;
		lFiringRequest.TotalLength = 128;
		lFiringRequest.Width = JAPANESE_AA_20MM_MACHINE_CANNON_GUN_WIDTH;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = JAPANESE_AA_20MM_MACHINE_CANNON_UPPER_T_VALUE;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));
	}
}

void CJapaneseAA20mmMachineCannon::RepositionGun()
{

	//the point on which the Canon should be centered on
	TPointIntFloat lMiddlePoint;
	lMiddlePoint.iX = iCoordinates.iX + TIntFloat::Convert(((JAPANESE_AA_20MM_MACHINE_CANNON_WIDTH / 2) + JAPANESE_AA_20MM_MACHINE_CANNON_OFFSET_VALUE_X));
	lMiddlePoint.iY = iCoordinates.iY + TIntFloat::Convert(JAPANESE_AA_20MM_MACHINE_CANNON_RELATIVE_CANON_HEIGHT_TO_BASE);

	TIntFloat lRequiredXDifferenceFromMiddlePoint = (iTextureObject->ReturnLowerRightCoordinate().iX - iTextureObject->ReturnLowerLeftCoordinate().iX) / 2;//half of the length should the difference be from the middle point
	TIntFloat lActualXDiffereneFromMiddlePoint = lMiddlePoint.iX - iTextureObject->ReturnLowerLeftCoordinate().iX;

	TIntFloat lRequiredYDifferenceFromMiddlePoint = (iTextureObject->ReturnLowerRightCoordinate().iY - iTextureObject->ReturnLowerLeftCoordinate().iY) / 2;//half of the length should the difference be from the middle point
	TIntFloat lActualYDiffereneFromMiddlePoint = lMiddlePoint.iY - iTextureObject->ReturnLowerLeftCoordinate().iY;

	TPointIntFloat lAdjustmentsRequired;
	lAdjustmentsRequired.iX = lActualXDiffereneFromMiddlePoint - lRequiredXDifferenceFromMiddlePoint;
	lAdjustmentsRequired.iY = lActualYDiffereneFromMiddlePoint - lRequiredYDifferenceFromMiddlePoint;
	iTextureObject->ChangeXCoordinate(lAdjustmentsRequired.iX);
	iTextureObject->ChangeYCoordinate(lAdjustmentsRequired.iY);

	iGunCoordinates = iTextureObject->ReturnCurrentFixPoint();
}

void CJapaneseAA20mmMachineCannon::DrawAtPositionWithValues(TPointIntFloat& aPosition, TIntFloat lWidth, TIntFloat lHeight)
{
	if(iAlive)
		iBase->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidth, lHeight);
	else
		iTextureObject->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidth, lHeight);
}

void CJapaneseAA20mmMachineCannon::PossibleCollision(CInterval*& aInterval)
{
	//check if captured
	CGameObject* lCaptureObject = CheckIfObjectGotCaptured(aInterval);
	if(lCaptureObject)
	{
		iConflictSide = lCaptureObject->GetConflictSide();
	}

	if(!iCollideWithPlanes)
		return;//don't do collision calculation

	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

		//only collide with alive objects
		if(lCurrentGameObject->IsAlive())//don't compare to anything if already dead
		{
			//collide
			if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
			{ //make sure plane is dead
				SDamage lDamage;
				lDamage.Damage = EXPLOSION_1000KG_DAMAGE;
				lDamage.ArmorPenetration = EXPLOSION_1000KG_ARMOR_PENETRATION;//should kill any object
				lCurrentGameObject->TakeDamage(lDamage, EDamageTypeAAGunPlaneCollision);
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CJapaneseAA20mmMachineCannon::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	AAArteriallyObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCollideWithPlanes);
	i13mmMachineGun->SaveOnDisk(aOutputStream);
	iBase->SaveOnDisk(aOutputStream);
}

void CJapaneseAA20mmMachineCannon::LoadFromDisk(CFileReadStream &aReadStream)
{
	AAArteriallyObjectLoadContentFromDisk(aReadStream);
	iCollideWithPlanes = aReadStream.ReadInt32();
	TWeaponIdentifier lSecondHeavyBomberWeapon = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	i13mmMachineGun->LoadFromDisk(aReadStream);
	iBase->LoadFromDisk(aReadStream);
}
