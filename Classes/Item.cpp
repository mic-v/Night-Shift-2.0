#include "Item.h"
#include "Definitions.h"
Item::Item()
{
}

Item::~Item()
{
}

Item * Item::create(const std::string& fileName)
{
	Item* item = new Item();

	if (item->initWithFile(fileName))
	{
		item->autorelease();
		item->init(fileName);
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool Item::init(const std::string & fileName)
{
	if (!this->initWithFile(fileName))
	{
		return false;
	}

	spriteFile = fileName;
	//this->setName("handOnGun.png");
	PhysicsBody* body = PhysicsBody::createBox(this->getContentSize());
	//body->setDynamic(true);
	body->setVelocityLimit(0);
	body->setRotationEnable(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setCategoryBitmask(ITEM_CATEGORY);
	body->setCollisionBitmask(MASK_ITEM);
	//body->setCategoryBitmask(0x01);
	//body->setCollisionBitmask(0x02);

	body->setTag(7);

	this->setPhysicsBody(body);

	return true;
}

void Item::initAnimations()
{
}

bool Item::onContactBegin(PhysicsContact &)
{
	return false;
}

bool Item::onContactPost(PhysicsContact &)
{
	return false;
}

bool Item::onContactSeparate(PhysicsContact &)
{
	return false;
}
