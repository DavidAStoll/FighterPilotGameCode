/*
 * CImageStore.h
 *
 *  Created on: Jun 12, 2012
 *      Author: dstoll
 */

#ifndef CIMAGESTORE_H_
#define CIMAGESTORE_H_

#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/core/standardLibrary/StandardLibrary.h"


enum TImageId
{
	//------------------- ADD FRONTEND IMAGES HERE -----------------------//

	//------------ Title Screen -----------//
	EImageIdFrontendTileScreen = 0,
	EImageIdFrontendMainMenuBackgroundScreen,
	EImageIdFrontendMainMenuBackgroundExitConfirmationScreen,
	//------------ Choose Campaign Screen -----------//
	EImageIdFrontendChooseCampaignScreenBackgroundScreen,
	EImageIdFrontendChooseCampaignScreenBackgroundScreenSelectionBackground,
	EImageIdFrontendChooseCampaignScreenSelectionBox,
	EImageIdFrontendChooseCampaignScreenSelectionBoxButton,
	EImageIdFrontendChooseCampaignScreenSelectionBoxButtonSelected,
	EImageIdFrontendChooseCampaignImageDauntlessTakeOfFromCarrier,
	EImageIdFrontendChooseCampaignImageIwoJimaLanding,
	EImageIdFrontendChooseCampaignImageJapaneseSurrender,
	EImageIdFrontendChooseCampaignImageMitchellAttackShip,
	EImageIdFrontendChooseCampaignImageSingleDauntless,
	//-------------- CampaignScreen ------------------//
	EImageIdFrontendCampaignScreen,
	EImageIdFrontendCampaignScreenStartLoadButton,
	EImageIdFrontendCampaignScreenStartLoadButtonSelected,
	EImageIdFrontendCampaignScreenBabySymbol,
	EImageIdFrontendCampaignScreenBabySymbolGreyedOut,
	EImageIdFrontendCampaignScreenEasySymbol,
	EImageIdFrontendCampaignScreenEasySymbolGreyedOut,
	EImageIdFrontendCampaignScreenMediumSymbol,
	EImageIdFrontendCampaignScreenMediumSymbolGreyedOut,
	EImageIdFrontendCampaignScreenHardSymbol,
	EImageIdFrontendCampaignScreenHardSymbolGreyedOut,
	//-------------- CampaignBriefingScreen ------------------//
	EImageIdFrontendCampaignBriefingScreen,
	EImageIdFrontendCampaignBriefingScreenMap1941Small,
	EImageIdFrontendCampaignBriefingScreenMap1942Small,
	EImageIdFrontendCampaignBriefingScreenMap1943Small,
	EImageIdFrontendCampaignBriefingScreenMap1944Small,
	EImageIdFrontendCampaignBriefingScreenMap1945Small,
	EImageIdFrontendCampaignDebriefingImageAmericanDisplayFlagSmall,
	EImageIdFrontendCampaignDebriefingImageGilbertArtillerySmall,
	EImageIdFrontendCampaignDebriefingImageHighscoolWaveKamikazeSmall,
	EImageIdFrontendCampaignDebriefingImageJapanSurrenderSmall,
	//-------------- Mission Briefing Screen ------------------//
	EImageIdFrontendMissionBriefingScreen,
	EImageIdFrontendMissionBriefingScreenDifficultyBaby,
	EImageIdFrontendMissionBriefingScreenDifficultyBabySelected,
	EImageIdFrontendMissionBriefingScreenDifficultyBabyGreyedOut,
	EImageIdFrontendMissionBriefingScreenDifficultyEasy,
	EImageIdFrontendMissionBriefingScreenDifficultyEasySelected,
	EImageIdFrontendMissionBriefingScreenDifficultyEasyGreyedOut,
	EImageIdFrontendMissionBriefingScreenDifficultyMedium,
	EImageIdFrontendMissionBriefingScreenDifficultyMediumSelected,
	EImageIdFrontendMissionBriefingScreenDifficultyMediumGreyedOut,
	EImageIdFrontendMissionBriefingScreenDifficultyHard,
	EImageIdFrontendMissionBriefingScreenDifficultyHardSelected,
	EImageIdFrontendMissionBriefingScreenDifficultyHardGreyedOut,
	//-------------- Game Settings Screen ------------------//
	EImageIdFrontendGameSettingsScreen,
	EImageIdFrontendGameSettingsScreenChangeSongButton,
	EImageIdFrontendGameSettingsScreenChangeSongButtonSelected,
	EImageIdFrontendGameSettingsScreenScrollBarButton,
	EImageIdFrontendGameSettingsScreenScrollBarButtonSelected,
	EImageIdFrontendGameSettingsScreenUseButtonIcon,
	EImageIdFrontendGameSettingsScreenUseButtonIconChecked,
	//-------------- Change Song Screen ------------------//
	EImageIdFrontendChangeSongScreen,
	EImageIdFrontendChangeSongScreenSongListTile,
	EImageIdFrontendChangeSongScreenSongListTileSelected,
	//-------------- Select Language Screen------------------//
	EImageIdFrontendSelectLanguageScreen,
	//-------------- Credits Screen------------------//
	EImageIdFrontendCreditsScreenBottomPart,
	EImageIdFrontendCreditsScreenAmericanDestroyer,
	EImageIdFrontendCreditsScreenDauntlessAndVal,
	EImageIdFrontendCreditsScreenJapanese97ChiHa,
	EImageIdFrontendCreditsScreenJapaneseBattleshipYamato,
	EImageIdFrontendCreditsScreenMitchellAndBetty,
	EImageIdFrontendCreditsScreenShermanTanks,
	EImageIdFrontendCreditsScreenShokaku,
	EImageIdFrontendCreditsScreenSoldiers,
	EImageIdFrontendCreditsScreenVictory,
	//-------------- Mission Victory Screen------------------//
	EImageIdFrontendMissionVictoryScreen,
	//-------------- Mission Images ----------------------------//
	EImageIdFrontendMissionImageDauntlessDropsBomb,
	EImageIdFrontendMissionImageDauntlessFlyInFormation,
	EImageIdFrontendMissionImageDauntlessOnAirport,
	EImageIdFrontendMissionImageDauntlessTakesOff,
	EImageIdFrontendMissionImageOkinawaCaveDemolition,
	EImageIdFrontendMissionImageLexingtionBurns,
	EImageIdFrontendMissionImageWaterBuffaloLoaded,
	EImageIdFrontendMissionImageG4MBetty,
	EImageIdFrontendMissionImageJapaneseBattleshipAttacked,
	EImageIdFrontendMissionImageTarawaOnHill,
	EImageIdFrontendMissionImageTransporterAttack,
	EImageIdFrontendMissionImageAmericansLandeAtLosNegros,
	EImageIdFrontendMissionImageShermanTankFlamethrower,
	EImageIdFrontendMissionImageShootDownBurningJapanesePlane,
	EImageIdFrontendMissionImageBunkerHitTwoKamikazes,
	EImageIdFrontendMissionImageIwoJimaFlagRising,
	EImageIdFrontendMissionImageBattleMarianas,
	EImageIdFrontendMissionImageOkinawaBeach,
	EImageIdFrontendMissionImageYamato,
	EImageIdFrontendMissionImageCrewBeforeWildCat,
	//-------------- PopUps ----------------------------//
	EImageIdFrontendPopUpBackground,
	EImageIdFrontendPopUpButton,
	EImageIdFrontendPopUpButtonSelected,
	EImageIdFrontendPopUpProgressBar,
	EImageIdFrontendPopUpTextBackground,
	EImageIdFrontendPopUpWarningSign,
	//-------------- Keyboard ----------------------------//
	EImageIdFrontendKeyboardAcceptButton,
	EImageIdFrontendKeyboardAcceptButtonSelected,
	EImageIdFrontendKeyboardBackspaceButton,
	EImageIdFrontendKeyboardBackspaceButtonSelected,
	EImageIdFrontendKeyboardCapsButton,
	EImageIdFrontendKeyboardCapsButtonSelected,
	EImageIdFrontendKeyboardDefaultBigButton,
	EImageIdFrontendKeyboardDefaultBigButtonSelected,
	EImageIdFrontendKeyboardDefaultButton,
	EImageIdFrontendKeyboardDefaultButtonSelected,
	EImageIdFrontendKeyboardNewLineButton,
	EImageIdFrontendKeyboardNewLineButtonSelected,
	EImageIdFrontendKeyboardSpacebarButton,
	EImageIdFrontendKeyboardSpacebarButtonSelected,
	//------------ Shared -----------//
	EImageIdFrontendSharedMenuNavigationButton,
	EImageIdFrontendSharedMenuNavigationButtonSelected,
	EImageIdFrontendSharedVerticalScrollBarSmall,
	EImageIdFrontendSharedVerticalScrollBarSmallSelected,
	EImageIdFrontendSharedHorizontalScrollBarSmall,
	EImageIdFrontendSharedHorizontalScrollBarSmallSelected,
	EImageIdFrontendSharedDescriptionBackground,
	EImageIdEndOfFrontEndImages, // THIS ID IS USED TO SIGNAL THE END OF FRONTEND IMAGES

