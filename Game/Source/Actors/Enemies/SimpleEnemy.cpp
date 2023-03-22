#include "GamePCH.h"
#include "Enemy.h"
#include "SimpleEnemy.h"
#include "../Arena.h"
#include "../Player.h"

SimpleEnemy::SimpleEnemy(Player* _pplayer, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Arena* _pArena,  fw::GameCore* _pgame) : 
    Enemy(_pplayer, _pmesh, _name, _pshader, _pArena, _pgame)
{
}

SimpleEnemy::~SimpleEnemy()
{
}

void SimpleEnemy::Update(float deltaTime)
{
    m_Angle += m_RotSpeed * deltaTime;

    if (m_IsDead)
    {
        m_Velocity += Vector2::Down() * 20 * deltaTime;
        m_RotSpeed *= 0.99f;
    }
    
    m_Position += m_Velocity * deltaTime;
}


