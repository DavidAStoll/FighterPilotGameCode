/*
 ============================================================================
 Name		: Explosive75mmShell.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosive75mmShell implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Projectiles/CExplosive75mmShell.h"
#include "includes/game/backend/gameObjects/Explosions/CExplosion500KgInAir.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CExplosive75mmShell::CExplosive75mmShell(TPoint aLocation,SGameObjectAttributes& aAttributes,TInt aAngel,TInt aRange)
:CProjectile(NULL,NULL,aLocation,aAttributes,TIntFloat::Convert(EXPLOSIVE75MMSHELL_SPEED),aRange,aAngel)
        {
        // No implementation required
        }

CExplosive75mmShell::CExplosive75mmShell(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TInt aAngel,TInt aRange)
:CProjectile(NULL,NULL,aLocation,aAttributes,TIntFloat::Convert(EXPLOSIVE75MMSHELL_SPEED),aRange,aAngel)
        {
        // No implementation required
        }
CExplosive75mmShell::~CExplosive75mmShell()
	{
	}

CExplosive75mmShell* CExplosive75mmShell::New(TPointIntFloat aLocation,TInt aAngel,TBool aReflected,TInt aRange)
        {
        SGameObjectAttributes lAttributes;
        lAttributes.Health=BASIC_PROJECTILE_HEALTH;    lAttributes.ConflictSide=EConflictSideAmerican; lAttributes.HitBox=NULL;
        lAttributes.ObjectType=EObjectTypesProjectileExplosive;  lAttributes.Reflected=aReflected;    lAttributes.Armor=BASIC_PROJECTILE_DEFAULT_ARMOR;
        lAttributes.ObjectIdentifier=EGameObjectIdentifierExplosive75mmShell;
        CExplosive75mmShell* self = new CExplosive75mmShell(aLocation,lAttributes,aAngel,aRange);
        self->ConstructL();
        return self;
        }

void CExplosive75mmShell::ConstructL()
	{
        iTextureObject = CTextureObject::New(GetCurrentPositionNormilized(),&CGame::Game->iImageStore->GetImage(EImageIdBullet75mmExplosiveShell),EXPLOSIVE75MMSHELL_WIDTH*CPlayer::ZoomFactor,EXPLOSIVE75MMSHELL_HEIGHT,iZCor,0,1,0,1);
        AdjustCoordinatesAndTexture();
  
        //no need for hit box since it doesn't collide with anything
        iHitBox = NULL;
	}

//---------------- functions -------------------------

void CExplosive75mmShell::LoadExplosive75mmShellTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdBullet75mmExplosiveShell);
}


void CExplosive75mmShell::UnloadExplosive75mmShellTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdBullet75mmExplosiveShell);
}

void CExplosive75mmShell::InitilizeExplosive75mmShell()
{
  CExplosive75mmShell::LoadExplosive75mmShellTextures();
}

void CExplosive75mmShell::RemoveExplosive75mmShell()
{
  CExplosive75mmShell::UnloadExplosive75mmShellTextures();
}

void CExplosive75mmShell::Destruct()//default behaviour, just set it to dead to remove it
{
  iAlive=false;
  iDoNotDraw=true;
  iRecycleObject=true;//remove from game
  
  //create explosion at current Location
  TPointIntFloat lExplosionLocation;
  //needs to be centered
  lExplosionLocation.iX = iCoordinates.iX - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_WIDTH)/2;
  lExplosionLocation.iY = iCoordinates.iY - TIntFloat::Convert(EXPLOSION_IN_AIR_500KG_HEIGHT)/2;
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddGameObject(CExplosion500KgInAir::New(lExplosionLocation,false));//sometimes hits ground troops running through the map
}
