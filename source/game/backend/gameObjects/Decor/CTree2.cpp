/*
 ============================================================================
 Name		: Tree2.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CTree2 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Decor/CTree2.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CTree2::CTree2(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(TREE_PALM_TREE_Z_COORDINATE, NULL, NULL, aLocation, aAttributes)
{
	// No implementation required
}

CTree2::~CTree2()
{
}

CTree2* CTree2::New(TPoint aLocation, TTree2Type aType)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_500KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_500KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierTree2;

	CTree2* self = new CTree2(aLocation, lAttributes);
	self->ConstructL(aType);
	return self;
}

CTree2* CTree2::New(TInt aXLocation, TTree2Type aType)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_500KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_500KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierTree2;

	CTree2* self = new CTree2(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL(aType);
	return self;
}

void CTree2::ConstructL(TTree2Type aType)
{
	//need to check what kind of texture we are going to use
	if(aType == ETree2BowedTree || aType == ETree2BowedTreeReflected || aType == ETree2BowedTreeSmaller || aType == ETree2BowedTreeSmallerReflected)
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdTreePalmTreeBowed), TREE2_DEFAULT_WIDHT, TREE2_DEFAULT_HEIGHT, iZCor, 0, 1, 0, 1);

	if(aType == ETree2MiddleTree || aType == ETree2MiddleTreeReflected || aType == ETree2MiddleTreeSmaller || aType == ETree2MiddleTreeSmallerReflected)
		iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdTreePalmTreeMiddle), TREE2_DEFAULT_WIDHT, TREE2_DEFAULT_HEIGHT, iZCor, 0, 1, 0, 1);

	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(TREE2_DEFAULT_WIDHT, TREE2_DEFAULT_HEIGHT)); //however, will not be used,since clouds will not collide with anyhting
	iCoordinates.iY += TIntFloat::Convert(TREE2_Y_COLLISION_BOX_ADJUSTMENT);//adjust hitbox

	if(aType == ETree2BowedTreeSmaller || aType == ETree2BowedTreeSmallerReflected || aType == ETree2MiddleTreeSmaller || aType == ETree2MiddleTreeSmallerReflected)
	{
		iTextureObject->ChangeYCoordinate(TIntFloat::Convert(TREE2_SMALLER_Y_OFFSET));
		iCoordinates.iY += TIntFloat::Convert(TREE2_SMALLER_Y_OFFSET);
	}

	if(aType == ETree2BowedTreeReflected || aType == ETree2BowedTreeSmallerReflected || aType == ETree2MiddleTreeReflected || aType == ETree2MiddleTreeSmallerReflected)
		iTextureObject->ReflectOverYAxis();

	iTreeType = aType;
}

//----------------- functions ----------------

void CTree2::Die(TDamageType aType) // sets the status of the Object to dead but also starts the dead animation loop
{
	iAlive = false;
	//need to change the texture
	//bowed
	if(iTreeType == ETree2BowedTree || iTreeType == ETree2BowedTreeReflected || iTreeType == ETree2BowedTreeSmaller || iTreeType == ETree2BowedTreeSmallerReflected)
	{
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdTreePalmTreeBowedDestroyed));
	}
	else
	//Middle tree
	{
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdTreePalmTreeMiddleDestroyed));
	}
}

void CTree2::PossibleCollision(CInterval*& aInterval)
{
	if(iAlive)
	{
		//go through all objects and check if care to collide with them
		CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane);

		for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

			//only collide with alive objects
			if(lCurrentGameObject->IsAlive() && lCurrentGameObject != this)//don't compare to itself
			{
				//collide
				if(iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
				{
					SDamage lDamage;
					lDamage.Damage = 1000;
					lDamage.ArmorPenetration = 1000;//should kill any object
					lCurrentGameObject->TakeDamage(lDamage, EDamageTypeTreePlaneCollision);
				}
			}
		}

		//clean up
		delete lGameObjects;
	}
}

void CTree2::LoadTree2Textures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdTreePalmTreeBowed);
	CGame::Game->iImageStore->LoadImage(EImageIdTreePalmTreeBowedDestroyed);
	CGame::Game->iImageStore->LoadImage(EImageIdTreePalmTreeMiddle);
	CGame::Game->iImageStore->LoadImage(EImageIdTreePalmTreeMiddleDestroyed);
}

void CTree2::UnloadTree2Textures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdTreePalmTreeBowed);
	CGame::Game->iImageStore->UnloadImage(EImageIdTreePalmTreeBowedDestroyed);
	CGame::Game->iImageStore->UnloadImage(EImageIdTreePalmTreeMiddle);
	CGame::Game->iImageStore->UnloadImage(EImageIdTreePalmTreeMiddleDestroyed);
}

void CTree2::InitilizeTree2()
{
	CTree2::LoadTree2Textures();
}

void CTree2::RemoveTree2()
{
	CTree2::UnloadTree2Textures();
}

void CTree2::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  GameObjectSaveContentToDisk(aOutputStream);
  aOutputStream.WriteInt32(iTreeType);
}

void  CTree2::LoadFromDisk(CFileReadStream &aReadStream)
{
  GameObjectLoadContentFromDisk(aReadStream);
  iTreeType = static_cast<TTree2Type>(aReadStream.ReadInt32());
}
