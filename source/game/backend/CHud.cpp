/*
 ============================================================================
 Name		: Hud.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHud implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CHud.h"
#include "includes/core/utility/CLocalizationEngine.h"
#include "includes/core/graphic/CGraphicsFont.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/backend/CPlayer.h"


CHud::CHud()
{
	iWidthOfHud = TIntFloat::Convert(1024);
	iHeightOfHud = TIntFloat::Convert(51);
}

CHud::~CHud()
{
	if(iTextureObject)
	{
		delete iTextureObject;
		iTextureObject = NULL;
	}
}

CHud* CHud::New()
{
	CHud* self = new  CHud();
	self->ConstructL();
	return self;
}

void CHud::ConstructL()
{
	iStringArmentHudBomb = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudBomb");
	iStringArmentHudRocket = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudRocket");
	iStringArmentHudTorpedo = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudTorpedo");
	iStringArmentHudBombSelection_1000Kg = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudBombSelection_1000kg");
	iStringArmentHudBombSelection_500Kg = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudBombSelection_500kg");
	iStringArmentHudBombSelection_100Kg = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudBombSelection_100kg");
	iStringArmentHudBombSelection_Cluster = CGame::Game->iLocalizationEngine->GetString("Frontend.ArmentHud.ArmentHudBombSelection_cluster");

	//find fitting fonts for arment hud
	CString** lWeaponSelectionStringArray = new  CString* [7];
	lWeaponSelectionStringArray[0] = iStringArmentHudBomb;
	lWeaponSelectionStringArray[1] = iStringArmentHudRocket;
	lWeaponSelectionStringArray[2] = iStringArmentHudTorpedo;
	lWeaponSelectionStringArray[3] = iStringArmentHudBombSelection_1000Kg;
	lWeaponSelectionStringArray[4] = iStringArmentHudBombSelection_500Kg;
	lWeaponSelectionStringArray[5] = iStringArmentHudBombSelection_100Kg;
	lWeaponSelectionStringArray[6] = iStringArmentHudBombSelection_Cluster;
	iFontForWeaponSelection = CGame::Game->iGraphicsFont->CreateFontThatFitsHorizontalStrings(lWeaponSelectionStringArray,7, 144, 55);
	delete lWeaponSelectionStringArray;

	iTextureObject = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdHudTopHud), 1024, 51, HUD_BASIC_Z_VALUE, 0, 1, 0, 1);//will be changed anyway when the hud is displayed

	//setup hit boxes for weapon selection
	iTopMenuBombSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 3) / 3), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT, HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 3); //texture is 128*128, we have three choices, thus the height of each choice is around 43 pixels
	iTopMenuRocketSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 2) / 3), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT,  HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 3);
	iTopMenuTorpedoSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 1) / 3), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT ,		HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 3);

	iBombMenu1000KgSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET + HUD_ARMENT_HUD_WIDTH_AND_HEIGHT + HUD_ARMENT_HUD_DIFFERENCE, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 4) / 4), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT, HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 4); //texture is 128*128, we have four choices, thus the height of each choice is around 32 pixels
	iBombMenu500KgSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET + HUD_ARMENT_HUD_WIDTH_AND_HEIGHT + HUD_ARMENT_HUD_DIFFERENCE, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 3) / 4), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT, HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 4);
	iBombMenu100KgSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET + HUD_ARMENT_HUD_WIDTH_AND_HEIGHT + HUD_ARMENT_HUD_DIFFERENCE, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 2) / 4), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT, HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 4);
	iBombMenuClusterSelectionRect = TRect(TPoint(HUD_ARMENT_HUD_X_OFFSET + HUD_ARMENT_HUD_WIDTH_AND_HEIGHT + HUD_ARMENT_HUD_DIFFERENCE, HUD_ARMENT_HUD_EXTRA_Y_OFFSET + (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT * 1) / 4), HUD_ARMENT_HUD_WIDTH_AND_HEIGHT, HUD_ARMENT_HUD_WIDTH_AND_HEIGHT / 4);
}

//------------------- functions -------------------------------//

void CHud::InitilizeHud()
{
	CHud::LoadHudTextures();
}

void CHud::RemoveHud()
{
	CHud::UnloadHudTextures();
}

void CHud::LoadHudTextures()
{
	CGame::Game->iImageStore->LoadImage(EImageIdHudTopHud);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudBombTypes1000KgSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudBombTypes500KgSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudBombTypes100KgSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudBombTypesClusterSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudBombTypesNoneSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudSelectWeaponBombSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudSelectWeaponRocketSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudSelectWeaponTorpedoSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdHudArmentHudSelectWeaponNoneSelected);
}

void CHud::UnloadHudTextures()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdHudTopHud);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudBombTypes1000KgSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudBombTypes500KgSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudBombTypes100KgSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudBombTypesClusterSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudBombTypesNoneSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudSelectWeaponBombSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudSelectWeaponRocketSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudSelectWeaponTorpedoSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdHudArmentHudSelectWeaponNoneSelected);
}

void CHud::Draw(CPlayer* aPlayer)
{
	//set properties of hud and background hud
	iPosition = TPointIntFloat(TIntFloat::Convert(0), TIntFloat::Convert(609));
	iTextureObject->SetNewLowerLeftCoordinate(iPosition);
	iTextureObject->SetNewSValue(0, 1);
	iTextureObject->SetNewTValue(0, 1);
	iTextureObject->SetHeight(iHeightOfHud);
	iTextureObject->SetWidth(iWidthOfHud);

	/*---------------------- Draw Actual HUD------------------*/
	iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudTopHud));

	//draw Background
	TIntFloat lStartingLocationOfHudBackground = iPosition.iX + iWidthOfHud * HUD_MAP_STARTING_POINT_FACTOR;
	TRectIntFloat lMapBackgroundRect = TRectIntFloat(lStartingLocationOfHudBackground, iPosition.iY + iHeightOfHud, //top left corner
			lStartingLocationOfHudBackground + iWidthOfHud * HUD_PERCENTAGE_OF_HUD_DEDICATED_TO_MAP, iPosition.iY // bottom right corner
			);

	glColor4f(0.615686274, 0.698039215, 0.929411764, 1); //R,G,B,A , some light-dark blue
	CGame::Game->iGraphicsEngine->FillRect(lMapBackgroundRect);

	//draws the map
	DrawMap(aPlayer);

	//draws the hud without secondary weapon and numbers
	iTextureObject->DrawArrays();

	/*---------------------- Draw Secondary Weapon------------------*/
	//calculate Width for secondary Weapon
	TIntFloat lCurrentWidth = iWidthOfHud;
	TIntFloat lCurrentHeight = iHeightOfHud;

	//assign a new texture, depending on what weapon type the secondary weapon of the airplane is
	CWeapon* lSecondaryWeaponOfPlayer = aPlayer->GetCurrentAircaft()->GetSecondaryWeapon();
	TIntFloat lMaxHeightOfSecondaryWeapon = (iHeightOfHud) * HUD_MAX_PERCENTAGE_OF_HUD_HEIGHT_USED_FOR_SECONDARY_WEAPON;
	TIntFloat lMaxWidthOfSecondaryWeapon = lCurrentWidth * HUD_PERCENTAGE_OF_HUD_WIDTH_FOR_SECONDARY_WEAPON;
	if(lSecondaryWeaponOfPlayer)
	{
		switch(lSecondaryWeaponOfPlayer->GetWeaponIdentifier())
		{
			case EWeaponIdentifier1000kgBomb:
			{
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBomb1000KgBomb));
				lCurrentHeight = TIntFloat::Convert(BOMB_1000KG_HEIGHT * 2);
				lCurrentWidth = TIntFloat::Convert(BOMB_1000KG_WIDTH * 2);
				iTextureObject->SetNewSandTValues(0, 0.9, 0, 1);//don't use the entire image since that part is blank
			}
				break;
			case EWeaponIdentifier13mmMachineGun:
			{
				return;//not a valid secondary weapon
			}
				break;
			case EWeaponIdentifier500kgBomb:
			{
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBomb500KgBomb));
				lCurrentHeight = TIntFloat::Convert(BOMB_500KG_HEIGHT * 2);
				lCurrentWidth = TIntFloat::Convert(BOMB_500KG_WIDTH * 2);
				iTextureObject->SetNewSandTValues(0, 0.9375, 0, 1);
			}
				break;

			case EWeaponIdentifier100kgBomb:
			{
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBomb100KgBomb));
				lCurrentHeight = TIntFloat::Convert(BOMB_100KG_HEIGHT * 2);
				lCurrentWidth = TIntFloat::Convert(BOMB_100KG_WIDTH * 2);
				iTextureObject->SetNewSandTValues(0, 0.59375, 0, 0.75);
			}
				break;
			case EWeaponIdentifierClusterBomb:
			{
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBombClusterBomb));
				lCurrentHeight = TIntFloat::Convert(CLUSTER_BOMB_HEIGHT * 2);
				lCurrentWidth = TIntFloat::Convert(CLUSTER_BOMB_WIDTH * 2);
				iTextureObject->SetNewSandTValues(0, 0.625, 0, 0.625);
			}
				break;
			case EWeaponStandardRocket:
			{
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdRocketStandardRocketNormalView));
				lCurrentHeight = TIntFloat::Convert(WEAPON_STANDARD_ROCKET_HEIGHT * 2);
				lCurrentWidth = TIntFloat::Convert(WEAPON_STANDARD_ROCKET_WIDTH * 2);
				iTextureObject->SetNewSandTValues(0, 1, 0, 1);
			}
				break;
			case EWeaponIdentifierTorpedo:
			{
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdBombTorpedo));
				lCurrentHeight = TIntFloat::Convert(TORPEDO_HEIGHT * 2);
				lCurrentWidth = TIntFloat::Convert(TORPEDO_WIDTH * 2);
				iTextureObject->SetNewSandTValues(0, 0.90625, 0, 1);
			}
				break;

		}
	}else
		return;//don't draw any Secondary Weapon with ammunition

	//check if height exceeds maximum height allowed for the weapon picture
	if(lCurrentHeight > lMaxHeightOfSecondaryWeapon)
		lCurrentHeight = lMaxHeightOfSecondaryWeapon;

	if(lCurrentWidth > lMaxWidthOfSecondaryWeapon)
		lCurrentWidth = lMaxWidthOfSecondaryWeapon;

	TIntFloat lSecondaryXLocation = (lMaxWidthOfSecondaryWeapon - lCurrentWidth) / 2; //centers the weapon on the hud
	TIntFloat lLowerYValueForSecondaryWeapon = ((iHeightOfHud) - lCurrentHeight) / 2;//calculates how many pixels of hud are below the weapon image
	//set up Texture Object to draw Secondary Weapon
	iTextureObject->ChangeXCoordinate(lSecondaryXLocation);
	iTextureObject->ChangeYCoordinate(lLowerYValueForSecondaryWeapon);// move Texture coordinate up to center Weapon image on hud, vertically
	iTextureObject->SetHeight(lCurrentHeight);
	iTextureObject->SetWidth(lCurrentWidth);
	iTextureObject->ReflectOverYAxis();
	//draw Secondary Weapon
	iTextureObject->DrawArrays();

	//set back to original configuration
	iTextureObject->ReflectOverYAxis();
	iTextureObject->ChangeYCoordinate(-lLowerYValueForSecondaryWeapon);
	iTextureObject->ChangeXCoordinate(-lSecondaryXLocation);
	iTextureObject->SetNewSandTValues(0, 1, 0, 1);

	/*---------------------- Draw Health, Ammunition left,Lives and Points into Hud's boxes------------------*/
	//draw Numbers depending on Player stats and airplane health
	//set up Y Coordinate for drawing of Numbers
	iTextureObject->ChangeYCoordinate((iHeightOfHud) * TIntFloat::Convert(0.25));

	//retrieve Ammunition Left
	TInt lCurrentAmmunitionLeft = aPlayer->GetCurrentAircaft()->GetAmmunitionLeftOfSecondaryWeapon();
	if(lCurrentAmmunitionLeft > 99)
		lCurrentAmmunitionLeft = 99;
	//tens
	TInt lTents = lCurrentAmmunitionLeft / 10;
	//ones
	TInt lOnes = lCurrentAmmunitionLeft - lTents * 10;
	//Draw Weapon Number1
	DrawNumberOnHud(lTents, 114);
	//Draw Weapon Number2
	DrawNumberOnHud(lOnes, 151);

	TInt lLivesLeft = aPlayer->GetLivesLeft();
	if(lLivesLeft > 9)
		lLivesLeft = 9;
	//Draw Lives left
	DrawNumberOnHud(lLivesLeft, 257);

	//retrieve %Health of Airplane
	TInt lCurrentHealth = aPlayer->GetCurrentAircaft()->GetHealth();
	TInt lMaxHealth = aPlayer->GetCurrentAircaft()->GetMaxHealth();
	TInt lPercentageOfTotalHealth = (lCurrentHealth * 100) / lMaxHealth;

	//tens
	lTents = lPercentageOfTotalHealth / 10;
	//ones
	lOnes = lPercentageOfTotalHealth - lTents * 10;
	if(lPercentageOfTotalHealth > 99) //everything above 99 is shown as 99
	{
		lTents = 9;
		lOnes = 9;
	}
	//Draw Health1
	DrawNumberOnHud(lTents, 347);
	//Draw Health2
	DrawNumberOnHud(lOnes, 385);

	//retrieve Points
	TInt lCurrentPlayerPoints = aPlayer->GetPoints();
	if(lCurrentPlayerPoints > GMAX_NUMBER_OF_POINTS)
		lCurrentPlayerPoints = GMAX_NUMBER_OF_POINTS;//still has more points but can't display more
	else if(lCurrentPlayerPoints < 0)
		lCurrentPlayerPoints = 0; // has negative points, but can't display negative numbers

	//thousands
	TInt lThousand = lCurrentPlayerPoints / 1000;
	//hundreads
	TInt lHundreads = lCurrentPlayerPoints / 100 - lThousand * 10;
	//tens
	lTents = lCurrentPlayerPoints / 10 - lHundreads * 10 - lThousand * 100;
	//ones
	lOnes = lCurrentPlayerPoints - lTents * 10 - lHundreads * 100 - lThousand * 1000;
	//Draw Points1
	DrawNumberOnHud(lThousand, 874);
	//Draw Points2
	DrawNumberOnHud(lHundreads, 912);
	//Draw Points3
	DrawNumberOnHud(lTents, 949);
	//Draw Points4
	DrawNumberOnHud(lOnes, 986);
}

