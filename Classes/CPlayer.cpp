#include "CPlayer.h"
#include "DisplayHandler.h"
#include "Definitions.h"
#include "InputHandler.h"
#include "GameCamera.h"
#include "SimpleAudioEngine.h"
CPlayer::CPlayer()
{
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
	cbody->setVelocityLimit(VELOCITYLIMIT);
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
	sprintDuration = 0.f;
	cWeapNum = 0;
	isMoving = false;
	isSprinting = false;
	health = MAX_PLAYER_HEALTH;

	initAnimations();

	border = Sprite::create("UI/dashCooldownframe.png");
	border->setAnchorPoint(Vec2(0.5, 0.5));
	border->setPosition(this->getContentSize().width * 0.5f, -10);
	border->runAction(Hide::create());
	this->addChild(border, 0, "hpBorder");

	bar = Sprite::create("UI/dashCooldownbar.png");
	bar->setAnchorPoint(Vec2(0, 0));

	dashBar = ProgressTimer::create(bar);
	dashBar->setType(ProgressTimerType::BAR);
	dashBar->setAnchorPoint(Vec2(0, 0));
	dashBar->setBarChangeRate(Vec2(1, 0));
	dashBar->setMidpoint(Vec2(0, 0));
	border->addChild(dashBar, 10, "dashBar");
	dashBar->setPercentage(100);

	auto dashListener = EventListenerCustom::create("dash", [=](EventCustom* event) {
		//border->setVisible(true);
		dashBar->runAction(ProgressFromTo::create(4, 0, 100));
	});

	/* PHYSICS CONTACT LISTENERS*/
	auto contactListene = EventListenerPhysicsContact::create();
	contactListene->onContactBegin = CC_CALLBACK_1(CPlayer::onContactBegin, this);
	contactListene->onContactPreSolve = CC_CALLBACK_1(CPlayer::onContactPost, this);
	contactListene->onContactSeparate = CC_CALLBACK_1(CPlayer::onContactSeparate, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListene, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(dashListener, this);
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
	idleFrameL->retain();
	animation = Animation::createWithSpriteFrames(idleleft, 0.09f);
	idleL = Animate::create(animation);
	idleL->retain();

	spritecache->destroyInstance();
	spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("res/HDR/player-dying.plist");
	//this->stopAction(idleR);
	//this->getActionByTag()
	Vector<SpriteFrame*> die;
	for (int i = 0; i < 7; i++)
	{
		stringstream ss;
		ss << "frame" << i + 1 << ".png";
		die.pushBack(spritecache->getSpriteFrameByName(ss.str()));
	}
	animation = Animation::createWithSpriteFrames(die, 0.09f);
	dying = Animate::create(animation);
	dying->setTag(59);
	dying->retain();

}

void CPlayer::handleInput()
{
}

void CPlayer::handleMovement(float dt)
{
	if (sprintDuration <= 0.f)
		isSprinting = false;
	if(dashBar->getPercentage() == 100)
		border->runAction(Hide::create());

	isMoving = false;
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
		if (INPUTS->getKeyPress(KeyCode::KEY_SHIFT))
		{
			if (!isSprinting  && dashBar->getPercentage() == 100)
			{
				isSprinting = true;
				border->runAction(Show::create());
				this->runAction(Sequence::create(TintTo::create(0.1f, Color3B(183, 224, 255)), TintTo::create(0.1f, Color3B(255, 255, 255)), NULL));
				EventCustom event("dash");
				_eventDispatcher->dispatchEvent(&event);
			}
		}
	}

	if (isSprinting)
	{
		this->getPhysicsBody()->setVelocityLimit(VELOCITYLIMIT * 2);
		hspeed = 120;
		vspeed = 120;
		sprintDuration += dt;
		if (sprintDuration >= 0.5f)
		{
			sprintDuration = 0.f;
		}
	}
	else
	{
		hspeed = 70;
		vspeed = 70;
		this->getPhysicsBody()->setVelocityLimit(VELOCITYLIMIT);
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
	Vec2 nextVelocity = velocity;
	if (isMoving)
	{
		this->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity() + nextVelocity);
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
	faceUp_ = false;
	faceDown_ = false;
	if (CAMERA->getScreenMouse().x < this->getPosition().x)
	{
		if (weapons.size() != 0)
			weapons[cWeapNum]->setScaleX(-1.f);
		rot_ = 360 - rot_;
		
	}
	else
	{
		if (weapons.size() != 0)
			weapons[cWeapNum]->setScaleX(1.f);
	}
	if (weapons.size() != 0)
		weapons[cWeapNum]->setRotation(rot_);

	if (rot_ >= -45 && rot_ < 45)
		faceRight_ = true;
	else if (rot_ >= 45 && rot_ < 315)
		faceDown_ = true;
	else if (rot_ >= 315 && rot_ < 405)
		faceLeft_ = true;
	else
		faceUp_ = true;
	//Determine if the character is facing left, right, up or down. It is very important for the animation
	if (faceLeft_)
		orientation_ = FACELEFT;
	else if (faceRight_)
		orientation_ = FACERIGHT;
	else if (faceDown_)
		orientation_ = FACEDOWN;
	else
		orientation_ = FACEUP;

	if (weapons.size() != 0)
	{
		if (cOrientation_ == FACEUP)
			weapons[cWeapNum]->setLocalZOrder(-1000);
		else
			weapons[cWeapNum]->setLocalZOrder(1000);
	}
	if (isSprinting)
	{
		animationTimer += dt;
		if (animationTimer >= 0.125f * animationCounter)
			animationCounter++;
		if (animationCounter >= 4)
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
			string tmp = "CharacterAnimations/Player/Running/" + cOrientation_ + "/frame" + to_string(animationCounter) + ".png";
			this->setTexture(tmp);
		}
	}
	else if (isMoving)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/Character/Walking.wav", false);
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
			string tmp = "CharacterAnimations/Player/Walking/" + cOrientation_ + "/frame" + to_string(animationCounter) + ".png";
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
		}
		string tmp = "CharacterAnimations/Player/Walking/" + cOrientation_ + "/idle.png";
		this->setTexture(tmp);
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
				if (nodeA->getName() == "AK47.png")
					weapon->setAnchorPoint(Vec2(0.25f, 0.5f));
				else
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

				if (nodeB->getName() == "AK47.png")
					weapon->setAnchorPoint(Vec2(0.25f, 0.5f));
				else
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

void CPlayer::damage()
{
	this->health--;
	float* tmp = &(this->health);
	EventCustom event("health");
	event.setUserData((void*)&(this->health));
	_eventDispatcher->dispatchEvent(&event);
	if (this->health == 0)
	{
		weapons[cWeapNum]->setVisible(false);
		this->stopAllActions();
		this->runAction(dying);
		this->unscheduleUpdate();
		EventCustom event("gameOver");
		_eventDispatcher->dispatchEvent(&event);
	}
}
