#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "Player.h"
#include "Entity.h"
#include "Item.h"
#include "CPlayer.h"
#include "Enemy.h"
using namespace cocos2d;

class GameLayer : public cocos2d::Layer
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(GameLayer);
	//static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float dt);
	void startRound();
	void endRound();
	// a selector callback
	//Contact after checking if two shapes still
	bool onContactBegin(PhysicsContact & contact);
	//Contact after checking if two shapes are still colliding
	bool onContactPost(PhysicsContact & contact);
	void menuCloseCallback(cocos2d::Ref* pSender);

	void repositionSprite(float);
	Camera* sceneCamera;
	Follow* sceneFollow;
	bool dontmove;
	bool wallCollision;
private:
	void initLevel();


	Vec2 velocity;
	Vec2 previousPos;
	Label* label;
	CPlayer* cplayer;
	TMXTiledMap* map;
	Sprite* _tamara;
	Sprite* tile_;

	std::vector<Sprite*> tiles;
	std::vector<Enemy*> enemyList;

	void findEnemyandHurt(Node* node);
	//ENEMY POSITIONS
	Vec2 enemySpawn1;
	Vec2 enemySpawn2;
	Vec2 enemySpawn3;
	Vec2 enemySpawn4;
	//Bool
	bool roundStart_;
};

#endif // __HELLOWORLD_SCENE_H__
