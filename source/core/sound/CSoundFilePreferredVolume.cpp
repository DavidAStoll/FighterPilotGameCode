/*
 * CSoundFilePreferredVolume.cpp
 *
 *  Created on: May 17, 2012
 *      Author: dstoll
 */

#include "includes/core/sound/CSoundFilePreferredVolume.h"

CSoundFilePreferredVolume::CSoundFilePreferredVolume()
{
	iPreferredVolume[ESoundFileId_Airplane_Engine_Middle] = 40;
	iPreferredVolume[ESoundFileId_Airplane_Engine_End] = 40;
	iPreferredVolume[ESoundFileId_Anti_Air_Craft_Medium] = 30;
	iPreferredVolume[ESoundFileId_GroundAA_MachineGun13mm] = 50;
	iPreferredVolume[ESoundFileId_Ship_MachineGun13mm] = 20;
	iPreferredVolume[ESoundFileId_Battle_Ship_Cannon] = 60;
	iPreferredVolume[ESoundFileId_Battle_Ship_Explosion] = 50;
	iPreferredVolume[ESoundFileId_Black_Powder_Cannon] = 50;
	iPreferredVolume[ESoundFileId_Building_Fire_Middle] = 15;
	iPreferredVolume[ESoundFileId_Bullet_Hit_Ground] = 15;
	iPreferredVolume[ESoundFileId_Bullet_Hit_Tank] = 15;
	iPreferredVolume[ESoundFileId_Dauntless_Machine_Gun] = 20;
	iPreferredVolume[ESoundFileId_Explosion_Large] = 50;
	iPreferredVolume[ESoundFileId_Explosion_Large2] = 50;
	iPreferredVolume[ESoundFileId_Explosion_Large_Water] = 50;
	iPreferredVolume[ESoundFileId_Explosion_Medium] = 45;
	iPreferredVolume[ESoundFileId_Explosion_Medium2] = 45;
	iPreferredVolume[ESoundFileId_Explosion_Medium_Water] = 30;
	iPreferredVolume[ESoundFileId_Explosion_Small3] = 20;
	iPreferredVolume[ESoundFileId_Explosion_Small3b] = 30;
	iPreferredVolume[ESoundFileId_Explosion_Small3b_Water] = 25;
	iPreferredVolume[ESoundFileId_Flag] = 8;
	iPreferredVolume[ESoundFileId_Hit_Bullet_Small] = 5;
	iPreferredVolume[ESoundFileId_Howitzer_In_Tank] = 40;
	iPreferredVolume[ESoundFileId_Male_Painful] = 50;
	iPreferredVolume[ESoundFileId_Male_Painful2] = 50;
	iPreferredVolume[ESoundFileId_Male_Scream] = 30;
	iPreferredVolume[ESoundFileId_Male_Scream2] = 30;
	iPreferredVolume[ESoundFileId_Plane_Bomb_Empty] = 5;
	iPreferredVolume[ESoundFileId_Plane_Bomb_Lock] = 10;
	iPreferredVolume[ESoundFileId_Plane_Bomb_release] = 10;
	iPreferredVolume[ESoundFileId_Plane_Collision] = 10;
	iPreferredVolume[ESoundFileId_Plane_Collision_Building] = 10;
	iPreferredVolume[ESoundFileId_Plane_Collision_Cannon] = 10;
	iPreferredVolume[ESoundFileId_Plane_Collision_Soldier] = 30;
	iPreferredVolume[ESoundFileId_Plane_Collision_Tree] = 10;
	iPreferredVolume[ESoundFileId_Plane_Crash_On_Land] = 50;
	iPreferredVolume[ESoundFileId_Plane_Crush_Human] = 10;
	iPreferredVolume[ESoundFileId_Plane_Back_MachineGun13mm] = 20;
	iPreferredVolume[ESoundFileId_Plane_Skid] = 10;
	iPreferredVolume[ESoundFileId_Radio_Signals] = 18;
	iPreferredVolume[ESoundFileId_Rifle_Gun] = 20;
	iPreferredVolume[ESoundFileId_Rocket_Firing] = 30;
	iPreferredVolume[ESoundFileId_Rocket_Launcher] = 30;
	iPreferredVolume[ESoundFileId_Ship_Creak_Middle] = 40;
	iPreferredVolume[ESoundFileId_Ship_Turret_Turn] = 10;
	iPreferredVolume[ESoundFileId_Silence] = 10;
	iPreferredVolume[ESoundFileId_Tank_Tracks] = 40;
	iPreferredVolume[ESoundFileId_Torepedo_Splash] = 10;
	iPreferredVolume[ESoundFileId_Torepedo_Travel] = 10;
	iPreferredVolume[ESoundFileId_Water_Splash] = 10;

	for(TInt lIndex = 0; lIndex < ESoundFileId_LAST_INDEX; lIndex++)
	{
		iPreferredVolume[lIndex] *= PREFERRED_SOUND_VOLUME_MUTILPLY;
	}
}

CSoundFilePreferredVolume::~CSoundFilePreferredVolume()
{
	// nothing to delete
}

