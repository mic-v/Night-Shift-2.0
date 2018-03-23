#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "MainMenu.h"

using namespace cocos2d;

class SplashScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(SplashScene);
private:
	void FadeToMainMenu(float dt)
	{
		Scene *scene = MainMenu::createScene();
		TransitionFade *transition = TransitionFade::create(0.75f, scene);
		Director::getInstance()->replaceScene(transition);
	}
};

#endif
