#pragma once

class PlayerController;
class Arena;

class Player : public fw::GameObject
{

public:
    Player(Vector2 position, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, PlayerController* _pcontroller, Arena* _pArena, fw::GameCore* _pgame);
    ~Player();

    void Update(float deltaTime) override;

    void SetCircle(float _radius, int _vertCount, bool _filled);

    void SetVertCount(int _v);

    virtual fw::k_ObjectType GetType() override;

    void RemoveAccelerationInDirection(Vector2 _direction);

    float GetRadius();

    fw::Collider* GetCollider() override;
    void Reset();
    void Reset(float percent);

    void OnCollisionHit(GameObject* _pOtherObject) override;

    int GetHealth();
    void SetHealth(int h);

    bool IsAlive();
protected:
    fw::FWCore* m_pFramework = nullptr;
    PlayerController* m_pController = nullptr;
    Vector2 m_Acceleration;

    Arena* m_pArena;

    bool m_ArenaCollision;
    Vector2 m_wallDir;

    bool m_IsFilled = true;
    float m_Radius = k_Player_DefaultRadius;
    int m_VertCount = k_Player_DefaultVertCount;

    fw::CircleCollider* m_pCollider;
    fw::Timer* m_pFlashTimer;

    Color m_ResetColorStartPoint;

    unsigned int m_Health = k_Player_Health;
};