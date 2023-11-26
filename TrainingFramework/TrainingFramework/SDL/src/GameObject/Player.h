#pragma once
#include "Entity.h"
#include "BulletDataManager.h"
#include <list>
class GSPlay;

class Player : public Entity {
private:
	
	std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;

	std::shared_ptr<SpriteAnimation> obj;
	std::shared_ptr<SpriteAnimation> obj2;
	std::shared_ptr<SpriteAnimation> grabAnim;
	std::shared_ptr<SpriteAnimation> grabHand;
	std::shared_ptr<Sound> m_Sound;
	std::shared_ptr<Sound> m_GrabSound;


	//std::shared_ptr<GSPlay> currentGame;
	bool canShoot;
	bool canGrab;
	bool isGrabbing;

	float m_grabCooldown;
	float m_currentGrabCooldown;

	float m_grabDuration;
	float m_currentGrabDuration;
	void UpdateGrab(float deltaTime);

	BulletData m_bulletStats;
	const float m_maxDamage = 200.0f;
	const float m_maxSize = 100;

	struct GrabStats{
		float grabDistance;
		Vector2 grabSize;
		Vector2 grabPos;
		Vector2 grabPosOffset;
		
	}m_grabStats;

public:
	Player(float width, float height) {
		m_width = width;
		m_height = height;
		
	}
	Player(float width, float height, GSPlay* game) {
		Player(width, height);
	}
	void Init();
	void ProcessInput(int keyPress);
	void Shoot(int x, int y, GSPlay* currentGame);
	void CheckCollision(Entity* other);



	void Update(float deltaTime);

	void MouseMove(int x, int y);
	void GrabBullet(int x, int y);
	void Draw(SDL_Renderer* renderer);

	void ResetCombat();
	void SetGrabStats(GrabStats grabStats);
	void SetFrozen();
	void SetHealth(float healthChange); // Health


};