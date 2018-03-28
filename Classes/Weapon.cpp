#include "Weapon.h"
#include "Projectile.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include "GameCamera.h"
#include "Definitions.h"
#include <iomanip>

Weapon::Weapon()
{
	reloadTimer = 0;
	shotTimer = 0.f;
	reloadState = false;
	fireState = false;
	reloading = false;
	fired = false;
}

Weapon::~Weapon()
{
}

Weapon * Weapon::create(const std::string& fileName)
{
	Weapon* weapon = new Weapon();

	if (weapon->initWithFile(fileName))
	{
		weapon->autorelease();
		weapon->init(fileName);
		return weapon;
	}
	CC_SAFE_DELETE(weapon);
	return nullptr;
}

bool Weapon::init(const std::string & fileName)
{
	if (!this->initWithFile(fileName))
	{
		return false;
	}

	if (fileName == "silencedGun.png")
	{
		ammo = 7;
		maxCartridgeAmmo = 7;
		totalAmmo = 0;
		isPistol_ = true;
		this->setTag(PISTOLPICKUPTAG);
		initPistolAnimations();

	}
	if (fileName == "handOnGun.png")
	{
		ammo = 7;
		maxCartridgeAmmo = 7;
		totalAmmo = 0;
		isPistol_ = true;
		this->setTag(PISTOLPICKUPTAG);
		this->setName(PISTOLNAME);
		initPistolAnimations();
	}
	if (fileName == "AK47.png")
	{
		ammo = 30;
		maxCartridgeAmmo = 30;
		totalAmmo = 0;
		isPistol_ = false;
		this->setTag(AKPICKUPTAG);
		this->setName(AKNAME);
		initAKAnimation();
	}

	this->scheduleUpdate();

	return true;
}

void Weapon::initAnimations()
{
}

