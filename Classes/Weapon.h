#ifndef _WEAPON_H_
#define _WEAPON_H_

#define FIRETAG 1
#define RELOADINGTAG 2

#include "Entity.h"
#include <string>

class Weapon : public Entity
{
public:
	Weapon();
	~Weapon();
	static Weapon* create(const std::string&);
	virtual bool init(const std::string & fileName);
	virtual void initAnimations();
	void initPistolAnimations();
	void initAKAnimation();
	void initZombieAnimation();
	void setIfPlayer(bool);
	bool isPistol();
	void enemyFire(const Vec2 &, const Vec2 &);

	void update(float);
	void displayEquip();

	void addAmmo();

	virtual bool onContactBegin(PhysicsContact&);
	virtual bool onContactPost(PhysicsContact&);
	virtual bool onContactSeparate(PhysicsContact&);

private:
	bool player;
	bool isPistol_;
	int ammo;
	int maxCartridgeAmmo;
	int totalAmmo;
	float shotTimer;
	float reloadTimer;
	bool fired;
	bool fireState;
	bool reloadState;
	bool reloading;

	//Animations
	Animate* animFire;
	Animate* animFireL;

	Animate* reload;
	Animate* reloadLeft;

	SpriteFrame* idleFrame;
	SpriteFrame* idleFrameLeft;
	ParticleSystem* muzzle;
};


#endif // !_WEAPON_H_