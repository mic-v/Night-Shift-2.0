#include "Entity.h"

bool Entity::onContactBegin(cocos2d::PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if (nodeA->getPhysicsBody()->getTag() != this->getPhysicsBody()->getTag() && nodeB->getPhysicsBody()->getTag() != this->getPhysicsBody()->getTag())
		{
			return true;
		}
	}
	return false;
}

bool Entity::onContactPost(cocos2d::PhysicsContact &)
{
	return true;
}

bool Entity::onContactSeparate(cocos2d::PhysicsContact &)
{
	return true;
}

std::string Entity::getSpriteFile()
{
	return spriteFile;
}
