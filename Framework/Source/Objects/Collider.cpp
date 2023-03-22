#include "Framework.h"
#include "Collider.h"
//#include "FWConsts.h"

namespace fw {

    Collider::Collider(GameObject* _parent) : m_pParent(_parent) {};
    Collider::Collider(GameObject* _parent, Camera* _pcamera, ShaderProgram* _pdebugshader) : m_pCamera(_pcamera), m_pParent(_parent), m_DebugShader(_pdebugshader)
    {
        if (_pcamera !=nullptr && _parent != nullptr && _pdebugshader != nullptr)
        {
            m_Debuggable = true;
        }
    }
    Collider::Collider() {}
    Collider::~Collider() {}

    void Collider::SetRelativePosition(Vector2 _relativePosition)
    {
        m_RelativePosition = _relativePosition;
    }

    Vector2 Collider::GetRelativePosition()
    {
        if(m_pParent == nullptr)
        return m_RelativePosition;

        return m_pParent->GetPosition() + m_RelativePosition;
    }

    bool Collider::GetResolveCollisions()
    {
        return m_ResolveCollisions;
    }

    void Collider::SetResolveCollisions(bool b)
    {
        m_ResolveCollisions = b;
    }

    fw::GameObject* Collider::GetParent()
    {
        return m_pParent;
    }

    void Collider::SetParent(fw::GameObject* _p)
    {
        m_pParent = _p;
    }

    void Collider::SetupDebugInfo(Camera* _pcamera, ShaderProgram* _pdebugshader)
    {
        m_DebugShader = _pdebugshader;
        m_pCamera = _pcamera;
        if (_pcamera != nullptr && _pdebugshader != nullptr)
        {
            m_Debuggable = true;
        }
    }

    void Collider::Draw()
    {
        if(m_Debuggable)
        m_pDebugMesh->Draw(GetRelativePosition(), 0, m_pCamera, m_DebugShader, Vector2(1,1), k_Debug_Collider_Color);
    }

    void Collider::Enable()
    {
        m_Enabled = true;
    }

    void Collider::Disable()
    {
        m_Enabled = false;
    }

    bool Collider::IsEnabled()
    {
        return m_Enabled;
    }
}