#include "GamePCH.h"
#include "Arena.h"
#include "Game.h"

Arena::Arena(Vector2 position, fw::Mesh* _pmesh, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    : GameObject(position, _pmesh, "Arena", _pshader, _pgame)
{

}

Arena::~Arena()
{
}

void Arena::Update(float deltaTime)
{
    return;
}

void Arena::SetRadius(float _radius)
{
    m_Radius = _radius;
    GetMesh()->MakeCircle(m_Radius, k_Arena_VertCount, false);
}

float Arena::GetRadius()
{
    return m_Radius;
}

void Arena::SetCalculatedVelocity(Vector2 _v)
{
    m_CalculatedVelocity = _v;
}

Vector2 Arena::GetCalculatedVelocity()
{
    return m_CalculatedVelocity;
}


fw::k_ObjectType Arena::GetType()
{
    return fw::k_ObjectType::Arena;
}
