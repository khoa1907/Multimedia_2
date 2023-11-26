#pragma once
#include "GameStateBase.h"
#include "GameObject/MouseButton.h"
#include "Sound.h"
#include "GameObject/Player.h"
#include "GameObject/Enemy.h"
#include "Bullet.h"

class Sprite2D;
class SpriteAnimation;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void	Init() override;
	void	Exit() override;

	void	Pause() override;
	void	Resume() override;

	void	HandleEvents() override;
	void	HandleKeyEvents(SDL_Event& e) override;
	void	HandleTouchEvents(SDL_Event& e) override;
	void	HandleMouseEvents(SDL_Event& e) override;

	void	HandleMouseMoveEvents(int x, int y) override;
	void	Update(float deltaTime) override;
	void	Draw(SDL_Renderer* renderer) override;
	int m_KeyPress;


	void AddEntity(std::shared_ptr<Entity > entity);
	void AddBullet(Vector3 startPos, Vector3 dir, int bulletID, int LayerID);
	void AddBullet(Vector3 startPos, Vector3 dir, BulletData bulletData, int LayerID);
	void AddBullet(float lifetime, float damage, float movementSpeed, Vector3 startPos, Vector3 dir, float width, float height, int LayerID);
	void NextLevel();
	void SetDeath();
	bool isLevelChanged = false;
	std::shared_ptr<Player> player;


private:
	std::map<int, std::string> m_levelList;
	std::shared_ptr<Sprite2D>	m_background;
	std::shared_ptr<Sprite2D>	m_mouseSprite;
	std::shared_ptr<Sprite2D>	m_pauseMenu;
	std::shared_ptr<Sprite2D>	m_deathMenu;
	std::shared_ptr<Sprite2D>	m_winMenu;

	//std::shared_ptr<Text>		m_score;
	std::list<std::shared_ptr<MouseButton>>	m_listButton;
	std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;
	std::shared_ptr<SpriteAnimation> obj;
	std::shared_ptr<MouseButton> button;
	std::shared_ptr<MouseButton> MenuButton2;
	std::list<std::shared_ptr<MouseButton>> m_pauseMenuButtons;


	std::list<std::shared_ptr<Entity>> m_entities;
	std::list<std::shared_ptr<Entity>> m_inactiveEntities;

	std::list<std::shared_ptr<Entity>> m_inactiveBullets;
	std::shared_ptr<Sound> m_Sound;


	void GenerateBullets(int numOfBullets);
	void UnloadCurrentLevel();
	void LoadLevel(int level);
	int m_currentLevel;
	
	float time = 0.0f;
	float m_Velocity = 10.0f;
	std::shared_ptr<Enemy> enemy;

	bool m_mouseClickedLEFT = false;
	bool m_mouseClickedRIGHT = false;
	bool m_mouseMoved = false;
	int m_playState;

	int x = 0;
	int y = 0;

	void LoadGame();
	void SaveGame();

	
};

