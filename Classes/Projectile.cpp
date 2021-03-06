#include "Projectile.h"
#include "Definitions.h"
Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

Projectile * Projectile::create(const std::string& fileName)
{
	Projectile* projectile = new Projectile();

	if (projectile->initWithFile(fileName))
	{
		projectile->autorelease();
		projectile->init(fileName);
		return projectile;
	}
	CC_SAFE_DELETE(projectile);
	return nullptr;
}


bool Projectile::init(const std::string & fileName)
{
	if (!this->initWithFile(fileName))
	{
		return false;
	}
	PhysicsBody* body = PhysicsBody::createBox(this->getContentSize());
	body->setDynamic(true);
	body->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(body);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Projectile::onContactBegin, this);
	contactListener->onContactPreSolve = CC_CALLBACK_1(Projectile::onContactPost, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();
	return true;
}

void Projectile::update(float dt)
{
	auto p = this->getPosition() - Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f);
	p = CC_POINT_POINTS_TO_PIXELS(p);
	this->setLocalZOrder(-((p.y + 64) / 64));
}

void Projectile::setShot(Vec2 position, Vec2 direction)
{
	this->setPosition(position);
	float rot_ = Vec2::angle(Vec2(0, 1), direction.getNormalized()) * (180.f / M_PI) - 90.f;
	if (direction.x < 0)
	{
		rot_ = 360 - rot_;
		this->setScaleX(-1.f);
	}
	else
	{
		this->setScaleX(1.f);
	}
	this->setRotation(rot_);
	this->getPhysicsBody()->setVelocity(1200 * direction.getNormalized());
	this->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(), NULL));
}


bool Projectile::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		//WALL CHECK
		std::cout << nodeA->getPhysicsBody()->getTag() << " " << nodeB->getPhysicsBody()->getTag() << std::endl;
		if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG)
		{
			nodeA->runAction(RemoveSelf::create());
		}
		else if (nodeB->getPhysicsBody()->getTag() == BULLET_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
			nodeB->runAction(RemoveSelf::create());
		}
		if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == WALL_TAG)
		{
			nodeA->runAction(RemoveSelf::create());
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeA->getPhysicsBody()->getTag() == WALL_TAG)
		{
			nodeB->runAction(RemoveSelf::create());
		}

		//PLAYER CHECK
		if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == PLAYER_TAG)
		{
			nodeB->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			nodeA->runAction(RemoveSelf::create());
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeA->getPhysicsBody()->getTag() == PLAYER_TAG)
		{
			nodeA->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			nodeB->runAction(RemoveSelf::create());
		}
		//ENEMY CHECK
		if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == ENEMY_TAG)
		{
			nodeB->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			nodeA->runAction(RemoveSelf::create());
		}
		else if (nodeB->getPhysicsBody()->getTag() == BULLET_TAG && nodeA->getPhysicsBody()->getTag() == ENEMY_TAG)
		{
			nodeA->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			nodeB->runAction(RemoveSelf::create());
		}

		if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == ENEMY_TAG)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeA->getPhysicsBody()->getTag() == ENEMY_TAG)
		{
			return false;
		}
		if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == 7)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == BULLET_TAG && nodeA->getPhysicsBody()->getTag() == 7)
		{
			return false;
		}
		if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == HEART_TAG)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeA->getPhysicsBody()->getTag() == HEART_TAG)
		{
			return false;
		}
		if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == 14)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == BULLET_TAG && nodeA->getPhysicsBody()->getTag() == 14)
		{
			return false;
		}

	}
	return true;
}

bool Projectile::onContactPost(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == 7)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == BULLET_TAG && nodeA->getPhysicsBody()->getTag() == 7)
		{
			return false;
		}
		if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == 7)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeA->getPhysicsBody()->getTag() == 7)
		{
			return false;
		}

		if (nodeA->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeB->getPhysicsBody()->getTag() == HEART_TAG)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == ENEMYBULLET_TAG && nodeA->getPhysicsBody()->getTag() == HEART_TAG)
		{
			return false;
		}
		if (nodeA->getPhysicsBody()->getTag() == BULLET_TAG && nodeB->getPhysicsBody()->getTag() == 14)
		{
			return false;
		}
		else if (nodeB->getPhysicsBody()->getTag() == BULLET_TAG && nodeA->getPhysicsBody()->getTag() == 14)
		{
			return false;
		}
	}
	return true;
}

bool Projectile::onContactSeparate(PhysicsContact & contact)
{
	return true;
}