void CHud::DrawNumberOnHud(TInt aNumber, TInt aXPosition)
{
	TPoint lNumberLocation = TPoint(aXPosition, 622);
	SFont lFont = CGame::Game->iGraphicsFont->CreateFont(38);
	CString* lString = new CString("");
	lString->AppendNum(aNumber);
	lNumberLocation.iX += (38 - CGame::Game->iGraphicsFont->GetWidthOfString(lFont, lString)) / 2;

	CGame::Game->iGraphicsFont->SetColourBlack();
	CGame::Game->iGraphicsFont->SetFont(lFont);
	CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lNumberLocation, lString);
	delete lString;

}

TIntFloat CHud::GetObjectWidthOnMap(TIntFloat aObjectWidth)
{
	TIntFloat64 lObjectWidth = aObjectWidth;
	TIntFloat lWidthOfMap = (iWidthOfHud) * HUD_PERCENTAGE_OF_HUD_DEDICATED_TO_MAP;
	return lObjectWidth * (lWidthOfMap / HUD_MAP_WIDTH);//shrink to map propotions
}

TIntFloat CHud::GetObjectHeightOnMap(TIntFloat aObjectHeight)
{
	TIntFloat64 lObjectHeight = aObjectHeight;
	TIntFloat lHeightOfMap = (iHeightOfHud);
	return lObjectHeight * (lHeightOfMap / HUD_MAP_HEIGHT);//shrink to map propotions
}

