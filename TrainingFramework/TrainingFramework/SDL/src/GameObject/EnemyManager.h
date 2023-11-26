#pragma once
#include "GameManager/Singleton.h"
#include "Enemy.h"
#include "GSPlay.h"
struct EnemyInfo {
	float health;
	float movementSpeed;
	int bulletType;
	std::string idleAnimDir;
	std::string moveAnimDir;
	int numOfFrames;
	float shootCooldown;
	float moveCooldown;
	int sizeMultiplier;

	EnemyInfo(float h, float  ms, int bt, std::string ia, std::string ma, int nof, float scd = 3.0f, float mcd = 3.0f, int sm = 1) :
		health(h), movementSpeed(ms), bulletType(bt), idleAnimDir(ia), moveAnimDir(ma), numOfFrames(nof), shootCooldown(scd), moveCooldown(mcd),sizeMultiplier(sm){}
	
};
class EnemyManager: public CSingleton<EnemyManager>{
private:
	EnemyInfo m_enemyTypes[8] = {
		{30.0f, 50.0f, 0,"Monster-idle.png", "Monster-walk.png", 4, 3.0f, 3.0f},
		{20.0f, 100.0f, 0, "Ghost-idle.png", "Ghost-walk.png", 4, 5.0f, 1.0f},//Ghost - use normal
		{20.0f, 150.0f, 7, "Drop-idle.png", "Drop-walk.png", 4, 10.0f, 5.0f},//Drop - use small, freeze, bounce
		{80.0f, 50.0f, 3, "Robot-idle.png", "Robot-walk.png", 4, 5.0f, 3.0f},//Robot - use big, freeze
		{110.0f, 50.0f, 4, "Skull-idle.png", "Skull-walk.png", 4, 7.0f, 4.0f},//Skull - use big, poison
		{150.0f, 20.0f, 5, "Robot-idle.png", "Robot-walk.png", 4, 10.0f, 5.0f, 2},//Giant robot- use big, freeze, bounce
		{200.0f, 20.0f, 6, "Skull-idle.png", "Skull-walk.png", 4, 10.0f, 5.0f, 2},//Giant skull- use big, poison, bounce
		{1400.0f, 20.0f, 8, "Robot-idle.png", "Robot-walk.png", 4, 2.2f, 1.0f, 5}//Boss robot- use big, freeze, bounce
	};
	void SetEnemyInfo(int i);
	int m_maxWaveCount;
	int m_currentWaveCount;
	int m_currentRoom;
	int m_maxConcurrentEnemy = 6;

public:
	int m_maxRoom;
	bool isInCombat;
	std::list<std::shared_ptr<Enemy>> m_activeEnemies;
	std::list<std::shared_ptr<Enemy>> m_inActiveEnemies;
	std::list<std::pair<Vector2,int>> m_spawnPos[2];
	GSPlay* m_currentGame;

	void SpawnEnemy(Vector3 position, int index);
	void ReturnInactiveEnemies();
	void Update(float deltaTime);
	void GenerateEnemies();
	void CreateEnemy();
	void SpawnWaveInRoom(int room);
	void SpawnCurrentWave(int room);
	void ChangeLevel();

};
