#include "CEnemy.h"
#include "Definitions.h"
#include "Item.h"
#include <string>
using std::string;
using std::stringstream;
bool CEnemy::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	this->setTexture("CharacterAnimations/Enemy/original_right.png");

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
	handleMovement(dt);
	updateAnimation(dt);

	auto p = this->getPosition() - Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f);
	p = CC_POINT_POINTS_TO_PIXELS(p);
	this->setLocalZOrder(-((p.y + 64) / 64));
}

void CEnemy::handleMovement(float dt)
{
	//isMoving = true;
	//velocity = Vec2(0, 0);
	//if (accelL)
	//	velocity += Vec2(-1, 0) * speed;
	//if (accelR)
	//	velocity += Vec2(1, 0) * speed;
	//if (accelD)
	//	velocity += Vec2(0, -1) * speed;
	//if (accelU)
	//	velocity += Vec2(0, 1) * speed;
	//if (velocity.x == 0 && velocity.y == 0)
	//{
	//	isMoving = false;
	//}

	//if (!isMoving)
	//{
	//	this->getPhysicsBody()->setVelocity(Vec2(0,0));
	//}
	//else
	//{
	//	this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + velocity);
	//
	isMoving = false;
	if (accelU || accelL || accelR || accelD)
	{
		isMoving = true;
	}

	velocity = Vec2(0, 0);
	if (accelL)
	{
		velocity += Vec2(-1, 0) * speed;
	}
	if (accelR)
	{
		velocity += Vec2(1, 0) * speed;
		velNorm += Vec2(1, 0);
	}
	if (accelU)
	{
		velocity += Vec2(0, 1) * speed;
		velNorm += Vec2(0, 1);
	}
	if (accelD)
	{
		velocity += Vec2(0, -1) * speed;
		velNorm += Vec2(0, -1);
	}

	Vec2 nextVelocity = velocity;
	if (isMoving)
	{
		this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + nextVelocity);
	}
	else
	{
		this->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}

	if (mood == 1)
	{
		accelL = accelR = accelU = accelD = false;
	}
}

void CEnemy::updateAnimation(float dt)
{
	Vec2 rotVec;
	float rot_ = 0;
	if (mood == ENEMYATTACK)
	{
		if (player_ != nullptr)
		{
			rotVec = rotVec = player_->getPosition() - this->getPosition();
			rot_ = Vec2::angle(Vec2(0, 1), rotVec.getNormalized()) * (180.f / M_PI) - 90.f;
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

	faceLeft_ = false;
	faceRight_ = false;
	faceUp_ = false;
	faceDown_ = false;


	if (rot_ >= -45 && rot_ < 45)
		faceRight_ = true;
	else if (rot_ >= 45 && rot_ < 315)
		faceDown_ = true;
	else if (rot_ >= 315 && rot_ < 405)
		faceLeft_ = true;
	else
		faceUp_ = true;
	//Determine if the character is facing left, right, up or down. It is very important for the animation
	if (faceLeft_)
		orientation_ = FACELEFT;
	else if (faceRight_)
		orientation_ = FACERIGHT;
	else if (faceDown_)
		orientation_ = FACEDOWN;
	else
		orientation_ = FACEUP;

	if (weapons.size() != 0)
	{
		if (cOrientation_ == FACEUP)
			weapons[cWeapNum]->setLocalZOrder(-1000);
		else
			weapons[cWeapNum]->setLocalZOrder(1000);
	}

	//if (cOrientation_ != orientation_)
	//{
	//	cOrientation_ = orientation_;
	//	string tmp = "CharacterAnimations/Enemy/Walking/" + cOrientation_ + "/idle.png";
	//	//std::cout << tmp << std::endl;
	//	this->setTexture(tmp);
	//}

	if (isMoving)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/Character/Walking.wav", false);
		animationTimer += dt;
		if (animationTimer >= 0.09f * animationCounter)
			animationCounter++;
		if (animationCounter >= 8)
		{
			animationCounter = 1;
			animationTimer = 0.0f;
		}

		if (currentACounter != animationCounter)
		{
			currentACounter = animationCounter;
			if (cOrientation_ != orientation_)
			{
				cOrientation_ = orientation_;
			}
			string tmp = "CharacterAnimations/Enemy/Walking/" + cOrientation_ + "/frame" + std::to_string(animationCounter) + ".png";
			/*std::cout << tmp << std::endl;*/
			this->setTexture(tmp);
		}
	}
	else
	{
		animationTimer = 0.f;
		currentACounter = 1;
		animationCounter = 1;
		if (cOrientation_ != orientation_)
		{
			cOrientation_ = orientation_;
		}
		string tmp = "CharacterAnimations/Enemy/Walking/" + cOrientation_ + "/idle.png";
		//std::cout << tmp << std::endl;
		this->setTexture(tmp);
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
	Vec2 itemPos = this->getPosition();

	mood = 5;
	weapons[cWeapNum]->setVisible(false);
	this->getPhysicsBody()->setVelocity(Vec2(0, 0));
	this->getPhysicsBody()->setVelocityLimit(0);
	this->getPhysicsBody()->removeFromWorld();
	this->unscheduleUpdate();
	if (!dyingAnim)
	{
		dyingAnim = true;
		this->stopAllActions();
		dyingSeq = Sequence::create(dying, RemoveSelf::create(), nullptr);
		this->runAction(dyingSeq);

		EventCustom event2("heartSpawn");
		//event2.setUserData((void*)&this->getPosition());
		_eventDispatcher->dispatchEvent(&event2);
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
