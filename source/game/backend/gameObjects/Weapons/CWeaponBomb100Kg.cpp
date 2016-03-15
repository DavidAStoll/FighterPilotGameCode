/*
 ============================================================================
 Name		: WeaponBomb100Kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBomb100Kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Weapons/CWeaponBomb100Kg.h"
#include "includes/game/backend/gameObjects/Projectiles/CBomb100kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponBomb100Kg::CWeaponBomb100Kg(TInt aAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TBool aDoNotDraw, const TIntFloat* aObjectSpeed) :
	CWeaponBomb(aAmmunition, EWeaponIdentifier100kgBomb, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_BOMB_100KG_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	iCreateAnotherWeapon = true;
	iExtraWeapon = NULL;
}

CWeaponBomb100Kg::CWeaponBomb100Kg(TInt aAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TBool aDoNotDraw, const TIntFloat* aObjectSpeed, TBool aCreateAnotherWeapon) :
	CWeaponBomb(aAmmunition, EWeaponIdentifier100kgBomb, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, WEAPON_BOMB_100KG_FRAMES_BETWEEN_FIRE, aDoNotDraw, aObjectSpeed)
{
	iCreateAnotherWeapon = aCreateAnotherWeapon;
	iExtraWeapon = NULL;
}

CWeaponBomb100Kg::~CWeaponBomb100Kg()
{
	if(iExtraWeapon)
	{
		delete iExtraWeapon;
		iExtraWeapon = NULL;
	}
}

CWeaponBomb100Kg* CWeaponBomb100Kg::New(TInt aAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TBool aDoNotDraw, const TIntFloat* aObjectSpeed)
{
	CWeaponBomb100Kg* self = new CWeaponBomb100Kg(aAmmunition, aLocation, aAngel, aUpSideDown, aXOffset, aYOffset, aReflected, aConflictSide, aDoNotDraw, aObjectSpeed);
	self->ConstructL();
	return self;
}

void CWeaponBomb100Kg::ConstructL()
{

	TPointIntFloat lFloatPoint = GetRelativeWeaponPosition();
	TPoint lLocation;
	lLocation.iX = lFloatPoint.iX.GetIntInBaseInt();
	lLocation.iY = lFloatPoint.iY.GetIntInBaseInt();

	iTextureObject = CTextureObject::New(lLocation, &CGame::Game->iImageStore->GetImage(EImageIdBomb100KgBomb), BOMB_100KG_WIDTH, BOMB_100KG_HEIGHT, NOT_IMPORTANT_Z_VALUE, 0, 0.59375, 0, 0.75);

	//create one extra weapon and then set it though that no other weapons are created by setting the boolean value
	if(iCreateAnotherWeapon)
	{
		iExtraWeapon = new CWeaponBomb100Kg(iAmmunitionLeft, iCoordinate, iAngel, iIsUpSideDown, iXOffset + WEAPON_BOMB_100KG_EXTRA_WEAPON_X_OFFSET, iYOffset, iWeaponReflectedOverYAxis, iConflictSide, iDoNotDraw, iSpeed, false);
		iExtraWeapon->ConstructL();
	}

}

//---------------------- functions ----------------------//


void CWeaponBomb100Kg::ResetAmmunitionToMaxValue()
{
	iAmmunitionLeft = iMaxAmmunitionLeft;
	if(iCreateAnotherWeapon)
		iExtraWeapon->ResetAmmunitionToMaxValue();
}

void CWeaponBomb100Kg::SetAmmunitionToValue(TInt aAmount)
{
	iAmmunitionLeft = aAmount;
	if(iCreateAnotherWeapon)
		iExtraWeapon->SetAmmunitionToValue(aAmount);
}

void CWeaponBomb100Kg::Draw()
{
	if(!iDoNotDraw && !iIsDisabled && iTextureObject && iAmmunitionLeft > 0)
	{
		iTextureObject->SetNewLowerLeftCoordinate(GetRelativeWeaponPosition());//need to adjust Texture to new position
		//need to rotate Texture Object if we are not at 180 degrees
		if(*iWeaponReflectedOverYAxis)
		{
			if(!iTextureObject->IsReflectedOverYAxis())
				iTextureObject->ReflectOverYAxis();

			TInt lRotateAngel = *iAngel;
			iTextureObject->RotateVertexes(lRotateAngel);
			//need to reset the image back to the original position, since after rotating the image, the image will not be at its default position
			TPointIntFloat lTextureLowerCorner = iTextureObject->ReturnCurrentFixPoint();
			TPointIntFloat lRelativeWeaponPosition = GetRelativeWeaponPosition();
			lTextureLowerCorner.iX = -lTextureLowerCorner.iX + lRelativeWeaponPosition.iX;
			lTextureLowerCorner.iY = -lTextureLowerCorner.iY + lRelativeWeaponPosition.iY;
			iTextureObject->ChangeXCoordinate(lTextureLowerCorner.iX);
			iTextureObject->ChangeYCoordinate(lTextureLowerCorner.iY);
		}else
		{
			if(iTextureObject->IsReflectedOverYAxis())
				iTextureObject->ReflectOverYAxis();

			TInt lRotateAngel = -180 + *iAngel;//since 180 degrees is the normal orientation of the image
			iTextureObject->RotateVertexes(lRotateAngel);
			//need to reset the image back to the original position, since after rotating the image, the image will not be at its default position
			TPointIntFloat lTextureLowerCorner = iTextureObject->ReturnCurrentFixPoint();
			TPointIntFloat lRelativeWeaponPosition = GetRelativeWeaponPosition();
			lTextureLowerCorner.iX = -lTextureLowerCorner.iX + lRelativeWeaponPosition.iX;
			lTextureLowerCorner.iY = -lTextureLowerCorner.iY + lRelativeWeaponPosition.iY;
			iTextureObject->ChangeXCoordinate(lTextureLowerCorner.iX);
			iTextureObject->ChangeYCoordinate(lTextureLowerCorner.iY);
		}
		iTextureObject->DrawArrays();
	}

	if(iCreateAnotherWeapon)
		iExtraWeapon->Draw();
}
//just makes sure that that the ReadtoShoot flag is reset to true after a certain timeInterval
void CWeaponBomb100Kg::Update()
{
	CWeapon::Update();

	if(iCreateAnotherWeapon)
		iExtraWeapon->Update();
}

TIntFloat CWeaponBomb100Kg::GetXMiddlePointOfWeapon()
{
	TIntFloat lWidthOfWeapon = iTextureObject->GetWidth();
	TIntFloat lMountingOffset =  CMath::Abs(iCoordinate->iX - GetRelativeWeaponPosition().iX);
	return lWidthOfWeapon + lMountingOffset;
}

void CWeaponBomb100Kg::Shoot()
{
	if(iIsDisabled || *iIsUpSideDown)
	{
		//maybe play a sound
		CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Lock, GetRelativeWeaponPosition());
	}
	else
	{
		if(iAmmunitionLeft == 0)
		{
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_Empty, GetRelativeWeaponPosition());
		}
		else if(iReadyToShoot && iAmmunitionLeft > 0)
		{
			//play sound
			CGame::Game->iAudioEngine->PlayCheckPoint(ESoundFileId_Plane_Bomb_release, GetRelativeWeaponPosition());
			iReadyToShoot = false;
			iCurrentFrame = 0;
			iAmmunitionLeft--;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb100kg::New(GetRelativeWeaponPosition(), (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));

			if(iCreateAnotherWeapon)
				iExtraWeapon->Shoot();
		}
	}
}

void CWeaponBomb100Kg::Explode()
{
	if(iAmmunitionLeft > 0)
	{
		iAmmunitionLeft = 0;
		TPointIntFloat lBombPosition = GetRelativeWeaponPosition();
		lBombPosition.iY = TIntFloat::Convert(FLOOR_TEXTURE_HEIGHT);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CBomb100kg::New(lBombPosition, (*iSpeed), *iAngel, *iWeaponReflectedOverYAxis));

		if(iCreateAnotherWeapon)
			iExtraWeapon->Explode();
	}
}

void CWeaponBomb100Kg::SetDisabled(TBool aValue)
{
	iIsDisabled = aValue;
	if(iExtraWeapon)
		iExtraWeapon->SetDisabled(aValue);
}

void CWeaponBomb100Kg::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	WeaponBombObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iCreateAnotherWeapon);
	if(iExtraWeapon)
	{
		aOutputStream.WriteInt32(1);
		iExtraWeapon->SaveOnDisk(aOutputStream);
	}else
		aOutputStream.WriteInt32(0);
}
void CWeaponBomb100Kg::LoadFromDisk(CFileReadStream &aReadStream)
{
	WeaponBombObjectLoadContentFromDisk(aReadStream);
	iCreateAnotherWeapon = aReadStream.ReadInt32();
	TInt lHasAnotherWeapon = aReadStream.ReadInt32();
	if(lHasAnotherWeapon == 1)
	{
		TWeaponIdentifier l100kgWeaponId = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());//not used
		iExtraWeapon->LoadFromDisk(aReadStream);
	}
}
