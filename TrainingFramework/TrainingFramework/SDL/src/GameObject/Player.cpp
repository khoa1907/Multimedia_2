#include "Player.h"
#include "Bullet.h"
#include "GSPlay.h"
class Bullet;
class GSPlay;
float disX;
float disY;
void Player::MouseMove(int x, int y) {

    //Hand animation
    if (m_currentFrozenCooldown <= 0) {
        disX = x - m_position.x;
        disY = y - m_position.y;
    }

    float norm = sqrt(disX * disX + disY * disY);
    if (norm != 0) {
        disX /= norm;
        disY /= norm;
    }
    grabHand->Set2DPosition(m_position.x + disX * m_grabStats.grabDistance - grabHand->GetWidth()/2,
        m_position.y + disY * m_grabStats.grabDistance- grabHand->GetHeight() / 2);

    grabHand->SetRotation(atan2(disY, disX) * 180 / 3.14259 - 180);


    //Actual important updates
    if (m_currentFrozenCooldown > 0) return;
    if (x >= m_position.x) {
        it->SetFlip(SDL_FLIP_NONE);
        grabHand->SetFlip(SDL_FLIP_VERTICAL);
    }
    else {
        it->SetFlip(SDL_FLIP_HORIZONTAL);
        grabHand->SetFlip(SDL_FLIP_NONE);
    }
}

void Player::Init() {
    m_Sound = std::make_shared<Sound>();
    m_Sound->LoadSound("Data/Sounds/PlayerHurt.wav");   

    m_GrabSound = std::make_shared<Sound>();
    m_GrabSound->LoadSound("Data/Sounds/Retro Blop 18.wav");
    SetInitialHealth(440);

    m_frozenResistance = 2.0f;
    m_poisonResistance = 1.0f;
    Entity::Init();
    //Everything about shooting and grabbing
    ResetCombat();
    m_bulletStats.bulletSpriteDir = "BigPurpleRR2.png";
    m_bulletStats.frames = 1;

    //Grab stats
    SetGrabStats({ 25.0f, Vector2(50,50), Vector2(0,0) });

    //ID
    m_entityType = -1;
    layerID = 1;

    //Others
    m_isActive = true;
    m_movementSpeed = 200;
    SetPosition(240, 400);

    CreateAnim(obj, "Grandpa-idle.png", 4);
    CreateAnim(obj2, "Grandpa-walk.png", 4);
    it = obj;
    CreateAnim(grabAnim, "spin.png", 3, 0.05f);
    CreateAnim(grabHand, "thumb.png", 1);

    grabHand->SetSize(20, 20);
    grabAnim->SetSize(m_grabStats.grabSize.x, m_grabStats.grabSize.y);

    //Health

    auto texture = ResourceManagers::GetInstance()->GetTexture("bar_container.png");
    m_barContainer = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);

    texture = ResourceManagers::GetInstance()->GetTexture("bar_fill.png");
    m_barFill = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);

    m_barContainer->SetSize(300, 30);
    m_barContainer->Set2DPosition(20, 35);
    m_barFill->SetSize(300, 30);
    m_barFill->Set2DPosition(50, 35);
}
void Player::ProcessInput(int keyPress) {
    if (m_currentFrozenCooldown > 0) return;

    m_moveDirection.y = -((keyPress >> 3) & 1) + ((keyPress >> 1) & 1);
    m_moveDirection.x = ((keyPress >> 2) & 1) - (keyPress & 1);

    float norm = sqrt(abs(m_moveDirection.y) + abs(m_moveDirection.x));
    if (norm != 0) {
        m_moveDirection.y /= norm;
        m_moveDirection.x /= norm;
        it = obj2;
    }
    else it = obj;
    //printf("%f", norm);
}
void Player::Shoot(int x, int y, GSPlay* currentGame) {
    if (canShoot && m_currentFrozenCooldown <= 0) {
        //printf("\n damage: %f \n", m_bulletStats.damage);
       // currentGame->AddBullet(1.0f, 10.0f * m_bulletStats.damage, 200, m_position, Vector3(x - m_position.x, y - m_position.y, 0), 16 * m_bulletStats.sizeX, 16 * m_bulletStats.sizeY, layerID);
        currentGame->AddBullet (m_position, Vector3(x - m_position.x, y - m_position.y, 0), m_bulletStats, layerID);
        //printf("size %f %f \n", m_bulletStats.sizeX, m_bulletStats.sizeY);
        m_bulletStats.damage = 0;
        m_bulletStats.sizeX = 0;
        m_bulletStats.sizeY = 0;
        m_bulletStats.bulletType = 0;
        m_bulletStats.speed = 0;
        canShoot = false;
    }
}

