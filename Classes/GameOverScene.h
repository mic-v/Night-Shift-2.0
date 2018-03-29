#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include "GameScene.h"
#include "MainMenu.h"
using namespace cocos2d;

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameOverScene);

	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuGameCallback(cocos2d::Ref* pSender);
	void menuMenuCallback(cocos2d::Ref* pSender);
private:
	void FadeToGame(float dt)
	{
		Scene *scene = GameScene::createScene();
		TransitionFade *transition = TransitionFade::create(0.75f, scene);
		Director::getInstance()->replaceScene(transition);
	}
};

#endif
