#include "Enemy.h"
#include "GSPlay.h"
#include "EnemyManager.h"
#include "Camera.h"
class GSPlay;
void Enemy::Draw(SDL_Renderer* renderer)
{
    if (m_currentGame->player->GetPosition().x >= m_position.x) it->SetFlip(SDL_FLIP_NONE);
    else it->SetFlip(SDL_FLIP_HORIZONTAL);
    Entity::Draw(renderer);
    if (m_barContainer != nullptr) {
        Vector3 campos = Vector3(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y, 0);
        m_barContainer->SetPosition(m_position + Vector3(-m_barContainer->GetWidth() / 2, -50, 0) - campos);
        m_barFill->SetPosition(m_position + Vector3(-m_barContainer->GetWidth() / 2, -50, 0) - campos);

        m_barContainer->Draw(renderer);
        m_barFill->Draw(renderer);
    }

}
void Enemy::Init() {
    Entity::Init();
    m_Sound = std::make_shared<Sound>();
    m_Sound->LoadSound("Data/Sounds/Hurt.wav");
    m_entityType = 0;
    m_shootCooldown = 2.0f;
    m_currentCooldown = m_shootCooldown;
    m_updatePositionCooldown = 3.0f;
    layerID = 2;
    m_isActive = true;
    m_movementSpeed = 50;
   /* m_health = 100;
    m_movementSpeed = 50;
    m_position.x = 300;
    m_position.y = 500;*/
    m_moveDirection = Vector3(0, 0, 0);

    auto texture = ResourceManagers::GetInstance()->GetTexture("bar_container.png");
    m_barContainer = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);

    texture = ResourceManagers::GetInstance()->GetTexture("bar_fill.png");
    m_barFill = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);

    m_barContainer->SetSize(60, 8);
    m_barFill->SetSize(60, 8);
}

void Enemy::Shoot(float x, float y, GSPlay* currentGame) {
    /*printf("%f % f \n", x - m_position.x, y - m_position.y);
    currentGame->AddBullet(m_position, Vector3(x - m_position.x, y - m_position.y, 0), m_bulletShootID, layerID);*/
    Vector3 dir = currentGame->player->GetPosition() - m_position;
    currentGame->AddBullet(m_position, dir, m_bulletShootID, layerID);
}

void Enemy::Update(float deltaTime) {
    Entity::Update(deltaTime);

    //if (m_health <= 0) SetActive(false);
    if (m_currentFrozenCooldown > 0) {
        m_moveDirection = Vector3(0, 0, 0);
        return;
    }
    if (abs(m_moveDirection.x) + abs(m_moveDirection.y) > 0) it = obj2;
    else it = obj;

    if (m_currentCooldown > 0) m_currentCooldown -= deltaTime;
    if (m_currentPositionTime > 0)m_currentPositionTime -= deltaTime;

    if (m_currentPositionTime <= 0) {
        Moving(m_currentGame->player);
        m_currentPositionTime = m_updatePositionCooldown;
    }

    if (m_currentCooldown <= 0) {
        Shoot(m_position.x + 1, m_position.y, m_currentGame);
        m_currentCooldown = m_shootCooldown;
    }
    //
}

void Enemy::SetActive(bool active)
{
    m_isActive = active;
    if (active) m_currentFrozenCooldown = 0;
}

void Enemy::SetAnim(const std::string& dir1, const std::string& dir2, int frames)
{
    CreateAnim(obj, dir1, frames);
    CreateAnim(obj2, dir2, frames);
    it = obj;
}

void Enemy::Moving(std::shared_ptr<Player>player) {
    float dx = player->GetPosition().x - m_position.x;
    float dy = player->GetPosition().y - m_position.y;
    float distance = sqrt(dx * dx + dy * dy);
    dx = dx / distance;
    dy = dy / distance;
    m_moveDirection = Vector3(dx, dy, 0);

}

void Enemy::SetSize(float width, float height)
{
    Entity::SetSize(width, height);
    obj->SetSize(width, height);
    obj2->SetSize(width, height);
    

}

void Enemy::SetHealth(float healthChange)
{
    Entity::SetHealth(healthChange);
    m_Sound->PlaySoundEffect();
}
