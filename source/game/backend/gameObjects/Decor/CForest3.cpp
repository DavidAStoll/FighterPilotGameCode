/*
 ============================================================================
 Name		: Forest3.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CForest3 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CForest3::CForest3(TPoint aLocation,SGameObjectAttributes& aAttributes)
:CGameObject(FOREST3_DEFAULT_Z_COORDINATE, NULL,NULL,aLocation, aAttributes)
	{
	// No implementation required
	}

CForest3::~CForest3()
	{
	}

CForest3* CForest3::LoadForest3()
   {
  SGameObjectAttributes lAttributes;
  lAttributes.Armor=1000; lAttributes.ConflictSide=EConflictSideNeutal; lAttributes.Health=0;
  lAttributes.HitBox=NULL; lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture; lAttributes.Reflected=false;
  lAttributes.ObjectIdentifier=EGameObjectIdentifierForest3;

  CForest3* self = new CForest3(TPoint(0,0),lAttributes);
  self->ConstructWithoutCreatingOtherObjectsL();
  return self;
   }

CForest3* CForest3::New(TInt aXLocation)
	{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor=1000; lAttributes.ConflictSide=EConflictSideNeutal; lAttributes.Health=0;
	lAttributes.HitBox=NULL; lAttributes.ObjectType = EObjectTypesNoHitBoxNoTexture; lAttributes.Reflected=false;
	lAttributes.ObjectIdentifier=EGameObjectIdentifierForest3;

	CForest3* self = new CForest3(TPoint(aXLocation,FLOOR_TEXTURE_HEIGHT),lAttributes);
	self->ConstructL();
	return self;
	}

void CForest3::ConstructL()
	{
  //texture object is just used as a dummy and will not be used for drawing purposes, it will just serve to know what intervals the forst is in  
  iTextureObject= CTextureObject::New(GetCurrentPositionNormilized(),NULL,FOREST3_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT,iZCor,0,1,0,1);
  //same goes for the hitBox, it will just be used to know if an tree is inside the PlayerViewRect
  iHitBox=CHitBox::New(&iCoordinates,&iObjectReflected,TSize(FOREST3_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT));

  //since this is just a container class for trees, it doesn't have a Texture for itself
  
  TPoint lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST3_SMALL_TREE1_OFFSET;
  iSmallTree1 = CSmallTree::New(lTreePosition,ESmallTreeNormal);
  iSmallTree1->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree1);
  
  lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST3_SMALL_TREE2_OFFSET;
  iSmallTree2 = CSmallTree::New(lTreePosition,ESmallTreeNormal);
  iSmallTree2->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iSmallTree2);

  //Add Big Trees
  lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST3_BIG_TREE1_OFFSET;
  iBigTree1 = CBigTree::New(lTreePosition,EBigTreeNormal);
  iBigTree1->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree1);

  lTreePosition = GetCurrentPositionNormilized();
  lTreePosition.iX += FOREST3_BIG_TREE2_OFFSET;
  iBigTree2 = CBigTree::New(lTreePosition,EBigTreeReflected);
  iBigTree2->SetDoNotDraw(true);//since we control drawing for this tree
  //add Tree to map
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(iBigTree2);
	}

void CForest3::ConstructWithoutCreatingOtherObjectsL()
{
  //texture object is just used as a dummy and will not be used for drawing purposes, it will just serve to know what intervals the forst is in  
  iTextureObject= CTextureObject::New(GetCurrentPositionNormilized(),NULL,FOREST3_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT,iZCor,0,1,0,1);
  //same goes for the hitBox, it will just be used to know if an tree is inside the PlayerViewRect
  iHitBox=CHitBox::New(&iCoordinates,&iObjectReflected,TSize(FOREST3_TOTAL_WIDTH,BASIC_FOREST_DEFAULT_HEIGHT));
}


//--------------------- functions ---------------------

void CForest3::Draw()
{
  //draw everything
  iSmallTree1->DrawAlways();
  iBigTree1->DrawAlways();
  iBigTree2->DrawAlways();
  iSmallTree2->DrawAlways();
}

void CForest3::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  GameObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteUint32((TUint32) iSmallTree1);
  aOutputStream.WriteUint32((TUint32) iSmallTree2);
  aOutputStream.WriteUint32((TUint32) iBigTree1);
  aOutputStream.WriteUint32((TUint32) iBigTree2);
}

void  CForest3::LoadFromDisk(CFileReadStream &aReadStream)
{
  GameObjectLoadContentFromDisk(aReadStream);
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree1));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iSmallTree2));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree1));
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iBigTree2));
}
