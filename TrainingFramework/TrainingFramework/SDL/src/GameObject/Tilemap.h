#pragma once
#include <CMath.h>
#include "Tile.h"
#include "Entity.h"
#include "GameManager/Singleton.h"
#include <list>
class Tilemap: public CSingleton <Tilemap> {
private:
	std::map<int, std::shared_ptr<TextureManager>> tilesets;

public:
	Vector3 m_startingPos;
	Vector3 m_playerSpawnPos;

	std::list<std::shared_ptr<Tile>> m_tiles;
	std::list<std::shared_ptr<Tile>> m_collisionTiles;
	std::list<std::shared_ptr<Tile>> m_doorTiles;
	std::list<std::shared_ptr<Tile>> m_triggerTiles;
	std::list<Vector2> m_spawnTiles[2];
	int m_map[20][20];

	void LoadFromTiled(const std::string& path);
	void GenerateTiles();
	void Draw(SDL_Renderer* renderer);
	void CheckCollision(Entity* entity);
	void Free();
	void SetDoorActive(bool isClosed);
	void SetTriggerActive(bool isActive, int tileType);
};