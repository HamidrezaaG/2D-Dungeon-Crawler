#include "GamePCH.h"
#include "Movable.h"

Movable::Movable(Vector2 position, Vector2 scale, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    : GameObject(position, _pmesh, _name, _pshader, _pgame)
{
    GameObject::m_Scale = scale;

    m_pCollider = new fw::CircleCollider(this, 0.45f, m_pActiveCamera, _pgame->GetDebugShader(), Vector2(0,0), true);
    //m_pCollider = new fw::RectangleCollider(this, Vector2(0.75, 0.75), Vector2(0, 0), m_pActiveCamera, _pgame->GetDebugShader(), true);

    m_pCollider->Enable();
}

Movable::~Movable()
{
    delete m_pCollider;
}

void Movable::SetSprite(fw::SpriteSheet* _pspritesheet, fw::SpriteSheet::SpriteInfo* _pspriteInfo)
{
    m_pSpriteSheet = _pspritesheet;
    GameObject::m_UVOffset = _pspriteInfo->UV_Offset;
    GameObject::m_UVScale = _pspriteInfo->UV_Scale;
}

void Movable::Draw()
{
    GameObject::Draw();
    if (m_DrawDebugCollider)
        m_pCollider->Draw();
}

fw::k_ObjectType Movable::GetType()
{
    return fw::k_ObjectType::Dynamic;
}

fw::Collider* Movable::GetCollider()
{
    return m_pCollider;
}
