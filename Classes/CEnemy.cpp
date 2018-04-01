#include "CEnemy.h"
#include "Definitions.h"
bool CEnemy::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	this->setTexture("e-idleR.png");

	initAnimations();

	PhysicsBody* body = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(1.f, 1.f, 1.f));
	body->setDynamic(true);
	body->setRotationEnable(false);
	body->setVelocityLimit(130);
	body->setVelocity(Vec2(0, 0));
	body->setCategoryBitmask(ENEMY_CATEGORY);
	body->setCollisionBitmask(MASK_ENEMY);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setGroup(-3);
	body->setTag(ENEMY_TAG);
	this->setPhysicsBody(body);

	accelL = accelR = accelU = accelD = false;
	pastSpawnGate = false;
	dyingAnim = false;
	health = 4.0f;

	//SETUP WEAPON
	Weapon* weapon = Weapon::create("silencedGun.png");
	weapon->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f + 10.f));
	weapon->setAnchorPoint(Vec2(-0.57f, 0.5f));
	weapon->setIfPlayer(false);
	//weapon->set
	weapons.push_back(weapon);
	cWeapNum = 0;
	weapons[cWeapNum]->setVisible(false);
	weapons[cWeapNum]->unscheduleUpdate();
	this->addChild(weapon, 3);

	speed = 70;
	movementTimer = 0.f;
	mood = ENEMYMOVESPAWN;
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(CEnemy::onContactBegin, this);
	//contactListener->onContactPreSolve = CC_CALLBACK_1(CEnemy::onContactPost, this);
	//contactListener->onContactSeparate = CC_CALLBACK_1(CEnemy::onContactSeparate, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


	this->scheduleUpdate();

	return true;
}

void CEnemy::initAnimations()
{
	auto spritecache2 = SpriteFrameCache::getInstance();
	spritecache2->addSpriteFramesWithFile("res/HDR/enemy-dying.plist");
	//this->stopAction(idleR);
	//this->getActionByTag()
	Vector<SpriteFrame*> die;
	for (int i = 0; i < 7; i++)
	{
		stringstream ss;
		ss << "frame" << i + 1 << ".png";
		die.pushBack(spritecache2->getSpriteFrameByName(ss.str()));
	}
	auto animation = Animation::createWithSpriteFrames(die, 0.09f);
	dying = Animate::create(animation);
	dying->setTag(59);
	dying->retain();
}

void CEnemy::setSpawn(Vec2 & position_)
{
	this->setPosition(position_);
	spawnPosition = position_;
	if (position_.x >= 3776)
	{
		accelL = true;
		spawnGate = Vec2(2496, 0);
	}
	else if (position_.x <= -320)
	{
		accelR = true;
		spawnGate = Vec2(896, 0);
	}
	else if (position_.y >= 3776)
	{
		accelD = true;
		spawnGate = Vec2(0, 2496);
	}
	else if (position_.y <= -320)
	{
		accelU = true;
		spawnGate = Vec2(0, 896);
	}
}

void CEnemy::update(float dt)
{
	if (mood == ENEMYMOVESPAWN)
	{
		exitSpawn(dt);
	}
	else if (mood == ENEMYATTACK)
	{
		attackPlayer(dt);
	}
	updateMovement();
	updateAnimation();

	auto p = this->getPosition() - Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f);
	p = CC_POINT_POINTS_TO_PIXELS(p);
	this->setLocalZOrder(-((p.y + 64) / 64));
}

void CEnemy::updateMovement()
{
	isMoving = true;
	velocity = Vec2(0, 0);
	if (accelL)
		velocity += Vec2(-1, 0) * speed;
	if (accelR)
		velocity += Vec2(1, 0) * speed;
	if (accelD)
		velocity += Vec2(0, -1) * speed;
	if (accelU)
		velocity += Vec2(0, 1) * speed;
	if (velocity.x == 0 && velocity.y == 0)
	{
		isMoving = false;
	}

	if (!isMoving)
	{
		this->getPhysicsBody()->setVelocity(Vec2(0,0));
	}
	else
	{
		this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + velocity);
	}
	if (mood == 1)
	{
		accelL = accelR = accelU = accelD = false;
	}
}

