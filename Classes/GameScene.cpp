#include "GameScene.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include "SimpleAudioEngine.h"
#include "GameCamera.h"

USING_NS_CC;
using namespace cocos2d;

Scene* GameScene::scenehandle = nullptr;
Layer* GameScene::layerhandle = nullptr;


Scene* GameScene::createScene()
{
	//Scene* scene = Scene::createWithPhysics();
	//GameLayer* layer = GameLayer::create();
	//layerhandle = layer;
	//GameScene* layer = GameScene::create();
	//scene->addChild(layer);
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//return GameScene::create();
	auto scene = Scene::createWithPhysics();
	auto layer = GameScene::create();
	scene->addChild(layer);
	PhysicsWorld* phys = scene->getPhysicsWorld();
	phys->setGravity(Vec2(0, 0));
	//phys->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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

	//label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	////label->setCameraMask((unsigned)CameraFlag::USER1);
 //   if (label == nullptr)
 //   {
 //       problemLoading("'fonts/Marker Felt.ttf'");
 //   }
 //   else
 //   {
 //       // position the label on the center of the screen
 //       label->setPosition(Vec2(origin.x + visibleSize.width/2,
 //                               origin.y + visibleSize.height - label->getContentSize().height));

 //       // add the label as a child to this layer
 //       this->addChild(label, 1);
 //   }
	GameLayer* layer = GameLayer::create();
	this->addChild(layer, 1);
	//h = Player::create();
	//layer->addChild(h);
	//h->setPosition(400, 540);
	//layer->setCameraMask
	this->addChild(GameUILayer::create(),1000);
	//sceneCamera = Camera::create();
	//this->addChild(sceneCamera);
	//auto contactListener = EventListenerPhysicsContact::create();
	//contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	//this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt)
{
	CAMERA->setMos(INPUTS->getMousePosition());
	INPUTS->clearForNextFrame();
	//CAMERA->setMousePos(INPUTS->getMousePosition());
	//CAMERA->getCamTarget()->setPosition(Vec2(500, 0));
	//if (INPUTS->getKey(KeyCode::KEY_A))
	//{
	//	h->setPosition(h->getPosition() + Vec2(-10, 0));
	//}
	//if (INPUTS->getKey(KeyCode::KEY_D))
	//{
	//	h->setPosition(h->getPosition() + Vec2(10, 0));

	//}
	//if (INPUTS->getKey(KeyCode::KEY_W))
	//{
	//	h->setPosition(h->getPosition() + Vec2(0, 10));

	//}
	//if (INPUTS->getKey(KeyCode::KEY_S))
	//{
	//	h->setPosition(h->getPosition() + Vec2(0, -10));
	//}
	//sceneCamera->setPosition(h->getPosition());
	//std::cout << h->getPosition().x << std::endl;
}

//bool GameScene::onContactBegin(PhysicsContact & contact)
//{
//	auto nodeA = contact.getShapeA()->getBody()->getNode();
//	auto nodeB = contact.getShapeB()->getBody()->getNode();
////
//	if (nodeA && nodeB)
//	{
//		if (nodeA->getPhysicsBody()->getTag() == 10)
//		{
//			std::cout << "bads" << std::endl;
//			return true;
//		}
//		else if (nodeB->getPhysicsBody()->getTag() == 10)
//		{
//			std::cout << "helo" << std::endl;
//			return true;
//		}
//	}
//	return false;
////	//auto shapeA = contact.getShapeA();
////	//auto shapeB = contact.getShapeB();
////	//std::cout << "test234" << std::endl;
////	//if (shapeA->getBody()->getNode() && shapeB->getBody()->getNode())
////	//{
////	//	if ((shapeA->getBody()->getCategoryBitmask() & shapeB->getBody()->getCollisionBitmask()) == 0
////	//		|| (shapeB->getBody()->getCategoryBitmask() & shapeA->getBody()->getCollisionBitmask()) == 0)
////	//	{
////	//		if (shapeA->getBody()->getCategoryBitmask() == 0x02)
////	//		{
////	//			//shapeA->getBody()->setVelocity(Vec2(0, 0));
////	//			std::cout << "hello papi124" << std::endl;
////	//			return true;
////	//		}
////	//		else if (shapeB->getBody()->getCategoryBitmask() == 0x02)
////	//		{
////	//			std::cout << "hello paaspi1234" << std::endl;
////	//			return true;
////
////	//		}
////	//		return false;
////	//	}
////	//	return false;
////	//}
//}


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
