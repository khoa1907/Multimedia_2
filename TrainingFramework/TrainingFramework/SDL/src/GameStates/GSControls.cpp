#include "GSControls.h"
GSControls::GSControls() : GameStateBase(StateType::STATE_CONTROLS)
{
}


GSControls::~GSControls()
{
}
void GSControls::Init()
{
	GameStateMachine::GetInstance()->currentTutScreen = 0;
	auto texture = ResourceManagers::GetInstance()->GetTexture("GameBG.png");

	// background
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	//Buttons
	texture = ResourceManagers::GetInstance()->GetTexture("Next Square Button.png");
	std::shared_ptr<MouseButton> btnNext = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);

	btnNext->SetSize(100, 100);
	btnNext->Set2DPosition(SCREEN_WIDTH - btnNext->GetWidth() / 2 - 100, SCREEN_HEIDHT - btnNext->GetHeight() / 2 - 100);
	btnNext->SetOnClick([]() {
		GameStateMachine::GetInstance()->currentTutScreen = std::min(7, ++GameStateMachine::GetInstance()->currentTutScreen);
		});
	m_listButton.push_back(btnNext);
	
	///////////////////////////////////////////////////////
	texture = ResourceManagers::GetInstance()->GetTexture("Back Square Button.png");
	std::shared_ptr<MouseButton> btnBack = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);

	btnBack->SetSize(100, 100);
	btnBack->Set2DPosition(100, SCREEN_HEIDHT - btnNext->GetHeight() / 2 - 100);
	btnBack->SetOnClick([]() {
		GameStateMachine::GetInstance()->currentTutScreen = std::max(0, --GameStateMachine::GetInstance()->currentTutScreen);
		});
	m_listButton.push_back(btnBack);

	////////////////////////////////////////////////////////
	texture = ResourceManagers::GetInstance()->GetTexture("Menu Button.png");
	std::shared_ptr<MouseButton> btnMenu = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnMenu->SetSize(250, 100);
	btnMenu->Set2DPosition(SCREEN_WIDTH - btnMenu->GetWidth() - 50, 10);
	btnMenu->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(btnMenu);
	/////////////////////////////////////////////////////////
	//Images
	SetImage("WASD.png", 798, 702, 0);
	SetImage("Shoot.png", 863, 579, 1);
	SetImage("Absorb.png", 663, 391, 2);
	SetImage("Firebacl.png", 658, 548, 3);
	SetImage("Big.png", 820, 635, 4);
	SetImage("Freeze.png", 848, 471, 5);
	SetImage("Poison.png", 868, 506, 6);
	SetImage("Return.png", 939, 644, 7);

}

void GSControls::Exit()
{
	ResourceManagers::FreeInstance();
}


void GSControls::Pause()
{

}

void GSControls::Resume()
{
}


void GSControls::HandleEvents()
{
}

void GSControls::HandleKeyEvents(SDL_Event& e)
{
}
void GSControls::HandleMouseEvents(SDL_Event& e)
{
}

void GSControls::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSControls::HandleMouseMoveEvents(int x, int y)
{
}

void GSControls::Update(float deltaTime)
{
	m_background->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSControls::Draw(SDL_Renderer* renderer)
{
	m_background->Draw(renderer);
	int i = 0;
	
	m_tutorialImages[GameStateMachine::GetInstance()->currentTutScreen]->Draw(renderer);
	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}
}

void GSControls::SetImage(std::string path, float sizeX, float sizeY, int index)
{
	auto texture = ResourceManagers::GetInstance()->GetTexture(path);
	m_tutorialImages[index] = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_tutorialImages[index]->SetSize(sizeX, sizeY);
	m_tutorialImages[index]->Set2DPosition(SCREEN_WIDTH / 2 - m_tutorialImages[index]->GetWidth() / 2, SCREEN_HEIDHT / 2 - m_tutorialImages[index]->GetHeight() / 2);
}
