#include "Tile.h"
#include "TextureManager.h"


void Tile::Draw(SDL_Renderer* renderer)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Render(m_position.x, m_position.y,  m_iWidth, m_iHeight,m_spritePos, m_spriteSize, m_angle, m_flip);
		//printf("rendered \n");
	}
	/*SDL_Rect debugBoxRect = { m_position.x, m_position.y, m_iWidth, m_iHeight };
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &debugBoxRect);*/
}
int Tile::GetType()
{
    return m_type;
}

SDL_Rect Tile::GetBox()
{
    return box;
}