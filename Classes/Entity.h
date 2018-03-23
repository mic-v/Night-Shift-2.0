#ifndef _ENTITY_H
#define _ENTITY_H

#include "cocos2d.h"
#include <string>
using namespace cocos2d;
class Entity : public cocos2d::Sprite
{
public:
	Entity() : contactListener(EventListenerPhysicsContact::create()){}
	virtual ~Entity(){}
	virtual bool init(const std::string & fileName) = 0;
	virtual bool onContactBegin(PhysicsContact &);
	virtual bool onContactPost(PhysicsContact &) = 0;
	virtual bool onContactSeparate(PhysicsContact &) = 0;
	std::string getSpriteFile();
protected:
	cocos2d::EventListenerPhysicsContact* contactListener;
	std::string spriteFile;
};

#endif // !_ENTITY_H

