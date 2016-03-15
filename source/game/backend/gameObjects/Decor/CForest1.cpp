/*
 ============================================================================
 Name		: Forest1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest1 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CForest1::CForest1(TPoint aLocation,SGameObjectAttributes& aAttributes)
:CGameObject(FOREST1_DEFAULT_Z_COORDINATE, NULL,NULL,aLocation, aAttributes)
	{
	// No implementation required
	}

CForest1::~CForest1()
	{
	}

CForest1* CForest1::LoadForest1()
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierForest1;

	CForest1* self = new CForest1(TPoint(0, 0), lAttributes);
	self->ConstructWithoutCreatingOtherObjectsL();
	return self;
}

CForest1* CForest1::New(TInt aXLocation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = 1000;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = 0;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierForest1;

	CForest1* self = new CForest1(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL();
	return self;
}

void CForest1::ConstructL()
{
	//texture object is just used as a dummy and will not be used for drawing purposes, it will just serve to know what intervals the forst is in
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), NULL, FOREST1_TOTAL_WIDTH,
			BASIC_FOREST_DEFAULT_HEIGHT, iZCor, 0, 1, 0, 1);
	//same goes for the hitBox, it will just be used to know if an tree is inside the PlayerViewRect
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(FOREST1_TOTAL_WIDTH, BASIC_FOREST_DEFAULT_HEIGHT));

	//since this is just a container class for trees, it doesn't have a Texture for itself
	TPoint lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_SMALL_TREE1_OFFSET;
	iSmallTree1 = CSmallTree::New(lTreePosition, ESmallTreeReflected);
	iSmallTree1->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree1);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_SMALL_TREE2_OFFSET;
	iSmallTree2 = CSmallTree::New(lTreePosition, ESmallTreeReflected);
	iSmallTree2->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree2);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_SMALL_TREE3_OFFSET;
	iSmallTree3 = CSmallTree::New(lTreePosition, ESmallTreeNormal);
	iSmallTree3->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree3);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_SMALL_TREE4_OFFSET;
	iSmallTree4 = CSmallTree::New(lTreePosition, ESmallTreeNormal);
	iSmallTree4->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree4);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_SMALL_TREE5_OFFSET;
	iSmallTree5 = CSmallTree::New(lTreePosition, ESmallTreeNormal);
	iSmallTree5->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree5);

	//Add Big Trees
	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_BIG_TREE1_OFFSET;
	iBigTree1 = CBigTree::New(lTreePosition, EBigTreeNormal);
	iBigTree1->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree1);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_BIG_TREE2_OFFSET;
	iBigTree2 = CBigTree::New(lTreePosition, EBigTreeNormal);
	iBigTree2->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree2);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_BIG_TREE3_OFFSET;
	iBigTree3 = CBigTree::New(lTreePosition, EBigTreeReflected);
	iBigTree3->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree3);

	//Add thin trees
	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_THIN_BIG_TREE1_OFFSET;
	iTree11 = CTree1::New(lTreePosition, EBigThinTreeNormal);
	iTree11->SetDoNotDraw(true); //since we control drawing for this tree
	//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iTree11);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_THIN_BIG_TREE2_OFFSET;
	iTree12 = CTree1::New(lTreePosition, EBigThinTreeReflected);
	iTree12->SetDoNotDraw(true); //since we control drawing for this tree
//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iTree12);

	lTreePosition = GetCurrentPositionNormilized();
	lTreePosition.iX += FOREST1_THIN_BIG_TREE3_OFFSET;
	iTree13 = CTree1::New(lTreePosition, EBigThinTreeNormal);
	iTree13->SetDoNotDraw(true); //since we control drawing for this tree
//add Tree to map
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iTree13);
}

void CForest1::ConstructWithoutCreatingOtherObjectsL()
{
  //texture object is just used as a dummy and will not be used for drawing purposes, it will just serve to know what intervals the forst is in  
  iTextureObject= CTextureObject::New(GetCurrentPositionNormilized(), NULL ,FOREST1_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT,iZCor,0,1,0,1);
  //same goes for the hitBox, it will just be used to know if an tree is inside the PlayerViewRect
  iHitBox=CHitBox::New(&iCoordinates,&iObjectReflected,TSize(FOREST1_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT));
}

//--------------------- functions ---------------------

void CForest1::Draw()
{
  //draw everything
  iBigTree1->DrawAlways();
  iSmallTree1->DrawAlways();
  iSmallTree2->DrawAlways();
  iTree11->DrawAlways();
  iBigTree2->DrawAlways();
  iSmallTree3->DrawAlways();
  iTree12->DrawAlways();
  iSmallTree4->DrawAlways();
  iTree13->DrawAlways();
  iBigTree3->DrawAlways();
}


void CForest1::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  GameObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteUint32((TUint32) iSmallTree1);
  aOutputStream.WriteUint32((TUint32) iSmallTree2);
  aOutputStream.WriteUint32((TUint32) iSmallTree3);
  aOutputStream.WriteUint32((TUint32) iSmallTree4);
  aOutputStream.WriteUint32((TUint32) iSmallTree5);
  aOutputStream.WriteUint32((TUint32) iBigTree1);
  aOutputStream.WriteUint32((TUint32) iBigTree2);
  aOutputStream.WriteUint32((TUint32) iBigTree3);
  aOutputStream.WriteUint32((TUint32) iTree11);
  aOutputStream.WriteUint32((TUint32) iTree12);
  aOutputStream.WriteUint32((TUint32) iTree13);
}

void  CForest1::LoadFromDisk(CFileReadStream &aReadStream)
{
  GameObjectLoadContentFromDisk(aReadStream);
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree1));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree2));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree3));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree4));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree5));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree1));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree2));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree3));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iTree11));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iTree12));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iTree13));
}
