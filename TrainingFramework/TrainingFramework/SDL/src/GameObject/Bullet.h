#pragma once
#include "Entity.h"
#include "BulletDataManager.h"

class Bullet : public Entity {
private:
	float m_damage;
	float m_lifetime;
	void DestroyBullet();
public:
	Bullet() {
		Init();
	}	
	int m_bulletEffect;

	void Update(float deltaTime);
	void Init();
	void SetFirstSpawn(float lifetime, float damage, float movementSpeed, Vector3 startPos, Vector3 dir);
	
	void ProcessCollision(Entity* other, SDL_FRect* intersect);
	void ProcessCollision(SDL_FRect* intersect);
	void ProcessCollision(Entity* other);
	void ProcessCollision();
	//void PostSolveVelocity();
	void SetBulletData(int id);
	void SetBulletData(BulletData bulletData);
	void SetStatusEffect(int statusType);
	void CloneBullets();

	void SetHealth(float healthChange);

};