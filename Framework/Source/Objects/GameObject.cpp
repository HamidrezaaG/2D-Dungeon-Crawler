#include "Framework.h"
#include "GameObject.h"
#include "../../Libraries/imgui/imgui.h"

namespace fw {

    GameObject::GameObject(float _x, float _y, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    {
        Init(Vector2(_x,_y), _pmesh, _name, _pshader, _pgame);
    }

    GameObject::GameObject(Vector2 _pos, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    {
        Init(_pos, _pmesh, _name, _pshader, _pgame);
    }

    GameObject::GameObject(fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    {
        Init(Vector2::Zero(), _pmesh, _name, _pshader, _pgame);
    }

    void GameObject::Init(Vector2 _pos, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame)
    {
        SetPosition(_pos);
        m_pShader = _pshader;
        m_pMesh = _pmesh;
        m_pGameCore = _pgame;
        m_Name = _name;
        m_pActiveCamera = _pgame->GetActiveCamera();
    }

    GameObject::~GameObject()
    {
        if (GetCollider() != nullptr)
            delete GetCollider();
    }

    void GameObject::Update(float deltaTime)
    {
    }

    fw::Collider* GameObject::GetCollider()
    {
        return nullptr;
    }

    void GameObject::OnCollisionHit(GameObject* _pOtherObject)
    {
        return;
    }

    void GameObject::Draw()
    {
        if (m_Hidden)
            return;

        float cameraZoom = 1;///(0.25f*(m_pGameCore->GetActiveCamera()->GetZoom()));

        Vector2 finv = m_Position;
            finv.x = round(finv.x * cameraZoom * fw::PositionRoundingFactor.x) / (cameraZoom * fw::PositionRoundingFactor.x);
            finv.y = round(finv.y * cameraZoom * fw::PositionRoundingFactor.y) / (cameraZoom * fw::PositionRoundingFactor.y);

        m_pMesh->Draw(finv, m_Angle, m_pActiveCamera, m_pShader, m_Scale, m_pTexture, m_Color, m_UVScale, m_UVOffset);
    }

    void GameObject::SetPosition(float _x, float _y)
    {
        m_Position.Set(_x, _y);
    }

    void GameObject::SetPosition(Vector2 _v)
    {
        m_Position = _v;
    }

    Vector2 GameObject::GetPosition()
    {
        return m_Position;
    }

    void GameObject::SetVelocity(Vector2 _v)
    {
        m_Velocity = _v;
    }

    Vector2 GameObject::GetVelocity()
    {
        return m_Velocity;
    }

    void GameObject::SetAngle(float r)
    {
        m_Angle = r;
    }
    float GameObject::GetAngle()
    {
        return m_Angle;
    }

    void GameObject::SetScale(float s)
    {
        m_Scale = Vector2(s,s);
    }

    void GameObject::SetScale(Vector2 s)
    {
        m_Scale = s;
    }

    void GameObject::SetTexture(fw::Texture* _texture)
    {
        m_pTexture = _texture;
    }

    fw::Texture* GameObject::GetTexture()
    {
        return m_pTexture;
    }

    void GameObject::SetHidden(bool _hidden)
    {
        m_Hidden = _hidden;
    }

    Vector2 GameObject::GetScale()
    {
        return m_Scale;
    }

    Color GameObject::GetColor()
    {
        return m_Color;
    }

    void GameObject::SetColor(Color color)
    {
        m_Color = color; 
    }

    float GameObject::GetColorAlpha()
    {
        return m_Color.w;
    }

    void GameObject::SetColorAlpha(float alpha)
    {
        m_Color.w = alpha;
    }

    fw::Mesh* GameObject::GetMesh()
    {
        return m_pMesh;
    }

    void GameObject::SetMesh(fw::Mesh* _mesh)
    {
        m_pMesh = _mesh;
    }

    std::string GameObject::GetName()
    {
        return m_Name;
    }

    k_ObjectType GameObject::GetType()
    {
        return k_ObjectType::Unknown;
    }

}