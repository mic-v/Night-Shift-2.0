#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include "cocos2d.h"
#include "Character.h"
#include <string>

class CPlayer : public Character
{
public:
	CPlayer();
	virtual ~CPlayer();
	static CPlayer * create(const std::string&);
	bool init(const std::string&);


	void initAnimations();
	void handleInput();
	void handleMovement(float);
	void updateCamera(float);
	void updateAnimation(float);

	void update(float);

	bool onContactBegin(PhysicsContact &);
	bool onContactPost(PhysicsContact &);
	bool onContactSeparate(PhysicsContact &);

	void damage();


private:
	float hspeed;
	float vspeed;
	float sprintDuration;
	SpriteFrame* idleFrameL;
	Sprite* border;
	Sprite* bar;
	ProgressTimer* dashBar;
};

#endif // !_CPLAYER_H_
