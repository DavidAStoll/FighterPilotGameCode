/*
 ============================================================================
 Name		: JapaneseAAConcrete.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseAAConcrete implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Buildings/CJapaneseAAConcrete.h"
#include "includes/game/backend/gameObjects/Weapons/CGunAA88mm.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"

CJapaneseAAConcrete::CJapaneseAAConcrete(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CAAArterially(aLocation, aAttributes, JAPANESE_AA_CONCRETE_MAX_RANGE, JAPANESE_AA_CONCRETE_MIN_RANGE, JAPANESE_AA_CONCRETE_MIN_ANGEL, JAPANESE_AA_CONCRETE_GUN_ROTATION_SPEED, EXPLOSIVE88MMSHELL_SPEED, EXPLOSION_IN_AIR_1000KG_WIDTH)
{
}

CJapaneseAAConcrete::~CJapaneseAAConcrete()
{
	if(iConcreteBase)
	{
		delete iConcreteBase;
		iConcreteBase = NULL;
	}
	if(iGunAA88mm)
	{
		delete iGunAA88mm;
		iGunAA88mm = NULL;
	}
}

CJapaneseAAConcrete* CJapaneseAAConcrete::New(TPoint aLocation, TConflictSide aSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_CONCRETE_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_CONCRETE_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAAConcrete;

	CJapaneseAAConcrete* self = new CJapaneseAAConcrete(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAAConcrete* CJapaneseAAConcrete::New(TPoint aLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_CONCRETE_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_CONCRETE_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAAConcrete;

	CJapaneseAAConcrete* self = new CJapaneseAAConcrete(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAAConcrete* CJapaneseAAConcrete::New(TInt aXLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_CONCRETE_HEALTH;
	lAttributes.ConflictSide = EConflictSideJapanese;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_CONCRETE_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAAConcrete;

	CJapaneseAAConcrete* self = new CJapaneseAAConcrete(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

CJapaneseAAConcrete* CJapaneseAAConcrete::New(TInt aXLocation, TConflictSide aSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = JAPANESE_AA_CONCRETE_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesBuilding | EObjectTypesBuildingAA;
	lAttributes.Reflected = false;
	lAttributes.Armor = JAPANESE_AA_CONCRETE_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierJapaneseAAConcrete;

	CJapaneseAAConcrete* self = new CJapaneseAAConcrete(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

void CJapaneseAAConcrete::ConstructL()
{
	//create Texture Object
	iConcreteBase = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAAConcrete), JAPANESE_AA_CONCRETE_WIDTH, JAPANESE_AA_CONCRETE_HEIGHT, iZCor, 0, 0.78125, 0, 0.75);

	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingAAConcreteGun), JAPANESE_AA_CONCRETE_WIDTH, JAPANESE_AA_CONCRETE_CANON_HEIGHT, iZCor, 0, 0.1953125, 0, 0.546875);

	iTextureObject->ChangeYCoordinate(TIntFloat::Convert(JAPANESE_AA_RELATIVE_CANON_HEIGHT_TO_BASE));

	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.FramesUntilSwitch = 100005;//since only one image is used, call method only once in a while
	lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAAConcreteGun);
	lDefaultRequest.TotalImages = 1;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.Width = 50;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.546875;
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	iGunCoordinates = iTextureObject->ReturnTopRightCoordinate();

	//create HitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(JAPANESE_AA_CONCRETE_WIDTH, JAPANESE_AA_CONCRETE_HEIGHT));

	//create GunWeapon
	iGunAA88mm = CGunAA88mm::New(&iGunCoordinates, &iGunAngel, 0, -JAPANESE_AA_CONCRETE_WIDTH / 2, &iObjectReflected, &iConflictSide);
	//create AI object
	SWaitingCase lWaitingCase;
	lWaitingCase.FrameDelay = JAPANESE_AA_CONCRETE_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE;
	lWaitingCase.FrameDelayAfterShot = JAPANESE_AA_CONCRETE_AI_FRAME_WAIT_DELAY_UNTIL_IT_DOES_NEXT_SEARCH_FOR_AIRPLANE_AFTER_SHOT;
	iAI = new CAAArteriallyAI(this, lWaitingCase);

	//used for burning sound when building is destroyed
	iSoundObject = CSoundObject::New(ESoundFileId_Building_Fire_Middle, iHitBox);
}

//----------------- functions --------------------//

void CJapaneseAAConcrete::LoadJapaneseAAConcreteTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAAConcrete);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAAConcreteGun);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingAAConcreteBroken);
}

void CJapaneseAAConcrete::UnloadJapaneseAAConcreteTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAAConcrete);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAAConcreteGun);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingAAConcreteBroken);
}

void CJapaneseAAConcrete::InitilizeJapaneseAAConcrete()
{
	CJapaneseAAConcrete::LoadJapaneseAAConcreteTextures();
}

void CJapaneseAAConcrete::RemoveJapaneseAAConcrete()
{
	CJapaneseAAConcrete::UnloadJapaneseAAConcreteTextures();
}

void CJapaneseAAConcrete::Update()
{
	iAnimationPlayer->Update();
	iAI->Update();//handles shooting and of the gun
}

void CJapaneseAAConcrete::Shoot(TInt aRange)
{
	if(iGunAA88mm->GetIsReadyToShoot())
	{
		iGunAA88mm->Shoot(aRange);
		//play animation
		SAnimationRequest lFiringRequest;
		lFiringRequest.FramesUntilSwitch = JAPANESE_AA_CONCRETE_FIRE_SEQUENCE_FRAMES_UNTIL_SWITCH;
		lFiringRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAAConcreteGun);
		lFiringRequest.TotalImages = 5;
		lFiringRequest.TotalLength = 256;
		lFiringRequest.Width = 50;
		lFiringRequest.LowerT = 0;
		lFiringRequest.UpperT = 0.546875;
		iAnimationPlayer->PlayAnimation(new SAnimationRequest(lFiringRequest));
	}
}

void CJapaneseAAConcrete::RepositionGun()
{
	//the point on which the Canon should be centered on
	TPointIntFloat lMiddlePoint;
	lMiddlePoint.iX = iCoordinates.iX + TIntFloat::Convert(JAPANESE_AA_CONCRETE_WIDTH / 2);
	lMiddlePoint.iY = iCoordinates.iY + TIntFloat::Convert(JAPANESE_AA_RELATIVE_CANON_HEIGHT_TO_BASE);

	TIntFloat lRequiredXDifferenceFromMiddlePoint = (iTextureObject->ReturnLowerRightCoordinate().iX - iTextureObject->ReturnLowerLeftCoordinate().iX) / 2;//half of the length should the difference be from the middle point
	TIntFloat lActualXDiffereneFromMiddlePoint = lMiddlePoint.iX - iTextureObject->ReturnLowerLeftCoordinate().iX;

	TIntFloat lRequiredYDifferenceFromMiddlePoint = (iTextureObject->ReturnLowerRightCoordinate().iY - iTextureObject->ReturnLowerLeftCoordinate().iY) / 2;//half of the length should the difference be from the middle point
	TIntFloat lActualYDiffereneFromMiddlePoint = lMiddlePoint.iY - iTextureObject->ReturnLowerLeftCoordinate().iY;

	TPointIntFloat lAdjustmentsRequired;
	lAdjustmentsRequired.iX = lActualXDiffereneFromMiddlePoint - lRequiredXDifferenceFromMiddlePoint;
	lAdjustmentsRequired.iY = lActualYDiffereneFromMiddlePoint - lRequiredYDifferenceFromMiddlePoint;

	iTextureObject->ChangeXCoordinate(lAdjustmentsRequired.iX);
	iTextureObject->ChangeYCoordinate(lAdjustmentsRequired.iY);

	iGunCoordinates = iTextureObject->ReturnTopRightCoordinate();
}

void CJapaneseAAConcrete::Draw()
{
	if(!iDoNotDraw)
	{
		if(iAlive)
		{
			//draw Canon first
			iTextureObject->DrawArrays();//uses the currently assianged texture
			//draw Base second
			iConcreteBase->DrawArrays();
		}else
		{
			iTextureObject->DrawArrays();
		}
	}
}

void CJapaneseAAConcrete::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once
	
	//change texture animation Player to use the destruction image
	SAnimationRequest lDestructionRequest;
	lDestructionRequest.FramesUntilSwitch = 5;
	lDestructionRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingAAConcreteBroken);
	lDestructionRequest.TotalImages = 4;
	lDestructionRequest.TotalLength = 256;
	lDestructionRequest.Width = 50;
	lDestructionRequest.LowerT = 0;
	lDestructionRequest.UpperT = 0.78125;
	iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	iTextureObject->SetHeight(TIntFloat::Convert(JAPANESE_AA_CONCRETE_HEIGHT));
	iTextureObject->SetWidth(TIntFloat::Convert(JAPANESE_AA_CONCRETE_WIDTH));
	iAnimationPlayer->AssignDefaultAnimation(lDestructionRequest);
	iAnimationPlayer->PlayAnimation(new SAnimationRequest(lDestructionRequest));

	BuildingBasicDieRoutine();
	GameObjectMoveIntoBackground();

	iSoundObject->PlayRepeatHitBox();
}

void CJapaneseAAConcrete::DrawAtPositionWithValues(TPointIntFloat& aPosition, TIntFloat lWidth, TIntFloat lHeight)
{
	if(iAlive)
		iConcreteBase->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidth, lHeight);
	else
		iTextureObject->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidth, lHeight);
}

void CJapaneseAAConcrete::PossibleCollision(CInterval*& aInterval)
{
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

	//check if captured
	CGameObject* lCaptureObject = CheckIfObjectGotCaptured(aInterval);
	if(lCaptureObject)
	{
		iConflictSide = lCaptureObject->GetConflictSide();
	}
}

void CJapaneseAAConcrete::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	AAArteriallyObjectSaveContentToDisk(aOutputStream);
	iGunAA88mm->SaveOnDisk(aOutputStream);
	iConcreteBase->SaveOnDisk(aOutputStream);
}

void CJapaneseAAConcrete::LoadFromDisk(CFileReadStream &aReadStream)
{
	AAArteriallyObjectLoadContentFromDisk(aReadStream);
	TWeaponIdentifier lSecondHeavyBomberWeapon = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//is always the same weapon, those don't need to change
	iGunAA88mm->LoadFromDisk(aReadStream);
	iConcreteBase->LoadFromDisk(aReadStream);
}

