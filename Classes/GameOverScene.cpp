#include "GameOverScene.h"
#include "GameCamera.h"
using namespace cocos2d;

cocos2d::Scene* GameOverScene::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	auto layer = GameOverScene::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size screenSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *title = Sprite::create("UI/gameOver.png");
	title->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.75 + origin.y));
	this->addChild(title);

	auto closeItem = MenuItemImage::create(
		"UI/exit.png",
		"UI/exit(pressed).png",
		CC_CALLBACK_1(GameOverScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = screenSize.width / 2 + origin.x;
		float y =  screenSize.height * 0.4 + origin.y;
		closeItem->setPosition(Vec2(x, y));
	}

	auto menuItem = MenuItemImage::create(
		"UI/menu.png",
		"UI/menu(pressed).png",
		CC_CALLBACK_1(GameOverScene::menuMenuCallback, this));

	if (menuItem == nullptr ||
		menuItem->getContentSize().width <= 0 ||
		menuItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = screenSize.width / 2 + origin.x;
		float y = screenSize.height * 0.5 + origin.y;
		menuItem->setPosition(Vec2(x, y));
	}

	auto gameItem = MenuItemImage::create(
		"UI/retry.png",
		"UI/retry(pressed).png",
		CC_CALLBACK_1(GameOverScene::menuGameCallback, this));

	if (gameItem == nullptr ||
		gameItem->getContentSize().width <= 0 ||
		gameItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = screenSize.width / 2 + origin.x;
		float y = screenSize.height * 0.6 + origin.y;
		gameItem->setPosition(Vec2(x, y));
	}
	auto menu = Menu::create(gameItem, menuItem, closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	return true;
}

void GameOverScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void GameOverScene::menuGameCallback(cocos2d::Ref * pSender)
{
	Scene *scene = GameScene::createScene();
	TransitionFade *transition = TransitionFade::create(2.f, scene);
	//Director::getInstance()->replaceScene(transition);
	Director::getInstance()->pushScene(transition);
	CAMERA->init();
}

void GameOverScene::menuMenuCallback(cocos2d::Ref * pSender)
{
	Scene *scene = MainMenu::createScene();
	TransitionFade *transition = TransitionFade::create(2.f, scene);
	Director::getInstance()->replaceScene(transition);
}

