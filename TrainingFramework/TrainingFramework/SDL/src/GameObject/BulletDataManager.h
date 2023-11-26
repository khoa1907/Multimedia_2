#pragma once
#include "GameManager/Singleton.h"
#include <string>
#include <CMath.h>
struct BulletData {
	float speed;
	float damage;
	float sizeX;
	float sizeY;
	float lifeTime;
	std::string bulletSpriteDir;
	int frames;
	int bulletType;
	BulletData(float s = 0, float d = 0, float s2x = 0, float s2y = 0, float lT = 0, 
		std::string a = "RedMidPR.png", int fr = 0, int bt = 0) :
		speed(s), damage(d), sizeX(s2x), sizeY(s2y), lifeTime(lT), bulletSpriteDir(a), frames(fr), bulletType(bt) {}
};
class BulletDataManager :public CSingleton<BulletDataManager> {
public:
	BulletData m_bulletData[12] = {
		{400, 30, 20, 20, 10.0f, "BigRedRR2.png", 1, 0},// Normal red
		{400, 20, 20, 20, 10.0f, "BigGreenRR2.png", 1, 1}, // Normal green
		{400, 20, 20, 20, 10.0f, "BigBlueRR2.png", 1, 2}, // Normal blue
		{300, 45, 32, 32, 10.0f, "BigBlueRR2.png", 1, 2}, //Big blue
		{300, 40, 32, 32, 10.0f, "BigGreenRR2.png", 1, 1},//Big green
		{250, 45, 60, 60, 15.0f, "BigBlueRR2.png", 1, 6},//Big blue bounce

		{250, 40, 60, 60, 15.0f, "BigGreenRR2.png", 1, 5},//Big green bounce
		
		{300, 10, 20, 20, 5.0f, "BigBlueRR2.png", 1, 6},//Small blue fast bounce
		{600, 30, 60, 60, 2.0f, "BigYellowRR2.png", 1, 12},//Big yellow fast bounce CLONE
		{450, 60, 60, 60, 3.0f, "BigGreenRR2.png", 1, 5},//Big green fast bounce
		{650, 20, 25, 25, 5.0f, "BigRedRR2.png", 1, 4},//Small red fast bounce
		{450, 60, 60, 60, 3.0f, "BigBlueRR2.png", 1, 6}//Big blue fast bounce

		/*{400, 30, 100, 100, 10.0f, "BigRedRR2.png", 1, 0},
		{400, 20, 100, 100, 10.0f, "BigGreenRR2.png", 1, 1},
		{400, 20, 100, 100, 10.0f, "BigBlueRR2.png", 1, 2},
		{250, 40, 100, 100, 10.0f, "BigBlueRR2.png", 1, 2},
		{250, 40, 100, 100, 10.0f, "BigGreenRR2.png", 1, 1},*/
	};
};