void CHud::DrawMap(CPlayer* aPlayer)
{
	TIntFloat lXStartingPointOfMap = iPosition.iX + (iWidthOfHud) * HUD_MAP_STARTING_POINT_FACTOR;
	TIntFloat lWidthOfMap = (iWidthOfHud) * HUD_PERCENTAGE_OF_HUD_DEDICATED_TO_MAP;
	TIntFloat lHeightOfMap = (iHeightOfHud);

	CGameObjectManager* lGameObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lGameObjects = lGameObjectManager->ReturnObjectsInRange(aPlayer->GetPosition().iX, TIntFloat::Convert(HUD_MAP_WIDTH) / 2, ~(EObjectTypesProjectile | EObjectTypesProjectileExplosive | EObjectTypesLandingSurface | EObjectTypesNoHitBoxNoTexture | EObjectTypesNotImportant));
	lGameObjects->SortMerge(CGameObject::DetermineObjectDrawOrder);//sort everything after Z value

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		//for each object retrieve position and calculate new position within map with relative width and height
		CGameObject* lCurrentObject = lGameObjects->Get(lCurrentObjectIndex);
		TPointIntFloat lCurrentPosition = lCurrentObject->GetCurrentPosition();
		TIntFloat64 lObjectWidth = lCurrentObject->GetWidth();
		TIntFloat64 lObjectHeight = lCurrentObject->GetHeight();
		if(lCurrentObject->IsReflected())
			lCurrentPosition.iX -= lObjectWidth;//need to substract since its location is centered at its right corner, instead of left corner

		lObjectWidth = lObjectWidth * (lWidthOfMap / HUD_MAP_WIDTH);//shrink to map propotions
		lObjectHeight = lObjectHeight * (lHeightOfMap / HUD_MAP_HEIGHT);//shrink to map propotions
		//need special exception for planes, since they might vanish on the map if they don't have at least a height of 2 Pixels
		if(lCurrentObject->GetGameObjectType() == EObjectTypesPlane)
		{
			if(lObjectHeight < TIntFloat::Convert(2))
				lObjectHeight = TIntFloat::Convert(2);
		}else
		{
			if(lObjectHeight < TIntFloat::Convert(1))
				lObjectHeight = TIntFloat::Convert(1);
		}

		TIntFloat lDistanceFromPlayer = lCurrentPosition.iX - aPlayer->GetPosition().iX;//to set the object relative to the player position
		lCurrentPosition.iX = lXStartingPointOfMap + ((lDistanceFromPlayer / (TIntFloat::Convert(HUD_MAP_WIDTH) / 2)) * (lWidthOfMap / 2)) + lWidthOfMap / 2;//set relative to middle of map
		TIntFloat lHeightFromGround = lCurrentPosition.iY;
		lCurrentPosition.iY = iPosition.iY + ((lHeightFromGround / TIntFloat::Convert(HUD_MAP_HEIGHT)) * lHeightOfMap);//set relative to bottom of map
		lCurrentObject->DrawAtPositionWithValues(lCurrentPosition, lObjectWidth, lObjectHeight);//draw object into map
	}

	//draw diveBomber locator, don't have it on highest difficulty since it makes dive bombing a little bit too easy
	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty != EGameDifficultyHard)
	{
		TIntFloat lDistanceFromPlayer = aPlayer->GetCurrentAircaft()->GetCurrentPosition().iX - aPlayer->GetPosition().iX;//to set the object relative to the player position
		TIntFloat lPlayerXPositionOnMap = lXStartingPointOfMap + ((lDistanceFromPlayer / (TIntFloat::Convert(HUD_MAP_WIDTH) / 2)) * (lWidthOfMap / 2)) + lWidthOfMap / 2;//set relative to middle of map
		TPointIntFloat lLowerPlayerPositionPoint;
		lLowerPlayerPositionPoint.iY = iPosition.iY;
		lLowerPlayerPositionPoint.iX = lPlayerXPositionOnMap;
		TPointIntFloat lUpperPlayerPositionPoint;
		lUpperPlayerPositionPoint.iY = iPosition.iY + lHeightOfMap;
		lUpperPlayerPositionPoint.iX = lPlayerXPositionOnMap;

		//draw Line that indicates where the player is currently located on the map
		glColor4f(0, 0, 0, 1); //R,G,B,A , dark Red
		CGame::Game->iGraphicsEngine->DrawLine(lLowerPlayerPositionPoint, lUpperPlayerPositionPoint);
	}

	//cleanup from heap
	delete lGameObjects;
}

