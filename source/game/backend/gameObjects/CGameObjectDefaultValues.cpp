/*
 * GameObjectDefaultValues.cpp
 *
 *  Created on: 2011-01-03
 *      Author: dstoll
 */
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/gameObjects/GameObjectDefaultValuesBaby.h"
#include "includes/game/backend/gameObjects/GameObjectDefaultValuesEasy.h"
#include "includes/game/backend/gameObjects/GameObjectDefaultValuesMedium.h"
#include "includes/game/backend/gameObjects/GameObjectDefaultValuesHard.h"

TInt CGameObjectDefaultValues::PlayerLives;
TInt CGameObjectDefaultValues::DauntlessHealth;
TInt CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad;
TInt CGameObjectDefaultValues::Dauntless500KgBombMaxLoad;
TInt CGameObjectDefaultValues::Dauntless100KgBombMaxLoad;
TInt CGameObjectDefaultValues::DauntlessClusterBombMaxLoad;
TInt CGameObjectDefaultValues::DauntlessRocketMaxLoad;
TInt CGameObjectDefaultValues::DauntlessTorpedoMaxLoad;
TInt CGameObjectDefaultValues::MitchellHealth;
TInt CGameObjectDefaultValues::Mitchell1000KgBombMaxLoad;
TInt CGameObjectDefaultValues::Mitchell500KgBombMaxLoad;

TInt CGameObjectDefaultValues::ValHealth;
TInt CGameObjectDefaultValues::Val1000KgBombMaxLoad;
TInt CGameObjectDefaultValues::Val500KgBombMaxLoad;
TInt CGameObjectDefaultValues::Val100KgBombMaxLoad;
TInt CGameObjectDefaultValues::ValClusterBombMaxLoad;
TInt CGameObjectDefaultValues::ValRocketMaxLoad;
TInt CGameObjectDefaultValues::ValTorpedoMaxLoad;
TInt CGameObjectDefaultValues::BettyHealth;
TInt CGameObjectDefaultValues::Betty1000KgBombMaxLoad;
TInt CGameObjectDefaultValues::Betty500KgBombMaxLoad;

TInt CGameObjectDefaultValues::USSVictoryHealth;
TInt CGameObjectDefaultValues::AmericanLCTHealth;
TInt CGameObjectDefaultValues::AmericanCarrierHealth;
TInt CGameObjectDefaultValues::AmericanDestroyerHealth;
TInt CGameObjectDefaultValues::AmericanBattleshipUSSTennesseeHealth;
TInt CGameObjectDefaultValues::AmericanTroopTransporterHealth;
TInt CGameObjectDefaultValues::JapaneseLCTHealth;
TInt CGameObjectDefaultValues::JapaneseCarrierHealth;
TInt CGameObjectDefaultValues::JapaneseDestroyerHealth;
TInt CGameObjectDefaultValues::JapaneseBattleshipYamatoHealth;
TInt CGameObjectDefaultValues::JapaneseTroopTransporterHealth;
TInt CGameObjectDefaultValues::JapaneseOilTankerHealth;




void CGameObjectDefaultValues::LoadDefaultValues()
{
 if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyBaby)
	 LoadDefaultValuesBaby();
 else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyEasy)
   LoadDefaultValuesEasy();
 else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyMedium)
   LoadDefaultValuesMedium();
 else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyHard)
   LoadDefaultValuesHard();
}

void CGameObjectDefaultValues::LoadDefaultValuesBaby()
{
	LoadDefaultValuesEasy();

	//override some values only
	//------------------------ Player --------------------------------- //
	CGameObjectDefaultValues::PlayerLives = PLAYER_NUMBER_OF_LIVES_BABY;
  //Dauntless
  CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad = DAUNTLESS_1000KG_BOMB_MAX_LOAD_BABY;
  CGameObjectDefaultValues::Dauntless500KgBombMaxLoad = DAUNTLESS_500KG_BOMB_MAX_LOAD_BABY;
  CGameObjectDefaultValues::Dauntless100KgBombMaxLoad = DAUNTLESS_100KG_BOMB_MAX_LOAD_BABY;
  CGameObjectDefaultValues::DauntlessClusterBombMaxLoad = DAUNTLESS_CLUSTER_BOMB_MAX_LOAD_BABY;
  CGameObjectDefaultValues::DauntlessRocketMaxLoad = DAUNTLESS_STANDARD_ROCKET_MAX_LOAD_BABY;
  CGameObjectDefaultValues::DauntlessTorpedoMaxLoad = DAUNTLESS_TORPEDO_MAX_LOAD_BABY;
}

