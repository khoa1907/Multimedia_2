#pragma once
#include "GameManager/Singleton.h"
#include "GSPlay.h"
#include <string>
class GameSaver : public CSingleton <GameSaver> {
public:
	float m_savedHealth;
	int m_savedStage;
	GSPlay* m_currentGame;

	void SaveFile(const std::string& path);
	void LoadFile(const std::string& path);
};