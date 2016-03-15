/*
 ============================================================================
 Name		: LandingSurface.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CLandingSurface implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CLandingSurface.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CMap.h"


CLandingSurface::CLandingSurface(TPoint aLocation, SGameObjectAttributes& aAttributes, const TIntFloat& aWidth, const TIntFloat& aHeight) :
	CGameObject(0, NULL, NULL, aLocation, aAttributes) // the actual location is unimportant since only the hitBoxLocation matters
{
	iDoNotDraw = true;//no Texture, so shouldn't be drawn
	iAlive = true; // not alive
	iWidthOfSurface = aWidth;
	iHeightOfSurface = aHeight;
	iIsMoveableObject = true; //might move around as well
}

CLandingSurface::CLandingSurface(TPoint aLocation, CTextureObject* aTextureObject, TInt aZValue, SGameObjectAttributes& aAttributes, const TIntFloat& aWidth, const TIntFloat& aHeight) :
	CGameObject(aZValue, aTextureObject, NULL, aLocation, aAttributes) // the actual location is unimportant since only the hitBoxLocation matters
{
	iAlive = true; // not alive
	iDoNotDraw = false;
	iWidthOfSurface = aWidth;
	iHeightOfSurface = aHeight;
}

CLandingSurface::~CLandingSurface()
{
	delete iPlanesLandedOnSurface;
}

CLandingSurface* CLandingSurface::New(TPoint aLocation, TConflictSide aSide, const TIntFloat& aWidth, const TIntFloat& aHeight)
{

	SGameObjectAttributes lAttributes;
	lAttributes.Health = LANDING_SURFACE_DEFAULT_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesLandingSurface;
	lAttributes.Reflected = false;
	lAttributes.Armor = LANDING_SURFACE_DEFAULT_ARMOUR;//can't die
	lAttributes.ObjectIdentifier = EGameObjectIdentifierLandingSurface;
	CLandingSurface* self = new CLandingSurface(aLocation, lAttributes, aWidth, aHeight);
	self->ConstructL();
	return self;
}

CLandingSurface* CLandingSurface::New(TPoint aLocation, CTextureObject* aTextureObject, TInt aZValue, TConflictSide aSide, const TIntFloat& aWidth, const TIntFloat& aHeight)
{
	SGameObjectAttributes lAttributes;
	lAttributes.Health = LANDING_SURFACE_DEFAULT_HEALTH;
	lAttributes.ConflictSide = aSide;
	lAttributes.HitBox = NULL;
	lAttributes.ObjectType = EObjectTypesLandingSurface;
	lAttributes.Reflected = false;
	lAttributes.Armor = LANDING_SURFACE_DEFAULT_ARMOUR;
	lAttributes.ObjectIdentifier = EGameObjectIdentifierLandingSurface;
	CLandingSurface* self = new CLandingSurface(aLocation, aTextureObject, aZValue, lAttributes, aWidth, aHeight);
	self->ConstructL();
	return self;
}

void CLandingSurface::ConstructL()
{
	iPlanesLandedOnSurface = CPointerArray<CAircraft>::New();
	TPoint lLocation = TPoint(GetCurrentPosition().iX.GetIntInBaseInt(), GetCurrentPosition().iY.GetIntInBaseInt());

	if(iDoNotDraw) //if we don't have one
		iTextureObject = CTextureObject::New(lLocation, NULL, iWidthOfSurface.GetIntInBaseInt(), iHeightOfSurface.GetIntInBaseInt(), 0, 0, 1, 0, 1); // the texture object is not really used at all

	//create HitBox
	iHitBox = CHitBox::New(&iCoordinates, &iObjectReflected, TSize(iWidthOfSurface.GetIntInBaseInt(), iHeightOfSurface.GetIntInBaseInt()));//uses most of the data from a second object that encapsulates this one except for the location
}

///------------ functions -----------------//


TIntFloat CLandingSurface::GetLandingSurfaceHeightPosition()
{
	return iHeightOfSurface + iCoordinates.iY - 1;
}

void CLandingSurface::SetLandingSurfaceDestroyed()
{
	iAlive = false;
}

void CLandingSurface::UpdateXLocation(TIntFloat aChange)
{
	iCoordinates.iX += aChange;
	//also need to move planes along that are currently sitting on the landing surface
	for(TInt lIndex = 0; lIndex < iPlanesLandedOnSurface->GetCount(); lIndex++)
	{
		iPlanesLandedOnSurface->Get(lIndex)->ChangeXPosition(aChange);
		//special case for player, since the PlayerView needs to be adjusted as well
		if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetCurrentAircaft() == iPlanesLandedOnSurface->Get(lIndex))
		{
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePlayerXLocation(aChange);
		}
	}
}

void CLandingSurface::AddPlaneLandedOnSurface(CAircraft* aPlane)
{
	for(TInt lIndex = 0; lIndex < iPlanesLandedOnSurface->GetCount(); lIndex++)
	{
		if(iPlanesLandedOnSurface->Get(lIndex) == aPlane)
		{
			return; //plane has already been added. Don't add twice
		}
	}
	//plane has not been added yet
	iPlanesLandedOnSurface->Append(aPlane);
}

void CLandingSurface::RemovePlaneLandedOnSurface(CAircraft* aPlane)
{
	for(TInt lIndex = 0; lIndex < iPlanesLandedOnSurface->GetCount(); lIndex++)
	{
		if(iPlanesLandedOnSurface->Get(lIndex) == aPlane)
		{
			iPlanesLandedOnSurface->Remove(lIndex);
			lIndex--; //since array shrinked by one
		}
	}
	//all objects of that pointer should have been removed
}

void CLandingSurface::SaveOnDisk(CFileWriteStream& aOutputStream)
{
	GameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteIntFloat(iWidthOfSurface);
	aOutputStream.WriteIntFloat(iHeightOfSurface);

	CMap::SaveDynamicArray((CPointerArray<void>*) iPlanesLandedOnSurface, aOutputStream);
}

void CLandingSurface::LoadFromDisk(CFileReadStream &aReadStream)
{
	GameObjectLoadContentFromDisk(aReadStream);
	iWidthOfSurface = aReadStream.ReadIntFloat();
	iHeightOfSurface = aReadStream.ReadIntFloat();

	CMap::LoadDynamicArray((CPointerArray<void>*) iPlanesLandedOnSurface, aReadStream);
}
