#include "Projectile.h"

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
	body->setCategoryBitmask(0x02);
	body->setCollisionBitmask(0x01);
	this->setPhysicsBody(body);

	auto contactListene = EventListenerPhysicsContact::create();
	contactListene->onContactBegin = CC_CALLBACK_1(Projectile::onContactBegin, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListene, this);

	return true;
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
		if (nodeA->getPhysicsBody()->getTag() == 4 && nodeB->getPhysicsBody()->getTag() == 2)
		{
			nodeA->runAction(RemoveSelf::create());
			return true;
		}
		else if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 4)
		{
			nodeB->runAction(RemoveSelf::create());
			return true;
		}

		if (nodeA->getPhysicsBody()->getTag() == 4 && nodeB->getPhysicsBody()->getTag() == 11)
		{
			nodeA->runAction(RemoveSelf::create());
			nodeB->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			return true;
		}
		else if (nodeA->getPhysicsBody()->getTag() == 11 && nodeB->getPhysicsBody()->getTag() == 4)
		{
			nodeB->runAction(RemoveSelf::create());
			nodeA->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			return true;
		}
		if (nodeA->getPhysicsBody()->getTag() == 5 && nodeB->getPhysicsBody()->getTag() == 10)
		{
			nodeA->runAction(RemoveSelf::create());
			nodeB->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));

			return true;
		}
		else if (nodeA->getPhysicsBody()->getTag() == 10 && nodeB->getPhysicsBody()->getTag() == 5)
		{
			nodeB->runAction(RemoveSelf::create());
			nodeA->runAction(Sequence::create(TintTo::create(0.1f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
			return true;
		}
	}
	return false;
}

bool Projectile::onContactPost(PhysicsContact &)
{
	return false;
}

bool Projectile::onContactSeparate(PhysicsContact &)
{
	return false;
}
