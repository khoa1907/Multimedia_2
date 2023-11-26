#include "EnemyManager.h"
#include "Tilemap.h"

void EnemyManager::SpawnEnemy(Vector3 position, int index)
{
	if (m_inActiveEnemies.empty()) {
		printf("EMPTY!");
		return;
	}
	printf("ENEMY ADDED\n");
	m_currentGame->AddEntity(m_inActiveEnemies.back());
	std::shared_ptr<Enemy> tempEnemy = m_inActiveEnemies.back();

	tempEnemy->SetActive(true);
	tempEnemy->SetInitialHealth(m_enemyTypes[index].health);
	tempEnemy->SetMovementSpeed(m_enemyTypes[index].movementSpeed);
	tempEnemy->SetAnim(m_enemyTypes[index].idleAnimDir, m_enemyTypes[index].moveAnimDir, m_enemyTypes[index].numOfFrames);
	tempEnemy->SetBulletID(m_enemyTypes[index].bulletType);
	tempEnemy->SetSize(30 * m_enemyTypes[index].sizeMultiplier, 45 * m_enemyTypes[index].sizeMultiplier);
	if (m_enemyTypes[index].sizeMultiplier >= 4) tempEnemy->m_frozenResistance = 2.0f;
	tempEnemy->SetPosition(position);
	tempEnemy->SetCooldown(m_enemyTypes[index].shootCooldown, m_enemyTypes[index].moveCooldown);
	m_activeEnemies.push_back(tempEnemy);
	m_inActiveEnemies.pop_back();
}

void EnemyManager::ReturnInactiveEnemies()
{	
	if (m_activeEnemies.empty() && isInCombat) {
		if (m_currentWaveCount <= m_maxWaveCount) {
			SpawnCurrentWave(m_currentRoom);
		}
		else {
			printf("out of combat");
			isInCombat = false;
			Tilemap::GetInstance()->SetDoorActive(true);
		}
	}
	/*while (!m_activeEnemies.empty() && m_activeEnemies.back()->IsDead()) {
		m_inActiveEnemies.push_back(m_activeEnemies.back());
		m_activeEnemies.pop_back();
	}*/

	auto iit = m_activeEnemies.begin();
	while (iit != m_activeEnemies.end()) {
		if ((*iit)->IsDead()) {
			m_inActiveEnemies.push_back(*iit);
			iit = m_activeEnemies.erase(iit);
		}
		else std::advance(iit, 1);
	}
}

void EnemyManager::Update(float deltaTime)
{

}

void EnemyManager::GenerateEnemies()
{
	for (int i = 0; i < 20; i++) CreateEnemy();
}

void EnemyManager::CreateEnemy()
{
	std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(30, 45, m_currentGame);
	enemy->Init();
	enemy->SetActive(false);
	m_inActiveEnemies.push_back(enemy);

}

void EnemyManager::SpawnWaveInRoom(int room)
{
	m_currentRoom = room;
	if (m_spawnPos[room].size() > m_maxConcurrentEnemy) {
		m_maxWaveCount = m_spawnPos[room].size() / m_maxConcurrentEnemy;
		if (m_spawnPos[room].size() % m_maxConcurrentEnemy > 0) m_maxWaveCount++;
		m_currentWaveCount = 1;
	}
	else m_maxWaveCount = 1;
	m_currentWaveCount = 1;

	isInCombat = true;
	SpawnCurrentWave(room);
}

void EnemyManager::SpawnCurrentWave(int room)
{
	int waveOrder = 0;
	printf("room spawn at %d\n", room);
	for (auto i : m_spawnPos[room]) {
		if (waveOrder < m_currentWaveCount * m_maxConcurrentEnemy - m_maxConcurrentEnemy) {
			waveOrder++;
			continue;
		}
		if (waveOrder < m_currentWaveCount * m_maxConcurrentEnemy) waveOrder++;
		else break;
		printf("spawning %f %f \n", i.first.x, i.first.y);
		SpawnEnemy(Vector3(i.first.x, i.first.y, 0), i.second);
	}
	m_currentWaveCount++;
}

void EnemyManager::ChangeLevel()
{
	for (int i = 0; i < 2; i++) m_spawnPos[i].clear();
	m_currentGame->isLevelChanged = true;
}
