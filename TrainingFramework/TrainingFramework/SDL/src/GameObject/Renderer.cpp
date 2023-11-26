#include"Renderer.h"
#include "Define.h"
Renderer::Renderer()
{
	gWindow = NULL;
	gRenderer = NULL;
	gFont = NULL;
}
	
Renderer::~Renderer()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
}
bool Renderer::Init()
{
	//Initialization flag
	bool success = true;
	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}
	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIDHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create renderer for window ( tao doi tuong renderer de co the render on window)
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		}
	}
	return success;
}
SDL_Window* Renderer::GetWindow()
{
	return gWindow;
}
SDL_Renderer* Renderer::GetRenderer()
{
	return gRenderer;
}
void Renderer::SetWindow(SDL_Window* mWindow)
{
	gWindow = mWindow;
}

void Renderer::SetRenderer(SDL_Renderer* mRenderer)
{
	gRenderer = mRenderer;
}
