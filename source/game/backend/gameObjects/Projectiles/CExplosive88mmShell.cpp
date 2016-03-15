/*
 ============================================================================
 Name		: Explosive88mmShell.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosive88mmShell implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Projectiles/CExplosive88mmShell.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion1000KgInAir.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CExplosive88mmShell::CExplosive88mmShell(TPoint aLocation,SGameObjectAttributes& aAttributes,TInt aAngel,TInt aRange)
:CProjectile(NULL,NULL,aLocation,aAttributes,TIntFloat::Convert(EXPLOSIVE88MMSHELL_SPEED),aRange,aAngel)
        {
        // No implementation required
        }

CExplosive88mmShell::CExplosive88mmShell(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TInt aAngel,TInt aRange)
:CProjectile(NULL,NULL,aLocation,aAttributes,TIntFloat::Convert(EXPLOSIVE88MMSHELL_SPEED),aRange,aAngel)
        {
        // No implementation required
        }

CExplosive88mmShell::~CExplosive88mmShell()
        {
        }

CExplosive88mmShell* CExplosive88mmShell::New(TPointIntFloat aLocation,TInt aAngel,TBool aReflected,TInt aRange)
        {
        SGameObjectAttributes lAttributes;
        lAttributes.Health=BASIC_PROJECTILE_HEALTH;    lAttributes.ConflictSide=EConflictSideNeutal; lAttributes.HitBox=NULL;
        lAttributes.ObjectType=EObjectTypesProjectileExplosive;  lAttributes.Reflected=aReflected;    lAttributes.Armor=BASIC_PROJECTILE_DEFAULT_ARMOR;
        lAttributes.ObjectIdentifier=EGameObjectIdentifierExplosive88mmShell;
        CExplosive88mmShell* self = new CExplosive88mmShell(aLocation,lAttributes,aAngel,aRange);
        self->ConstructL();
        return self;
        }

void CExplosive88mmShell::ConstructL()
{
  
  iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(),&CGame::Game->iImageStore->GetImage(EImageIdBullet88mmExplosiveShell),EXPLOSIVE88MMSHELL_WIDTH,EXPLOSIVE88MMSHELL_HEIGHT,iZCor,0,1,0,1);
  AdjustCoordinatesAndTexture();
  
  //need to create bounding Hitbox
  iHitBox = NULL;
}

//---------------- functions -------------------------

void CExplosive88mmShell::LoadExplosive88mmShellTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet88mmExplosiveShell);
}

void CExplosive88mmShell::UnloadExplosive88mmShellTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet88mmExplosiveShell);
}

void CExplosive88mmShell::InitilizeExplosive88mmShell()
{
  CExplosive88mmShell::LoadExplosive88mmShellTextures();
}

void CExplosive88mmShell::RemoveExplosive88mmShell()
{
  CExplosive88mmShell::UnloadExplosive88mmShellTextures();
}

void CExplosive88mmShell::Destruct()//default behaviour, just set it to dead to remove it
{
  iAlive=false;
  iDoNotDraw=true;
  iRecycleObject=true;//remove from game
  
  //create explosion at current Location
  TPointIntFloat lExplosionLocation;
  //needs to be centered
  lExplosionLocation.iX = iCoordinates.iX - TIntFloat::Convert(EXPLOSION_IN_AIR_1000KG_WIDTH) / 2;
  lExplosionLocation.iY = iCoordinates.iY - TIntFloat::Convert(EXPLOSION_IN_AIR_1000KG_HEIGHT) / 2;
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion1000KgInAir::New(lExplosionLocation));
}
