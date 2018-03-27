#include "CPlayer.h"
#include "DisplayHandler.h"
#include "Definitions.h"
#include "InputHandler.h"
#include "GameCamera.h"
CPlayer::CPlayer()
{

	velocityLimit = 200;
	cWeapNum = 0;
	isMoving = false;
	isSprinting = false;
	//hspeed = 100.f;
	//vspeed = 100.f;
}

CPlayer::~CPlayer()
{
}

CPlayer * CPlayer::create(const std::string & fileName)
{
	CPlayer* player = new CPlayer();
	if (player->initWithFile(fileName))
	{
		player->autorelease();
		player->init(fileName);
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool CPlayer::init(const std::string & fileName)
{
	if (!this->initWithFile(fileName))
	{
		return false;
	}

	/* PHYSICS BODY */
	PhysicsBody* cbody = PhysicsBody::createBox(Size(61,159), PhysicsMaterial(1.f,1.f,1.f));
	cbody->setDynamic(true);
	cbody->setRotationEnable(false);
	cbody->setVelocityLimit(130);
	cbody->setCategoryBitmask(PLAYER_CATEGORY);
	cbody->setCollisionBitmask(MASK_PLAYER);
	cbody->setContactTestBitmask(0xFFFFFFFF);
	cbody->setGroup(-2);
	cbody->setTag(PLAYER_TAG);
	cbody->setVelocity(Vec2(0, 0));
	this->setTag(PLAYER_TAG);
	this->setPhysicsBody(cbody);

	hspeed = 70;
	vspeed = 70;

	initAnimations();

	auto contactListene = EventListenerPhysicsContact::create();
	contactListene->onContactBegin = CC_CALLBACK_1(CPlayer::onContactBegin, this);
	contactListene->onContactPreSolve = CC_CALLBACK_1(CPlayer::onContactPost, this);
	contactListene->onContactSeparate = CC_CALLBACK_1(CPlayer::onContactSeparate, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListene, this);
	
	this->scheduleUpdate();

	return true;
}



void CPlayer::initAnimations()
{

	FileUtils::getInstance()->addSearchResolutionsOrder("HDR");
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/player_.plist");

	Vector<SpriteFrame*> frames;
	for (int i = 0; i < 8; i++)
	{
		stringstream ss;
		ss << "frame-l-00" << i + 1 << ".png";
		frames.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	auto animation = Animation::createWithSpriteFrames(frames, 0.09f);
	animL = Animate::create(animation);
	animL->retain();

	Vector<SpriteFrame*> framesRight;
	for (int i = 0; i < 8; i++)
	{
		stringstream ss;
		ss << "frame-r-00" << i + 1 << ".png";
		framesRight.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(framesRight, 0.09f);
	animR = Animate::create(animation);
	animR->retain();

	Vector<SpriteFrame*> framesRunRight;
	for (int i = 0; i < 4; i++)
	{
		stringstream ss;
		ss << "rframe-r-00" << i + 1 << ".png";
		framesRunRight.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(framesRunRight, 0.09f);
	runAnimR = Animate::create(animation);
	runAnimR->setTag(69);
	runAnimR->retain();

	Vector<SpriteFrame*> framesRunLeft;
	for (int i = 0; i < 4; i++)
	{
		stringstream ss;
		ss << "rframe-l-00" << i + 1 << ".png";
		framesRunLeft.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(framesRunLeft, 0.09f);
	runAnimL = Animate::create(animation);
	runAnimL->retain();

	Vector<SpriteFrame*> idleright;
	for (int i = 0; i < 1; i++)
	{
		stringstream ss;
		ss << "idle.png";
		idleright.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(idleright, 0.09f);
	idleR = Animate::create(animation);
	idleR->retain();
	//spr->runAction(RepeatForever::create(idleR));



	Vector<SpriteFrame*> idleleft;
	for (int i = 0; i < 1; i++)
	{
		stringstream ss;
		ss << "idleleft.png";
		idleleft.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	stringstream ss2;
	ss2 << "frame-r-006.png";
	idleFrameL = spritecache->getSpriteFrameByName(ss2.str());
	//idleFrameL = spritecache->getSpriteFrameByName("idleLeft.png");
	idleFrameL->retain();
	animation = Animation::createWithSpriteFrames(idleleft, 0.09f);
	idleL = Animate::create(animation);
	idleL->retain();
	//this->runAction(RepeatForever::create(runAnimL));

	spritecache->destroyInstance();

}

void CPlayer::handleInput()
{
}

void CPlayer::handleMovement(float dt)
{
	isSprinting = false;
	if (INPUTS->getKey(KeyCode::KEY_A) || INPUTS->getKey(KeyCode::KEY_D) || INPUTS->getKey(KeyCode::KEY_W) || INPUTS->getKey(KeyCode::KEY_S))
	{
		isMoving = true;
		if (INPUTS->getKey(KeyCode::KEY_A) && INPUTS->getKey(KeyCode::KEY_D))
		{
			isMoving = false;
		}
		if (INPUTS->getKey(KeyCode::KEY_W) && INPUTS->getKey(KeyCode::KEY_S))
		{
			isMoving = false;
		}
		if (INPUTS->getKey(KeyCode::KEY_SHIFT))
		{
			isSprinting = true;
		}
	}
	else
	{
		isMoving = false;
	}

	if (isSprinting)
	{
		this->getPhysicsBody()->setVelocityLimit(200);
	}
	else
	{
		this->getPhysicsBody()->setVelocityLimit(130);
	}
	velocity = Vec2(0, 0);
	if (INPUTS->getKey(KeyCode::KEY_A))
	{
		velocity += Vec2(-1, 0) * hspeed;
		velNorm += Vec2(-1, 0);
	}
	if (INPUTS->getKey(KeyCode::KEY_D))
	{
		velocity += Vec2(1, 0) * hspeed;
		velNorm += Vec2(1, 0);
	}
	if (INPUTS->getKey(KeyCode::KEY_W))
	{
		velocity += Vec2(0, 1) * vspeed;
		velNorm += Vec2(0, 1);
	}
	if (INPUTS->getKey(KeyCode::KEY_S))
	{
		velocity += Vec2(0, -1) * vspeed;
		velNorm += Vec2(0, -1);
	}
	//}
	Vec2 velocity4 = velocity;
	//this->getPhysicsBody()->applyForce(velocity4);
	if (isMoving)
	{
		this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + velocity4);
	}
	else
	{
		this->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}

}

void CPlayer::updateCamera(float dt)
{
	Vec2 origin = this->getPosition() - DISPLAY->getWindowSizeAsVec2() * 0.5f;
	Vec2 screenMos = origin + INPUTS->getMousePosition();
	Vec2 camPos = this->getPosition() - (this->getPosition() - screenMos) / 4.f;
	screenMos -= (this->getPosition() - screenMos) / 4.f;

	//float lerp = 0.1f;
	//camPos.x += (this->position.x - camPos.x) / 100.f;
	//camPos.y += (this->position.y - camPos.y) / 100.f;

	CAMERA->setOrigin(origin);
	CAMERA->setScreenMouse(screenMos);
	CAMERA->setCameraPosition(camPos);
	CAMERA->getCameraTarget()->setPosition(camPos);
}

void CPlayer::updateAnimation(float dt)
{
	Vec2 angleVec = CAMERA->getScreenMouse() - this->getPosition();
	float rot_ = Vec2::angle(Vec2(0, 1), angleVec.getNormalized()) * (180.f / M_PI) - 90.f;

	//Set rotation of the gun
	faceLeft_ = false;
	faceRight_ = false;
	if (CAMERA->getScreenMouse().x < this->getPosition().x)
	{
		if (weapons.size() != 0)
			weapons[cWeapNum]->setScaleX(-1.f);
		rot_ = 360 - rot_;
		faceLeft_ = true;
		
	}
	else
	{
		if (weapons.size() != 0)
			weapons[cWeapNum]->setScaleX(1.f);
		faceRight_ = true;
	}
	if (weapons.size() != 0)
		weapons[cWeapNum]->setRotation(rot_);

	//Determine if the character is facing left, right, up or down. It is very important for the animation
	if (faceLeft_)
		orientation_ = FACELEFT;
	else if (faceRight_)
		orientation_ = FACERIGHT;

	if (isMoving)
	{
		animationTimer += dt;
		if (animationTimer >= 0.09f * animationCounter)
			animationCounter++;
		if (animationCounter >= 8)
		{
			animationCounter = 1;
			animationTimer = 0.0f;
		}

		if (currentACounter != animationCounter)
		{
			currentACounter = animationCounter;
			if (cOrientation_ != orientation_)
			{
				cOrientation_ = orientation_;
			}
			string tmp = "CharacterAnimations/Player/Walking/frame-" + cOrientation_ + "-00" + to_string(animationCounter) + ".png";
			//string tmp = "CharacterAnimations/Player/Walking/frame" + to_string(animationCounter) + ".png";
			this->setTexture(tmp);
		}
	}
	else
	{
		animationTimer = 0.f;
		currentACounter = 1;
		animationCounter = 1;
		if (cOrientation_ != orientation_)
		{
			cOrientation_ = orientation_;
			string tmp = "CharacterAnimations/Player/idle" + orientation_ + ".png";
			this->setTexture(tmp);
		}
	}
}

void CPlayer::update(float dt)
{
	handleMovement(dt);
	updateCamera(dt);
	updateAnimation(dt);
	auto p = this->getPosition() - Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f);
	p = CC_POINT_POINTS_TO_PIXELS(p);
	this->setLocalZOrder(-((p.y + 64) / 64));
}
bool CPlayer::onContactBegin(PhysicsContact & contact)
{
	return true;
}

bool CPlayer::onContactPost(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if ((nodeA->getPhysicsBody()->getTag() == 7 && nodeB->getPhysicsBody()->getTag() == PLAYER_TAG) || (nodeA->getPhysicsBody()->getTag() == PLAYER_TAG && nodeB->getPhysicsBody()->getTag() == 7))
	{
		if (INPUTS->getKey(KeyCode::KEY_E))
		{
			if (nodeA->getPhysicsBody()->getTag() == 7)
			{
				Weapon* weapon = Weapon::create(nodeA->getName());
				weapon->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f - 10.f));
				weapon->setAnchorPoint(Vec2(-0.25f, 0.5f));
				this->addChild(weapon);
				weapon->displayEquip();
				weapons.push_back(weapon);
				nodeA->runAction(RemoveSelf::create());
				return true;
			}
			else
			{
				Weapon* weapon = Weapon::create(nodeB->getName());
				weapon->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f - 10.f));
				weapon->setAnchorPoint(Vec2(-0.25f, 0.5f));
				weapon->setIfPlayer(true);
				this->addChild(weapon);
				weapon->displayEquip();
				weapons.push_back(weapon);
				nodeB->runAction(RemoveSelf::create());
				return true;
			}
		}
		return false;
	}
	return true;
}

bool CPlayer::onContactSeparate(PhysicsContact & contact)
{
	return true;
}
