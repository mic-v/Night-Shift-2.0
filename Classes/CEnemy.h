#ifndef _CENEMY_H_
#define _CENEMY_H_

#include "Character.h"
#include "CPlayer.h"

class CEnemy : public Character
{
public:
	virtual bool init();
	void initAnimations();
	void initZombieAnimations();
	CREATE_FUNC(CEnemy);

	void setSpawn(Vec2 &);
	void update(float);
	void handleMovement(float);
	void updateAnimation(float);
	//MOOD for the A.I.
	void exitSpawn(float);
	void attackPlayer(float);
	void setPlayer(CPlayer &);
	void setZombie(bool);

	void damageEnemy();
	void removeEnemy();
private:
	bool onContactBegin(PhysicsContact & contact);
	bool onContactPost(PhysicsContact & contact);
	bool onContactSeparate(PhysicsContact & contact);

	//Physics/Movement
	bool accelL; bool accelR;
	bool accelU; bool accelD;
	
	float movementTimer;
	//A.I. variables
	int mood;
	bool isZombie;
	bool pastSpawnGate;
	CPlayer* player_;
	Vec2 spawnPosition;
	Vec2 spawnGate;
	bool dyingAnim;
	Sequence* dyingSeq;

};

#endif
