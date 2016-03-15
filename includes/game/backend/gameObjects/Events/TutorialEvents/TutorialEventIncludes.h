/*
 * TutorialEventIncludes.h
 *
 *  Created on: Aug 26, 2012
 *      Author: dstoll
 */

#ifndef TUTORIALEVENTINCLUDES_H_
#define TUTORIALEVENTINCLUDES_H_

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CBasicTutorialClass.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CBasicAircraftNeededTutorialClass.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CRestartLastTutorialScreenIfDiedEvent.h"

// Tutorial 1
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialLooping.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialTurns.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialSpeedUp.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialSlowDown.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialZoomInAndZoomOut.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialMoveToEndOfMap.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialStartFromCarrier.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialLandOnCarrier.h"

//Tutorial 2
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialFireButtons.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialBomb1000kg.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialLoseIfFailedToKillAnything.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialKillSoldiersUsingMachineGun.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialLoseIfAllAmericanSoldiersDied.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialDiveBombingKill.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialAttackBomber.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialAttackFighter.h"
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialAttackShip.h"

#define TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL 3
#define CONTROLS_TUTORIAL_LOOP_SECS_UNTIL_SCUCCESS_MESSAGE 3
#define TUTORIAL_DIE_EVENT_SECS_UNTIL_CALL 3
#define CONTROLS_TUTORIAL_SPEED_UP_SECS_UNTIL_SCUCCESS_MESSAGE 10
#define CONTROLS_TUTORIAL_SLOW_DOWN_SECS_UNTIL_SCUCCESS_MESSAGE_IF_CRASHED 3
#define CONTROLS_TUTORIAL_SLOW_DOWN_SECS_UNTIL_SCUCCESS_MESSAGE_IF_SPEED_UP_AGAIN 6
#define WEAPONS_TUTORIAL_BOMBS_SECS_UNTIL_SCUCCESS_MESSAGE 10
#define WEAPONS_TUTORIAL_ATTACK_BOMBER_SECS_UNTIL_SCUCCESS_MESSAGE 5
#define WEAPONS_TUTORIAL_ATTACK_SHIP_SECS_UNTIL_SCUCCESS_MESSAGE 15

#endif /* TUTORIALEVENTINCLUDES_H_ */
