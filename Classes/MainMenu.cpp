#include "MainMenu.h"
#include "GameCamera.h"
#include <fstream>
using namespace cocos2d;

cocos2d::Scene* MainMenu::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	auto layer = MainMenu::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

bool MainMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size screenSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *title = Sprite::create("MainMenuTitle-Alt.png");
	title->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.75 + origin.y));
	this->addChild(title);

	auto closeItem = MenuItemImage::create(
		"UI/exit.png",
		"UI/exit(selected).png",
		CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

	float x = screenSize.width / 2 + origin.x;
	float y = screenSize.height * 0.4 + origin.y;
	closeItem->setPosition(Vec2(x, y));

	auto gameItem = MenuItemImage::create(
		"UI/start.png",
		"UI/start(selected).png",
		CC_CALLBACK_1(MainMenu::menuStartCallback, this));

	x = screenSize.width / 2 + origin.x;
	y = screenSize.height * 0.5 + origin.y;
	gameItem->setPosition(Vec2(x, y));

	menu = Menu::create(gameItem, closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	newGameButton = ui::Button::create("UI/newGame.png", "UI/newGame(selected).png");
	newGameButton->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));
	this->addChild(newGameButton);
	newGameButton->setVisible(false);
	newGameButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::newGameTouchEvent, this));

	startGameButton = ui::Button::create("UI/start.png", "UI/start(selected).png");
	startGameButton->setPosition(Vec2(screenSize.width / 2 + origin.x + 300, screenSize.height / 2 + origin.y));
	this->addChild(startGameButton);
	startGameButton->setVisible(false);
	startGameButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::startGameTouchEvent, this));



	saveLabel = Label::createWithTTF("Saves", "fonts/double_pixel-7.ttf", 48);
	saveLabel->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y - 115));
	saveLabel->setColor(Color3B(0, 55, 255));
	this->addChild(saveLabel);
	saveLabel->setVisible(false);

	textField = ui::TextField::create("Input text here", "fonts/double_pixel-7.ttf", 48);
	textField->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));
	this->addChild(textField);
	textField->setVisible(false);
	textField->setTouchEnabled(true);
	textField->setMaxLengthEnabled(true);
	textField->setMaxLength(12);
	textField->addEventListener(CC_CALLBACK_2(MainMenu::textFieldEvent, this));

	scrollView = ui::ScrollView::create();

	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(700, 400));
	scrollView->setInnerContainerSize(Size(1280, 2500));
	scrollView->setBackGroundImage("HelloWorld.png");
	scrollView->setBounceEnabled(true);
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y - 350));

	for (int i = 0; i < 50; i++)
	{

		auto text = ui::Text::create("Empty", "fonts/double_pixel-7.ttf",32);
		text->setPosition(Vec2(scrollView->getContentSize().width / 2, i * 50));
		text->setTouchEnabled(true);
		text->addTouchEventListener(CC_CALLBACK_2(MainMenu::startTouchEvent, this));

		scrollView->addChild(text);
	}
	this->addChild(scrollView);
	scrollView->setVisible(false);
	//ui::Button* startButton = ui::Button::create("UI/start.png", "UI/start(selected).png");
	//startButton->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y - 200));
	//startButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::startTouchEvent, this));
	//this->addChild(startButton);
	return true;
}

void MainMenu::menuCloseCallback(cocos2d::Ref * pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void MainMenu::menuGameCallback(cocos2d::Ref * pSender)
{
	Scene *scene = GameScene::createScene();
	TransitionFade *transition = TransitionFade::create(2.f, scene);
	Director::getInstance()->replaceScene(transition);
	CAMERA->init();
}

void MainMenu::menuStartCallback(cocos2d::Ref * pSender)
{
	Scene *scene = GameScene::createScene();
	TransitionFade *transition = TransitionFade::create(2.f, scene);
	Director::getInstance()->replaceScene(transition);
	CAMERA->init();
}

void MainMenu::menuSaveCallback(cocos2d::Ref * pSender, ui::Text::TouchEventType event)
{
}

void MainMenu::startTouchEvent(cocos2d::Ref * pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		// code to handle when the button is first clicked
		break;

	case ui::Widget::TouchEventType::ENDED:

		break;

	default:
		break;
	}
}

void MainMenu::newGameTouchEvent(cocos2d::Ref * pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::ENDED:
		newGameButton->setVisible(false);
		textField->setVisible(true);
		break;

	default:
		break;
	}

}

void MainMenu::startGameTouchEvent(cocos2d::Ref * pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::ENDED:
		initiateNewGame(saveFile);
		break;

	default:
		break;
	}
}

void MainMenu::selectedItemEvent(cocos2d::Ref * pSender, ui::ListView::EventType type)
{
	switch (type)
	{
	case ui::ListView::EventType::ON_SELECTED_ITEM_START:
		log("selected child end index =");
		break;
	case ui::ListView::EventType::ON_SELECTED_ITEM_END:
		log("selected child end index =");
		break;
	default:
		break;
	}
}

void MainMenu::textFieldEvent(cocos2d::Ref * pSender, ui::TextField::EventType type)
{
	ui::TextField* textf_ = dynamic_cast<ui::TextField *>(pSender);
	switch (type)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME:
		break;
	case ui::TextField::EventType::DETACH_WITH_IME:
		saveFile = textf_->getString();
		startGameButton->setVisible(true);
		break;
	case ui::TextField::EventType::INSERT_TEXT:
		break;
	case ui::TextField::EventType::DELETE_BACKWARD:
		break;
	default:
		break;
	}
}

void MainMenu::initiateNewGame(string tmp)
{
	//std::ofstream file;
	//file.open("Listofsaves.txt", ios::app);
	//file << tmp << endl;
	//file.close();

	////NEW SAVE INITIALIZE
	///*
	//Format
	//Level 
	//Score

	//*/
	//file.open("Saves/" + tmp + ".txt");
	//file << "1" << endl;
	//file << "0" << endl;
	//file.close();

	//GameLayer::setSaveFile(tmp);
	Scene *scene = GameScene::createScene();
	TransitionFade *transition = TransitionFade::create(2.f, scene);
	Director::getInstance()->replaceScene(transition);
	CAMERA->init();
}

