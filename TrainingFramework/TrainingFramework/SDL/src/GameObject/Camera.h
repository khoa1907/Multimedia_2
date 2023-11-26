#pragma once
#include "CMath.h"
#include "SDL.h"
#include "SpriteAnimation.h"
#include "Define.h"
#include "GameManager/Singleton.h"
#include "Entity.h"
#include "Tile.h"
class Camera : public CSingleton<Camera>
{
public:
	Camera() { m_ViewBox = { 0, 0, SCREEN_WIDTH, SCREEN_HEIDHT }; }
	SDL_Rect GetViewBox();
	Vector2 GetPosition();
	void SetTarget(std::shared_ptr<Entity> target);
	void Update(float deltaTime);
	void SetMaxCamSpeed(float speed);
	bool isWithinCamera(std::shared_ptr<Tile> tile);
private:
	//std::shared_ptr<SpriteAnimation> m_Target;
	std::shared_ptr<Entity> m_Target;
	Vector2 m_Position;
	SDL_Rect m_ViewBox;
	float m_Smooth = 5.0f;
	float m_maxCamSpeed;
};