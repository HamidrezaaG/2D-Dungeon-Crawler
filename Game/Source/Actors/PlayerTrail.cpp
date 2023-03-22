#include "GamePCH.h"
#include "PlayerTrail.h"

PlayerTrail::PlayerTrail(GameObject* _targetObj, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    : GameObject(_targetObj->GetPosition(), _pmesh, _name, _pshader, _pgame)
{
    m_TargetObject = _targetObj;
}

void PlayerTrail::init(float _lerpIntensity, float _radius)
{
    m_Radius = _radius;
    m_pMesh->MakeCircle(m_Radius, m_VertCount, m_IsFilled);
    m_LerpIntensity = _lerpIntensity;
    m_alpha = m_Color.w;
}


PlayerTrail::~PlayerTrail()
{
}

void PlayerTrail::Update(float deltaTime)
{
    m_Position = m_Position + (m_TargetObject->GetPosition() - m_Position) * deltaTime * m_LerpIntensity;
    m_Angle = m_Angle + (m_TargetObject->GetAngle() - m_Angle) * deltaTime * m_LerpIntensity;
    m_Color.SetWithoutAlpha(m_TargetObject->GetColor());
    m_Color.SetAlpha(m_alpha * m_TargetObject->GetColor().w * m_TargetObject->GetColor().w);
}


fw::k_ObjectType PlayerTrail::GetType()
{
    return fw::k_ObjectType::Unknown;
}

void PlayerTrail::SetCircle(float _radius, int _vertCount, bool _filled)
{
    m_Radius = _radius;
    m_VertCount = _vertCount;
    m_IsFilled = _filled;
    m_pMesh->MakeCircle(m_Radius, m_VertCount, m_IsFilled);
}

void PlayerTrail::SetVertCount(int _vertCount)
{
    m_VertCount = _vertCount;
    m_pMesh->MakeCircle(m_Radius, m_VertCount, m_IsFilled);
}