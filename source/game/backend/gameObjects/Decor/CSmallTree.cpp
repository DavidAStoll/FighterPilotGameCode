/*
 ============================================================================
 Name		: SmallTree.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CSmallTree implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CSmallTree::CSmallTree(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(TREE_DEFAULT_Z_COORDINATE, NULL, NULL, aLocation, aAttributes)
{
	// No implementation required
}

CSmallTree::~CSmallTree()
{
}

CSmallTree* CSmallTree::New(TPoint aLocation, TSmallTree aOrientation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_500KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_500KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierSmallTree;

	CSmallTree* self = new CSmallTree(aLocation, lAttributes);
	self->ConstructL(aOrientation);
	return self;
}

CSmallTree* CSmallTree::New(TInt aXLocation, TSmallTree aType)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_500KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_500KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierSmallTree;

	CSmallTree* self = new CSmallTree(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL(aType);
	return self;
}

void CSmallTree::ConstructL(TSmallTree aOrientation)
{
	//since the texture will be repeated over and over again until the entire distance has been covered given by width
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdTreeSmallTree), TREE_SMALL_TREE_DEFAULT_WIDHT, TREE_SMALL_TREE_DEFAULT_HEIGHT, iZCor, 0, 1, 0, 1);

	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(TREE_SMALL_TREE_DEFAULT_WIDHT, TREE_SMALL_TREE_DEFAULT_HEIGHT)); //however, will not be used,since clouds will not collide with anyhting

	if(aOrientation == ESmallTreeReflected)
		iTextureObject->ReflectOverYAxis();
}

//----------------- functions ----------------

void CSmallTree::Die(TDamageType aType) // sets the status of the Object to dead but also starts the dead animation loop
{
	iAlive = false;
	iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdTreeSmallTreeDestroyed));
}

void CSmallTree::PossibleCollision(CInterval*& aInterval)
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

void CSmallTree::LoadSmallTreeTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdTreeSmallTree);
	CGame::Game->iImageStore->LoadImage(EImageIdTreeSmallTreeDestroyed);
}

void CSmallTree::UnloadSmallTreeTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdTreeSmallTree);
	CGame::Game->iImageStore->UnloadImage(EImageIdTreeSmallTreeDestroyed);
}

void CSmallTree::InitilizeSmallTree()
{
	CSmallTree::LoadSmallTreeTextures();
}

void CSmallTree::RemoveSmallTree()
{
	CSmallTree::UnloadSmallTreeTextures();
}
