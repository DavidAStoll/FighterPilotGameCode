/*
 * CHud.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CHUD_H_
#define CHUD_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/game/backend/basicClasses/CWeapon.h"
#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/graphic/CGraphicsFont.h"

class CAircraft;

// CLASS DECLARATION

#define HUD_HEIGHT_NORMAL_ZOOM 32
#define HUD_PERCENTAGE_OF_HUD_WIDTH_FOR_SECONDARY_WEAPON 0.106
#define HUD_MAX_PERCENTAGE_OF_HUD_HEIGHT_USED_FOR_SECONDARY_WEAPON 0.9
#define HUD_MAP_WIDTH 2500
#define HUD_MAP_HEIGHT 350
#define HUD_MAP_STARTING_POINT_FACTOR (0.41875) //depends on iWidth
#define HUD_PERCENTAGE_OF_HUD_DEDICATED_TO_MAP (0.43125)
#define HUD_ARMENT_HUD_EXTRA_Y_OFFSET 20 // this value will move the arment hud Up, to be drawn at a higher Y value
#define HUD_ARMENT_HUD_WIDTH_AND_HEIGHT 256
#define HUD_ARMENT_HUD_X_OFFSET 80
#define HUD_ARMENT_HUD_DIFFERENCE 10

//Arment HUD
#define ARMENT_HUD_BOMB_TYPES_OPTIONS 5
#define ARMENT_HUD_ROCKET_TYPES_OPTIONS 0
#define ARMENT_HUD_TORPEDO_TYPES_OPTIONS 0
#define ARMENT_HUD_WEAPON_OPTIONS 4

enum THudWeaponTypeSelection
{
	EHudWeaponTypeSelectionBomb = 0, EHudWeaponTypeSelectionRocket, EHudWeaponTypeSelectionTorpedo, EHudWeaponTypeSelectionNoneSelected
};
enum THudWeaponSelection
{
	EHudBombType1000kg = 0, EHudBombType500kg, EHudBombType100kg, EHudBombTypeCluster, EHudBombTypeNoneSelected, EHudRocketTypeExplosive, EHudRocketTypeArmorPiercing, EHudNormalTorpedoType
};
enum THudArmentCurrentSubMenuSelected
{
	EHudArmentBomb = 0, EHudArmentRocket, EHudArmentTorpedo, EHudArmentNone
};
enum THudArmentSubMenuBombTextures
{
	EHudBombType1000kgTexture = 0, EHudBombType500kgTexture, EHudBombType100kgTexture, EHudBombTypeClusterTexture, EHudBombTypeNoneSelectedTexture
};
enum THudArmentSubMenuRocketTextures
{
	EHudRocketTypeExplosiveTexture = 0, EHudRocketTypeArmorPiercingTexture, EHudRocketTypeNoneSelectedTexture
};
enum THudArmentSubMenuTorpedoTextures
{
	EHudNormalTorpedoTypeTexture = 0, EHudTorpedoTypeNoneSelectedTexture
};


class CPlayer;//forward declaration

class CHud
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHud();
	static CHud* New();
	static void InitilizeHud();
	static void RemoveHud();
	TIntFloat GetObjectWidthOnMap(TIntFloat aObjectWidth);
	TIntFloat GetObjectHeightOnMap(TIntFloat aObjectHeight);

	void Draw(CPlayer* aPlayer);

	void InitilizeArmentHud();//resets some values for displaying arment hud the first time it is called after being closed
	void MoveWeaponTypeSelectionHudUp();
	void MoveWeaponTypeSelectionHudDown();
	void MoveWeaponSelectionHudUp();
	void MoveWeaponSelectionHudDown();
	void OpenWeaponSelectionHud();
	void CloseWeaponSelectionHud();
	//returns what current weapon is currently selected in the arment hud
	TWeaponIdentifier GetCurrentSelectedWeapon();
	THudWeaponTypeSelection GetCurrentTypeSelection();
	void DrawArmentHud();
	TBool FocusedOnSubMenuArmentHud();
	TBool HandleTouchEvent(const STouchEventLocationConverted& aTouchEvent, CAircraft* aAircraftToArm);
	void SaveToDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	static void LoadHudTextures();
	static void UnloadHudTextures();
	void DrawMap(CPlayer* aPlayer);
	void DrawNumberOnHud(TInt aNumber, TInt aXPosition);
	/**
	 * Constructor for performing 1st stage construction
	 */
	CHud();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	SFont iFontForWeaponSelection;
	CTextureObject* iTextureObject;
	TPointIntFloat iPosition;
	TIntFloat iWidthOfHud;
	TIntFloat iHeightOfHud;

	CString* iStringArmentHudBomb;
	CString* iStringArmentHudRocket;
	CString* iStringArmentHudTorpedo;
	CString* iStringArmentHudBombSelection_1000Kg;
	CString* iStringArmentHudBombSelection_500Kg;
	CString* iStringArmentHudBombSelection_100Kg;
	CString* iStringArmentHudBombSelection_Cluster;

	TRect iTopMenuBombSelectionRect;
	TRect iTopMenuRocketSelectionRect;
	TRect iTopMenuTorpedoSelectionRect;
	TRect iBombMenu1000KgSelectionRect;
	TRect iBombMenu500KgSelectionRect;
	TRect iBombMenu100KgSelectionRect;
	TRect iBombMenuClusterSelectionRect;
	//for armentHud, what option is currently selected
	THudWeaponTypeSelection iCurrentWeaponTypeSelection;
	THudArmentCurrentSubMenuSelected iCurrentSelectedArmentSubMenu;
	THudWeaponSelection iCurrentWeaponSelection;

};


#endif /* CHUD_H_ */
