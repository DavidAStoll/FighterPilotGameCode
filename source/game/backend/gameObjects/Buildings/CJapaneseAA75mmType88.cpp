/*
 ============================================================================
 Name		: JapaneseAA75mmType88.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseAA75mmType88 implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Buildings/CJapaneseAA75mmType88.h"
#include "includes/game/backend/gameObjects/Weapons/CGunAA75mm.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/core/utility/CRandomGenerator.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CJapaneseAA75mmType88::CJapaneseAA75mmType88(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAAArterially(aLocation, aAttributes, JAPANESE_AA_75MM_TYPE88_MAX_RANGE, JAPANESE_AA_75MM_TYPE88_MIN_RANGE, JAPANESE_AA_75MM_TYPE88_MIN_ANGEL, JAPANESE_AA_75MM_TYPE88_GUN_ROTATION_SPEED, EXPLOSIVE75MMSHELL_SPEED, EXPLOSION_IN_AIR_500KG_WIDTH)
{
	iCollideWithPlanes = true;
}

CJapaneseAA75mmType88::CJapaneseAA75mmType88(TPoint aLocation, SGameObjectAttributes& aAttributes, TBool aCollideWithPlanes) :
	CAAArterially(aLocation, aAttributes, JAPANESE_AA_75MM_TYPE88_MAX_RANGE, JAPANESE_AA_75MM_TYPE88_MIN_RANGE, JAPANESE_AA_75MM_TYPE88_MIN_ANGEL, JAPANESE_AA_75MM_TYPE88_GUN_ROTATION_SPEED, EXPLOSIVE75MMSHELL_SPEED, EXPLOSION_IN_AIR_500KG_WIDTH)
{
	iCollideWithPlanes = aCollideWithPlanes;
}

CJapaneseAA75mmType88::~CJapaneseAA75mmType88()
{
	if(iBase)
	{
		delete iBase;
		iBase = NULL;
	}
	if(iGunAA75mm)
	{
		delete iGunAA75mm;
		iGunAA75mm = NULL;
	}
}

CJapaneseAA75mmType88* CJapaneseAA75mmType88::New(TPoint aLocation, TConflictSide aSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_75MM_TYPE88_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_75MM_TYPE88_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA75mmType88;

	CJapaneseAA75mmType88* self = new CJapaneseAA75mmType88(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAA75mmType88* CJapaneseAA75mmType88::New(TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_75MM_TYPE88_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_75MM_TYPE88_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA75mmType88;

	CJapaneseAA75mmType88* self = new CJapaneseAA75mmType88(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAA75mmType88* CJapaneseAA75mmType88::New(TInt aXLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_75MM_TYPE88_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_75MM_TYPE88_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA75mmType88;

	CJapaneseAA75mmType88* self = new CJapaneseAA75mmType88(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}
CJapaneseAA75mmType88* CJapaneseAA75mmType88::New(TInt aXLocation, TConflictSide aSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_75MM_TYPE88_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_75MM_TYPE88_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA75mmType88;

	CJapaneseAA75mmType88* self = new CJapaneseAA75mmType88(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAA75mmType88* CJapaneseAA75mmType88::New(TPoint aLocation, TBool aCollideWithPlanes)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_75MM_TYPE88_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_75MM_TYPE88_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAA75mmType88;

	CJapaneseAA75mmType88* self = new CJapaneseAA75mmType88(aLocation, lAttributes, aCollideWithPlanes);
	self->ConstructL();
	return self;
}

void CJapaneseAA75mmType88::ConstructL()
{
	//create Texture Object
	iBase = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA75mmType88), JAPANESE_AA_75MM_TYPE88_WIDTH, JAPANESE_AA_75MM_TYPE88_HEIGHT, iZCor, 0, JAPANESE_AA_75MM_TYPE88_BASE_UPPER_S_VALUE, 0, 1);

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA75mmType88Gun), JAPANESE_AA_75MM_TYPE88_WIDTH, JAPANESE_AA_75MM_TYPE88_CANON_HEIGHT, iZCor, 0, 0.15625/*Not important*/, 0, JAPANESE_AA_75MM_TYPE88_UPPER_T_VALUE);

	iTextureObject->ChangeYCoordinate(TIntFloat::Convert(JAPANESE_AA_75MM_TYPE88_RELATIVE_CANON_HEIGHT_TO_BASE));
	iTextureObject->ChangeXCoordinate(TIntFloat::Convert(JAPANESE_AA_75MM_TYPE88_CANON_OFFSET_VALUE_X));

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100000;//since image never changes, call the method only once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA75mmType88Gun);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 512;
	lDefaultRequest.Width = JAPANESE_AA_75MM_TYPE88_WIDTH;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = JAPANESE_AA_75MM_TYPE88_UPPER_T_VALUE;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	iGunCoordinates = iTextureObject->ReturnCurrentFixPoint();

	//create HitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(JAPANESE_AA_75MM_TYPE88_WIDTH, JAPANESE_AA_75MM_TYPE88_HIT_BOX_HEIGHT));

	//create GunWeapon
	iGunAA75mm = CGunAA75mm::New(&iGunCoordinates, &iGunAngel, JAPANESE_AA_75MM_TYPE88_CANON_HEIGHT, JAPANESE_AA_75MM_TYPE88_WIDTH / 2 + JAPANESE_AA_75MM_TYPE88_CANON_OFFSET_VALUE_X, &iObjectReflected, &iConflictSide);
	//create AI object
	SWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = JAPANESE_AA_75MM_TYPE88_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE;
	lWaitingCase.FrameDelayAfterShot = JAPANESE_AA_75MM_TYPE88_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE_AFTER_SHOT;
	iAI = new CAAArteriallyAI(this, lWaitingCase, 4, GUNAA75MM_FRAMES_BETWEEN_FIRE);

	//used for burning sound when building is destroyed
	iSoundObject = CSoundObject::New(ESoundFileId_Building_Fire_Middle, iHitBox);
}

