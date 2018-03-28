	#include "Enemy.h"
#include "Definitions.h"

/*
Note for AI
Patrol back and forth
if player is in vicinity of flashlight then go to ATTACK player
if player is in vicinity, then go seek players known position.
else go back to patrolling
*/

Enemy::Enemy()
{
	mood = ESLEEP;
	timer = 0.0f;
	isMoving = false;
	isSprinting = false;
	speed = 0;
	health = 5.0f;
	accelL = false;
	accelR = false;
	accelU = false;
	accelD = false;
	dyingAnim = false;
	pastSpawnGate = false;
}

Enemy::~Enemy()
{
}

Enemy * Enemy::create()
{
	Enemy* enemy = new Enemy();
	if (enemy->initWithFile("enemy-idle.png"))
	{
		enemy->autorelease();
		enemy->init();
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Enemy::init()
{
	if (!Node::init())
	{
		return false;
	}
	this->setTexture("e-idleR.png");
	PhysicsBody* body = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(1.f, 1.f, 1.f));
	body->setDynamic(true);
	body->setRotationEnable(false);
	body->setVelocityLimit(130);
	//body->setCategoryBitmask(ENEMY_CATEGORY);
	//body->setCollisionBitmask(MASK_ENEMY);
	body->setContactTestBitmask(-3);
	body->setTag(ENEMY_TAG);
	body->setVelocity(Vec2(0, 0));
	this->setPhysicsBody(body);

	vspeed = 70;
	hspeed = 70;

	initAnimations();

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

	auto contactListene = EventListenerPhysicsContact::create();
	contactListene->onContactBegin = CC_CALLBACK_1(Enemy::onContactBegin, this);
	contactListene->onContactPreSolve = CC_CALLBACK_1(Enemy::onContactPost, this);
	contactListene->onContactSeparate = CC_CALLBACK_1(Enemy::onContactSeparate, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListene, this);

	this->scheduleUpdate();
	return true;
}

void Enemy::update(float dt)
{
	if (mood == ESLEEP)
		sleep(dt);
	else if (mood == ESEEK)
		seek(dt);
	else if (mood == EATTACK)
		attack(dt);
	handleMovement(dt);
	updateAnimation(dt);
}

void Enemy::sleep(float dt )
{

	if (this->getPosition().x >= 2732 - 300)
	{
		accelL = true;
	}
	else
	{
		pastSpawnGate = true;
		accelL = false;
		accelR = true;
		//mood = ESEEK;
		//accelL = false;
		//accelR = false;
		//accelU = false;
		//accelD = false;
		//this->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}
}

void Enemy::seek(float dt)
{

}

void Enemy::attack(float dt)
{
	Vec2 rotVec = player->getPosition() - this->getPosition();
	float rot_ = Vec2::angle(Vec2(0, 1), rotVec.getNormalized()) * (180.f / M_PI) - 90.f;
	if (rotVec.x < 0)
	{
		setVisionRotation(270.f - rot_);
		this->setSpriteFrame(idleFrameL);
	}
	else
	{
		setVisionRotation(rot_ + 90.f);
		this->setSpriteFrame(idleFrameR);
	}
	weapons[cWeapNum]->enemyFire(this->getPosition(), player->getPosition() - this->getPosition());
}

void Enemy::setPlayer(CPlayer * player_)
{
	player = player_;
}

void Enemy::setIfVertical(bool temp_)
{
	vertical = temp_;
}

void Enemy::setSpawn(Vec2 & spawn_)
{
	spawnPosition = spawn_;
}

void Enemy::initAnimations()
{
	FileUtils::getInstance()->addSearchResolutionsOrder("HDR");
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/enemy-1.plist");

	Vector<SpriteFrame*> frames;
	for (int i = 0; i < 8; i++)
	{
		stringstream ss;
		ss << "e-frame-l-00" << i + 1 << ".png";
		frames.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	auto animation = Animation::createWithSpriteFrames(frames, 0.09f);
	animL = Animate::create(animation);
	animL->retain();

	Vector<SpriteFrame*> framesRight;
	for (int i = 0; i < 8; i++)
	{
		stringstream ss;
		ss << "e-frame-r-00" << i + 1 << ".png";
		framesRight.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(framesRight, 0.09f);
	animR = Animate::create(animation);
	animR->retain();

	Vector<SpriteFrame*> framesRunRight;
	for (int i = 0; i < 4; i++)
	{
		stringstream ss;
		ss << "e-rframe-r-00" << i + 1 << ".png";
		framesRunRight.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(framesRunRight, 0.09f);
	runAnimR = Animate::create(animation);
	runAnimR->retain();

	Vector<SpriteFrame*> framesRunLeft;
	for (int i = 0; i < 4; i++)
	{
		stringstream ss;
		ss << "e-rframe-l-00" << i + 1 << ".png";
		framesRunLeft.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(framesRunLeft, 0.09f);
	runAnimL = Animate::create(animation);
	runAnimL->retain();


	Vector<SpriteFrame*> idleright;
	for (int i = 0; i < 1; i++)
	{
		stringstream ss;
		ss << "e-idleR.png";
		idleright.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(idleright, 0.09f);
	idleR = Animate::create(animation);
	idleR->retain();

	Vector<SpriteFrame*> idleleft;
	for (int i = 0; i < 1; i++)
	{
		stringstream ss;
		ss << "e-idleL.png";
		idleleft.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(idleleft, 0.09f);
	idleL = Animate::create(animation);
	idleL->retain();
	this->runAction(RepeatForever::create(idleR));

	stringstream ss1;
	ss1 << "e-idleL.png";
	idleFrameL = spritecache->getSpriteFrameByName(ss1.str());
	idleFrameL->retain();

	stringstream ss2;
	ss2 << "e-idleR.png";
	idleFrameR = spritecache->getSpriteFrameByName(ss2.str());
	idleFrameR->retain();

	spritecache->destroyInstance();
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
	animation = Animation::createWithSpriteFrames(die, 0.09f);
	dying = Animate::create(animation);
	dying->setTag(59);
	dying->retain();
}

void Enemy::updateAnimation(float dt)
{
	if (mood == ESLEEP)
	{
	}
	if (mood == ESEEK)
	{
	}
	if (mood == EATTACK)
	{
		Vec2 rotVec;
		if (player != nullptr)
		{
			rotVec = player->getPosition() - this->getPosition();
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

void Enemy::handleMovement(float dt)
{
	velocity = Vec2(0, 0);
	if (accelL)
		velocity += Vec2(-1, 0) * hspeed;
	if (accelR)
		velocity += Vec2(1, 0) * hspeed;
	if (accelD)
		velocity += Vec2(0, -1) * vspeed;
	if (accelU)
		velocity += Vec2(0, 1) * vspeed;

	this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + velocity);

}

bool Enemy::playerInFOV()
{
	return true;
}

void Enemy::setVisionRotation(float angle)
{
	float rad = angle * (M_PI / 180.0f);
	direction.x = sinf(rad);
	direction.y = cosf(rad);
	search->setRotation(angle);
}

bool Enemy::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if ((nodeA->getPhysicsBody()->getTag() == WALL_TAG && nodeB->getPhysicsBody()->getTag() == ENEMY_TAG) || (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG))
		{
		}
	}
	return true;
}

bool Enemy::onContactPost(PhysicsContact & contact )
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if ((nodeA->getPhysicsBody()->getTag() == WALL_TAG && nodeB->getPhysicsBody()->getTag() == ENEMY_TAG) || (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG))
		{
		}
	}
	return true;
}

bool Enemy::onContactSeparate(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if ((nodeA->getPhysicsBody()->getTag() == WALL_TAG && nodeB->getPhysicsBody()->getTag() == ENEMY_TAG) || (nodeA->getPhysicsBody()->getTag() == ENEMY_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG))
		{

		}
	}
	return true;
}

float Enemy::getDistance()
{
	Vec2 distVec= player->getPosition() - this->getPosition();
	float dist = distVec.getLength();
	return dist;
}

void Enemy::damageEnemy()
{
	this->health--;
}

void Enemy::removeEnemy()
{
	//Stop enemy's state 
	mood = 5;

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

//void Enemy::ifDead()
//{
//}

