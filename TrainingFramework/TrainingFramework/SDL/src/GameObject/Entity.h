#pragma once
#include <BaseObject.h>
#include "SpriteAnimation.h"
#include "ResourceManagers.h"
#include "Tile.h"
#include"Text.h"
#include "Sound.h"


class TextureManager;
class Entity : public BaseObject {
private:
	
protected:
	std::shared_ptr<Sprite2D> m_barContainer;
	std::shared_ptr<Sprite2D> m_barFill;
	std::shared_ptr<Text> m_frozenText;
	std::shared_ptr<Text> m_poisonText;

	float m_frozenCooldown = 3.0f;
	float m_currentFrozenCooldown ;

	float m_poisonCooldown = 2.0f;
	float m_currentPoisonCooldown ;

	int m_bulletShootID;
	int m_poisonTicks = 0;
	float m_poisonDamage = 5;
	

	float m_movementSpeed;
	float m_health;
	float m_maxHealth;
	Vector3 m_moveDirection;
	Vector3 m_moveVelocity;

	float m_width;
	float m_height;
	float m_radius;

	float relativePushY, relativePushX;
	Vector3 otherVelocity;


	bool m_isDead = false;
	bool m_isActive = false;
	bool m_touchXWall = false;

	std::shared_ptr<SpriteAnimation> it;
	void CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, std::shared_ptr<TextureManager>& texture, int frames);
	void CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, const std::string& txtLink, int frames);
	void CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, std::shared_ptr<TextureManager>& texture, int frames, float speed);
	void CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, const char* txtLink, int frames, float speed);

public:
	float m_poisonResistance = 0;
	float m_frozenResistance = 0;
	int layerID;
	int m_entityType;
	virtual void Init();
	Vector2 GetSize();
	Vector3 GetPosition();
	Vector3 GetVelocity();
	float GetHealth();
	virtual void ProcessCollision(Entity* other, SDL_FRect* intersect);
	virtual void ProcessCollision(SDL_FRect* intersect);
	virtual void ProcessCollision(Entity* other);
	virtual void ProcessCollision();
	bool IsDead();

	void Update(float deltaTime);
	virtual void Draw(SDL_Renderer* renderer);
	
	virtual void CheckCollision(Entity* other);
	void CheckCollision(Tile* other);

	virtual void SetHealth(float healthChange); // Health
	void SetInitialHealth(float health); // Health
	void SetSize(float width, float height);// Size
	virtual void SetActive(bool isActive); //Active
	//Position: Inherited
	void SetPosition(Vector3 position) { m_position = position; }
	void SetPosition(float x, float y) { m_position.x = x; m_position.y = y; }

	void SetMovementSpeed(float movementSpeed); //speed
	void SetMovementDirection(Vector3 dir) { m_moveDirection = dir; }
	
	virtual void PostSolveVelocity();
	virtual void PreSolveVelocity(float deltaTime);
	virtual void SetAnim(const std::string& dir1, const std::string& dir2, int frames);
	virtual void SetBulletID(int id) { m_bulletShootID = id; }

	virtual void SetStatusEffect(int statusType);
	virtual void SetFrozen();
	virtual void SetPoisoned();
	float GetSpeed() { return m_movementSpeed; }

};