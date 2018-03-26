#ifndef _CENEMY_H_
#define _CENEMY_H_

#include "Character.h"
#include "CPlayer.h"
#include <string>
using std::string;
using std::stringstream;

class CEnemy : public Character
{
public:
	virtual bool init();
	CREATE_FUNC(CEnemy);

	void setSpawn(Vec2 &);

	void update(float);
	void updateMovement();
	void updateAnimation();
	//MOOD for the A.I.
	void exitSpawn();
	void attackPlayer();

private:
	bool onContactBegin(PhysicsContact & contact);
	bool onContactPost(PhysicsContact & contact);
	bool onContactSeparate(PhysicsContact & contact);

	//Physics/Movement
	bool accelL; bool accelR;
	bool accelU; bool accelD;
	float speed;
	

	//A.I. variables
	int mood;
	bool pastSpawnGate;
	CPlayer* player_;
	Vec2 spawnPosition;
	Vec2 spawnGate;

};

#endif