void Player::CheckCollision(Entity* other)
{
    if (isGrabbing && other->m_entityType == 1 && other->layerID != layerID) {
        SDL_FRect debugBoxRect = { m_grabStats.grabPos.x - m_grabStats.grabSize.x / 2, m_grabStats.grabPos.y - m_grabStats.grabSize.y / 2, m_grabStats.grabSize.x,  m_grabStats.grabSize.y };
        SDL_FRect otherDebugRect =
        { other->GetPosition().x - other->GetSize().x / 2 + other->GetVelocity().x,
          other->GetPosition().y - other->GetSize().y / 2 + other->GetVelocity().y,
          other->GetSize().x, other->GetSize().y };

        if (SDL_HasIntersectionF(&debugBoxRect, &otherDebugRect)) {
            other->ProcessCollision();
            canShoot = true;
            m_bulletStats.damage += other->GetHealth();
            if (m_bulletStats.damage > m_maxDamage) m_bulletStats.damage = m_maxDamage;
            float growthRatio = 1;
            if (m_bulletStats.sizeX > 0) growthRatio = 0.5f;
            m_bulletStats.sizeX += other->GetSize().x * growthRatio;
            m_bulletStats.sizeY += other->GetSize().y * growthRatio;
            m_bulletStats.speed = std::max(m_bulletStats.speed, other->GetSpeed());
            if (m_bulletStats.sizeX > m_maxSize) m_bulletStats.sizeX = m_maxSize;
            if (m_bulletStats.sizeY > m_maxSize) m_bulletStats.sizeY = m_maxSize;

            m_bulletStats.bulletType |= ((Bullet*)(other))->m_bulletEffect;
            m_bulletStats.bulletType &= 7;
            //printf("%d \n", m_bulletStats.bulletType);
            //m_bulletStats.speed = max(m_bulletStats.speed, other->m_movementSpeed);
            //m_bulletStats.bulletSpriteDir = BulletDataManager::GetInstance()->m_bulletData[((Bullet*)(other))->m_bulletDataIndex].bulletSpriteDir;
            //m_bulletStats.frames = BulletDataManager::GetInstance()->m_bulletData[((Bullet*)(other))->m_bulletDataIndex].frames;
        }

    }
    Entity::CheckCollision(other);
}
void Player::GrabBullet(int x, int y)
{
    if (!canGrab || m_currentFrozenCooldown > 0) return;
    float disX = x - m_position.x;
    float disY = y - m_position.y;

    float norm = sqrt(disX * disX + disY * disY);
    if (norm != 0) {
        disX /= norm;
        disY /= norm;
    }
    m_grabStats.grabPosOffset =  Vector2(disX, disY) * m_grabStats.grabDistance;
    m_grabStats.grabPos = Vector2(m_position.x, m_position.y) + m_grabStats.grabPosOffset;
    isGrabbing = true;
    canGrab = false;
    m_currentGrabCooldown = m_grabCooldown;
    m_currentGrabDuration = m_grabDuration;
    m_GrabSound->PlaySoundEffect();
}

void Player::Draw(SDL_Renderer* renderer)
{
   
    Entity::Draw(renderer);
    m_barContainer->Draw(renderer);
    m_barFill->Draw(renderer);
    grabHand->Draw(renderer);
    if (isGrabbing) {
        /*SDL_Rect debugBoxRect = { m_grabStats.grabPos.x - m_grabStats.grabSize.x / 2, m_grabStats.grabPos.y - m_grabStats.grabSize.y / 2, m_grabStats.grabSize.x,  m_grabStats.grabSize.y };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(renderer, &debugBoxRect);*/
        grabAnim->Set2DPosition(m_grabStats.grabPos.x - m_grabStats.grabSize.x / 2, m_grabStats.grabPos.y - m_grabStats.grabSize.y / 2);
        grabAnim->Draw(renderer);

    }

}

void Player::ResetCombat()
{
    canGrab = true;
    canShoot = false;
    isGrabbing = false;

    //Grab cooldowns
    m_grabDuration = 0.2f;
    m_currentGrabDuration = m_grabDuration;
    m_grabCooldown = 0.5f;
    m_currentGrabCooldown = m_grabCooldown;

    //Bullet stats
    m_bulletStats.damage = 0;
    m_bulletStats.sizeX = 0;
    m_bulletStats.sizeY = 0;
    m_bulletStats.speed = 300;
    m_bulletStats.lifeTime = 6.0f;

    SetHealth(m_maxHealth * 0.2f);
    m_health = std::min(m_health, m_maxHealth);
    SetHealth(0.0f);
    m_bulletStats.bulletType = 0;

}

void Player::SetGrabStats(GrabStats grabStats)
{
    m_grabStats.grabDistance = grabStats.grabDistance;
    m_grabStats.grabSize = grabStats.grabSize;
    m_grabStats.grabPos = grabStats.grabPos;
    m_grabStats.grabPosOffset = Vector2(0, 0);
}

void Player::SetFrozen()
{
    Entity::SetFrozen();
    m_moveDirection = Vector3(0, 0, 0);
    if (isGrabbing) {
        isGrabbing = false;
        m_currentGrabDuration = m_grabDuration;
        grabAnim->ResetAnim();
        //printf("grab over\n");
    }
}

void Player::SetHealth(float healthChange)
{
    Entity::SetHealth(healthChange);
    m_Sound->PlaySoundEffect();
}



void Player::UpdateGrab(float deltaTime)
{
    if (isGrabbing) {
        grabAnim->Update(deltaTime);
        m_grabStats.grabPos = Vector2(m_position.x, m_position.y) + m_grabStats.grabPosOffset;

        m_currentGrabDuration -= deltaTime;
        if (m_currentGrabDuration <= 0) {
            isGrabbing = false;
            m_currentGrabDuration = m_grabDuration;
            grabAnim->ResetAnim();
            //printf("grab over\n");

        }
    }

    if (!canGrab) {
        m_currentGrabCooldown -= deltaTime;
        if (m_currentGrabCooldown <= 0) {
            canGrab = true;
            m_currentGrabCooldown = m_grabCooldown;
            //printf("can now grab\n");

        }
    }
}
void Player::Update(float deltaTime) {
    Entity::Update(deltaTime);
    UpdateGrab(deltaTime);
}