void CHud::InitilizeArmentHud()
{
	iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionNoneSelected;
	iCurrentSelectedArmentSubMenu = EHudArmentNone;
}

void CHud::MoveWeaponTypeSelectionHudUp()
{
	if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionBomb)
		iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionTorpedo;
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionRocket)
		iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionBomb;
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionTorpedo)
		iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionRocket;
}

void CHud::MoveWeaponTypeSelectionHudDown()
{
	if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionBomb)
		iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionRocket;
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionRocket)
		iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionTorpedo;
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionTorpedo)
		iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionBomb;
}
//opens up a submenu, depending what current WeaponType selection is currently selected
void CHud::OpenWeaponSelectionHud()
{
	iCurrentSelectedArmentSubMenu = static_cast<THudArmentCurrentSubMenuSelected> (iCurrentWeaponTypeSelection);
	//sub menu gets opened, set to default selection
	if(iCurrentSelectedArmentSubMenu == EHudArmentBomb)
		iCurrentWeaponSelection = EHudBombTypeNoneSelected;
	else if(iCurrentSelectedArmentSubMenu == EHudArmentRocket)
		iCurrentWeaponSelection = EHudRocketTypeExplosive;
	else
		iCurrentWeaponSelection = EHudNormalTorpedoType;
}
//closes the current sub menu, by setting the sub menu identifier to None
void CHud::CloseWeaponSelectionHud()
{
	iCurrentSelectedArmentSubMenu = EHudArmentNone;
}

