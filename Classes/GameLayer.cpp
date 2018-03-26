
#include "GameLayer.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include "SimpleAudioEngine.h"
#include "Definitions.h"
#include "GameCamera.h"

USING_NS_CC;
using namespace cocos2d;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameLayerScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//Director::getInstance()->setProjection(Director::Projection::_2D);
	//Director::getInstance()->setDepthTest(true);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// LEVEL SETUP
	enemySpawn1 = Vec2(-320, 1728);
	enemySpawn2 = Vec2(3776, 1728);
	enemySpawn3 = Vec2(1728, -320);
	enemySpawn4 = Vec2(1728, 3776);

	initLevel();
	level_ = 1;
	levelTimer = 0.f;
	spawnLimit = level_ * 4;
	spawnRate = 0.f;
	randomSpawn = 0;
	randomWeapon = 0;
	//map->getLayer("Layer")->setGlobalZOrder(100);

	//CEnemy* enmy2 = CEnemy::create();
	//enmy2->getPhysicsBody()->setVelocity(Vec2(200, 0));
	////enmy2->setPosition(1300, 1700);
	//this->addChild(enmy2, 0);

	cplayer = CPlayer::create("original.png");
	cplayer->retain();
	cplayer->setPosition(1700, 1700);
	this->addChild(cplayer, 0, PLAYERNAME);

	enmy = CEnemy::create();
	enmy->setPlayer(*cplayer);
	//enmy->setTexture("original.png");
	enmy->setSpawn(enemySpawn3);
	this->addChild(enmy);

	Item* item = Item::create("silencedGun.png");
	item->setPosition(Vec2(750, 640));
	this->addChild(item, 2);

	//Enemy* enemy = Enemy::create();
	////enemy->retain();
	//this->addChild(enemy,0);
	//enemy->setPosition(Vec2(3692, 1728));
	////enemy->setPlayer(cplayer);
	////enemy->setIfVertical(false);
	////enemy->setSpawn(Vec2(3692,1728));
	////enemyList.push_back(enemy);

	roundStart_ = false;
	dontmove = false;
	wallCollision = false;
	this->scheduleUpdate();
	this->addChild(CAMERA->getCameraTarget());
	this->runAction(CAMERA->getCamera());

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
	//contactListener->onContactPreSolve = CC_CALLBACK_1(GameLayer::onContactPost, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);



	return true;
}

void GameLayer::update(float dt)
{
	//tile_->setLocalZOrder(tile_->getLocalZOrder() + 1);
	//std::cout << cplayer->getZOrder() << std::endl;
	if (!roundStart_)
	{
		if (cplayer->getPosition().y > 800)
		{
			levelTimer += dt;
			std::string tmp = "12 AM";
			EventCustom event("roundStart");
			event.setUserData((void*)tmp.c_str());

			_eventDispatcher->dispatchEvent(&event);
			roundStart_ = true;
		}
	}
	else if (roundStart_ && !roundEnd_)
	{
		levelTimer += dt;
		std::cout << (int)levelTimer << std::endl;
		if ((int)levelTimer == MAXTIME * 0.2)
		{
			std::string tmp = "1 AM";
			EventCustom event("roundStart");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else if ((int)levelTimer == MAXTIME * 0.4)
		{
			std::string tmp = "2 AM";
			EventCustom event("roundStart");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else  if ((int)levelTimer == MAXTIME * 0.6)
		{
			std::string tmp = "3 AM";
			EventCustom event("roundStart");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else if ((int)levelTimer == MAXTIME * 0.8)
		{
			std::string tmp = "4 AM";
			EventCustom event("roundStart");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else if ((int)levelTimer == MAXTIME)
		{
			std::string tmp = "5 AM";
			EventCustom event("roundStart");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
			roundEnd_ = true;
		}
	}

}


bool GameLayer::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == BULLET_TAG)
		{
			findEnemyandHurt(nodeA);
			return true;
		}
		else if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == ENEMY_TAG)
		{
			findEnemyandHurt(nodeB);
			return true;
		}
	}
	return false;
}

bool GameLayer::onContactPost(PhysicsContact & contact)
{
	return true;
}

void GameLayer::menuCloseCallback(Ref* pSender)
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

void GameLayer::repositionSprite(float dt)
{

}

