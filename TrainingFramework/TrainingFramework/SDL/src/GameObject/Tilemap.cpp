#include "Tilemap.h"
#include "ResourceManagers.h"
#include "EnemyManager.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
void Tilemap::LoadFromTiled(const std::string& path) {
    m_startingPos = Vector3(0, 0, 0);

    // Load and parse the Tiled map with tmxlite
    tmx::Map tiled_map;
    tiled_map.load(path);

    // We need to know the size of the map (in tiles)
    auto map_dimensions = tiled_map.getTileCount();
    int rows = map_dimensions.y;
    int cols = map_dimensions.x;
    //printf("%d %d \n", rows, cols);
    // We also need to know the dimensions of the tiles.
    auto tilesize = tiled_map.getTileSize();
    int tile_width = tilesize.x;
    int tile_height = tilesize.y;
    //printf("%d \n", tile_width);

    // Load all of the tilesets and store them in a data structure.
    // I chose to store them in a map.
    auto& map_tilesets = tiled_map.getTilesets();
    for (auto& tset : map_tilesets) {
        //auto tex = ResourceManagers::GetInstance()->GetTexture(tset.getImagePath());
        auto tex = ResourceManagers::GetInstance()->GetTexture("2D Pixel Dungeon Asset Pack/Dungeon_Tileset_at.png");
        tilesets.insert(std::pair<int, std::shared_ptr<TextureManager>>(tset.getFirstGID(), tex));
    }

    // This is the hard part; iterate through each layer in the map,
    // poke each tile for the information you need, and store it in
    // our tiles data structure. 
    //
    // We start at the bottom most layer, and work our way up with this
    // outer for-loop.
    int collisionType = 0;
    int cnt = 0;
    auto& map_layers = tiled_map.getLayers();
    int numOfMisc = (map_layers.size() - 3) / 2;
    int doorLayer = (map_layers.size() - 2);
    EnemyManager::GetInstance()->m_maxRoom = numOfMisc - 1;

    //Layer positions
    //0: Ground-Background
    //1: Player spawn position
    //k: 2 -> n: k - 1 room spawn location
    //n + 1: blank
    //kk: n + 2 -> 2n: kk - n - 1 room spawn trigger
    // 2n + 1: Door
    // 2n + 2: Collisions 
    // Total of ((2n + 3) - 3) / 2 special tiles 

    for (auto& layer : map_layers) {
        // We're only looking to render the tiles on the map, so if
        // this layer isn't a tile layer, we'll move on.
        if (layer->getType() != tmx::Layer::Type::Tile) {
            continue;
        }

        auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());

        // Grab all of this layer's tiles. 
        auto& layer_tiles = tile_layer->getTiles();

        Vector3 currentP = m_startingPos;
        if (cnt >= 1 + numOfMisc) collisionType = 1;

        for (auto y = 0; y < rows; ++y) {
            for (auto x = 0; x < cols; ++x) {
               
                auto tile_index = x + (y * cols);
                auto cur_gid = layer_tiles[tile_index].ID;

                // If the GID is 0, that means it's an empty tile,
                // we don't want to waste time on nothing, nor do we
                // want to store an empty tile.
                if (cur_gid == 0) {
                    currentP.x += 32;
                    continue;
                }
               
                // Find the tileset that this tile belongs to. The tileset
                // whose first GID is less-than-or-equal to this GID, is
                // the tileset our GID belongs to. A tileset with a first
                // GID that is greater than this tile means that this tile
                // does not belong to that tileset.
                auto tset_gid = -1;
                for (auto& ts : tilesets) {
                    if (ts.first <= cur_gid) {
                        tset_gid = ts.first;
                        break;
                    }
                }
                // If we didn't find a valid tileset, skip the tile. We can't
                // render it if we don't have a tileset to pull from.
                if (tset_gid == -1) {
                    //currentP.x += 32;
                    continue;
                }
                if (cnt == 1) {
                    m_playerSpawnPos = currentP;
                    currentP.x += 32;
                    continue;
                }
                if (cnt > 1 && cnt <= numOfMisc) {//spawn tiles
                    //1: Normal monster
                    //2: Normal ghost
                    //3: Ice sniper
                    //4: Ice robot
                    //5: Poison skull
                    //printf("lol %d %d \n", cnt - 2, cur_gid);
                    EnemyManager::GetInstance()->m_spawnPos[cnt - 2].push_back(std::make_pair(Vector2(currentP.x, currentP.y), cur_gid - 1));
                    currentP.x += 32;
                    //printf("AFTER\n");
                    continue;
                }
                // Normalize the GID.
                cur_gid -= tset_gid;
                // Find the dimensions of the tile sheet. This is important,
                // because our tile is only a small sprite on that sheet, not
                // the whole sheet.
                auto ts_width = 512;
                auto ts_height = 512;

                // Calculate the area on the tilesheet to draw from.
                auto region_x = (cur_gid % (ts_width / tile_width)) * tile_width;
                auto region_y = (cur_gid / (ts_width / tile_width)) * tile_height;
                Vector2 region = Vector2(region_x, region_y);
                Vector2 size = Vector2(tile_width, tile_height);

            
                std::shared_ptr<Tile> tempTile = std::make_shared<Tile>(tilesets[tset_gid], SDL_FLIP_NONE, currentP.x, currentP.y, region, size, collisionType);
                //Tile* tempTile = new Tile(tilesets[tset_gid], SDL_FLIP_NONE, currentP.x, currentP.y, region, size, collisionType);
                
                if (cnt == numOfMisc + 1) {//level end tiles
                    tempTile->m_tileType = -100;
                   
                    m_triggerTiles.push_back(tempTile);
                }
                else if (cnt > numOfMisc + 1 && cnt <= numOfMisc * 2) {//trigger tiles
                    tempTile->m_tileType = cnt - numOfMisc - 1;
                    m_triggerTiles.push_back(tempTile);
                    currentP.x += 32;
                    continue;
                }
               
                else if (cnt == doorLayer) {//door tiles
                    tempTile->m_tileType = -2;
                    tempTile->SetType(-1);
                    m_doorTiles.push_back(tempTile);
                }
                else if (cnt == doorLayer + 1)
                {
                    tempTile->m_tileType = -1;
                    m_collisionTiles.push_back(tempTile);
                }
                
                m_tiles.push_back(tempTile);
                currentP.x += 32;
            }
            currentP.y += 32;
            currentP.x = m_startingPos.x;
        }
        cnt++;

    }
}
void Tilemap::GenerateTiles()
{
    LoadFromTiled("Data/Textures/Dungeon2.tmx");
}

