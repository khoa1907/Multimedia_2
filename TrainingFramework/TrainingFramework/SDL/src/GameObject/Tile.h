#pragma once
#include <Sprite2D.h>
const int TILE_SIZE = 32;
class Tile : public Sprite2D {
private:
	SDL_Rect box;
	int m_type;
	Vector2 m_spritePos;
	Vector2 m_spriteSize;
	
public:
	int m_tileType;

	Tile(std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip, int x, int y, Vector2 spritePos, Vector2 spriteSize, int ttype) 
	{
		m_tileType = 0;
		m_pTexture = texture;
		m_flip = flip;
		m_position.x = x;
		m_position.y = y;
		m_type = ttype;
		m_iWidth = TILE_SIZE;
		m_iHeight = TILE_SIZE;
		m_spritePos = spritePos;
		m_spriteSize = spriteSize;
		SetSize(TILE_SIZE, TILE_SIZE);
	}
	void Draw(SDL_Renderer* renderer) override;
	int GetType();
	void SetType(int type) { m_type = type; }
	SDL_Rect GetBox();
};