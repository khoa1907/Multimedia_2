#include "Entity.h"
#include <EnemyManager.h>
#include <Tilemap.h>
#include "Camera.h"

void Entity::Update(float deltaTime) {
    //PostSolveVelocity();
    SetPosition(m_position + m_moveVelocity);

    if (m_poisonTicks > 0) {
        if (m_currentPoisonCooldown > 0) m_currentPoisonCooldown -= deltaTime;
        if (m_currentPoisonCooldown <= 0){
            m_poisonTicks--;
            m_currentPoisonCooldown = m_poisonCooldown - m_poisonResistance;
            SetHealth(-m_poisonDamage);
        }
    }
    if (m_currentFrozenCooldown > 0) {
        m_currentFrozenCooldown -= deltaTime;
        return;
    }
    it->Update(deltaTime);

}
void Entity::Init()
{
    SDL_Color color = { 0, 0, 255 };
    m_frozenText = std::make_shared<Text>("Data/impact.ttf", color);
    m_frozenText->LoadFromRenderText("FROZEN!");

    color = { 0, 255, 0 };
    m_poisonText = std::make_shared<Text>("Data/impact.ttf", color);
    m_poisonText->LoadFromRenderText("POISONED!");

    m_frozenText->SetSize(80, 20);
    m_poisonText->SetSize(80, 20);
}
void Entity::CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, std::shared_ptr<TextureManager>& texture, int frames) {
    CreateAnim(tempObj, texture, frames, 0.2f);
}
void Entity::CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, const std::string& txtLink, int frames)
{
    auto texture = ResourceManagers::GetInstance()->GetTexture(txtLink);
    CreateAnim(tempObj, texture, frames);
}
void Entity::CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, std::shared_ptr<TextureManager>& texture, int frames, float speed)
{
    tempObj = std::make_shared<SpriteAnimation>(texture, 1, frames, 1, speed);
    tempObj->SetFlip(SDL_FLIP_NONE);
    tempObj->SetSize(m_width, m_height);
}
void Entity::CreateAnim(std::shared_ptr<SpriteAnimation>& tempObj, const char* txtLink, int frames, float speed)
{
    auto texture = ResourceManagers::GetInstance()->GetTexture(txtLink);
    CreateAnim(tempObj, texture, frames, speed);
}
void Entity::Draw(SDL_Renderer* renderer) {

    if (m_currentFrozenCooldown > 0) {
        m_frozenText->Set2DPosition(m_position.x - m_frozenText->GetWidth() / 2 - Camera::GetInstance()->GetPosition().x,
            m_position.y - m_frozenText->GetHeight() / 2 - Camera::GetInstance()->GetPosition().y + m_height / 2 + 10);
        m_frozenText->Draw(renderer);
    }
    if (m_poisonTicks > 0) {
        m_poisonText->Set2DPosition(m_position.x - m_poisonText->GetWidth() / 2 - Camera::GetInstance()->GetPosition().x,
            m_position.y - m_poisonText->GetHeight() / 2 - Camera::GetInstance()->GetPosition().y + m_height / 2 + 30);
        m_poisonText->Draw(renderer);
    }
    if (it != nullptr) {
        it->Set2DPosition(m_position.x - it->GetWidth() / 2, m_position.y - it->GetHeight() / 2);
        it->Draw(renderer);
    }

   /* SDL_Rect debugBoxRect = { m_position.x - m_width / 2 - Camera::GetInstance()->GetPosition().x, m_position.y - m_height / 2 - Camera::GetInstance()->GetPosition().y, m_width, m_height };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(renderer, &debugBoxRect);*/
}
Vector2 Entity::GetSize()
{
    return Vector2(m_width, m_height);
}
Vector3 Entity::GetPosition()
{
    return m_position;
}
Vector3 Entity::GetVelocity()
{
    return m_moveVelocity;
}
float Entity::GetHealth()
{
    return m_health;
}
void Entity::ProcessCollision(Entity* other, SDL_FRect* intersect)
{
	otherVelocity = other->GetVelocity();
	Vector3 relativeVelocity = m_moveVelocity - otherVelocity;
	if (abs(relativeVelocity.x) / intersect->w > abs(relativeVelocity.y) / intersect->h) {
		relativePushX = intersect->w + 2;
		relativePushY = intersect->w * relativeVelocity.y / relativeVelocity.x;
	}
	else {
		relativePushY = intersect->h + 2;
		relativePushX = intersect->h * relativeVelocity.x / relativeVelocity.y;
	}

    if (relativeVelocity.x < 0) relativePushX = -abs(relativePushX);
    else relativePushX = abs(relativePushX); 
    
    if (relativeVelocity.y < 0) relativePushY = -abs(relativePushY);
    else relativePushY = abs(relativePushY);

	relativePushX /= 2;
	relativePushY /= 2;
}
void Entity::ProcessCollision(SDL_FRect* intersect)
{
    Vector3 relativeVelocity = m_moveVelocity;
    if (m_moveVelocity.x == 0 && m_moveVelocity.y == 0) return;
    float iW = abs(intersect->w);
    float iH = abs(intersect->h);
    if (abs(relativeVelocity.x) / iW > abs(relativeVelocity.y) / iH) {
        m_touchXWall = false;
        relativePushX = iW + 2;
        relativePushY = iW * abs(relativeVelocity.y / relativeVelocity.x);
    }
    else {
        m_touchXWall = true;
        relativePushY = iH + 2;
        relativePushX = iH * abs(relativeVelocity.x / relativeVelocity.y);
    }

    if (relativeVelocity.x < 0) relativePushX = -abs(relativePushX);
    else relativePushX = abs(relativePushX);

    if (relativeVelocity.y < 0) relativePushY = -abs(relativePushY);
    else relativePushY = abs(relativePushY);

    //printf("%f and %f with %f h %f layerid: %d\n", relativePushX, relativePushY, m_moveVelocity.x , m_moveVelocity.y, layerID);
}
void Entity::ProcessCollision(Entity* other)
{
}
void Entity::ProcessCollision()
{
}
void Entity::CheckCollision(Entity* other)
{
    if (other->layerID == this->layerID //if same layer
        && ((other->m_entityType != m_entityType) || (other->m_entityType == 1 && m_entityType == 1))//and different entity type
        ) return;
    /*SDL_FRect debugBoxRect = { m_position.x - m_width / 2, m_position.y - m_height / 2, m_width, m_height };
    SDL_FRect otherDebugRect = { other->GetPosition().x - other->GetSize().x / 2, other->GetPosition().y - other->GetSize().y / 2, other->GetSize().x, other->GetSize().y };
    SDL_FRect result;*/

    SDL_FRect debugBoxRect = { m_position.x - m_width / 2 + m_moveVelocity.x , m_position.y - m_height / 2 + m_moveVelocity.y, m_width, m_height };
    SDL_FRect otherDebugRect =
    { other->GetPosition().x - other->GetSize().x / 2 + other->GetVelocity().x,
      other->GetPosition().y - other->GetSize().y / 2 + other->GetVelocity().y,
      other->GetSize().x, other->GetSize().y };
    SDL_FRect result;


    if (SDL_IntersectFRect(&debugBoxRect, &otherDebugRect, &result)) {
        if (m_entityType == 1 || other->m_entityType != 1) ProcessCollision(other, &result);
        if (m_entityType != 1 || other->m_entityType == 1)other->ProcessCollision(this, &result);
        PostSolveVelocity();
        other->PostSolveVelocity();
    }
}
void Entity::CheckCollision(Tile* other)
{
    Vector3 dis = Vector3(other->GetPosition().x + other->GetWidth() / 2, other->GetPosition().y + other->GetHeight() / 2, 0) - m_position;
    float dis2 = dis.x * dis.x + dis.y * dis.y;
    if (dis2 > m_height * m_height * 2) return;

    SDL_FRect debugBoxRect = { m_position.x - m_width / 2 + m_moveVelocity.x , m_position.y - m_height / 2 + m_moveVelocity.y, m_width, m_height };
    SDL_FRect otherDebugRect = { other->GetPosition().x, other->GetPosition().y, other->GetWidth(), other->GetHeight()};
    SDL_FRect result;

    if (SDL_IntersectFRect(&debugBoxRect, &otherDebugRect, &result)) {
        if (other->m_tileType == -100 && m_entityType == -1) {
            //printf("change level");
            EnemyManager::GetInstance()->ChangeLevel();
            Tilemap::GetInstance()->SetTriggerActive(false, other->m_tileType);
            return;
        }
        if (other->m_tileType >= 0 && m_entityType == -1) {
            EnemyManager::GetInstance()->SpawnWaveInRoom(other->m_tileType - 1);
            Tilemap::GetInstance()->SetTriggerActive(false, other->m_tileType);
            Tilemap::GetInstance()->SetDoorActive(false);
            return;
        }
        ProcessCollision(&result);
        PostSolveVelocity();
    }
}
void Entity::SetHealth(float healthChange)
{
    m_health += healthChange;
    if (m_health <= 0)
    {
        printf("HEALTH IS 0 \n");
        SetActive(false);
    }
    //m_health = std::min(m_health, m_maxHealth);

    if (m_barFill != NULL) m_barFill->SetSize(m_barContainer->GetWidth() * m_health / m_maxHealth, m_barContainer->GetHeight());
}
void Entity::SetInitialHealth(float health)
{
    m_maxHealth = health;
    m_health = health;
    m_poisonTicks = 0;
    m_currentPoisonCooldown = 0;
    m_currentFrozenCooldown = 0;
    SetHealth(0);
}
bool Entity::IsDead() {
    return (!m_isActive);
}
void Entity::SetActive(bool isActive) {
    m_isActive = isActive;
}
void Entity::SetMovementSpeed(float movementSpeed)
{
    m_movementSpeed = movementSpeed;
}
void Entity::PostSolveVelocity()
{
    //SetPosition(Vector3(m_position.x - relativePushX, m_position.y - relativePushY, 0) + otherVelocity);*/
    m_moveVelocity += -Vector3(relativePushX, relativePushY, 0) + otherVelocity;
    //printf("current velocity after solve %f %f layerID: %d \n", m_moveVelocity.x, m_moveVelocity.y, layerID);
    relativePushX = 0;
    relativePushY = 0;

}
void Entity::PreSolveVelocity(float deltaTime)
{
    relativePushX = 0;
    relativePushY = 0;
    otherVelocity = Vector3(0, 0, 0);
    m_moveVelocity = m_moveDirection * deltaTime * m_movementSpeed;
}
void Entity::SetAnim(const std::string& dir1, const std::string& dir2, int frames)
{
    CreateAnim(it, dir1, frames);
}
void Entity::SetStatusEffect(int statusType)
{
    printf("Status %d\n", statusType);
    if ((statusType >> 1) & 1) SetFrozen();
    if (statusType & 1) SetPoisoned();
}
void Entity::SetFrozen()
{
    m_currentFrozenCooldown = m_frozenCooldown - m_frozenResistance;
    printf("frozen \n");
}
void Entity::SetPoisoned()
{
    printf("POISONED\n");
    m_currentPoisonCooldown = m_poisonCooldown;
    m_poisonTicks = 5;
}
void Entity::SetSize(float width, float height) {
    m_width = width;
    m_height = height;
    if (it != nullptr) {
        it->SetSize(width, height);
    }
}
