/*
 ============================================================================
 Name		: GameObject.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGameObject implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/backend/CGameObjectManager.h"


CGameObject::CGameObject(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CDrawObject(aZCor, aTextureObject, aAnimationPlayer)
{
	iCoordinates.iX = TIntFloat::Convert(aLocation.iX);
	iCoordinates.iY = TIntFloat::Convert(aLocation.iY);
	iHealth = aAttributes.Health;
	iMaxHealth = aAttributes.Health;
	iArmor = aAttributes.Armor;
	iAlive = true;
	iRecycleObject = false;
	iAlreadyPutIntoInterval = false;
	iIsMoveableObject = false;
	iObjectReflected = aAttributes.Reflected;
	iHitBox = aAttributes.HitBox;
	iConflictSide = aAttributes.ConflictSide;
	iGameObjectType = aAttributes.ObjectType;
	iGameObjectIdentifier = aAttributes.ObjectIdentifier;
	iSoundObject = NULL;
}

CGameObject::CGameObject(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPointIntFloat aLocation, SGameObjectAttributes& aAttributes) :
	CDrawObject(aZCor, aTextureObject, aAnimationPlayer)
{
	iCoordinates = aLocation;
	iHealth = aAttributes.Health;
	iMaxHealth = aAttributes.Health; //has to be true since all objects start with maximum health
	iArmor = aAttributes.Armor;
	iAlive = true;
	iRecycleObject = false;
	iAlreadyPutIntoInterval = false;
	iIsMoveableObject = false;
	iObjectReflected = aAttributes.Reflected;
	iHitBox = aAttributes.HitBox;
	iConflictSide = aAttributes.ConflictSide;
	iGameObjectType = aAttributes.ObjectType;
	iGameObjectIdentifier = aAttributes.ObjectIdentifier;
	iSoundObject = NULL;
}

CGameObject::~CGameObject()
{
	//need HitBox when SoundObject is deleted, DON'T CHANGE ORDER!
	if(iSoundObject)
	{
		delete iSoundObject;
		iSoundObject = NULL;
	}
	if(iHitBox)
	{
		delete iHitBox;
		iHitBox = NULL;
	}
}

CGameObject*
CGameObject::New(
/*values neeeded for Inheritance*/TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer,
/*new values*/TPoint aLocation, SGameObjectAttributes& aAttributes)
{
	CGameObject* self = new CGameObject(aZCor, aTextureObject, aAnimationPlayer, aLocation, aAttributes);
	self->ConstructL();
	return self;
}

void CGameObject::ConstructL()
{
}

//----------------------functions----------------------

void CGameObject::TakeDamage(const SDamage& aDamage, const TDamageType aType)
{
	//check if it penetrates armor
	if(aDamage.ArmorPenetration > iArmor)
	{
		iHealth -= aDamage.Damage;
		if(iHealth <= 0)
		{
			iHealth = 0;
			Die(aType);
		}
	}
	//does not penetrate armor, no damage is dealt
}

void CGameObject::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once

	GameObjectBasicDieRoutine();
	GameObjectMoveIntoBackground();
}

void CGameObject::GameObjectMoveIntoBackground()
{
	iZCor += PUT_INTO_BACKGROUND_Z_VALUE;
}

void CGameObject::GameObjectBasicDieRoutine()
{
	if(!iAlive)
		return; //can only die once

	//by default set object to dead
	iAlive = false;
	if(iConflictSide == EConflictSideAmerican)
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectLost(iGameObjectIdentifier, this);
	else
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectKilled(iGameObjectIdentifier, this);
}

//stub
void CGameObject::Update()
{
	//do nothing
}

TPointIntFloat&
CGameObject::GetCurrentPosition()
{
	return iCoordinates;
}

TPoint CGameObject::GetCurrentPositionNormilized()
{
	return TPoint(iCoordinates.iX.GetIntInBaseInt(), iCoordinates.iY.GetIntInBaseInt());
}

TBool CGameObject::RecycleObject()
{
	return iRecycleObject;
}

void CGameObject::DrawSimpleHitBoxL()
{
	if(iHitBox)
		iHitBox->DrawSimpleHitBoxL();
}

