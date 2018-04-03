#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "cocos2d.h"
using namespace cocos2d;

//PHYSICS MOVEMENT
/*---------------------------------------------------------- */
#define UP Vec2(0,1);
#define DOWN Vec2(0,-1);
#define LEFT Vec2(-1,0);
#define RIGHT Vec2(1,0);
#define VELOCITYLIMIT 150
#define FACELEFT "Left"
#define FACERIGHT "Right"
#define FACEUP "Back"
#define FACEDOWN "Front"
/*---------------------------------------------------------- *
//PHYSICS COLLISION DETECTION
/*---------------------------------------------------------- */
#define PLAYER_TAG 1
#define ENEMY_TAG 2
#define BULLET_TAG 3
#define ENEMYBULLET_TAG 4
#define	PICKUP_TAG 5
#define WALL_TAG 6
#define HEART_TAG 8

#define PLAYER_CATEGORY 0x0001
#define ENEMY_CATEGORY 0x0002
#define ENEMYBULLET_CATEGORY 0x0004 
#define PLAYERBULLET_CATEGORY 0x0008
#define WALL_CATEGORY 0x0016
#define ITEM_CATEGORY 0x0032
#define MASK_PLAYER WALL_CATEGORY | ENEMYBULLET_CATEGORY
#define MASK_ENEMY WALL_CATEGORY | PLAYERBULLET_CATEGORY
#define MASK_PLAYERBULLET WALL_CATEGORY | ENEMY_CATEGORY
#define MASK_ENEMYBULLET WALL_CATEGORY | PLAYER_CATEGORY
#define MASK_ITEM PLAYER_CATEGORY
#define MASK_WALL -1
/*---------------------------------------------------------- */
//LEVEL SETUP
/*---------------------------------------------------------- */
#define MAXTIME 120
#define MAX_PLAYER_HEALTH 30

#define PISTOLPICKUPTAG 1000
#define AKPICKUPTAG 1001
#define M16PICKUPTAG 1002
#define DYINGTAG 1004

#define PISTOLNAME "Silenced Pistol"
#define AKNAME "AK-47"
#define M16NAME "M-16"

#define PLAYERNAME "Player"
#define ESLEEP 1
#define	ESEEK 2
#define EATTACK 3

#define ENEMYMOVESPAWN 1
#define ENEMYATTACK 2

#endif // !_DEFINITIONS_H_