	//------------ Company Image --------------//
	EImageIdCompanyImage, //we going to load the Company image separate from all the other Frontend Images

	//-------------- CampaignBriefingScreen On Demand Images ------------------//
	//Don't want to load them by default because they are very large
	EImageIdFrontendCampaignBriefingScreenMap1941,
	EImageIdFrontendCampaignBriefingScreenMap1942,
	EImageIdFrontendCampaignBriefingScreenMap1943,
	EImageIdFrontendCampaignBriefingScreenMap1944,
	EImageIdFrontendCampaignBriefingScreenMap1945,
	EImageIdFrontendCampaignDebriefingImageAmericanDisplayFlag,
	EImageIdFrontendCampaignDebriefingImageGilbertArtillery,
	EImageIdFrontendCampaignDebriefingImageHighscoolWaveKamikaze,
	EImageIdFrontendCampaignDebriefingImageJapanSurrender,
	//---------------------------- Loading Screen Images ---------------------//
	EImageIdLoadingScreenAmericansOnGilberyBigExplosion,
	EImageIdLoadingScreenBunkerHillHitByTwoKamikazes,
	EImageIdLoadingScreenBattleIwoJimaLanding,
	EImageIdLoadingScreenAmericanFleetAndHellDiver,
	EImageIdLoadingScreenParachuteBombAttack,
	EImageIdLoadingScreenPhilippinesShermanTank,
	//-------------------------------- Map Images --------------------------//
	EImageIdMapTutorial1,
	EImageIdMapTutorial2,
	EImageIdMapTutorial3,
	EImageIdMapCampaign1Mission1,
	EImageIdMapCampaign1Mission2,
	EImageIdMapCampaign1Mission3,
	EImageIdMapCampaign1Mission4,
	EImageIdMapCampaign2Mission1,
	EImageIdMapCampaign2Mission2,
	EImageIdMapCampaign2Mission3,
	EImageIdMapCampaign2Mission4,
	EImageIdMapCampaign3Mission1,
	EImageIdMapCampaign3Mission2,
	EImageIdMapCampaign3Mission3,
	EImageIdMapCampaign3Mission4,
	EImageIdMapCampaign4Mission1,
	EImageIdMapCampaign4Mission2,
	EImageIdMapCampaign4Mission3,
	EImageIdMapCampaign4Mission4,
	//-------------------------------- Tutorial Images --------------------------//
	EImageIdTutorialScreen,
	EImageIdTutorialControlsAvoidAA,
	EImageIdTutorialControlsLanding,
	EImageIdTutorialControlsSlowDown,
	EImageIdTutorialControlsSpeedUp,
	EImageIdTutorialControlsStartFromCarrier,
	EImageIdTutorialControlsTurn360Degree,
	EImageIdTutorialControlsTurnLeftAndRight,
	EImageIdTutorialControlsZoomInAndOut,
	EImageIdTutorialWeaponsAttackBomber,
	EImageIdTutorialWeaponsAttackFighter,
	EImageIdTutorialWeaponsDiveBombing,
	EImageIdTutorialWeaponsKillSoldiersUseMachineGun,
	EImageIdTutorialWeaponsProtectSoldiers,
	EImageIdTutorialWeaponsTorpedoAttack,
	EImageIdTutorialWeaponsUse100Kg,
	EImageIdTutorialWeaponsUse500Kg,
	EImageIdTutorialWeaponsUse1000Kg,
	EImageIdTutorialWeaponsUseClusterBomb,
	EImageIdTutorialWeaponsFireButtons,
	EImageIdTutorialExtraAirport,
	EImageIdTutorialExtraComStation,
	EImageIdTutorialExtraTutorialAAGun,
	EImageIdTutorialExtraChangeSongScreen,
	EImageIdTutorialExtraTutorialHud,

