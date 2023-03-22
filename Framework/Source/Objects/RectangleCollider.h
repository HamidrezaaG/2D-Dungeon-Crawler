#pragma once

namespace fw {

class RectangleCollider : public Collider
{
public:

    RectangleCollider(GameObject* _pparent, Vector2 _extents, Vector2 _relativePos, bool _resolveCollisions);
    RectangleCollider(fw::GameObject* _pparent, Vector2 _extents, Vector2 _relativePos, Camera* _pcamera, ShaderProgram* _pdebugshader, bool _resolveCollisions);
    RectangleCollider();
    ~RectangleCollider();

    k_ColliderType GetType() override;
    bool CheckCollision(Collider* _pOtherCollider) override;
    void SetExtents(Vector2 _e);
    Vector2 RectangleCollider::GetExtents();


    Vector2 m_Extents = Vector2::Zero();
    float m_rectOuterCircleRadius = 0;
    float m_rectInnerCircleRadius = 0;
    //float m_rectDiagonalAngle;
};

}