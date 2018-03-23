#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameUILayer.h"
#include "GameLayer.h"
using namespace cocos2d;
class GameScene : public cocos2d::Scene
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void update(float dt);

	bool onContactBegin(PhysicsContact & contact);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	Camera* sceneCamera;
	Follow* sceneFollow;
	//GameScene* glayer;
private:
	Sprite* spr;
	Player* h;
	DrawNode* node;
	Label* label;
	static Scene* scenehandle;
	static Layer* layerhandle;
};


#endif // __HELLOWORLD_SCENE_H__
