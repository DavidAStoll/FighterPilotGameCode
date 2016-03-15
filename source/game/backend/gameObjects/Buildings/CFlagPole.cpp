/*
 ============================================================================
 Name		: FlagPole.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFlagPole implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Buildings/CFlagPole.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CFlagPole::CFlagPole(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CBuilding(DECOR_DEFAULT_Z_VALUE, NULL, NULL, aLocation, aAttributes)
{
	// all object that are not reflected look to the left
	iAngle = 180;
	iIsMoveableObject = true; // because some flags due move around, need to keep updating in what interval they are
}

CFlagPole::~CFlagPole()
{
	if(iFlagPole)
	{
		delete iFlagPole;
		iFlagPole = NULL;
	}
}

CFlagPole* CFlagPole::New(TPoint aLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierFlagPole;

	CFlagPole* self = new CFlagPole(aLocation, lAttributes);
	self->ConstructL();
	return self;
}

CFlagPole* CFlagPole::New(TInt aXLocation, TConflictSide aConflictSide)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = aConflictSide;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierFlagPole;

	CFlagPole* self = new CFlagPole(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

void CFlagPole::ConstructL()
{

	TPoint lFlagPosition = GetCurrentPositionNormilized();
	lFlagPosition.iY += FLAG_POLE_FLAG_POSITION_HEIGHT_OFFSET;
	//since the texture will be repeated over and over again until the entire distance has been covered given by width
	iTextureObject = CTextureObject::New(lFlagPosition, &CGame::Game->iImageStore->GetImage(EImageIdBuildingFlagPoleJapaneseFlagSequence), FLAG_POLE_FLAG_WIDTH, FLAG_POLE_FLAG_HEIGHT, iZCor, 0, -1, 0, 0.625);
	iFlagPole = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdBuildingFlagPole), FLAG_POLE_WIDTH, FLAG_POLE_HEIGHT, iZCor, 0, 1, 0, 1);

	//since Flag moves, we need an animation Player to display the animation
	//Animation Player
	SAnimationRequest lDefaultRequest; // just a dummy, will be switched out by ChangeConflict Side call
	iAnimationPlayer = CAnimationPlayer::New(iTextureObject, lDefaultRequest);

	//set what side we are on
	ChangeFlagConflictSide(iConflictSide);
	
	iSoundObject = CSoundObject::New(ESoundFileId_Flag, GetCurrentPosition());
	iSoundObject->PlayRepeatCheckPoint();
}

//-------------- functions-------------------//

void CFlagPole::Rotate(TInt aDegree)
{
	iAngle += aDegree;
	iFlagPole->RotateTextureObjectToAngleAndAdjustToPoint(iAngle, iObjectReflected,iCoordinates);

	TPointIntFloat lCoordinate;
	lCoordinate = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, 0, FLAG_POLE_FLAG_POSITION_HEIGHT_OFFSET);
	iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(iAngle, iObjectReflected, lCoordinate);
}

void CFlagPole::ChangeFlagConflictSide(TConflictSide aSide)
{
	iConflictSide = aSide;
	//Animation Player
	SAnimationRequest lDefaultRequest;
	lDefaultRequest.TotalImages = 4;
	lDefaultRequest.Width = -FLAG_POLE_FLAG_WIDTH;
	lDefaultRequest.TotalLength = 256;
	lDefaultRequest.LowerT = 0;
	lDefaultRequest.UpperT = 0.625;
	if(iConflictSide == EConflictSideAmerican)
	{
		lDefaultRequest.FramesUntilSwitch = FLAG_POLE_AMERICAN_FLAG_SEQUENCE_FRAMES_UNTIL_SWITCH;//since image never changes, call the method only once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingFlagPoleAmericanFlagSequence);
	}else //has to be Japanese
	{
		lDefaultRequest.FramesUntilSwitch = FLAG_POLE_JAPANESE_FLAG_SEQUENCE_FRAMES_UNTIL_SWITCH;//since image never changes, call the method only once in a while
		lDefaultRequest.TextureId = &CGame::Game->iImageStore->GetImage(EImageIdBuildingFlagPoleJapaneseFlagSequence);
	}
	iAnimationPlayer->AssignDefaultAnimation(lDefaultRequest);
	iAnimationPlayer->CancelCurrentAnimation();//so new default animation is played
	iAnimationPlayer->Update();
}

void CFlagPole::SetPosition(TPoint& aPoint)
{
	TPointIntFloat lIntFloatPoint;
	lIntFloatPoint.iX = TIntFloat::Convert(aPoint.iX);
	lIntFloatPoint.iY = TIntFloat::Convert(aPoint.iY);
	iCoordinates = lIntFloatPoint;
	iFlagPole->SetNewLowerLeftCoordinate(lIntFloatPoint);
	TPointIntFloat lFlagPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, 0, FLAG_POLE_FLAG_POSITION_HEIGHT_OFFSET);
	iTextureObject->SetNewLowerLeftCoordinate(lFlagPosition);
}

void CFlagPole::SetPosition(TPointIntFloat& aPoint)
{
	iCoordinates = aPoint;
	iFlagPole->SetNewLowerLeftCoordinate(aPoint);
	TPointIntFloat lFlagPosition = CMath::GetPointRelativeToCoordinates(iCoordinates, iObjectReflected, iAngle, 0, FLAG_POLE_FLAG_POSITION_HEIGHT_OFFSET);
	iTextureObject->SetNewLowerLeftCoordinate(lFlagPosition);
}

void CFlagPole::Update()
{
	iAnimationPlayer->Update();
}

void CFlagPole::Draw()
{
	iFlagPole->DrawArrays();//draw flag pole with and then the associated flag
	iTextureObject->DrawArrays();
}

void CFlagPole::DrawAtPositionWithValues(TPointIntFloat& aPosition, TIntFloat lWidth, TIntFloat lHeight)
{

	CHud* lCurrentHud = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetHud();

	TIntFloat lWidthOfFlagPole = iFlagPole->GetWidth();
	TIntFloat lHeightOfFlagPole = iFlagPole->GetHeight();
	lWidthOfFlagPole = lCurrentHud->GetObjectWidthOnMap(lWidthOfFlagPole);
	lHeightOfFlagPole = lCurrentHud->GetObjectHeightOnMap(lHeightOfFlagPole);

	iFlagPole->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidthOfFlagPole, lHeightOfFlagPole);

	TIntFloat lWidthOfFlag = iTextureObject->GetWidth();
	TIntFloat lHeightOfFlag = iTextureObject->GetHeight();
	TIntFloat lYPositionOfFlagOffSet;

	lWidthOfFlag = lCurrentHud->GetObjectWidthOnMap(lWidthOfFlag);
	lHeightOfFlag = lCurrentHud->GetObjectHeightOnMap(lHeightOfFlag);
	lYPositionOfFlagOffSet = lHeightOfFlagPole - lHeightOfFlag;//since it is located at the top of the flagPole
	aPosition.iY += lYPositionOfFlagOffSet;

	iTextureObject->DrawAtPositionWithValuesWithDrawArrays(aPosition, lWidthOfFlag, lHeightOfFlag);
}

void CFlagPole::LoadFlagPoleTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingFlagPoleAmericanFlagSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingFlagPoleJapaneseFlagSequence);
	CGame::Game->iImageStore->LoadImage(EImageIdBuildingFlagPole);
}

void CFlagPole::UnloadFlagPoleTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingFlagPoleAmericanFlagSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingFlagPoleJapaneseFlagSequence);
	CGame::Game->iImageStore->UnloadImage(EImageIdBuildingFlagPole);
}

void CFlagPole::InitilizeFlagPole()
{
	CFlagPole::LoadFlagPoleTextures();
}

void CFlagPole::RemoveFlagPole()
{
	CFlagPole::UnloadFlagPoleTextures();
}

void CFlagPole::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BuildingSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iAngle);
	iFlagPole->SaveOnDisk(aOutputStream);
}

void CFlagPole::LoadFromDisk(CFileReadStream &aReadStream)
{
	BuildingLoadContentFromDisk(aReadStream);
	iAngle = aReadStream.ReadInt32();
	iFlagPole->LoadFromDisk(aReadStream);
}
