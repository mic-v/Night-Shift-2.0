#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "Entity.h"
#include <string>
#include <iostream>
class Projectile : public Entity
{
public:
	Projectile();
	virtual ~Projectile();
	static Projectile* create(const std::string&);
	bool init(const std::string&);
	
	void update(float);

	void setShot(Vec2, Vec2);

	bool onContactBegin(PhysicsContact&);
	bool onContactPost(PhysicsContact&);
	bool onContactSeparate(PhysicsContact&);
private:

};
#endif // !_PROJECTILE_H_