void CGameObject::DrawAdvancedHitBoxL()
{
	if(iHitBox)
		iHitBox->DrawAdvancedHitBoxL();
}

const TBool& CGameObject::IsReflected()
{
	return iObjectReflected;
}

TIntFloat CGameObject::DistanceBetweenGameObjects(CGameObject* aGameObject)
{
	TIntFloat lObjectPosition1 = iCoordinates.iX;
	TIntFloat lObjectPosition2 = aGameObject->GetCurrentPosition().iX;
	if(lObjectPosition1 < lObjectPosition2)//to the left of 2
	{
		if(iObjectReflected)
		{
			//Icoordiantes returned right corner, therefore no need to add Width of the Object
			if(aGameObject->IsReflected())//must subtract width of second object
				return (lObjectPosition2 - lObjectPosition1) - aGameObject->GetWidth();
			else
				return (lObjectPosition2 - lObjectPosition1);
		}else
		{
			if(aGameObject->IsReflected())
				return (lObjectPosition2 - lObjectPosition1) - GetWidth() - aGameObject->GetWidth();
			else
				return (lObjectPosition2 - lObjectPosition1) - GetWidth();
		}
	}else
	{
		if(aGameObject->IsReflected())
		{
			if(iObjectReflected)
				return (lObjectPosition1 - lObjectPosition2) - GetWidth();
			else
				return (lObjectPosition1 - lObjectPosition2);
		}else if(iObjectReflected)
			return (lObjectPosition1 - lObjectPosition2) - GetWidth() - aGameObject->GetWidth();
		else
			return (lObjectPosition1 - lObjectPosition2) - aGameObject->GetWidth();
	}
}

void CGameObject::PossibleCollision(CInterval*& aInterval)
{
	return;
}

void CGameObject::SetPosition(TPointIntFloat& aPoint)
{
	iCoordinates = aPoint;
	if(iTextureObject)
	{
		iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	}
}
void CGameObject::SetPositionWithoutTextureObjectUpdate(TPointIntFloat& aPoint)
{
	iCoordinates = aPoint;
}

void CGameObject::SetPosition(TPoint& aPoint)
{
	iCoordinates.iX = TIntFloat::Convert(aPoint.iX);
	iCoordinates.iY = TIntFloat::Convert(aPoint.iY);
	if(iTextureObject)
	{
		iTextureObject->SetNewLowerLeftCoordinate(iCoordinates);
	}
}

const TBool& CGameObject::IsAlive()
{
	return iAlive;
}

const TConflictSide& CGameObject::GetConflictSide()
{
	return iConflictSide;
}

TGameObjectType CGameObject::GetGameObjectType()
{
	return iGameObjectType;
}

CHitBox*
CGameObject::GetHitBox()
{
	return iHitBox;
}

void CGameObject::AssignConflictSide(TConflictSide aSide)
{
	iConflictSide = aSide;
}