void Weapon::initPistolAnimations()
{
	std::cout << "initialize" << std::endl;
	FileUtils::getInstance()->addSearchResolutionsOrder("HDR");
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/pistol_fire_right.plist");

	Vector<SpriteFrame*> fireframes;
	for (int i = 0; i < 4; i++)
	{
			stringstream ss;
			ss << "fram-r-00" << i + 1 << ".png";
			fireframes.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	auto animation = Animation::createWithSpriteFrames(fireframes, 0.15f);
	animFire = Animate::create(animation);
	animFire->retain();
	animFire->setTag(FIRETAG);

	stringstream ss;
	ss << "handOnGun.png";
	idleFrame = spritecache->getSpriteFrameByName(ss.str());
	idleFrame->retain();
	spritecache->destroyInstance();

	spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/pistolreload.plist");

	Vector<SpriteFrame*> reloadR;
	for (int i = 0; i < 23; i++)
	{
		stringstream ss1;
		ss1 << "frame-r-" << setfill('0') << setw(3) << i + 1 << ".png";
		reloadR.pushBack(spritecache->getSpriteFrameByName(ss1.str()));
	}
	animation = Animation::createWithSpriteFrames(reloadR, 0.09f);
	reload = Animate::create(animation);
	reload->setTag(RELOADINGTAG);
	reload->retain();
	spritecache->destroyInstance();
	

}

void Weapon::initAKAnimation()
{
	std::cout << "initialize" << std::endl;
	FileUtils::getInstance()->addSearchResolutionsOrder("HDR");
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/AK47_firing_right.plist");

	Vector<SpriteFrame*> fireframes;
	for (int i = 0; i < 4; i++)
	{
		stringstream ss;
		ss << "frame-r-00" << i + 1 << ".png";
		fireframes.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	auto animation = Animation::createWithSpriteFrames(fireframes, 0.03f);
	animFire = Animate::create(animation);
	animFire->retain();
	animFire->setTag(FIRETAG);

	stringstream ss;
	ss << "AK47.png";
	idleFrame = spritecache->getSpriteFrameByName("frame-r-001.png");
	idleFrame->retain();
	spritecache->destroyInstance();

	spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/AK47_firing.plist");

	Vector<SpriteFrame*> reloadR;
	for (int i = 0; i < 27; i++)
	{
		stringstream ss1;
		ss1 << "frame-r-" << setfill('0') << setw(3) << i + 1 << ".png";
		reloadR.pushBack(spritecache->getSpriteFrameByName(ss1.str()));
	}
	animation = Animation::createWithSpriteFrames(reloadR, 0.09f);
	reload = Animate::create(animation);
	reload->setTag(RELOADINGTAG);
	reload->retain();
	spritecache->destroyInstance();
}


void Weapon::setIfPlayer(bool set)
{
	player = set;
}

bool Weapon::isPistol()
{
	return isPistol_;
}

void Weapon::enemyFire(const Vec2 & pos_, const Vec2 & dir)
{
	if (!reloading)
	{
		fireState = true;
	}
	if (fireState)
	{
		if (ammo >= 1 && !fired)
		{
			Vec2 position_ = pos_;
			Vec2 dir_ = dir;
			Projectile* projectile;
			if (isPistol_)
			{
				projectile = Projectile::create("0305Bullet.png");
				//projectile->getPhysicsBody()->setCategoryBitmask(ENEMYBULLET_CATEGORY);
				//projectile->getPhysicsBody()->setCollisionBitmask(MASK_ENEMYBULLET);
				projectile->getPhysicsBody()->setTag(ENEMYBULLET_TAG);
				projectile->getPhysicsBody()->setGroup(-1);
				if (isPistol_)
					projectile->setTexture("0305Bullet.png");
				projectile->setShot(position_, dir_);
				//std::cout << projectile->getPosition().x << " ASD " << projectile->getPosition().y << std::endl;
				this->getParent()->getParent()->addChild(projectile);
			}
			fired = true;
			ammo--;
			this->runAction(animFire);
		}
		else if (ammo == 0)
		{
			reloading = true;
			fireState = false;
			this->stopAllActions();
			this->runAction(reload);
		}
		else if (fired)
		{
			if (this->getActionByTag(FIRETAG) == nullptr)
			{
				fired = false;
				this->stopAllActions();
				this->setSpriteFrame(idleFrame);
			}
		}
	}
	if (reloading)
	{
		if (this->getActionByTag(RELOADINGTAG) == nullptr)
		{
			ammo = maxCartridgeAmmo;
			reloading = false;
			fired = false;
			this->setSpriteFrame(idleFrame);
		}
	}
}

void Weapon::update(float dt)
{
	if (player)
	{
		//	bool pressed = INPUTS->getMouseButton(MouseButton::BUTTON_LEFT);
		//	bool pressedR = INPUTS->getKey(KeyCode::KEY_R);
		if (!reloading)
		{
			fireState = INPUTS->getMouseButton(MouseButton::BUTTON_LEFT);
			reloadState = INPUTS->getKey(KeyCode::KEY_R);
		}
		if (fireState)
		{
			if (ammo >= 1 && !fired)
			{
				Vec2 position_ = CAMERA->getCameraTarget()->convertToWorldSpace(this->getPosition()) + CAMERA->getOrigin() - (CAMERA->getOrigin() + Vec2(DISPLAY->getWindowSizeAsVec2().x * 0.5f, DISPLAY->getWindowSizeAsVec2().y * 0.5f) - CAMERA->getScreenMouse()) / 4.f;
				Vec2 direction = CAMERA->getScreenMouse() - (CAMERA->getOrigin() + Vec2(DISPLAY->getWindowSizeAsVec2().x * 0.5f, DISPLAY->getWindowSizeAsVec2().y * 0.5f));
				position_ = (CAMERA->getOrigin() + Vec2(DISPLAY->getWindowSizeAsVec2().x * 0.5f, DISPLAY->getWindowSizeAsVec2().y * 0.5f)) + position_.getNormalized() + direction.getNormalized() * 100.f;
				Projectile* projectile;
				if (!isPistol_)
				{
					projectile = Projectile::create("45CalBullet.png");
				}
				else
				{
					projectile = Projectile::create("0305Bullet.png");
				}
				projectile->getPhysicsBody()->setCategoryBitmask(PLAYERBULLET_CATEGORY);
				projectile->getPhysicsBody()->setCollisionBitmask(MASK_PLAYERBULLET);
				projectile->getPhysicsBody()->setContactTestBitmask(MASK_PLAYERBULLET);
				projectile->getPhysicsBody()->setTag(BULLET_TAG);


				float rot_ = Vec2::angle(Vec2(0, 1), direction.getNormalized()) * (180.f / M_PI) - 90.f;


				projectile->setRotation(rot_);
				projectile->setShot(position_, direction);

				//Get Parent(Player), get its parent(Layer), then add the projectile to layercccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
				this->getParent()->getParent()->addChild(projectile);
				fired = true;
				ammo--;
				EventCustom event2("ammo");
				if (isPistol_)
				{
					string tmp = to_string(ammo) + " / Infinite";
					event2.setUserData((void*)tmp.c_str());
					_eventDispatcher->dispatchEvent(&event2);
				}
				this->runAction(animFire);
			}
			else if (fired)
			{
				if (this->getActionByTag(FIRETAG) == nullptr)
				{
					fired = false;
					this->stopAllActions();
					this->setSpriteFrame(idleFrame);
				}
				//shotTimer += dt;
				//if (shotTimer > 0.5f)
				//{
				//	this->stopAllActions();
				//	this->setSpriteFrame(idleFrame);
				//	shotTimer = 0;
				//	fired = false;
				//}
			}
		}
		else if (reloadState)
		{
			if (ammo != maxCartridgeAmmo)
			{
				reloading = true;
				this->stopAllActions();
				this->runAction(reload);
				reloadState = false;
				fireState = false;
			}
		}
		if (reloading)
		{
			if (this->getActionByTag(RELOADINGTAG) == nullptr)
			{
				ammo = maxCartridgeAmmo;
				reloading = false;
				this->setSpriteFrame(idleFrame);
				EventCustom event2("ammo");
				if (isPistol_)
				{
					string tmp = to_string(ammo) + " / Infinite";
					event2.setUserData((void*)tmp.c_str());
					_eventDispatcher->dispatchEvent(&event2);
				}
			}
		}
	}
	//if (player)
	//{
	//	bool pressed = INPUTS->getMouseButton(MouseButton::BUTTON_LEFT);
	//	bool pressedR = INPUTS->getKey(KeyCode::KEY_R);
	//	if (pressed && shotTimer == 0 && !pressedR)
	//	{
	//		if (ammo >= 1)
	//		{
	//			this->runAction(animFire);
	//			Vec2 position_ = CAMERA->getCameraTarget()->convertToWorldSpace(this->getPosition()) + CAMERA->getOrigin() - (CAMERA->getOrigin() + Vec2(DISPLAY->getWindowSizeAsVec2().x * 0.5f, DISPLAY->getWindowSizeAsVec2().y * 0.5f) - CAMERA->getScreenMouse()) / 4.f;
	//			Vec2 direction = CAMERA->getScreenMouse() - (CAMERA->getOrigin() + Vec2(DISPLAY->getWindowSizeAsVec2().x * 0.5f, DISPLAY->getWindowSizeAsVec2().y * 0.5f));
	//			position_ = (CAMERA->getOrigin() + Vec2(DISPLAY->getWindowSizeAsVec2().x * 0.5f, DISPLAY->getWindowSizeAsVec2().y * 0.5f)) + position_.getNormalized() + direction.getNormalized() * 100.f;
	//			Projectile* projectile;
	//			if (isPistol_)
	//			{
	//				//projectile = Projectile::create("0305Bullet.png");
	//				//if (player)
	//				//	projectile->getPhysicsBody()->setTag(4);
	//				//projectile->setPosition(position_);
	//				//std::cout << position_.x << std::endl;
	//				//this->getParent()->getParent()->addChild(projectile);
	//				projectile = Projectile::create("0305Bullet.png");
	//				projectile->getPhysicsBody()->setTag(4);
	//				if (isPistol_)
	//					projectile->setTexture("0305Bullet.png");

	//				float rot_ = Vec2::angle(Vec2(0, 1), direction.getNormalized()) * (180.f / M_PI) - 90.f;


	//				projectile->setRotation(rot_);
	//				projectile->setShot(position_, direction);
	//				//projectile->setPosition(position_);
	//				this->getParent()->getParent()->addChild(projectile);
	//			}
	//			fired = true;
	//			ammo--;
	//		}
	//	}
	//	if (fired)
	//	{
	//		shotTimer += dt;
	//	}
	//	if (shotTimer > 0.5f)
	//	{
	//		this->stopAllActions();
	//		this->setSpriteFrame(idleFrame);
	//		shotTimer = 0;
	//		fired = false;
	//		//fired = false;
	//	}
	//}
	//else
	//{
	//	if (ammo == 0)
	//	{
	//		reloadTimer += dt;
	//		//std::cout << reloadTimer << std::endl;
	//		if (reloadTimer > 2.f)
	//		{
	//			if(isPistol_)
	//			{
	//				ammo = 7;
	//			}
	//			reloadTimer = 0.f;
	//		}
	//	}
	//	if (fired)
	//	{
	//		shotTimer += dt;
	//	}
	//	if (shotTimer > 0.5f)
	//	{
	//		this->stopAllActions();
	//		this->setSpriteFrame(idleFrame);
	//		shotTimer = 0;
	//		fired = false;
	//	}
	//}
	INPUTS->clearForNextFrame();
}

void Weapon::displayEquip()
{
	EventCustom event("weapon");

	event.setUserData((void*)this->getName().c_str());

	_eventDispatcher->dispatchEvent(&event);

	EventCustom event2("ammo");
	//if (isPistol_)
	{
		string tmp = to_string(ammo) + " / Infinite";
		event2.setUserData((void*)tmp.c_str());
		_eventDispatcher->dispatchEvent(&event2);
	}

}

bool Weapon::onContactBegin(PhysicsContact &)
{
	return false;
}

bool Weapon::onContactPost(PhysicsContact &)
{
	return false;
}

bool Weapon::onContactSeparate(PhysicsContact &)
{
	return false;
}