void Tilemap::Draw(SDL_Renderer* renderer)
{
	for (auto it : m_tiles) {
		if (it->GetType() >= 0) it->Draw(renderer);
	}
}

void Tilemap::CheckCollision(Entity* entity)
{
	for (auto it : m_collisionTiles) entity->CheckCollision(it.get());
    for (auto it : m_triggerTiles) {
        if (it->GetType() > 0) {
            entity->CheckCollision(it.get());
        }
    }
    for (auto it : m_doorTiles) {
        if (it->GetType() > 0) entity->CheckCollision(it.get());
    }
}

void Tilemap::Free()
{
    for (auto it : m_tiles) it.reset();
    for (auto it : m_collisionTiles) it.reset();
    for (auto it : m_doorTiles) it.reset();
    for (auto it : m_triggerTiles) it.reset();
    
    while (!m_tiles.empty()) m_tiles.pop_back();
    while (!m_collisionTiles.empty()) m_collisionTiles.pop_back();
    while (!m_triggerTiles.empty()) m_triggerTiles.pop_back();
    while (!m_doorTiles.empty()) m_doorTiles.pop_back();
    for (auto it : m_tiles) {
        //printf("exists \n");
    }
    //printf("numtiles left %d \n", m_tiles.size());
}
void Tilemap::SetDoorActive(bool isClosed)
{
    for (auto it : m_doorTiles) {
        if (isClosed) it->SetType(-1);
        else it->SetType(1);
        //printf("%d \n", it->GetType());
    }
}

void Tilemap::SetTriggerActive(bool isActive,int tileType)
{
    for (auto it : m_triggerTiles) {
        if (it->m_tileType == tileType)
        {
            it->SetType(isActive);
        }
        //printf("%d %d\n", it->m_tileType, it->GetType());

    }
}

