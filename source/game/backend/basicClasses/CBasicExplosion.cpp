/*
 ============================================================================
 Name		: BasicExplosion.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CBasicExplosion implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CBasicExplosion.h"
#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CBasicExplosion::CBasicExplosion(const TPoint& aLocation, SGameObjectAttributes& aAttributes,	SExplosionAttributes& aExplosionAttributes)
: 	CGameObject(EXPLOSION_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
	{
	iFirstCall = true;
	iCurrentFrame = 0;
	iCurrentCollisionCheckFrame = 0;
	iDamage = aExplosionAttributes.Damage;
	iArmorPenetration = aExplosionAttributes.ArmorPenetration;
	iFramesForExplosion = aExplosionAttributes.FramesForExplosion;
	iOnlyKillDuringFirstFrame = aExplosionAttributes.KillOnlyDuringFirstFrame;
	iObjectTypesItCanDamage = aExplosionAttributes.ObjectTypesItCanDamage;

	iObjectsAlreadyDamaged = CPointerArray<CGameObject>::New();
	}

CBasicExplosion::CBasicExplosion(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes,	SExplosionAttributes& aExplosionAttributes)
: 	CGameObject(EXPLOSION_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
	{
	iFirstCall = true;
	iCurrentFrame = 0;
	iCurrentCollisionCheckFrame = 0;
	iDamage = aExplosionAttributes.Damage;
	iArmorPenetration = aExplosionAttributes.ArmorPenetration;
	iFramesForExplosion = aExplosionAttributes.FramesForExplosion;
	iOnlyKillDuringFirstFrame = aExplosionAttributes.KillOnlyDuringFirstFrame;
	iObjectTypesItCanDamage = aExplosionAttributes.ObjectTypesItCanDamage;

	iObjectsAlreadyDamaged = CPointerArray<CGameObject>::New();
	}

CBasicExplosion::~CBasicExplosion()
	{
		if(iObjectsAlreadyDamaged != NULL)
		{
			iObjectsAlreadyDamaged->Clear();
			delete iObjectsAlreadyDamaged;
			iObjectsAlreadyDamaged = NULL;
		}
	}

//-------------------------------- FUNCTIONS ---------------------------------//

void CBasicExplosion::Update()
{
	iCurrentFrame++;
	if(iAnimationPlayer)
		iAnimationPlayer->Update();

	if(iCurrentFrame == iFramesForExplosion)
	{ // explosion sequence is over, the object should be removed from the game now
		iAlive = false;
		iRecycleObject = true;
		iDoNotDraw = true;
	}
}

TBool CBasicExplosion::ObjectHasNotBeenDamagedBefore(CGameObject* aGameObjects)
{
	for(TInt lIndex = 0; lIndex < iObjectsAlreadyDamaged->GetCount(); lIndex++)
	{
		if(aGameObjects == iObjectsAlreadyDamaged->Get(lIndex))
			return false;
	}

	return true;//none of the pointers matched, thus hasn't been damaged yet
}

void CBasicExplosion::PossibleCollision(CInterval*& aInterval)
{
	if(iOnlyKillDuringFirstFrame)
	{
		if(iFirstCall)
		{
			iFirstCall = false;
			iCurrentCollisionCheckFrame = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetCurrentFrame();
		}else
		{//to avoid that an explosion does more than in one Frame damage
			if(iCurrentCollisionCheckFrame != CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetCurrentFrame())
				return;
		}
	}
	//go through all objects and check if care to collide with them
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(iObjectTypesItCanDamage);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);
		//only collide with alive objects
		if(lCurrentGameObject->IsAlive() && iAlive)//don't compare to anything if already dead
		{
			//collide
			if(ObjectHasNotBeenDamagedBefore(lCurrentGameObject) && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
			{
				SDamage lDamage;
				lDamage.Damage = iDamage;
				lDamage.ArmorPenetration = iArmorPenetration;//should kill any object
				lCurrentGameObject->TakeDamage(lDamage, EDamageTypeExplosion);
				iObjectsAlreadyDamaged->Append(lCurrentGameObject);
			}
		}
	}

	//clean up
	delete lGameObjects;
}

void CBasicExplosion::BasicExplosionSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	GameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCurrentFrame);
	aOutputStream.WriteInt32(iCurrentCollisionCheckFrame);
	aOutputStream.WriteInt32(iFirstCall);
	aOutputStream.WriteInt32(iDamage);
	aOutputStream.WriteInt32(iArmorPenetration);
	aOutputStream.WriteInt32(iFramesForExplosion);
	aOutputStream.WriteInt32(iOnlyKillDuringFirstFrame);
	aOutputStream.WriteInt32(iObjectTypesItCanDamage);
	CMap::SaveDynamicArray((CPointerArray<void>*) iObjectsAlreadyDamaged, aOutputStream);
}

void CBasicExplosion::BasicExplosionLoadContentFromDisk(CFileReadStream &aReadStream)
{
	GameObjectLoadContentFromDisk(aReadStream);
	iCurrentFrame = aReadStream.ReadInt32();
	iCurrentCollisionCheckFrame = aReadStream.ReadInt32();
	iFirstCall = aReadStream.ReadInt32();
	iDamage = aReadStream.ReadInt32();
	iArmorPenetration = aReadStream.ReadInt32();
	iFramesForExplosion = aReadStream.ReadInt32();
	iOnlyKillDuringFirstFrame = aReadStream.ReadInt32();
	iObjectTypesItCanDamage = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iObjectsAlreadyDamaged, aReadStream);
}

void CBasicExplosion::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicExplosionSaveContentToDisk(aOutputStream);
}

void CBasicExplosion::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicExplosionLoadContentFromDisk(aReadStream);
}

