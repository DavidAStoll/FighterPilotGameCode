
/*
 ============================================================================
 Name		: SmallPlaneLocator.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CSmallPlaneLocator implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CGameObjectManager.h"
#include "includes/game/backend/CSmallPlaneLocator.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/core/utility/CImageStore.h"

CSmallPlaneLocator::CSmallPlaneLocator(TPointIntFloat* aCenterOfView,TConflictSide aConflictSide)
	{
	iCenterOfView = aCenterOfView;
	iAngle = 0;
	iCurrentFrame = 0;
	iConflictSide = aConflictSide;
	iCurrentTarget = NULL;
	}

CSmallPlaneLocator::~CSmallPlaneLocator()
	{
  if(iTextureObject)
    {
    delete iTextureObject;
    }
	}

CSmallPlaneLocator* CSmallPlaneLocator::New(TPointIntFloat* aCenterOfView,TConflictSide aConflictSide)
        {
        CSmallPlaneLocator* self = new CSmallPlaneLocator(aCenterOfView,aConflictSide);;
        self->ConstructL();
        return self;
        }

void CSmallPlaneLocator::ConstructL()
{
  iTextureObject = CTextureObject::New(TPoint(0,0),&CGame::Game->iImageStore->GetImage(EImageIdHudEnemySmallPlaneLocationArrow), SMALL_PLANE_LOCATOR_WIDTH,SMALL_PLANE_LOCATOR_HEIGHT,0,0,1,0,1);//will be changed anyway when the hud is displayed
}

//-------------------- functions -----------------------//

void CSmallPlaneLocator::Update()
{
  //look for aircraft and update angle
  if(iCurrentTarget && (!iCurrentTarget->IsAlive() || iCurrentTarget->RecycleObject())) // check if our current target is still a valid one
    iCurrentTarget = NULL;

  iCurrentFrame++;
  if(!iCurrentTarget || iCurrentFrame == SMALL_PLANE_LOCATOR_FRAMES_UNTIL_CHECK_FOR_CLOSER_TARGET) // look for a closest bomber
    {
  	iCurrentFrame=0;
    CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
    CPointerArray<CGameObject>* lGameObjects = lObjectManager->GetGameObjectsByType(EObjectTypesSmallPlane);
    TInt lCurrentShortestPossibleDistance=-1;//used to identify the first object that we find, don't want to use an hardcoded, possible value
    CGameObject* lCurrentBestTarget = NULL;

		for (TInt lCurrentGameObjects = 0; lCurrentGameObjects < lGameObjects->GetCount(); lCurrentGameObjects++)
		{
			CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentGameObjects);

			if (iConflictSide != lCurrentGameObject->GetConflictSide() && lCurrentGameObject->IsAlive()
					&& (lCurrentGameObject))
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
		delete lGameObjects;
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

void CSmallPlaneLocator::Draw()
{
  if(iCurrentTarget)
    {
  	iTextureObject->RotateTextureObjectToAngleAndAdjustToPoint(iAngle, true, GetSmallPlaneLocation());
    iTextureObject->DrawArrays();
    }
}

TPointIntFloat CSmallPlaneLocator::GetSmallPlaneLocation()
{
	TSizeIntFloat lSizeOfScreen =  CGame::Game->iGraphicsEngine->GetVirtualDimensionsOfScreen();
	TPointIntFloat lCentereOfScreen (lSizeOfScreen.iWidth / 2, lSizeOfScreen.iHeight / 2);
	lCentereOfScreen.iX += CMath::GraphicsCosTable(iAngle)*SMALL_PLANE_LOCATOR_X_OFFSET - CMath::GraphicsSinTable(iAngle)*SMALL_PLANE_LOCATOR_Y_OFFSET ;
	lCentereOfScreen.iY += CMath::GraphicsSinTable(iAngle)*SMALL_PLANE_LOCATOR_X_OFFSET + CMath::GraphicsCosTable(iAngle)*SMALL_PLANE_LOCATOR_Y_OFFSET ;
  return lCentereOfScreen;
}


void CSmallPlaneLocator::InitilizeSmallPlaneLocator()
{
  CSmallPlaneLocator::LoadSmallPlaneLocatorTextures();
}
void CSmallPlaneLocator::RemoveSmallPlaneLocator()
{
  CSmallPlaneLocator::UnloadSmallPlaneLocatorTextures();
}

void CSmallPlaneLocator::LoadSmallPlaneLocatorTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdHudEnemySmallPlaneLocationArrow);
}

void CSmallPlaneLocator::UnloadSmallPlaneLocatorTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdHudEnemySmallPlaneLocationArrow);
}

void CSmallPlaneLocator::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  aOutputStream.WriteInt32(iAngle);
  aOutputStream.WriteInt32(iConflictSide);
  aOutputStream.WriteInt32(iCurrentFrame);
  iTextureObject->SaveOnDisk(aOutputStream);
  aOutputStream.WriteUint32((TUint32) iCurrentTarget);
}

void CSmallPlaneLocator::LoadFromDisk(CFileReadStream &aReadStream)
{
  iAngle = aReadStream.ReadInt32();
  iConflictSide = static_cast<TConflictSide> (aReadStream.ReadInt32());
  iCurrentFrame = aReadStream.ReadInt32();
  iTextureObject->LoadFromDisk(aReadStream);
  CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddNewPointerSettingInstruction((TUint32)aReadStream.ReadUint32(),(TUint32)(&iCurrentTarget));
}