void GameLayer::initLevel()
{
	auto map = TMXTiledMap::create("level/Arena-1.3.tmx");
	this->addChild(map,-500,20);
	auto layerMeta = map->getLayer("LayerMeta");
	auto layer64Top = map->getLayer("Layer64Top");
	auto layer256 = map->getLayer("Layer256");
	for (int i = 0; i < layerMeta->getLayerSize().width; i++)
	{
		for (int j = layerMeta->getLayerSize().height - 1; j >= 0; j--)
		{
			auto gid64 = layer64Top->getTileGIDAt(Vec2(i, j));
			auto gid256 = layer256->getTileGIDAt(Vec2(i, j));
			auto meta = layerMeta->getTileGIDAt(Vec2(i, j));
			if (gid64)
			{
				auto properties = map->getPropertiesForGID(gid64).asValueMap();
				auto tileTag = properties["Tag"].asString();
				string filePath = "Tiles/" + tileTag + ".png";
				Sprite* tile = Sprite::create(filePath);
				tile->setAnchorPoint(Vec2(0, 0));
				tile->setPosition(Vec2(i * 64, (layer64Top->getLayerSize().height - j - 1) * 64));
				//auto p = tile->getPosition() - Vec2(tile->getContentSize().width * 0.5f, tile->getContentSize().height * 0.5f);
				//p = CC_POINT_POINTS_TO_PIXELS(p);
				tile->setLocalZOrder(200);
				this->addChild(tile);
			}
			if (gid256)
			{
				auto properties = map->getPropertiesForGID(gid256).asValueMap();
				auto tileTag = properties["Tag"].asString();
				string filePath = "Tiles/" + tileTag + ".png";
				Sprite* tile = Sprite::create(filePath);
				tile->setAnchorPoint(Vec2(0, 0));
				tile->setPosition(Vec2(i * 64, (layer64Top->getLayerSize().height - j - 1) * 64));
				//auto p = tile->getPosition() - Vec2(tile->getContentSize().width * 0.5f, tile->getContentSize().height * 0.5f);
				auto p = tile->getPosition();
				p = CC_POINT_POINTS_TO_PIXELS(p);
				tile->setLocalZOrder(-((p.y + 64) / 64));
				this->addChild(tile);
			}
			if (meta)
			{
				auto properties2 = map->getPropertiesForGID(meta).asValueMap();
				if (!properties2.empty())
				{
					auto isHorizontal = properties2["IsHorizontal"].asString();
					if (isHorizontal == "True")
					{
						auto wallSize = properties2["Horizontal"].asInt();
						auto getTile = layerMeta->getTileAt(Vec2(i, j));
						Vec2 offset = Vec2(0, 0);
						if (wallSize % 2 == 0)
						{
							offset = Vec2(32, 0);
						}
						auto tilePhysics = PhysicsBody::createBox(Size(wallSize * 64, 64), PhysicsMaterial(1.0f, 1.0f, 0.0f), offset);
						tilePhysics->setDynamic(false);
						tilePhysics->setContactTestBitmask(0xFFFFFFFF);
						tilePhysics->setTag(WALL_TAG);
						tilePhysics->setGroup(-2);
						getTile->setPhysicsBody(tilePhysics);
						getTile->setZOrder(-1000);
					}
					else if (isHorizontal == "TrueBottom")
					{
						auto wallSize = properties2["Horizontal"].asInt();
						auto getTile = layerMeta->getTileAt(Vec2(i, j));
						Vec2 offset = Vec2(0, -(48));
						if (wallSize % 2 == 0)
						{
							offset += Vec2(32, 0);
						}
						auto tilePhysics = PhysicsBody::createBox(Size(wallSize * 64, 64), PhysicsMaterial(1.0f, 1.0f, 0.0f), offset);
						tilePhysics->setDynamic(false);
						tilePhysics->setGroup(-2);
						tilePhysics->setContactTestBitmask(0xFFFFFFFF);
						tilePhysics->setTag(WALL_TAG);
						getTile->setPhysicsBody(tilePhysics);
						getTile->setZOrder(-1000);
					}
					else
					{
						auto wallSize = properties2["Vertical"].asInt();
						auto getTile = layerMeta->getTileAt(Vec2(i, j));
						Vec2 offset = Vec2(0, 0);
						if (wallSize % 2 == 0)
						{
							offset = Vec2(0, 32);
						}
						auto tilePhysics = PhysicsBody::createBox(Size(64, wallSize * 64), PhysicsMaterial(1.0f, 1.0f, 0.0f), offset);
						tilePhysics->setDynamic(false);
						tilePhysics->setGroup(-2);
						//tilePhysics->setCategoryBitmask(WALL_CATEGORY);
						//tilePhysics->setCollisionBitmask(MASK_WALL);
						tilePhysics->setContactTestBitmask(0xFFFFFFFF);
						tilePhysics->setTag(WALL_TAG);
						getTile->setPhysicsBody(tilePhysics);
						getTile->setZOrder(-1000);
					}
				}
			}
		}
	}
	map->removeChildByTag(1);
	map->removeChildByTag(2);
}

void GameLayer::findEnemyandHurt(Node* node)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i] == node)
		{
			enemyList[i]->damageEnemy();
			if (enemyList[i]->getHealth() == 0)
			{
				enemyList[i]->removeEnemy();
				enemyList.erase(enemyList.begin() + i);
				return;
			}
			return;
		}
	}
}
