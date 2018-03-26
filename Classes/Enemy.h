#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Character.h"
#include "CPlayer.h"
#include <string>
using std::string;
using std::stringstream;

class Enemy : public Character
{
public:
	Enemy();
	virtual ~Enemy();
	//CREATE_FUNC(Enemy);
	static Enemy* create();
	virtual bool init();
	void update(float);
	void sleep(float);
	void seek(float);
	void attack(float);
	void initAnimations();
	void updateAnimation(float);
	void handleMovement(float);

	bool playerInFOV();

	void setVisionRotation(float);
	void setPlayer(CPlayer*);
	void setIfVertical(bool);
	void setSpawn(Vec2 &);

	bool onContactBegin(PhysicsContact&);
	bool onContactPost(PhysicsContact&);
	bool onContactSeparate(PhysicsContact&);

	void damageEnemy();
	void removeEnemy();
private:
	float getDistance();
	int mood;
	float fovDistance;
	float timer;
	bool vertical;
	bool reachedX;
	bool reachedY;
	bool reachedSpawnX;//
	bool reachedSpawnY;
	bool dyingAnim;
	bool pastSpawnGate;
	//Physics Portion
	float vspeed;
	float hspeed;
	bool accelL;
	bool accelR;
	bool accelU;
	//PhysicsBody* body;
	bool accelD;

	//void ifDead();

	Vec2 lastPlayerPosition;
	Vec2 spawnPosition;

	CPlayer* player;
	DrawNode* search;
	Vec2 direction;

	SpriteFrame* idleFrameL;
	SpriteFrame* idleFrameR;
	Sequence* dyingSeq;
};

#endif // !_ENEMY_H_