void CHud::MoveWeaponSelectionHudDown()
{
	if(iCurrentSelectedArmentSubMenu == EHudArmentBomb)
	{
		if(iCurrentWeaponSelection == EHudBombType1000kg)
			iCurrentWeaponSelection = EHudBombType500kg;
		else if(iCurrentWeaponSelection == EHudBombType500kg)
			iCurrentWeaponSelection = EHudBombType100kg;
		else if(iCurrentWeaponSelection == EHudBombType100kg)
			iCurrentWeaponSelection = EHudBombTypeCluster;
		else if(iCurrentWeaponSelection == EHudBombTypeCluster)
			iCurrentWeaponSelection = EHudBombType1000kg;
	}else if(iCurrentSelectedArmentSubMenu == EHudArmentRocket)
	{
		if(iCurrentWeaponSelection == EHudRocketTypeExplosive)
			iCurrentWeaponSelection = EHudRocketTypeArmorPiercing;
		else if(iCurrentWeaponSelection == EHudRocketTypeArmorPiercing)
			iCurrentWeaponSelection = EHudRocketTypeExplosive;
	}
}

void CHud::MoveWeaponSelectionHudUp()
{
	if(iCurrentSelectedArmentSubMenu == EHudArmentBomb)
	{
		if(iCurrentWeaponSelection == EHudBombType1000kg)
			iCurrentWeaponSelection = EHudBombTypeCluster;
		else if(iCurrentWeaponSelection == EHudBombType500kg)
			iCurrentWeaponSelection = EHudBombType1000kg;
		else if(iCurrentWeaponSelection == EHudBombType100kg)
			iCurrentWeaponSelection = EHudBombType500kg;
		else if(iCurrentWeaponSelection == EHudBombTypeCluster)
			iCurrentWeaponSelection = EHudBombType100kg;
	}else if(iCurrentSelectedArmentSubMenu == EHudArmentRocket)
	{
		if(iCurrentWeaponSelection == EHudRocketTypeExplosive)
			iCurrentWeaponSelection = EHudRocketTypeArmorPiercing;
		else if(iCurrentWeaponSelection == EHudRocketTypeArmorPiercing)
			iCurrentWeaponSelection = EHudRocketTypeExplosive;
	}
}

