#include "Player.h"
#include "InputHandler.h"
#include <iostream>
Player::Player()
{
}

Player::~Player()
{
}

Player * Player::create()
{
	Player* player = new Player();
	if (player->initWithFile("idle.png"))
	{
		player->autorelease();
		player->init();
		return player;
	}
	CC_SAFE_DELETE(player);
	return NULL;
}

bool Player::init()
{
	if (!this->initWithFile("idle.png"))
	{
		return false;
	}

	PhysicsBody* body = PhysicsBody::createBox(this->getContentSize());
	body->setRotationEnable(false);
	body->setDynamic(true);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setCategoryBitmask(0x02);
	body->setCollisionBitmask(0x01);
	body->setTag(10);
	body->setVelocityLimit(100.f);
	this->setTag(10);
	this->setPhysicsBody(body);

	accelL = false; 
	accelR = false;
	accelU = false;
	accelD = false;
	collD = false;
	collU = false;
	collL = false;
	collR = false;

	velocityLimit = 300.f;

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Player::onContactBegin, this);
	contactListener->onContactPreSolve = CC_CALLBACK_1(Player::onContactPost, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(Player::onContactSeperate, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	//this->_eventDispatcher->addEventListenerWithFixedPriority(contactListener, -20);

	scheduleUpdate();
	return true;
}

bool Player::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 1)
		{
			std::cout << "player 1" << std::endl;
			//velocityLimit = 10.f;
			this->setPosition(previousPos);
			return true;
		}
		else if (nodeB->getPhysicsBody()->getTag() == 2 && nodeA->getPhysicsBody()->getTag() == 1)
		{
			////velocity = -velocity;
			//Vec2 vel = Vec2(0, 0);
			//if (velocity.x == 1)
			//{
			//	std::cout << "bo" << std::endl;
			//	vel.x -= velocity.x;
			//}
			//if (velocity.x == -1)
			//{
			//	std::cout << "boA" << std::endl;
			//	vel.x -= velocity.x;
			//}
			//if (velocity.y == 1)
			//{
			//	std::cout << "boS" << std::endl;
			//	vel.y -= velocity.y;
			//}
			//if (velocity.y == -1)
			//{
			//	std::cout << "boE" << std::endl;
			//	vel.y -= velocity.y;
			//}
			//Vec2 newPos = this->getPosition() + vel.getNormalized() * 1;
			//std::cout << "player 2" << std::endl;
			//this->setPosition(newPos);
			//velocityLimit = 10.f;
			this->setPosition(previousPos);
			return true;
		}
	}
	return false;
}

bool Player::onContactPost(PhysicsContact & contact)
{
	std::cout << "yes" << std::endl;
	if (velocity.x = 1)
	{
		//velocity.x = 0;
		accelR = false;
		collR = true;
		std::cout << "TEST" << std::endl;

	}
	else if (velocity.x = -1)
	{
		//velocity.x = 0;
		accelL = false;
		collL = true;
		std::cout << "TEST" << std::endl;
	}
	//velocityLimit = -340.f;
	//velocityLimit = -350.f;
	return true;
	//auto nodeA = contact.getShapeA()->getBody()->getNode();
	//auto nodeB = contact.getShapeB()->getBody()->getNode();
	//if (nodeA && nodeB)
	//{
	//	if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 10)
	//	{

	//		std::cout << "TOUCHING 1" << std::endl;
	//		//velocity = Vec2(0, 0);
	//		//return true;
	//		return true;
	//	}
	//	else if (nodeB->getPhysicsBody()->getTag() == 2 && nodeA->getPhysicsBody()->getTag() == 10)
	//	{
	//		////velocity = -velocity;
	//		//velocity = Vec2(0, 0);
	//		//std::cout << "player 2" << std::endl;
	//		std::cout << "TOUCHING 2" << std::endl;
	//		return true;
	//		//return true;
	//	}
	//}
	//return false;
}

bool Player::onContactSeperate(PhysicsContact &)
{
	velocityLimit = 300.f;
	std::cout << "seperate" << std::endl;
	collR = false;
	collL = false;
	return true;
}

void Player::handleInput()
{
	if (INPUTS->getKey(KeyCode::KEY_A))
	{
		std::cout << "A" << std::endl;
		accelL = true;
		//this->setPosition(this->getPosition() + Vec2(-10, 0));
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_A))
	{
		std::cout << "A released" << std::endl;
		accelL = false;


	}
	if (INPUTS->getKey(KeyCode::KEY_D))
	{
		std::cout << "A" << std::endl;
		accelR = true;
		//this->setPosition(this->getPosition() + Vec2(10, 0));
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_D))
	{
		std::cout << "A released" << std::endl;
		accelR = false;
	}
	if (INPUTS->getKey(KeyCode::KEY_W))
	{
		std::cout << "A" << std::endl;
		accelU = true;
		//this->setPosition(this->getPosition() + Vec2(0,10));
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_W))
	{
		std::cout << "A released" << std::endl;
		accelU = false;
	}
	if (INPUTS->getKey(KeyCode::KEY_S))
	{
		std::cout << "A" << std::endl;
		accelD = true;
		//this->setPosition(this->getPosition() + Vec2(0,-10));

	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_S))
	{
		std::cout << "A released" << std::endl;
		accelD = false;
	}
}

void Player::handleMovement(float dt)
{
	previousPos = this->getPosition();
	velocity = Vec2(0, 0);
	if (!collL)
	{
		if (accelL)
		{
			velocity += Vec2(-1, 0);
			//this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(-200, 0));
		}
	}
	//else
	//{
	//	this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(200, 0));
	//}
	if (!collR) 
	{
		if (accelR)
		{
			velocity += Vec2(1, 0);
			//this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(200, 0));
		}
	}
	//else
	//{
	//	this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(-200, 0));
	//}
	if (!collU) 
	{
		if (accelU)
		{
			velocity += Vec2(0, 1);
			//this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(0,200));
		}
	}
	//else
	//{
	//	this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(0, -200));

	//}
	if (!collD)
	{
		if (accelD)
		{
			velocity += Vec2(0, -1);
			//this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(0, -200));

		}
	}
	
	this->getPhysicsBody()->setVelocity(velocity * velocityLimit);
	//Vec2 position = this->getPosition() + velocity.getNormalized() * 300 * dt;
	//this->setPosition(position);
	//else
	//{
	//	this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + Vec2(0, 200));
	//}
}

void Player::update(float dt)
{
	handleInput();
	handleMovement(dt);
	INPUTS->clearForNextFrame();
}
