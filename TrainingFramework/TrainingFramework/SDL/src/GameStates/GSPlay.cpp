#include "GSPlay.h"
#include "GameObject/TextureManager.h"
#include "GameObject/EnemyManager.h"
#include "GameObject/Sprite2D.h"
#include "GameObject/MouseButton.h"
#include "GameObject/SpriteAnimation.h"
#include "GameObject/Tilemap.h"

#include "GameObject/Camera.h"
#include "GSPlayStates.h"
#include "KeyState.h"
#include "Entity.h"
#include <list>
#include <fstream>
#include <algorithm>

class Bullet;
class Entity;
int pressed = 0;
GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{
	GSPlayStates::GetInstance()->m_currentPlayState = PLAYING;
	m_Sound = std::make_shared<Sound>();
	m_Sound->LoadSound("Data/Sounds/22_Slash_04.wav");
	m_levelList.insert({ 1, "Data/Textures/Dungeon2.tmx" });//Normal
	m_levelList.insert({ 2, "Data/Textures/Dungeon1.tmx" });//Ice
	m_levelList.insert({ 3, "Data/Textures/Dungeon_Poison.tmx" });//Poison
	m_levelList.insert({ 4, "Data/Textures/Room_All.tmx" });//All
	m_levelList.insert({ 5, "Data/Textures/Dungeon5.tmx" });//Boss
	auto texture = ResourceManagers::GetInstance()->GetTexture("Crosshair17.png");
	m_mouseSprite = std::make_shared<Sprite2D>( texture, SDL_FLIP_NONE);
	m_mouseSprite->SetSize(24, 24);
	// background

	texture = ResourceManagers::GetInstance()->GetTexture("GameBG.png");
	m_background = std::make_shared<Sprite2D>( texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	//Pause menu background
	texture = ResourceManagers::GetInstance()->GetTexture("Pause Menu.png");
	m_pauseMenu = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_pauseMenu->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_pauseMenu->Set2DPosition(0, 0);
	
	//Death menu background
	texture = ResourceManagers::GetInstance()->GetTexture("Deathscreen.png");
	m_deathMenu = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_deathMenu->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_deathMenu->Set2DPosition(0, 0);

	//Win menu background
	texture = ResourceManagers::GetInstance()->GetTexture("WinScreen.png");
	m_winMenu = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_winMenu->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_winMenu->Set2DPosition(0, 0);

	// button go back to menu
	texture = ResourceManagers::GetInstance()->GetTexture("Menu Button.png");
	std::shared_ptr<MouseButton> btnMenu = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnMenu->SetSize(250, 100);
	btnMenu->Set2DPosition(SCREEN_WIDTH / 2 - btnMenu->GetWidth() - 50, SCREEN_HEIDHT / 2 + 50);
	btnMenu->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_pauseMenuButtons.push_back(btnMenu);
	
	// button go back to menu 2 (for death screen)
	texture = ResourceManagers::GetInstance()->GetTexture("Menu Button.png");
	MenuButton2 = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	MenuButton2->SetSize(250, 100);
	MenuButton2->Set2DPosition(SCREEN_WIDTH / 2 - MenuButton2->GetWidth() / 2, SCREEN_HEIDHT / 2 - MenuButton2->GetHeight() /2);
	MenuButton2->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		});

	//button continue
	texture = ResourceManagers::GetInstance()->GetTexture("Resume Button.png");
	std::shared_ptr<MouseButton> btnResume = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnResume->SetSize(250, 100);
	btnResume->Set2DPosition(SCREEN_WIDTH / 2 + 50, SCREEN_HEIDHT / 2 + 50);
	btnResume->SetOnClick([]() {
		GSPlayStates::GetInstance()->m_currentPlayState = PLAYING;

		});
	m_pauseMenuButtons.push_back(btnResume);

	//button pause
	texture = ResourceManagers::GetInstance()->GetTexture("Pause Square Button.png");
	std::shared_ptr<MouseButton> btnPause = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnPause->SetSize(100, 100);
	btnPause->Set2DPosition(SCREEN_WIDTH - btnPause->GetWidth() - 10, 10);
	btnPause->SetOnClick([]() {
		GSPlayStates::GetInstance()->m_currentPlayState = PAUSED;
		});
	m_listButton.push_back(btnPause);
   // Animation 
	texture = ResourceManagers::GetInstance()->GetTexture("Actor1_2.tga");
	obj = std::make_shared<SpriteAnimation>( texture, 2, 9, 6, 0.2f);
	obj->SetFlip(SDL_FLIP_HORIZONTAL);
	obj->SetSize(40, 50);
	obj->Set2DPosition(240, 400);
	//Camera::GetInstance()->SetTarget(obj);
	m_listAnimation.push_back(obj);

	m_KeyPress = 0;

	/////////////////////////////////////////////////Player/////////////////////////////////////
	player = std::make_shared<Player>(30, 45);
	player->Init();
	m_entities.push_back(player);

	Camera::GetInstance()->SetTarget(m_entities.back());

	GenerateBullets(30);
	if (GameStateMachine::GetInstance()->newGame) LoadLevel(1); 
	else LoadGame();
	// player->SetPosition(m_tilemap->m_playerSpawnPos);

	EnemyManager::GetInstance()->m_currentGame = this;
	EnemyManager::GetInstance()->GenerateEnemies();
	//EnemyManager::GetInstance()->SpawnWaveInRoom(0);


}

