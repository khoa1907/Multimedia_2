#pragma once
#include "Entity.h"
#include "Player.h"
#include <list>
class GSPlay;
class Enemy : public Entity {
private:

	std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;

	std::shared_ptr<SpriteAnimation> obj;
	std::shared_ptr<SpriteAnimation> obj2;
	std::shared_ptr<Sound> m_Sound;


	GSPlay* m_currentGame;
	float m_shootCooldown;
	float m_currentCooldown;
	float m_currentPositionTime;
	float m_updatePositionCooldown;

public:
	Enemy(float width, float height) {
		m_width = width;
		m_height = height;

	}
	Enemy(float width, float height, GSPlay* game) {
		m_width = width;
		m_height = height;
		m_currentGame = game;
	}
	void Draw(SDL_Renderer* renderer);
	void Init();
	void Shoot(float x, float y, GSPlay* currentGame);
	void Update(float deltaTime);
	void SetActive(bool active);
	void SetAnim(const std::string& dir1, const std::string& dir2, int frames);

	void Moving(std::shared_ptr<Player> player);
	void SetCooldown(float shootCD, float moveCD) { m_shootCooldown = shootCD, m_currentPositionTime = moveCD; }
	void SetSize(float width, float height);
	void SetHealth(float healthChange);

};