//--------------- functions -------------------------//


void CJapaneseAA75mmType88::LoadJapaneseAA75mmType88Textures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAA75mmType88);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAA75mmType88Gun);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAA75mmType88Broken);
}

void CJapaneseAA75mmType88::UnloadJapaneseAA75mmType88Textures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAA75mmType88);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAA75mmType88Gun);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAA75mmType88Broken);
}

void CJapaneseAA75mmType88::InitilizeJapaneseAA75mmType88()
{
	CJapaneseAA75mmType88::LoadJapaneseAA75mmType88Textures();
}

void CJapaneseAA75mmType88::RemoveJapaneseAA75mmType88()
{
	CJapaneseAA75mmType88::UnloadJapaneseAA75mmType88Textures();
}

void CJapaneseAA75mmType88::Draw()
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

void CJapaneseAA75mmType88::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once
	
	//change texture animation Player to use the destruction image
	SAnimationRequest lDestructionRequest;
	lDestructionRequest.FramesUntilSwitch = 5;
	lDestructionRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA75mmType88Broken);
	lDestructionRequest.TotalImages = 5;
	lDestructionRequest.TotalLength = 512;
	lDestructionRequest.Width = 80;
	lDestructionRequest.LowerT = 0;
	lDestructionRequest.UpperT = 1;
	iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	iTextureObject->SetHeight(TIntFloat::Convert(JAPANESE_AA_75MM_TYPE88_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(JAPANESE_AA_75MM_TYPE88_WIDTH));
	iAnimationPlayer->AssignDefaultAnimation(lDestructionRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDestructionRequest));

	BuildingBasicDieRoutine();
	GameObjectMoveIntoBackground();

	iSoundObject->PlayRepeatHitBox();
}

void CJapaneseAA75mmType88::Update()
{
	iGunAA75mm->Update();
	iAnimationPlayer->Update();
	iAI->Update();//handles shooting and of the gun
}

void CJapaneseAA75mmType88::Shoot(TInt aRange)
{
	if(iGunAA75mm->GetIsReadyToShoot())
	{
		SRandomFiringOffset lOffset;
		lOffset.AngleOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_LOWER_BOUND, JAPANESE_AA_75MM_TYPE88_ANGLE_RANDOM_OFFSET_UPPER_BOUND);
		lOffset.RangeOffset = CGame::Game->iRandomGenerator->GetRandomNumberTInt(JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_LOWER_BOUND, JAPANESE_AA_75MM_TYPE88_RANGE_RANDOM_OFFSET_UPPER_BOUND);
		iGunAA75mm->Shoot(aRange + lOffset.RangeOffset, lOffset.AngleOffset);

		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_75MM_TYPE88_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAA75mmType88Gun);
		lFiringRequest.TotalImages = 5;
		lFiringRequest.TotalLength = 512;
		lFiringRequest.Width = JAPANESE_AA_75MM_TYPE88_WIDTH;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = JAPANESE_AA_75MM_TYPE88_UPPER_T_VALUE;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));
	}
}

void CJapaneseAA75mmType88::RepositionGun()
{
	//the point on which the Canon should be centered on
	TPointIntFloat lMiddlePoint;
	lMiddlePoint.iX = iCoordinates.iX + TIntFloat::Convert(((JAPANESE_AA_75MM_TYPE88_WIDTH / 2) + JAPANESE_AA_75MM_TYPE88_CANON_OFFSET_VALUE_X));
	lMiddlePoint.iY = iCoordinates.iY + TIntFloat::Convert(JAPANESE_AA_75MM_TYPE88_RELATIVE_CANON_HEIGHT_TO_BASE);

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

void CJapaneseAA75mmType88::DrawAtPositionWithValues(TPointIntFloat& aPosition, TIntFloat lWidth, TIntFloat lHeight)
{
	if(iAlive)
		iBase->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidth, lHeight);
	else
		iTextureObject->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidth, lHeight);
}

void CJapaneseAA75mmType88::PossibleCollision(CInterval*& aInterval)
{
	//check if captured
	CGameObject* lCaptureObject = CheckIfObjectGotCaptured(aInterval);

	if(lCaptureObject)
	{
		iConflictSide = lCaptureObject->GetConflictSide();
	}

	if(!iCollideWithPlanes)
		return;//don't do collision calculation

	//go through all objects and check if care to collide with them
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

void CJapaneseAA75mmType88::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	AAArteriallyObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCollideWithPlanes);
	iGunAA75mm->SaveOnDisk(aOutputStream);
	iBase->SaveOnDisk(aOutputStream);
}

void CJapaneseAA75mmType88::LoadFromDisk(CFileReadStream &aReadStream)
{
	AAArteriallyObjectLoadContentFromDisk(aReadStream);
	iCollideWithPlanes = aReadStream.ReadInt32();
	TWeaponIdentifier lSecondHeavyBomberWeapon = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iGunAA75mm->LoadFromDisk(aReadStream);
	iBase->LoadFromDisk(aReadStream);
}
