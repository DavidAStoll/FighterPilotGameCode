/*
 * GlobalDefines.h
 *
 *  Created on: May 24, 2012
 *      Author: dstoll
 *
 *      This file should have all the important defines needed in the game
 *      That way we only need to look into a single play if we want to change something
 */

#ifndef GLOBALDEFINES_H_
#define GLOBALDEFINES_H_

//----------------------- COMPILER ----------------------//

#define FALSE false
#define TRUE true

//signal that the code should compile for the ANDROID platform
#define PLATFORM_ANDROID true

//Especial marco needed for Amazon Gamecircle Code
#if PLATFORM_ANDROID
	#define AMAZON_BUILD false
#endif

//to let the code what we code know if certain elements should be excluded if not full version
#define GGAME_FULL_VERSION false
#define GENABLE_BUY_NOW_BUTTON false
//Social Network URLS
#define URL_FACEBOOK "http://www.facebook.com/pages/Fighter-Pilot-The-Pacific-War/187155327981657"
#define URL_YOUTUBE "http://www.youtube.com/playlist?list=PLS7KP6I8INcyNRJiETkO2LSow49WNPyHR"
//Store URLs
#define URL_GOOGLE_PLAY "market://details?id=fighterPilot.ThePacificWar"
#define URL_AMAZON "http://www.amazon.com/Fighter-Pilot-The-Pacific-War/dp/B00A8FYXSM/"
#define URL_SAMSUNG "http://www.samsungapps.com/mars/topApps/topAppsDetail.as?productId=000000524654"
#define URL_APP_IN_KOREA "http://tsto.re/0000324714"
//BUY NOW BUTTON URL
#define STORE_URL URL_AMAZON

//Sets after how many seconds the player is allowed to cancel TRIAL clip
#define GTRIALER_CLIP_SECS_UNTIL_PLAYER_CAN_CANCEL 4

//for User Stats Tracker
#if GGAME_FULL_VERSION
#define GSTATS_TRACKER_UPLOAD_SCORES_INTERVAL_IN_MINS 5
#else
#define GSTATS_TRACKER_UPLOAD_SCORES_INTERVAL_IN_MINS 1 //upload more often since player might quit very quickly
#endif
#define GSTATS_TRACKER_SERVER_IP "74.208.13.149"
#define GSTATS_TRACKER_SERVER_PORT 7555
#define GSTATS_TRACKER_SERVER_CONNECTION_TIMEOUT 10

//for Highscore Server
#define GHIGHSCORE_SERVER_IP "74.208.13.149"
#define GHIGHSCORE_SERVER_PORT 8555
#define GHIGHSCORE_SERVER_CONNECTION_TIMEOUT 6

//Frames Per Second Counter Options
#define FRAMES_PER_SECOND_SHOW_FPS true
#define FRAMES_PER_SECOND_SHOW_AVG_TIME_BETWEEN_FRAMES true
#define FRAMES_PER_SECOND_SHOW_AVG_LOSS_DUE_TO_SLEEP true
#define FRAMES_PER_SECOND_SHOW_PERCENTAGE_USED_BY_LOGIC true
#define FRAMES_PER_SECOND_SHOW_PERCENTAGE_USED_BY_DRAW true
#define FRAMES_PER_SECOND_SHOW_PERCENTAGE_USED_BY_LOGIC_AND_DRAW true

//tells us at how many frames per Second the games runs
#define GFRAMES_PER_SECOND_FRONTEND 30
#define GFRAMES_PER_SECOND_BACKEND 30

#endif /* GLOBALDEFINES_H_ */