THudWeaponTypeSelection CHud::GetCurrentTypeSelection()
{
	return iCurrentWeaponTypeSelection;
}

TWeaponIdentifier CHud::GetCurrentSelectedWeapon()
{
	switch(iCurrentWeaponSelection)
	{
		case EHudBombType1000kg:
		{
			return EWeaponIdentifier1000kgBomb;
		}
		case EHudBombType500kg:
		{
			return EWeaponIdentifier500kgBomb;
		}
		case EHudBombType100kg:
		{
			return EWeaponIdentifier100kgBomb;
		}
		case EHudBombTypeCluster:
		{
			return EWeaponIdentifierClusterBomb;
		}
		case EHudRocketTypeExplosive:
		{
			return EWeaponStandardRocket;
		}
		default:
			return EWeaponIdentifier1000kgBomb;
	}
}

void CHud::DrawArmentHud()
{
	//set new S and T values
	iTextureObject->SetNewSandTValues(0, 1, 0, 1);
	//assign right main arment hud texture depending oh what option has been selected
	if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionNoneSelected)
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudSelectWeaponNoneSelected));
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionBomb)
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudSelectWeaponBombSelected));
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionRocket)
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudSelectWeaponRocketSelected));
	else if(iCurrentWeaponTypeSelection == EHudWeaponTypeSelectionTorpedo)
		iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudSelectWeaponTorpedoSelected));

	//set Position for the MainArmentHud
	TPointIntFloat lMainHudPosition;
	iPosition.iX = TIntFloat::Convert(HUD_ARMENT_HUD_X_OFFSET);
	lMainHudPosition.iX = iPosition.iX;
	lMainHudPosition.iY = TIntFloat::Convert(HUD_ARMENT_HUD_EXTRA_Y_OFFSET);
	iTextureObject->DrawAtPositionWithValuesWithDrawArrays(lMainHudPosition, TIntFloat::Convert(HUD_ARMENT_HUD_WIDTH_AND_HEIGHT), TIntFloat::Convert(HUD_ARMENT_HUD_WIDTH_AND_HEIGHT));

	//draw TopSelection strings
	TPoint lTopSelectionLocation (HUD_ARMENT_HUD_X_OFFSET,0);
	lTopSelectionLocation.iY += HUD_ARMENT_HUD_WIDTH_AND_HEIGHT;
	lTopSelectionLocation.iY += HUD_ARMENT_HUD_EXTRA_Y_OFFSET;
	//lTopSelectionLocation.iY -= 10; //border of arment hud
	CGame::Game->iGraphicsFont->SetColourBlack();
	CGame::Game->iGraphicsFont->SetFont(iFontForWeaponSelection);
	TInt lHeightOfTopSelectionFont = iFontForWeaponSelection.HeightAndDescentOfFont.GetIntInBaseInt();

	//draw Bomb string, located at the top
	TPoint lTopWeaponBombStringLocation = lTopSelectionLocation;
	lTopWeaponBombStringLocation.iY -= lHeightOfTopSelectionFont;
	lTopWeaponBombStringLocation.iY -= (85 - (lHeightOfTopSelectionFont)) / 2;
	lTopWeaponBombStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudBomb)) / 2; //center it
	CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lTopWeaponBombStringLocation,iStringArmentHudBomb);
	//draw Rocket string, located at the top
	lTopSelectionLocation.iY -= 85;//skip over Bomb selection, is a bit bigger than the other two sections
	TPoint lTopWeaponRocketStringLocation = lTopSelectionLocation;
	lTopWeaponRocketStringLocation.iY -= lHeightOfTopSelectionFont;
	lTopWeaponRocketStringLocation.iY -= (85 - (lHeightOfTopSelectionFont)) / 2;
	lTopWeaponRocketStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudRocket)) / 2; //center it
	CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lTopWeaponRocketStringLocation,iStringArmentHudRocket);
	//draw Torpedo string, located at the top
	lTopSelectionLocation.iY -= 85;//skip over Rocket selection, need to add 3 since we leave extra space in the middle
	TPoint lTopWeaponTorpedoStringLocation = lTopSelectionLocation;
	lTopWeaponTorpedoStringLocation.iY -= lHeightOfTopSelectionFont;
	lTopWeaponTorpedoStringLocation.iY -= (85 - (lHeightOfTopSelectionFont)) / 2;
	lTopWeaponTorpedoStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudTorpedo)) / 2; //center it
	CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lTopWeaponTorpedoStringLocation,iStringArmentHudTorpedo);

	//check if we need to draw subMenuArmentHud
	if(iCurrentSelectedArmentSubMenu != EHudArmentNone)
	{
		//set location where the sub arment menu will be drawn
		TPointIntFloat lSubMenuHudPosition;
		lSubMenuHudPosition.iX = TIntFloat::Convert(HUD_ARMENT_HUD_X_OFFSET + HUD_ARMENT_HUD_WIDTH_AND_HEIGHT + HUD_ARMENT_HUD_DIFFERENCE);
		lSubMenuHudPosition.iY = TIntFloat::Convert(HUD_ARMENT_HUD_EXTRA_Y_OFFSET);

		//find out what sub menu is currently selected
		if(iCurrentSelectedArmentSubMenu == EHudArmentBomb)
		{
			if(iCurrentWeaponSelection == EHudBombTypeNoneSelected)
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudBombTypesNoneSelected));
			else if(iCurrentWeaponSelection == EHudBombType1000kg)
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudBombTypes1000KgSelected));
			else if(iCurrentWeaponSelection == EHudBombType500kg)
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudBombTypes500KgSelected));
			else if(iCurrentWeaponSelection == EHudBombType100kg)
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudBombTypes100KgSelected));
			else if(iCurrentWeaponSelection == EHudBombTypeCluster)
				iTextureObject->AssignTexture(&CGame::Game->iImageStore->GetImage(EImageIdHudArmentHudBombTypesClusterSelected));
			//draw the arment sub menu
			iTextureObject->DrawAtPositionWithValuesWithDrawArrays(lSubMenuHudPosition, TIntFloat::Convert(HUD_ARMENT_HUD_WIDTH_AND_HEIGHT), TIntFloat::Convert(HUD_ARMENT_HUD_WIDTH_AND_HEIGHT));

			//draw TopSelection strings
			TPoint lBombSelectionLocation (HUD_ARMENT_HUD_X_OFFSET + HUD_ARMENT_HUD_WIDTH_AND_HEIGHT + HUD_ARMENT_HUD_DIFFERENCE, 0);
			lBombSelectionLocation.iY += HUD_ARMENT_HUD_WIDTH_AND_HEIGHT;
			lBombSelectionLocation.iY += HUD_ARMENT_HUD_EXTRA_Y_OFFSET;
			//lBombSelectionLocation.iY -= 10; //border of arment hud
			CGame::Game->iGraphicsFont->SetColourBlack();
			CGame::Game->iGraphicsFont->SetFont(iFontForWeaponSelection);
			TInt lHeightOfBombSelectionFont = iFontForWeaponSelection.HeightAndDescentOfFont.GetIntInBaseInt();

			//draw 1000 kg Bomb string, located at the top
			TPoint l1000KgBombStringLocation = lBombSelectionLocation;
			l1000KgBombStringLocation.iY -= lHeightOfBombSelectionFont;
			l1000KgBombStringLocation.iY -= (60 - (lHeightOfBombSelectionFont)) / 2;
			l1000KgBombStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudBombSelection_1000Kg)) / 2; //center it
			CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(l1000KgBombStringLocation,iStringArmentHudBombSelection_1000Kg);
			//draw 500 kg Bomb string, located at the top
			lBombSelectionLocation.iY -= 60;
			TPoint l500KgBombStringLocation = lBombSelectionLocation;
			l500KgBombStringLocation.iY -= lHeightOfBombSelectionFont;
			l500KgBombStringLocation.iY -= (60 - (lHeightOfBombSelectionFont)) / 2;
			l500KgBombStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudBombSelection_500Kg)) / 2; //center it
			CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(l500KgBombStringLocation,iStringArmentHudBombSelection_500Kg);
			//draw 100 kg Bomb string, located at the top
			lBombSelectionLocation.iY -= 60;
			TPoint l100KgBombStringLocation = lBombSelectionLocation;
			l100KgBombStringLocation.iY -= lHeightOfBombSelectionFont;
			l100KgBombStringLocation.iY -= (60 - (lHeightOfBombSelectionFont)) / 2;
			l100KgBombStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudBombSelection_100Kg)) / 2; //center it
			CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(l100KgBombStringLocation,iStringArmentHudBombSelection_100Kg);
			//draw Cluster Bomb string, located at the top
			lBombSelectionLocation.iY -= 60;
			TPoint lClusterBombStringLocation = lBombSelectionLocation;
			lClusterBombStringLocation.iY -= lHeightOfBombSelectionFont;
			lClusterBombStringLocation.iY -= (60 - (lHeightOfBombSelectionFont)) / 2;
			lClusterBombStringLocation.iX += (HUD_ARMENT_HUD_WIDTH_AND_HEIGHT - CGame::Game->iGraphicsFont->GetWidthOfString(iFontForWeaponSelection, iStringArmentHudBombSelection_Cluster)) / 2; //center it
			CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(lClusterBombStringLocation,iStringArmentHudBombSelection_Cluster);

		}else if(iCurrentSelectedArmentSubMenu == EHudArmentRocket)
		{

		}else if(iCurrentSelectedArmentSubMenu == EHudArmentTorpedo)
		{

		}
	}
}

