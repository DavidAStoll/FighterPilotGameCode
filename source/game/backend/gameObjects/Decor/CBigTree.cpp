/*
 ============================================================================
 Name		: BigTree.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBigTree implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Decor/CBigTree.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CBigTree::CBigTree(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(TREE_DEFAULT_Z_COORDINATE, NULL, NULL, aLocation, aAttributes)
{
	// No implementation required
}

CBigTree::~CBigTree()
{
}

CBigTree* CBigTree::New(TPoint aLocation, TBigTree aOrientation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_1000KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_1000KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierBigTree;

	CBigTree* self = new CBigTree(aLocation, lAttributes);
	self->ConstructL(aOrientation);
	return self;
}

CBigTree* CBigTree::New(TInt aXLocation, TBigTree aType)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_1000KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_1000KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierBigTree;

	CBigTree* self = new CBigTree(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL(aType);
	return self;
}

void CBigTree::ConstructL(TBigTree aOrientation)
{
	//since the texture will be repeated over and over again until the entire distance has been covered given by width
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdTreeBigTree), TREE_BIG_TREE_DEFAULT_WIDHT, TREE_BIG_TREE_DEFAULT_HEIGHT, iZCor, 0, 1, 0, 1);

	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(TREE_BIG_TREE_HITBOX_WIDTH, TREE_BIG_TREE_HITBOX_HEIGHT));

	if(aOrientation == EBigTreeReflected)
		iTextureObject->ReflectOverYAxis();
}

//----------------- functions ----------------

void CBigTree::Die(TDamageType aType) // sets the status of the Object to dead but also starts the dead animation loop
{
	iAlive = false;
	iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdTreeBigTreeDestroyed));
}

void CBigTree::PossibleCollision(CInterval*& aInterval)
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

void CBigTree::LoadBigTreeTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdTreeBigTree);
	CGame::Game->iImageStore->LoadImage(EImageIdTreeBigTreeDestroyed);
}

void CBigTree::UnloadBigTreeTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdTreeBigTree);
	CGame::Game->iImageStore->UnloadImage(EImageIdTreeBigTreeDestroyed);
}

void CBigTree::InitilizeBigTree()
{
	CBigTree::LoadBigTreeTextures();
}

void CBigTree::RemoveBigTree()
{
	CBigTree::UnloadBigTreeTextures();
}

