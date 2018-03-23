
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

	initLevel();
	//map->getLayer("Layer")->setGlobalZOrder(100);
	cplayer = CPlayer::create("original.png");
	cplayer->retain();
	cplayer->setPosition(640, 640);
	this->addChild(cplayer, 0, PLAYERNAME);

	Item* item = Item::create("silencedGun.png");
	item->setPosition(Vec2(750, 640));
	this->addChild(item, 2);

	//Enemy* enemy = Enemy::create();
	//this->addChild(enemy);
	//enemy->setPosition(Vec2(3000, 1600));
	//enemy->setPlayer(cplayer);
	//enemy->setIfVertical(false);
	//enemy->setSpawn(Vec2(950, 3000));

	enemySpawn1 = Vec2(-320, 1728);
	enemySpawn2 = Vec2(3456 + 320, 1728);
	enemySpawn3 = Vec2(1728, -320);
	enemySpawn4 = Vec2(1728, 3456 + 320);

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


	//this->addChild(camTarget->target);
	//this->runAction(camTarget->camera);
	////sceneFollow = Follow::create(player);

	//this->addChild(node);

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
			EventCustom event("roundStart");

			_eventDispatcher->dispatchEvent(&event);
			roundStart_ = true;
		}
	}
	else
	{
		if (enemyList.size() < 3)
		{
			for (int i = 0; i < 4; i++)
			{
				if (i == 0)
				{
					Enemy* enemy = Enemy::create();
					enemy->setTag(enemyList.size());
					this->addChild(enemy);
					enemy->setPosition(enemySpawn1);
					enemy->setSpawn(enemySpawn1);
					enemyList.push_back(enemy);
				}
				if (i == 1)
				{
					Enemy* enemy = Enemy::create();
					this->addChild(enemy);
					enemy->setTag(enemyList.size());
					enemy->setPosition(enemySpawn2);
					enemy->setSpawn(enemySpawn2);
					enemyList.push_back(enemy);
				}
				if (i == 2)
				{
					Enemy* enemy = Enemy::create();
					this->addChild(enemy);
					enemy->setTag(enemyList.size());
					enemy->setPosition(enemySpawn3);
					enemy->setSpawn(enemySpawn3);
					enemyList.push_back(enemy);
				}
				if (i == 3)
				{
					Enemy* enemy = Enemy::create();
					this->addChild(enemy);
					enemy->setTag(enemyList.size());
					enemy->setPosition(enemySpawn4);
					enemy->setSpawn(enemySpawn4);
					enemyList.push_back(enemy);
				}
			}
		}
	}
}


bool GameLayer::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == 11 && nodeB->getPhysicsBody()->getTag() == 4)
		{
			//this->health -= 1.f;
			//if (this->health == 0.0f)
			//{
			//	std::cout << "dead" << std::endl;
			//	mood = 4;
			//	nodeA->stopAllActions();
			//	nodeA->getPhysicsBody()->setVelocityLimit(0);
			//	nodeA->getPhysicsBody()->setVelocity(Vec2(0, 0));
			//	/*this->runAction(RepeatForever::create(dying));*/
			//	auto seq = Sequence::create(dying, RemoveSelf::create(), nullptr);
			//	nodeA->runAction(seq);
			//}
			findEnemyandHurt(nodeA);
			std::cout << nodeA->getTag() << "asd" << std::endl;
		}
		else if (nodeA->getPhysicsBody()->getTag() == 4 && nodeB->getPhysicsBody()->getTag() == 11)
		{
			std::cout << nodeB->getTag() << "asd" << std::endl;
			findEnemyandHurt(nodeB);
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
	auto p = cplayer->getPosition() - Vec2(cplayer->getContentSize().width * 0.5f, cplayer->getContentSize().height * 0.5f + 5);
	p = CC_POINT_POINTS_TO_PIXELS(p);
	cplayer->setPositionZ(-((p.y + 64) / 64));

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
				auto p = tile->getPosition() - Vec2(tile->getContentSize().width * 0.5f, tile->getContentSize().height * 0.5f);
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
						tilePhysics->setCategoryBitmask(0x01);
						tilePhysics->setCollisionBitmask(0x02);
						tilePhysics->setTag(2);
						getTile->setPhysicsBody(tilePhysics);
						getTile->setZOrder(-1000);
					}
					else if (isHorizontal == "TrueBottom")
					{
						auto wallSize = properties2["Horizontal"].asInt();
						auto getTile = layerMeta->getTileAt(Vec2(i, j));
						Vec2 offset = Vec2(0, -(64));
						if (wallSize % 2 == 0)
						{
							offset += Vec2(32, 0);
						}
						auto tilePhysics = PhysicsBody::createBox(Size(wallSize * 64, 64), PhysicsMaterial(1.0f, 1.0f, 0.0f), offset);
						tilePhysics->setDynamic(false);
						tilePhysics->setContactTestBitmask(0xFFFFFFFF);
						tilePhysics->setCategoryBitmask(0x01);
						tilePhysics->setCollisionBitmask(0x02);
						tilePhysics->setTag(2);
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
						tilePhysics->setContactTestBitmask(0xFFFFFFFF);
						tilePhysics->setCategoryBitmask(0x01);
						tilePhysics->setCollisionBitmask(0x02);
						tilePhysics->setTag(2);
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