TBool CHud::HandleTouchEvent(const STouchEventLocationConverted& aTouchEvent, CAircraft* aAircraftToArm)
{
	TPoint lTouchPosition = aTouchEvent.VirtualLocation;

	if(aTouchEvent.Type == ETouchEventHandlerTouchTypeDown || aTouchEvent.Type == ETouchEventHandlerTouchTypeDrag)
	{
		if(iTopMenuBombSelectionRect.Contains(lTouchPosition))
		{
			iCurrentWeaponTypeSelection = EHudWeaponTypeSelectionBomb;
			OpenWeaponSelectionHud();
			return true;
		}else if(iTopMenuRocketSelectionRect.Contains(lTouchPosition))
		{
			iCurrentWeaponTypeSelection	= EHudWeaponTypeSelectionRocket;
			aAircraftToArm->EquipWithNewSecondaryWeapon(EWeaponStandardRocket);
			CloseWeaponSelectionHud();
			return true;
		}else if(iTopMenuTorpedoSelectionRect.Contains(lTouchPosition))
		{
			iCurrentWeaponTypeSelection	= EHudWeaponTypeSelectionTorpedo;
			aAircraftToArm->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo);
			CloseWeaponSelectionHud();
			return true;
		}
		//this case only work if the submenu arment hud is open as well
		else if(FocusedOnSubMenuArmentHud())
		{
			if(iBombMenu1000KgSelectionRect.Contains(lTouchPosition))
			{
				iCurrentWeaponSelection = EHudBombType1000kg;
				aAircraftToArm->EquipWithNewSecondaryWeapon(EWeaponIdentifier1000kgBomb);
				return true;
			}else if(iBombMenu500KgSelectionRect.Contains(lTouchPosition))
			{
				iCurrentWeaponSelection = EHudBombType500kg;
				aAircraftToArm->EquipWithNewSecondaryWeapon(EWeaponIdentifier500kgBomb);
				return true;
			}else if(iBombMenu100KgSelectionRect.Contains(lTouchPosition))
			{
				iCurrentWeaponSelection = EHudBombType100kg;
				aAircraftToArm->EquipWithNewSecondaryWeapon(EWeaponIdentifier100kgBomb);
				return true;
			}else if(iBombMenuClusterSelectionRect.Contains(lTouchPosition))
			{
				iCurrentWeaponSelection = EHudBombTypeCluster;
				aAircraftToArm->EquipWithNewSecondaryWeapon(EWeaponIdentifierClusterBomb);
				return true;
			}
		}
	}
	return false;
}

