#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "cocos2d.h"
using namespace cocos2d;

#define UP Vec2(0,1);
#define DOWN Vec2(0,-1);
#define LEFT Vec2(-1,0);
#define RIGHT Vec2(1,0);

#define PLAYER_TAG 1
#define ENEMY_TAG 2
#define BULLET_TAG 4
#define	PICKUP_TAG 8
#define OBJECT_TAG 16
#define WALL_TAG 32

#define PISTOLPICKUPTAG 1000
#define AKPICKUPTAG 1001
#define M16PICKUPTAG 1002

#define PISTOLNAME "Silenced Pistol"
#define AKNAME "AK-47"
#define M16NAME "M-16"

#define FACELEFT "l" // L 
#define FACERIGHT "r"
#define FACEUP "u"
#define FACEDOWN "d"

#define PLAYERNAME "Player"
#define ESLEEP 1
#define	ESEEK 2
#define EATTACK 3

#define MAXWALKACCEL 200
#define MAXRUNACCEL 400

#endif // !_DEFINITIONS_H_