void GSPlay::Exit()
{
	EnemyManager::FreeInstance();
	Tilemap::FreeInstance();
}


void GSPlay::Pause()
{

}
void GSPlay::Resume()
{
	// button close
	//auto texture = ResourceManagers::GetInstance()->GetTexture("btn_restart.tga");
	//button->SetTexture(texture);

}
void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN)// && e.key.repeat == 0) //For e.key.repeat it's because key repeat is enabled by default and if you press and hold a key it will report multiple key presses. That means we have to check if the key press is the first one because we only care when the key was first pressed.
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_KeyPress |= 1;
			break;
		case SDLK_s:
			m_KeyPress |= 1 << 1;
			break;
		case SDLK_d:
			m_KeyPress |= 1 << 2;
			break;
		case SDLK_w:
			m_KeyPress |= 1 << 3;
			break;
		case SDLK_1:
			pressed = 1;
			break;
		case SDLK_2:
			pressed = 2;
			break;
		default:
			break;
		}
	}
	////Key Up
	else if (e.type == SDL_KEYUP )//&& e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_KeyPress ^= 1;
			break;
		case SDLK_s:
			m_KeyPress ^= 1 << 1;
			break;
		case SDLK_d:
			m_KeyPress ^= 1 << 2;
			break;
		case SDLK_w:
			m_KeyPress ^= 1 << 3;
			break;
		case SDLK_1:
			pressed = 0;
			break;
		case SDLK_2:
			pressed = 0;
			break;
		default:
			break;
		}
	}
}

void GSPlay::HandleTouchEvents(SDL_Event& e)
{
	if (GSPlayStates::GetInstance()->m_currentPlayState == PLAYING) {
		for (auto button : m_listButton){
			if (button->HandleTouchEvent(&e))break;

		}
	}

	if (GSPlayStates::GetInstance()->m_currentPlayState == PAUSED) {
		for (auto button : m_pauseMenuButtons) {
			if (button->HandleTouchEvent(&e))break;
		}
	}
	
	if (GSPlayStates::GetInstance()->m_currentPlayState == DEATH || GSPlayStates::GetInstance()->m_currentPlayState == WIN) {
		(MenuButton2->HandleTouchEvent(&e));
	}

	

}

void GSPlay::HandleMouseEvents(SDL_Event& e) {

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) m_mouseClickedLEFT = true;
		else if (e.button.button == SDL_BUTTON_RIGHT) m_mouseClickedRIGHT = true;
	}

}
void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}

void GSPlay::Update(float deltaTime)
{
	if (GSPlayStates::GetInstance()->m_currentPlayState == PAUSED){
		for (auto it : m_pauseMenuButtons) it->Update(deltaTime);
		return;
	}
	if (GSPlayStates::GetInstance()->m_currentPlayState == DEATH){
		MenuButton2->Update(deltaTime);
		return;

	}if (GSPlayStates::GetInstance()->m_currentPlayState == WIN){
		MenuButton2->Update(deltaTime);
		return;
	}
	if (pressed > 0 && m_currentLevel != pressed) {
		UnloadCurrentLevel();
		LoadLevel(pressed);
		pressed = 0;
		return;
	}
	SDL_GetMouseState(&x, &y);
	x += Camera::GetInstance()->GetPosition().x;
	y += Camera::GetInstance()->GetPosition().y;
	player->MouseMove(x, y);

	if (m_mouseClickedLEFT) player->Shoot(x, y, this);
	if (m_mouseClickedRIGHT) player->GrabBullet(x, y);

	m_mouseClickedLEFT = false;
	m_mouseClickedRIGHT = false;
	player->ProcessInput(m_KeyPress);

	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	//Update
	for (auto it : m_entities) it->PreSolveVelocity(deltaTime);

	for (int i = 0; i < 1; i++) {
		//Collision n^2
		auto iit = m_entities.begin();
		while (iit != std::prev(m_entities.end(), 1)) {
			auto iit2 = iit;
			std::advance(iit2, 1);
			while (iit2 != m_entities.end()) {
				(*iit)->CheckCollision((*iit2).get());
				std::advance(iit2, 1);
			}
			std::advance(iit, 1);
		}

		//Collision against walls
		for (auto it : m_entities) Tilemap::GetInstance()->CheckCollision(it.get());
	}

	for (auto it : m_entities) it->Update(deltaTime);

	//Sorting inactive objects
	auto iit = m_entities.begin();
	while (iit != m_entities.end()) {
		if ((*iit)->IsDead()) {
			if ((*iit)->m_entityType == 1) m_inactiveBullets.push_back(*iit);
			else m_inactiveEntities.push_back(*iit);
			iit = m_entities.erase(iit);
		}
		else std::advance(iit, 1);
	}

	if (player->IsDead()) GSPlayStates::GetInstance()->m_currentPlayState = DEATH;
	//Do the same for EnemyManager
	EnemyManager::GetInstance()->ReturnInactiveEnemies();
	//Update position of camera
	Camera::GetInstance()->Update(deltaTime);
	if (isLevelChanged) NextLevel();
}