void CGameObjectDefaultValues::LoadDefaultValuesEasy()
{
	//------------------------ Player --------------------------------- //
	CGameObjectDefaultValues::PlayerLives = PLAYER_NUMBER_OF_LIVES_EASY;

  //--------------------Airplanes-----------------------------------//
  //Dauntless
  CGameObjectDefaultValues::DauntlessHealth = DAUNTLESS_DEFAULT_HEALTH_EASY;
  CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad = DAUNTLESS_1000KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::Dauntless500KgBombMaxLoad = DAUNTLESS_500KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::Dauntless100KgBombMaxLoad = DAUNTLESS_100KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::DauntlessClusterBombMaxLoad = DAUNTLESS_CLUSTER_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::DauntlessRocketMaxLoad = DAUNTLESS_STANDARD_ROCKET_MAX_LOAD_EASY;
  CGameObjectDefaultValues::DauntlessTorpedoMaxLoad = DAUNTLESS_TORPEDO_MAX_LOAD_EASY;
  //Mitchell
  CGameObjectDefaultValues::MitchellHealth = MITCHELL_DEFAULT_HEALTH_EASY;
  CGameObjectDefaultValues::Mitchell1000KgBombMaxLoad = MITCHELL_1000KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::Mitchell500KgBombMaxLoad = MITCHELL_500KG_BOMB_MAX_LOAD_EASY;
  //Val
  CGameObjectDefaultValues::ValHealth = VAL_DEFAULT_HEALTH_EASY;
  CGameObjectDefaultValues::Val1000KgBombMaxLoad = VAL_1000KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::Val500KgBombMaxLoad = VAL_500KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::Val100KgBombMaxLoad = VAL_100KG_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::ValClusterBombMaxLoad = VAL_CLUSTER_BOMB_MAX_LOAD_EASY;
  CGameObjectDefaultValues::ValRocketMaxLoad = VAL_STANDARD_ROCKET_MAX_LOAD_EASY;
  CGameObjectDefaultValues::ValTorpedoMaxLoad = VAL_TORPEDO_MAX_LOAD_EASY;
  //Betty
	CGameObjectDefaultValues::BettyHealth = BETTY_DEFAULT_HEALTH_EASY;
	CGameObjectDefaultValues::Betty1000KgBombMaxLoad = BETTY_1000KG_BOMB_MAX_LOAD_EASY;
	CGameObjectDefaultValues::Betty500KgBombMaxLoad = BETTY_500KG_BOMB_MAX_LOAD_EASY;

    //--------------------Ships-----------------------------------//
  CGameObjectDefaultValues::USSVictoryHealth = USS_VICTORY_HEALTH_EASY;
  CGameObjectDefaultValues::AmericanLCTHealth = AMERICAN_LCT_HEALTH_EASY;
  CGameObjectDefaultValues::AmericanCarrierHealth = AMERICAN_CARRIER_HEALTH_EASY;
  CGameObjectDefaultValues::AmericanDestroyerHealth = AMERICAN_DESTROYER_HEALTH_EASY;
  CGameObjectDefaultValues::AmericanBattleshipUSSTennesseeHealth = AMERICAN_BATTLESHIP_USS_TENNESSEE_HEALTH_EASY;
  CGameObjectDefaultValues::AmericanTroopTransporterHealth = AMERICAN_TROOP_TRANSPORTER_HEALTH_EASY;
  CGameObjectDefaultValues::JapaneseLCTHealth = JAPANESE_LCT_HEALTH_EASY;
  CGameObjectDefaultValues::JapaneseCarrierHealth = JAPANESE_CARRIER_HEALTH_EASY;
  CGameObjectDefaultValues::JapaneseDestroyerHealth = JAPANESE_DESTROYER_HEALTH_EASY;
  CGameObjectDefaultValues::JapaneseBattleshipYamatoHealth = JAPANESE_BATTLESHIP_YAMATO_HEALTH_EASY;
  CGameObjectDefaultValues::JapaneseTroopTransporterHealth = JAPANESE_TROOP_TRANSPORTER_HEALTH_EASY;
  CGameObjectDefaultValues::JapaneseOilTankerHealth = JAPANESE_OIL_TANKER_HEALTH_EASY;
}

