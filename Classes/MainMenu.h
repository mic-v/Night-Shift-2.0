#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include "GameScene.h"
using namespace cocos2d;

class MainMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainMenu);

	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuGameCallback(cocos2d::Ref* pSender);
	void menuStartCallback(cocos2d::Ref* pSender);
	void menuSaveCallback(cocos2d::Ref *sender, ui::Text::TouchEventType event);
	
	void startTouchEvent(cocos2d::Ref* pSender, ui::Widget::TouchEventType type);
	void newGameTouchEvent(cocos2d::Ref* pSender, ui::Widget::TouchEventType type);
	void selectedItemEvent(cocos2d::Ref *pSender, ui::ListView::EventType type);
	void textFieldEvent(cocos2d::Ref *pSender, ui::TextField::EventType type);
	void initiateNewGame(string);
private:
	void FadeToGame(float dt)
	{
		Scene *scene = GameScene::createScene();
		TransitionFade *transition = TransitionFade::create(0.75f, scene);
		Director::getInstance()->replaceScene(transition);
	}

	ui::Button* newGameButton;
	ui::TextField *textField;
	Menu* menu;
	ui::ScrollView *scrollView;
	Label* saveLabel;

	std::vector<std::string> saveList;
};

#endif
