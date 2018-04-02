#include "GameScene.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include "GameCamera.h"
#include "GameOverScene.h"
#include "VictoryScene.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
using namespace cocos2d;
USING_NS_CC;

Scene* GameScene::scenehandle = nullptr;
Layer* GameScene::layerhandle = nullptr;


Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	auto layer = GameScene::create();
	scene->addChild(layer);
	PhysicsWorld* phys = scene->getPhysicsWorld();
	phys->setGravity(Vec2(0, 0));
	/*phys->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);*/

	return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init())
    {
        return false;
    }
	//Director::getInstance()->setProjection(Director::Projection::_2D);
	//Director::getInstance()->setDepthTest(true);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	std::cout << "initialized" << std::endl;

	GameLayer* layer = GameLayer::create();
	this->addChild(layer, 1);
	//h = Player::create();
	//layer->addChild(h);
	//h->setPosition(400, 540);
	//layer->setCameraMask
	this->addChild(GameUILayer::create(),1000);
	//sceneCamera = Camera::create();
	//this->addChild(sceneCamera);
	auto gameOverListener = EventListenerCustom::create("gameOver", [=](EventCustom* event) {
		Scene *scene = GameOverScene::createScene();
		TransitionFade *transition = TransitionFade::create(3.f, scene);
		Director::getInstance()->pushScene(transition);
		AudioEngine::stopAll();
	});

	auto finishShiftListener = EventListenerCustom::create("finishShift", [=](EventCustom* event) {
		Scene *scene = VictoryScene::createScene();
		TransitionFade *transition = TransitionFade::create(3.f, scene);
		Director::getInstance()->pushScene(transition);
		AudioEngine::stopAll();
	});


	_eventDispatcher->addEventListenerWithSceneGraphPriority(gameOverListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(finishShiftListener, this);


	AudioEngine::play2d("Sounds/Music/OnlyYou.mp3", true, 0.05f);

	this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt)
{
	CAMERA->setMos(INPUTS->getMousePosition());
	INPUTS->clearForNextFrame();
}

void GameScene::menuCloseCallback(Ref* pSender)
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

