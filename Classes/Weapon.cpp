#include "Weapon.h"
#include "Projectile.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include "GameCamera.h"
#include "Definitions.h"
#include "audio/include/AudioEngine.h"
#include <iomanip>
using namespace cocos2d::experimental;

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
		totalAmmo = 999;
		isPistol_ = true;
		this->setTag(PISTOLPICKUPTAG);
		initPistolAnimations();

	}
	if (fileName == "handOnGun.png")
	{
		ammo = 7;
		maxCartridgeAmmo = 7;
		totalAmmo = 999;
		isPistol_ = true;
		this->setTag(PISTOLPICKUPTAG);
		this->setName(PISTOLNAME);
		initPistolAnimations();
	}
	if (fileName == "AK47.png")
	{
		ammo = 30;
		maxCartridgeAmmo = 30;
		totalAmmo = 30;
		isPistol_ = false;
		this->setTag(AKPICKUPTAG);
		this->setName(AKNAME);
		initAKAnimation();
	}
	if (fileName == "hand.png")
	{
		initZombieAnimation();
		auto body = PhysicsBody::createBox(Size(32,32));
		body->setRotationEnable(false);
		body->setVelocityLimit(0);
		body->setDynamic(false);
		body->setTag(14);
		body->setContactTestBitmask(0xFFFFFFFF);
		this->setPhysicsBody(body);
	}

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Weapon::onContactBegin, this);

	muzzle = ParticleSystem::create("Particles/muzzleFlash.plist");
	this->scheduleUpdate();

	return true;
}

void Weapon::initAnimations()
{
}

void Weapon::initPistolAnimations()
{
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
	for (int i = 0; i < 22; i++)
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

void Weapon::initZombieAnimation()
{
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/zombie-attack.plist");

	Vector<SpriteFrame*> fireframes;
	for (int i = 0; i < 5; i++)
	{
		stringstream ss;
		ss << "frame" << i + 1 << ".png";
		fireframes.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	auto animation = Animation::createWithSpriteFrames(fireframes, 0.03f);
	animFire = Animate::create(animation);
	animFire->retain();
	animFire->setTag(FIRETAG);
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
				projectile = Projectile::create("45CalBullet.png");
			}
			else
			{
				projectile = Projectile::create("45CalBullet.png");
			}
			projectile->getPhysicsBody()->setTag(ENEMYBULLET_TAG);
			projectile->getPhysicsBody()->setGroup(-1);
			projectile->setShot(position_, dir_);
			this->getParent()->getParent()->addChild(projectile);
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

				//Get Parent(Player), get its parent(Layer), then add the projectile to layer
				this->getParent()->getParent()->addChild(projectile);
				fired = true;
				ammo--;
				EventCustom event2("ammo");
				if (this->getTag() == PISTOLPICKUPTAG)
				{
					string tmp = to_string(ammo) + " / Infinite";
					event2.setUserData((void*)tmp.c_str());
					_eventDispatcher->dispatchEvent(&event2);
				}
				else if(this->getTag() == AKPICKUPTAG)
				{
					string tmp = to_string(ammo) + " / " + to_string(totalAmmo);
					event2.setUserData((void*)tmp.c_str());
					_eventDispatcher->dispatchEvent(&event2);
				}
				this->runAction(animFire);

				auto gunMuzle = ParticleSystemQuad::create("Particles/muzzleFlash.plist");
				//auto gunMuzle = ParticleExplosion::create();
				gunMuzle->setDuration(0.2f);
				gunMuzle->setPosition(position_);
				gunMuzle->setAutoRemoveOnFinish(true);
				this->getParent()->getParent()->addChild(gunMuzle);

				AudioEngine::play2d("Sounds/Gun/AK47fire.mp3", false);
				
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
			else
			{
				EventCustom event("reload");
				_eventDispatcher->dispatchEvent(&event);
			}
		}
		else if (reloadState)
		{
			if (totalAmmo <= 0)
			{

			}
			else if (ammo != maxCartridgeAmmo)
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

				reloading = false;
				this->setSpriteFrame(idleFrame);
				EventCustom event2("ammo");
				if (isPistol_)
				{
					ammo = maxCartridgeAmmo;
					string tmp = to_string(ammo) + " / Infinite";
					event2.setUserData((void*)tmp.c_str());
					_eventDispatcher->dispatchEvent(&event2);
				}
				else
				{

					int leftover = (maxCartridgeAmmo - ammo);;
					ammo += leftover;
					totalAmmo -= leftover;
					if (totalAmmo < 0)
					{
						ammo = ammo + totalAmmo;
						totalAmmo = 0;
					}
					string tmp = to_string(ammo) + " / " + to_string(totalAmmo);
					event2.setUserData((void*)tmp.c_str());
					_eventDispatcher->dispatchEvent(&event2);
				}
			}
		}
	}
	//INPUTS->clearForNextFrame();
}

void Weapon::displayEquip()
{
	EventCustom event("weapon");

	event.setUserData((void*)this->getName().c_str());

	_eventDispatcher->dispatchEvent(&event);

	EventCustom event2("ammo");
	if (isPistol_)
	{
		string tmp = to_string(ammo) + " / Infinite";
		event2.setUserData((void*)tmp.c_str());
		_eventDispatcher->dispatchEvent(&event2);
	}
	else
	{
		string tmp = to_string(ammo) + " / " + to_string(totalAmmo);
		event2.setUserData((void*)tmp.c_str());
		_eventDispatcher->dispatchEvent(&event2);
	}

}

void Weapon::addAmmo()
{
	totalAmmo += 10;
}

bool Weapon::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
	
	}
	return true;

}

bool Weapon::onContactPost(PhysicsContact &)
{
	return false;
}

bool Weapon::onContactSeparate(PhysicsContact &)
{
	return false;
}
