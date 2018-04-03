#ifndef _GAME_UI_LAYER_H_
#define _GAME_UI_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.H"
#include <iostream>

using namespace cocos2d;
class GameUILayer : public cocos2d::Layer
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(GameUILayer);

	virtual bool init();

	bool onContactBegin(PhysicsContact &);
	bool onContactPost(PhysicsContact &);
	bool onContactSeparate(PhysicsContact &);
	//void update(float dt);
	// a selector callback
	//void menuCloseCallback(cocos2d::Ref* pSender);
	static int score_;
private:
	Label* label;
	Label* time;
	Label* ammo;
	Label* currentWeapon;
	Label* roundStart; Label* roundEnd;
	Sprite* border;
	Sprite* bar;
	ProgressTimer* hpBar;
};

#endif
