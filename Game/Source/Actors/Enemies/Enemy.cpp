#include "GamePCH.h"
#include "Enemy.h"
#include "../Arena.h"
#include "../Player.h"

Enemy::Enemy(Player* _pplayer, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Arena* _pArena,  fw::GameCore* _pgame) : GameObject(_pmesh, _name, _pshader, _pgame)
{
    m_pArena = _pArena;
    m_pPlayer = _pplayer;

    float RandomAngle = (float)(rand() % 360) * fw::k_Convert_Deg2Rad;

    float percent = (float)(rand() % 100) * 0.01f;

    m_RotSpeed = k_Enemy_MinRotSpeedDeg + (k_Enemy_MaxRotSpeedDeg - k_Enemy_MinRotSpeedDeg) * percent;
    m_RotSpeed *= rand() % 2 ? 1 : -1;

    SetPosition(m_pArena->GetPosition() + Vector2((float)cos(RandomAngle), (float)sin(RandomAngle)) * m_pArena->GetRadius());

    m_Speed = k_Enemy_Speed;
    m_Direction = (m_pPlayer->GetPosition() - m_Position).Normalized();
    m_Velocity = m_Direction * m_Speed;
    m_Angle = k_Enemy_ShapeStartingAngle;

    m_pCollider = new fw::CircleCollider(this, k_Enemy_Radius);
    m_pCollider->Enable();
}

Enemy::~Enemy()
{
    delete m_pCollider;
}

void Enemy::Update(float deltaTime)
{
    m_Angle += m_RotSpeed * deltaTime;

    //GetMesh()->MakeCircle(k_Enemy_Radius, k_Enemy_VertCount, m_Angle, k_Enemy_IsFilled);

    if (m_IsDead)
    {
        m_Velocity += Vector2::Down() * k_World_Gravity * deltaTime;
        m_RotSpeed *= 0.99f;
    }
    
    m_Position += m_Velocity * deltaTime;
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
    if (_pOtherObject->GetType() == fw::k_ObjectType::Player) 
    {
        Die();
        Vector2 dir = (m_Position - _pOtherObject->GetPosition()).Normalized();
        m_Velocity.RemoveDirection(dir);
        m_Velocity += dir * k_Player_HurtBumpPower;
        //(static_cast<Player*>(_pOtherObject))->OnCollisionHit(this);
    }
}

void Enemy::Die()
{
    SetColor(Color::Black());
    m_IsDead = true;
}

fw::Collider* Enemy::GetCollider()
{
    return m_pCollider;
}


