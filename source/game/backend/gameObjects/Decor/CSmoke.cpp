/*
 * CSmoke.cpp
 *
 *  Created on: May 7, 2013
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Decor/CSmoke.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CImageStore.h"

CSmoke::CSmoke(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aDirectionAngle, TColour aColour, TSizeIntFloat aSize,  TInt aFramesUntilDestruct, TInt aZValue, TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY, SGameObjectAttributes& aAttributes) :
	CProjectile(NULL, NULL, aLocation, aAttributes, aSpeed, AIRCRAFT_SMOKE_RANGE, aDirectionAngle)
{
	iZCor = aZValue;
	iSmokeSize = aSize;
	iIncreaseSizeFactor = TIntFloat::Convert(1.01);
	iIsMoveableObject = false;
	iFramesUntilDestruct = aFramesUntilDestruct;
	iColour = aColour;
	iAlphaValue = iColour & (0x000000FF);

	iExtraDisplacementFactorX = aSquareDisplacementFactorX / iFramesUntilDestruct;
	iExtraDisplacementFactorY = aSquareDisplacementFactorY / iFramesUntilDestruct;
	iExtraDisplacementAccumaltorX = TIntFloat::Convert(0);
	iExtraDisplacementAccumaltorY = TIntFloat::Convert(0);
}

CSmoke::~CSmoke()
{
}

CSmoke* CSmoke::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aDirectionAngle, TBool aReflected, TColour aColour, TSizeIntFloat aSize,  TInt aFramesUntilDestruct, TInt aZValue)
{
	return CSmoke::New(aLocation, aSpeed, aDirectionAngle, aReflected, aColour, aSize, aFramesUntilDestruct, aZValue, TIntFloat::Convert(0), TIntFloat::Convert(0));
}

CSmoke* CSmoke::New(TPointIntFloat aLocation, TIntFloat aSpeed, TInt aDirectionAngle, TBool aReflected, TColour aColour, TSizeIntFloat aSize,  TInt aFramesUntilDestruct, TInt aZValue,
									 TIntFloat aSquareDisplacementFactorX, TIntFloat aSquareDisplacementFactorY)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = BASIC_PROJECTILE_HEALTH;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesNotImportant;
	lAttributes.Reflected = aReflected;
	lAttributes.Armor = BASIC_PROJECTILE_DEFAULT_ARMOR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierSmoke;

	CSmoke* self = new CSmoke(aLocation, aSpeed, aDirectionAngle, aColour, aSize, aFramesUntilDestruct, aZValue, aSquareDisplacementFactorX, aSquareDisplacementFactorY, lAttributes);
	self->Construct();
	return self;
}

void CSmoke::Construct()
{
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdCloudSmokeSequence), iSmokeSize.iWidth.GetIntInBaseInt(), iSmokeSize.iHeight.GetIntInBaseInt(), iZCor, 0, 0.1, 0, 1);

	TInt lRandomInt  = CFighterPilotThePacificWar::FighterGame->iRandomGenerator->GetRandomNumberTInt(0, 10);
	iTextureObject->SetNewSValue((TFloat) 0.1 * lRandomInt, (TFloat) 0.1 * (lRandomInt + 1));

	AdjustCoordinatesAndTexture();
	iTextureObject->SetColourAndAlpha((iColour & (0xFF000000)) >> 24, (iColour & (0x00FF0000)) >> 16, (iColour & (0x0000FF00)) >> 8, iAlphaValue);
}

//---------------- functions -------------------------

void CSmoke::LoadSmokeTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdCloudSmokeSequence);
}

void CSmoke::UnloadSmokeTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdCloudSmokeSequence);
}

void CSmoke::InitilizeSmoke()
{
	CSmoke::LoadSmokeTextures();
}

void CSmoke::RemoveSmoke()
{
	CSmoke::UnloadSmokeTextures();
}

void CSmoke::Draw()
{
	//check what alpha value we should use
	TIntFloat lAlpha = TIntFloat::Convert(iFramesUntilDestruct - iCurrentFrame) / iFramesUntilDestruct;
	if(lAlpha < TIntFloat::Convert(0))
		lAlpha = TIntFloat::Convert(0);

	iTextureObject->SetAlpha((lAlpha * iAlphaValue).GetIntInBaseInt());
  iTextureObject->DrawArrays();//uses the currently assianged texture
}

void CSmoke::Move()
{

	if(iCurrentFrame == iFramesUntilDestruct)
	{
		Destruct();
	}
	else
	{
		TIntFloat lRandomInt  = CFighterPilotThePacificWar::FighterGame->iRandomGenerator->GetRandomNumberTIntFloat(0, 4);
		iIncreaseSizeFactor =  TIntFloat::Convert(1) + (lRandomInt / 100);
		iSmokeSize.iWidth  *= iIncreaseSizeFactor;
		iSmokeSize.iHeight *= iIncreaseSizeFactor;

		iTextureObject->ChangeWidth(iSmokeSize.iWidth - iTextureObject->GetWidth());
		iTextureObject->ChangeHeight(iSmokeSize.iHeight - iTextureObject->GetHeight());

		//find out how much smoke will move
		//X
		iExtraDisplacementAccumaltorX += iExtraDisplacementFactorX;
		TIntFloat lXChange = iExtraDisplacementAccumaltorX;
		lXChange = lXChange * lXChange;
		lXChange += iPixelsPerMoveX;

		//Y
		iExtraDisplacementAccumaltorY += iExtraDisplacementFactorY;
		TIntFloat lYChange = iExtraDisplacementAccumaltorY;
		lYChange = lYChange * lYChange;
		lYChange += iPixelsPerMoveY;

		//update location
		iCoordinates.iX += lXChange;
		iCoordinates.iY += lYChange;
		iTextureObject->ChangeXCoordinate(lXChange);
		iTextureObject->ChangeYCoordinate(lYChange);
	}
	iCurrentFrame++;
}

void CSmoke::Update()
{
	return;
}

void CSmoke::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	ProjectileObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iFramesUntilDestruct);
	aOutputStream.WriteInt32(iColour);
	aOutputStream.WriteInt32(iAlphaValue);
	aOutputStream.WriteIntFloat(iSmokeSize.iWidth);
	aOutputStream.WriteIntFloat(iSmokeSize.iHeight);
	aOutputStream.WriteIntFloat(iIncreaseSizeFactor);
	aOutputStream.WriteIntFloat(iExtraDisplacementFactorX);
	aOutputStream.WriteIntFloat(iExtraDisplacementFactorY);
	aOutputStream.WriteIntFloat(iExtraDisplacementAccumaltorX);
	aOutputStream.WriteIntFloat(iExtraDisplacementAccumaltorY);
}

void CSmoke::LoadFromDisk(CFileReadStream &aReadStream)
{
	ProjectileObjectLoadContentFromDisk(aReadStream);
	iFramesUntilDestruct = aReadStream.ReadInt32();
	iColour = aReadStream.ReadInt32();
	iAlphaValue = aReadStream.ReadInt32();
	iSmokeSize.iWidth = aReadStream.ReadIntFloat();
	iSmokeSize.iHeight = aReadStream.ReadIntFloat();
	iIncreaseSizeFactor = aReadStream.ReadIntFloat();
	iExtraDisplacementFactorX = aReadStream.ReadIntFloat();
	iExtraDisplacementFactorY = aReadStream.ReadIntFloat();
	iExtraDisplacementAccumaltorX = aReadStream.ReadIntFloat();
	iExtraDisplacementAccumaltorY = aReadStream.ReadIntFloat();
}
