#pragma once

namespace fw {

    enum class k_ColliderType { Circle, Rectangle };

class Collider
{
public:
    Collider();
    virtual ~Collider();
    Collider(GameObject* _parent);
    Collider(GameObject* _parent, Camera* _pcamera, ShaderProgram* _pdebugshader);

    virtual k_ColliderType GetType() = 0;
    virtual bool CheckCollision(Collider* _otherCollider) = 0;
    void SetupDebugInfo(Camera* _pcamera, ShaderProgram* _pdebugshader);

    void SetRelativePosition(Vector2 _relativePosition);

    Vector2 GetRelativePosition();

    bool GetResolveCollisions();
    void SetResolveCollisions(bool b);

    GameObject* GetParent();
    void SetParent(fw::GameObject* _p);
    void Draw();

    void Enable();
    void Disable();
    bool IsEnabled();

protected:
    Vector2 m_RelativePosition = Vector2::Zero();
    bool m_Enabled = true;
    bool m_ResolveCollisions = false;
    
    Mesh* m_pDebugMesh = nullptr;
    GameObject* m_pParent = nullptr;
    ShaderProgram* m_DebugShader = nullptr;
    Camera* m_pCamera = nullptr;
    bool m_Debuggable = false;
};

}