/*
 ============================================================================
 Name		: Tree1.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CTree1 implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Decor/CTree1.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CTree1::CTree1(TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(TREE_DEFAULT_Z_COORDINATE, NULL, NULL, aLocation, aAttributes)
{
	// No implementation required
}

CTree1::~CTree1()
{
}

CTree1* CTree1::New(TPoint aLocation, TBigThinTree aOrientation)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_1000KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_1000KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierTree1;

	CTree1* self = new CTree1(aLocation, lAttributes);
	self->ConstructL(aOrientation);
	return self;
}

CTree1* CTree1::New(TInt aXLocation, TBigThinTree aType)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Armor = EXPLOSION_1000KG_ARMOR_PENETRATION - 1;
	lAttributes.ConflictSide = EConflictSideNeutal;
	lAttributes.Health = EXPLOSION_1000KG_DAMAGE - 1;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesDestructibleDecor;
	lAttributes.Reflected = false;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierTree1;

	CTree1* self = new CTree1(TPoint(aXLocation, FLOOR_TEXTURE_HEIGHT), lAttributes);
	self->ConstructL(aType);
	return self;
}

void CTree1::ConstructL(TBigThinTree aOrientation)
{
	//since the texture will be repeated over and over again until the entire distance has been covered given by width
	iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(), &CGame::Game->iImageStore->GetImage(EImageIdTreeTallThinTree), TREE1_DEFAULT_WIDHT, TREE1_DEFAULT_HEIGHT, iZCor, 0, 1, 0, 1);

	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(TREE1_DEFAULT_WIDHT, TREE1_DEFAULT_HEIGHT)); //however, will not be used,since clouds will not collide with anyhting

	if(aOrientation == EBigThinTreeReflected)
		iTextureObject->ReflectOverYAxis();
}

//----------------- functions ----------------

void CTree1::Die(TDamageType aType) // sets the status of the Object to dead but also starts the dead animation loop
{
	iAlive = false;
	iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdTreeTallThinTreeDestroyed));
}

void CTree1::PossibleCollision(CInterval*& aInterval)
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

void CTree1::LoadTree1Textures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdTreeTallThinTree);
	CGame::Game->iImageStore->LoadImage(EImageIdTreeTallThinTreeDestroyed);
}

void CTree1::UnloadTree1Textures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdTreeTallThinTree);
	CGame::Game->iImageStore->UnloadImage(EImageIdTreeTallThinTreeDestroyed);
}

void CTree1::InitilizeTree1()
{
	CTree1::LoadTree1Textures();
}

void CTree1::RemoveTree1()
{
	CTree1::UnloadTree1Textures();
}
