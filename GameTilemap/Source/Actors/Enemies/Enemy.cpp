#include "GamePCH.h"
#include "Enemy.h"
#include "Events/GameEvents.h"

Enemy::Enemy(fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Player* _pplayer, fw::GameCore* _pgame) : GameObject(_pmesh, _name, _pshader, _pgame),
m_pPlayer(_pplayer)
{

    //m_pCollider = new fw::CircleCollider(this, 0.2f);
    //m_pCollider->Enable();
    m_pCollider = new fw::CircleCollider(this, 0.25, m_pActiveCamera, _pgame->GetDebugShader(), Vector2::Zero(), true);
    //m_pCollider = new fw::RectangleCollider(this, Vector2(1, 2), Vector2(0, 0), m_pActiveCamera, _pgame->GetDebugShader(), true);
    m_pCollider->Enable();
}

Enemy::~Enemy()
{
    //if(m_pCollider != nullptr)
    //delete m_pCollider;
}

void Enemy::Update(float deltaTime)
{

}

bool Enemy::IsDead()
{
    return m_IsDead;
}

fw::k_ObjectType Enemy::GetType()
{
    return fw::k_ObjectType::Enemy;
}

void Enemy::OnCollisionHit(GameObject* _pOtherObject)
{

}

void Enemy::Die()
{
    m_IsDead = true;
    m_pGameCore->GetEventManager()->AddEvent(new RemoveFromGameEvent(this));
}

fw::Collider* Enemy::GetCollider()
{
    return m_pCollider;
}

void Enemy::Draw()
{
    GameObject::Draw();

    if (m_DrawDebugCollider)
        m_pCollider->Draw();
}

void Enemy::TakeDamage(fw::GameObject* instigator, int damage)
{
    m_Velocity += (GetPosition() - instigator->GetCollider()->GetRelativePosition()).Normalized() * k_Shade_Kickback_Intensity;
    TakeDamage(damage);
}

void Enemy::TakeDamage(int damage)
{
    if (damage >= m_Health)
    {
        Die();
        return;
    }
    m_Health -= damage;
}

int Enemy::GetHealth()
{
    return m_Health;
}

void Enemy::SetHealth(int health)
{
    m_Health = health;
}