void CGameObject::TurnLeft()
{
	//need to add anitation somehow,  do later
	if(iObjectReflected)
	{
		iObjectReflected = false;
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

//maybe CGameObject use 180 degree flip of standard bitmap
void CGameObject::TurnRight()
{
	if(!iObjectReflected)
	{
		iObjectReflected = true;
		iTextureObject->ReflectOverYAxis();
		iCoordinates = iTextureObject->ReturnCurrentFixPoint();
	}
}

void CGameObject::SetRecycleObject(TBool aValue)
{
	iRecycleObject = aValue;
}

TGameObjectIdentifier CGameObject::GetGameObjectIdentifier()
{
	return iGameObjectIdentifier;
}

TInt CGameObject::GetMaxHealth()
{
	return iMaxHealth;
}

TBool&
CGameObject::GetAlreadyAddedToInterval()
{
	return iAlreadyPutIntoInterval;
}

TBool&
CGameObject::GetIsMoveableGameObject()
{
	return iIsMoveableObject;
}

TIntFloat CGameObject::GetHeight()
{
	if(iTextureObject != NULL)
		return iTextureObject->GetHeight();
	else
		return TIntFloat::Convert(0);
}

TIntFloat CGameObject::GetWidth()
{
	if(iTextureObject != NULL)
		return iTextureObject->GetWidth();
	else
		return TIntFloat::Convert(0);
}

TInt CGameObject::GetHealth()
{
	return iHealth;
}

TInt CGameObject::DetermineObjectDrawOrder(const CGameObject* aTextureObject1, const CGameObject* aTextureObject2)
{
	//returns 0 if equal, negative if 1 less than 2, or positive if 1 is bigger than 2
	return aTextureObject1->iZCor - aTextureObject2->iZCor;
}

CAircraft* CGameObject::CreateNewAircraft() //creates a new aircraft and positions it on the deck of the carrier
{
	//stub function
	return NULL; //remove warning
}

void CGameObject::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	GameObjectSaveContentToDisk(aOutputStream);
}

void CGameObject::LoadFromDisk(CFileReadStream &aInputStream)
{
	GameObjectLoadContentFromDisk(aInputStream);
}

void CGameObject::GameObjectSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iGameObjectIdentifier);//the first thing that gets written out, so we know what kind of object needs to be created, will be readin by the GameMap function when a game is loaded
	//write down what the memory address of this object was during this life cycle
	TUint lPointerMemoryAddress = (TUint)this;
	aOutputStream.WriteUint32(lPointerMemoryAddress);
	DrawObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iHealth);
	aOutputStream.WriteInt32(iMaxHealth);
	aOutputStream.WriteInt32(iArmor);
	aOutputStream.WriteIntFloat(iCoordinates.iX);
	aOutputStream.WriteIntFloat(iCoordinates.iY);
	aOutputStream.WriteInt32(iObjectReflected);
	aOutputStream.WriteInt32(iRecycleObject);
	aOutputStream.WriteInt32(iAlive);
	aOutputStream.WriteInt32(iIsMoveableObject);
	aOutputStream.WriteInt32(iConflictSide);
	aOutputStream.WriteInt32(iGameObjectType);
	aOutputStream.WriteInt32(iGameObjectIdentifier);
	if(iHitBox)
	{
		aOutputStream.WriteInt32(1);
		iHitBox->SaveOnDisk(aOutputStream);
	}else
		aOutputStream.WriteInt32(0);
	if(iSoundObject)
	{
		aOutputStream.WriteInt32(1);
		iSoundObject->SaveOnDisk(aOutputStream);
	}else
		aOutputStream.WriteInt32(0);
}

void CGameObject::GameObjectLoadContentFromDisk(CFileReadStream &aReadStream)
{
	//load up the value of the memory address that this object had in its past life cycle
	TUint lOldPointerMemoryAddress = aReadStream.ReadUint32();
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddPointerMapping(lOldPointerMemoryAddress, (TUint)this); //add mapping, so that other objects can update their pointers as well
	DrawObjectLoadContentFromDisk(aReadStream);
	iHealth = aReadStream.ReadInt32();
	iMaxHealth = aReadStream.ReadInt32();
	iArmor = aReadStream.ReadInt32();
	iCoordinates.iX = aReadStream.ReadIntFloat();
	iCoordinates.iY = aReadStream.ReadIntFloat();
	iObjectReflected = aReadStream.ReadInt32();
	iRecycleObject = aReadStream.ReadInt32();
	iAlive = aReadStream.ReadInt32();
	iIsMoveableObject = aReadStream.ReadInt32();
	iConflictSide = static_cast<TConflictSide> (aReadStream.ReadInt32());
	iGameObjectType = static_cast<TGameObjectType> (aReadStream.ReadInt32());
	iGameObjectIdentifier = static_cast<TGameObjectIdentifier> (aReadStream.ReadInt32());
	TInt lHitBoxDefined = aReadStream.ReadInt32();
	if(lHitBoxDefined == 1)
		iHitBox->LoadFromDisk(aReadStream);
	TInt lSoundObjectDefined = aReadStream.ReadInt32();
	if(lSoundObjectDefined == 1)
		iSoundObject->LoadFromDisk(aReadStream);
}
