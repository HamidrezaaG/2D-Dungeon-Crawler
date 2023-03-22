#include "GamePCH.h"
#include "TestEnemy.h"

TestEnemy::TestEnemy(Vector2 position, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Player* _pplayer, fw::GameCore* _pgame) : Enemy(_pmesh, _name, _pshader, _pplayer, _pgame)
{
    m_Position = position;
    m_pCollider = new fw::RectangleCollider(this,Vector2(2,2),Vector2(0,0),false);
}

TestEnemy::~TestEnemy()
{
    delete m_pCollider;
}