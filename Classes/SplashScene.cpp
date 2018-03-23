#include "SplashScene.h"
#include "MainMenu.h"


cocos2d::Scene* SplashScene::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	SplashScene* layer = SplashScene::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;

}

bool SplashScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	Sprite *sprite = Sprite::create("OPEN_BOX_STUDIOS.png");
	
	sprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y));
	this->addChild(sprite);

	scheduleOnce(schedule_selector(SplashScene::FadeToMainMenu), 1.0f);

	return true;

}

