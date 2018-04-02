#include "Character.h"
#include "InputHandler.h"
#include "Definitions.h"
Character::Character()
{
	speed = 0;
	faceLeft_ = false;
	faceRight_ = false;
	faceUp_ = false;
	faceDown_ = false;
	animationCounter = 1;
	currentACounter = 1;
	animationTimer = 0.f;
	orientation_ = "r";
	cOrientation_ = "r";
	hasAk = false;
	hasM16 = false;
}

Character::~Character()
{
}

Character * Character::create(const std::string & fileName)
{
	Character* character = new Character();
	
	if (character->initWithFile(fileName))
	{
		character->autorelease();
		character->init(fileName);
		return character;
	}
	CC_SAFE_DELETE(character);
	return nullptr;
}

bool Character::init(const std::string & fileName)
{
	if (!this->initWithFile(fileName))
	{
		return false;
	}

	//this->setTexture("Obstacle.png");

	/* PHYSICS BODY */
	/*body = PhysicsBody::createBox(this->getContentSize());*/
	//Character set tag to 0
	//body->setTag(0);
	//this->setPhysicsBody(body);

	velocityLimit = 300.f;

	contactListener->onContactBegin = CC_CALLBACK_1(Character::onContactBegin, this);
	contactListener->onContactPreSolve = CC_CALLBACK_1(Character::onContactPost, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(Character::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

void Character::initAnimations()
{
}

bool Character::onContactBegin(PhysicsContact & contact)
{
	return false;
}

bool Character::onContactPost(PhysicsContact &)
{
	return false;
}

bool Character::onContactSeparate(PhysicsContact &)
{
	return false;
}


void Character::handleMovement(float dt)
{
}

void Character::updateAnimation(float)
{
}


float Character::getHealth()
{
	return health;
}

