#include "MainMenu.h"
#include "GameCamera.h"
using namespace cocos2d;

cocos2d::Scene* MainMenu::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	auto layer = MainMenu::create();

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

bool MainMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size screenSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *title = Sprite::create("MainMenuTitle-Alt.png");
	title->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.75 + origin.y));
	this->addChild(title);

	auto closeItem = MenuItemImage::create(
		"exit.png",
		"exit_selected.png",
		CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

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

	auto gameItem = MenuItemImage::create(
		"startButton.png",
		"startButton_selected.png",
		CC_CALLBACK_1(MainMenu::menuGameCallback, this));

	if (gameItem == nullptr ||
		gameItem->getContentSize().width <= 0 ||
		gameItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = screenSize.width / 2 + origin.x;
		float y = screenSize.height * 0.5 + origin.y;
		gameItem->setPosition(Vec2(x, y));
	}
	auto menu = Menu::create(gameItem, closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	return true;
}

void MainMenu::menuCloseCallback(cocos2d::Ref * pSender)
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

void MainMenu::menuGameCallback(cocos2d::Ref * pSender)
{
	Scene *scene = GameScene::createScene();
	TransitionFade *transition = TransitionFade::create(2.f, scene);
	Director::getInstance()->replaceScene(transition);
	CAMERA->init();
}