void CGameObjectDefaultValues::LoadDefaultValuesMedium()
{
	//------------------------ Player --------------------------------- //
	CGameObjectDefaultValues::PlayerLives = PLAYER_NUMBER_OF_LIVES_MEDIUM;

  //--------------------Airplanes-----------------------------------//
  //Dauntless
  CGameObjectDefaultValues::DauntlessHealth = DAUNTLESS_DEFAULT_HEALTH_MEDIUM;
  CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad = DAUNTLESS_1000KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::Dauntless500KgBombMaxLoad = DAUNTLESS_500KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::Dauntless100KgBombMaxLoad = DAUNTLESS_100KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::DauntlessClusterBombMaxLoad = DAUNTLESS_CLUSTER_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::DauntlessRocketMaxLoad = DAUNTLESS_STANDARD_ROCKET_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::DauntlessTorpedoMaxLoad = DAUNTLESS_TORPEDO_MAX_LOAD_MEDIUM;
  //Mitchell
  CGameObjectDefaultValues::MitchellHealth = MITCHELL_DEFAULT_HEALTH_MEDIUM;
  CGameObjectDefaultValues::Mitchell1000KgBombMaxLoad = MITCHELL_1000KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::Mitchell500KgBombMaxLoad = MITCHELL_500KG_BOMB_MAX_LOAD_MEDIUM;
  //Val
  CGameObjectDefaultValues::ValHealth = VAL_DEFAULT_HEALTH_MEDIUM;
  CGameObjectDefaultValues::Val1000KgBombMaxLoad = VAL_1000KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::Val500KgBombMaxLoad = VAL_500KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::Val100KgBombMaxLoad = VAL_100KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::ValClusterBombMaxLoad = VAL_CLUSTER_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::ValRocketMaxLoad = VAL_STANDARD_ROCKET_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::ValTorpedoMaxLoad = VAL_TORPEDO_MAX_LOAD_MEDIUM;
  //Betty
  CGameObjectDefaultValues::BettyHealth = BETTY_DEFAULT_HEALTH_MEDIUM;
  CGameObjectDefaultValues::Betty1000KgBombMaxLoad = BETTY_1000KG_BOMB_MAX_LOAD_MEDIUM;
  CGameObjectDefaultValues::Betty500KgBombMaxLoad = BETTY_500KG_BOMB_MAX_LOAD_MEDIUM;

  //--------------------Ships-----------------------------------//
  CGameObjectDefaultValues::USSVictoryHealth = USS_VICTORY_HEALTH_MEDIUM;
  CGameObjectDefaultValues::AmericanLCTHealth = AMERICAN_LCT_HEALTH_MEDIUM;
  CGameObjectDefaultValues::AmericanCarrierHealth = AMERICAN_CARRIER_HEALTH_MEDIUM;
  CGameObjectDefaultValues::AmericanDestroyerHealth = AMERICAN_DESTROYER_HEALTH_MEDIUM;
  CGameObjectDefaultValues::AmericanBattleshipUSSTennesseeHealth = AMERICAN_BATTLESHIP_USS_TENNESSEE_HEALTH_MEDIUM;
  CGameObjectDefaultValues::AmericanTroopTransporterHealth = AMERICAN_TROOP_TRANSPORTER_HEALTH_MEDIUM;
  CGameObjectDefaultValues::JapaneseLCTHealth = JAPANESE_LCT_HEALTH_MEDIUM;
  CGameObjectDefaultValues::JapaneseCarrierHealth = JAPANESE_CARRIER_HEALTH_MEDIUM;
  CGameObjectDefaultValues::JapaneseDestroyerHealth = JAPANESE_DESTROYER_HEALTH_MEDIUM;
  CGameObjectDefaultValues::JapaneseBattleshipYamatoHealth = JAPANESE_BATTLESHIP_YAMATO_HEALTH_MEDIUM;
  CGameObjectDefaultValues::JapaneseTroopTransporterHealth = JAPANESE_TROOP_TRANSPORTER_HEALTH_MEDIUM;
  CGameObjectDefaultValues::JapaneseOilTankerHealth = JAPANESE_OIL_TANKER_HEALTH_MEDIUM;
}

