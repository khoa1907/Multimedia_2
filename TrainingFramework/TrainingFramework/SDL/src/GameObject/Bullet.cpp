#include<Bullet.h>
#include <EnemyManager.h>
void Bullet::DestroyBullet() {
	m_isDead = true;
	SetActive(false);
	if (((m_bulletEffect >> 3) & 1)) {
		CloneBullets();
	}
}
void Bullet :: Update(float deltaTime) {
	if (!m_isActive) return;
	Entity::Update(deltaTime);
	m_lifetime -= deltaTime;
	if (m_lifetime < 0) DestroyBullet();
}
void Bullet::SetFirstSpawn(float lifetime, float damage, float movementSpeed, Vector3 startPos, Vector3 dir) {
	m_damage = damage;
	m_lifetime = lifetime;
	m_movementSpeed = movementSpeed;
	m_moveDirection = dir;
	m_position = startPos;
	m_health = m_damage;


	float norm = sqrt(m_moveDirection.y * m_moveDirection.y + m_moveDirection.x * m_moveDirection.x);
	if (norm != 0) {
		m_moveDirection.y /= norm;
		m_moveDirection.x /= norm;
	}
}

void Bullet::Init() {
	m_entityType = 1;
	m_isDead = false;
	m_isActive = false;
	m_health = m_damage;

	////////////////////////////////
	printf("Init'ed");
	/*m_width = m_height = 16;*/
	/*auto texture = ResourceManagers::GetInstance()->GetTexture("MidRedPR2.png");
	CreateAnim(it, texture, 1);*/
	//it->SetTexture(ResourceManagers::GetInstance()->GetTexture("Grandpa-walk.png"));

}


void Bullet::ProcessCollision(Entity* other, SDL_FRect* intersect)
{
	ProcessCollision(other);
}

void Bullet::ProcessCollision(SDL_FRect* intersect)
{
	if (!((m_bulletEffect >> 2) & 1)) {
		SetActive(false);
		if (((m_bulletEffect >> 3) & 1)) {
			CloneBullets();
		}
		return;

	}
	Entity::ProcessCollision(intersect);
	relativePushX = 0;
	relativePushY = 0;
	if (m_touchXWall) {
		m_moveVelocity.y *= -1;
		m_moveDirection.y *= -1;

	}
	else {
		m_moveVelocity.x *= -1;
		m_moveDirection.x *= -1;
	}


}

void Bullet::ProcessCollision(Entity* other)
{
	//printf("%f vs %f \n", m_health, other->GetHealth());
	SetHealth(std::min(-other->GetHealth(), 0.0f));
	other->SetHealth(-m_damage);
	other->SetStatusEffect(m_bulletEffect);
	
	
	//if (m_health <= 0) SetActive(false);
}

void Bullet::ProcessCollision()
{
	SetActive(false);
	if (((m_bulletEffect >> 3) & 1)) {
		CloneBullets();
	}
}

//void Bullet::PostSolveVelocity()
//{
//}

void Bullet::SetBulletData(int id)
{
	BulletData bdt = BulletDataManager::GetInstance()->m_bulletData[id];
	SetBulletData(bdt);
}

void Bullet::SetBulletData(BulletData bulletData)
{
	m_damage = bulletData.damage;
	m_lifetime = bulletData.lifeTime;
	m_movementSpeed = bulletData.speed;
	m_health = m_damage;
	m_bulletEffect = bulletData.bulletType;
	SetSize(bulletData.sizeX, bulletData.sizeY);
	CreateAnim(it, bulletData.bulletSpriteDir, 1);


	float norm = sqrt(m_moveDirection.y * m_moveDirection.y + m_moveDirection.x * m_moveDirection.x);
	if (norm != 0) {
		m_moveDirection.y /= norm;
		m_moveDirection.x /= norm;
	}
}

void Bullet::SetStatusEffect(int statusType)
{
}

void Bullet::CloneBullets()
{
	printf("Cloned %d\n", m_bulletEffect);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(1, 1, 0), 9, layerID);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(1, -1, 0), 9, layerID);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(-1, 1, 0), 9, layerID);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(-1, -1, 0), 9, layerID);

	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(0, -1, 0), 10, layerID);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(-1, 0, 0), 11, layerID);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(1, 0, 0), 11, layerID);
	EnemyManager::GetInstance()->m_currentGame->AddBullet(m_position, Vector3(0, 1, 0), 10, layerID);
}

void Bullet::SetHealth(float healthChange)
{
	Entity::SetHealth(healthChange);
	if (IsDead() && ((m_bulletEffect >> 3) & 1)) {
		printf("EXPLODE!\n");
		CloneBullets();
	}
}


