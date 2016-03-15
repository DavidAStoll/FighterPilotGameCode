/*
 * CSoundFileNameMapping.h
 *
 *  Created on: May 17, 2012
 *      Author: dstoll
 */

#ifndef CSOUNDFILENAMEMAPPING_H_
#define CSOUNDFILENAMEMAPPING_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

enum TSoundFileId
{
	ESoundFileId_Airplane_Engine_Middle = 0,
	ESoundFileId_Airplane_Engine_End,
	ESoundFileId_Anti_Air_Craft_Medium,
	ESoundFileId_GroundAA_MachineGun13mm,
	ESoundFileId_Ship_MachineGun13mm,
	ESoundFileId_Battle_Ship_Cannon,
	ESoundFileId_Battle_Ship_Explosion,
	ESoundFileId_Black_Powder_Cannon,
	ESoundFileId_Building_Fire_Middle,
	ESoundFileId_Bullet_Hit_Ground,
	ESoundFileId_Bullet_Hit_Tank,
	ESoundFileId_Dauntless_Machine_Gun,
	ESoundFileId_Explosion_Large,
	ESoundFileId_Explosion_Large2,
	ESoundFileId_Explosion_Large_Water,
	ESoundFileId_Explosion_Medium,
	ESoundFileId_Explosion_Medium2,
	ESoundFileId_Explosion_Medium_Water,
	ESoundFileId_Explosion_Small3,
	ESoundFileId_Explosion_Small3b,
	ESoundFileId_Explosion_Small3b_Water,
	ESoundFileId_Flag,
	ESoundFileId_Hit_Bullet_Small,
	ESoundFileId_Howitzer_In_Tank,
	ESoundFileId_Male_Painful,
	ESoundFileId_Male_Painful2,
	ESoundFileId_Male_Scream,
	ESoundFileId_Male_Scream2,
	ESoundFileId_Plane_Bomb_Empty,
	ESoundFileId_Plane_Bomb_Lock,
	ESoundFileId_Plane_Bomb_release,
	ESoundFileId_Plane_Collision,
	ESoundFileId_Plane_Collision_Building,
	ESoundFileId_Plane_Collision_Cannon,
	ESoundFileId_Plane_Collision_Soldier,
	ESoundFileId_Plane_Collision_Tree,
	ESoundFileId_Plane_Crash_On_Land,
	ESoundFileId_Plane_Crush_Human,
	ESoundFileId_Plane_Back_MachineGun13mm,
	ESoundFileId_Plane_Skid,
	ESoundFileId_Radio_Signals,
	ESoundFileId_Rifle_Gun,
	ESoundFileId_Rocket_Firing,
	ESoundFileId_Rocket_Launcher,
	ESoundFileId_Ship_Creak_Middle,
	ESoundFileId_Ship_Turret_Turn,
	ESoundFileId_Silence,
	ESoundFileId_Tank_Tracks,
	ESoundFileId_Torepedo_Splash,
	ESoundFileId_Torepedo_Travel,
	ESoundFileId_Water_Splash,
	ESoundFileId_LAST_INDEX
};

class CSoundFileNameMapping
{
public:
	CSoundFileNameMapping();
	virtual ~CSoundFileNameMapping();

	CString* iSoundMappingArray[ESoundFileId_LAST_INDEX];
};

#endif /* CSOUNDFILENAMEMAPPING_H_ */
