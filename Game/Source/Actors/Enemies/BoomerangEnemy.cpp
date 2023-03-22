#include "GamePCH.h"
#include "Enemy.h"
#include "BoomerangEnemy.h"
#include "../Arena.h"
#include "../Player.h"

BoomerangEnemy::BoomerangEnemy(Player* _pplayer, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Arena* _pArena,  fw::GameCore* _pgame) :
    Enemy(_pplayer, _pmesh, _name, _pshader, _pArena, _pgame)
{
   m_Velocity -= m_Velocity.Ortho() * 0.3f;
}

BoomerangEnemy::~BoomerangEnemy()
{
}

void BoomerangEnemy::Update(float deltaTime)
{
    m_Angle += m_RotSpeed * deltaTime;

    m_Velocity -= m_Direction * 4 * deltaTime;
    m_Velocity += m_Velocity.Ortho() * 0.1f * deltaTime;

    if (m_IsDead)
    {
        m_Velocity += Vector2::Down() * 20 * deltaTime;
        m_RotSpeed *= 0.99f;
    }
    
    m_Position += m_Velocity * deltaTime;
}


