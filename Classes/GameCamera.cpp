#include "GameCamera.h"
#include "InputHandler.h"

//--- Static Variables ---//
GameCamera* GameCamera::inst = nullptr;
//Follow* GameCamera::camera = nullptr;
//Sprite* GameCamera::cameraTarget = nullptr;

GameCamera::GameCamera()
{
	hasBeenInit = false;

	cameraTarget = Sprite::create();
	cameraTarget->setPosition(Vec2(0, 0));
	camera = Follow::create(cameraTarget, Rect::ZERO);
	origin = Director::getInstance()->getVisibleOrigin();
}

GameCamera::~GameCamera()
{
	//Delete the singleton instance
	if (inst)
		delete inst;

	//Clean up the pointer
	inst = nullptr;
}

Follow * GameCamera::getCamera()
{
	return camera;
}

GameCamera * GameCamera::getInstance()
{
	//Create the singleton instance if it hasn't already been created
	if (!inst)
		inst = new GameCamera();

	//Return the singleton instance
	return inst;
}

void GameCamera::init()
{
	if (!hasBeenInit)
	{
		hasBeenInit = true;
	}
	else
	{
		std::cout << "WARNING: The init() function for the display handler has been called more than once. It should only be called once!" << std::endl;
	}
}

Sprite * GameCamera::getCameraTarget()
{
	return cameraTarget;
}

Vec2 GameCamera::getOrigin() const
{
	return origin;
}
Vec2 GameCamera::getScreenMouse() const
{
	return screenMos;
}

Vec2 GameCamera::getCameraPosition() const
{
	return camPos;
}

void GameCamera::setOrigin(Vec2 origin_)
{
	origin = origin_;
}

void GameCamera::setScreenMouse(Vec2 screenMos_)
{
	screenMos = screenMos_;
}

void GameCamera::setCameraPosition(Vec2 camPos_)
{

	camPos = camPos_;
}

//void GameCamera::updateCamera(CPlayer * player)
//{
//	origin = player->getPosition() - Vec2(1792 * 0.5f, 1008 * 0.5f);
//	screenMos = origin + INPUTS->getMousePosition();
//	camPos = player->getPosition() - (player->getPosition() - screenMos) / 4.f;
//	screenMos -= (player->getPosition() - screenMos) / 4.f;
//	cameraTarget->setPosition(camPos);
//
//	Vec2 angleVec = screenMos - player->getPosition();
//	float agle = Vec2::angle(Vec2(0, 1), angleVec.getNormalized()) * (180.f / M_PI) - 90.f;
//
//	if (screenMos.x < player->getPosition().x)
//	{
//
//	}
//	player->updateGunPosition(agle);
//}

