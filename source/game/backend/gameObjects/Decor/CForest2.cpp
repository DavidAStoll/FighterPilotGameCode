/*
 ============================================================================
 Name		: Forest2.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest2 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CForest2::CForest2(TPoint aLocation,SGameObjectAttributes& aAttributes)
:CGameObject(FOREST2_DEFAULT_Z_COORDINATE, NULL,NULL,aLocation, aAttributes)
	{
	// No implementation required
	}

CForest2::~CForest2()
	{
	}

CForest2* CForest2::LoadForest2()
   {
  SGameObjectAttributes lAttributes;
  lAttributes.Armor=1000; lAttributes.ConflictSide=EConflictSideNeutal; lAttributes.Health=0;
  lAttributes.HitBox=NULL; lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture; lAttributes.Reflected=false;
  lAttributes.ObjectIdentifier=EGameObjectIdentifierForest2;

  CForest2* self = new CForest2(TPoint(0,0),lAttributes);
  self->ConstructWithoutCreatingOtherObjectsL();
  return self;
   }

CForest2* CForest2::New(TInt aXLocation)
	{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor=1000; lAttributes.ConflictSide=EConflictSideNeutal; lAttributes.Health=0;
	lAttributes.HitBox=NULL; lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture; lAttributes.Reflected=false;
	lAttributes.ObjectIdentifier=EGameObjectIdentifierForest2;

	CForest2* self = new CForest2(TPoint(aXLocation,FLOOR_TEXTURE_HEIGHT),lAttributes);
	self->ConstructL();
	return self;
	}

void CForest2::ConstructL()
	{
  //texture object is just used as a dummy and will not be used for drawing purposes, it will just serve to know what intervals the forst is in  
  iTextureObject= CTextureObject::New(GetCurrentPositionNormilized(), NULL,FOREST2_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT,iZCor,0,1,0,1);
  //same goes for the hitBox, it will just be used to know if an tree is inside the PlayerViewRect
  iHitBox=CHitBox::New(&iCoordinates,&iObjectReflected,TSize(FOREST2_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT));

  //since this is just a container class for trees, it doesn't have a Texture for itself
  
  TPoint lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST2_SMALL_TREE1_OFFSET;
  iSmallTree1 = CSmallTree::New(lTreePosition,ESmallTreeReflected);
  iSmallTree1->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree1);

  //Add Big Trees
  lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST2_BIG_TREE1_OFFSET;
  iBigTree1 = CBigTree::New(lTreePosition,EBigTreeNormal);
  iBigTree1->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree1);

  lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST2_BIG_TREE2_OFFSET;
  iBigTree2 = CBigTree::New(lTreePosition,EBigTreeReflected);
  iBigTree2->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree2);
	}

void CForest2::ConstructWithoutCreatingOtherObjectsL()
{
  //texture object is just used as a dummy and will not be used for drawing purposes, it will just serve to know what intervals the forst is in  
  iTextureObject= CTextureObject::New(GetCurrentPositionNormilized(),NULL,FOREST2_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT,iZCor,0,1,0,1);
  //same goes for the hitBox, it will just be used to know if an tree is inside the PlayerViewRect
  iHitBox=CHitBox::New(&iCoordinates,&iObjectReflected,TSize(FOREST2_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT));
}


//--------------------- functions ---------------------

void CForest2::Draw()
{
  //draw everything
  iBigTree1->DrawAlways();
  iSmallTree1->DrawAlways();
  iBigTree2->DrawAlways();
}

void CForest2::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  GameObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteUint32((TUint32) iSmallTree1);
  aOutputStream.WriteUint32((TUint32) iBigTree1);
  aOutputStream.WriteUint32((TUint32) iBigTree2);
}

void  CForest2::LoadFromDisk(CFileReadStream &aReadStream)
{
  GameObjectLoadContentFromDisk(aReadStream);
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree1));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree1));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree2));
}
