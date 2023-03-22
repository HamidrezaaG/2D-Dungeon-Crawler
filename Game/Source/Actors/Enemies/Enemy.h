#pragma once

class Arena;
class Player;

      enum class k_EnemyTypes { Simple, Boomerang };

class Enemy : public fw::GameObject
{

public:

         Enemy(Player* _pplayer, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Arena* _pArena, fw::GameCore* _pgame);
        ~Enemy();

    virtual void Update(float deltaTime);
    virtual void OnCollisionHit(GameObject* _pOtherObject);

    virtual void Die();

    bool IsDead();

    virtual fw::k_ObjectType GetType() override;
    virtual fw::Collider* GetCollider() override;

protected:
    fw::FWCore* m_pFramework = nullptr;
    Arena* m_pArena = nullptr;
    float m_Speed = .0f;
    Vector2 m_Direction = Vector2(.0f, .0f);;
    float m_RotSpeed = .0f;
    Player* m_pPlayer = nullptr;
    bool m_IsDead = false;
    fw::CircleCollider* m_pCollider = nullptr;
    fw::Timer m_rotationSlowdownTimer;
};