	//------------------- ADD IN GAME IMAGES HERE -----------------------//
				//------------------------------------------------------//

	//-------------------------------- Aircrafts --------------------------//
	//Dauntless
	EImageIdAircraftDauntlessNormalSequence,
	EImageIdAircraftDauntlessFireSequence,
	EImageIdAircraftDauntlessTurnSequence,
	EImageIdAircraftDauntlessCrashed,
	EImageIdAircraftDauntlessLandingGear,
	//Mitchell
	EImageIdAircraftMitchellNormalSequence,
	EImageIdAircraftMitchellCrashed,
	EImageIdAircraftMitchellTurnSequence,
	//Val
	EImageIdAircraftValNormalSequence,
	EImageIdAircraftValFireSequence,
	EImageIdAircraftValTurnSequence,
	EImageIdAircraftValCrashed,
	EImageIdAircraftValLandingGear,
	//Betty
	EImageIdAircraftBettyNormalSequence,
	EImageIdAircraftBettyCrashed,
	//Back Cannon
	EImageIdAircraftBackCannon,
	EImageIdAircraftDoubleBackCannon,
	//-------------------------------- BackGrounds --------------------------//
	EImageIdBackgroundBlueDay,
	//-------------------------------- AircraftWeapons --------------------------//
	EImageIdAircraftWeaponCannon20mm,
	EImageIdAircraftWeaponMachineGun13mm,
	EImageIdAircraftWeaponMachineGun8mm,
	//-------------------------------- Bombs --------------------------//
	EImageIdBomb100KgBomb,
	EImageIdBomb500KgBomb,
	EImageIdBomb1000KgBomb,
	EImageIdBombClusterBomb,
	EImageIdBombTorpedo,
	//-------------------------------- Buildings --------------------------//
	//Airport
	EImageIdBuildingAirportMainBuilding,
	EImageIdBuildingAirportMainBuildingBroken1,
	EImageIdBuildingAirportMainBuildingBroken2,
	EImageIdBuildingAirportHangar,
	EImageIdBuildingAirportHangarBroken1,
	EImageIdBuildingAirportHangarBroken2,
	EImageIdBuildingAirportRunway,
	//ComStation
	EImageIdBuildingComStation,
	EImageIdBuildingComStationBroken1,
	EImageIdBuildingComStationBroken2,
	//FlagPole
	EImageIdBuildingFlagPole,
	EImageIdBuildingFlagPoleAmericanFlagSequence,
	EImageIdBuildingFlagPoleJapaneseFlagSequence,
	//HQ
	EImageIdBuildingHQ,
	EImageIdBuildingHQBroken1,
	EImageIdBuildingHQBroken2,
	//AA20mmMachineCannon
	EImageIdBuildingAA20mmMachineCannon,
	EImageIdBuildingAA20mmMachineCannonBroken,
	EImageIdBuildingAA20mmMachineCannonGun,
	//AA75mmType88
	EImageIdBuildingAA75mmType88,
	EImageIdBuildingAA75mmType88Broken,
	EImageIdBuildingAA75mmType88Gun,
	//AA Concrete
	EImageIdBuildingAAConcrete,
	EImageIdBuildingAAConcreteBroken,
	EImageIdBuildingAAConcreteGun,
	//-------------------------------- Bullets --------------------------//
	EImageIdBullet7InchExplosiveShell,
	EImageIdBullet14InchExplosiveShell,
	EImageIdBullet75mmExplosiveShell,
	EImageIdBullet88mmExplosiveShell,
	EImageIdBullet80mmTankShell,
	EImageIdBullet8mmBulletAmerican,
	EImageIdBullet13mmBulletAmerican,
	EImageIdBulletCannon20mmBulletAmerican,
	EImageIdBullet8mmBulletJapanese,
	EImageIdBullet13mmBulletJapanese,
	EImageIdBulletCannon20mmBulletJapanese,
	EImageIdBulletPropelledGrenade,
	EImageIdBulletBulletRifleAmerican,
	EImageIdBulletBulletRifleJapanese,
	//-------------------------------- Clouds --------------------------//
	EImageIdCloudAirplaneSmokeSequence,
	EImageIdCloudSmokeSequence,
	//-------------------------------- Explosions --------------------------//
	EImageIdExplosion14InchShell,
	EImageIdExplosion14InchShellInWater,
	EImageIdExplosion100KgShell,
	EImageIdExplosion100KgShellInWater,
	EImageIdExplosion100KgShellInAir,
	EImageIdExplosion500KgShell,
	EImageIdExplosion500KgShellInWater,
	EImageIdExplosion500KgShellInAir,
	EImageIdExplosion1000KgShell,
	EImageIdExplosion1000KgShellInWater,
	EImageIdExplosion1000KgShellInAir,
	EImageIdExplosionWaterSplash,
	EImageIdExplosionTorpedoGas,
	//-------------------------------- Ground --------------------------//
	EImageIdGroundSandDirtFloor,
	EImageIdGroundWater1,
	EImageIdGroundWater2,
	EImageIdGroundWater3,
	EImageIdGroundWater4,
	//-------------------------------- Ground Units --------------------------//
	//AmericanAAShermanTank
	EImageIdGroundUnitAAShermanTankDriving,
	EImageIdGroundUnitAAShermanTankDestroyed,
	EImageIdGroundUnitAAShermanTankAAGun,
	//American ShermanTank
	EImageIdGroundUnitShermanTankDriving,
	EImageIdGroundUnitShermanTankDestroyed,
	EImageIdGroundUnitShermanTankShooting,
	//American Rifleman
	EImageIdGroundUnitAmericanRiflemanStanding,
	EImageIdGroundUnitAmericanRiflemanWalking,
	EImageIdGroundUnitAmericanRiflemanShooting,
	EImageIdGroundUnitAmericanRiflemanFalling,
	EImageIdGroundUnitAmericanRiflemanDeadOnFloor,
	//American Rocketman
	EImageIdGroundUnitAmericanRocketmanStanding,
	EImageIdGroundUnitAmericanRocketmanWalking,
	EImageIdGroundUnitAmericanRocketmanShooting,
	EImageIdGroundUnitAmericanRocketmanFalling,
	EImageIdGroundUnitAmericanRocketmanDeadOnFloor,
	//Japanese 97 Chi-Ha Tank
	EImageIdGroundUnitJapanese97ChiHaTankDriving,
	EImageIdGroundUnitJapanese97ChiHaTankDestroyed,
	EImageIdGroundUnitJapanese97ChiHaTankShooting,
	//Japanese AA 97 Chi-Ha Tank
	EImageIdGroundUnitJapaneseAA97ChiHaTankDriving,
	EImageIdGroundUnitJapaneseAA97ChiHaTankDestroyed,
	//Japanese Rifleman
	EImageIdGroundUnitJapaneseRiflemanStanding,
	EImageIdGroundUnitJapaneseRiflemanWalking,
	EImageIdGroundUnitJapaneseRiflemanShooting,
	EImageIdGroundUnitJapaneseRiflemanFalling,
	EImageIdGroundUnitJapaneseRiflemanDeadOnFloor,
	//Japanese Rocketman
	EImageIdGroundUnitJapaneseRocketmanStanding,
	EImageIdGroundUnitJapaneseRocketmanWalking,
	EImageIdGroundUnitJapaneseRocketmanShooting,
	EImageIdGroundUnitJapaneseRocketmanFalling,
	EImageIdGroundUnitJapaneseRocketmanDeadOnFloor,
	//-------------------------------- HUD --------------------------//
	EImageIdHudPauseMenuBackground,
	EImageIdHudTopHud,
	EImageIdHudSpeedometer,
	EImageIdHudEnemyBomberLocationArrow,
	EImageIdHudEnemySmallPlaneLocationArrow,
	EImageIdHudArmentHudBombTypes100KgSelected,
	EImageIdHudArmentHudBombTypes500KgSelected,
	EImageIdHudArmentHudBombTypes1000KgSelected,
	EImageIdHudArmentHudBombTypesClusterSelected,
	EImageIdHudArmentHudBombTypesNoneSelected,
	EImageIdHudArmentHudSelectWeaponBombSelected,
	EImageIdHudArmentHudSelectWeaponRocketSelected,
	EImageIdHudArmentHudSelectWeaponTorpedoSelected,
	EImageIdHudArmentHudSelectWeaponNoneSelected,
	EImageIdHudButtonBlankGreen,
	EImageIdHudButtonBlankRed,
	EImageIdHudButtonBlankPushed,
	EImageIdHudButtonBlankDisabled,
	EImageIdHudButtonDecreasedSpeed,
	EImageIdHudButtonDecreasedSpeedPushed,
	EImageIdHudButtonDecreasedSpeedDisabled,
	EImageIdHudButtonIncreaseSpeed,
	EImageIdHudButtonIncreaseSpeedPushed,
	EImageIdHudButtonIncreaseSpeedDisabled,
	EImageIdHudButtonDown,
	EImageIdHudButtonDownPushed,
	EImageIdHudButtonDownDisabled,
	EImageIdHudButtonUp,
	EImageIdHudButtonUpPushed,
	EImageIdHudButtonUpDisabled,
	EImageIdHudButtonLeft,
	EImageIdHudButtonLeftPushed,
	EImageIdHudButtonLeftDisabled,
	EImageIdHudButtonRight,
	EImageIdHudButtonRightPushed,
	EImageIdHudButtonRightDisabled,
	EImageIdHudButtonPrimaryFire,
	EImageIdHudButtonPrimaryFirePushed,
	EImageIdHudButtonPrimaryFireDisabled,
	EImageIdHudButtonSecondaryFire,
	EImageIdHudButtonSecondaryFirePushed,
	EImageIdHudButtonSecondaryFireDisabled,
	EImageIdHudButtonZoomIn,
	EImageIdHudButtonZoomInPushed,
	EImageIdHudButtonZoomInDisabled,
	EImageIdHudButtonZoomOut,
	EImageIdHudButtonZoomOutPushed,
	EImageIdHudButtonZoomOutDisabled,
	EImageIdHudButtonMenu,
	EImageIdHudButtonMenuPushed,
	EImageIdHudButtonMenuDisabled,
	//-------------------------------- Rocket --------------------------//
	EImageIdRocketStandardRocketNormalView,
	EImageIdRocketStandardRocketFireSequence,
	//-------------------------------- Ship --------------------------//
	//American
	EImageIdShipAmericanBattleshipUSSTennessee,
	EImageIdShipAmericanCarrierUSSLexington,
	EImageIdShipAmericanCarrierUSSVictory,
	EImageIdShipAmericanDestroyer,
	EImageIdShipAmericanTroopTransporter,
	EImageIdShipAmericanLCTEmpty,
	EImageIdShipAmericanLCTLoadedWithSoldiers,
	EImageIdShipAmericanLCTLoadedWithTanks,
	EImageIdShipAmericanLCTLoadedWithSoldiersAndTanks,
	//Japanese
	EImageIdShipJapaneseBattleshipYamato,
	EImageIdShipJapaneseCarrierShoho,
	EImageIdShipJapaneseCarrierShokaku,
	EImageIdShipJapaneseCarrierTaiho,
	EImageIdShipJapaneseDestroyer,
	EImageIdShipJapaneseTroopTransporter,
	EImageIdShipJapaneseOilTanker,
	EImageIdShipJapaneseLCTEmpty,
	EImageIdShipJapaneseLCTLoadedWithSoldiers,
	EImageIdShipJapaneseLCTLoadedWithTanks,
	EImageIdShipJapaneseLCTLoadedWithSoldiersAndTanks,
	//Ship Guns
	EImageIdShipGun7InchGunFire,
	EImageIdShipGun7InchGunTurret,
	EImageIdShipGun14InchGunFire,
	EImageIdShipGun14InchGunTurret,
	EImageIdShipGunDual75mmGunFire,
	EImageIdShipGunDual75mmGunTurret,
	EImageIdShipGunDualMachineGunFire,
	EImageIdShipGunDualMachineGunTurret,
	EImageIdShipGunCamoflage7InchGunFire,
	EImageIdShipGunCamoflage7InchGunTurret,
	EImageIdShipGunCamoflageDual75mmGunTurret,
	EImageIdShipGunCamoflageDualMachineGunTurret,
	//-------------------------------- Trees --------------------------//
	EImageIdTreeBigTree,
	EImageIdTreeBigTreeDestroyed,
	EImageIdTreePalmTreeBowed,
	EImageIdTreePalmTreeBowedDestroyed,
	EImageIdTreePalmTreeMiddle,
	EImageIdTreePalmTreeMiddleDestroyed,
	EImageIdTreeSmallTree,
	EImageIdTreeSmallTreeDestroyed,
	EImageIdTreeTallThinTree,
	EImageIdTreeTallThinTreeDestroyed

};

