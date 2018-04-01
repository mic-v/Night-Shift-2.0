#ifndef CAMERA_H_
#define CAMERA_H_

#include "cocos2d.h";
#include <iostream>

using namespace cocos2d;
using namespace std;

class GameCamera
{
protected:
	GameCamera();
public:
	~GameCamera();

	//Get the Camera
	static GameCamera* getInstance();

	static void destroyInstance();

	//Get the target the Camera is following
	Follow* getCamera();

	Sprite* getCameraTarget();

	void init();

	//GETTERS//
	Vec2 getOrigin() const;
	Vec2 getScreenMouse() const;
	Vec2 getCameraPosition() const;

	Vec2 getMosPos()
	{
		return mos;
	}

	void setMos(Vec2 tmp)
	{
		mos = tmp;
	}

	//SETTERS//
	void setOrigin(Vec2);
	void setScreenMouse(Vec2);
	void setCameraPosition(Vec2);
	void setCameraTarget(Sprite*);
	void setCameraFollow();

	//void updateCamera(CPlayer *);

private:
	bool hasBeenInit;

	static GameCamera* inst;
	Follow* camera;
	Sprite* cameraTarget;

	Vec2 origin;
	Vec2 screenMos;
	Vec2 camPos;
	Vec2 mos;
};

#define CAMERA GameCamera::getInstance()

#endif