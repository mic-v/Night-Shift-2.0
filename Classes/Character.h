/* CHARACTER CLASS
*  3/16/2018 AS OF THIS DATE, DON'T MAKE AN OBJECT WITH THIS. USE PLAYER OR ENEMY INSTEAD
*/

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "cocos2d.h"
#include <string>
#include <iostream>
#include "Entity.h"
#include "Weapon.h"
#include <vector>

class Character : public Entity
{
public:
	Character();
	virtual ~Character();
	static Character * create(const std::string & fileName);
	virtual bool init(const std::string & fileName);
	virtual void initAnimations();
	virtual bool onContactBegin(PhysicsContact&);
	virtual bool onContactPost(PhysicsContact&);
	virtual bool onContactSeparate(PhysicsContact&);
	virtual void handleMovement(float);
	virtual void updateAnimation(float);
	float getHealth();
protected:
	float health;
	bool isMoving;
	bool isSprinting;
	cocos2d::Animate* animL;
	cocos2d::Animate* animR;
	cocos2d::Animate* runAnimR;
	cocos2d::Animate* runAnimL;
	cocos2d::Animate* idleR;
	cocos2d::Animate* idleL;

	cocos2d::Animate* dying;

	Vec2 previousPos;
	Vec2 velocity;
	Vec2 velNorm;
	Vec2 lastVel;

	float speed;


	std::vector<Weapon*> weapons;
	float velocityLimit;

	//current weapon equipped
	unsigned int cWeapNum;
	bool hasAk;
	bool hasM16;
	//NEW Animation
	
	bool faceLeft_;
	bool faceRight_;
	bool faceUp_;
	bool faceDown_;
	int animationCounter;
	float animationTimer;
	int currentACounter;
	std::string orientation_;
	std::string cOrientation_;
};

#endif // !_CHARACTER_H_