struct SImageStoreEntry
{
	TImageId ImageId;
	CString* ImageFileLocation;
	TGraphicsEngineTextureLoadType LoadMethod;
	SGraphicsEngineTextureId TextureId;
	TSize SizeInPixels;
	TBool ClampImage;
	SBitmap Bitmap; //just used for oversized images
};

class CImageStore
{

public:

	static CImageStore* New();
	~CImageStore();

	//returns the graphics Engine Id for this image
	SGraphicsEngineTextureId& GetImage(TImageId aImageId);
	//returns the size of the Image In Pixels, image has to be loaded
	TSize GetImageSize(TImageId aImageId);
	//returns the Bitmap that holds the Oversized Image
	SBitmap* GetOverSizeImage(TImageId aImageId);
	//loads all Images into OpenGL textures that are part of the frontend
	void LoadAllFrontendImages();
	//loads a specifc image into OpenGL
	void LoadImage(TImageId aImageId);
	//loads an oversized Image
	void LoadOverSizedImage(TImageId aImageId);
	//Unloads a particular image from OpenGL, if it is not loaded this method will have no effect
	//may also be used to unload an oversize image
	void UnloadImage(TImageId aImageId);
	//Unloads all Frontend Images that have been loaded
	void UnloadAllFrontendImages();
	//Unloads all images from OpenGL that have been loaded
	void UnloadAllImages();


private:

	CImageStore();

	//just a shorthand Method to add new Entries to the ImageStore array - for convience
	void Add(TImageId aId, const char* aImageLocation, TGraphicsEngineTextureLoadType aLoadMethod, TBool aClampImage = true);
	//should be called before using any of the ImageStore functions
	void InitializeImageStore();

	CPointerArray<SImageStoreEntry>* iImageStoreEntries;
};

#endif /* CIMAGESTORE_H_ */
