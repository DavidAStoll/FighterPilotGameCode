/*
 ============================================================================
 Name		: HeavyBomberLocator.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHeavyBomberLocator implementation
 ============================================================================
 */
#include "includes/game/backend/CGameObjectManager.h"
#include "includes/game/backend/CHeavyBomberLocator.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CGameData.h"
#include "includes/core/utility/CImageStore.h"

CHeavyBomberLocator::CHeavyBomberLocator(TPointIntFloat* aCenterOfView,TConflictSide aConflictSide)
	{
	iCenterOfView = aCenterOfView;
	iAngle = 0;
	iCurrentFrame = 0;
	iConflictSide = aConflictSide;
	iCurrentTarget = NULL;
	}

CHeavyBomberLocator::~CHeavyBomberLocator()
	{
  if(iTextureObject)
    {
    delete iTextureObject;
    }
	}

CHeavyBomberLocator* CHeavyBomberLocator::New(TPointIntFloat* aCenterOfView,TConflictSide aConflictSide)
        {
        CHeavyBomberLocator* self = new CHeavyBomberLocator(aCenterOfView,aConflictSide);
        self->ConstructL();
        return self;
        }


void CHeavyBomberLocator::ConstructL()
{
  iTextureObject = CTextureObject::New(TPoint(0,0),&CGame::Game->iImageStore->GetImage(EImageIdHudEnemyBomberLocationArrow),HEAVY_BOMBER_LOCATOR_WIDTH,HEAVY_BOMBER_LOCATOR_HEIGHT,0,0,1,0,1);//will be changed anyway when the hud is displayed
}

//-------------------- functions -----------------------//

void CHeavyBomberLocator::Update()
{
  //look for aircraft and update angle
	if (iCurrentTarget && (!iCurrentTarget->IsAlive() || iCurrentTarget->RecycleObject())) // check if our current target is still a valid one
		iCurrentTarget = NULL;

	iCurrentFrame++;
	if (!iCurrentTarget || iCurrentFrame == HEAVY_BOMBER_LOCATOR_FRAMES_UNTIL_CHECK_FOR_CLOSER_TARGET) // look for a closest bomber
	{
		iCurrentFrame = 0;
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>* lBigPlanesArray = lObjectManager->GetGameObjectsByType(EObjectTypesBigPlane);
		TInt lCurrentShortestPossibleDistance = -1; //used to identify the first object that we find, don't want to use an hardcoded, possible value
		CGameObject* lCurrentBestTarget = NULL;

		for (TInt lCurrentGameObjects = 0; lCurrentGameObjects < lBigPlanesArray->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lBigPlanesArray->Get(lCurrentGameObjects);

			if (iConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive())
			{
				CAircraft* lCurrentAircraft = static_cast<CAircraft*>(lCurrentGameObject);
				TPointIntFloat lTargetLocation = lCurrentAircraft->GetCurrentPosition();
				TInt lDistanceBetweenTargetLocation = CMath::Abs(iCenterOfView->iX - lTargetLocation.iX).GetIntInBaseInt();

				if (lCurrentShortestPossibleDistance > lDistanceBetweenTargetLocation || lCurrentShortestPossibleDistance == -1) //is the target better than the previous one that we found
				{
					lCurrentShortestPossibleDistance = lDistanceBetweenTargetLocation;
					lCurrentBestTarget = lCurrentAircraft;
				}
			}
		}
		//not needed anymore
		delete lBigPlanesArray;

		//check if we found a valid target
		if (lCurrentBestTarget)
		{
			iCurrentTarget = lCurrentBestTarget;
		}
	}

  //update what angle it has relative to our position
  if(iCurrentTarget)
    {
    //find the middle of the bomber
    TPointIntFloat lBomberMiddlePoint = iCurrentTarget->GetCurrentPosition();
    if(iCurrentTarget->IsReflected())
      lBomberMiddlePoint.iX -= iCurrentTarget->GetWidth()/2;
    else
      lBomberMiddlePoint.iX += iCurrentTarget->GetWidth()/2;

    iAngle = CMath::AngleFromOriginBetweenPoints(*iCenterOfView,lBomberMiddlePoint);
    }
}

void CHeavyBomberLocator::Draw()
{
  if(iCurrentTarget)
    {
  	iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(iAngle,true,GetHeavyBomberLocation());
    iTextureObject->DrawArrays();
    }
}

TPointIntFloat CHeavyBomberLocator::GetHeavyBomberLocation()
{
	TSizeIntFloat lSizeOfScreen =  CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen();
	TPointIntFloat lCentereOfScreen (lSizeOfScreen.iWidth / 2, lSizeOfScreen.iHeight / 2);
	lCentereOfScreen.iX += CMath::GraphicsCosTable(iAngle)*HEAVY_BOMBER_LOCATOR_X_OFFSET - CMath::GraphicsSinTable(iAngle)*HEAVY_BOMBER_LOCATOR_Y_OFFSET ;
	lCentereOfScreen.iY += CMath::GraphicsSinTable(iAngle)*HEAVY_BOMBER_LOCATOR_X_OFFSET + CMath::GraphicsCosTable(iAngle)*HEAVY_BOMBER_LOCATOR_Y_OFFSET ;
  return lCentereOfScreen;
}

void CHeavyBomberLocator::InitilizeHeavyBomberLocator()
{
  CHeavyBomberLocator::LoadHeavyBomberLocatorTextures();
}
void CHeavyBomberLocator::RemoveHeavyBomberLocator()
{
  CHeavyBomberLocator::UnloadHeavyBomberLocatorTextures();
}

void CHeavyBomberLocator::LoadHeavyBomberLocatorTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdHudEnemyBomberLocationArrow);
}

void CHeavyBomberLocator::UnloadHeavyBomberLocatorTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdHudEnemyBomberLocationArrow);
}

void CHeavyBomberLocator::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  aOutputStream.WriteInt32(iAngle);
  aOutputStream.WriteInt32(iConflictSide);
  aOutputStream.WriteInt32(iCurrentFrame);
  iTextureObject->SaveOnDisk(aOutputStream);
  aOutputStream.WriteUint32((TUint32) iCurrentTarget);
}

void CHeavyBomberLocator::LoadFromDisk(CFileReadStream &aReadStream)
{
  iAngle = aReadStream.ReadInt32();
  iConflictSide = static_cast<TConflictSide> (aReadStream.ReadInt32());
  iCurrentFrame = aReadStream.ReadInt32();
  iTextureObject->LoadFromDisk(aReadStream);
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iCurrentTarget));
}
