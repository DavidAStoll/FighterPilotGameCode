/*
 * CSoundFileNameMapping.cpp
 *
 *  Created on: May 17, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CSoundFileNameMapping.h"

CSoundFileNameMapping::CSoundFileNameMapping()
{
		iSoundMappingArray[ESoundFileId_Airplane_Engine_Middle] = new CString("airplane_engine_middle.wav");
		iSoundMappingArray[ESoundFileId_Airplane_Engine_End] = new CString("airplane_engine_end.wav");
		iSoundMappingArray[ESoundFileId_Anti_Air_Craft_Medium] = new CString("anti_air_craft_medium.wav");
		iSoundMappingArray[ESoundFileId_GroundAA_MachineGun13mm] = new CString("battle_ambience_gunfire1.wav");
		iSoundMappingArray[ESoundFileId_Ship_MachineGun13mm] = new CString("battle_ambience_gunfire4.wav");
		iSoundMappingArray[ESoundFileId_Battle_Ship_Cannon] = new CString("battle_ship_cannon.wav");
		iSoundMappingArray[ESoundFileId_Battle_Ship_Explosion] = new CString("battle_ship_explosion.wav");
		iSoundMappingArray[ESoundFileId_Black_Powder_Cannon] = new CString("black_powder_cannon.wav");
		iSoundMappingArray[ESoundFileId_Building_Fire_Middle] = new CString("building_fire_middle.wav");
		iSoundMappingArray[ESoundFileId_Bullet_Hit_Ground] = new CString("bullet_hit_ground.wav");
		iSoundMappingArray[ESoundFileId_Bullet_Hit_Tank] = new CString("bullet_hit_tank.wav");
		iSoundMappingArray[ESoundFileId_Dauntless_Machine_Gun] = new CString("dauntless_machine_gun.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Large] = new CString("explosion_large.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Large2] = new CString("explosion_large2.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Large_Water] = new CString("explosion_large_water.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Medium] = new CString("explosion_medium.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Medium2] = new CString("explosion_medium2.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Medium_Water] = new CString("explosion_medium_water.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Small3] = new CString("explosion_small3.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Small3b] = new CString("explosion_small3b.wav");
		iSoundMappingArray[ESoundFileId_Explosion_Small3b_Water] = new CString("explosion_small3b_water.wav");
		iSoundMappingArray[ESoundFileId_Flag] = new CString("flag.wav");
		iSoundMappingArray[ESoundFileId_Hit_Bullet_Small] = new CString("hit_bullet_small.wav");
		iSoundMappingArray[ESoundFileId_Howitzer_In_Tank] = new CString("howitzer_in_tank.wav");
		iSoundMappingArray[ESoundFileId_Male_Painful] = new CString("male_painful.wav");
		iSoundMappingArray[ESoundFileId_Male_Painful2] = new CString("male_painful2.wav");
		iSoundMappingArray[ESoundFileId_Male_Scream] = new CString("male_scream.wav");
		iSoundMappingArray[ESoundFileId_Male_Scream2] = new CString("male_scream2.wav");
		iSoundMappingArray[ESoundFileId_Plane_Bomb_Empty] = new CString("plane_bomb_empty.wav");
		iSoundMappingArray[ESoundFileId_Plane_Bomb_Lock] = new CString("plane_bomb_lock.wav");
		iSoundMappingArray[ESoundFileId_Plane_Bomb_release] = new CString("plane_bomb_release.wav");
		iSoundMappingArray[ESoundFileId_Plane_Collision] = new CString("plane_collision.wav");
		iSoundMappingArray[ESoundFileId_Plane_Collision_Building] = new CString("plane_collision_building.wav");
		iSoundMappingArray[ESoundFileId_Plane_Collision_Cannon] = new CString("plane_collision_cannon.wav");
		iSoundMappingArray[ESoundFileId_Plane_Collision_Soldier] = new CString("plane_collision_soldier.wav");
		iSoundMappingArray[ESoundFileId_Plane_Collision_Tree] = new CString("plane_collision_tree.wav");
		iSoundMappingArray[ESoundFileId_Plane_Crash_On_Land] = new CString("plane_collision.wav");
		iSoundMappingArray[ESoundFileId_Plane_Crush_Human] = new CString("plane_crush_human.wav");
		iSoundMappingArray[ESoundFileId_Plane_Back_MachineGun13mm] =  new CString("dauntless_machine_gun.wav");
		iSoundMappingArray[ESoundFileId_Plane_Skid] = new CString("plane_skid.wav");
		iSoundMappingArray[ESoundFileId_Radio_Signals] = new CString("radio_signals.wav");
		iSoundMappingArray[ESoundFileId_Rifle_Gun] = new CString("rifle_gun.wav");
		iSoundMappingArray[ESoundFileId_Rocket_Firing] = new CString("rocket_firing.wav");
		iSoundMappingArray[ESoundFileId_Rocket_Launcher] = new CString("rocket_launcher.wav");
		iSoundMappingArray[ESoundFileId_Ship_Creak_Middle] = new CString("ship_creak_middle.wav");
		iSoundMappingArray[ESoundFileId_Ship_Turret_Turn] = new CString("ship_turret_turn.wav");
		iSoundMappingArray[ESoundFileId_Silence] = new CString("silence.wav");
		iSoundMappingArray[ESoundFileId_Tank_Tracks] = new CString("tank_tracks.wav");
		iSoundMappingArray[ESoundFileId_Torepedo_Splash] = new CString("torepedo_splash.wav");
		iSoundMappingArray[ESoundFileId_Torepedo_Travel] = new CString("torepedo_travel.wav");
		iSoundMappingArray[ESoundFileId_Water_Splash] = new CString("water_splash.wav");
}

CSoundFileNameMapping::~CSoundFileNameMapping()
{
	//remove CStrings
	for(TInt lIndex = 0; lIndex < ESoundFileId_LAST_INDEX; lIndex++)
	{
		delete iSoundMappingArray[lIndex];
		iSoundMappingArray[lIndex] = NULL;
	}
}
