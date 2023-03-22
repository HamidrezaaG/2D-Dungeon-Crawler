#pragma once

namespace fw {

class CircleCollider : public Collider
{
public:

    CircleCollider(fw::GameObject* _pparent, float _radius, Vector2 _relativePos, bool _resolveCollisions);
    CircleCollider(fw::GameObject* _pparent, float _radius, Camera* _pcamera, ShaderProgram* _pdebugshader, Vector2 _relativePos, bool _resolveCollisions);

    CircleCollider();
    ~CircleCollider();

    float GetRadius();
    void SetRadius(float _r);

    k_ColliderType GetType() override;
    bool CheckCollision(Collider* _pOtherCollider) override;

protected:
    float m_Radius;
};

}