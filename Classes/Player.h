#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include <string>
using namespace cocos2d;
class Player : public cocos2d::Sprite
{
public:
	Player();
	~Player();
	static Player* create();

	virtual bool init();

	bool onContactBegin(PhysicsContact &);
	bool onContactPost(PhysicsContact &);
	bool onContactSeperate(PhysicsContact &);
	void handleInput();
	void handleMovement(float dt);
	void update(float dt);
private:
	Vec2 previousPos;
	Vec2 velocity;
	float velocityLimit;
	bool collL; bool collR; bool collU; bool collD;
	bool accelL;
	bool accelR;
	bool accelU;
	bool accelD;
};

#endif // !_PLAYER_H_
