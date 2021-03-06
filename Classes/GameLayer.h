#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Item.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include <string>
using namespace cocos2d;

class GameLayer : public cocos2d::Layer
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(GameLayer);
	//static cocos2d::Scene* createScene();

	virtual bool init();
	static void setSaveFile(std::string);
	void update(float dt);
	void createHeart(Vec2 &);
	void createAK(Vec2 &);
	void createAmmo(Vec2 &);
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
	static int level_;
private:
	void initLevel();
	void findEnemyandHurt(Node* node);
	int getObjectZ(Vec2);

	Vec2 velocity;
	Vec2 previousPos;
	Label* label;

	//Level Setup
	CPlayer* cplayer;
	CEnemy* enmy;
	TMXTiledMap* map;
	static std::string saveFile;


	std::vector<Sprite*> tiles;

	//Round Variables
		//Bool start and end
	bool roundStart_;
	bool roundEnd_;
	float spawnRate;
	float spawnLimit;
	float levelTimer;
	bool spawnHeart;
	Vec2 spawnHeartPos;
	bool spawnAK;
	Vec2 spawnAKPos;
	bool spawnAmmo;
	Vec2 spawnAmmoPos;
		//ENEMY POSITIONS
	int randomSpawn;
	int randomWeapon;
	Vec2 enemySpawn1;
	Vec2 enemySpawn2;
	Vec2 enemySpawn3;
	Vec2 enemySpawn4;

	std::vector<Sprite*> doorList;

	std::vector<CEnemy*> enemyList;
};

#endif 