TBool CHud::FocusedOnSubMenuArmentHud()
{
	if(iCurrentSelectedArmentSubMenu == EHudArmentNone)
		return false;
	else
		return true;
}

void CHud::SaveToDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteIntFloat(iPosition.iX);
	aOutputStream.WriteIntFloat(iPosition.iY);
	aOutputStream.WriteIntFloat(iWidthOfHud);
	aOutputStream.WriteIntFloat(iHeightOfHud);
	aOutputStream.WriteInt32(iCurrentWeaponTypeSelection);
	aOutputStream.WriteInt32(iCurrentSelectedArmentSubMenu);
	aOutputStream.WriteInt32(iCurrentWeaponSelection);
	iTextureObject->SaveOnDisk(aOutputStream);
}
void CHud::LoadFromDisk(CFileReadStream &aReadStream)
{
	iPosition.iX = aReadStream.ReadIntFloat();
	iPosition.iY = aReadStream.ReadIntFloat();
	iWidthOfHud = aReadStream.ReadIntFloat();
	iHeightOfHud = aReadStream.ReadIntFloat();
	iCurrentWeaponTypeSelection = static_cast<THudWeaponTypeSelection> (aReadStream.ReadInt32());
	iCurrentSelectedArmentSubMenu = static_cast<THudArmentCurrentSubMenuSelected> (aReadStream.ReadInt32());
	iCurrentWeaponSelection = static_cast<THudWeaponSelection> (aReadStream.ReadInt32());
	iTextureObject->LoadFromDisk(aReadStream);
}
