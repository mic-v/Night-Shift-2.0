#ifndef _ITEM_H_
#define _ITEM_H_


#include "Entity.h"
#include <string>

class Item : public Entity
{
public:
	Item();
	~Item();
	static Item* create(const std::string&);
	virtual bool init(const std::string & fileName);
	virtual void initAnimations();
	virtual bool onContactBegin(PhysicsContact&);
	virtual bool onContactPost(PhysicsContact&);
	virtual bool onContactSeparate(PhysicsContact&);

private:
	std::string spriteFile;
};
#endif // !_ITEM_H_