void CEnemy::updateAnimation()
{
	if (mood == ENEMYATTACK)
	{
		Vec2 rotVec;
		if (player_ != nullptr)
		{
			rotVec = player_->getPosition() - this->getPosition();
			float rot_ = Vec2::angle(Vec2(0, 1), rotVec.getNormalized()) * (180.f / M_PI) - 90.f;
			if (rotVec.x < 0)
			{
				rot_ = 360 - rot_;
				weapons[cWeapNum]->setScaleX(-1.f);
			}
			else
			{
				weapons[cWeapNum]->setScaleX(1.f);
			}
			weapons[cWeapNum]->setRotation(rot_);
		}
	}
}

void CEnemy::exitSpawn(float dt)
{
	if (spawnGate.x == 2496)
	{
		if (this->getPosition().x > spawnGate.x)
		{
			accelL = true;
		}
		else
		{
			mood = ENEMYATTACK;
			pastSpawnGate = true;
			//this->getPhysicsBody()->setCategoryBitmask(ENEMY_CATEGORY);
			//this->getPhysicsBody()->setCollisionBitmask(MASK_ENEMY);
			//this->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			//this->getPhysicsBody()->setTag(ENEMY_TAG);
			accelL = false;
			accelR = false;
			accelU = false;
			accelD = false;
			weapons[cWeapNum]->setVisible(true);
		}
	}
	else if (spawnGate.x == 896)
	{
		if (this->getPosition().x < spawnGate.x)
		{
			accelR = true;
		}
		else
		{
			mood = ENEMYATTACK;
			pastSpawnGate = true;
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			weapons[cWeapNum]->setVisible(true);
		}
	}
	else if (spawnGate.y == 2496)
	{
		if (this->getPosition().y > spawnGate.y)
			accelD = true;
		else
		{
			pastSpawnGate = true;
			mood = ENEMYATTACK;
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			accelL = false;
			accelR = false;
			accelU = false;
			accelD = false;
			weapons[cWeapNum]->setVisible(true);

		}
	}
	else if (spawnGate.y == 896)
	{
		if (this->getPosition().y < spawnGate.y)
			accelU = true;
		else
		{
			pastSpawnGate = true;
			mood = ENEMYATTACK;
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			accelL = false;
			accelR = false;
			accelU = false;
			accelD = false;
			weapons[cWeapNum]->setVisible(true);

		}
	}
	
}

void CEnemy::attackPlayer(float dt)
{
	//accelL = true;
	weapons[cWeapNum]->enemyFire(this->getPosition(), player_->getPosition() - this->getPosition());
	int randomMove = RandomHelper::random_int(1, 9);
	movementTimer += dt;
	if (movementTimer >= 1.f)
	{
		switch (randomMove) {
		case 1:
			accelL = true;
			break;
		case 2:
			accelR = true;
			break;
		case 3:
			accelU = true;
			break;
		case 4:
			accelD = true;
			break;
		case 5:
			accelL = true;
			accelD = true;
			break;
		case 6:
			accelU = true;
			accelL = true;
			break;
		case 7:
			accelR = true;
			accelD = true;
			break;
		case 8:
			accelR = true;
			accelU = true;
			break;
		case 9:
			accelL = accelR = accelU = accelD = false;
			break;
		}
		movementTimer = 0.f;
	}
}

void CEnemy::setPlayer(CPlayer & player)
{
	player_ = &(player);
}

void CEnemy::damageEnemy()
{
	this->health--;
}

void CEnemy::removeEnemy()
{
	//Stop enemy's state 
	mood = 5;
	weapons[cWeapNum]->setVisible(false);
	this->getPhysicsBody()->setVelocity(Vec2(0, 0));
	this->getPhysicsBody()->setVelocityLimit(0);
	this->getPhysicsBody()->removeFromWorld();
	if (!dyingAnim)
	{
		dyingAnim = true;
		this->stopAllActions();
		dyingSeq = Sequence::create(dying, RemoveSelf::create(), nullptr);
		this->runAction(dyingSeq);
	}
	else
	{

	}
}

bool CEnemy::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG)
		{
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMY_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
		}
	}
	return true;
}

bool CEnemy::onContactPost(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG)
		{
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMY_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
		}
	}
	return true;
}

bool CEnemy::onContactSeparate(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
	}
	return true;
}