void GSPlay::Draw(SDL_Renderer* renderer)
{
	if (GSPlayStates::GetInstance()->m_currentPlayState == PAUSED) {
		m_pauseMenu->Draw(renderer);
		for (auto it : m_pauseMenuButtons) it->Draw(renderer);
		return;
	}

	if (GSPlayStates::GetInstance()->m_currentPlayState == DEATH) {
		m_deathMenu->Draw(renderer);
		MenuButton2->Draw(renderer);
		return;
	}if (GSPlayStates::GetInstance()->m_currentPlayState == WIN) {
		m_winMenu->Draw(renderer);
		MenuButton2->Draw(renderer);
		return;
	}
	m_background->Draw(renderer);
	//m_score->Draw();
	
	Tilemap::GetInstance()->Draw(renderer);

	for (auto it : m_listButton) it->Draw(renderer);
	for (auto it: m_entities) it->Draw(renderer);
	
	

	m_mouseSprite->Set2DPosition(x- Camera::GetInstance()->GetPosition().x - m_mouseSprite->GetWidth()/2, 
		y - Camera::GetInstance()->GetPosition().y-m_mouseSprite->GetHeight()/2);
	m_mouseSprite->Draw(renderer);

}
void GSPlay::AddEntity(std::shared_ptr<Entity> entity) {
	m_entities.push_back(entity);
}

void GSPlay::AddBullet(Vector3 startPos, Vector3 dir, int bulletID, int LayerID)
{
	if (m_inactiveBullets.empty()) return;
	std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(m_inactiveBullets.back());
	bullet->layerID = LayerID;
	bullet->SetPosition(startPos);
	bullet->SetMovementDirection(dir);
	bullet->SetBulletData(bulletID);
	bullet->SetActive(true);
	m_inactiveBullets.pop_back();
	m_entities.push_back(bullet);
	m_Sound->PlaySoundEffect();

}

void GSPlay::AddBullet(Vector3 startPos, Vector3 dir, BulletData bulletData, int LayerID)
{
	if (m_inactiveBullets.empty()) return;
	std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(m_inactiveBullets.back());
	bullet->layerID = LayerID;
	bullet->SetPosition(startPos);
	bullet->SetMovementDirection(dir);
	bullet->SetBulletData(bulletData);
	bullet->SetActive(true);
	m_inactiveBullets.pop_back();
	m_entities.push_back(bullet);
	m_Sound->PlaySoundEffect();

}

void GSPlay::AddBullet(float lifetime, float damage, float movementSpeed, Vector3 startPos, Vector3 dir, float width, float height, int LayerID)
{
	if (m_inactiveBullets.empty()) return;
	std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(m_inactiveBullets.back());
	bullet->layerID = LayerID;

	bullet->SetFirstSpawn(lifetime, damage, movementSpeed, startPos, dir);
	bullet->SetActive(true);
	m_inactiveBullets.pop_back();
	m_entities.push_back(bullet);
	m_Sound->PlaySoundEffect();
}

void GSPlay::NextLevel()
{
	UnloadCurrentLevel();
	LoadLevel(m_currentLevel + 1);
	isLevelChanged = false;
}

void GSPlay::GenerateBullets(int numOfBullets)
{
	for (int i = 0; i < numOfBullets; i++) {
		std::shared_ptr<Entity> bullet = std::make_shared<Bullet>();
		m_inactiveBullets.push_back(bullet);
	}
}

void GSPlay::UnloadCurrentLevel()
{
	Tilemap::GetInstance()->Free();
	auto iit = m_entities.begin();
	while (iit != m_entities.end()) {
		if ((*iit)->m_entityType == -1) {
			std::advance(iit, 1);
			continue;
		}
		//printf("deactivated \n");
		(*iit)->SetActive(false);
		m_inactiveEntities.push_back(*iit);
		iit = m_entities.erase(iit);
	}
	EnemyManager::GetInstance()->ReturnInactiveEnemies();

}

void GSPlay::LoadLevel(int level)
{
	m_currentLevel = level;
	if (level >= 6) {
		GSPlayStates::GetInstance()->m_currentPlayState = WIN;
		return;
	}
	SaveGame();

	Tilemap::GetInstance()->LoadFromTiled(m_levelList[level]);
	player->ResetCombat();
	player->SetPosition(Tilemap::GetInstance()->m_playerSpawnPos);
}

void GSPlay::LoadGame()
{
	std::ifstream myfile("Data/GameSave.txt");
	if (myfile.is_open())
	{
		int level;
		float health;
		myfile >> level;
		myfile >> health;
		myfile.close();
		m_currentLevel = level;
		player->SetInitialHealth(health);
		LoadLevel(level);
	}
}

void GSPlay::SaveGame()
{
	std::ofstream myfile;
	myfile.open("Data/GameSave.txt");
	myfile << m_currentLevel;
	myfile << '\n';
	myfile << player->GetHealth();
	myfile.close();

}

