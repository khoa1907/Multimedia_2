#pragma once
#include "GameManager/Singleton.h"

enum playState {
	PLAYING = 0,
	PAUSED = 1,
	DEATH = 2,
	WIN = 3
};
class GSPlayStates : public CSingleton<GSPlayStates> {
public:
	int m_currentPlayState;
};