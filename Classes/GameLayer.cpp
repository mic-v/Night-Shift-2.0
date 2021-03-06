
#include "GameLayer.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include "SimpleAudioEngine.h"
#include "Definitions.h"
#include "GameCamera.h"
#include <fstream>
USING_NS_CC;
using namespace cocos2d;

string GameLayer::saveFile = "";
int GameLayer::level_ = 1;

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* cameraTarget_ = Sprite::create();
	this->addChild(cameraTarget_);
	CAMERA->setCameraTarget(cameraTarget_);
	CAMERA->setCameraFollow();

	// LEVEL SETUP
	enemySpawn1 = Vec2(-320, 1728);
	enemySpawn2 = Vec2(3776, 1728);
	enemySpawn3 = Vec2(1728, -320);
	enemySpawn4 = Vec2(1728, 3776);

	initLevel();
	levelTimer = 0.f;
	spawnLimit = level_ * 4;
	spawnRate = 0.f;
	randomSpawn = 0;
	randomWeapon = 0;

	cplayer = CPlayer::create("original.png");
	cplayer->retain();
	cplayer->setPosition(640, 640);
	this->addChild(cplayer, 0, PLAYERNAME);

	Item* item2 = Item::create("Items/silencedGun.png");
	item2->setName("handOnGun.png");
	item2->setPosition(Vec2(890, 640));
	this->addChild(item2, 2);

	roundStart_ = false;
	dontmove = false;
	wallCollision = false;
	spawnHeart = false;
	this->scheduleUpdate();

	this->runAction(CAMERA->getCamera());


	auto heartSpawnListener = EventListenerCustom::create("heartSpawn", [=](EventCustom* event) {
		Vec2* tmp = static_cast<Vec2*>(event->getUserData());
		spawnHeartPos = *tmp;
		spawnHeart = true;
	});

	auto akSpawnListener = EventListenerCustom::create("akSpawn", [=](EventCustom* event) {
		Vec2* tmp = static_cast<Vec2*>(event->getUserData());
		spawnAKPos = *tmp;
		spawnAK = true;
	});


	auto ammoSpawnListener = EventListenerCustom::create("m16Spawn", [=](EventCustom* event) {
		Vec2* tmp = static_cast<Vec2*>(event->getUserData());
		spawnAmmoPos = *tmp;
		spawnAmmo = true;
	});


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(heartSpawnListener, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(akSpawnListener, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(ammoSpawnListener, this);

	return true;
}

void GameLayer::setSaveFile(string saveFile_)
{
	saveFile = saveFile_;
}

void GameLayer::update(float dt)
{

	if (spawnHeart)
	{
		createHeart(spawnHeartPos);
		spawnHeart = false;
	}
	if (spawnAK)
	{
		createAK(spawnAKPos);
		spawnAK = false;
	}
	if (spawnAmmo)
	{

	}

	if (!roundStart_)
	{
		if (cplayer->getPosition().y > 860 && cplayer->hasPistol())
		{

			levelTimer += dt;
			std::string tmp = "12 AM";
			EventCustom event("timer");
			event.setUserData((void*)tmp.c_str());

			_eventDispatcher->dispatchEvent(&event);

			EventCustom event2("roundStart");
			_eventDispatcher->dispatchEvent(&event2);

			for (int i = 0; i < 4; i++)
			{
				Sprite* door = Sprite::create("Tiles/Wall1.png");
				door->setPosition(64 * i + 64 * 13, 12 * 64);
				door->setAnchorPoint(Vec2(0, 0));
				this->addChild(door);
				doorList.push_back(door);
			}
			auto wallBody = PhysicsBody::createBox(Size(256, 64), PhysicsMaterial(1.0f, 1.0f, 0.0f), Vec2(-32, -96));
			wallBody->setDynamic(false);
			wallBody->setContactTestBitmask(0xFFFFFFFF);
			wallBody->setTag(WALL_TAG);
			wallBody->setGroup(-3);
			doorList[2]->setPhysicsBody(wallBody);
			roundStart_ = true;
		}
	}
	else if (roundStart_ && !roundEnd_)
	{
		levelTimer += dt;
		if (enemyList.size() < spawnLimit)
		{
			spawnRate += dt;
			std::cout << level_ << std::endl;
			if (spawnRate >= 8.0)
			{
				spawnRate = 0;
				randomSpawn = RandomHelper::random_int(1, 4);
				if (randomSpawn == 1)
				{
					CEnemy* enemy = CEnemy::create();
					if(level_ % 2 == 0)
						enemy->setZombie(true);
					enemy->setPlayer(*cplayer);
					enemy->setSpawn(enemySpawn1);
					this->addChild(enemy);
					enemyList.push_back(enemy);
				}
				else if (randomSpawn == 2)
				{
					CEnemy* enemy = CEnemy::create();
					if (level_ % 2 == 0)
						enemy->setZombie(true);
					enemy->setPlayer(*cplayer);
					enemy->setSpawn(enemySpawn2);
					this->addChild(enemy);
					enemyList.push_back(enemy);
				}
				else if (randomSpawn == 3)
				{
					CEnemy* enemy = CEnemy::create();
					if (level_ % 2 == 0)
						enemy->setZombie(true);
					enemy->setPlayer(*cplayer);
					enemy->setSpawn(enemySpawn3);
					this->addChild(enemy);
					enemyList.push_back(enemy);
				}
				else if (randomSpawn == 4)
				{
					CEnemy* enemy = CEnemy::create();
					if (level_ % 2 == 0)
						enemy->setZombie(true);
					enemy->setPlayer(*cplayer);
					enemy->setSpawn(enemySpawn4);
					this->addChild(enemy);
					enemyList.push_back(enemy);
				}

			}
		}

		if ((int)levelTimer == MAXTIME * 0.2)
		{
			std::string tmp = "1 AM";
			EventCustom event("timer");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else if ((int)levelTimer == MAXTIME * 0.4)
		{
			std::string tmp = "2 AM";
			EventCustom event("timer");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else  if ((int)levelTimer == MAXTIME * 0.6)
		{
			std::string tmp = "3 AM";
			EventCustom event("timer");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else if ((int)levelTimer == MAXTIME * 0.8)
		{
			std::string tmp = "4 AM";
			EventCustom event("timer");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
		}
		else if ((int)levelTimer == MAXTIME)
		{
			std::string tmp = "5 AM";
			EventCustom event("timer");
			event.setUserData((void*)tmp.c_str());
			_eventDispatcher->dispatchEvent(&event);
			roundEnd_ = true;
			//roundStart_ = false;
		}
	}
	else if (roundEnd_)
	{
		if (enemyList.size() <= 0)
		{
			EventCustom event("roundEnd");
			_eventDispatcher->dispatchEvent(&event);
			if (doorList.size() != 0)
			{
				for (int i = 0; i < doorList.size(); i++)
				{
					doorList[i]->runAction(RemoveSelf::create());
				}
				doorList.clear();
			}
			if (cplayer->getPosition().y < 860)
			{
				EventCustom event("finishShift");
				_eventDispatcher->dispatchEvent(&event);
			}
		}
		else
		{
			EventCustom event("finishEnemy");
			_eventDispatcher->dispatchEvent(&event);
		}

	}

}

void GameLayer::createHeart(Vec2 & pos_)
{
	Item* item = Item::create("Items/heart.png");
	item->setName("heart.png");
	item->getPhysicsBody()->setTag(HEART_TAG);
	item->setPosition(pos_);


	this->addChild(item, 2);
}

void GameLayer::createAK(Vec2 & pos_)
{
	Item* item = Item::create("AK47NOHANDS.png");
	item->setName("AK47.png");
	item->setPosition(pos_);

	this->addChild(item, 2);
}

void GameLayer::createAmmo(Vec2 & pos_)
{
	Item* item = Item::create("Items/AKClip.png");
	item->setName("AKClip.png");
	item->getPhysicsBody()->setTag(16);
	item->setPosition(pos_);

	this->addChild(item, 2);
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

		if (nodeA->getPhysicsBody()->getTag() == PLAYER_TAG && nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG)
		{
			cplayer->damage();
			return true;
		}
		else if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == PLAYER_TAG)
		{
			cplayer->damage();
			return true;
		}
	}
	return true;
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
	//SpriteBatchNode* spriteSheet = SpriteBatchNode::create("Tiles/LevelTiles.png");
	//this->addChild(spriteSheet);
	//
	//SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
	//spriteFrameCache->addSpriteFramesWithFile

	auto map = TMXTiledMap::create("level/Arena-1.3.tmx");
	this->addChild(map,-500,20);
	auto layerMeta = map->getLayer("LayerMeta");
	auto layer64Top = map->getLayer("Layer64Top");
	auto layer256 = map->getLayer("Layer256");
	auto layer128 = map->getLayer("Layer128");
	for (int i = 0; i < layerMeta->getLayerSize().width; i++)
	{
		for (int j = layerMeta->getLayerSize().height - 1; j >= 0; j--)
		{
			auto gid64 = layer64Top->getTileGIDAt(Vec2(i, j));
			auto gid256 = layer256->getTileGIDAt(Vec2(i, j));
			auto gid128 = layer128->getTileGIDAt(Vec2(i, j));
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
			if (gid128)
			{
				Sprite* tile = Sprite::create("Pillar.png");
				tile->setAnchorPoint(Vec2(0, 0));
				tile->setPosition(Vec2(i * 64, (layer128->getLayerSize().height - j - 1) * 64));
				auto p = tile->getPosition();
				p = CC_POINT_POINTS_TO_PIXELS(p);
				tile->setLocalZOrder(-((p.y + 64) / 64));
				this->addChild(tile);

				auto tilePhysics = PhysicsBody::createBox(Size(48, 64), PhysicsMaterial(1.0f, 1.0f, 0.0f));
				tilePhysics->setDynamic(false);
				tilePhysics->setGroup(-3);
				tilePhysics->setContactTestBitmask(0xFFFFFFFF);
				tilePhysics->setTag(WALL_TAG);
				tile->setPhysicsBody(tilePhysics);
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
						tilePhysics->setGroup(-3);
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
						tilePhysics->setGroup(-3);
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
						tilePhysics->setGroup(-3);
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
	map->removeChildByTag(3);

	Sprite* desk = Sprite::create("Tiles/Desk.png");
	//desk->setScale(3.f);
	desk->setPosition(640, 640);
	desk->setLocalZOrder(getObjectZ(desk->getPosition()));
	auto deskBody = PhysicsBody::createBox(Size(desk->getContentSize().width, 32));
	deskBody->setDynamic(false);
	deskBody->setContactTestBitmask(0xFFFFFFFF);
	deskBody->setTag(WALL_TAG);
	deskBody->setGroup(-3);
	desk->setPhysicsBody(deskBody);
	this->addChild(desk);

	Sprite* dummy = Sprite::create("Tiles/Dummy_Left.png");
	dummy->setLocalZOrder(300);
	dummy->setPosition(1030, 640);
	auto dummyBody = PhysicsBody::createBox(dummy->getContentSize());
	dummyBody->setDynamic(false);
	dummyBody->setContactTestBitmask(0xFFFFFFFF);
	dummyBody->setTag(WALL_TAG);
	dummyBody->setGroup(-3);
	dummy->setPhysicsBody(dummyBody);
	dummy->setLocalZOrder(getObjectZ(dummy->getPosition()));
	this->addChild(dummy);

	Sprite* poster = Sprite::create("Tiles/poster.png");
	poster->setPosition(640, 864);
	poster->setLocalZOrder(getObjectZ(Vec2(640,764)));
	this->addChild(poster);
	
}

void GameLayer::findEnemyandHurt(Node* node)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i] == node)
		{
			enemyList[i]->damageEnemy();
			if (enemyList[i]->getHealth() <= 0)
			{
				enemyList[i]->removeEnemy();
				enemyList.erase(enemyList.begin() + i);
				return;
			}
			return;
		}
	}
}

int GameLayer::getObjectZ(Vec2 pos_)
{
	auto p = CC_POINT_POINTS_TO_PIXELS(pos_);
	return (-((p.y + 64) / 64));
}