void CGameObjectDefaultValues::LoadDefaultValuesHard()
{
	//------------------------ Player --------------------------------- //
	CGameObjectDefaultValues::PlayerLives = PLAYER_NUMBER_OF_LIVES_HARD;

  //--------------------Airplanes-----------------------------------//
  //Dauntless
  CGameObjectDefaultValues::DauntlessHealth = DAUNTLESS_DEFAULT_HEALTH_HARD;
  CGameObjectDefaultValues::Dauntless1000KgBombMaxLoad = DAUNTLESS_1000KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::Dauntless500KgBombMaxLoad = DAUNTLESS_500KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::Dauntless100KgBombMaxLoad = DAUNTLESS_100KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::DauntlessClusterBombMaxLoad = DAUNTLESS_CLUSTER_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::DauntlessRocketMaxLoad = DAUNTLESS_STANDARD_ROCKET_MAX_LOAD_HARD;
  CGameObjectDefaultValues::DauntlessTorpedoMaxLoad = DAUNTLESS_TORPEDO_MAX_LOAD_HARD;
  //Mitchell
  CGameObjectDefaultValues::MitchellHealth = MITCHELL_DEFAULT_HEALTH_HARD;
  CGameObjectDefaultValues::Mitchell1000KgBombMaxLoad = MITCHELL_1000KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::Mitchell500KgBombMaxLoad = MITCHELL_500KG_BOMB_MAX_LOAD_HARD;
  //Val
  CGameObjectDefaultValues::ValHealth = VAL_DEFAULT_HEALTH_HARD;
  CGameObjectDefaultValues::Val1000KgBombMaxLoad = VAL_1000KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::Val500KgBombMaxLoad = VAL_500KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::Val100KgBombMaxLoad = VAL_100KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::ValClusterBombMaxLoad = VAL_CLUSTER_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::ValRocketMaxLoad = VAL_STANDARD_ROCKET_MAX_LOAD_HARD;
  CGameObjectDefaultValues::ValTorpedoMaxLoad = VAL_TORPEDO_MAX_LOAD_HARD;
  //Betty
  CGameObjectDefaultValues::BettyHealth = BETTY_DEFAULT_HEALTH_HARD;
  CGameObjectDefaultValues::Betty1000KgBombMaxLoad = BETTY_1000KG_BOMB_MAX_LOAD_HARD;
  CGameObjectDefaultValues::Betty500KgBombMaxLoad = BETTY_500KG_BOMB_MAX_LOAD_HARD;

  //--------------------Ships-----------------------------------//
  CGameObjectDefaultValues::USSVictoryHealth = USS_VICTORY_HEALTH_HARD;
  CGameObjectDefaultValues::AmericanLCTHealth = AMERICAN_LCT_HEALTH_HARD;
  CGameObjectDefaultValues::AmericanCarrierHealth = AMERICAN_CARRIER_HEALTH_HARD;
  CGameObjectDefaultValues::AmericanDestroyerHealth = AMERICAN_DESTROYER_HEALTH_HARD;
  CGameObjectDefaultValues::AmericanBattleshipUSSTennesseeHealth = AMERICAN_BATTLESHIP_USS_TENNESSEE_HEALTH_HARD;
  CGameObjectDefaultValues::AmericanTroopTransporterHealth = AMERICAN_TROOP_TRANSPORTER_HEALTH_HARD;
  CGameObjectDefaultValues::JapaneseLCTHealth = JAPANESE_LCT_HEALTH_HARD;
  CGameObjectDefaultValues::JapaneseCarrierHealth = JAPANESE_CARRIER_HEALTH_HARD;
  CGameObjectDefaultValues::JapaneseDestroyerHealth = JAPANESE_DESTROYER_HEALTH_HARD;
  CGameObjectDefaultValues::JapaneseBattleshipYamatoHealth = JAPANESE_BATTLESHIP_YAMATO_HEALTH_HARD;
  CGameObjectDefaultValues::JapaneseTroopTransporterHealth = JAPANESE_TROOP_TRANSPORTER_HEALTH_HARD;
  CGameObjectDefaultValues::JapaneseOilTankerHealth = JAPANESE_OIL_TANKER_HEALTH_HARD;
}
