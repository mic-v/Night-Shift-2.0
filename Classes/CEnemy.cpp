#include "CEnemy.h"
#include "Definitions.h"
bool CEnemy::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	this->setTexture("e-idleR.png");

	PhysicsBody* body = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(1.f, 1.f, 1.f));
	body->setDynamic(true);
	body->setRotationEnable(false);
	body->setVelocityLimit(130);
	body->setVelocity(Vec2(0, 0));
	body->setCategoryBitmask(ENEMY_CATEGORY);
	body->setCollisionBitmask(MASK_ENEMY);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setGroup(-2);
	body->setTag(ENEMY_TAG);
	this->setPhysicsBody(body);

	accelL = accelR = accelU = accelD = false;
	pastSpawnGate = false;

	speed = 70;
	mood = ENEMYMOVESPAWN;
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(CEnemy::onContactBegin, this);
	//contactListener->onContactPreSolve = CC_CALLBACK_1(CEnemy::onContactPost, this);
	//contactListener->onContactSeparate = CC_CALLBACK_1(CEnemy::onContactSeparate, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


	this->scheduleUpdate();

	return true;
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
		exitSpawn();
	}
	else if (mood == ENEMYATTACK)
	{
		attackPlayer();
	}
	updateMovement();
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

	accelL = accelR = accelU = accelD = false;
}

void CEnemy::exitSpawn()
{
	if (spawnGate.x == 2486)
	{
		if(this->getPosition().x > spawnGate.x)
			accelL = true;
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
		}
	}
	else if (spawnGate.x == 896)
	{
		std::cout << this->getPosition().x << std::endl;
		if (this->getPosition().x < spawnGate.x)
		{
			accelR = true;
		}
		else
		{
			mood = ENEMYATTACK;
			//this->getPhysicsBody()->setCategoryBitmask(ENEMY_CATEGORY);
			//this->getPhysicsBody()->setCollisionBitmask(MASK_ENEMY);
			//this->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
			//this->getPhysicsBody()->setGroup(-1);
			//this->getPhysicsBody()->setTag(ENEMY_TAG);
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			//accelL = false;
			//accelR = false;
			//accelU = false;
			//accelD = false;
		}
	}
	else if (spawnGate.y == 2496)
	{
		if (this->getPosition().y > spawnGate.y)
			accelD = true;
		else
		{
			mood = ENEMYATTACK;
			//this->getPhysicsBody()->setCategoryBitmask(ENEMY_CATEGORY);
			//this->getPhysicsBody()->setCollisionBitmask(MASK_ENEMY);
			//this->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
			//this->getPhysicsBody()->setGroup(-1);
			//this->getPhysicsBody()->setTag(ENEMY_TAG);
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			accelL = false;
			accelR = false;
			accelU = false;
			accelD = false;

		}
	}
	else if (spawnGate.y == 896)
	{
		if (this->getPosition().y < spawnGate.y)
			accelU = true;
		else
		{
			mood = ENEMYATTACK;
			this->getPhysicsBody()->setVelocity(Vec2(0, 0));
			this->getPhysicsBody()->setGroup(-1);
			accelL = false;
			accelR = false;
			accelU = false;
			accelD = false;

		}
	}
	
}

void CEnemy::attackPlayer()
{
	//accelL = true;
}

bool CEnemy::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG)
		{
			if (!pastSpawnGate)
			{
				return false;
			}
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMY_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
			if (!pastSpawnGate)
			{
				return false;
			}
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
			if (!pastSpawnGate)
			{
				return false;
			}
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMY_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
			if (!pastSpawnGate)
			{
				return false;
			}
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
		if (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG)
		{
			return true;
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMY_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
			return true;
		}
	}
	return true;
}