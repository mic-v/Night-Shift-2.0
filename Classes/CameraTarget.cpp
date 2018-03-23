#include "CameraTarget.h"

CameraTarget* CameraTarget::inst = nullptr;

CameraTarget::CameraTarget()
{
	hasBeenInit = false;
	target = Sprite::create();
	target->setPosition(Vec2(200, 200));
	camera = Follow::create(target, Rect::ZERO);
}
CameraTarget::~CameraTarget()
{
	//Delete the singleton instance
	if (inst)
		delete inst;

	//Clean up the pointer
	inst = nullptr;
}

CameraTarget* CameraTarget::getInstance()
{
	if (!inst)
		inst = new CameraTarget();

	//Return the singleton instance
	return inst;
}

void CameraTarget::setMousePos(Vec2 temp)
{
	mosPos = temp;
}

Vec2 CameraTarget::getMousePos()
{
	return mosPos;
}

Vec2 CameraTarget::getCamPos()
{
	return pos;
}

void CameraTarget::setCamPos(Vec2 temp)
{
	pos = temp;
	target->setPosition(pos);
}

void CameraTarget::init()
{
	if (!hasBeenInit)
	{
		hasBeenInit = true;
	}
	else
	{
		//std::cout << "WARNING: The init() function for the display handler has been called more than once. It should only be called once!" << std::endl;
	}
}

Sprite* CameraTarget::getCamTarget()
{
	return target;
}
