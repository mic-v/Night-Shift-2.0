#ifndef _CAMERATARGET_H_
#define _CAMERATARGET_H_



#include "cocos2d.h"
#include <string>
using namespace cocos2d;
class CameraTarget
{
protected:
	CameraTarget();
public:
	~CameraTarget();

	static CameraTarget* getInstance();
	void setMousePos(Vec2 temp);
	Vec2 getMousePos();
	Vec2 getCamPos();
	void setCamPos(Vec2 temp);
	void init();
	Sprite* getCamTarget();
private:
	Follow* camera;
	Sprite* target;
	Vec2 mosPos;
	Vec2 pos;
	bool hasBeenInit;
	static CameraTarget* inst;
};

#define CAMERA CameraTarget::getInstance()

#endif // !_CAMERATARGET_H_