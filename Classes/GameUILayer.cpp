#include "GameUILayer.h"
#include "Definitions.h"

USING_NS_CC;
using namespace cocos2d;

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

bool GameUILayer::init()
{
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	auto vsize = director->getVisibleSize();
	auto vorigin = director->getVisibleOrigin();


	Sprite* phone = Sprite::create("UI/phone.png");
	phone->setPosition(Vec2(vorigin.x + vsize.width - 200, vsize.height - 200));
	this->addChild(phone);

	label = Label::createWithTTF("Press 'E' to Pick Up", "fonts/Marker Felt.ttf", 24);
	//label->setCameraMask((unsigned)CameraFlag::USER1);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(vorigin.x + vsize.width / 2,
			vorigin.y + vsize.height / 2 - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}
	label->setVisible(false);

	time = Label::createWithTTF("0:00", "fonts/double_pixel-7.ttf", 64);
	phone->addChild(time);

	currentWeapon = Label::createWithTTF("Barehands", "fonts/double_pixel-7.ttf", 64);
	currentWeapon->setPosition(Vec2(vorigin.x + vsize.width - 200, vorigin.y + 100));
	this->addChild(currentWeapon);

	ammo = Label::createWithTTF("0 / 0", "fonts/double_pixel-7.ttf", 64);
	ammo->setPosition(Vec2(vorigin.x + vsize.width - 200, vorigin.y + 50));
	this->addChild(ammo);

	border = Sprite::create("healthbar_empty.png");
	border->setAnchorPoint(Vec2(0.5, 0.5));
	border->setPosition(Vec2(vorigin.x + 300, vsize.height - 200));
	this->addChild(border, 0, "hpBorder");

	bar = Sprite::create("healthbar_healthV2.png");
	bar->setAnchorPoint(Vec2(0, 0));
	
	hpBar = ProgressTimer::create(bar);
	hpBar->setType(ProgressTimerType::BAR);
	hpBar->setAnchorPoint(Vec2(0, 0));
	hpBar->setBarChangeRate(Vec2(1, 0));
	hpBar->setMidpoint(Vec2(0, 0));
	border->addChild(hpBar, 10, "hpBar");

	roundStart = Label::createWithTTF("SURVIVE TILL 5 AM", "fonts/double_pixel-7.ttf", 64);
	roundStart->setPosition(Vec2(vorigin.x + vsize.width / 2, vorigin.y + vsize.height - 400));
	this->addChild(roundStart);
	roundStart->runAction(Hide::create());

	roundEnd = Label::createWithTTF("RETURN TO YOUR OFFICE", "fonts/double_pixel-7.ttf", 64);
	roundEnd->setPosition(Vec2(vorigin.x + vsize.width / 2, vorigin.y + vsize.height - 400));
	this->addChild(roundEnd);
	roundEnd->runAction(Hide::create());

	auto roundStartListener = EventListenerCustom::create("roundStart", [=](EventCustom* event) {
		auto sequence = Sequence::create(Show::create(), DelayTime::create(1.f), Hide::create(), NULL);
		roundStart->runAction(sequence);
	});

	auto finishEnemyListener = EventListenerCustom::create("finishEnemy", [=](EventCustom* event) {
		roundEnd->setString("KILL ANY REMAINING ENEMIES");
		roundEnd->runAction(Show::create());
	});

	auto roundEndListener = EventListenerCustom::create("roundEnd", [=](EventCustom* event) {
		std::string tmp("RETURN TO YOUR OFFICE");
		roundEnd->setString(tmp);
		roundEnd->runAction(Show::create());
	});

	auto timerListener = EventListenerCustom::create("timer", [=](EventCustom* event) {
		char* text = static_cast<char*>(event->getUserData());
		std::string tmp(text);
		time->setString(tmp);
	});

	auto ammoListener = EventListenerCustom::create("ammo", [=](EventCustom* event) {
		char* text = static_cast<char*>(event->getUserData());
		std::string tmp(text);
		ammo->setString(tmp);
	});
	auto weaponChangeListener = EventListenerCustom::create("weapon", [=](EventCustom* event) {
		char* text = static_cast<char*>(event->getUserData());
		std::string tmp(text);
		currentWeapon->setString(tmp);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(ammoListener, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(weaponChangeListener, 2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(roundStartListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(roundEndListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(timerListener, this);
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameUILayer::onContactBegin, this);
	contactListener->onContactPreSolve = CC_CALLBACK_1(GameUILayer::onContactPost, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(GameUILayer::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

bool GameUILayer::onContactBegin(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if ((nodeA->getPhysicsBody()->getTag() == 7 && nodeB->getPhysicsBody()->getTag() == PLAYER_TAG) || (nodeA->getPhysicsBody()->getTag() == PLAYER_TAG && nodeB->getPhysicsBody()->getTag() == 7))
		{
			label->setVisible(true);
			std::cout << "ASDF" << std::endl;
		}
	}
	return true;
}

bool GameUILayer::onContactPost(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		if ((nodeA->getPhysicsBody()->getTag() == 7 && nodeB->getPhysicsBody()->getTag() == PLAYER_TAG) || (nodeA->getPhysicsBody()->getTag() == PLAYER_TAG && nodeB->getPhysicsBody()->getTag() == 7))
		{
		}
	}
	return true;
}

bool GameUILayer::onContactSeparate(PhysicsContact & contact)
{
	label->setVisible(false);

	return true;
}
